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

import processing.serial.*;

Serial port;

int input;

FloatDict wave1;
FloatDict wave2;

Float[] meas;

String text;

PImage logopoli, logopsi;

void setup() {
    //fullScreen();
    size(800, 600);
    printArray(Serial.list());

    port = new Serial(this, Serial.list()[0], 9600);
    port.clear();

    text = port.readStringUntil('\n');
    text = null;

    logopoli = loadImage("./Poli.png");
    logopoli.resize(100, 114);
    //logopsi = loadImage("psi.png");

    wave1 = new FloatDict();
    wave1.set("Vrms", 0.0);
    wave1.set("def", 0.0);
    wave1.set("frequencia", 0.0);
    wave1.set("periodo", 0.0);
    wave1.set("amplitude", 0.0);
    wave1.set("form", 0.0);

    wave2 = new FloatDict();
    wave2.set("Vrms", 0.0);
    wave2.set("def", 0.0);
    wave2.set("frequencia", 0.0);
    wave2.set("periodo", 0.0);
    wave2.set("amplitude", 0.0);
    wave2.set("form", 0.0);
    
    meas = new Float[128];
}

void draw() {
    background(255, 255, 255);
    image(logopoli, width - logopoli.width - 10, height - logopoli.height - 10);
    //image(logopoli, 10, 10);

    textSize(60);
    fill(29, 34, 232);
    textAlign(CENTER, CENTER);
    text("PSÍmetro", width/2, height/2 - 150 - 30);

    reader();
    textSize(30);
    switch(input) {
    //  switch(1) {
        case 0:
            text("Tensão DC Ch1: " + nf(wave1.get("Vrms"), 0, 2) + "V", width/2, height/2 - 150 + 35);
            text("Tensão DC Ch2: " + nf(wave2.get("Vrms"), 0, 2) + "V", width/2, height/2 - 150 + 85);
            break;

        case 3:

        case 1:
            text("Tensão AC", width/2, height/2 - 150 + 25);
            text("Canal 1", width/4, height/2 - 150 + 85);
            text("Canal 2", 3*width/4, height/2 - 150 + 85);
            text("Defasagem: " + nf(wave2.get("def"), 0, 1) + "°", width/2, height - 150 + 30);
            line(width/2, height/2 - 150 + 50, width/2, height - 150);

            textSize(20);
            text("Amplitude: " + nf(wave1.get("amplitude"), 0, 2) + "V", width/4, height/2 - 150 + 120);
            text("Período: " + nf(wave1.get("periodo"), 0, 2) + "ms", width/4, height/2 - 150 + 150);
            text("Frequência: " + nf(wave1.get("frequencia"), 0, 1) + "Hz", width/4, height/2 - 150 + 180);
            text("Valor Eficaz: " + nf(wave1.get("Vrms"), 0, 2) + "V", width/4, height/2 - 150 + 210);

            text("Amplitude: " + nf(wave2.get("amplitude"), 0, 2) + "V", 3*width/4, height/2 - 150 + 120);
            text("Período: " + nf(wave2.get("periodo"), 0, 2) + "ms", 3*width/4, height/2 - 150 + 150);
            text("Frequência: " + nf(wave2.get("frequencia"), 0, 1) + "Hz", 3*width/4, height/2 - 150 + 180);
            text("Valor Eficaz: " + nf(wave2.get("Vrms"), 0, 2) + "V", 3*width/4, height/2 - 150 + 210);
            break;

        case 2:
            text("Corrente DC: " + nf(round(wave1.get("Vrms"))) + "mA", width/2, height/2 - 150 + 35);
            break;

        //cas;e 3:
            //text("Impedancia: " + nf(wave1[0], 0, 2) + "ohm", width/2, height/2 - 150 + 35);
            //break;
    }
    delay(100);
    //fill(0, 102, 153, 51);
    //text("word", 10, 90);
}

void reader() {
   char c;
   String temp;

   //for (;;) {
       while (port.available() > 0) {
           temp = port.readStringUntil('\n');
           if (temp != null) {
               temp = trim(temp);
               println(temp);
               if (temp.length() > 0) {
                   c = temp.charAt(temp.length() - 1);
                   //println(c);
                   switch (c) {
                       case 'i':
                           input = int(temp.substring(0, temp.length() - 1));
                           break;

                       case 'q': //wave1
                           wave1.set("Vrms", float(temp.substring(0, temp.length() - 1)));
                           //println(wave1.get("Vrms"));
                           break;

                       case 'e':
                           wave1.set("frequencia", float(temp.substring(0, temp.length() - 1)));
                           break;

                       case 'r':
                           wave1.set("periodo", float(temp.substring(0, temp.length() - 1)));
                           break;

                       case 't': // Value read
                           wave1.set("amplitude", float(temp.substring(0, temp.length() - 1)));
                           break;

                       case 'z': // Value read
                           wave1.set("form", float(temp.substring(0, temp.length() - 1)));
                           break;

                       case 'a': //wave1
                           wave2.set("Vrms", float(temp.substring(0, temp.length() - 1)));
                           break;

                       case 'd':
                           wave2.set("frequencia", float(temp.substring(0, temp.length() - 1)));
                           break;

                       case 'f':
                           wave2.set("periodo", float(temp.substring(0, temp.length() - 1)));
                           break;

                       case 'g': // Value read
                           wave2.set("amplitude", float(temp.substring(0, temp.length() - 1)));
                           break;

                       case 'x': // Value read
                           wave2.set("form", float(temp.substring(0, temp.length() - 1)));
                           break;

                       case 'h':
                           wave2.set("def", float(temp.substring(0, temp.length() - 1)));

                   }
               }
           }
           //delay(100);
       //}
   }
}