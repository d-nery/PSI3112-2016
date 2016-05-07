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
		printTitle();
		refresh();

		// info = newwin(6, 20, x + 5, meiox - 20/2);
		// info = newwin(5, 20, LINES / 2 - 3, meiox-10);
		// box(info, 0, 0);
		// wrefresh(info);
		// msg = " INFO ";
		// mvprintw(0, meiox - msg.length()/2, msg.data());
		x += 5 + 2;

		switch (input) {
			case DC_VOLT:
				msg = "Medindo Tensão DC";
				break;

			case AC_VOLT:
				msg = "Medindo Tensão AC";
				break;

			case DC_CURR:
				msg = "Medindo Corrente DC";
				break;
		}
		mvprintw(x++, meiox - msg.length()/2, msg.data());

		switch (wave) {
			case SINE:
				msg = "Onda Senoidal";
				break;

			case TRIANGLE:
				msg = "Onda Triangular";
				break;

			case SAWTOOTH:
				msg = "Onda Dente de Serra";
				break;

			case SQUARE:
				msg = "Onda Quadrada";
				break;

			case DC:
				msg = "Sinal DC";
				break;
		}
		mvprintw(x++, meiox - msg.length()/2, msg.data());

#ifdef DEBUG
		mvprintw(1, 0, "Raw: %2.2f", raw);
		mvprintw(2, 0, "Pot: %2.2f", pot);
#endif

		switch (input) {
			case DC_VOLT:
				msg = "Tensao: XX.XX V";
				mvprintw(x++, meiox - msg.length()/2, "Tensao: %2.2f V", volt);
				break;

			case AC_VOLT:
				msg = "Vrms: XX.XX V\n";
				mvprintw(x++, meiox - msg.length()/2, "Vrms: %2.2f V", volt);
				break;

			case DC_CURR:
				msg = "Corrente: XXX mA\n";
				mvprintw(x++, meiox - msg.length()/2, "Corrente: %3.0f mA", volt);
				break;
		}
		// mvprintw(x++, meiox - msg.length()/2, msg.data());
		mvprintw(x++, meiox - klzmsg.length()/2, "%s\n", klzmsg.data());

		// wrefresh(info);
		refresh();
	}

	void Interface::printTitle() {
        x = 5;

        msg = "..88.......88..88....88..88........88888888..88..88.......88..88888888..88888888..8888888...88888888..";
        mvprintw(x++, meiox - msg.length()/2, msg.data());
        msg = "..8888...8888..88....88..88...........88.....88..8888...8888..88...........88.....88....88..88....88..";
        mvprintw(x++, meiox - msg.length()/2, msg.data());
        msg = "..88.88.88.88..88....88..88...........88.....88..88.88.88.88..88...........88.....88....88..88....88..";
        mvprintw(x++, meiox - msg.length()/2, msg.data());
        msg = "..88...8...88..88....88..88...........88.....88..88...8...88..88...........88.....88....88..88....88..";
        mvprintw(x++, meiox - msg.length()/2, msg.data());
        msg = "..88.......88..88....88..88...........88.....88..88.......88..88888........88.....888888....88....88..";
        mvprintw(x++, meiox - msg.length()/2, msg.data());
        msg = "..88.......88..88....88..88...........88.....88..88.......88..88...........88.....88..88....88....88..";
        mvprintw(x++, meiox - msg.length()/2, msg.data());
        msg = "..88.......88..88....88..88...........88.....88..88.......88..88...........88.....88...88...88....88..";
        mvprintw(x++, meiox - msg.length()/2, msg.data());
        msg = "..88.......88..88888888..88888888.....88.....88..88.......88..88888888.....88.....88....88..88888888..";
        mvprintw(x++, meiox - msg.length()/2, msg.data());
        msg = "......................................................................................................";
        mvprintw(x++, meiox - msg.length()/2, msg.data());
        msg = ".......................................  Turmas 7 e 8 - Grupo 1 ......................................";
        mvprintw(x++, meiox - msg.length()/2, msg.data());
		msg = "......................................................................................................";
        mvprintw(x++, meiox - msg.length()/2, msg.data());
    }
}
