// 3CA64
void sub_3CA64() {

/*
	0003CA64                 push    edx
	0003CA65                 xor     edx, edx
	0003CA67                 mov     dl, ds:current_pal_id
	0003CA6D                 lea     eax, ds:0[edx*4]
	0003CA74                 sub     eax, edx
	0003CA76                 shl     eax, 8
	0003CA79                 mov     edx, offset current_rvb
	0003CA7E                 add     eax, offset rvb
	0003CA83                 call    do_fade_step?
	                                 0003CA88                 pop     edx
	0003CA89                 retn
*/

}

//3CA8C
void init_fade_in() {
	// stub

/*	0003CA8C init_fade_in    proc near               ; CODE XREF: do_perfect_bonus+1Dp
	0003CA8C                                         ; game_over+49p ...
	0003CA8C                 push    edx
	0003CA8D                 xor     edx, edx
	0003CA8F                 mov     dl, ds:current_pal_id
	0003CA95                 lea     eax, ds:0[edx*4]
	0003CA9C                 sub     eax, edx
	0003CA9E                 mov     edx, offset rvb
	0003CAA3                 shl     eax, 8
	0003CAA6                 add     edx, eax
	0003CAA8                 mov     eax, 2
	0003CAAD                 call    start_fade_in
	0003CAB2                 pop     edx
	0003CAB3                 retn
	0003CAB3 init_fade_in    endp*/

}

//3C54C
void start_fade_in(u32 par_0) {
	// apply palette par_0?
	// It seems fading is done through shifting the palette?
}



image_t load_vignet_pcx(u32 resource_id) {
	ASSERT(sizeof(pcx_header_t) == 128);
	FILE* fp = fopen("data\\VIGNET.DAT", "rb");
	image_t image = {};
	if (fp) {
		vignetinfo_t* info = vignetinfos + resource_id;
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

		if (header->bits_per_bitplane != 8 || header->n_color_planes != 1 || header->palette_mode != 1) {
			// unsupported file structure
			fatal_error();
		} else {
			image.memory = (u8*)malloc(width * height);
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
	FILE* fp = fopen("data\\VIGNET.DAT", "rb");
	if (fp) {
		vignetinfo_t* info = vignetinfos + resource_id;
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

//71A70
image_t load_ubisoft_logo() {
	return load_vignet_pcx(29);
}
