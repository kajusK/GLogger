#ifndef _BOARD_GPIO_H_
#define _BOARD_GPIO_H_

/* Generated by mx2board.py */


#define I2C_I2C1_SCL 1
#define I2C_I2C1_SDA 1
#define LINE_BT_CHECK PAL_LINE(GPIOB, 1U)
#define LINE_I2C1_SCL PAL_LINE(GPIOF, 1U)
#define LINE_I2C1_SDA PAL_LINE(GPIOF, 0U)
#define LINE_SPI1_MISO PAL_LINE(GPIOA, 6U)
#define LINE_SPI1_MOSI PAL_LINE(GPIOA, 7U)
#define LINE_SPI1_SCK PAL_LINE(GPIOA, 5U)
#define LINE_SYS_SWCLK PAL_LINE(GPIOA, 14U)
#define LINE_SYS_SWDIO PAL_LINE(GPIOA, 13U)
#define LINE_USART2_RX PAL_LINE(GPIOA, 3U)
#define LINE_USART2_TX PAL_LINE(GPIOA, 2U)
#define LINE_USB_DM PAL_LINE(GPIOA, 11U)
#define LINE_USB_DP PAL_LINE(GPIOA, 12U)
#define PAD_BT_CHECK 1
#define PAD_I2C1_SCL 1
#define PAD_I2C1_SDA 0
#define PAD_SPI1_MISO 6
#define PAD_SPI1_MOSI 7
#define PAD_SPI1_SCK 5
#define PAD_SYS_SWCLK 14
#define PAD_SYS_SWDIO 13
#define PAD_USART2_RX 3
#define PAD_USART2_TX 2
#define PAD_USB_DM 11
#define PAD_USB_DP 12
#define PORT_BT_CHECK GPIOB
#define PORT_I2C1_SCL GPIOF
#define PORT_I2C1_SDA GPIOF
#define PORT_SPI1_MISO GPIOA
#define PORT_SPI1_MOSI GPIOA
#define PORT_SPI1_SCK GPIOA
#define PORT_SYS_SWCLK GPIOA
#define PORT_SYS_SWDIO GPIOA
#define PORT_USART2_RX GPIOA
#define PORT_USART2_TX GPIOA
#define PORT_USB_DM GPIOA
#define PORT_USB_DP GPIOA
#define SPI_SPI1_MISO 1
#define SPI_SPI1_MOSI 1
#define SPI_SPI1_SCK 1
#define USART_USART2_RX 2
#define USART_USART2_TX 2


/* PORT A */
#define VAL_GPIOA_MODER ( \
    PIN_MODE_ANALOG(0) | \
    PIN_MODE_ANALOG(1) | \
    PIN_MODE_ALTERNATE(2) | \
    PIN_MODE_ALTERNATE(3) | \
    PIN_MODE_ANALOG(4) | \
    PIN_MODE_ALTERNATE(5) | \
    PIN_MODE_ALTERNATE(6) | \
    PIN_MODE_ALTERNATE(7) | \
    PIN_MODE_ANALOG(8) | \
    PIN_MODE_ANALOG(9) | \
    PIN_MODE_ANALOG(10) | \
    PIN_MODE_ALTERNATE(11) | \
    PIN_MODE_ALTERNATE(12) | \
    PIN_MODE_ALTERNATE(13) | \
    PIN_MODE_ALTERNATE(14) | \
    PIN_MODE_ANALOG(15) | \
    0)

#define VAL_GPIOA_OTYPER ( \
    PIN_OTYPE_PUSHPULL(0) | \
    PIN_OTYPE_PUSHPULL(1) | \
    PIN_OTYPE_PUSHPULL(2) | \
    PIN_OTYPE_PUSHPULL(3) | \
    PIN_OTYPE_PUSHPULL(4) | \
    PIN_OTYPE_PUSHPULL(5) | \
    PIN_OTYPE_PUSHPULL(6) | \
    PIN_OTYPE_PUSHPULL(7) | \
    PIN_OTYPE_PUSHPULL(8) | \
    PIN_OTYPE_PUSHPULL(9) | \
    PIN_OTYPE_PUSHPULL(10) | \
    PIN_OTYPE_PUSHPULL(11) | \
    PIN_OTYPE_PUSHPULL(12) | \
    PIN_OTYPE_PUSHPULL(13) | \
    PIN_OTYPE_PUSHPULL(14) | \
    PIN_OTYPE_PUSHPULL(15) | \
    0)

