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
#define min(a, b) ((a) < (b) ? (a) : (b))

extern Serial pc;

namespace PSI {
	Multimetro::Multimetro() : aIn(ADC_VOLT_IN), aIn2(DEF_IN), pot(POT_IN) {
		medir.start();
	}

	double Multimetro::getInput(InputType_t input, WaveForm_t& wave) {
		double ACVolts[VECTOR_SIZE]; // guarda as últimas VECTOR_SIZE medicoes para identificar o tipo de onda
		double ACVolts2[VECTOR_SIZE]; // guarda as últimas VECTOR_SIZE medicoes para identificar o tipo de onda
		switch (input) {
			case AC_VOLT:
				ACVolt = 0;
				medir.reset();
				for (int i = 0; i < VECTOR_SIZE; i++) {
					while (medir.read_us() < 50);
					medir.reset();
					ACVolt = aIn.read();
					ACVolts[i] = ACVolt;
				}
				ACVolt = findVrms(ACVolts, wave);
				return ACVolt;

			case DC_VOLT:
				DCVolt = 0;
				for (int i = 0; i < 1000; i++)
					DCVolt += aIn.read();

				DCVolt /= 1000;   // Media
				// DCVolt = DCVolt * VCC; // Porcentagem de VCC
				// (VCC - MINV)/(Xlido - MINV) = (10/X)
				DCVolt = 10. * double((DCVolt - MINV))/(MAXV - MINV);
				wave = DC;
				return DCVolt;

			case DC_CURR:
				wave = DC;
				DCCurrent = 0;
				for (int i = 0; i < 1000; i++)
					DCCurrent += aIn.read();

				DCCurrent /= 1000;
				// DCCurrent *= VCC * 1000; // mV
				DCCurrent = 100. * double((DCCurrent - MINVI))/(MAXVI - MINVI);
				// DCCurrent /= Rshunt;
				return DCCurrent;

			// @TODO aqui
			case DEFASAGEM:
				Defasagem = 0;
				medir.reset();
				for (int i = 0; i < VECTOR_SIZE; i++) {
					while (medir.read_us() < 50);
					medir.reset();
					ACVolt = aIn.read();
					ACVolts[i] = ACVolt;

					while (medir.read_us() < 50);       // Le a segunda medida 50us depois, subtrair isso ao final
					medir.reset();
					ACVolt = aIn2.read();
					ACVolts2[i] = ACVolt;
				}
				Defasagem = findDef(ACVolts, ACVolts2, wave);
				return Defasagem;
		}
		return 0.;
	}

	InputType_t Multimetro::getInputType() {
		double val = pot.read();
		if (val < 0.25)
			return DC_VOLT;
		if (val < 0.5)
			return DC_CURR;
		if (val < 0.75)
			return AC_VOLT;
		return DEFASAGEM;
	}

	// @TODO Definir qual tipo de onda e definir o Vrms
	double Multimetro::findVrms(double* ACVolts, WaveForm_t& wave) {
		double vrms = 0;
		// double _max = 0, _min = 0;
		wave = SINE;

		// for (int i = 0; i < VECTOR_SIZE; i++) {
		// 	if (getInputType() != AC_VOLT)
		// 		return 0;
		// 	_max = max(_max, ACVolts[i]);
		// 	_min = min(_min, ACVolts[i]);
		// }
		//
		// for (int i = 0; i < VECTOR_SIZE; i++) {
		// 	if (getInputType() != AC_VOLT)
		// 		return 0;
		// 	ACVolts[i] -= (_max + _min) / 2; // Tira offset
		// 	ACVolts[i] *= (_max + _min) * 2; // Multiplica VCC
		// 	vrms += ACVolts[i] * ACVolts[i];
		// }
		// vrms /= VECTOR_SIZE;
		// vrms = sqrt(vrms);

		for (int i = 0; i < VECTOR_SIZE; i++) {
			if (getInputType() != AC_VOLT)
				return 0;
			vrms = max(vrms, ACVolts[i]);     // Acha o valor maximo da onda
		}

		vrms = 10. * double((vrms - MINV))/(MAXV - MINV);

		vrms /= sqrt(2);

		// wait(1);

		return vrms;
	}

	// @TODO aqui
	double Multimetro::findDef(double* ACVolts, double* ACVolts2, WaveForm_t& wave) {
		double vrms = 0;
		// double _max = 0, _min = 0;
		wave = SINE;

		// wait(1);

		return vrms;
	}
}
