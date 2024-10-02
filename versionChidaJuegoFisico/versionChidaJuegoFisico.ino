#include "jugar.h"
Jugar jugar;

void setup() {
  Serial.begin(115200); 
  jugar.inicializarJuego();
}

void loop() {
  jugar.loopJuego();
}