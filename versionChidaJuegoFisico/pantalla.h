#ifndef Pantalla_h
#define Pantalla_h

#include <Wire.h>               
#include <LiquidCrystal_I2C.h>
#include "joystick.h"
#include "MicroSD.h"

#define NUM_COLS 19

LiquidCrystal_I2C lcd (0x27, 20, 4);

class Pantalla {
  public: 
    byte cabeza[8] = {
      B00000,
      B00000,
      B01111,
      B11111,
      B11111,
      B01111,
      B00000,
      B00000
    };

    byte cuerpo[8] = {
      B00000,
      B00000,
      B11111,
      B11111,
      B11111,
      B11111,
      B00000,
      B00000
    };

    byte bolas[8] = {
      B00110,
      B01111,
      B11111,
      B11110,
      B11110,
      B11111,
      B01111,
      B00110
    };

    byte pollaMenu[8] = {
      B00100,
      B01110,
      B01110,
      B01110,
      B01110,
      B01110,
      B11111,
      B11111
    };

    byte cabezaDer[8] = {
      B00000,
      B00000,
      B11110,
      B11111,
      B11111,
      B11110,
      B00000,
      B00000
    };

    byte bolasDer[8] = {
      B01100,
      B11110,
      B11111,
      B01111,
      B01111,
      B11111,
      B11110,
      B01100
    };

    byte jugador[8] = {
      B01110,
      B01101,
      B01101,
      B11110,
      B11110,
      B01111,
      B01010,
      B01010
    };

    int pantallaActual = 6;
    /*
    0 menu
    1 play
    2 highscore
    3 pause
    4 gameOver
    5 newHighscore
    6 intro
    7 maricones
    */
    
    bool pause = false;

    //Encender y crear los caracteres de la pantalla
    void inicializar();
    //Imprimir un mensaje en la pantalla
    void setText(String mensaje, int col=0, int row=0);
    void introJuego();
    void menu();
    //Poner el blanco una celda del LCD
    void limpiarCelda(int col, int row);
    void limpiar();
    void moverPartePolla(int col, int row, int numParte);
    void moverPeneMenu(int y);
    int obtenerRowPolla();
    void highscore(Highscore* highscores);
    void moverJugador(int posY);
    void folladores();
    void play(int posY);
    bool colision();
    void gameOver();
    bool hayNewHighscore(Highscore* highscores);
    void newHighscore(valoresJoystick valores);
    void arribaMaricones();

    //Publica para que pueda ser modificada por el boton de regresar o empezar juego
    int colPolla = NUM_COLS;
    int rowJugador = 3;
    int rowPolla = 2;
    int score = 0;
    int posNuevoHighscore = 0;
    char nickname[3] = {'A', 'A', 'A'};
    int delayPollas = 120;
    int indiceAbecedario = 0;
    int cursorPos = 0;

  private:

    int colJugador = 0;
    int delayJugador = 150;
    int delayCreadores = 5000;

    unsigned long ultimoMovimientoJoystick = 0;
    unsigned long ultimoMovimientoPolla = 0;


    bool limpiarIntro = true;

    char abecedario[26] = {
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 
      'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
    };

    void cambiarRowPolla(int row);
};

void Pantalla::inicializar() {
  lcd.init();
  lcd.backlight();

  lcd.createChar(0, cabeza);
  lcd.createChar(1, cuerpo);
  lcd.createChar(2, bolas);
  lcd.createChar(3, pollaMenu);
  lcd.createChar(4, cabezaDer);
  lcd.createChar(5, bolasDer);
  lcd.createChar(6, jugador);
};

void Pantalla::setText(String mensaje, int col, int row) {
  //lcd.clear();
  lcd.setCursor(col, row);
  lcd.print(mensaje);
};

void Pantalla::introJuego(){
  unsigned long tiempoActual = millis();

  if (tiempoActual < delayCreadores) {
    setText("BIENVENIDO", 5, 0);
    setText("A LA", 7, 1);
    setText("POLLA QUE TE FOLLA", 0, 2);

    lcd.setCursor(8, 3);
    lcd.write(0);
    lcd.setCursor(9, 3);
    lcd.write(1);
    lcd.setCursor(10, 3);
    lcd.write(2);
  }
  else {
    if(limpiarIntro){
      lcd.clear();
      limpiarIntro = false;
    }

    setText("By", 8, 0);
    setText("Hanzeel", 6, 1);
    setText("&", 9, 2);
    setText("Fernando", 6, 3);
  }
}

void Pantalla::menu() {
  lcd.setCursor(3, 0);
  lcd.print("Menu");
  lcd.setCursor(3, rowPolla);
  lcd.write(5); //BolasDer
  lcd.setCursor(4, rowPolla);
  lcd.write(1); //cuerpo
  lcd.setCursor(5, rowPolla);
  lcd.write(4); //CabezaDer
  lcd.setCursor(7, 2);
  lcd.print("Play");
  lcd.setCursor(7, 3);
  lcd.print("Highscores");
}

void Pantalla::limpiarCelda(int col, int row){
  lcd.setCursor(col, row);
  lcd.print(' '); //blanco
}

void Pantalla::limpiar() {
  lcd.clear();
}

void Pantalla::cambiarRowPolla(int row) {
  rowPolla = row;
}

void Pantalla::moverPartePolla(int col, int row, int numParte) {
  lcd.setCursor(col, row);
  lcd.write(numParte);
}

