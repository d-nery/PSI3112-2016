#pragma once

#include "mbed.h"
#include "pins.h"

#define VCC 3.3

// Ver Esquemático
#define R1 (10 * 1000)
#define R2 (10 * 1000)
#define R3 (20 * 1000)

namespace PSI {
	enum InputType_t {
		DC_VOLT = 0,
		AC_VOLT,
		DC_CURR
	};

	class Multimetro {
	public:
		Multimetro();

		// void update();

		double getInput(InputType_t input);

		AnalogIn aIn;
	private:

		double DCVolt;
		double ACVolt;
		double DCCurrent;
	};
}
