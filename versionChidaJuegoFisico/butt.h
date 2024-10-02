#ifndef Butt_h
#define Butt_h

class Boton {
  private:
    int pin;

  public:
    // Constructor
    Boton(int _pin) {
      pin = _pin;
    }

    // Métodos
    void inicializar();
    int estado();
};

// Definición de los métodos fuera de la clase
void Boton::inicializar() {
  pinMode(pin, INPUT_PULLUP);
}

int Boton::estado() {
  return !digitalRead(pin); // Cambiado a digitalRead para leer el estado del pin
}

#endif