#define VAL_GPIOA_OSPEEDR ( \
    PIN_OSPEED_VERYLOW(0) | \
    PIN_OSPEED_VERYLOW(1) | \
    PIN_OSPEED_MEDIUM(2) | \
    PIN_OSPEED_MEDIUM(3) | \
    PIN_OSPEED_VERYLOW(4) | \
    PIN_OSPEED_MEDIUM(5) | \
    PIN_OSPEED_MEDIUM(6) | \
    PIN_OSPEED_MEDIUM(7) | \
    PIN_OSPEED_VERYLOW(8) | \
    PIN_OSPEED_VERYLOW(9) | \
    PIN_OSPEED_VERYLOW(10) | \
    PIN_OSPEED_MEDIUM(11) | \
    PIN_OSPEED_MEDIUM(12) | \
    PIN_OSPEED_MEDIUM(13) | \
    PIN_OSPEED_MEDIUM(14) | \
    PIN_OSPEED_VERYLOW(15) | \
    0)

#define VAL_GPIOA_PUPDR ( \
    PIN_PUPDR_PULLUP(0) | \
    PIN_PUPDR_PULLUP(1) | \
    PIN_PUPDR_PULLUP(2) | \
    PIN_PUPDR_PULLUP(3) | \
    PIN_PUPDR_PULLUP(4) | \
    PIN_PUPDR_PULLUP(5) | \
    PIN_PUPDR_PULLUP(6) | \
    PIN_PUPDR_PULLUP(7) | \
    PIN_PUPDR_PULLUP(8) | \
    PIN_PUPDR_PULLUP(9) | \
    PIN_PUPDR_PULLUP(10) | \
    PIN_PUPDR_PULLUP(11) | \
    PIN_PUPDR_PULLUP(12) | \
    PIN_PUPDR_PULLUP(13) | \
    PIN_PUPDR_PULLUP(14) | \
    PIN_PUPDR_PULLUP(15) | \
    0)

#define VAL_GPIOA_ODR ( \
    PIN_ODR_LOW(0) | \
    PIN_ODR_LOW(1) | \
    PIN_ODR_LOW(2) | \
    PIN_ODR_LOW(3) | \
    PIN_ODR_LOW(4) | \
    PIN_ODR_LOW(5) | \
    PIN_ODR_LOW(6) | \
    PIN_ODR_LOW(7) | \
    PIN_ODR_LOW(8) | \
    PIN_ODR_LOW(9) | \
    PIN_ODR_LOW(10) | \
    PIN_ODR_LOW(11) | \
    PIN_ODR_LOW(12) | \
    PIN_ODR_LOW(13) | \
    PIN_ODR_LOW(14) | \
    PIN_ODR_LOW(15) | \
    0)

#define VAL_GPIOA_AFRL ( \
    PIN_AFIO_AF(0, 0) | \
    PIN_AFIO_AF(1, 0) | \
    PIN_AFIO_AF(2, 1) | \
    PIN_AFIO_AF(3, 1) | \
    PIN_AFIO_AF(4, 0) | \
    PIN_AFIO_AF(5, 0) | \
    PIN_AFIO_AF(6, 0) | \
    PIN_AFIO_AF(7, 0) | \
    0)

#define VAL_GPIOA_AFRH ( \
    PIN_AFIO_AF(8, 0) | \
    PIN_AFIO_AF(9, 0) | \
    PIN_AFIO_AF(10, 0) | \
    PIN_AFIO_AF(11, 0) | \
    PIN_AFIO_AF(12, 0) | \
    PIN_AFIO_AF(13, 0) | \
    PIN_AFIO_AF(14, 0) | \
    PIN_AFIO_AF(15, 0) | \
    0)

