
//3E66C
void sub_3E66C(void) {
    //stub
}

//3E6B0
void sub_3E6B0(void) {
    //stub
}

//3E6EC
void rayman_sound_fatal_error(const char* message) {
    printf("Rayman sound says fatal error :\n%s\n", message);
    exit(-1);
}

//3E710
void sub_3E710(void) {
    //stub
}

//3E780
void sub_3E780(void) {
    //stub
}

//3E820
void LoadBnkFile(i32 sound_set, i32 a2, u8* headers, u8* data) {
    // Load sound header
    mem_t* mem = read_entire_file("SNDH8B.DAT", true);
    if (!mem) fatal_error();

    archive_header_t* header_info = &snd8b_header_infos[sound_set];

    mem_seek(mem, header_info->offset);
    mem_read(bnkHeaderFixe, mem, header_info->size); // always 0x800 bytes

    u8 checksum = decode_xor(bnkHeaderFixe, header_info->size, header_info->xor_byte, header_info->checksum_byte);
    if (checksum != 0) {
        printf("[warning] LoadBnkFile(): incorrect checksum for SNDH8B.DAT\n");
    }

    free(mem);

    // stub


}

void LoadBnkFile_debug(i32 sound_set, i32 a2, u8** sound_buffer) {
    // Load sound header
    mem_t* mem = read_entire_file("SNDH8B.DAT", true);
    if (!mem) fatal_error();

    archive_header_t* header_info = &snd8b_header_infos[sound_set];

    mem_seek(mem, header_info->offset);
    mem_read(bnkHeaderFixe, mem, header_info->size); // always 0x800 bytes

    u8 checksum = decode_xor(bnkHeaderFixe, header_info->size, header_info->xor_byte, header_info->checksum_byte);
    if (checksum != 0) {
        printf("[warning] LoadBnkFile(): incorrect checksum for SNDH8B.DAT\n");
    }

    free(mem);

    // Load sound data
    archive_header_t* data_info = &snd8b_data_infos[sound_set];
    size_t sound_size = data_info->size;
    void* temp = realloc(*sound_buffer, sound_size);
    if (!temp) fatal_error();
    *sound_buffer = (u8*)temp;

    FILE* fp = open_data_file("SNDD8B.DAT", true);
    if (!fp) fatal_error();

    fseek(fp, data_info->offset, SEEK_SET);
    size_t bytes_read = fread(*sound_buffer, 1, sound_size, fp);
    if (bytes_read != data_info->size) fatal_error();

    fclose(fp);

    checksum = decode_xor(*sound_buffer, (u32)sound_size, data_info->xor_byte, data_info->checksum_byte);
    if (checksum != 0) {
        printf("[warning] LoadBnkFile(): incorrect checksum for SNDD8B.DAT\n");
    }


}

//3E9B4
void LoadBnkFixe(void) {
    if (CarteSonAutorisee && !SonLimite) {
        LoadTchatchPerdu();
    }
    LoadBnkFile(2, 2, bnkHeaderFixe, bnkDataFixe);
}

//3EA00
void LoadBnkWorld(i16 world) {
    switch(world) {
        case 1: {
            LoadBnkFile(4, 4, bnkHeaderWorld, bnkDataWorld);
        } break;
        case 2: {
            LoadBnkFile(6, 6, bnkHeaderWorld, bnkDataWorld);
        } break;
        case 3: {
            LoadBnkFile(5, 5, bnkHeaderWorld, bnkDataWorld);
        } break;
        case 4: {
            LoadBnkFile(3, 3, bnkHeaderWorld, bnkDataWorld);
        } break;
        case 5: {
            LoadBnkFile(1, 1, bnkHeaderWorld, bnkDataWorld);
        } break;
        case 6: {
            LoadBnkFile(0, 0, bnkHeaderWorld, bnkDataWorld);
        } break;
        default: break;
    }
}

//3EABC
void KeyOn(u8 bank, u8 prog, u8 tone, u8 note, u8 volume, u8 a6) {
    //stub
}

//3EC0C
void KeyOff(u32 a1, u8 bank, u8 prog, u8 tone, u8 note) {
    //stub
}

//3EC1C
void KeyVol(u32 a1, u32 a2, u32 a3, i16 a4) {
    //stub
}

//3EC78
void DigiMusicDone(void) {
    //stub
}

//3ECC4
void LoadTchatchPerdu(void) {
    //stub
}

//3EDF4
void PlayTchatch(void) {
    //stub
}

//3EFA4
void FreeTchatchVignette(void) {
    //stub
}

//3EFD4
void SetPort(i32 port) {
    current_port = port;
}

//3EFE0
void SetIrq(i32 irq) {
    current_irq = irq;
}

//3EFE8
void SetDma(i32 dma) {
    current_dma = dma;
}

//3EFF0
void SetParam(i32 param) {
    current_param = param;
}

//3EFF8
void SetDeviceID(i32 device_id) {
    current_device_id = device_id;
}

//3F000
char* GetDeviceName(void) {
    return current_device_name;
}

//3F008
i32 GetPort(void) {
    return current_port;
}

//3F010
i32 GetIrq(void) {
    return current_irq;
}

//3F018
i32 GetDma(void) {
    return current_dma;
}

//3F020
i32 GetParam(void) {
    return current_param;
}

//3F028
i32 GetDeviceID_Ray(void) {
    return current_device_id;
}

//3F030
void FreeDetectDriver(void) {
    //stub
}


