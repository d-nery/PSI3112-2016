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

// static Mutex mtx;
static double ACVolts[2][VECTOR_SIZE];  // guarda as últimas VECTOR_SIZE medicoes das entrada 1 e 2
static bool start;

namespace PSI {
	Multimetro::Multimetro() : aIn(ADC_VOLT_IN), pot(POT_IN), _medir(medicao) {
		medir.start();
	}

	double Multimetro::getInput(InputType_t input, WaveForm_t& wave) {

		switch (input) {
			case AC_VOLT:
				medida = 0;
				medir.reset();
				for (int i = 0; i < VECTOR_SIZE; i++) {
					while (medir.read_us() < 50);
					medir.reset();
					ACVolts[0][i] = aIn.read();
				}
				medida = findVrms(ACVolts[0], wave);
				return medida;

			case DC_VOLT:
				medida = 0;
				for (int i = 0; i < 1000; i++)
					medida += aIn.read();

				medida /= 1000;   // Media
				// DCVolt = DCVolt * VCC; // Porcentagem de VCC
				// (VCC - MINV)/(Xlido - MINV) = (10/X)
				medida = 10. * double((medida - MINV))/(MAXV - MINV);
				wave = DC;
				return medida;

			case DC_CURR:
				wave = DC;
				medida = 0;
				for (int i = 0; i < 1000; i++)
					medida += aIn.read();

				medida /= 1000;
				// DCCurrent *= VCC * 1000; // mV
				medida = 100. * double((medida - MINVI))/(MAXVI - MINVI);
				// DCCurrent /= Rshunt;
				return medida;

			case DEFASAGEM:
				medida = 0;
				medir.reset();
				start = true;
				for (int i = 0; i < VECTOR_SIZE; i++) {
					while (medir.read_us() < 50);
					medir.reset();
					ACVolts[0][i] = aIn.read();
				}
				start = false;
				wait_ms(1); //Espera para garantir que a outra medida tambem terminou
				medida = findDef(ACVolts, wave);
				return medida;
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
	double Multimetro::findVrms(double ACVolts[VECTOR_SIZE], WaveForm_t& wave) {
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
	double Multimetro::findDef(double ACVolts[2][VECTOR_SIZE], WaveForm_t& wave) {
		double def = 0;
		wave = SINE;

		return def;
	}

	void Multimetro::medicao(const void*) {
		AnalogIn aIn2(DEF_IN);
		Timer t;
		t.start();

		for (;;) {
			while(!start);
			t.reset();
			for (int i = 0; i < VECTOR_SIZE; i++) {
				while (t.read_us() < 50);
				t.reset();
				ACVolts[1][i] = aIn2.read();
			}
			start = false;
		}
	}
}
