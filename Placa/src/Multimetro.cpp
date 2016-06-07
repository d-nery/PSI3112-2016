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

#include "Multimetro.hpp"

#include "pins.h"

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

extern Serial bt;

namespace PSImetro {

	Multimetro::Multimetro() :
		aIn(ADC_VOLT_IN),
		aIn2(DEF_IN),
		currIn(CURR_IN),
		pot(POT_IN),
		buzzer(BUZZER, 0),
		led_dcv(PTE29, 0),
		led_dcc(PTE21, 0),
		led_acv(PTE20, 0)
		// lcd(LCD_RX, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7)
	{
		medir.start();
	}

	void Multimetro::getInput(InputType_t input, Wave& wave1, Wave& wave2) {
		switch (input) {
			case DC_VOLT:
				led_dcv = 1;
				led_acv = led_dcc = 0;

				wave1 = 0;
				wave2 = 0;
				for (int i = 0; i < 1000; i++) {
					wave1.Vrms += aIn.read();
					wave2.Vrms += aIn2.read();
				}

				wave1.Vrms /= 1000;   // Media
				wave2.Vrms /= 1000;   // Media
				// buzzer = (wave1.Vrms < MINV + 0.01 ? 1 : 0);
				// DCVolt = DCVolt * VCC; // Porcentagem de VCC
				// (VCC - MINV)/(Xlido - MINV) = (10/X)
				wave1.Vrms = 10. * double((wave1.Vrms - MINV))/(MAXV - MINV);
				wave2.Vrms = 10. * double((wave2.Vrms - MINV2))/(MAXV2 - MINV2);
				wave1.form = DC;
				wave2.form = DC;
				return;

			case DC_CURR:
				led_dcc = 1;
				led_dcv = led_acv = 0;

				wave1.form = DC;
				wave1.Vrms = 0;
				buzzer = 0;
				for (int i = 0; i < 1000; i++)
					wave1.Vrms += currIn.read();

				wave1.Vrms /= 1000;
				// DCCurrent *= VCC * 1000; // mV
				wave1.Vrms = 100. * double((wave1.Vrms - MINVI))/(MAXVI - MINVI);
				// DCCurrent /= Rshunt;
				return;

			case AC_VOLT:
				led_acv = 1;
				led_dcv = led_dcc = 0;

				wave1.Vrms = 0;
				buzzer = 0;
				medir.reset();
				for (int i = 0; i < VECTOR_SIZE; i++) {
					while (medir.read_us() < 100*i);
					ACVolts1[0][i] = medir.read_us();
					ACVolts1[1][i] = aIn.read();

					ACVolts2[0][i] = medir.read_us();
					ACVolts2[1][i] = aIn2.read();
				}

				findVrms(wave1, wave2);
				findDef(wave1, wave2);
				// findImpedance(wave1, wave2);

				for (int i = 0; i < VECTOR_SIZE && getInputType() == AC_VOLT; i++) {
					bt.printf("%d,%.1f,%.4f,%d,1,0,0,d\r\n",
						AC_VOLT, ACVolts1[0][i], ACVolts1[1][i], i);
					bt.printf("%d,%.1f,%.4f,%d,2,0,0,d\r\n",
						AC_VOLT, ACVolts2[0][i], ACVolts2[1][i], i);
				}
				return;

			default:
				led_dcc = led_dcv = led_acv = 0;
				wave1 = 0;
				wave2 = 0;
		}
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
	void Multimetro::findVrms(Wave& wave1, Wave& wave2) {
		wave1 = 0;     // Reinicia as ondas
		wave2 = 0;

		wave1.form = wave2.form = SINE; // Ondas Senoidais

		for (int i = 0; i < VECTOR_SIZE; i++) {
			ACVolts1[1][i] = 10. * double((ACVolts1[1][i] - MINV)) /(MAXV  - MINV);     // Mapeia as entradas para -10 - 10 V
			ACVolts2[1][i] = 10. * double((ACVolts2[1][i] - MINV2))/(MAXV2 - MINV2);
			wave1.amplitude = max(wave1.amplitude, ACVolts1[1][i]);     // Acha o valor maximo da onda
			wave2.amplitude = max(wave2.amplitude, ACVolts2[1][i]);     // Acha o valor maximo da onda
		}

		wave1.Vrms = wave1.amplitude / sqrt(2);
		wave2.Vrms = wave2.amplitude / sqrt(2);

		// wait(1);
	}

	void Multimetro::findDef(Wave& wave1, Wave& wave2) {
		int index1 = 0, index2 = 0, pico1= 0, pico2 = 0;

		// Canal1
		if (ACVolts1[1][index1] > ACVolts1[1][index1 + 1]) {                 // Se comecar no meio de uma descida
			for (; ACVolts1[1][index1] > ACVolts1[1][index1 + 1]; index1++); // Espera achar o primeiro vale
			index1++;
		}
		for (; ACVolts1[1][index1] < ACVolts1[1][index1 + 1]; index1++); // Espera achar primeiro pico
		index2 = pico1 = index1;
		for (; ACVolts1[1][index1] > ACVolts1[1][index1 + 1]; index1++); // Espera achar proximo vale

		wave1.periodo = (ACVolts1[0][index1] - ACVolts1[0][index2]) * 2 * 1e-3; // Periodo em ms
		wave1.frequencia =  1./(wave1.periodo * 1e-3);                          // Frequencia em Hz

		// Canal2
		index1 = 0, index2 = 0;
		if (ACVolts2[1][index1] > ACVolts2[1][index1 + 1]) {
			for (; ACVolts2[1][index1] > ACVolts2[1][index1 + 1]; index1++);
			index1++;
		}
		for (; ACVolts2[1][index1] < ACVolts2[1][index1 + 1]; index1++);
		index2 = pico2 = index1;
		for (; ACVolts2[1][index1] > ACVolts2[1][index1 + 1]; index1++);

		wave2.periodo = (ACVolts2[0][index1] - ACVolts2[0][index2]) * 2 * 1e-3;
		wave2.frequencia =  1./(wave2.periodo * 1e-3);

		// Calcular defasagem
		wave2.def = 360 * wave1.frequencia * (ACVolts2[0][pico2] - ACVolts1[0][pico1]) * 1e-6; // Defasagem em graus
		wave2.def = (wave2.def < 0 ? 360 + wave2.def : wave2.def);

		return;
	}
}
