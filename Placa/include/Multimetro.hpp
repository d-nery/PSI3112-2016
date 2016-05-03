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

#pragma once

#include "mbed.h"
#include "pins.h"

#define VCC 3.3

// Ver Esquemático
#define R1 (10 * 1000)
#define R2 (10 * 1000)
#define R3 (20 * 1000)

namespace PSI {
	enum InputType_t {
		DC_VOLT = 0,
		AC_VOLT,
		DC_CURR
	};

	class Multimetro {
	public:
		Multimetro();

		// void update();

		double getInput(InputType_t input);

		AnalogIn aIn;
	private:

		double DCVolt;
		double ACVolt;
		double DCCurrent;
	};
}
