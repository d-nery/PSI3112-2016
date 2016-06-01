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

#define ADC_VOLT_IN PTB0
#define DEF_IN      PTB1
#define POT_IN      PTB2
#define CURR_IN     PTC1
#define BUZZER      PTE3

// LCD pins
#define LCD_RX PTD5
#define LCD_E  PTA13
#define LCD_D4 PTC9
#define LCD_D5 PTC8
#define LCD_D6 PTA5
#define LCD_D7 PTA4
