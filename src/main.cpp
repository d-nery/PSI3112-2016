#include "mbed.h"
#include "pins.h"

#include "Multimetro.hpp"

using namespace PSI;

Multimetro mult;

Serial pc(USBTX, USBRX);

int main() {
	double volt;

	for (;;) {
		volt = mult.getInput(DC_VOLT);
		pc.printf("Valor raw:  %.2f\r\n", mult.aIn.read());
		pc.printf("Valor lido: %.2f\r\n", volt);
		wait(0.3);
	}
}
