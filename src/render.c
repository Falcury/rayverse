
void render_weird_gradient(surface_t* surface, int x_offset, int y_offset) {
	u8* row = surface->memory;
	for (int y = 0; y < surface->height; ++y) {
		u32* pixel = (u32*) row;
		for (int x = 0; x < surface->width; ++x) {
			// little endian: BB GG RR xx
			u8 blue = (u8) (y + y_offset);
			u8 green = (u8) (-x / 2 + x_offset);
			u8 red = (u8) (x + x_offset);
			u8 alpha = 255;
			*pixel = (blue << 0 | (green ^ blue) << 8 | red << 16 | alpha << 24);
			pixel += 1;
		}
		row += surface->pitch;
	}
}

void render_clear(surface_t* surface, rgb_t color) {
	// TODO: make pixel format agnostic?
	u32 pixel = (color.b << 0 | color.g << 8 | color.r << 16 | 0xFF << 24);
	s32 pixel_count = surface->memory_size / surface->bytes_per_pixel;
	for (s32 i = 0; i < pixel_count; ++i) {
		((u32*)surface->memory)[i] = pixel;
	}
}

void surface_blit_palettized_image(image_t* source, rgb_palette_t* palette, rect_t* source_rect, surface_t* target,
                                   rect_t* target_rect) {
	if (!source || !target) {
		// error
		return;
	}
	if (!source_rect) {
		source_rect = (rect_t*) alloca(sizeof(rect_t));
		source_rect->x = 0;
		source_rect->y = 0;
		source_rect->w = source->width;
		source_rect->h = source->height;
	}
	if (!target_rect) {
		target_rect = (rect_t*) alloca(sizeof(rect_t));
		target_rect->x = 0;
		target_rect->y = 0;
		target_rect->w = source->width;
		target_rect->h = source->height;
	}

	// TODO: clip rects

	s32 source_max_y = source_rect->y + source_rect->h;
	s32 source_max_x = source_rect->x + source_rect->w;
	u8* source_row = source->memory + source_rect->y * source->width + source_rect->x;
	u8* target_row = target->memory + target_rect->y * target->pitch + target_rect->x * target->bytes_per_pixel;
	for (s32 source_y = source_rect->y; source_y < source_max_y; ++source_y) {
		
		u8* source_pixel = source_row;
		u32* target_pixel = (u32*) target_row;
		for (s32 source_x = source_rect->x; source_x < source_max_x; ++source_x) {

			rgb_t color = palette->colors[*source_pixel];
			*target_pixel = (color.b << 0 | color.g << 8 | color.r << 16 | 0xFF << 24);
			
			source_pixel += 1;
			target_pixel += 1; // TODO: scaling
		}

		source_row += source->width;
		target_row += target->pitch;
	}

}
