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
 * @file    app/logger/main.c
 * @brief   Main file for GLogger app
 *
 * @addtogroup utils
 * @{
 */
#include <libopencm3/stm32/rcc.h>

#include <hal/io.h>
#include <hal/i2c.h>
#include <hal/spi.h>
#include <hal/uart.h>
#include <hal/wdg.h>
#include <hal/rtc.h>
#include <modules/log.h>
#include <modules/uf2.h>
#include <drivers/spi_flash.h>
#include <drivers/ssd1306.h>
#include <drivers/gps.h>
#include <utils/time.h>
#include <utils/button.h>
#include <modules/ramdisk.h>
#include "storage.h"
#include "stats.h"
#include "usb.h"
#include "gui/gui.h"
#include "gui/gui.h"
#include "utils/assert.h"
#include "version.h"

gps_desc_t gps_desc;
spiflash_desc_t spiflash_desc;
ssd1306_desc_t ssd1306_desc;

/**
 * Configure system clock
 *
 * External 16 MHz used to generate precise 48 MHz for USB
 * System clock running directly from crystal (16 MHz)
 */
static void setClock(void)
{
    /* Enable crystal oscillator and use it as a system clock */
    rcc_osc_on(RCC_HSE);
    rcc_wait_for_osc_ready(RCC_HSE);
    rcc_set_sysclk_source(RCC_HSE);

    rcc_apb1_frequency = 16000000;
    rcc_ahb_frequency = 16000000;

    /* No prescalers for system clock and peripherals */
    rcc_set_hpre(RCC_CFGR_HPRE_NODIV);
    rcc_set_ppre(RCC_CFGR_PPRE_NODIV);

    /* Setup PLL to generate 48 MHz for usb */
    rcc_set_pll_multiplication_factor(RCC_CFGR_PLLMUL_MUL6);
    rcc_set_pll_source(RCC_CFGR_PLLSRC_HSE_CLK);
    rcc_set_prediv(RCC_CFGR2_PREDIV_DIV2);
    rcc_osc_on(RCC_PLL);
    rcc_wait_for_osc_ready(RCC_PLL);
    rcc_set_usbclk_source(RCC_PLL);
}

static void addReadme(void)
{
    const char *readme = "GLogger gps logger by deadbadger.cz, for more info "
            "check out deadbadger.cz/projects/glogger.";

    Ramdisk_AddTextFile("README", "TXT", 0, readme);
}

static void btnCheck(void)
{
    static button_t bt_next = { LINE_SW_NEXT, };
    static button_t bt_enter = { LINE_SW_ENTER, };
    button_event_t event;

    event = Button(&bt_next);
    if (event == BTN_RELEASED_SHORT) {
        Gui_Event(GUI_EVT_SHORT_NEXT);
    } else if (event == BTN_LONG_PRESS) {
        Gui_Event(GUI_EVT_LONG_NEXT);
    }

    event = Button(&bt_enter);
    if (event == BTN_RELEASED_SHORT) {
        Gui_Event(GUI_EVT_SHORT_ENTER);
    } else if (event == BTN_LONG_PRESS) {
        //TODO poweroff
    }
}

static void loop(void)
{
    uint32_t time = millis();
    const gps_info_t *gps;

    // turn on the gps
    // while gps not valid
        // sleep until woken up by interrupt, process nema, if not complete, sleep
    //store gps position
    //turn off the gps
    //go to stop mode, wait for rtc to wake mcu up

    //wake up sources - buttons in on mode, holding button in off mode

    //read buttons, if pressed, turn on display, set turn off timer

    gps = Gps_Loop(&gps_desc);
    if (gps != NULL) {
        //TODO verify target has moved since last gps fix
        Stats_Update(gps);
        Storage_Add(gps);
        Gui_Event(GUI_EVT_REDRAW);
    }

    if (time % 5 == 0) {
        btnCheck();
    }
}

static void fw_read(uint32_t offset, uint8_t *buf, size_t len)
{
    (void)len;
    UF2_Read(buf, offset/512);
}

static void ramdisk_write(const uint8_t *buf, size_t size, uint32_t offset)
{
    (void)size;
    (void)offset;
    UF2_Write(buf);
}

int main(void)
{
    static uint8_t fbuf[SSD1306_FBUF_SIZE];
    /* Initialize clock system, IO pins and systick */
    setClock();
    IOd_Init();
    Time_Init();

    /* early init - debug output */
    UARTd_Init(USART_DEBUG_TX, 115200);
    Log_Init(USART_DEBUG_TX);
    Log_SetLevel(LOG_DEBUG);
    Log_Info(NULL, "GLogger, fw version %d.%d", FW_MAJOR, FW_MINOR);
    Log_Info(NULL, "Deadbadger.cz");

    I2Cd_Init(1, true);
    SPId_Init(1, SPID_PRESC_2, SPI_MODE_0);
    UARTd_Init(USART_GPS_TX, 9600);
    RTCd_Init(false);

    if (SSD1306_Init(&ssd1306_desc, fbuf, 1,
            SSD1306_ADDR_0, LINE_SSD1306_RES) == false) {
        Log_Error("I2C", "Failed to initialize display driver");
    } else {
        SSD1306_SetOrientation(&ssd1306_desc, true);
        SSD1306_DispEnable(&ssd1306_desc, true);
    }
    Gui_Init();

    Gps_Init(&gps_desc, USART_GPS_RX);
    SpiFlash_Init(&spiflash_desc, 1, LINE_FLASH_CS);
    SpiFlash_WriteUnlock(&spiflash_desc);
    Storage_Init();
    //Stats_Init();

    Ramdisk_Init(64000000, "GLogger");
    Ramdisk_RegisterWriteCb(ramdisk_write);
    addReadme();
    Ramdisk_AddFile("fw", "uf2", 0, UF2_GetImgSize(), fw_read);
    Usb_Init();

    Gui_Event(GUI_EVT_REDRAW);
    Log_Info(NULL, "System initialized, running main loop");
    while (1) {
        loop();
        Usb_Poll();
    }
}

/** @} */