void Pantalla::moverPeneMenu(int y) {
  //Si la polla quiere ir para arriba y que no se salga de las opciones del menu
  if(y < MIN_JOYSTICK && rowPolla < 4){
    limpiarCelda(3, rowPolla);
    limpiarCelda(4, rowPolla); 
    limpiarCelda(5, rowPolla);
    cambiarRowPolla(3);

    Serial.println("Mover polla abajo");
  }
  else if (y == MAX_JOYSTICK && rowPolla > 1) {
    limpiarCelda(3, rowPolla);
    limpiarCelda(4, rowPolla); 
    limpiarCelda(5, rowPolla);
    cambiarRowPolla(2);

    Serial.println("Mover polla arriba");
  }
    //Mover la polla en la nueva posicion
    moverPartePolla(3, rowPolla, 5); //bolasDer
    moverPartePolla(4, rowPolla, 1); //bolasDer
    moverPartePolla(5, rowPolla, 4); //bolasDer 
}

int Pantalla::obtenerRowPolla(){
  return rowPolla;
}

void Pantalla::highscore(Highscore* highscores) {
  for (int i = 0; i < 4; i++) {
    
    lcd.setCursor(0, i);

    // Imprimir el nombre de usuario y el puntaje en una sola línea
    lcd.print(highscores[i].username); // Imprimir el nombre del usuario
    lcd.print(" ");                    // Espacio separador
    lcd.print(highscores[i].score);    // Imprimir el puntaje
  }
}

void Pantalla::moverJugador(int posY) {
  unsigned long tiempoActual = millis();

  if (tiempoActual - ultimoMovimientoJoystick > delayJugador) {
    if (posY == MAX_JOYSTICK && rowJugador > 0){
      rowJugador--;
      limpiarCelda(colJugador, rowJugador + 1);
    }
    else if (posY < MIN_JOYSTICK && rowJugador < 3){
      rowJugador++;
      limpiarCelda(colJugador, rowJugador - 1);
    }

    //Serial.print("Mover jugador a: ");
    //Serial.println(rowJugador);
    lcd.setCursor(colJugador, rowJugador);
    lcd.write(6); //jugador

    //actualizar el ultimo movimiento
    ultimoMovimientoJoystick = tiempoActual;
  }
}

void Pantalla::folladores() {
  unsigned long tiempoActual = millis();

  if(tiempoActual - ultimoMovimientoPolla > delayPollas){
    if (colPolla > 1) {
      lcd.setCursor(colPolla - 2, rowPolla);
      lcd.write(0); //cabeza
      lcd.setCursor(colPolla - 1, rowPolla);
      lcd.write(1); //cuerpo
      lcd.setCursor(colPolla, rowPolla);
      lcd.write(2); //bolas
    }

    limpiarCelda(colPolla + 2, rowPolla); //limpiar las bolas

    if(colPolla < 0){ 
      limpiarCelda(0, rowPolla);
      colPolla = NUM_COLS;
      rowPolla = random(4);
      score++;
      Serial.print("Score: ");
      Serial.println(score);

      //aumentar velocidad
      if (score > 10 && delayPollas > 70) {
        delayPollas -= 10;
      }
    }
    else {
      colPolla--;
    }

    ultimoMovimientoPolla = tiempoActual;
  }
}

void Pantalla::play(int posY) {
  //rowPolla = 3;
  moverJugador(posY);
  folladores();
}

bool Pantalla::colision() {
  return (rowJugador == colPolla && rowPolla == rowJugador);
}

void Pantalla::gameOver() {
  setText("Te follaron", 3, 0);
  setText("Score", 5, 1);
  
  lcd.setCursor(8, 2);
  lcd.print(score);
}

bool Pantalla::hayNewHighscore(Highscore* highscores) {
  for (int i = 0; i < 4; i++) {
    if (score > highscores[i].score) {
      posNuevoHighscore = i;

      for (int j = 3; j > i; j--) {
        highscores[j] = highscores[j - 1];
      }

      return true;
    }
    //Serial.println("Hola desde el ciclo");
  }
  return false;
}

void Pantalla::newHighscore(valoresJoystick valores) {
  unsigned long tiempoActual = millis();

  if (tiempoActual - ultimoMovimientoJoystick > delayJugador) {
    // Mostrar las etiquetas en la pantalla
    setText("Nuevo highscore");
    setText("Nickname:", 0, 1);
    setText("Max 3 chars", 0, 2);

    // Cambiar la letra con el joystick hacia arriba/abajo
    if (valores.y < MIN_JOYSTICK) {
      indiceAbecedario++;
    }
    
    if (valores.y == MAX_JOYSTICK) {
      indiceAbecedario--;
    }

    // Asegurarse de que el índice del abecedario esté dentro del rango
    if (indiceAbecedario >= 26)
      indiceAbecedario = 0;
    if (indiceAbecedario < 0)
      indiceAbecedario = 25;

    if (valores.btnJoystick) {
      nickname[cursorPos] = abecedario[indiceAbecedario];
      if (cursorPos < 2) {
        cursorPos++;
        indiceAbecedario = 0;
      }
        
    }

    // Mostrar el nickname almacenado hasta ahora en la pantalla
    for (int i = 0; i < 3; i++) {
      lcd.setCursor(i, 3);
      lcd.print(nickname[i]);  // Mostrar la letra previamente seleccionada
    }

    // Mostrar la letra actual seleccionada en la posición del cursor
    lcd.setCursor(cursorPos, 3);
    lcd.print(abecedario[indiceAbecedario]);

    // Actualizar el tiempo del último movimiento
    ultimoMovimientoJoystick = tiempoActual;
  }
}

void Pantalla::arribaMaricones() {
  setText("Arriba Maricones", 3, 0);
  lcd.setCursor(8, 3);
  lcd.write(0);
  lcd.setCursor(9, 3);
  lcd.write(1);
  lcd.setCursor(10, 3);
  lcd.write(2);
}

#endif