/* PORT B */
#define VAL_GPIOB_MODER ( \
    PIN_MODE_ANALOG(0) | \
    PIN_MODE_INPUT(1) | \
    PIN_MODE_ANALOG(2) | \
    PIN_MODE_ANALOG(3) | \
    PIN_MODE_ANALOG(4) | \
    PIN_MODE_ANALOG(5) | \
    PIN_MODE_ANALOG(6) | \
    PIN_MODE_ANALOG(7) | \
    PIN_MODE_ANALOG(8) | \
    PIN_MODE_ANALOG(9) | \
    PIN_MODE_ANALOG(10) | \
    PIN_MODE_ANALOG(11) | \
    PIN_MODE_ANALOG(12) | \
    PIN_MODE_ANALOG(13) | \
    PIN_MODE_ANALOG(14) | \
    PIN_MODE_ANALOG(15) | \
    0)

#define VAL_GPIOB_OTYPER ( \
    PIN_OTYPE_PUSHPULL(0) | \
    PIN_OTYPE_PUSHPULL(1) | \
    PIN_OTYPE_PUSHPULL(2) | \
    PIN_OTYPE_PUSHPULL(3) | \
    PIN_OTYPE_PUSHPULL(4) | \
    PIN_OTYPE_PUSHPULL(5) | \
    PIN_OTYPE_PUSHPULL(6) | \
    PIN_OTYPE_PUSHPULL(7) | \
    PIN_OTYPE_PUSHPULL(8) | \
    PIN_OTYPE_PUSHPULL(9) | \
    PIN_OTYPE_PUSHPULL(10) | \
    PIN_OTYPE_PUSHPULL(11) | \
    PIN_OTYPE_PUSHPULL(12) | \
    PIN_OTYPE_PUSHPULL(13) | \
    PIN_OTYPE_PUSHPULL(14) | \
    PIN_OTYPE_PUSHPULL(15) | \
    0)

#define VAL_GPIOB_OSPEEDR ( \
    PIN_OSPEED_VERYLOW(0) | \
    PIN_OSPEED_VERYLOW(1) | \
    PIN_OSPEED_VERYLOW(2) | \
    PIN_OSPEED_VERYLOW(3) | \
    PIN_OSPEED_VERYLOW(4) | \
    PIN_OSPEED_VERYLOW(5) | \
    PIN_OSPEED_VERYLOW(6) | \
    PIN_OSPEED_VERYLOW(7) | \
    PIN_OSPEED_VERYLOW(8) | \
    PIN_OSPEED_VERYLOW(9) | \
    PIN_OSPEED_VERYLOW(10) | \
    PIN_OSPEED_VERYLOW(11) | \
    PIN_OSPEED_VERYLOW(12) | \
    PIN_OSPEED_VERYLOW(13) | \
    PIN_OSPEED_VERYLOW(14) | \
    PIN_OSPEED_VERYLOW(15) | \
    0)

#define VAL_GPIOB_PUPDR ( \
    PIN_PUPDR_PULLUP(0) | \
    PIN_PUPDR_PULLUP(1) | \
    PIN_PUPDR_PULLUP(2) | \
    PIN_PUPDR_PULLUP(3) | \
    PIN_PUPDR_PULLUP(4) | \
    PIN_PUPDR_PULLUP(5) | \
    PIN_PUPDR_PULLUP(6) | \
    PIN_PUPDR_PULLUP(7) | \
    PIN_PUPDR_PULLUP(8) | \
    PIN_PUPDR_PULLUP(9) | \
    PIN_PUPDR_PULLUP(10) | \
    PIN_PUPDR_PULLUP(11) | \
    PIN_PUPDR_PULLUP(12) | \
    PIN_PUPDR_PULLUP(13) | \
    PIN_PUPDR_PULLUP(14) | \
    PIN_PUPDR_PULLUP(15) | \
    0)

#define VAL_GPIOB_ODR ( \
    PIN_ODR_LOW(0) | \
    PIN_ODR_LOW(1) | \
    PIN_ODR_LOW(2) | \
    PIN_ODR_LOW(3) | \
    PIN_ODR_LOW(4) | \
    PIN_ODR_LOW(5) | \
    PIN_ODR_LOW(6) | \
    PIN_ODR_LOW(7) | \
    PIN_ODR_LOW(8) | \
    PIN_ODR_LOW(9) | \
    PIN_ODR_LOW(10) | \
    PIN_ODR_LOW(11) | \
    PIN_ODR_LOW(12) | \
    PIN_ODR_LOW(13) | \
    PIN_ODR_LOW(14) | \
    PIN_ODR_LOW(15) | \
    0)

