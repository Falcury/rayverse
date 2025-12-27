
//39210
void INIT_TXT_FEE(void) {
    for (s32 i = 0; i <= 9; ++i) {
        if (text_to_display[i].text[0] != '\0') {
            text_to_display[i].xpos = 150;
            text_to_display[i].ypos = 188;
            text_to_display[i].font_size = 2;
            text_to_display[i].color = 5;
            text_to_display[i].field_D5 = 0;
            text_to_display[i].is_fond = 0;
            INIT_TXT_BOX(&text_to_display[i]);
            text_to_display[i].xpos = text_to_display[i].centered_x_pos;
            text_to_display[i].text[0] = ' ';
        } else {
            text_to_display[i].field_D5 = 0;
        }
    }
    display_txt_fee = -1;
    old_txt_fee = -1;
}

//392AC
void allocate_poing_or_free(void) {
    obj_t* obj = NULL;
    for (s32 i = 0; i < level.nb_objects; ++i) {
        obj = level.objects + i;
        if (obj->type == TYPE_238_POING_FEE && !obj->flags.alive) {
            break;
        }
    }
    if (obj) {
        obj->flags.alive = 0;
        obj->is_active = 0;
        add_alwobj(obj);
        obj->flags.flip_x = 1;
        obj->speed_x = 8;
        obj->speed_y = 8;
        obj->anim_frame = 0;
        png_or_fee_id = obj->id;
    }
}

//3932C
void init_fee(void) {
    display_txt_fee = -1;
    if (!ModeDemo) {
        record.is_playing = 0;
    }
    new_txt_fee = 0;
    is_fee = 0;
    INIT_TEXT_TO_DISPLAY();
    if (num_world == 1) {
        if (num_level == 3) {
            is_fee = 1;
            strcpy(text_to_display[0].text, language_txt[44]); // /hello rayman/
            strcpy(text_to_display[1].text, language_txt[45]); // /i'm betilla the fairy/
            strcpy(text_to_display[2].text, language_txt[46]); // /and i'm going to help you/
            strcpy(text_to_display[3].text, language_txt[47]); // /in your quest./
            strcpy(text_to_display[4].text, language_txt[48]); // /i can give you a new power./
            strcpy(text_to_display[5].text, language_txt[49]); // /now you can punch with your fist/
            strcpy(text_to_display[6].text, language_txt[50]); // /good luck!/
            strcpy(text_to_display[7].text, language_txt[51]); // /press the $ button to punch/
            strcpy(text_to_display[8].text, language_txt[52]); // /the longer you press/
            strcpy(text_to_display[9].text, language_txt[53]); // /the further you punch/
            Deter_Option_Caract(text_to_display[7].text, options_jeu.fist, 255);
        } else if (num_level == 8) {
            is_fee = 1;
            strcpy(text_to_display[0].text, language_txt[54]); // /you're courageous rayman/
            strcpy(text_to_display[1].text, language_txt[55]); // /but you will have to keep at it/
            strcpy(text_to_display[2].text, language_txt[56]); // (empty string)
            strcpy(text_to_display[3].text, language_txt[57]); // (empty string)
            strcpy(text_to_display[4].text, language_txt[58]); // (empty string)
            strcpy(text_to_display[5].text, language_txt[59]); // /you now have the power to hang/
            strcpy(text_to_display[6].text, language_txt[60]); // (empty string)
            strcpy(text_to_display[7].text, language_txt[61]); // (empty string)
            strcpy(text_to_display[8].text, language_txt[62]); // (empty string)
            strcpy(text_to_display[9].text, language_txt[63]); // (empty string)
        } else if (num_level == 17) {
            is_fee = 1;
            strcpy(text_to_display[0].text, language_txt[64]); // /a gloomy part of your quest/
            strcpy(text_to_display[1].text, language_txt[65]); // /will start now.../
            strcpy(text_to_display[2].text, language_txt[66]); // /i hope this will help you/
            strcpy(text_to_display[3].text, language_txt[67]); // /to avoid the worst./
            strcpy(text_to_display[4].text, language_txt[68]); // (empty string)
            strcpy(text_to_display[5].text, language_txt[69]); // /now you can grab things/
            strcpy(text_to_display[6].text, language_txt[70]); // /with your fist./
            strcpy(text_to_display[7].text, language_txt[71]); // (empty string)
            strcpy(text_to_display[8].text, language_txt[72]); // /to grab use the $ button/
            strcpy(text_to_display[9].text, language_txt[73]); // (empty string)
            Deter_Option_Caract(text_to_display[7].text, options_jeu.fist, 255);
        }
    } else if (num_world == 2) {
        if (num_level == 11) {
            is_fee = 1;
            strcpy(text_to_display[0].text, language_txt[74]); // /things are not going to get/
            strcpy(text_to_display[1].text, language_txt[75]); // /any easier for you rayman.../
            strcpy(text_to_display[2].text, language_txt[76]); // /but this new power /
            strcpy(text_to_display[3].text, language_txt[77]); // /will make you stronger./
            strcpy(text_to_display[4].text, language_txt[78]); // (empty string)
            strcpy(text_to_display[5].text, language_txt[79]); // (empty string)
            strcpy(text_to_display[6].text, language_txt[80]); // (empty string)
            strcpy(text_to_display[7].text, language_txt[81]); // /now you can use your hair/
            strcpy(text_to_display[8].text, language_txt[82]); // /as a helicopter/
            strcpy(text_to_display[9].text, language_txt[83]); // /to do it press $ twice/
            Deter_Option_Caract(text_to_display[7].text, options_jeu.jump, 255);
        }
    } else if (num_world == 3) {
        if (num_level == 11) {
            is_fee = 1;
            strcpy(text_to_display[0].text, language_txt[84]); // /things are going to get/
            strcpy(text_to_display[1].text, language_txt[85]); // /really wild from now on rayman/
            strcpy(text_to_display[2].text, language_txt[86]); // /you'll have to be very quick./
            strcpy(text_to_display[3].text, language_txt[87]); // (empty string)
            strcpy(text_to_display[4].text, language_txt[88]); // (empty string)
            strcpy(text_to_display[5].text, language_txt[89]); // /now you can run faster/
            strcpy(text_to_display[6].text, language_txt[90]); // (empty string)
            strcpy(text_to_display[7].text, language_txt[91]); // /to run press $ while walking/
            strcpy(text_to_display[8].text, language_txt[92]); // (empty string)
            strcpy(text_to_display[9].text, language_txt[93]); // (empty string)
            Deter_Option_Caract(text_to_display[7].text, options_jeu.action, 255);
        }
    }
    if (is_fee) {
        INIT_TXT_FEE();
        allocate_poing_or_free();
    }
}

