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

#include "pins.h"

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

extern Serial pc;

static Mutex mtx;
static double ACVolts1[2][VECTOR_SIZE];  // guarda as últimas VECTOR_SIZE medicoes das entrada 1 e seu tempo
static double ACVolts2[2][VECTOR_SIZE];  // guarda as últimas VECTOR_SIZE medicoes das entrada 2 e seu tempo

namespace PSI {

	Multimetro::Multimetro() :
		aIn(ADC_VOLT_IN),
		aIn2(DEF_IN),
		currIn(CURR_IN),
		pot(POT_IN),
		buzzer(BUZZER, 0)
		// lcd(LCD_RX, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7)
	{
		medir.start();
	}

	Wave Multimetro::getInput(InputType_t input) {
		Wave wave;
		switch (input) {
			case AC_VOLT:
				wave.Vrms = 0;
				buzzer = 0;
				medir.reset();
				for (int i = 0; i < VECTOR_SIZE; i++) {
					while (medir.read_us() < 50*i);
					// medir.reset();
					ACVolts1[0][i] = medir.read_us();
					ACVolts1[1][i] = aIn.read();
				}
				wave.Vrms = findVrms(ACVolts1[1], wave.form);
				return wave;

			case DC_VOLT:
				wave.Vrms = 0;
				for (int i = 0; i < 1000; i++)
					wave.Vrms += aIn.read();

				wave.Vrms /= 1000;   // Media
				buzzer = (wave.Vrms < MINV + 0.05 ? 1 : 0);
				// DCVolt = DCVolt * VCC; // Porcentagem de VCC
				// (VCC - MINV)/(Xlido - MINV) = (10/X)
				wave.Vrms = 10. * double((wave.Vrms - MINV))/(MAXV - MINV);
				wave.form = DC;
				return wave;

			case DC_CURR:
				wave.form = DC;
				wave.Vrms = 0;
				buzzer = 0;
				for (int i = 0; i < 1000; i++)
					wave.Vrms += currIn.read();

				wave.Vrms /= 1000;
				// DCCurrent *= VCC * 1000; // mV
				wave.Vrms = 100. * double((wave.Vrms - MINVI))/(MAXVI - MINVI);
				// DCCurrent /= Rshunt;
				return wave;

			case DEFASAGEM:
				wave.Vrms = 0;
				buzzer = 0;
				// t = new Thread(Multimetro::threadStarter, this);
				medir.reset();
				// start = true;
				// t->signal_set(START_MEAS);
				for (int i = 0; i < VECTOR_SIZE; i++) {
					while (medir.read_us() < 50*i);
					ACVolts1[0][i] = medir.read_us();
					ACVolts1[1][i] = aIn.read();
				}
				// t->signal_clr(START_MEAS);
				// start = false;
				wait_ms(1); // Espera para garantir que a outra medicao tambem terminou
				wave.Vrms = findDef(ACVolts1, ACVolts2, wave.form);
				return wave;

			case IMPEDANCIA:
				buzzer = 0;
				break;
		}
		wave = 0;
		return wave;
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
	double Multimetro::findDef(double ACVolts1[2][VECTOR_SIZE], double ACVolts2[2][VECTOR_SIZE], WaveForm_t& wave) {
		double def = 0;
		double ampl1 = 0, tempo1 = 0.;
		double ampl2 = 0, tempo2 = 0.;
		int maxindex = 0, difftempo = 0;
		wave = SINE;

		for (int i = 0; i < VECTOR_SIZE; i++) {
			if (getInputType() != DEFASAGEM)
				return 0;
			if (ampl1 < ACVolts1[1][i]) { // Acha o valor maximo da onda
				ampl1 = ACVolts1[1][i];
				tempo1 = ACVolts1[0][i];
				maxindex = i;
			}
			// ampl2 = max(ampl2, ACVolts2[1][i]);     // Acha o valor maximo da onda
			printf("%f %f\r\n", ACVolts1[0][i], ACVolts1[1][i]);
			wait_ms(10);
		}
		wait_ms(100);

		ampl2 = ACVolts2[1][maxindex];
		for (int i = maxindex + 1; i < VECTOR_SIZE; i++) {
			if (getInputType() != DEFASAGEM)
				return 0;
			if (ACVolts2[1][i] < ampl2)
				break;
			ampl2 = ACVolts2[1][i];
			tempo2 = ACVolts2[0][i];
		}

		ampl1 = 10. * double((ampl1 - MINV))/(MAXV - MINV);
		ampl2 = 10. * double((ampl2 - MINV2))/(MAXV2 - MINV2);
		difftempo = tempo2 - tempo1;

		printf("%f\r\n", ampl1); // Amplitude Ch1
		printf("%f\r\n", ampl2); // Amplitude Ch2

		return def;
	}

	void Multimetro::begin() {
		t = new Thread(Multimetro::threadStarter, this);
	}

	void Multimetro::medicao() {
		Timer timer;
		for (;;) {
			// printf("start!\r\n");
			// t->signal_wait(START_MEAS);
			timer.reset();
			for (int i = 0; i < VECTOR_SIZE; i++) {
				while (timer.read_us() <= 50*i);
				// timer.reset();
				ACVolts2[0][i] = medir.read_us();
				ACVolts2[1][i] = aIn2.read();
			}
			// mtx.lock();
			// start = false;
			// mtx.unlock();
		}
	}

	void Multimetro::threadStarter(void const* p) {
		Multimetro* inst = static_cast<Multimetro*>(const_cast<void*>(p));

		inst->medicao();
	}
}
