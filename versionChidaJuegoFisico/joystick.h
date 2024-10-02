#ifndef Joystick_h
#define Joystick_h

#define VERT_PIN 32
#define HORZ_PIN 33
#define SEL_PIN 12

#define MAX_JOYSTICK 4095
#define MIN_JOYSTICK 500

struct valoresJoystick {
  int x;
  int y;
  int btnJoystick;
};

class Joystick {
  public: 
    void inicializar();
    valoresJoystick obtenerValores(); 
  
  private:
    void actualizarValores();
    valoresJoystick valores;
};

void Joystick::inicializar() {
  pinMode(VERT_PIN, INPUT);
  pinMode(HORZ_PIN, INPUT);
  pinMode(SEL_PIN, INPUT_PULLUP);
}

void Joystick::actualizarValores() {
  int x = analogRead(HORZ_PIN);
  int y = analogRead(VERT_PIN);
  int btnJoystick = !digitalRead(SEL_PIN);

  valores.x = x;
  valores.y = y;
  valores.btnJoystick = btnJoystick;
}

valoresJoystick Joystick::obtenerValores() {
  actualizarValores();
  return valores;
}

#endif