/**
Escola Politecnica da Universidade de Sao Paulo
PSI312 - Laboratorio de Circuitos Eletricos - 2016

Multimetro Digital Microcontrolado

Turmas 7 e 8 - Grupo 1
	Alexandre Henrique Costa Rossi   9348950
	Arthur Gandelman                 9349176
	Arthur Kenji Furuko Sakai        9349217
	Daniel Nery Silva de Oliveira    9349051
	Mateus Almeida Barbosa           9349072
	Rubens Funabashi                 9348939
*/

#include "mbed.h"
#include "pins.h"

#include "Multimetro.hpp"

using namespace PSI;

Multimetro mult;

DigitalOut led_dcv(PTE21);
DigitalOut led_acv(PTE29);
DigitalOut led_dcc(PTD4);
DigitalOut led_blue(LED_BLUE);

Serial     pc(USBTX, USBRX);

int main() {
	double volt = 0;
	WaveForm_t wave;

	pc.baud(115200);

	for (;;) {
		switch (mult.getInputType()) {
		case DC_VOLT:
			led_dcv = 1;
			led_acv = led_dcc = 0;
			volt = mult.getInput(DC_VOLT, wave);
			pc.printf("%di\r\n", DC_VOLT);
			break;

		case AC_VOLT:
			led_acv = 1;
			led_dcv = led_dcc = 0;
			volt = mult.getInput(AC_VOLT, wave);
			pc.printf("%di\r\n", AC_VOLT);
			break;

		case DC_CURR: // Corrente em mA
			led_dcc = 1;
			led_dcv = led_acv = 0;
			volt = mult.getInput(DC_CURR, wave);
			pc.printf("%di\r\n", DC_CURR);
			break;

		default:
			volt = 0;
			led_dcc = led_dcv = led_acv = 0;
		}

		pc.printf("%2.4fr\r\n", mult.aIn.read()); // Raw
		pc.printf("%2.4fp\r\n", mult.pot.read()); // Pot
		pc.printf("%2.4fv\r\n", volt);            // Out
		pc.printf("%dw\r\n", wave);               // Waveform

		wait_ms(100);
	}
}
