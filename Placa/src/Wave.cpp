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

#include "Wave.hpp"

namespace PSI {
	Wave::Wave() {
		frequency = 0;
		periodo = 0;
		form = DC;
		Vrms = 0;
	}

	Wave& Wave::operator=(int a){
		frequency = a;
		Vrms = a;
		return *this;
	}
}
