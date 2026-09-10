//teoria de ver cor dos pisos
/*
1. cores:
  azul
  vermelho
  prata
  preto
Azul:
  parar por 5 s
Preto:
  nao pode passar mais de 50% do robo
Prata:
  checkpoint
*/
#include <Wire.h>
#include "Adafruit_TCS34725.h"
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
int rp_min, rp_max, gp_min, gp_max

                              rpr_min = 1;
rpr_max = 10;
gpr_min = 1;
gpr_max = 10;
bpr_min = 1;
bpr_max = 10;

rsil_min = 55;
rsil_max = 63;
gsil_min = 48;
gsil_max = 51;
bsil_min = 29;
bsil_max = 33;

raz_min = 55;
raz_max = 63;
gaz_min = 48;
gaz_max = 51;
baz_min = 29;
baz_max = 33;

bool calibrar = 0

  bool
  azul() {
  //Azul
  if (r < raz_min && r > raz_max && g < gaz_min && g > gaz_max && b < baz_min && b > baz_max) {
    return 1;
  }
  return 0;
}

bool preto() {
  if (r > rp_min && r < rp_max && g > gp_min && g < gp_max && b > bp_min && b < bp_max) {
    return 1;
  }
  return 0;
}

bool silver() {
  if (r < rsil_min && r > rsil_max && g < gsil_min && g > gsil_max && b < bsil_min && b > bsil_max) {
    return 1;
  }
  return 0;
}

void setup(void) {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1)
      ;
  }
}

void loop() {

  // put your main code here, to run repeatedly:
  uint16_t r, g, b, c, colorTemp, lux;

//calibra
  while (calibrar) {
    tcs.getRawData(&r, &g, &b, &c);
    // colorTemp = tcs.calculateColorTemperature(r, g, b);
    colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
    lux = tcs.calculateLux(r, g, b);

    Serial.print("Color Temp: ");
    Serial.print(colorTemp, DEC);
    Serial.print(" K - ");
    Serial.print("Lux: ");
    Serial.print(lux, DEC);
    Serial.print(" - ");
    Serial.print("R: ");
    Serial.print(r, DEC);
    Serial.print(" ");
    Serial.print("G: ");
    Serial.print(g, DEC);
    Serial.print(" ");
    Serial.print("B: ");
    Serial.print(b, DEC);
    Serial.print(" ");
    Serial.print("C: ");
    Serial.print(c, DEC);
    Serial.print(" ");
    Serial.println(" ");
  }

//enquanto n calibra
  while (!calibrar) {
    tcs.getRawData(&r, &g, &b, &c);
    // colorTemp = tcs.calculateColorTemperature(r, g, b);
    colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
    lux = tcs.calculateLux(r, g, b);

    //Preto
    if (preto()) {
      tras(1800);
      dir(1800);
      fre(1800);
    }

    //Prata
    if (silver()) {
      //reset
    }

    if (azul()) {
      delay(5000);
    }
  }
}
