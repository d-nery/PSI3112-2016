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

#pragma once

#include "mbed.h"
// #include "TextLCD.h"

#include "Wave.hpp"

#define VCC 3.3

// Ver Esquemático
#define R1 (09.69 * 1000)
#define R2 (09.83 * 1000)
#define R3 (32.56 * 1000)

#define Rshunt 32
#define RImp   1000

// Valores determinados experimentalmente
#define MINV  0.448144  // Tensão na saída (entrada da Freedom) com 0V
#define MAXV  0.882387  // Tensão na saída (entrada da Freedom) com 10V

#define MINV2 0.452034  // Tensão na saída (entrada da Freedom) com 0V
#define MAXV2 0.863714  // Tensão na saída (entrada da Freedom) com 10V

#define MINVI 0.258238  // Tensão na saída (entrada da Freedom) com 0 mA
#define MAXVI 0.735067  // Tensão na saída (entrada da Freedom) com 100 mA
// Fim valores determinados experimentalmente

#define VECTOR_SIZE 128

// #define R1 (10 * 1000)
// #define R2 (10 * 1000)
// #define R3 (20 * 1000)

namespace PSImetro {
	enum InputType_t {  // Tipos de Entrada
		DC_VOLT = 0,
		AC_VOLT,
		DC_CURR
		// DEFASAGEM,
		// IMPEDANCIA
	};

	class Multimetro {
	public:
		Multimetro();

		void getInput(InputType_t input, Wave& wave1, Wave& wave2);

		InputType_t getInputType();

#ifndef PCDEBUG
	private:  // So nao sao privados se for enviar pro PC
#endif
		AnalogIn aIn;
		AnalogIn aIn2;
		AnalogIn currIn;
		AnalogIn pot;

	private:
		DigitalOut buzzer;
		Timer medir;

		double ACVolts1[2][VECTOR_SIZE];  // guarda as últimas VECTOR_SIZE medicoes das entrada 1 e seu tempo
		double ACVolts2[2][VECTOR_SIZE];  // guarda as últimas VECTOR_SIZE medicoes das entrada 2 e seu tempo

		void findVrms(Wave& wave1, Wave& wave2);        // Acha Vrms do conjunto de pontos ACVolts1 e 2
		void findDef(Wave& wave1, Wave& wave2);         // Acha defasagem dos conjuntos de pontos ACVolts1 e 2
		// void findImpedance(Wave& wave1, Wave& wave2);   // Acha a impedancia medida no canal1 com base na tensao no canal 2

		// Status LEDs
		DigitalOut led_dcv;   // DC Voltage RED
		DigitalOut led_dcc;   // DC Current GREEN
		DigitalOut led_acv;   // AC Voltage YELLOW
	};
}