//39BD0
void wait_for_dialogue_fee(obj_t* obj, s16 time) {
    /* 4DFD0 801727D0 -O2 -msoft-float */
    if (obj->timer < time) {
        obj->timer++;
    } else {
        obj->timer = 0;
        display_txt_fee++;
    }
}

//39C04
void DO_FEE_ETAPE(obj_t* obj) {
    /* 4E020 80172820 -O2 -msoft-float */
    u8 state = obj->configuration;

    switch (state) {
        case 1:
            wait_for_dialogue_fee(obj, 100);
            if (display_txt_fee == 5 || text_to_display[display_txt_fee].text[0] == '\0')
            {
                skipToLabel(obj, 1, false);
                obj->configuration = 0;
                display_txt_fee = 255;
                obj->timer = 0;
            }
            break;
        case 2:
            wait_for_dialogue_fee(obj, 100);
            if (display_txt_fee == 7) {
                display_txt_fee = 255;
                obj->configuration = 3;
                obj->timer = 0;
            }
            break;
        case 3:
            obj->timer++;
            if (obj->timer >= 51) {
                obj->configuration = 4;
                display_txt_fee = 7;
                obj->timer = 0;
            }
            break;
        case 4:
            if (horloge[2] != 0)
                obj->speed_y = -1;
            else
                obj->speed_y = 0;

            if (display_txt_fee < 9 && text_to_display[display_txt_fee + 1].text[0] != '\0')
                wait_for_dialogue_fee(obj, 120);
            break;
        case 0:
        default:
            display_txt_fee = 255;
            break;
    }
}

//39D20
void fee_gives_super_evts(void) {
    /* 4E1B4 801729B4 -O2 -msoft-float */
    switch (num_world) {
        case 1:
            if (num_level == 3)
                RayEvts.poing = true;
            if (num_level == 8)
                RayEvts.hang = true;
            if (num_level == 17)
                RayEvts.grap = true;
            break;
        case 2:
            if (num_level == 11)
                RayEvts.helico = true;
            break;
        case 3:
            if (num_level == 11)
                RayEvts.run = true;
            break;
    }
}

