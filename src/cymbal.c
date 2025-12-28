

//31F00
void DO_CYMBAL_COMMAND(obj_t* obj) {
    /* 49B74 8016E374 -O2 */
    s16 spr_x; s16 spr_y; s16 spr_w; s16 spr_h;
    s32 diff_y;
    s16 unk_1;
    s32 unk_2;
    s32 unk_3;
    u8 unk_4;
    u8 unk_5;
    u8 sub_etat;
    s16 spd_x = obj->speed_x;
    s16 diff_x = ray.x + ray.offset_bx - obj->x - obj->offset_bx;

    if (ray.follow_id == obj->id)
    {
        obj->gravity_value_2 = 5;
        if (diff_x > 0)
        {
            if (diff_x > 98)
                ray.y += 8;
            else
            {
                if (diff_x > 70)
                    unk_1 = 4;
                else if (diff_x > 60)
                    unk_1 = 3;
                else if (diff_x > 40)
                    unk_1 = 2;
                else if (diff_x > 20)
                    unk_1 = 1;
                else
                    unk_1 = 0;
            }

            if (spd_x < unk_1 && obj->gravity_value_1-- == 0)
            {
                spd_x++;
                obj->gravity_value_1 = 5;
            }
            else if (spd_x > unk_1 && obj->gravity_value_1-- == 0)
            {
                spd_x--;
                obj->gravity_value_1 = 5;
            }
        }
        else
        {
            diff_x = -diff_x;
            unk_2 = 510;
            unk_3 = 255;
            if (diff_x > 105)
                ray.y += 8;
            else
            {
                if (diff_x > 70)
                    unk_1 = -4;
                else if (diff_x > 60)
                    unk_1 = -3;
                else if (diff_x > 40)
                    unk_1 = -2;
                else if (diff_x > 20)
                    unk_1 = -1;
                else
                    unk_1 = 0;
            }

            if (spd_x > unk_1)
            {
                /* TODO: ??? */
                unk_4 = obj->gravity_value_1;
                unk_5 = unk_4 + unk_3;
                obj->gravity_value_1 = unk_5;
                if (unk_4 == 0 || (obj->gravity_value_1 = unk_4 + unk_2, unk_5 == 0))
                {
                    spd_x--;
                    obj->gravity_value_1 = 5;
                }
            }
        }
    }
    else
    {
        if (obj->gravity_value_2-- == 0)
        {
            obj->gravity_value_2 = 5;
            if (spd_x != 0)
            {
                if (spd_x > 0)
                    spd_x--;
                else
                    spd_x++;
            }
        }
    }
    if (on_block_chdir(obj, obj->offset_bx + spd_x * 2, obj->offset_by))
        spd_x = 0;

    obj->speed_x = spd_x;
    if (obj->main_etat == 0)
    {
        sub_etat = obj->sub_etat;
        if (sub_etat == 0)
        {
            if (obj->link == 0 && ray.follow_id == obj->id)
            {
                if (obj->timer > 100)
                {
                    obj->link = 1;
                    obj->timer = 0;
                    skipToLabel(obj, 2, true);
                }
                else
                    obj->timer++;
            }
        }
        else if (sub_etat == 1)
        {
            obj->link = 0;
            if (
                    ray.main_etat == 2 && ray.iframes_timer == -1 &&
                    obj->anim_frame >= 20 && obj->anim_frame <= 22
                    )
            {
                GET_SPRITE_POS(obj, 1, &spr_x, &spr_y, &spr_w, &spr_h);
                if (Abs(diff_x) < 100)
                {
                    diff_y = ray.y + ray.offset_by - spr_y - obj->offset_hy;
                    if (ABS(diff_y - 28) < 20)
                    {
                        if (diff_x < 0)
                            ray.flags.flip_x = 1;
                        else
                            ray.flags.flip_x = 0;
                        RAY_HIT(true, obj);
                    }
                }
            }
        }
        else if (sub_etat == 3)
        {
            if (!(ray.is_active) && obj->anim_frame == 0 && ray.follow_id == obj->id)
            {
                ray.is_active = 1;
                ray.iframes_timer = -1;
                ray.x -= 4;
                GET_SPRITE_POS(obj, 1, &spr_x, &spr_y, &spr_w, &spr_h);
                ray.y = obj->offset_hy + spr_y - ray.offset_by;
                set_main_and_sub_etat(&ray, 0, 8);
            }
        }
    }
}

