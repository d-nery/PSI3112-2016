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
		double ACVolts[1000]; // guarda as últimas 1000 medicoespara identificar o tipo de onda
		switch (input) {
			case AC_VOLT:
				ACVolt = 0;
				for (int j = 0; j < 1000; j++) {
					for (int i = 0; i < 10; i++) {
						ACVolt += aIn.read();
						// wait_us(1);
					}
					ACVolt /= 10;
					ACVolts[j] = ACVolt;
					wait_us(1); // Le 1 ms de onda (1 periodo a 1kHz)
				}
				ACVolt = findVrms(ACVolts, wave);
				return ACVolt;

			case DC_VOLT:
				DCVolt = 0;
				for (int i = 0; i < 1000; i++) {
					DCVolt += aIn.read();
					wait_us(1);
				}
				DCVolt /= 1000;   // Media
				DCVolt = DCVolt * VCC; // Porcentagem de VCC
				DCVolt = R3 * ((1./R1 + 1./R2 + 1./R3) * DCVolt - double(VCC)/R1); // Faz a correcao para 0-10 V - Ver Esquemático
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

		for (int i = 0; i < 1000; i++) {
			if (getInputType() != AC_VOLT)
				return 0;
			// pc.printf("%f\r\n", ACVolts[0] * VCC);
			// wait_us(100);
		}
		wait(1);

		return vrms;
	}
}
