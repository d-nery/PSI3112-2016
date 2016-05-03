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

#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <string>

#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

namespace PSI {
	class Serial {
	private:
		int fd;
		struct termios tty;

	public:
		Serial(std::string dev);
		~Serial();
		int read(std::string& str);
		void write(std::string data);
	};
}
