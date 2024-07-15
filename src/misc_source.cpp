

FILE* open_data_file(const char* filename) {
	FILE* fp = fopen(filename, "rb");
	if (!fp) {
		char data_path[256];
		snprintf(data_path, 256, "data\\%s", filename);
		fp = fopen(data_path, "rb");
	}
	return fp;
}

image_t load_vignet_pcx(u32 resource_id) {
	ASSERT(sizeof(pcx_header_t) == 128);
	FILE* fp = open_data_file("VIGNET.DAT");
	image_t image = {};
	if (fp) {
		archive_header_t* info = vignetinfos + resource_id;
		fseek(fp, info->offset, 0);

		u8* pcx_buf = (u8*) malloc(info->size);
		fread(pcx_buf, info->size, 1, fp);
		u8* p = (u8*)pcx_buf;
		u8 checksum = info->checksum_byte;
		for (i32 i = 0; i < info->size; ++i) {
			u8 b = *p;
			checksum -= b;
			b ^= info->xor_byte;
			*p++ = b;
		}
#if 0
		FILE* test_out = fopen("test_out.pcx", "wb");
		fwrite(pcx_buf, info->size, 1, test_out);
		fclose(test_out);

#endif
		if (checksum != 0) {
			fatal_error();
		}

		pcx_header_t* header = (pcx_header_t*)pcx_buf;
		i32 width = header->max_x - header->min_x + 1;
		i32 height = header->max_y - header->min_y + 1;
		image.width = width;
		image.height = height;

		if (header->bits_per_bitplane != 8 || header->n_color_planes != 1 /*|| header->palette_mode != 1*/) {
			// unsupported file structure
			fatal_error();
		} else {
			image.memory = (u8*)malloc(width * height);
			image.memory_size = width * height;
			// run-length decoding
			u8* pos = pcx_buf + 128;
			u8* end = pcx_buf + info->size;
			i32 i = 0;
			i32 decompressed_byte_count = width * height;
			while (i < decompressed_byte_count && pos < end) {
				u8 b = *pos;
				if (b >= 0xC0) {
					// upper two bits are set -> remaining bits interpreted as run length (0-63)
					i32 run_length = b & 0x3F;
					++pos;
					if (i + run_length <= decompressed_byte_count && pos < end) {
						b = *pos;
						memset(image.memory + i, b, run_length);
					} else {
						fatal_error(); // RLE encoding exceeding the end of one of the buffers
					}
					++pos;
					i += run_length;
				} else {
					// byte interpreted as single pixel value
					image.memory[i] = b;
					++pos;
					++i;
				}
			}
			if (i != decompressed_byte_count) {
				fatal_error();
			}
			// Save the image palette at the end of the file
			i32 pal_offset = info->size - 768;
			image.pal = (rgb_palette_t*)malloc(sizeof(rgb_palette_t));
			image.pal_needs_free = true;
			memcpy(image.pal, pcx_buf + pal_offset, sizeof(rgb_palette_t));
			image.is_valid = true;
		}

		free(pcx_buf);


	} else {
		fatal_error();
	}
	return image;
}





void load_vignet_palette(u32 resource_id, rgb_palette_t* palette) {
	FILE* fp = open_data_file("VIGNET.DAT");
	if (fp) {
		archive_header_t* info = vignetinfos + resource_id;
		i32 read_offset = info->offset + info->size - 0x300;
		fseek(fp, read_offset, 0);
		fread(palette, 0x300, 1, fp);
		fclose(fp);

		// decrypt
		u8* palette_byte = (u8*)palette;
		for (i32 i = 0; i < 0x300; ++i) {
			u8 xor_byte =  info->xor_byte;
			u8 b = *palette_byte ^ xor_byte;
			b >>= 2; // ?
			*palette_byte = b;
		}
	} else {
		fatal_error(); // cannot open palette
	}
}

void load_plan2_in_vignet(void* buffer, u32 resource_id)  {
	//stop_cd();
	image_t image = load_vignet_pcx(resource_id);
}

sprite_group_t global_sprite_groups[16];
u16 num_sprite_groups;
eta_t** global_eta_block[16];

//46078
void load_big_ray() {
	FILE* fp = open_data_file("PCMAP\\BRAY.DAT");
	if (fp) {
		sprite_group_t* sprite_group = global_sprite_groups + num_sprite_groups;
		u32 sprite_group_size = 0;
		fread(&sprite_group_size, 4, 1, fp);
		sprite_group->size_in_bytes = sprite_group_size;
		u8* DES8_buffer = (u8*)malloc(sprite_group_size);
		fread(DES8_buffer, sprite_group_size, 1, fp);
		for (u32 i = 0; i < sprite_group_size; ++i) {
			DES8_buffer[i] ^= 0x8F;
		}
		sprite_group->DES8_block = DES8_buffer;

		fread(&sprite_group->sprite_desc_count, 2, 1, fp);
		ASSERT(sizeof(sprite_desc_t) == 12);
		u8* DES_buffer = (u8*)malloc(sprite_group->sprite_desc_count * sizeof(sprite_desc_t));
		fread(DES_buffer, sizeof(sprite_desc_t), sprite_group->sprite_desc_count, fp);
		sprite_group->DES0 = (sprite_desc_t*)DES_buffer;

		fread(&sprite_group->anim_desc_count, 1, 1, fp);
		sprite_group->DES4 = (anim_desc_t*)calloc(sprite_group->anim_desc_count, sizeof(anim_desc_t));
		for (i32 i = 0; i < sprite_group->anim_desc_count; ++i) {
			anim_desc_t* anim_desc = sprite_group->DES4 + i;
			fread(&anim_desc->layers_per_frame, 2, 1, fp);
			fread(&anim_desc->frame_count, 2, 1, fp);
			i32 field_4 = 0;
			fread(&field_4, 4, 1, fp); // ptr field, but might be -1
//			anim_desc->anim_layers_or_frames = (u8*)field_4;
			u16 layer_table_size = 0;
			fread(&layer_table_size, 2, 1, fp);
			anim_desc->layers = (u8*)malloc(layer_table_size);
			fread(anim_desc->layers, 1, layer_table_size, fp);
			if (field_4 != -1) {
				anim_desc->anim_layers_or_frames = (u8*)malloc(anim_desc->frame_count * 4);
				fread(anim_desc->anim_layers_or_frames, 4, anim_desc->frame_count, fp);
			}
		}
		fread(&num_eta_blocks, 1, 1, fp);
		ASSERT(num_eta_blocks < COUNT(global_eta_block));
		ASSERT(sizeof(eta_t) == 8);
		for (i32 i = 0; i < num_eta_blocks; ++i) {
			u8 num_eta_groups = 0;
			fread(&num_eta_groups, 1, 1, fp);
			global_eta_block[i] = (eta_t**)calloc(num_eta_groups, sizeof(eta_t*));
			for (i32 j = 0; j < num_eta_groups; ++j) {
				u8 num_eta_entries = 0;
				fread(&num_eta_entries, 1, 1, fp);
				global_eta_block[i][j] = (eta_t*)calloc(num_eta_entries, sizeof(eta_t));
				fread(global_eta_block[i][j], sizeof(eta_t), num_eta_entries, fp);
			}

		}

		// TODO: copy sprite_group and eta_block to a separate copy (ray_sprite_group?)
		++num_sprite_groups;

		fclose(fp);

	}
	//sub_7FA70(filename);
	//sub_6BAE0();
}


