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

#include "Multimetro.hpp"

namespace PSI {
	Multimetro::Multimetro() : aIn(ADC_VOLT_IN) {}

	// void Multimetro::update() {
	// }

	double Multimetro::getInput(InputType_t input) {
		switch(input) {
			case AC_VOLT:
				ACVolt = aIn.read();
				return ACVolt;

			case DC_VOLT:
				DCVolt = 0;
				for (int i = 0; i < 1000; i++) {
					DCVolt += aIn.read();
				}
				DCVolt /= 1000;
				DCVolt = DCVolt * VCC;
				DCVolt = R3 * ((1./R1 + 1./R2 + 1./R3) * DCVolt - double(VCC)/R1);
				return DCVolt;

			case DC_CURR:
				DCCurrent = aIn.read();
				return DCCurrent;
		}
		return 0.;
	}
}