//32280
void DO_2_PARTS_CYMBAL(obj_t* cym_obj) {
    /* 4A078 8016E878 -O2 -msoft-float */
    s16 cym_x; s16 cym_y; s16 cym_w; s16 cym_h;
    s16 cym_offs_bx; s16 cym_offs_by;
    u8 cym_sub_etat;
    obj_t *link_obj;
    s16 link_offs_bx; s16 link_offs_by;
    u8 link_sub_etat;
    s16 diff_x_1;
    s16 diff_x_2;
    s16 diff_y;
    s32 unk_1;

    cym_offs_bx = cym_obj->offset_bx;
    cym_offs_by = cym_obj->offset_by;
    if (cym_obj->link == 0)
    {
        cym_obj->speed_x = 0;
        cym_obj->speed_y = 0;
    }
    else if (cym_obj->link == 1)
    {
        if (ray.follow_id == cym_obj->id)
        {
            diff_x_1 = (ray.x + ray.offset_bx) - cym_obj->x - cym_offs_bx;
            diff_x_2 = diff_x_1; /* ??? */
            if (diff_x_1 > 96 || diff_x_2 <= -108)
                ray.y += 8;
        }
        if (
                (s16) on_block_chdir(cym_obj, cym_offs_bx, cym_offs_by) &&
                (s16) test_allowed(cym_obj, cym_offs_bx, cym_offs_by)
                )
            skipToLabel(cym_obj, 99, true);
        cym_obj->flags.flip_x = 0;
        if (cym_obj->cmd == 20)
        {
            /* ??? */
            cym_obj->speed_x = cym_obj->iframes_timer;
            cym_obj->speed_y = cym_obj->follow_id;
        }
    }
    if (cym_obj->link > 0)
    {
        link_obj = &level.objects[link_init[cym_obj->id]];
        if (!(link_obj->is_active))
            link_obj->is_active = 1;

        unk_1 = -1;
        if (cym_obj->speed_x == 0 && cym_obj->speed_y == 0) // NOTE: speed_y check added in PC version
        {
            if (cym_obj->link == 1)
            {
                cym_obj->link = 2;
                skipToLabel(link_obj, 99, true);
            }
            if (cym_obj->link == 2)
            {
                link_obj->link = 3;
                diff_x_2 = link_obj->x - cym_obj->x;
                diff_y = link_obj->y - cym_obj->y;
                if (diff_x_2 > 6)
                    link_obj->speed_x = -4;
                else if (diff_x_2 < -6)
                    link_obj->speed_x = 4;
                else if (diff_x_2 > 0)
                    link_obj->speed_x = -1;
                else if (diff_x_2 < 0)
                    link_obj->speed_x = 1;
                else
                {
                    link_obj->speed_x = 0;
                    if (diff_y < -8)
                        link_obj->speed_y = 4;
                    else if (diff_y > 8)
                        link_obj->speed_y = -4;
                    else if (diff_y > 0)
                        link_obj->speed_y = -1;
                    else if (diff_y >= 0)
                    {
                        link_obj->speed_y = 0;
                        cym_obj->link = 4;
                        link_obj->link = 4;
                        if (link_obj->type == TYPE_CYMBAL1)
                        {
                            cym_sub_etat = 9;
                            link_sub_etat = 5;
                        }
                        else
                        {
                            cym_sub_etat = 5;
                            link_sub_etat = 9;
                        }
                        set_main_and_sub_etat(link_obj, 0, link_sub_etat);
                        set_main_and_sub_etat(cym_obj, 0, cym_sub_etat);
                    }
                    else
                        link_obj->speed_y = 1;
                }
            }
            else
            {
                if (cym_obj->link == 4)
                {
                    if (cym_obj->type == TYPE_CYMBAL1)
                        cym_sub_etat = 7;
                    else
                        cym_sub_etat = 11;

                    if (
                            cym_obj->main_etat == 0 && cym_obj->sub_etat == cym_sub_etat &&
                            cym_obj->anim_frame == 0
                            )
                    {
                        cym_obj->link = 5;
                        if (
                                ray.is_active == 0 && ray.follow_id == cym_obj->id &&
                                ray.iframes_timer == unk_1 && cym_obj->type == TYPE_CYMBAL2
                                )
                        {
                            ray.x -= 4;
                            ray.is_active = 1;
                            GET_SPRITE_POS(cym_obj, 1, &cym_x, &cym_y, &cym_w, &cym_h);
                            ray.y = cym_obj->offset_hy + cym_y - ray.offset_by;
                            set_main_and_sub_etat(&ray, 0, 8);
                        }
                    }
                }
                else if (cym_obj->link == 5)
                {
                    link_offs_bx = link_obj->offset_bx;
                    link_offs_by = link_obj->offset_by;
                    /* ??? */
                    cym_x = on_block_chdir(cym_obj, cym_offs_bx, cym_offs_by);
                    cym_y = on_block_chdir(link_obj, link_offs_bx, link_offs_by);
                    cym_w = test_allowed(cym_obj, cym_offs_bx, cym_offs_by);
                    cym_h = test_allowed(link_obj, link_offs_bx, link_offs_by);
                    if (
                            (s16) on_block_chdir(cym_obj, cym_offs_bx, cym_offs_by) &&
                            (s16) on_block_chdir(link_obj, link_offs_bx, link_offs_by)
                            )
                    {
                        cym_obj->link = 1;
                        link_obj->link = 1;
                        test_allowed(cym_obj, cym_offs_bx, cym_offs_by);
                        test_allowed(link_obj, link_offs_bx, link_offs_by);
                        skipToLabel(cym_obj, 99, true);
                        skipToLabel(link_obj, 99, true);
                    }
                    else if (!(s16) on_block_chdir(link_obj, link_offs_bx, link_offs_by))
                    {
                        if (link_obj->type == TYPE_CYMBAL1)
                            link_obj->speed_y = -1;
                        else
                            link_obj->speed_y = 1;
                    }
                }
            }
        }
    }
}

