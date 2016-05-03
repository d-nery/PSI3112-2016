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

#include "Interface.hpp"

namespace PSI {
	Interface::Interface() {
		initscr();
		raw();
		noecho();

		meiox = COLS/2;
        x = 0;
	}

	Interface::~Interface() {
		endwin();
	}

	void Interface::tela(InputType_t input, WaveForm_t wave, double raw, double pot, double volt, std::string msg) {
		clear();

		switch (input) {
			case DC_VOLT:
				mvprintw(1, 0, "Medindo Tensão DC\n");
				break;

			case AC_VOLT:
				mvprintw(1, 0, "Medindo Tensão AC\n");
				break;

			case DC_CURR:
				mvprintw(1, 0, "Medindo Corrente DC\n");
				break;
		}

		switch (wave) {
			case SINE:
				printw("Onda Senoidal\n");
				break;

			case TRIANGLE:
				printw("Onda Triangular\n");
				break;

			case SAWTOOTH:
				printw("Onda Dente de Serra\n");
				break;

			case SQUARE:
				printw("Onda Quadrada\n");
				break;

			case DC:
				printw("Sinal DC\n");
				break;
		}

		printw("Raw: %2.2f\n", raw);
		printw("Pot: %2.2f\n", pot);
		printw("Volt: %2.2f\n", volt);
		printw("%s\n", msg.data());

		refresh();
	}
}
