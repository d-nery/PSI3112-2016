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

DigitalOut led_blue(LED_BLUE, 1);

Serial     pc(USBTX, USBRX);

int main() {
	Wave wave1, wave2;
	InputType_t input;

	pc.baud(115200);

	wait(2);

	for (;;) {
		input = mult.getInputType();
		mult.getInput(input, wave1, wave2);
		pc.printf("%di\r\n", input);

#ifdef PCDEBUG
		pc.printf("Ch1:  %2.4fl\r\n", mult.aIn.read());     // Raw Ch1
		pc.printf("Ch2:  %2.4fl\r\n", mult.aIn2.read());    // Raw Ch2
		pc.printf("Curr: %2.4fl\r\n", mult.currIn.read()); // Raw Curr
		pc.printf("Pot:  %2.4fl\r\n", mult.pot.read());      // Pot
#endif

		pc.printf("%2.4fq\r\n", wave1.Vrms);
		wait_us(1000);
		pc.printf("%2.4fe\r\n", wave1.frequencia);
		wait_us(1000);
		pc.printf("%2.4fr\r\n", wave1.periodo);
		wait_us(1000);
		pc.printf("%2.4ft\r\n", wave1.amplitude);
		wait_us(1000);

		pc.printf("%2.4fa\r\n", wave2.Vrms);
		wait_us(1000);
		pc.printf("%2.4fd\r\n", wave2.frequencia);
		wait_us(1000);
		pc.printf("%2.4ff\r\n", wave2.periodo);
		wait_us(1000);
		pc.printf("%2.4fg\r\n", wave2.amplitude);
		wait_us(1000);

		pc.printf("%.1fh\r\n", wave2.def);
		wait_us(1000);

		pc.printf("%dz\r\n", wave1.form);                // Waveform
		wait_us(1000);
		pc.printf("%dx\r\n", wave2.form);                // Waveform

		wait_ms(100);
	}
}
