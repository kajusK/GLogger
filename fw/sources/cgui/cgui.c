/*
 * Copyright (C) 2019 Jakub Kaderka
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file    cgui.c
 * @brief   Small custom GUI library supporting basic drawing on monochromatic lcd
 *
 * @addtogroup cgui
 * @{
 */

#include "cgui.h"
#include "fonts.h"

static cgui_display_t cguii_disp;

void Cgui_FillScreen(bool value)
{
    Cgui_DrawFilledBox(0, 0, cguii_disp.width - 1, cguii_disp.height - 1, value);
}

void Cgui_DrawPixel(uint16_t x, uint16_t y, bool value)
{
    cguii_disp.draw(x, y, value);
}

void Cgui_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    /* driver acceleration */
    if (cguii_disp.line != NULL) {
        cguii_disp.line(x1, y1, x2, y2);
        return;
    }

    int16_t dx = x2 - x1;
    int16_t dy = y2 - y1;
    uint16_t absdx = abs(dx);
    uint16_t absdy = abs(dy);
    int16_t sgx = dx > 0 ? 1 : -1;
    int16_t sgy = dy > 0 ? 1 : -1;
    uint16_t i;
    uint16_t x = x1;
    uint16_t y = y1;
    uint16_t errx = absdx / 2;
    uint16_t erry = absdy / 2;

    /* Based on Bresenham's line algorithm */
    cguii_disp.draw(x, y, true);
    if (absdx > absdy) {
        for (i = 0; i < absdx; i++) {
            erry += absdy;
            if (erry > absdx) {
                erry -= absdy;
                y += sgy;
            }
            x += sgx;
            cguii_disp.draw(x, y, true);
        }
    } else {
        for (i = 0; i < absdy; i++) {
            errx += absdy;
            if (errx > absdy) {
                errx -= absdx;
                x += sgx;
            }
            y += sgy;
            cguii_disp.draw(x, y, true);
        }
    }
}

void Cgui_DrawBox(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    Cgui_DrawLine(x1, y1, x1, y2);
    Cgui_DrawLine(x1, y1, x2, y1);
    Cgui_DrawLine(x2, y1, x2, y2);
    Cgui_DrawLine(x1, y2, x2, y2);
}

void Cgui_DrawFilledBox(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
        bool value)
{
    uint16_t x, y;

    /* driver acceleration */
    if (cguii_disp.fill != NULL) {
        cguii_disp.fill(x1, y1, x2, y2, value);
        return;
    }

    for (x = x1; x <= x2; x++) {
        for (y = y1; y <= y2; y++) {
            cguii_disp.draw(x, y, value);
        }
    }
}

void Cgui_DrawImage(uint16_t pos_x, uint16_t pos_y, cgui_img_t *img)
{
    uint16_t x, y;
    uint32_t offset = 0;
    uint8_t rot = 0;

    for (y = pos_y; y < pos_y + img->height; y++) {
        for (x = pos_x; x < pos_x + img->width; x++) {
            if ((img->img[offset] >> rot) & 0x01) {
                cguii_disp.draw(x, y, true);
            } else if (!img->transparent) {
                cguii_disp.draw(x, y, false);
            }

            rot++;
            if (rot == 8) {
                rot = 0;
                offset += 1;
            }
        }
        y++;
    }
}

bool Cgui_Putc(uint16_t x, uint16_t y, char c)
{
    uint8_t *font;
    cgui_img_t img;
    uint32_t index;

    if (cguii_disp.font == NULL) {
        return false;
    }
    if (c < cguii_disp.font->start_id || c > cguii_disp.font->end_id) {
        Cgui_DrawFilledBox(x, y,
                x + cguii_disp.font->width - 1,
                y + cguii_disp.font->height - 1,
                false);
        return false;
    }

    index = (c - cguii_disp.font->start_id)*
            cguii_disp.font->width*cguii_disp.font->height/8;

    img.img = &cguii_disp.font->chars[index];
    img.width = cguii_disp.font->width;
    img.height = cguii_disp.font->height;
    img.transparent = false;

    Cgui_DrawImage(x, y, &img);
    return true;
}

void Cgui_Puts(uint16_t x, uint16_t y, const char *msg)
{
    uint16_t pos_x = x;

    if (cguii_disp.font == NULL) {
        return;
    }

    while (*msg != '\0') {
        Cgui_Putc(pos_x, y, *msg++);
        pos_x += cguii_disp.font->width;
    }
}

void Cgui_SetFont(const cgui_font_t *font)
{
    cguii_disp.font = font;
}

void Cgui_Init(cgui_draw_pixel_t draw, uint16_t width,
        uint16_t height)
{
    cguii_disp.draw = draw;
    cguii_disp.width = width;
    cguii_disp.height = height;

    cguii_disp.font = &cgui_font_8x12;
}

void Cgui_SetHwAccel(cgui_fill_frame_t fill, cgui_draw_line line)
{
    cguii_disp.fill = fill;
    cguii_disp.line = line;
}

/** @} */