#define VAL_GPIOB_AFRL ( \
    PIN_AFIO_AF(0, 0) | \
    PIN_AFIO_AF(1, 0) | \
    PIN_AFIO_AF(2, 0) | \
    PIN_AFIO_AF(3, 0) | \
    PIN_AFIO_AF(4, 0) | \
    PIN_AFIO_AF(5, 0) | \
    PIN_AFIO_AF(6, 0) | \
    PIN_AFIO_AF(7, 0) | \
    0)

#define VAL_GPIOB_AFRH ( \
    PIN_AFIO_AF(8, 0) | \
    PIN_AFIO_AF(9, 0) | \
    PIN_AFIO_AF(10, 0) | \
    PIN_AFIO_AF(11, 0) | \
    PIN_AFIO_AF(12, 0) | \
    PIN_AFIO_AF(13, 0) | \
    PIN_AFIO_AF(14, 0) | \
    PIN_AFIO_AF(15, 0) | \
    0)

/* PORT C */
#define VAL_GPIOC_MODER ( \
    PIN_MODE_ANALOG(0) | \
    PIN_MODE_ANALOG(1) | \
    PIN_MODE_ANALOG(2) | \
    PIN_MODE_ANALOG(3) | \
    PIN_MODE_ANALOG(4) | \
    PIN_MODE_ANALOG(5) | \
    PIN_MODE_ANALOG(6) | \
    PIN_MODE_ANALOG(7) | \
    PIN_MODE_ANALOG(8) | \
    PIN_MODE_ANALOG(9) | \
    PIN_MODE_ANALOG(10) | \
    PIN_MODE_ANALOG(11) | \
    PIN_MODE_ANALOG(12) | \
    PIN_MODE_ANALOG(13) | \
    PIN_MODE_ANALOG(14) | \
    PIN_MODE_ANALOG(15) | \
    0)

#define VAL_GPIOC_OTYPER ( \
    PIN_OTYPE_PUSHPULL(0) | \
    PIN_OTYPE_PUSHPULL(1) | \
    PIN_OTYPE_PUSHPULL(2) | \
    PIN_OTYPE_PUSHPULL(3) | \
    PIN_OTYPE_PUSHPULL(4) | \
    PIN_OTYPE_PUSHPULL(5) | \
    PIN_OTYPE_PUSHPULL(6) | \
    PIN_OTYPE_PUSHPULL(7) | \
    PIN_OTYPE_PUSHPULL(8) | \
    PIN_OTYPE_PUSHPULL(9) | \
    PIN_OTYPE_PUSHPULL(10) | \
    PIN_OTYPE_PUSHPULL(11) | \
    PIN_OTYPE_PUSHPULL(12) | \
    PIN_OTYPE_PUSHPULL(13) | \
    PIN_OTYPE_PUSHPULL(14) | \
    PIN_OTYPE_PUSHPULL(15) | \
    0)

#define VAL_GPIOC_OSPEEDR ( \
    PIN_OSPEED_VERYLOW(0) | \
    PIN_OSPEED_VERYLOW(1) | \
    PIN_OSPEED_VERYLOW(2) | \
    PIN_OSPEED_VERYLOW(3) | \
    PIN_OSPEED_VERYLOW(4) | \
    PIN_OSPEED_VERYLOW(5) | \
    PIN_OSPEED_VERYLOW(6) | \
    PIN_OSPEED_VERYLOW(7) | \
    PIN_OSPEED_VERYLOW(8) | \
    PIN_OSPEED_VERYLOW(9) | \
    PIN_OSPEED_VERYLOW(10) | \
    PIN_OSPEED_VERYLOW(11) | \
    PIN_OSPEED_VERYLOW(12) | \
    PIN_OSPEED_VERYLOW(13) | \
    PIN_OSPEED_VERYLOW(14) | \
    PIN_OSPEED_VERYLOW(15) | \
    0)

