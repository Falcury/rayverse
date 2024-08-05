

FILE* open_data_file(const char* filename) {
	FILE* fp = fopen(filename, "rb");
	if (!fp) {
		char data_path[256];
		snprintf(data_path, 256, "data\\%s", filename);
		fp = fopen(data_path, "rb");
	}
	if (!fp) {
		char message[256];
		snprintf(message, 256, "Error: couldn't locate %s\n", filename);
		fprintf(stdout, "%s", message);
		message_box(message);
	}
	return fp;
}

mem_t* read_entire_file(const char* filename, bool error_is_fatal) {
	mem_t* result = NULL;
	FILE* fp = fopen(filename, "rb");
	if (fp) {
		struct stat st;
		if (fstat(fileno(fp), &st) == 0) {
			int filesize = st.st_size;
			if (filesize > 0) {
				size_t allocation_size = sizeof(mem_t) + filesize;
				result = (mem_t*) malloc(allocation_size);
				if (result) {
					((u8*)result)[allocation_size - 1] = '\0';
					result->len = filesize;
					result->capacity = filesize;
					result->cursor = 0;
					size_t bytes_read = fread(result->data, 1, filesize, fp);
					if (bytes_read != (size_t)filesize) {
						printf("Error while reading file '%s'\n", filename);
						exit(1);
					}
				}
			}
		}
		fclose(fp);
	} else if (error_is_fatal) {
		char message[256];
		snprintf(message, 256, "Error: couldn't locate %s\n", filename);
		fprintf(stdout, "%s", message);
		message_box(message);
		fatal_error();
	}
	return result;
}

i32 mem_write(void* src, mem_t* mem, size_t bytes_to_write) {
	i32 bytes_left = mem->capacity - mem->cursor;
	if (bytes_left >= 1) {
		bytes_to_write = MIN(bytes_to_write, (size_t)bytes_left);
		memcpy(mem->data + mem->cursor, src, bytes_to_write);
		mem->cursor += bytes_to_write;
		mem->len = MAX(mem->cursor, (i32)mem->len);
		return bytes_to_write;
	}
	return 0;
}

i32 mem_read(void* dest, mem_t* mem, size_t bytes_to_read) {
	i32 bytes_left = mem->len - mem->cursor;
	if (bytes_left >= 1) {
		bytes_to_read = MIN(bytes_to_read, (size_t)bytes_left);
		memcpy(dest, mem->data + mem->cursor, bytes_to_read);
		mem->cursor += bytes_to_read;
		return bytes_to_read;
	}
	return 0;
}

i32 mem_read_byte(mem_t* mem) {
	i32 byte = 0;
	i32 bytes_read = mem_read(&byte, mem, 1);
	if (bytes_read <= 0) byte = -1;
	return byte;
}

void mem_seek(mem_t* mem, i32 offset) {
	if (offset >= 0 && (u32)offset < mem->len) {
		mem->cursor = offset;
	} else {
		fatal_error();
	};
}
