
#include <hal/io.h>
#include <hal/i2c.h>
#include <hal/spi.h>
#include <hal/uart.h>
#include <hal/wdg.h>

#include <drivers/spi_flash.h>
#include <drivers/ssd1306.h>
#include <drivers/gps.h>
#include <utils/time.h>

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

    while (1) {
        (void) Gps_Get();
    }
}

/** @} */
