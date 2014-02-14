#include "i2c.h"
#include "gpio.h"
#include "type.h"
#include "mcu_regs.h"
#include "oled.h"
#include "light.h"

#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80

#define FREQUENCIA SystemCoreClock

void setBaudrate(uint32_t baudrate) {
	uint32_t Fdiv;
	uint32_t regVal;

	NVIC_DisableIRQ(UART_IRQn); //Interrupção externa da UART desativada
	LPC_IOCON ->PIO1_6 &= ~0x07;
	LPC_IOCON ->PIO1_6 |= 0x01; //rx
	LPC_IOCON ->PIO1_7 &= ~0x07; //tx
	LPC_IOCON ->PIO1_7 |= 0x01;
	LPC_SYSCON ->SYSAHBCLKCTRL |= (1 << 12);
	LPC_SYSCON ->UARTCLKDIV = 0x1;
	LPC_UART ->LCR = 0x83; // 8 bits, sem paridade, 1 bit de parada
	regVal = LPC_SYSCON ->UARTCLKDIV;
	Fdiv = (((FREQUENCIA / LPC_SYSCON ->SYSAHBCLKDIV) / regVal) / 16)/ baudrate;
	LPC_UART ->DLM = Fdiv / 256;
	LPC_UART ->DLL = Fdiv % 256;
	LPC_UART ->LCR = 0x03;
	LPC_UART ->FCR = 0x87; /* Enable and reset TX and RX FIFO. */
	regVal = LPC_UART ->LSR;
	while ((LPC_UART ->LSR & (LSR_THRE | LSR_TEMT)) != (LSR_THRE | LSR_TEMT));
	while (LPC_UART ->LSR & LSR_RDR) {
		regVal = LPC_UART ->RBR;
	}

}

void init_uart() {
	setBaudrate(115200);
	NVIC_EnableIRQ(UART_IRQn); //Interrupção externa via UART ativada
	LPC_UART ->IER = IER_RBR | IER_RLS; //Interrupção de chegada ativada

}

void UART_IRQHandler() {
	uint8_t IIRValue, LSRValue;
	uint8_t Dummy = Dummy;

	IIRValue = LPC_UART ->IIR;
	IIRValue >>= 1;
	IIRValue &= 0x07;

	if (IIRValue == IIR_RLS) {

		LSRValue = LPC_UART ->LSR;

		if (LSRValue & (LSR_OE | LSR_PE | LSR_FE | LSR_RXFE | LSR_BI)) {
			Dummy = LPC_UART ->RBR; //Ler limpa o registrador
			return;
		}
		if (LSRValue & LSR_RDR){ // DADO PRONTO, Limpa interrupção e RBR
			setCaracterEntrada();
		}
	}
	else if (IIRValue == IIR_RDA){ //DADO DISPONIVEL - de 8 em 8
		int i = 0;
		for (i = 0; i < 8; i++) {
			setCaracterEntrada();
		}
	}
	else if (IIRValue == IIR_CTI){ //TIMEOUT valor = 0x6
		setCaracterEntrada();
	}
	return;
}