#define VAL_GPIOC_PUPDR ( \
    PIN_PUPDR_PULLUP(0) | \
    PIN_PUPDR_PULLUP(1) | \
    PIN_PUPDR_PULLUP(2) | \
    PIN_PUPDR_PULLUP(3) | \
    PIN_PUPDR_PULLUP(4) | \
    PIN_PUPDR_PULLUP(5) | \
    PIN_PUPDR_PULLUP(6) | \
    PIN_PUPDR_PULLUP(7) | \
    PIN_PUPDR_PULLUP(8) | \
    PIN_PUPDR_PULLUP(9) | \
    PIN_PUPDR_PULLUP(10) | \
    PIN_PUPDR_PULLUP(11) | \
    PIN_PUPDR_PULLUP(12) | \
    PIN_PUPDR_PULLUP(13) | \
    PIN_PUPDR_PULLUP(14) | \
    PIN_PUPDR_PULLUP(15) | \
    0)

#define VAL_GPIOC_ODR ( \
    PIN_ODR_LOW(0) | \
    PIN_ODR_LOW(1) | \
    PIN_ODR_LOW(2) | \
    PIN_ODR_LOW(3) | \
    PIN_ODR_LOW(4) | \
    PIN_ODR_LOW(5) | \
    PIN_ODR_LOW(6) | \
    PIN_ODR_LOW(7) | \
    PIN_ODR_LOW(8) | \
    PIN_ODR_LOW(9) | \
    PIN_ODR_LOW(10) | \
    PIN_ODR_LOW(11) | \
    PIN_ODR_LOW(12) | \
    PIN_ODR_LOW(13) | \
    PIN_ODR_LOW(14) | \
    PIN_ODR_LOW(15) | \
    0)

#define VAL_GPIOC_AFRL ( \
    PIN_AFIO_AF(0, 0) | \
    PIN_AFIO_AF(1, 0) | \
    PIN_AFIO_AF(2, 0) | \
    PIN_AFIO_AF(3, 0) | \
    PIN_AFIO_AF(4, 0) | \
    PIN_AFIO_AF(5, 0) | \
    PIN_AFIO_AF(6, 0) | \
    PIN_AFIO_AF(7, 0) | \
    0)

#define VAL_GPIOC_AFRH ( \
    PIN_AFIO_AF(8, 0) | \
    PIN_AFIO_AF(9, 0) | \
    PIN_AFIO_AF(10, 0) | \
    PIN_AFIO_AF(11, 0) | \
    PIN_AFIO_AF(12, 0) | \
    PIN_AFIO_AF(13, 0) | \
    PIN_AFIO_AF(14, 0) | \
    PIN_AFIO_AF(15, 0) | \
    0)

/* PORT F */
#define VAL_GPIOF_MODER ( \
    PIN_MODE_ALTERNATE(0) | \
    PIN_MODE_ALTERNATE(1) | \
    PIN_MODE_ANALOG(2) | \
    PIN_MODE_ANALOG(3) | \
    PIN_MODE_ANALOG(4) | \
    PIN_MODE_ANALOG(5) | \
    PIN_MODE_ANALOG(6) | \
    PIN_MODE_ANALOG(7) | \
    PIN_MODE_ANALOG(8) | \
    PIN_MODE_ANALOG(9) | \
    PIN_MODE_ANALOG(10) | \
    PIN_MODE_ANALOG(11) | \
    PIN_MODE_ANALOG(12) | \
    PIN_MODE_ANALOG(13) | \
    PIN_MODE_ANALOG(14) | \
    PIN_MODE_ANALOG(15) | \
    0)

#define VAL_GPIOF_OTYPER ( \
    PIN_OTYPE_OPENDRAIN(0) | \
    PIN_OTYPE_OPENDRAIN(1) | \
    PIN_OTYPE_PUSHPULL(2) | \
    PIN_OTYPE_PUSHPULL(3) | \
    PIN_OTYPE_PUSHPULL(4) | \
    PIN_OTYPE_PUSHPULL(5) | \
    PIN_OTYPE_PUSHPULL(6) | \
    PIN_OTYPE_PUSHPULL(7) | \
    PIN_OTYPE_PUSHPULL(8) | \
    PIN_OTYPE_PUSHPULL(9) | \
    PIN_OTYPE_PUSHPULL(10) | \
    PIN_OTYPE_PUSHPULL(11) | \
    PIN_OTYPE_PUSHPULL(12) | \
    PIN_OTYPE_PUSHPULL(13) | \
    PIN_OTYPE_PUSHPULL(14) | \
    PIN_OTYPE_PUSHPULL(15) | \
    0)

