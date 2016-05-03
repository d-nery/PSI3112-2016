#include "Multimetro.hpp"

namespace PSI {
	Multimetro::Multimetro() : aIn(ADC_VOLT_IN) {}

	// void Multimetro::update() {
	// }

	double Multimetro::getInput(InputType_t input) {
		switch(input) {
			case AC_VOLT:
				ACVolt = aIn.read();
				return ACVolt;

			case DC_VOLT:
				DCVolt = 0;
				for (int i = 0; i < 1000; i++) {
					DCVolt += aIn.read();
				}
				DCVolt /= 1000;
				DCVolt = DCVolt * VCC;
				DCVolt = R3 * ((1./R1 + 1./R2 + 1./R3) * DCVolt - double(VCC)/R1);
				return DCVolt;

			case DC_CURR:
				DCCurrent = aIn.read();
				return DCCurrent;
		}
		return 0.;
	}
}
