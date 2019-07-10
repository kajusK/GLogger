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
 * @file    main.h
 * @brief   Main file for unit tests
 *
 * @addtogroup tests
 * @{
 */

#include "main.h"

uint8_t assert_should_fail = false;

static void RunAll(void)
{
    Time_RunTests();
    Nmea_RunTests();
    Ringbuf_RunTests();
    Math_RunTests();
    Nav_RunTests();
    Ramdisk_RunTests();
}

int main(int argc, const char *argv[])
{
    UnityMain(argc, argv, RunAll);
}

/** @} */
