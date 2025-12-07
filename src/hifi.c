
//3E66C
void sub_3E66C(void) {
    print_once("Not implemented: sub_3E66C"); //stub
}

//3E6B0
void sub_3E6B0(void) {
    print_once("Not implemented: sub_3E6B0"); //stub
}

//3E6EC
void rayman_sound_fatal_error(const char* message) {
    printf("Rayman sound says fatal error :\n%s\n", message);
    exit(-1);
}

//3E710
void sub_3E710(void) {
    print_once("Not implemented: sub_3E710"); //stub
}

//3E780
void sub_3E780(void) {
    print_once("Not implemented: sub_3E780"); //stub
}

//3E820
void LoadBnkFile(i32 header_index, i32 data_index, bnk_header_t* headers, u8** data) {
    lock_audio(); // added

    // Load sound header
    FILE* fp = open_data_file("SNDH8B.DAT", false);
    if (!fp) {
        rayman_sound_fatal_error("Fichier data introuvable.\n");
    }

    archive_header_t* header_info = snd8b_header_infos + header_index;
    fseek(fp, header_info->offset, SEEK_SET);
    fread(headers, 1, header_info->size, fp); // always 0x800 bytes
    fclose(fp);

    u8 checksum = decode_xor((u8*)headers, header_info->size, header_info->xor_byte, header_info->checksum_byte);
    if (checksum != 0) {
        rayman_sound_fatal_error("Load error in sndh8b.dat.\n");
    }

    stop_cd();
    fp = open_data_file("SNDD8B.DAT", false);
    if (!fp) {
        rayman_sound_fatal_error("Fichier data introuvable.\n");
    }
    header_info = snd8b_data_infos + header_index;
    if (*data != NULL) {
        free(*data);
    }
    *data = (u8*)malloc(header_info->size);
    if (!*data) {
        rayman_sound_fatal_error("Mmoire insuffisante pour charger les samples.\n");
    }
    fseek(fp, header_info->offset, SEEK_SET);
    fread(*data, 1, header_info->size, fp);
    fclose(fp);
    checksum = decode_xor(*data, header_info->size, header_info->xor_byte, header_info->checksum_byte);
    if (checksum != 0) {
        rayman_sound_fatal_error("Load error in sndd8b.dat.\n");
    }

    // NOTE: here, apparently the game replaces the offsets in the header with pointers to live data
    // We skip this because it obviously only works on 32-bit systems (for now; might need to change the code later).
    // We should keep this difference in mind when we want to access the sound data.
#if 0
    for (i32 i = 1; i < 128; ++i) {
        bnk_header_t* bnk_header = headers + i;
        bnk_header[i].offset += (i32)*data;
    }
#endif

    unlock_audio(); // added
}

void LoadBnkFile_debug(i32 sound_set, i32 a2, u8** sound_buffer) {
    // Load sound header
    mem_t* mem = read_entire_file("SNDH8B.DAT", true);
    if (!mem) fatal_error();

    archive_header_t* header_info = &snd8b_header_infos[sound_set];

    mem_seek(mem, header_info->offset);
    mem_read(bnkHeaderFixe, mem, header_info->size); // always 0x800 bytes

    u8 checksum = decode_xor((u8*)bnkHeaderFixe, header_info->size, header_info->xor_byte, header_info->checksum_byte);
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
    LoadBnkFile(2, 2, bnkHeaderFixe, &bnkDataFixe);
}

//3EA00
void LoadBnkWorld(i16 world) {
    switch(world) {
        case 1: {
            LoadBnkFile(4, 4, bnkHeaderWorld, &bnkDataWorld);
        } break;
        case 2: {
            LoadBnkFile(6, 6, bnkHeaderWorld, &bnkDataWorld);
        } break;
        case 3: {
            LoadBnkFile(5, 5, bnkHeaderWorld, &bnkDataWorld);
        } break;
        case 4: {
            LoadBnkFile(3, 3, bnkHeaderWorld, &bnkDataWorld);
        } break;
        case 5: {
            LoadBnkFile(1, 1, bnkHeaderWorld, &bnkDataWorld);
        } break;
        case 6: {
            LoadBnkFile(0, 0, bnkHeaderWorld, &bnkDataWorld);
        } break;
        default: break;
    }
}

