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
 * @file    drivers/spi_flash.h
 * @brief   Driver for SST26 SPI flash memories
 *
 * @addtogroup drivers
 * @{
 */

#include "utils/assert.h"
#include "hal/io.h"
#include "hal/spi.h"

#include "drivers/spi_flash.h"

#define SPI_DEVICE 1
#define PAGE_BYTES 256

/** status register */
typedef struct {
    uint8_t res: 2;
    uint8_t sec: 1;     /** Security ID status, 1 = locked */
    uint8_t wpld: 1;    /** Write protection lock down, 1 = enabled */
    uint8_t wsp: 1;     /** Write suspend-program status, 1 = suspended */
    uint8_t wse: 1;     /** Write suspend-erase status, 1 = suspended */
    uint8_t wel: 1;     /** Write enable latch status, 1 = write-enabled */
    uint8_t busy: 1;    /** Write operation status, 1 = write in progress */
} spiflash_status_t;

/** configuration register */
typedef struct {
    uint8_t wpen: 1;    /** Write-Protect pin enable, 1 = enabled */
    uint8_t res: 3;
    uint8_t bpnv: 1;    /** Block protection volatility state, 1 no block locked */
    uint8_t res2: 1;
    uint8_t ioc: 1;     /** IO config in SPI, 1 = WP and HOLD pins disabled */
    uint8_t res3: 1;
} spiflash_config_t;

/** Flash memory commands */
typedef enum {
    /* Configuration */
    CMD_NOP = 0x00,
    CMD_RSTEN = 0x66,   /** reset enable */
    CMD_RST = 0x99,     /** reset memory */
    CMD_EQIO = 0x38,    /** enable quad io */
    CMD_RSTQIO = 0xff,  /** reset quad io */
    CMD_RDSR = 0x05,    /** read status reg */
    CMD_WRSR = 0x01,    /** write status reg */
    CMD_RDCR = 0x35,    /** read configuration reg */

    /* read */
    CMD_READ = 0x03,    /** read memory */
    CMD_HSREAD = 0x0b,  /** High speed read */
    CMD_SQOR = 0x6b,    /** QSPI output read */
    CMD_SQIOR = 0xeb,   /** QSPI io read */
    CMD_SDOR = 0x3b,    /** dual spi output read */
    CMD_SDIOR = 0xbb,   /** dual spi io read */
    CMD_SB = 0xc0,      /** set burst length */
    CMD_RBSQI = 0x0c,   /** SQI read burst with wrap */
    CMD_RBSPI = 0xec,   /** SPI read burst with wrap */

    /* Identification */
    CMD_JEDEC = 0x9f,   /** read jedec id */
    CMD_QJID = 0xaf,    /** read quad IO J-ID */
    CMD_SFDP = 0x5a,    /** serial flash discoverable params */

    /* write */
    CMD_WREN = 0x06,    /** write enable */
    CMD_WRDI = 0x04,    /** write_disable */
    CMD_SE = 0x20,      /** Erase 4 kB */
    CMD_BE = 0xd8,      /** Erase 64,32 or 8 k memory */
    CMD_CE = 0xc7,      /** Erase full array */
    CMD_PP = 0x02,      /** Page program */
    CMD_SQPP = 0x32,    /** QSPI page program */
    CMD_WRSU = 0xb0,    /** Suspend program/erase */
    CMD_WRRE = 0x30,    /** Resume program/erase */

    /* protection */
    CMD_RBPR = 0x71,    /** Read block protection reg */
    CMD_WBPR = 0x42,    /** Write block protection reg */
    CMD_LBPR = 0x8d,    /** Lock down block protection reg */
    CMD_NVWLDR = 0xe8,  /** Non volatile write lockdown reg */
    CMD_ULBPR = 0x98,   /** Global block protection unlock */
    CMD_RSID = 0x88,    /** Read security ID */
    CMD_PSID = 0xa5,    /** Program user security ID */
    CMD_LSID = 0x85,    /** Lock out security Id programming */
} spiflash_cmd_t;

/**
 * Drive cs pin to selected level
 *
 * @param val       true for low (selected), false for high
 */
static void SpiFlashi_Cs(bool val)
{
    IOd_SetLine(LINE_FLASH_CS, !val);
}

/**
 * Send command followed by address and optionally also dummy bytes
 *
 * @param cmd       Command
 * @param addr      24 bytes of address
 * @param dummy     Amount (up to 4) of dummy bytes to send after address
 * @param release_cs    Release cs pin after sending data
 */
static void SpiFlashi_CmdWithAddr(spiflash_cmd_t cmd, uint32_t addr,
        uint8_t dummy, bool release_cs)
{
    uint8_t data[8] = {0};

    ASSERT_NOT(dummy > 4);

    data[0] = cmd;
    data[1] = addr >> 2;
    data[2] = addr >> 1;
    data[3] = addr;

    SpiFlashi_Cs(true);
    SPId_Send(SPI_DEVICE, data, 4+dummy);

    if (release_cs) {
        SpiFlashi_Cs(false);
    }
}

static void SpiFlashi_Cmd(spiflash_cmd_t cmd)
{
    SpiFlashi_Cs(true);
    SPId_Send(SPI_DEVICE, &cmd, 1);
    SpiFlashi_Cs(false);
}

void SpiFlash_WriteEnable(void)
{
    SpiFlashi_Cmd(CMD_WREN);
}

void SpiFlash_WriteDisable(void)
{
    SpiFlashi_Cmd(CMD_WRDI);
}


void SpiFlash_Read(uint32_t addr, uint8_t *buf, size_t len)
{
    SpiFlashi_CmdWithAddr(CMD_READ, addr, 0, false);
    SPId_Receive(SPI_DEVICE, buf, len);
    SpiFlashi_Cs(false);
}

void SpiFlash_Write(uint32_t addr, const uint8_t *buf, size_t len)
{
    uint16_t bytes;
    size_t offset = 0;

    /* up to 256 bytes in single write */
    while (len != 0) {
        if (len >= PAGE_BYTES) {
            bytes = PAGE_BYTES;
        } else {
            bytes = len;
        }
        SpiFlashi_CmdWithAddr(CMD_PP, addr, 0, false);
        SPId_Send(SPI_DEVICE, buf, bytes);
        SpiFlashi_Cs(false);

        buf += bytes;
        addr += bytes;
        len -= bytes;
    }
}

void SpiFlash_Erase(void)
{
    SpiFlashi_Cmd(CMD_CE);
}

void SpiFlash_EraseSector(uint32_t addr)
{
    SpiFlashi_CmdWithAddr(CMD_SE, addr, 0, true);
}

/** @} */
