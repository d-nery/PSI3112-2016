/**
Escola Politecnica da Universidade de Sao Paulo
PSI3112 - Laboratorio de Circuitos Eletricos - 2016

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

using namespace PSImetro;

Multimetro mult;

DigitalOut led_blue(LED_BLUE, 1); // So para mostrar que esta ligado

// Serial     bt(USBTX, USBRX);
Serial     bt(PTE0, PTE1);   // Bluetooth

int main() {
	Wave wave1, wave2;
	InputType_t input;

	// pc.baud(9600);
	bt.baud(9600);

	wait(2);

	for (;;) {
		input = mult.getInputType();
		mult.getInput(input, wave1, wave2);
		// bt.printf("%di\r\n", input);
		wait_us(1000);

#ifdef PCDEBUG
		bt.printf("Ch1:  %2.4fl\r\n", mult.aIn.read());    // Raw Ch1
		wait_us(1000);
		bt.printf("Ch2:  %2.4fl\r\n", mult.aIn2.read());   // Raw Ch2
		wait_us(1000);
		bt.printf("Curr: %2.4fl\r\n", mult.currIn.read()); // Raw Curr
		wait_us(1000);
		bt.printf("Pot:  %2.4fl\r\n", mult.pot.read());    // Pot
		wait_us(1000);
#endif

		// Input,Vrms,def,freq,per,ampl,[0|1],w
		bt.printf("%d,%2.4f,%.1f,%2.4f,%2.4f,%2.4f,0,w\r\n",
			input, wave1.Vrms, wave1.def, wave1.frequencia, wave1.periodo, wave1.amplitude);

		bt.printf("%d,%2.4f,%.1f,%2.4f,%2.4f,%2.4f,1,w\r\n",
		 	input, wave2.Vrms, wave2.def, wave2.frequencia, wave2.periodo, wave2.amplitude);

		wait_ms(100);
	}
}
