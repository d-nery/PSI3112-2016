import processing.serial.*;

final int WIDTH = 800;
final int HEIGHT = 600;

Serial port;

float[] values;
int input;
int wave;

String text;

PImage logopoli, logopsi;

void setup() {
    //fullScreen();
    size(800, 600);
    printArray(Serial.list());

    port = new Serial(this, Serial.list()[0], 115200);
    port.clear();

    text = port.readStringUntil('\n');
    text = null;

    logopoli = loadImage("Poli.png");
    logopoli.resize(100, 114);
    logopsi = loadImage("psi.png");

    values = new float[3];
    fill(255);
}

void draw() {
    reader();

    background(255, 255, 255);
    image(logopoli, 690, 476);

    textSize(60);
    fill(0, 102, 153);
    textAlign(CENTER, CENTER);
    text("PSImetro", WIDTH/2, 300 - 30);

    textSize(30);
    fill(0, 102, 153);
    switch(input) {
      case 0:
        text("Tensão DC: " + nf(values[2], 0, 2) + "V", WIDTH/2, 315);
        break;

       case 1:
        text("Tensão AC: " + nf(values[2], 0, 2) + "Vrms", WIDTH/2, 315);
        break;

       case 2:
        text("Corrente DC: " + nf(round(values[2])) + "mA", WIDTH/2, 315);
        break;

       case 3:
        text("Defasagem: " + nf(values[2], 0, 2) + "°", WIDTH/2, 315);
        break;

       case 4:
        text("Impedancia: " + nf(values[2], 0, 2) + "ohm", WIDTH/2, 315);
        break;
    }

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
                if (temp.length() > 0) {
                    c = temp.charAt(temp.length() - 1);
                    println(c);
                    switch (c) {
                        case 'i':
                            input = int(temp.substring(0, temp.length() - 1));
                            break;

                        case 'w':
                            wave = int(temp.substring(0, temp.length() - 1));
                            break;

                        case 'r': // Raw Value
                            values[0] = float(temp.substring(0, temp.length() - 1));
                            break;

                        case 'p': // Raw Potentiometer value read
                            values[1] = float(temp.substring(0, temp.length() - 1));
                            break;

                        case 'v': // Value read
                            values[2] = float(temp.substring(0, temp.length() - 1));
                            break;
                    }
                }
            }
            delay(10);
        //}
    }
}