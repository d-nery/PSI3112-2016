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

import controlP5.*;
import processing.serial.*;

ControlP5 control;
PImage logopoli;
Multimetro mult;
Serial port;

int time = 6400;
int ampl, deslocx, deslocy;
int input;

final short DC_VOLT = 0;
final short AC_VOLT = 1;
final short DC_CURR = 2;

Float[][] dados1, dados2;
FloatDict[] wave;

void setup() {
    fullScreen();
    //size(800, 600);

    control = new ControlP5(this);
    mult = new Multimetro();

    println(Serial.list()[0]);
    port = new Serial(this, Serial.list()[0], 9600);
    port.clear();

    logopoli = loadImage("./Poli.png");
    logopoli.resize(int(width*0.1), 0);

    dados1 = new Float[2][128];
    dados2 = new Float[2][128];

    wave = new FloatDict[2];

    wave[0] = new FloatDict();
    wave[0].set("Vrms", 0.0);
    wave[0].set("def", 0.0);
    wave[0].set("frequencia", 0.0);
    wave[0].set("periodo", 0.0);
    wave[0].set("amplitude", 0.0);
    wave[0].set("form", 0.0);

    wave[1] = new FloatDict();
    wave[1].set("Vrms", 0.0);
    wave[1].set("def", 0.0);
    wave[1].set("frequencia", 0.0);
    wave[1].set("periodo", 0.0);
    wave[1].set("amplitude", 0.0);
    wave[1].set("form", 0.0);

    for (int i = 0; i < 128; i++)
        dados1[0][i] = dados2[0][i] = dados1[1][i] = dados2[1][i] = 0.;

    control.addKnob("time")
         .setPosition(width*0.03 + logopoli.width + 20, height*0.98 - logopoli.height)
         .setRadius(50)
         .setRange(250, 12800)
         .setValue(6400)
         .setLabel("Tempo")
         .setColorForeground(color(245))
         .setColorBackground(color(90))
         .setColorActive(color(255));

    control.addKnob("ampl")
         .setPosition(width*0.03 + logopoli.width + 140, height*0.98 - logopoli.height)
         .setRadius(50)
         .setRange(1, 15)
         .setValue(10)
         .setNumberOfTickMarks(14)
         .snapToTickMarks(true)
         .setLabel("Amplitude")
         .setColorForeground(color(245))
         .setColorBackground(color(90))
         .setColorActive(color(255));

    control.addKnob("deslocy")
         .setPosition(width*0.03 + logopoli.width + 140, height*0.98 - logopoli.height + 120)
         .setRadius(50)
         .setRange(-15, 15)
         .setValue(0)
         .setNumberOfTickMarks(30)
         .snapToTickMarks(true)
         .setLabel("Offset Y")
         .setColorForeground(color(245))
         .setColorBackground(color(90))
         .setColorActive(color(255));

    control.addKnob("deslocx")
         .setPosition(width*0.03 + logopoli.width + 20, height*0.98 - logopoli.height + 120)
         .setRadius(50)
         .setRange(0, 12000)
         .setValue(0)
         .setLabel("Offset X")
         .setColorForeground(color(245))
         .setColorBackground(color(90))
         .setColorActive(color(255));

    port.bufferUntil('\n');
}

void draw() {
    background(150);
    mult.update();
    delay(1);
}

void serialEvent(Serial p) {
    int i;
    String temp;

    temp = p.readString();
    temp = trim(temp);
    //println(temp);
    String[] vals = split(temp, ',');
    //println(vals.length);
    if (vals.length != 8)
        return;
    //println(vals);
    input = int(vals[0]);
    if (vals[7].equals("d")) {
        if (vals[4].equals("1")) {
            i = int(vals[3]);
            dados1[0][i] = float(vals[1]);
            dados1[1][i] = float(vals[2]);
        } else if (vals[4].equals("2")) {
            i = int(vals[3]);
            dados2[0][i] = float(vals[1]);
            dados2[1][i] = float(vals[2]);
        }
    } else if (vals[7].equals("w")) {
        i = int(vals[6]);
        wave[i].set("Vrms", float(vals[1]));
        wave[i].set("def", float(vals[2]));
        wave[i].set("frequencia", float(vals[3]));
        wave[i].set("periodo", float(vals[4]));
        wave[i].set("amplitude", float(vals[5]));
    }
}


