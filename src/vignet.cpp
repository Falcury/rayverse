
image_t load_vignet_pcx(u32 resource_id) {
	ASSERT(sizeof(pcx_header_t) == 128);
	FILE* fp = open_data_file("VIGNET.DAT", true);
	image_t image = {0};
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
	FILE* fp = open_data_file("VIGNET.DAT", true);
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
	// stub
	//stop_cd();
	image_t image = load_vignet_pcx(resource_id);
}
