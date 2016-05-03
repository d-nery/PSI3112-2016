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

#include <iostream>
#include <thread>
#include <mutex>

#include "Interface.hpp"
#include "Serial.hpp"

using namespace std;
using namespace PSI;

static Serial klz("/dev/ttyACM0");

static string klz_msg;
static mutex klz_msg_mtx;

static InputType_t input;
static WaveForm_t wave;
static double values[] = { 0., 0., 0. }; // Raw, Pot, Value

static void reader();

int main(int argc, char** argv) {
	try {
		int c;

		Interface tela;

		thread _reader(reader);
		_reader.detach();

		for (;;) {
			klz_msg_mtx.lock();
			tela.tela(input, wave, values[0], values[1], values[2], klz_msg);
			klz_msg_mtx.unlock();

			timeout(100);
			c = getch();
			switch(tolower(c)) {
			case 'q':
				return 0;
			}
		}
	} catch (out_of_range& e) {
		endwin();
		cerr << "Ocorreu um erro: " << e.what() << "\nPor favor, verifique a conexão recomece o programa\n";
		exit(-1);
	}

	return 0;
}

static void reader() {
	try {
		int n;
		string temp;

		for (;;) {
			if ((n = klz.read(temp)) > 0) {
				klz_msg_mtx.lock();
				switch (temp.back()) {
				case 'i': // Input Type
					input = InputType_t(atoi(temp.data()));
					break;

				case 'w': // Wave Form being read
					wave = WaveForm_t(atoi(temp.data()));
					break;

				case 'r': // Raw Value
					values[0] = atof(temp.data());
					break;

				case 'p': // Raw Potentiometer value read
					values[1] = atof(temp.data());
					break;

				case 'v': // Value read
					values[2] = atof(temp.data());
					break;

				case 'm': // Mensagem
					temp.pop_back();
					klz_msg = temp;
					break;
				}
				klz_msg_mtx.unlock();
			}
			usleep(10 * 1000);
		}
	} catch (out_of_range& e) {
		cerr << "Ocorreu um erro: " << e.what() << "\nPor favor, verifique a conexão recomece o programa\n";
		exit(-1);
	}
}
