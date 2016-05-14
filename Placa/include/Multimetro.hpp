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

#pragma once

#include "mbed.h"
#include "rtos.h"
#include "pins.h"

#define VCC 3.3

// Ver Esquemático
#define R1 (9.69 * 1000)
#define R2 (9.83 * 1000)
#define R3 (19.47 * 1000)

#define Rshunt 32

// Valores determinados experimentalmente
#define MINV 0.44     // Voltagem na saída (entrada da Freedom) com 0V
#define MAXV 0.83     // Voltagem na saída (entrada da Freedom) com 10V

#define MINVI 0.0040  // Voltagem na saída (entrada da Freedom) com 0 mA
#define MAXVI 0.9965  // Voltagem na saída (entrada da Freedom) com 100 mA
// Fim valores determinados experimentalmente

#define VECTOR_SIZE 100

// #define R1 (10 * 1000)
// #define R2 (10 * 1000)
// #define R3 (20 * 1000)

namespace PSI {
	enum InputType_t {
		DC_VOLT = 0,
		AC_VOLT,
		DC_CURR,
		DEFASAGEM
	};

	enum WaveForm_t {
		SINE = 0,      // /sqrt(2)
		TRIANGLE,      // /sqrt(3)
		SQUARE,        // /sqrt(1)
		SAWTOOTH,      // /sqrt(3)
		DC             // /sqrt(1)
	};

	class Multimetro {
	public:
		Multimetro();

		double getInput(InputType_t input,WaveForm_t& wave);

		InputType_t getInputType();

#ifndef PCDEBUG
	private:  // So nao sao privados se for enviar pro PC
#endif
		AnalogIn aIn;
		AnalogIn pot;
	private:
		Timer medir;
		Thread _medir;

		double medida;

		double findVrms(double ACVolts[VECTOR_SIZE], WaveForm_t& wave);  // Acha Vrms do conjunto de pontos ACVolts
		double findDef(double ACVolts[2][VECTOR_SIZE], WaveForm_t& wave);  // Acha defasagem dos conjuntos de pontos ACVolts
		static void medicao(const void*);                    // Funcao auxiliar que mede a segunda onda ao mesmo tempo da primeira
	};
}
