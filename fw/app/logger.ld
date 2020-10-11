/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2015 Karl Palsson <karlp@tweak.net.au>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Linker script for STM32F07xzB, 128k flash, 16k RAM. */

/* Define memory regions. */
MEMORY
{
/* first 0x800 bytes is bootloader, 0x80 is 7bit aligned fw header */
    rom (rx) : ORIGIN = 0x08000880, LENGTH = 128K-68K
/* First 0xc0 bytes reserved for vector table remapping */
    ram (rwx) : ORIGIN = 0x200000c0, LENGTH = 16K-192
}

/* Include the common ld script. */
INCLUDE cortex-m-generic.ld
