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

DigitalOut led_dcv(PTE21, 0);  // DC Voltage RED
DigitalOut led_acv(PTE29, 0);  // AC Voltage YELLOW
DigitalOut led_def(PTE20, 0);  // Delay      BLUE
DigitalOut led_dcc(PTD4,  0);   // DC Current GREEN

DigitalOut led_blue(LED_BLUE, 1);

Serial     pc(USBTX, USBRX);

int main() {
	double volt = 0;
	WaveForm_t wave;

	pc.baud(115200);

	wait(2);

	for (;;) {
		switch (mult.getInputType()) {
		case DC_VOLT:
			led_dcv = 1;
			led_acv = led_dcc = led_def = 0;
			volt = mult.getInput(DC_VOLT, wave);
			pc.printf("%di\r\n", DC_VOLT);
			break;

		case AC_VOLT:
			led_acv = 1;
			led_dcv = led_dcc = led_def = 0;
			volt = mult.getInput(AC_VOLT, wave);
			pc.printf("%di\r\n", AC_VOLT);
			break;

		case DC_CURR:   // Corrente em mA
			led_dcc = 1;
			led_dcv = led_acv = led_def = 0;
			volt = mult.getInput(DC_CURR, wave);
			pc.printf("%di\r\n", DC_CURR);
			break;

		case DEFASAGEM: // Defasagem
			led_def = 1;
			led_dcv = led_acv = led_dcc = 0;
			volt = mult.getInput(DEFASAGEM, wave);
			pc.printf("%di\r\n", DEFASAGEM);
			break;

		default:
			volt = 0;
			led_dcc = led_dcv = led_acv = 0;
		}

#ifdef PCDEBUG
		pc.printf("%2.4fr\r\n", mult.aIn.read());  // Raw
		// pc.printf("%2.4fd\r\n", mult.aIn2.read()); // Raw Def
		pc.printf("%2.4fp\r\n", mult.pot.read());  // Pot
#endif

		pc.printf("%2.4fv\r\n", volt);             // Out
		pc.printf("%dw\r\n", wave);                // Waveform

		wait_ms(100);
	}
}
