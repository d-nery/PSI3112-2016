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
// #include "TextLCD.h"

#include "Wave.hpp"

#define VCC 3.3

// Ver Esquemático
#define R1 (9.69 * 1000)
#define R2 (9.83 * 1000)
#define R3 (19.47 * 1000)

#define Rshunt 32

// Valores determinados experimentalmente
#define MINV 0.438     // Voltagem na saída (entrada da Freedom) com 0V
#define MAXV 0.835     // Voltagem na saída (entrada da Freedom) com 10V

#define MINV2 0.442     // Voltagem na saída (entrada da Freedom) com 0V
#define MAXV2 0.827     // Voltagem na saída (entrada da Freedom) co

#define MINVI 0.0040  // Voltagem na saída (entrada da Freedom) com 0 mA
#define MAXVI 0.9965  // Voltagem na saída (entrada da Freedom) com 100 mA
// Fim valores determinados experimentalmente

#define VECTOR_SIZE 100

#define START_MEAS 1

// #define R1 (10 * 1000)
// #define R2 (10 * 1000)
// #define R3 (20 * 1000)

namespace PSI {
	enum InputType_t {
		DC_VOLT = 0,
		AC_VOLT,
		DC_CURR,
		DEFASAGEM,
		IMPEDANCIA
	};

	class Multimetro {
	public:
		Multimetro();

		Wave getInput(InputType_t input);

		InputType_t getInputType();

		// TextLCD lcd;

#ifndef PCDEBUG
	private:  // So nao sao privados se for enviar pro PC
#endif
		AnalogIn aIn;
		AnalogIn aIn2;
		AnalogIn currIn;
		AnalogIn pot;

	private:
		DigitalOut buzzer;   // Impedancia WHITE
		Timer medir;

		double findVrms(double ACVolts[VECTOR_SIZE], WaveForm_t& wave);  // Acha Vrms do conjunto de pontos ACVolts
		double findDef(double ACVolts1[2][VECTOR_SIZE], double ACVolts2[2][VECTOR_SIZE], WaveForm_t& wave);  // Acha defasagem dos conjuntos de pontos ACVolts1 e 2

		// Thread para ler canal 2 ao mesmo tempo
		Thread* t;
		static void threadStarter(void const*);
		void medicao();                    // Funcao auxiliar que mede a segunda onda ao mesmo tempo da primeira
	public:
		void begin();
	};
}
