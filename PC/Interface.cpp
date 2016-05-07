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

	void Interface::update(InputType_t input, WaveForm_t wave, double raw, double pot, double volt, std::string klzmsg) {
		clear();
		x = 25;

		info = newwin(5, 20, LINES / 2 - 3, meiox-10);
		box(info, 0, 0);

		switch (input) {
			case DC_VOLT:
				msg = "Medindo Tensão DC\n";
				break;

			case AC_VOLT:
				msg = "Medindo Tensão AC\n";
				break;

			case DC_CURR:
				msg = "Medindo Corrente DC\n";
				break;
		}
		mvprintw(x++, meiox - msg.length()/2, msg.data());

		switch (wave) {
			case SINE:
				msg = "Onda Senoidal\n";
				break;

			case TRIANGLE:
				msg = "Onda Triangular\n";
				break;

			case SAWTOOTH:
				msg = "Onda Dente de Serra\n";
				break;

			case SQUARE:
				msg = "Onda Quadrada\n";
				break;

			case DC:
				msg = "Sinal DC\n";
				break;
		}
		mvprintw(x++, meiox - msg.length()/2, msg.data());

#ifdef DEBUG
		mvprintw(1, 0, "Raw: %2.2f\n", raw);
		mvprintw(2, 0, "Pot: %2.2f\n", pot);
#endif

		switch (input) {
			case DC_VOLT:
				msg = "Tensao: XX.XX V\n";
				mvprintw(x++, meiox - msg.length()/2, "Tensao: %2.2f V\n", volt);
				break;

			case AC_VOLT:
				msg = "Vrms: XX.XX V\n";
				mvprintw(x++, meiox - msg.length()/2, "Vrms: %2.2f V\n", volt);
				break;

			case DC_CURR:
				msg = "Corrente: XXX mA\n";
				mvprintw(x++, meiox - msg.length()/2, "Corrente: %3.0f mA\n", volt);
				break;
		}
		// mvprintw(x++, meiox - msg.length()/2, msg.data());
		mvprintw(x++, meiox - klzmsg.length()/2, "%s\n", klzmsg.data());

		wrefresh(info);
		refresh();
	}
}
