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

Serial pc(USBTX, USBRX);

int main() {
	double volt;

	for (;;) {
		volt = mult.getInput(DC_VOLT);
		pc.printf("Valor raw:  %.2f\r\n", mult.aIn.read());
		pc.printf("Valor lido: %.2f\r\n", volt);
		wait(0.3);
	}
}