//32738
void DO_COLL_RAY_CYMBALE(obj_t* cym_obj) {
    /* 20400 80144C00 -O2 -msoft-float */
    s32 cym_sub_etat;
    u8 cym_link_id;
    obj_t *cym_link_obj;
    s16 *cym_speed_y;
    s16 ray_speed_y;
    s16 diff_x;
    s16 sprite = 0; /* i think, BOX_IN_COLL_ZONES returned a sprite? */

    if (cym_obj->type == TYPE_CYMBAL2)
    {
        cym_sub_etat = 10;
        cym_link_id = link_init[cym_obj->id];
        if (cym_link_id != cym_obj->id)
        {
            cym_link_obj = &level.objects[cym_link_id];
            if (cym_link_obj->type == TYPE_CYMBAL1)
            {
                cym_speed_y = &cym_link_obj->speed_y;
                if (*cym_speed_y > 128)
                    *cym_speed_y = 1;
                else if (*cym_speed_y < -128)
                    *cym_speed_y = -1;
                cym_link_obj->y += cym_link_obj->speed_y * 2;
                sprite = BOX_IN_COLL_ZONES(TYPE_RAYMAN, ray_zdc_x, ray_zdc_y, ray_zdc_w, ray_zdc_h, cym_link_obj);
                cym_link_obj->y -= (u16) cym_link_obj->speed_y * 2;
                ray_speed_y = cym_link_obj->speed_y * 2;
            }
        }
    }
    else
    {
        sprite = BOX_IN_COLL_ZONES(TYPE_RAYMAN, ray_zdc_x, ray_zdc_y, ray_zdc_w, ray_zdc_h, cym_obj);
        cym_sub_etat = 1;
        ray_speed_y = cym_obj->speed_y;
    }
    if (sprite != -1)
    {
        sprite--;
        if (ray.main_etat == 2 && !(ray.sub_etat == 1 || ray.sub_etat == 2))
        {
            if (ray.sub_etat != 8)
                set_sub_etat(&ray, 1);

            if (ray_speed_y > 128)
                ray_speed_y = 1;
            else if (ray_speed_y < -128)
                ray_speed_y = -1;
            ray.y += ray_speed_y;
            ray.link = 0;
            ray.speed_y = 0;
        }

        if (!(ray.main_etat == 2 && ray.sub_etat == 8))
        {
            if (
                    !(ray.main_etat == 0 && ray.sub_etat == 61) &&
                    cym_obj->anim_frame >= 20 && cym_obj->anim_frame <= 22 &&
                    cym_obj->main_etat == 0 && cym_obj->sub_etat == cym_sub_etat
                    )
            {
                if (
                        (cym_obj->follow_sprite == 1 || sprite == 1) &&
                        ray.flags.alive &&
                        !(ray.main_etat == 2 && ray.sub_etat == 8) &&
                        !(ray.main_etat == 0 && ray.sub_etat == 61)
                        )
                {
                    diff_x = ray.x + ray.offset_bx - cym_obj->x - cym_obj->offset_bx;
                    if (Abs(diff_x) < 25)
                    {
                        ray.speed_x = 0;
                        ray.speed_y = 0;
                        decalage_en_cours = 0;
                        ray.is_active = 0;
                        ray.iframes_timer = -1;
                        set_main_and_sub_etat(&ray, 0, 0);
                        ray.follow_id = cym_obj->id;
                    }
                    else
                    {
                        if (diff_x < 0)
                            ray.flags.flip_x = 1;
                        else
                            ray.flags.flip_x = 0;
                        RAY_HIT(true, cym_obj);
                    }
                }
                else
                {
                    if (cym_obj->follow_sprite == 0 || sprite == 0)
                        ray.flags.flip_x = 1;
                    else
                        ray.flags.flip_x = 0;
                    RAY_HIT(true, cym_obj);
                }
            }
        }
    }
}