class Multimetro {
    Visor telaOnda;
    Canal telaCh1;
    Canal telaCh2;
    Info outros;

    Multimetro() {
        telaOnda = new Visor   (new Float[]{height*0.85, width*0.65, width*0.30, height*0.05});
        telaCh1  = new Canal(1, new Float[]{height*0.15, width*0.20, width*0.05, height*0.13});
        telaCh2  = new Canal(2, new Float[]{height*0.15, width*0.20, width*0.05, height*0.34});
        outros   = new Info    (new Float[]{height*0.10, width*0.20, width*0.05, height*0.55});
    }

    void update() {
        telaOnda.show();
        if (input == AC_VOLT)
            telaOnda.points();
        if (input != DC_CURR) {
            telaCh1.update();
            telaCh2.update();
        }
        outros.update();
    }
}

class Canal {
    float esquerda;
    float direita;
    float topo;
    float base;
    float tambase;
    float tamlat;

    int num;

    Canal(int num, Float[] meas) {
        tamlat   = meas[0];
        tambase  = meas[1];
        esquerda = meas[2];
        direita  = esquerda + tambase;
        topo     = meas[3];
        base     = topo + tamlat;

        this.num = num;
    }

    void update() {
        noStroke();
        fill(160);
        rect(esquerda, topo, tambase, tamlat);

        stroke(255);
        noFill();
        line(esquerda + 30, topo, esquerda + 30, base);

        fill(255);

        pushMatrix();
            textAlign(CENTER, CENTER);
            textSize(20);
            translate(esquerda + 15, (base+topo)/2);
            rotate(radians(-90));
            text("Canal "+nf(num), 0, 0);
        popMatrix();

        textSize(18);
        textAlign(LEFT, CENTER);

        pushMatrix();
            translate(esquerda + 40, topo);
            if (input == AC_VOLT) {
                text("Amplitude: "    + nf(wave[num-1].get("amplitude"),  0, 2) + "V",  0, 1*(tamlat)/5);
                text("Período: "      + nf(wave[num-1].get("periodo"),    0, 2) + "ms", 0, 2*(tamlat)/5);
                text("Frequencia: "   + nf(wave[num-1].get("frequencia"), 0, 1) + "Hz", 0, 3*(tamlat)/5);
                text("Valor Eficaz: " + nf(wave[num-1].get("Vrms"),       0, 2) + "V",  0, 4*(tamlat)/5);
            } else {
                text("Tensão DC: " + nf(wave[num-1].get("Vrms"),       0, 2) + "V",  0, 1*(tamlat)/2);
            }
        popMatrix();
    }
}

class Info {
    float esquerda;
    float direita;
    float topo;
    float base;
    float tambase;
    float tamlat;

    Info(Float[] meas) {
        tamlat   = meas[0];
        tambase  = meas[1];
        esquerda = meas[2];
        direita  = esquerda + tambase;
        topo     = meas[3];
        base     = topo + tamlat;
    }

