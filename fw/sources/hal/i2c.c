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
 * @file    hal/i2c.c
 * @brief   I2C driver
 *
 * @addtogroup hal
 * @{
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>

#include "utils/assert.h"
#include "hal/i2c.h"

static const uint32_t i2cdi_regs[] = {
		I2C1,
#ifdef I2C2_BASE
		I2C2,
#endif
#ifdef I2C3_BASE
		I2C3,
#endif
};

static const uint32_t i2cdi_rcc[] = {
		RCC_I2C1,
#ifdef I2C2_BASE
		RCC_I2C2,
#endif
#ifdef I2C3_BASE
		RCC_I2C3,
#endif
};

/**
 * Get I2C device address from device id
 *
 * @param device	Device ID (starts from 1)
 * @return Address of the device's base register
 */
static uint32_t I2Cdi_GetDevice(uint8_t device)
{
	ASSERT_NOT(device == 0 || device > sizeof(i2cdi_regs)/sizeof(i2cdi_regs[0]));
	return i2cdi_regs[device - 1];
}

/**
 * Get I2C device rcc register
 *
 * @param device	Device ID (starts from 1)
 * @return Address of the device's rcc register
 */
static enum rcc_periph_clken I2Cdi_GetRcc(uint8_t device)
{
	ASSERT_NOT(device == 0 || device > sizeof(i2cdi_rcc)/sizeof(i2cdi_rcc[0]));
	return i2cdi_rcc[device - 1];
}

bool I2Cd_Transceive(uint8_t device, uint8_t address, const uint8_t *txbuf,
		uint8_t txlen, uint8_t *rxbuf, uint8_t rxlen)
{
    uint32_t i2c = I2Cdi_GetDevice(device);

    /** modified code from libopencm3 library - infinite loop on nack, wtf?? */
    if (txlen) {
        i2c_set_7bit_address(i2c, address);
        i2c_set_write_transfer_dir(i2c);
        i2c_set_bytes_to_transfer(i2c, txlen);
        if (rxlen) {
            i2c_disable_autoend(i2c);
        } else {
            i2c_enable_autoend(i2c);
        }
        i2c_send_start(i2c);

        while (txlen--) {
            bool wait = true;
            while (wait) {
                if (i2c_transmit_int_status(i2c)) {
                    wait = false;
                }
                if (i2c_nack(i2c)) {
                    return false;
                }
            }
            i2c_send_data(i2c, *txbuf++);
        }
        if (rxlen) {
            /* Wait until last byte was send before sending start again */
            while (!i2c_transfer_complete(i2c)) {
            	;
            }
        }
    }

    if (rxlen) {
        /* Setting transfer properties */
        i2c_set_7bit_address(i2c, address);
        i2c_set_read_transfer_dir(i2c);
        i2c_set_bytes_to_transfer(i2c, rxlen);
        /* start transfer */
        i2c_send_start(i2c);
        /* important to do it afterwards to do a proper repeated start! */
        i2c_enable_autoend(i2c);

        for (size_t i = 0; i < rxlen; i++) {
            while (!i2c_received_data(i2c)) {
            	;
            }
            rxbuf[i] = i2c_get_data(i2c);
        }
    }
    return true;
}

void I2Cd_Init(uint8_t device, bool fast)
{
	enum rcc_periph_clken rcc = I2Cdi_GetRcc(device);
	uint32_t i2c = I2Cdi_GetDevice(device);

    rcc_set_i2c_clock_hsi(i2c);
    rcc_periph_clock_enable(rcc);

    i2c_reset(i2c);
    i2c_peripheral_disable(i2c);
    i2c_enable_analog_filter(i2c);
    i2c_set_digital_filter(i2c, 0);
    if (fast) {
		i2c_set_speed(i2c, i2c_speed_fm_400k, 8);
    } else {
		i2c_set_speed(i2c, i2c_speed_sm_100k, 8);
    }
    i2c_enable_stretching(i2c);
    i2c_set_7bit_addr_mode(i2c);
    i2c_peripheral_enable(i2c);
}

/** @} */
