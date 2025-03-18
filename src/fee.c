
//39210
void INIT_TXT_FEE(void) {
    for (i32 i = 0; i <= 9; ++i) {
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
    for (i32 i = 0; i < level.nb_objects; ++i) {
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
void wait_for_dialogue_fee(obj_t* obj, i16 a2) {
    //stub
}

//39C04
void DO_FEE_ETAPE(obj_t* obj) {
    //stub
}

//39D20
void fee_gives_super_evts(void) {
    //stub
}

//39DB0
void DO_FEE(obj_t* obj) {
    //stub
}

//3A118
void DoFeeRaymanZDD(obj_t* obj) {
    //stub
}