    void update() {
        noStroke();
        fill(160);
        rect(esquerda, topo, tambase, tamlat);

        stroke(255);
        noFill();
        line(esquerda + 30, topo, esquerda + 30, base);

        fill(255);

        textSize(60);
        textAlign(CENTER, TOP);
        text("PSImetro", width*0.15, height*0.04);
        textSize(15);
        text("Muito mais que um multimetro", width*0.15, height*0.04 + 65);

        pushMatrix();
            textAlign(CENTER, CENTER);
            textSize(20);
            translate(esquerda + 15, (base+topo)/2);
            rotate(radians(-90));
            text("Outros", 0, 0);
        popMatrix();

        textSize(18);
        textAlign(LEFT, CENTER);

        pushMatrix();
            translate(esquerda + 40, topo);
            if (input == AC_VOLT) {
                text("Defasagem: " + nf(wave[1].get("def")) + "°", 0, 1*(tamlat)/3);
                text("Impedancia: Ω", 0, 2*(tamlat)/3);
            }
            if (input == DC_CURR)
                text("Corrente: " + nf(wave[0].get("Vrms")) + "mA", 0, 1*(tamlat)/2);
        popMatrix();

        textSize(32);
        textAlign(CENTER, TOP);
        String text = (input == 1 ? "AC" : "DC");
        text("Modo " + text, (esquerda + direita)/2, base + 20);
    }
}

class Visor {
    float esquerda;
    float direita;
    float topo;
    float base;
    float tambase;
    float tamlat;

    float minX, maxX;
    float minY, maxY;

    Visor(Float[] meas) {
        tamlat   = meas[0];
        tambase  = meas[1];
        esquerda = meas[2];
        direita  = esquerda + tambase;
        topo     = meas[3];
        base     = topo + tamlat;

        minX = 0;
        maxX = time;
        minY = -5;
        maxY = 5;
    }

    void show() {
        image(logopoli, width*0.02, height*0.98 - logopoli.height);
        noStroke();
        fill(255);
        rect(esquerda, topo, tambase, tamlat);

        stroke(0);
        noFill();
        line(esquerda, (base+topo)/2, direita, (base+topo)/2);
        stroke(150);
        pushMatrix();
            translate(esquerda, topo);
            line(0, 1*tamlat/4, tambase, 1*tamlat/4);
            line(0, 3*tamlat/4, tambase, 3*tamlat/4);
            line(tambase/2, 0, tambase/2, tamlat);
        popMatrix();

        textSize(20);
        textAlign(CENTER, BOTTOM);
        text("Controles", (width*0.03 + logopoli.width + 130), height*0.98 - logopoli.height - 10);
        textSize(20);
        textAlign(RIGHT, CENTER);
        pushMatrix();
            translate(esquerda - 5, topo);
            text(nf((minY + maxY)/2), 0, 2*(tamlat)/4);
            text(nf(((minY + maxY)/2 + maxY)/2),0, 1*(tamlat)/4);
            text(nf(((minY + maxY)/2 + minY)/2),0, 3*(tamlat)/4);
        popMatrix();
        textAlign(RIGHT, TOP);
        text(nf(maxY), esquerda - 5, topo);
        textAlign(RIGHT, BOTTOM);
        text(nf(minY), esquerda - 5, base);

        textAlign(LEFT, TOP);
        text(nf(minX), esquerda, base + 5);
        textAlign(RIGHT, TOP);
        text(nf(maxX), direita, base + 5);
        textAlign(CENTER, TOP);
        text(nf((minX + maxX)/2), (esquerda + direita)/2, base + 5);

        textAlign(CENTER, TOP);
        text("Tempo (us)", (esquerda+direita)/2, base + 40);

        pushMatrix();
            textAlign(CENTER, CENTER);
            translate(esquerda - 55, (base+topo)/2);
            rotate(radians(-90));
            text("Tensão (V)", 0, 0);
        popMatrix();
    }