//39DB0
void DO_FEE(obj_t* obj) {
    /* 4E2D0 80172AD0 -O2 -msoft-float */
    u8 old_txt;
    obj_t* png_or_fee;
    u8 ray_main_etat;
    s16 y;

    old_txt = display_txt_fee;
    png_or_fee = &level.objects[png_or_fee_id];
    switch (obj->sub_etat)
    {
        case 0:
            if (obj->detect_zone_flag != 0) {
                if (Abs(ray.x + ray.offset_bx - obj->x + 2) < 3) {
                    ray_main_etat = ray.main_etat;
                    if (ray_main_etat < 2 || (ray.main_etat == 3 && (ray.sub_etat == 20 || ray.sub_etat == 21))) {
                        set_sub_etat(obj, 1);
                        skipToLabel(obj, 0, true);
                        set_main_and_sub_etat(&ray, 3, 34);
                        remoteRayXToReach = -32000;
                        ray.flags.flip_x = 1;
                    }
                }
            } else {
                ray_in_fee_zone = false;
            }
            obj->configuration = 0;
            break;
        case 1:
            if (display_txt_fee == 255) {
                obj->configuration = 1;
                display_txt_fee = 0;
                obj->timer = 0;
            }
            break;
        case 2:
        case 3:
            ray_main_etat = ray.main_etat;
            if (ray_main_etat < 2 || (ray.main_etat == 3 && ray.sub_etat == 34))
                set_main_and_sub_etat(&ray, 3, 35);
            if (ray.main_etat == 3 && ray.sub_etat == 36)
                ray.anim_frame = -1;
            break;
        case 4:
            if (EOA(obj)) {
                fee_gives_super_evts();
                obj->configuration = 2;
                display_txt_fee = 5;
                obj->timer = 0;
            }
            else
                obj->configuration = 0;
            break;
        case 6:
            if (ray.main_etat == 3)
            {
                if (ray.sub_etat == 36)
                    set_sub_etat(&ray, 37);
                if (ray.main_etat == 3 && ray.sub_etat == 20)
                {
                    remoteRayXToReach = ray.x;
                    if (obj->configuration == 4)
                    {
                        set_main_and_sub_etat(&ray, 0, 0);
                        remoteRayXToReach = -32000;
                    }
                }
            }
            break;
    }
    DO_FEE_ETAPE(obj);
    if (display_txt_fee != old_txt)
    {
        old_txt_fee = old_txt; // added in PC version
        new_txt_fee = 1;
        if (display_txt_fee != 255)
        {
            if (!png_or_fee->is_active) {
                add_alwobj(png_or_fee); // added in PC version
            }
            png_or_fee->active_timer = 0;
            png_or_fee->flags.alive = 1;
            png_or_fee->flags.flip_x = 1;
            png_or_fee->x = xmap - png_or_fee->offset_bx;
            s16 y_fee = (SCREEN_HEIGHT < 240) ? SCREEN_HEIGHT - 10 : SCREEN_HEIGHT - 20; // added to account for PC/PS1 difference
            y = ymap - (png_or_fee->offset_by - y_fee);
            png_or_fee->y = y;
            png_or_fee->screen_x = -png_or_fee->offset_bx;
            png_or_fee->screen_y = y_fee - png_or_fee->offset_by;
            old_txt_fee = old_txt;
            png_or_fee->init_x = png_or_fee->x;
            png_or_fee->init_y = png_or_fee->y;
        }
    }
    if (new_txt_fee != 0) {
        // NOTE: fixed out-of-bounds access: old_txt_fee may be 255!
        if (old_txt_fee != 255 && text_to_display[old_txt_fee].ypos < SCREEN_HEIGHT + 20) {
            text_to_display[old_txt_fee].ypos++;
        } else {
            new_txt_fee = 0;
        }
    }
    if (png_or_fee->flags.alive) {
        //NOTE: below is added in PC versino
        if (png_or_fee->active_flag == 2 && !png_or_fee->is_active) {
            png_or_fee->active_timer = 0;
            png_or_fee->active_flag = 0;
            png_or_fee->flags.alive = 0;
        }
        png_or_fee->x += 4;
        png_or_fee->screen_x += 4;
        if (png_or_fee->screen_x > 310) {
            png_or_fee->anim_frame = 0;
            png_or_fee->flags.alive = 0;
            png_or_fee->is_active = 0;
        }
    }
}

//3A118
void DoFeeRaymanZDD(obj_t* obj) {
    if (obj->sub_etat != 0) {
        ray_in_fee_zone = 0;
    } else if (ray.main_etat == 2) {
        ray.speed_x = 0;
        ray.link = 0;
        ray_in_fee_zone = 1;
        decalage_en_cours = 0;
    } else if (ray.main_etat == 1 || ray.main_etat == 0 || (ray.main_etat == 3 && ray.sub_etat == 20)) {
        remoteRayXToReach = (obj->x - ray.offset_bx - 2) & ~1;
        if (ray.x & 1) {
            remoteRayXToReach |= 1;
        } else {
            remoteRayXToReach &= ~1;
        }
        ray_in_fee_zone = 0;
    }
}
