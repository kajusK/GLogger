
#include <hal/io.h>
#include <hal/i2c.h>
#include <hal/spi.h>
#include <hal/uart.h>
#include <hal/wdg.h>

#include <utils/time.h>

int main(void)
{
    IOd_Init();
    I2Cd_Init(1, true);
    SPId_Init(1, SPID_PRESC_8, SPI_MODE_0);
    UARTd_Init(2, 9600);

    Time_Init();
}

/** @} */
