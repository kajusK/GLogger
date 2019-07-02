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
 * @file    hal/usb_msc.h
 * @brief   USB Mass storage class setup
 *
 * @addtogroup hal
 * @{
 */

#include <stdlib.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/msc.h>

#include "hal/usb_msc.h"

/** Device descriptor - https://www.keil.com/pack/doc/mw/USB/html/_u_s_b__device__descriptor.html */
static const struct usb_device_descriptor usb_dev_descr = {
    .bLength = USB_DT_DEVICE_SIZE,
    .bDescriptorType = USB_DT_DEVICE,
    .bcdUSB = 0x0110,
    .bDeviceClass = 0,
    .bDeviceSubClass = 0,
    .bDeviceProtocol = 0,
    .bMaxPacketSize0 = 64,
    .idVendor = 0x0483, /* STMicroelectronics */
    .idProduct = 0x5720, /* Mass storage device */
    .bcdDevice = 0x0001,
    .iManufacturer = 1,
    .iProduct = 2,
    .iSerialNumber = 3,
    .bNumConfigurations = 1,
};

/** USB strings descriptors, order in usb_dev_descr - iManufacturer,... */
static const char *usb_strings[] = {
    "Deadbadger",
    "GLogger",
    "0000",
};

/** https://www.keil.com/pack/doc/mw/USB/html/_u_s_b__endpoint__descriptor.html */
static const struct usb_endpoint_descriptor usb_msc_endp[] = {{
    .bLength = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType = USB_DT_ENDPOINT,
    .bEndpointAddress = 0x01,
    .bmAttributes = USB_ENDPOINT_ATTR_BULK,
    .wMaxPacketSize = 64,
    .bInterval = 0,
}, {
    .bLength = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType = USB_DT_ENDPOINT,
    .bEndpointAddress = 0x82,
    .bmAttributes = USB_ENDPOINT_ATTR_BULK,
    .wMaxPacketSize = 64,
    .bInterval = 0,
}};

/** https://www.keil.com/pack/doc/mw/USB/html/_u_s_b__interface__descriptor.html */
static const struct usb_interface_descriptor usb_msc_iface[] = {{
    .bLength = USB_DT_INTERFACE_SIZE,
    .bDescriptorType = USB_DT_INTERFACE,
    .bInterfaceNumber = 0,
    .bAlternateSetting = 0,
    .bNumEndpoints = 2,
    .bInterfaceClass = USB_CLASS_MSC,
    .bInterfaceSubClass = USB_MSC_SUBCLASS_SCSI,
    .bInterfaceProtocol = USB_MSC_PROTOCOL_BBB,
    .iInterface = 0,
    /* Not part of standard, used by libopencm3 */
    .endpoint = usb_msc_endp,
    .extra = NULL,
    .extralen = 0
}};

static const struct usb_interface usb_ifaces[] = {{
    .num_altsetting = 1,
    .altsetting = usb_msc_iface,
}};

/** https://www.keil.com/pack/doc/mw/USB/html/_u_s_b__configuration__descriptor.html */
static const struct usb_config_descriptor usb_config_descr = {
    .bLength = USB_DT_CONFIGURATION_SIZE,
    .bDescriptorType = USB_DT_CONFIGURATION,
    .wTotalLength = 0,
    .bNumInterfaces = 1,
    .bConfigurationValue = 1,
    .iConfiguration = 0,
    .bmAttributes = 0x80,
    .bMaxPower = 0x32,
    /* not part of standard */
    .interface = usb_ifaces,
};

static usbd_device *usb_msc_dev;
/* Buffer to be used for control requests. */
static uint8_t usb_control_buffer[128];

void Usbd_MscPool(void)
{
    usbd_poll(usb_msc_dev);
}

void Usbd_MscInit(uint32_t blocks, usb_read_block_t read,
        usb_write_block_t write)
{
    rcc_periph_clock_enable(RCC_USB);
    usb_msc_dev = usbd_init(&st_usbfs_v1_usb_driver, &usb_dev_descr,
                &usb_config_descr, usb_strings, 3,
                usb_control_buffer, sizeof(usb_control_buffer));

    usb_msc_init(usb_msc_dev, 0x82, 64, 0x01, 64, "Deadbadg", "GLogger",
        "0.00", blocks, read, write);
}

/** @} */
