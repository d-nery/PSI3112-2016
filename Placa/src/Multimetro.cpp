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

#define max(a, b) ((a) > (b) ? (a) : (b))

extern Serial pc;

namespace PSI {
	Multimetro::Multimetro() : aIn(ADC_VOLT_IN), pot(POT_IN) {}

	double Multimetro::getInput(InputType_t input, WaveForm_t& wave) {
		double ACVolts[VECTOR_SIZE]; // guarda as últimas VECTOR_SIZE medicoespara identificar o tipo de onda
		switch (input) {
			case AC_VOLT:
				ACVolt = 0;
				for (int j = 0; j < VECTOR_SIZE; j++) {
					ACVolt = aIn.read();
					ACVolts[j] = ACVolt;
					wait_us(1); // Le 1 ms de onda (1 periodo a 1kHz)
				}
				ACVolt = findVrms(ACVolts, wave);
				return ACVolt;

			case DC_VOLT:
				DCVolt = 0;
				for (int i = 0; i < VECTOR_SIZE; i++) {
					DCVolt += aIn.read();
					wait_us(1);
				}
				DCVolt /= VECTOR_SIZE;   // Media
				DCVolt = DCVolt * VCC; // Porcentagem de VCC
				// (VCC - MINV)/(Xlido - MINV) = (10/X)
				DCVolt = 10./((VCC - MINV)/(DCVolt - MINV));
				wave = DC;
				return DCVolt;

			// @TODO Corrente DC
			case DC_CURR:
				DCCurrent = aIn.read();
				wave = DC;
				return DCCurrent;
		}
		return 0.;
	}

	InputType_t Multimetro::getInputType() {
		double val = pot.read();
		if (val < 0.3)
			return DC_VOLT;
		if (val < 0.7)
			return DC_CURR;
		return AC_VOLT;
	}

	// @TODO Definir qual tipo de onda e definir o Vrms
	double Multimetro::findVrms(double* ACVolts, WaveForm_t& wave) {
		double vrms = 0;
		wave = SINE;

		for (int i = 0; i < VECTOR_SIZE; i++) {
			if (getInputType() != AC_VOLT)
				return 0;
			pc.printf("%f\r\n", ACVolts[i] * VCC);
			wait_ms(1);
		}
		wait(1);

		return vrms;
	}
}