    void points() {
        float x1, y1, x2, y2;
        boolean p1out, p2out;
        minX = deslocx;
        maxX = time + deslocx;
        minY = -ampl - deslocy;
        maxY =  ampl - deslocy;

        noStroke();
        fill(0, 0, 255);
        for (int i = 0; i < 127; i++) {
            x1 = map(dados1[0][i], minX, maxX, esquerda, direita);
            y1 = map(dados1[1][i], minY, maxY, base, topo);
            x2 = map(dados1[0][i+1], minX, maxX, esquerda, direita);
            y2 = map(dados1[1][i+1], minY, maxY, base, topo);

            p1out = (x1 > direita || x1 < esquerda || y1 < topo || y1 > base);
            p2out = (x2 > direita || x2 < esquerda || y2 < topo || y2 > base);

            // Se ambos pontos estao fora do retangulo, nada acontece
            if (p1out && p2out || x1 > x2)
                continue;

            // Se o primeiro ponto esta dentro, desenha uma circulo
            if (!p1out) {
                noStroke();
                fill(0, 0, 255);
                ellipse(x1, y1, 10, 10);
            } else { // Se o primeiro ponto esta fora
                if (x1 < esquerda) {
                    y1 = ((y2 - y1)*(esquerda - x1))/(x2 - x1) + y1;
                    x1 = esquerda;
                } else {
                    x1 = (y1 < topo ? ((y1-topo)*(x2-x1))/(y1-y2) + x1 : x1);
                    x1 = (y1 > base ? ((y1-base)*(x2-x1))/(y1-y2) + x1 : x1);
                    y1 = (y1 < topo ? topo : y1);
                    y1 = (y1 > base ? base : y1);
                }
            }

            // Se o segundo ponto esta pra fora
            if (p2out) {
                if (x2 > direita) { // Saiu pela direita
                    y2 = ((y2 - y1)*(direita - x1))/(x2 - x1) + y1;
                    x2 = direita;
                } else { // Saiu pelo topo ou pela base
                    x2 = (y2 < topo ? ((y1-topo)*(x2-x1))/(y1-y2) + x1 : x2);
                    x2 = (y2 > base ? ((y1-base)*(x2-x1))/(y1-y2) + x1 : x2);
                    y2 = (y2 < topo ? topo : y2);
                    y2 = (y2 > base ? base : y2);
                }
            }


            stroke(0, 0, 255);
            noFill();
            line(x1, y1, x2, y2);
        }
        fill(255, 0, 0);
        for (int i = 0; i < 127; i++) {
            x1 = map(dados2[0][i], minX, maxX, esquerda, direita);
            y1 = map(dados2[1][i], minY, maxY, base, topo);
            x2 = map(dados2[0][i+1], minX, maxX, esquerda, direita);
            y2 = map(dados2[1][i+1], minY, maxY, base, topo);

            p1out = (x1 > direita || x1 < esquerda || y1 < topo || y1 > base);
            p2out = (x2 > direita || x2 < esquerda || y2 < topo || y2 > base);

            // Se ambos pontos estao fora do retangulo, nada acontece
            if (p1out && p2out || x1 > x2)
                continue;

            // Se o primeiro ponto esta dentro, desenha uma circulo
            if (!p1out) {
                noStroke();
                fill(255, 0, 0);
                ellipse(x1, y1, 10, 10);
            } else { // Se o primeiro ponto esta fora
                if (x1 < esquerda) {
                    y1 = ((y2 - y1)*(esquerda - x1))/(x2 - x1) + y1;
                    x1 = esquerda;
                } else {
                    x1 = (y1 < topo ? ((y1-topo)*(x2-x1))/(y1-y2) + x1 : x1);
                    x1 = (y1 > base ? ((y1-base)*(x2-x1))/(y1-y2) + x1 : x1);
                    y1 = (y1 < topo ? topo : y1);
                    y1 = (y1 > base ? base : y1);
                }
            }

            // Se o segundo ponto esta pra fora
            if (p2out) {
                if (x2 > direita) { // Saiu pela direita
                    y2 = ((y2 - y1)*(direita - x1))/(x2 - x1) + y1;
                    x2 = direita;
                } else { // Saiu pelo topo ou pela base
                    x2 = (y2 < topo ? ((y1-topo)*(x2-x1))/(y1-y2) + x1 : x2);
                    x2 = (y2 > base ? ((y1-base)*(x2-x1))/(y1-y2) + x1 : x2);
                    y2 = (y2 < topo ? topo : y2);
                    y2 = (y2 > base ? base : y2);
                }
            }


            stroke(255, 0, 0);
            noFill();
            line(x1, y1, x2, y2);
        }
    }
}