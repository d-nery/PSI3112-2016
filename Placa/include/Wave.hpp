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

#pragma once

namespace PSI {
	enum WaveForm_t {
		SINE = 0,      // /sqrt(2)
		TRIANGLE,      // /sqrt(3)
		SQUARE,        // /sqrt(1)
		SAWTOOTH,      // /sqrt(3)
		DC             // /sqrt(1)
	};

	class Wave {
	public:
		WaveForm_t form;
		double frequencia;
		double amplitude;
		double periodo;
		double Vrms;
		double def;

		Wave();
		Wave& operator=(int);
	};
}
