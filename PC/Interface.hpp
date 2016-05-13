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

#include <string>

#include <ncurses.h>

namespace PSI {
	enum InputType_t {
		DC_VOLT = 0,
		AC_VOLT,
		DC_CURR,
		DEFASAGEM
	};

	enum WaveForm_t {
		SINE = 0,
		TRIANGLE,
		SQUARE,
		SAWTOOTH,
		DC
	};

	class Interface {
		WINDOW* info;

		std::string msg;
		int meiox;
		int x;

	public:
		Interface();
		~Interface();

		void printTitle();
		void update(InputType_t input, WaveForm_t wave, double raw, double pot, double volt, std::string msg);
	};
}
