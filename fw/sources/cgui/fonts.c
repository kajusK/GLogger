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
 * @file    fonts.c
 * @brief   Fonts for cgui
 *
 * @addtogroup cgui
 * @{
 */

#include "fonts.h"

/* Dejavu Sans Mono font - https://fontlibrary.org/en/font/dejavu-sans-mono */
static const uint8_t cguii_font_data_8x12[] = {
    0xff, 0xff, 0xff, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xff, 0xf7, 0xf7, 0x00,
    0xff, 0xff, 0xff, 0xeb, 0xeb, 0xeb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xd7, 0xd3, 0x81, 0xcb, 0xeb, 0x80, 0xed, 0xe5, 0x00,
    0xff, 0xff, 0xff, 0xf7, 0xe3, 0xd1, 0xf5, 0xf1, 0xc7, 0x97, 0x95, 0xc3, 0x00,
    0xff, 0xff, 0xff, 0xf9, 0xf6, 0xf6, 0x99, 0xe3, 0xc9, 0xb7, 0xb7, 0x8f, 0x00,
    0xff, 0xff, 0xff, 0xe3, 0xf9, 0xf9, 0xfb, 0xf1, 0xa4, 0x8c, 0xcc, 0x81, 0x00,
    0xff, 0xff, 0xff, 0xf7, 0xf7, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
    0xff, 0xff, 0xef, 0xe7, 0xf7, 0xf7, 0xf3, 0xf3, 0xf3, 0xf7, 0xf7, 0xe7, 0x00,
    0xff, 0xff, 0xfb, 0xf7, 0xf7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xf7, 0xf7, 0x00,
    0xff, 0xff, 0xff, 0xf7, 0xd5, 0xe3, 0xe3, 0xd5, 0xf7, 0xff, 0xff, 0xff, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xf7, 0xf7, 0x80, 0xf7, 0xf7, 0xf7, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xf7, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe3, 0xff, 0xff, 0xff, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xe7, 0x00,
    0xff, 0xff, 0xff, 0xdf, 0xcf, 0xef, 0xe7, 0xf7, 0xf3, 0xfb, 0xfb, 0xf9, 0x00,
    0xff, 0xff, 0xff, 0xe3, 0xc9, 0xdd, 0x9d, 0x95, 0x9d, 0xdd, 0xc9, 0xe3, 0x00,
    0xff, 0xff, 0xff, 0xe1, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0x81, 0x00,
    0xff, 0xff, 0xff, 0xe3, 0xcd, 0xdf, 0xdf, 0xef, 0xe7, 0xf3, 0xf9, 0xc1, 0x00,
    0xff, 0xff, 0xff, 0xe3, 0xcd, 0xdf, 0xcf, 0xe3, 0xcf, 0xdf, 0xcd, 0xe1, 0x00,
    0xff, 0xff, 0xff, 0xcf, 0xc7, 0xc3, 0xcb, 0xc9, 0xcd, 0x80, 0xcf, 0xcf, 0x00,
    0xff, 0xff, 0xff, 0xc1, 0xfd, 0xfd, 0xe1, 0xcf, 0xdf, 0xdf, 0xcd, 0xe1, 0x00,
    0xff, 0xff, 0xff, 0xe3, 0xd9, 0xfd, 0xc1, 0xd9, 0x9d, 0x9d, 0xd9, 0xc3, 0x00,
    0xff, 0xff, 0xff, 0xc1, 0xdf, 0xcf, 0xef, 0xe7, 0xe7, 0xf7, 0xf3, 0xf3, 0x00,
    0xff, 0xff, 0xff, 0xe3, 0xd9, 0xdd, 0xd9, 0xe3, 0xd9, 0x9d, 0xd9, 0xc3, 0x00,
    0xff, 0xff, 0xff, 0xe3, 0xc9, 0xdd, 0x9d, 0x89, 0x83, 0xdf, 0xcd, 0xe3, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xe7, 0xff, 0xff, 0xe7, 0xe7, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xe7, 0xff, 0xff, 0xe7, 0xf7, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xc7, 0xf9, 0xf9, 0xc7, 0x9f, 0xff, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0xff, 0x80, 0xff, 0xff, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xe1, 0x8f, 0x8f, 0xe1, 0xfc, 0xff, 0x00,
    0xff, 0xff, 0xff, 0xe3, 0xcd, 0xdf, 0xcf, 0xf7, 0xf7, 0xff, 0xf7, 0xf7, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xc3, 0x99, 0xbc, 0x86, 0xb2, 0xb2, 0x86, 0xfc, 0x00,
    0xff, 0xff, 0xff, 0xe7, 0xe3, 0xeb, 0xcb, 0xc9, 0xd9, 0xc1, 0x9c, 0x9c, 0x00,
    0xff, 0xff, 0xff, 0xc1, 0xdd, 0xdd, 0xdd, 0xc1, 0xdd, 0x9d, 0x9d, 0xc1, 0x00,
    0xff, 0xff, 0xff, 0xc3, 0xd9, 0xf9, 0xfd, 0xfd, 0xfd, 0xf9, 0xd9, 0xc3, 0x00,
    0xff, 0xff, 0xff, 0xe1, 0xcd, 0xdd, 0x9d, 0x9d, 0x9d, 0xdd, 0xcd, 0xe1, 0x00,
    0xff, 0xff, 0xff, 0x81, 0xf9, 0xf9, 0xf9, 0xc1, 0xf9, 0xf9, 0xf9, 0x81, 0x00,
    0xff, 0xff, 0xff, 0x81, 0xf9, 0xf9, 0xf9, 0xc1, 0xf9, 0xf9, 0xf9, 0xf9, 0x00,
    0xff, 0xff, 0xff, 0xc3, 0xd9, 0xfd, 0xfd, 0x8c, 0x9d, 0x9d, 0x99, 0xc3, 0x00,
    0xff, 0xff, 0xff, 0x9d, 0x9d, 0x9d, 0x9d, 0x81, 0x9d, 0x9d, 0x9d, 0x9d, 0x00,
    0xff, 0xff, 0xff, 0xc1, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xc1, 0x00,
    0xff, 0xff, 0xff, 0xc3, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcf, 0xcd, 0xe1, 0x00,
    0xff, 0xff, 0xff, 0x9d, 0xcd, 0xe5, 0xf1, 0xf1, 0xe5, 0xcd, 0xdd, 0x9d, 0x00,
    0xff, 0xff, 0xff, 0xf9, 0xf9, 0xf9, 0xf9, 0xf9, 0xf9, 0xf9, 0xf9, 0x81, 0x00,
    0xff, 0xff, 0xff, 0x9c, 0x88, 0x88, 0x88, 0x84, 0x94, 0x9c, 0x9c, 0x9c, 0x00,
    0xff, 0xff, 0xff, 0x99, 0x99, 0x99, 0x95, 0x95, 0x85, 0x8d, 0x8d, 0x9d, 0x00,
    0xff, 0xff, 0xff, 0xe3, 0xc9, 0x9d, 0x9d, 0x9d, 0x9d, 0x9d, 0xc9, 0xe3, 0x00,
    0xff, 0xff, 0xff, 0xc1, 0x99, 0x99, 0x99, 0xc1, 0xf9, 0xf9, 0xf9, 0xf9, 0x00,
    0xff, 0xff, 0xff, 0xe3, 0xc9, 0x9d, 0x9d, 0x9d, 0x9d, 0xdd, 0xc9, 0xe3, 0x00,
    0xff, 0xff, 0xff, 0xe1, 0xcd, 0xdd, 0xcd, 0xe1, 0xcd, 0xdd, 0x9d, 0xbd, 0x00,
    0xff, 0xff, 0xff, 0xe3, 0xd9, 0xfd, 0xf9, 0xc3, 0xdf, 0x9f, 0xdd, 0xc1, 0x00,
    0xff, 0xff, 0xff, 0x80, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0x00,
    0xff, 0xff, 0xff, 0x9d, 0x9d, 0x9d, 0x9d, 0x9d, 0x9d, 0xdd, 0xd9, 0xe3, 0x00,
    0xff, 0xff, 0xff, 0x9c, 0x9d, 0xdd, 0xd9, 0xc9, 0xeb, 0xeb, 0xe3, 0xe7, 0x00,
    0xff, 0xff, 0xff, 0xbe, 0xbe, 0xb4, 0xa4, 0x80, 0x89, 0xc9, 0xc9, 0xd9, 0x00,
    0xff, 0xff, 0xff, 0x9d, 0xd9, 0xcb, 0xe3, 0xe7, 0xe3, 0xc9, 0xdd, 0x9c, 0x00,
    0xff, 0xff, 0xff, 0x9c, 0xdd, 0xc9, 0xe3, 0xe7, 0xf7, 0xf7, 0xf7, 0xf7, 0x00,
    0xff, 0xff, 0xff, 0x81, 0x9f, 0xcf, 0xef, 0xe7, 0xf3, 0xfb, 0xf9, 0x81, 0x00,
    0xff, 0xff, 0xe7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0x00,
    0xff, 0xff, 0xff, 0xfd, 0xf9, 0xfb, 0xfb, 0xf3, 0xf7, 0xe7, 0xef, 0xcf, 0x00,
    0xff, 0xff, 0xe3, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0x00,
    0xff, 0xff, 0xff, 0xe3, 0xcb, 0x9d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
    0xff, 0xff, 0xfb, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe1, 0xdf, 0xdf, 0xc1, 0xdd, 0xcd, 0xc1, 0x00,
    0xff, 0xff, 0xfd, 0xfd, 0xfd, 0xc1, 0xd9, 0x9d, 0x9d, 0x9d, 0xd9, 0xc1, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc3, 0xdb, 0xf9, 0xfd, 0xf9, 0xd9, 0xc3, 0x00,
    0xff, 0xff, 0xdf, 0xdf, 0xdf, 0xc3, 0xc9, 0xdd, 0xdd, 0xdd, 0xc9, 0xc3, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe3, 0xd9, 0x9d, 0x81, 0xfd, 0xd9, 0xc3, 0x00,
    0xff, 0xff, 0xc7, 0xf7, 0xf7, 0xc1, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc3, 0xc9, 0xdd, 0xdd, 0xdd, 0xc9, 0xc3, 0x00,
    0xff, 0xff, 0xfd, 0xfd, 0xfd, 0xc1, 0xd9, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0x00,
    0xff, 0xff, 0xf7, 0xff, 0xff, 0xf1, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0x81, 0x00,
    0xff, 0xff, 0xe7, 0xff, 0xff, 0xe1, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0x00,
    0xff, 0xff, 0xf9, 0xf9, 0xf9, 0xd9, 0xe9, 0xf1, 0xe1, 0xe9, 0xd9, 0x99, 0x00,
    0xff, 0xff, 0xf1, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xc7, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x94, 0x94, 0xb4, 0xb4, 0xb4, 0xb4, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc1, 0xd9, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe3, 0xc9, 0xdd, 0x9d, 0xdd, 0xc9, 0xe3, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc1, 0xd9, 0x9d, 0x9d, 0x9d, 0xd9, 0xc1, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc3, 0xc9, 0xdd, 0xdd, 0xdd, 0xc9, 0xc3, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x83, 0xf3, 0xfb, 0xfb, 0xfb, 0xfb, 0xfb, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe3, 0xd9, 0xf9, 0xe3, 0xdf, 0xcd, 0xe3, 0x00,
    0xff, 0xff, 0xff, 0xf3, 0xf3, 0xc1, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xc7, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xc9, 0xc3, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x9d, 0xdd, 0xd9, 0xcb, 0xeb, 0xe3, 0xe7, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xbe, 0xbc, 0x94, 0x85, 0xc9, 0xc9, 0xc9, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xdd, 0xcb, 0xe3, 0xe7, 0xe3, 0xc9, 0x9d, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x9d, 0xdd, 0xd9, 0xcb, 0xe3, 0xe3, 0xe7, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc1, 0xcf, 0xef, 0xf7, 0xf3, 0xf9, 0xc1, 0x00,
    0xff, 0xff, 0xc7, 0xe7, 0xf7, 0xf7, 0xf7, 0xf1, 0xf7, 0xf7, 0xf7, 0xe7, 0x00,
    0xff, 0xff, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0x00,
    0xff, 0xff, 0xf1, 0xf7, 0xf7, 0xf7, 0xe7, 0xcf, 0xe7, 0xf7, 0xf7, 0xf7, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb1, 0xc6, 0xff, 0xff, 0xff, 0x00
};

const cgui_font_t cgui_font_8x12 = { cguii_font_data_8x12, 8, 12, 33, 126 };

/** @} */