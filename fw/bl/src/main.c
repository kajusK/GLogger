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
 * @file    app/main.c
 * @brief   Main file for bootloader
 *
 * @addtogroup app
 * @{
 */

#include <hal/usb_msc.h>
#include <drivers/ramdisk.h>
#include <modules/fw.h>

/**
 * Select latest valid image and boot it
 */
int main(void)
{
    Fw_Run();

    /* If fw image failed to boot, wait forever */
    /* TODO - create flash disk for fw update */

    /*
    Ramdisk_Init(64000000, "Bootloader");
    Usbd_MscInit(Ramdisk_GetSectors(), Ramdisk_Read, Ramdisk_Write);
    */

    while (1) {
     //   Usbd_MscPool();
    }
    return 0;
}
