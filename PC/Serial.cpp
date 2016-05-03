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

#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <cstring>

#include "Serial.hpp"

namespace PSI {
	Serial::Serial(std::string dev) {
		if ((fd = open(dev.data(), O_RDWR | O_NOCTTY)) < 0)
			throw std::runtime_error("Error opening serial");

		std::cerr << "Serial opened with file descriptor: " << fd << std::endl;

		usleep(3500 * 1000);

		tcgetattr(fd, &tty);
		bzero(&tty, sizeof(tty));

		cfsetispeed(&tty, B115200);
		cfsetospeed(&tty, B115200);

		// 115200 baud, 8 bits, 1 stop bit, no parity
		// https://developer.mbed.org/handbook/Terminals

		tty.c_cflag &= ~PARENB;
		tty.c_cflag &= ~CSTOPB;
		tty.c_cflag &= ~CSIZE;
		tty.c_cflag |= CS8;

		tty.c_lflag |= ICANON;

	  	tcsetattr(fd, TCSANOW, &tty);
	}

	Serial::~Serial() {
		close(fd);
	}

	int Serial::read(std::string& str) {
		int n = 0;

		char buf[256];
		memset(buf, '\0', sizeof buf);

		n = ::read(fd, buf, 255);
		buf[n] = 0;

		str = buf;
		str.pop_back(); // Remove \r\n
		str.pop_back();

		return n;
	}

	void Serial::write(std::string data) {
		int i = atoi(data.data());
		char c = i;
		::write(fd, &c, 1);
	}
}
