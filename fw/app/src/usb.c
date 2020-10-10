/*
 * Copyright (C) 2020 Jakub Kaderka
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
 * @file    app/usb.c
 * @brief   USB configuration, file descriptors, etc.
 *
 * @addtogroup app
 * @{
 */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/msc.h>
#include <modules/msc.h>
#include <modules/ramdisk.h>
#include "config.h"
#include "usb.h"


/**
 *  Device descriptor - top level descriptor
 *  https://www.keil.com/pack/doc/mw/USB/html/_u_s_b__device__descriptor.html
 */
static const struct usb_device_descriptor usbi_dev_descr = {
    .bLength = USB_DT_DEVICE_SIZE,      //Length of this descriptor
    .bDescriptorType = USB_DT_DEVICE,   //This is device descriptor
    .bcdUSB = 0x0200,                   //USB version 2.0

    .bDeviceClass = 0,                  //USB class
    .bDeviceSubClass = 0,
    .bDeviceProtocol = 0,
    .bMaxPacketSize0 = 64,              //USB packet size
    .idVendor = USB_VENDOR,             //VID
    .idProduct = USB_PRODUCT,           //PID
    .bcdDevice = 0x0200,                //Device release 2.0
    .iManufacturer = 1,                 //Name of manufacturer (index of string descriptor)
    .iProduct = 2,                      //Name of product (index)
    .iSerialNumber = 3,                 //Serial number (index)
    .bNumConfigurations = 1,            //How many configurations we support
};

/** USB strings descriptors, order in usb_dev_descr - iManufacturer,... */
static const char * const usbi_strings[] = {
     USB_MANUFACTURE_STR,
     USB_DEVICE_STR,
     USB_VERSION_STR,
};

/** https://www.keil.com/pack/doc/mw/USB/html/_u_s_b__endpoint__descriptor.html */
static const struct usb_endpoint_descriptor usbi_msc_endp[] = {{
    .bLength = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType = USB_DT_ENDPOINT,
    .bEndpointAddress = 0x01,   //IN endpoint number 1
    .bmAttributes = USB_ENDPOINT_ATTR_BULK,
    .wMaxPacketSize = 64,
    .bInterval = 0,
}, {
    .bLength = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType = USB_DT_ENDPOINT,
    .bEndpointAddress = 0x82,   //OUT endpoint, number 2 (0x80 for OUT)
    .bmAttributes = USB_ENDPOINT_ATTR_BULK,
    .wMaxPacketSize = 64,
    .bInterval = 0,
}};

/** https://www.keil.com/pack/doc/mw/USB/html/_u_s_b__interface__descriptor.html */
static const struct usb_interface_descriptor usbi_msc_iface[] = {{
    .bLength = USB_DT_INTERFACE_SIZE,
    .bDescriptorType = USB_DT_INTERFACE,
    .bInterfaceNumber = 0,              //Interface order, this is first one
    .bAlternateSetting = 0,
    .bNumEndpoints = 2,                 //The interface has 2 endpoints
    .bInterfaceClass = USB_CLASS_MSC,   //Mass Storage Class
    .bInterfaceSubClass = USB_MSC_SUBCLASS_SCSI,    //SCSI command set
    .bInterfaceProtocol = USB_MSC_PROTOCOL_BBB,     //Bulk only over 2 endpoints
    .iInterface = 0,    //Name of interface (0 for none)
    /* Not part of standard, used by libopencm3 */
    .endpoint = usbi_msc_endp,
    .extra = NULL,
    .extralen = 0
}};

static const struct usb_interface usbi_ifaces[] = {{
    .num_altsetting = 1,
    .altsetting = usbi_msc_iface,
}};

/**
 * Configuration descriptor
 * Contains list of interfaces and expected power supplied to device
 * https://www.keil.com/pack/doc/mw/USB/html/_u_s_b__configuration__descriptor.html
 */
static const struct usb_config_descriptor usbi_config_descr = {
    .bLength = USB_DT_CONFIGURATION_SIZE,   //Length of this descriptor
    .bDescriptorType = USB_DT_CONFIGURATION,//Configuration descriptor
    .wTotalLength = 0,
    .bNumInterfaces = 1,                    //Number of interfaces supported
    .bConfigurationValue = 1,               //This is config ID 1
    .iConfiguration = 0,                    //Name of config (0 = no name)
    .bmAttributes = 0x80,                   //Bus powered
    .bMaxPower = 0x32,                      //Max power in 2mA units (100 mA)
    /* not part of standard */
    .interface = usbi_ifaces,                //List of interfaces
};

static usbd_device *usbi_dev;
/* Buffer to be used for control requests. */
static uint8_t usbi_control_buffer[128];

void Usb_Poll(void)
{
    usbd_poll(usbi_dev);
}

void Usb_Init(void)
{
    rcc_periph_clock_enable(RCC_USB);

    usbi_dev = usbd_init(&st_usbfs_v2_usb_driver, &usbi_dev_descr,
                &usbi_config_descr, usbi_strings,
                sizeof(usbi_strings)/sizeof(usbi_strings[0]),
                usbi_control_buffer, sizeof(usbi_control_buffer));

    Msc_Init(usbi_dev, 0x82, 64, 0x01, 64, USB_MANUFACTURE_STR,
            USB_DEVICE_STR, USB_VERSION_STR, Ramdisk_Read, Ramdisk_Write,
            Ramdisk_GetSectors());
}

/** @} */
