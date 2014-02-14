//#include "i2c.h"
//#include "gpio.h"
//#include "type.h"
//#include "mcu_regs.h"
//#include "oled.h"
//#include "light.h"
//
//#define TAMANHOBUFFERENTRADA 32
//#define TAMANHOBUFFERSAIDA 64
//
//volatile char bufferEntrada[TAMANHOBUFFERENTRADA];
//volatile char bufferSaida[TAMANHOBUFFERSAIDA];
//
//int contadorLeituraBufferEntrada = 0;
//int contadorEscritaBufferEntrada = 0;
//int contadorLeituraBufferSaida = 0;
//int contadorEscritaBufferSaida = 0;
//int contadorEntradas = 0;
//int contadorSaidas = 0;
//
//int flagCaracterNaoLido = 0;
//
//void setCaracterSaida(char caracter) {
//	contadorSaidas++;
//	if (contadorSaidas > TAMANHOBUFFERSAIDA)
//		contadorSaidas = TAMANHOBUFFERSAIDA;
//	bufferSaida[contadorEscritaBufferSaida] = caracter;
//	contadorEscritaBufferSaida++;
//	if (contadorEscritaBufferSaida == TAMANHOBUFFERSAIDA)
//		contadorEscritaBufferSaida = 0;
//}
//
//char trocaCaracter(char caracter) {
//	if (caracter > 64 && caracter < 91)
//		caracter += 32;
//	else if (caracter > 96 && caracter < 123)
//		caracter -= 32;
//	return caracter;
//}
//
//void enviarCaracter() {
//	contadorSaidas--;
//	LPC_UART ->THR = bufferSaida[contadorLeituraBufferSaida];
//	contadorLeituraBufferSaida++;
//	if (contadorLeituraBufferSaida == TAMANHOBUFFERSAIDA)
//		contadorLeituraBufferSaida = 0;
//}
//
//char getCaracterEntrada() {
//	contadorEntradas--;
//	char caracter = bufferEntrada[contadorLeituraBufferEntrada];
//	contadorLeituraBufferEntrada++;
//	if (contadorLeituraBufferEntrada == TAMANHOBUFFERENTRADA)
//		contadorLeituraBufferEntrada = 0;
//	if (contadorEntradas == 0)
//		flagCaracterNaoLido = 0;
//	return caracter;
//}
//
//void setCaracterEntrada() {
//	contadorEntradas++;
//	if (contadorEntradas > TAMANHOBUFFERENTRADA)
//		contadorEntradas = TAMANHOBUFFERENTRADA;
//	bufferEntrada[contadorEscritaBufferEntrada] = LPC_UART ->RBR;
//	contadorEscritaBufferEntrada++;
//	if (contadorEscritaBufferEntrada == TAMANHOBUFFERENTRADA)
//		contadorEscritaBufferEntrada = 0;
//	flagCaracterNaoLido = 1;
//	tratarCaracter();
//}
//
//void atualizarTela() {
//	char capacidadeEntradaUsada[2] = "00";
//	char capacidadeEntradaTotal[2] = "00";
//	char capacidadeSaidaUsada[2] = "00";
//	char capacidadeSaidaTotal[2] = "00";
//	int capacidadeEntradaUtilizada = contadorEntradas;
//	int capacidadeSaidaUtilizada = contadorSaidas;
//	if (capacidadeEntradaUtilizada < 0)
//		capacidadeEntradaUtilizada += TAMANHOBUFFERENTRADA;
//	if (capacidadeEntradaUtilizada < 10) {
//		sprintf(capacidadeEntradaUsada, "0%d", capacidadeEntradaUtilizada);
//	} else {
//		sprintf(capacidadeEntradaUsada, "%d", capacidadeEntradaUtilizada);
//	}
//	sprintf(capacidadeEntradaTotal, "%d", TAMANHOBUFFERENTRADA);
//	if (capacidadeSaidaUtilizada < 0)
//		capacidadeSaidaUtilizada += TAMANHOBUFFERSAIDA;
//	if (capacidadeSaidaUtilizada < 10) {
//		sprintf(capacidadeSaidaUsada, "0%d", capacidadeSaidaUtilizada);
//	} else {
//		sprintf(capacidadeSaidaUsada, "%d", capacidadeSaidaUtilizada);
//	}
//	sprintf(capacidadeSaidaTotal, "%d", TAMANHOBUFFERSAIDA);
//
//	oled_putString(1, 1, ((uint8_t*) "Entrada = "), OLED_COLOR_BLACK,
//			OLED_COLOR_WHITE);
//	oled_putString(1, 9, ((uint8_t*) bufferEntrada), OLED_COLOR_BLACK,
//			OLED_COLOR_WHITE);
//	oled_putString(1, 17,
//			((uint8_t*) bufferEntrada + (TAMANHOBUFFERENTRADA / 2 - 1)),
//			OLED_COLOR_BLACK, OLED_COLOR_WHITE);
//	oled_putString(1, 25, ((uint8_t*) "Cap Entrada = "), OLED_COLOR_BLACK,
//			OLED_COLOR_WHITE);
//	oled_putString(1, 33, ((uint8_t*) capacidadeEntradaUsada), OLED_COLOR_BLACK,
//			OLED_COLOR_WHITE);
//	oled_putString(21, 33, ((uint8_t*) "/"), OLED_COLOR_BLACK,
//			OLED_COLOR_WHITE);
//	oled_putString(41, 33, ((uint8_t*) capacidadeEntradaTotal),
//			OLED_COLOR_BLACK, OLED_COLOR_WHITE);
//	oled_putString(1, 41, ((uint8_t*) "Cap Saida = "), OLED_COLOR_BLACK,
//			OLED_COLOR_WHITE);
//	oled_putString(1, 49, ((uint8_t*) capacidadeSaidaUsada), OLED_COLOR_BLACK,
//			OLED_COLOR_WHITE);
//	oled_putString(21, 49, ((uint8_t*) "/"), OLED_COLOR_BLACK,
//			OLED_COLOR_WHITE);
//	oled_putString(41, 49, ((uint8_t*) capacidadeSaidaTotal), OLED_COLOR_BLACK,
//			OLED_COLOR_WHITE);
//
//}
//
//void tratarCaracter() {
//	if (flagCaracterNaoLido) {
//		char teste = getCaracterEntrada();
//		teste = trocaCaracter(teste);
//		setCaracterSaida(teste);
//		enviarCaracter();
//	} else {
//		atualizarTela();
//	}
//}
//
//int main() {
//	GPIOInit();
//	I2CInit((uint32_t) I2CMASTER, 0);
//	SSPInit();
//	oled_init();
//	light_enable();
//	light_setRange(LIGHT_RANGE_4000);
//	oled_clearScreen(OLED_COLOR_WHITE);
//	init_uart();
//	atualizarTela();
//	while (1) {
//		tratarCaracter();
//	}
//	return 0;
//}