#define VAL_GPIOF_OSPEEDR ( \
    PIN_OSPEED_MEDIUM(0) | \
    PIN_OSPEED_MEDIUM(1) | \
    PIN_OSPEED_VERYLOW(2) | \
    PIN_OSPEED_VERYLOW(3) | \
    PIN_OSPEED_VERYLOW(4) | \
    PIN_OSPEED_VERYLOW(5) | \
    PIN_OSPEED_VERYLOW(6) | \
    PIN_OSPEED_VERYLOW(7) | \
    PIN_OSPEED_VERYLOW(8) | \
    PIN_OSPEED_VERYLOW(9) | \
    PIN_OSPEED_VERYLOW(10) | \
    PIN_OSPEED_VERYLOW(11) | \
    PIN_OSPEED_VERYLOW(12) | \
    PIN_OSPEED_VERYLOW(13) | \
    PIN_OSPEED_VERYLOW(14) | \
    PIN_OSPEED_VERYLOW(15) | \
    0)

#define VAL_GPIOF_PUPDR ( \
    PIN_PUPDR_PULLUP(0) | \
    PIN_PUPDR_PULLUP(1) | \
    PIN_PUPDR_PULLUP(2) | \
    PIN_PUPDR_PULLUP(3) | \
    PIN_PUPDR_PULLUP(4) | \
    PIN_PUPDR_PULLUP(5) | \
    PIN_PUPDR_PULLUP(6) | \
    PIN_PUPDR_PULLUP(7) | \
    PIN_PUPDR_PULLUP(8) | \
    PIN_PUPDR_PULLUP(9) | \
    PIN_PUPDR_PULLUP(10) | \
    PIN_PUPDR_PULLUP(11) | \
    PIN_PUPDR_PULLUP(12) | \
    PIN_PUPDR_PULLUP(13) | \
    PIN_PUPDR_PULLUP(14) | \
    PIN_PUPDR_PULLUP(15) | \
    0)

#define VAL_GPIOF_ODR ( \
    PIN_ODR_LOW(0) | \
    PIN_ODR_LOW(1) | \
    PIN_ODR_LOW(2) | \
    PIN_ODR_LOW(3) | \
    PIN_ODR_LOW(4) | \
    PIN_ODR_LOW(5) | \
    PIN_ODR_LOW(6) | \
    PIN_ODR_LOW(7) | \
    PIN_ODR_LOW(8) | \
    PIN_ODR_LOW(9) | \
    PIN_ODR_LOW(10) | \
    PIN_ODR_LOW(11) | \
    PIN_ODR_LOW(12) | \
    PIN_ODR_LOW(13) | \
    PIN_ODR_LOW(14) | \
    PIN_ODR_LOW(15) | \
    0)

#define VAL_GPIOF_AFRL ( \
    PIN_AFIO_AF(0, 1) | \
    PIN_AFIO_AF(1, 1) | \
    PIN_AFIO_AF(2, 0) | \
    PIN_AFIO_AF(3, 0) | \
    PIN_AFIO_AF(4, 0) | \
    PIN_AFIO_AF(5, 0) | \
    PIN_AFIO_AF(6, 0) | \
    PIN_AFIO_AF(7, 0) | \
    0)

#define VAL_GPIOF_AFRH ( \
    PIN_AFIO_AF(8, 0) | \
    PIN_AFIO_AF(9, 0) | \
    PIN_AFIO_AF(10, 0) | \
    PIN_AFIO_AF(11, 0) | \
    PIN_AFIO_AF(12, 0) | \
    PIN_AFIO_AF(13, 0) | \
    PIN_AFIO_AF(14, 0) | \
    PIN_AFIO_AF(15, 0) | \
    0)


#endif