//3EABC
i16 KeyOn(u8 bank, u8 prog, u8 tone, u8 note, u8 volume, u8 pan) {
//    printf("KeyOn: bank %d prog %d tone %d note %d volume %d pan %d\n", bank, prog, tone, note, volume, pan);
    snd_t snd = {0};
    i32 sample_rate = 11025;
    float base_note = 54.5f; // NOTE: I determined this value by trial and error, it may not be exactly correct
    if (note != 60) {
        sample_rate = (i32)((float)sample_rate * powf(2.0f, ((float)note - base_note) / 12.0f));
    }

    if (bank == 1) {
        bnk_header_t* bnk_header = bnkHeaderWorld + prog;
        snd.data = bnkDataWorld + bnk_header->offset;
        snd.offset = bnk_header->offset;
        snd.size = bnk_header->size;
        snd.bytes_per_sample = 1;
        snd.sample_count = snd.size;
        snd.bnk_field_C = bnk_header->field_C;
        snd.sample_rate = sample_rate;
        snd.position = 0.0;
        snd.volume = MIN(1.0f, (float)volume / 50.0f);
    } else {
        bnk_header_t* bnk_header = bnkHeaderFixe + prog;
        snd.data = bnkDataFixe + bnk_header->offset;
        snd.offset = bnk_header->offset;
        snd.size = bnk_header->size;
        snd.sample_count = snd.size;
        snd.sample_rate = sample_rate;
        snd.bytes_per_sample = 1;
        snd.position = 0.0;
        snd.volume = MIN(1.0f, (float)volume / 50.0f);
    }
    i16 voice_index = play_digi_snd(&snd);
    return voice_index;
}

//3EC0C
void KeyOff(u32 voice_id, u8 bank, u8 prog, u8 tone, u8 note) {
    snd_t* digi_voice = digi_voices + voice_id;
    digi_voice->is_playing = false;
}

//3EC1C
void KeyVol(i32 voice_id, i32 volume, i32 pan) {
    snd_t* digi_voice = digi_voices + voice_id;
    digi_voice->volume = MIN(1.0f, (float)volume / 50.0f);
}

//3EC78
void DigiMusicDone(void) {
    for (i32 i = 0; i < COUNT(voice_table); ++i) {
        KeyOff(i, 0, 0, 0, 0);
    }
    print_once("Not implemented: DigiMusicDone"); //stub
}

//3ECC4
void LoadTchatchPerdu(void) {
    stop_cd();
    FILE* fp = open_data_file("SNDVIG.DAT", true);
    if (fp) {
        archive_header_t* sndvig_header = sndvig_infos + 6;
        ptrTchatch = (u8*)malloc(sndvig_header->size);
        fseek(fp, sndvig_header->offset, SEEK_SET);
        fread(ptrTchatch, 1, sndvig_header->size, fp);
        fclose(fp);
        u8 checksum = decode_xor(ptrTchatch, sndvig_header->size, sndvig_header->xor_byte, sndvig_header->checksum_byte);
        if (checksum != 0) {
            rayman_sound_fatal_error("Load error in sndvig.dat.\n");
        }
        TchachPerduPtr = ptrTchatch;
        TchachPerduSize = sndvig_header->size;
        ptrTchatch = NULL;
    }
}

//3EDF4
void PlayTchatch(i32 snd, i32 volume) {
    print_once("Not implemented: PlayTchatch"); //stub
}

//3EFA4
void FreeTchatchVignette(void) {
    print_once("Not implemented: FreeTchatchVignette"); //stub
}

//3EFD4
void SetPort_(i32 port) {
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
    print_once("Not implemented: FreeDetectDriver"); //stub
}


