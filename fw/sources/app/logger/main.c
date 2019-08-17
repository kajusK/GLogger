
#include <hal/io.h>
#include <hal/i2c.h>
#include <hal/spi.h>
#include <hal/uart.h>
#include <hal/wdg.h>
#include <hal/usb_msc.h>

#include <drivers/spi_flash.h>
#include <drivers/ssd1306.h>
#include <drivers/gps.h>
#include <utils/time.h>
#include <drivers/ramdisk.h>
#include "storage.h"
#include "stats.h"

static void addReadme(void)
{
    const char *readme = "GLogger gps logger by deadbadger.cz, for more info"
            "check out deadbadger.cz/projects/glogger.";

    Ramdisk_AddTextFile("README", "TXT", 0, readme);
}

static void loop(void)
{
    gps_info_t *gps;

    gps = Gps_Get();
    if (gps) {
        Stats_Update(gps);
        if (gps->altitude_dm != 0 && gps->timestamp != 0) {
            Storage_Add(gps);
        }
    }
}

int main(void)
{
    //TODO initialize clock - 48MHz for usb with clock synchronization */

    IOd_Init();
    I2Cd_Init(1, true);
    SPId_Init(1, SPID_PRESC_8, SPI_MODE_0);
    UARTd_Init(USART_GPS_TX, 9600);
    Time_Init();

    Gps_Init();
    SpiFlash_WriteEnable();
    SSD1306_Init();
    Storage_Init();
    Stats_Init();
    Gui_Init();
    Ramdisk_Init(64000000, "GLogger");
    Usbd_MscInit(Ramdisk_GetSectors(), Ramdisk_Read, Ramdisk_Write);

    addReadme();

    while (1) {
        loop();
        Usbd_MscPool();
    }
}

/** @} */
