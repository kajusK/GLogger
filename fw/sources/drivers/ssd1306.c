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
 * @file    drivers/ssd1306.c
 * @brief   SSD1306 oled controller driver, 128x64
 *
 * @addtogroup drivers
 * @{
 */

#include "hal/i2c.h"
#include "hal/io.h"
#include "utils/time.h"

#include "drivers/ssd1306.h"

#define SSD1306_I2C_ADDR 0x3c

typedef enum {
    SSD1306_DISP_OFF = 0xae,
    SSD1306_DISP_ON = 0xaf,
    SSD1306_CLK_DIV = 0xd5,
    SSD1306_MULTIPLEX = 0xa8,
    SSD1306_PRECHARGE = 0xd9,
    SSD1306_DISP_OFFSET = 0xd3,
    SSD1306_CHARGE_PUMP = 0x8d,
    SSD1306_MEM_MODE = 0x20,
    SSD1306_COLSCANDEC = 0xa1,
    SSD1306_ROWSCANDEC = 0xc8,
    SSD1306_COM_PINS = 0xda,
    SSD1306_VCOM_DETECT = 0xdb,
    SSD1306_DISP_NORM = 0xa6,
    SSD1306_HV_COL_ADDR = 0x21,
    SSD1306_HV_PAGE_ADDR = 0x22,
    SSD1306_START_LINE = 0x40,
} ssd1306_cmd_t;

/** Framebuffer for the display data, 1 byte per page for write command */
static uint8_t ssd1306i_fbuf[(SSD1306_WIDTH+1)*SSD1306_HEIGHT/8];

/**
 * Send graphical data to display
 *
 * @param buf       Data buffer to be send
 * @param len       Lenght of the buffer
 *
 * @return  True if ACKed
 */
static bool SSD1306i_Data(const uint8_t *buf, size_t len)
{
    return I2Cd_Transceive(SSD1306_I2C_DEV, SSD1306_I2C_ADDR, buf, len, NULL, 0);
}

/**
 * Send command to display
 *
 * @param cmd       Command
 *
 * @return  True if ACKed
 */
static bool SSD1306i_Cmd(ssd1306_cmd_t cmd)
{
    uint8_t buf[2];

    buf[0] = 0x00;
    buf[1] = cmd;

    return I2Cd_Transceive(SSD1306_I2C_DEV, SSD1306_I2C_ADDR, buf, 2, NULL, 0);
}

/**
 * Send command with additional data to display
 *
 * @param cmd       Command
 * @param data      Data for the command
 */
static void SSD1306i_Cmd2(ssd1306_cmd_t cmd, uint8_t data)
{
    SSD1306i_Cmd(cmd);
    SSD1306i_Cmd(data);
}

/**
 * Send command with 2 additional data to display
 *
 * @param cmd       Command
 * @param data1     Data for the command
 * @param data2     Data for the command
 */
static void SSD1306i_Cmd3(ssd1306_cmd_t cmd, uint8_t data1, uint8_t data2)
{
    SSD1306i_Cmd(cmd);
    SSD1306i_Cmd(data1);
    SSD1306i_Cmd(data2);
}

void SSD1306_DrawPixel(uint16_t x, uint16_t y, bool value)
{
    uint8_t bit = 1 << (y & 7);
    uint16_t pos = (x + y/8 + 1)/(SSD1306_WIDTH+1);

    if (value) {
        ssd1306i_fbuf[pos] |= bit;
    } else {
        ssd1306i_fbuf[pos] &= ~bit;
    }
}

/**
 * Draw content of internal frame buffer to display
 */
void SSD1306_Flush(void)
{
    uint8_t *addr = ssd1306i_fbuf;
    uint8_t pages = SSD1306_HEIGHT/8;

    SSD1306i_Cmd(SSD1306_START_LINE);

    while (pages--) {
        SSD1306i_Data(addr, SSD1306_WIDTH + 1);
        addr += SSD1306_WIDTH + 1;
    }
}

void SSD1306_DispEnable(bool on)
{
    if (on) {
        SSD1306i_Cmd(SSD1306_DISP_ON);
    } else {
        SSD1306i_Cmd(SSD1306_DISP_OFF);
    }
}

bool SSD1306_Init(void)
{
    uint32_t pos;
    for (pos = 0; pos < sizeof(ssd1306i_fbuf); pos += SSD1306_WIDTH + 1) {
        ssd1306i_fbuf[pos] = 0x40; /* data command */
    }

    IOd_SetLine(LINE_SSD1306_RES, 1);
    delay_ms(20);
    IOd_SetLine(LINE_SSD1306_RES, 0);
    delay_ms(20);

    if (!SSD1306i_Cmd(SSD1306_DISP_OFF)) {
        return false;
    }
    SSD1306i_Cmd2(SSD1306_CLK_DIV, 0x80);
    SSD1306i_Cmd2(SSD1306_MULTIPLEX, SSD1306_HEIGHT - 1);
    SSD1306i_Cmd2(SSD1306_PRECHARGE, 0x1f);
    SSD1306i_Cmd2(SSD1306_DISP_OFFSET, 0);
    SSD1306i_Cmd2(SSD1306_CHARGE_PUMP, 0x14);
    SSD1306i_Cmd2(SSD1306_MEM_MODE, 0); /* horizontal addressing */
    SSD1306i_Cmd(SSD1306_COLSCANDEC);
    SSD1306i_Cmd(SSD1306_ROWSCANDEC);

#if SSD1306_HEIGHT == 64
    SSD1306i_Cmd2(SSD1306_COM_PINS, 0x12);
#elif SSD1306_HEIGHT == 32
    SSD1306i_Cmd2(SSD1306_COM_PINS, 0x02);
#else
    SSD1306i_Cmd2(SSD1306_COM_PINS, 0x22);
#endif

    SSD1306i_Cmd2(SSD1306_VCOM_DETECT, 0x10);
    SSD1306i_Cmd(SSD1306_DISP_ON);
    SSD1306i_Cmd(SSD1306_DISP_NORM);
    SSD1306i_Cmd3(SSD1306_HV_COL_ADDR, 0, SSD1306_WIDTH - 1);
    SSD1306i_Cmd3(SSD1306_HV_PAGE_ADDR, 0, SSD1306_HEIGHT/8 - 1);

    return true;
}

/** @} */
