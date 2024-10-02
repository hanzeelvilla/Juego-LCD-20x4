#ifndef Jugar_h;
#define Jugar_h;

#include "pantalla.h"
#include "butt.h"
#include "musica.h"

Pantalla gameScreen;
Joystick joystick;
MicroSD MSD;
Boton btnB(17);
Boton btnA(16);
Musica musica;

int delayIntro = 10000;
int delayLost = 5000;
int delayPausa = 1000;

Highscore* highscores;

class Jugar {
  public:
    void inicializarJuego();
    void loopJuego();
    void imprimirValoresJoystick(valoresJoystick valores);

  private:
    bool cancionReproducida = false;
    void limpiar(int pantalla);
    void imprimirHighscores(Highscore* highscores);
    void seleccionarCancion(String cancion);
};

void Jugar::limpiar(int pantalla) {
  if(gameScreen.pantallaActual != pantalla) {
    gameScreen.limpiar();
  }
  //cambio de pantalla
  gameScreen.pantallaActual = pantalla;
}

void Jugar::imprimirValoresJoystick(valoresJoystick valores) {
  Serial.print("X: ");
  Serial.println(valores.x);
  Serial.print("y: ");
  Serial.println(valores.y);
  Serial.print("btn joystick: ");
  Serial.println(valores.btnJoystick);
}

void imprimirHighscores(Highscore* highscores) {
  for (int i = 0; i < MAX_HIGHSCORES; i++) {
    Serial.print("Usuario: ");
    Serial.print(highscores[i].username);
    Serial.print(" - Puntaje: ");
    Serial.println(highscores[i].score);
  }
}

void Jugar::seleccionarCancion(String cancion) {
  if (!cancionReproducida) {
    Serial.println("Seleccionando y reproduciendo la canción...");
    musica.seleccionarCancion(cancion);  
    cancionReproducida = true; 
  }
}

void Jugar::inicializarJuego() {
  joystick.inicializar();
  gameScreen.inicializar();
  MSD.MicroSD_init();
  btnB.inicializar();
  btnA.inicializar();
  musica.inicializar();

  highscores = MSD.ReadJSONFile();

  if (highscores == nullptr) {
    Serial.println("No se pudo leer el archivo JSON.");
  }
}

void Jugar::loopJuego() {
  valoresJoystick valores = joystick.obtenerValores();

  switch (gameScreen.pantallaActual) {
    // Menu
    case 0:
      //Serial.println("Mostrar menu");
      gameScreen.menu();
      gameScreen.moverPeneMenu(valores.y);
      /*
      musica.seleccionarCancion("oh");  
      if (valores.y == MAX_JOYSTICK || valores.y < MIN_JOYSTICK) {
        musica.reproducir();
      }
      */

      break;
    
    // Jugar
    case 1:
      //Serial.println("Mostrar juego");
      if (!gameScreen.colision()){
        seleccionarCancion("maiAiHee8bits"); 
        musica.reproducir();

        gameScreen.play(valores.y);
      }
      else {
        musica.detener();
        if (gameScreen.hayNewHighscore(highscores)) {
          Serial.println("Nuevo highscore");
          limpiar(5);
        }
        else
          limpiar(4);
      }
      break;

    // Highscores
    case 2:
      //Serial.println("Mostrar highscores");
      gameScreen.highscore(highscores);
      //gameScreen.arribaMaricones();
      break;

    // Pausa
    case 3:
      gameScreen.setText("Pause");
      break;
    
    // Game over
    case 4:
      gameScreen.gameOver();
      break;

    case 5:
      gameScreen.newHighscore(valores);
      break;

    case 6:
      unsigned long tiempoActual = millis();
      if (tiempoActual < delayIntro) {
        gameScreen.introJuego();
        seleccionarCancion("intro");
        musica.reproducir();
      }
      else{
        limpiar(0);
        musica.detener(); 
        cancionReproducida = false;
        Serial.println("La canción ya ha sido reproducida.");
      }
      break;
  }

  if (valores.btnJoystick) {
    //Ver que opcion se selecciona del menu
    if (gameScreen.pantallaActual == 0) {
      int rowPollaMenu = gameScreen.obtenerRowPolla();

      switch (rowPollaMenu) {
        case 2:
          //pantalla actual = juego
          gameScreen.rowJugador = 3;
          limpiar(1);
          break;
        
        case 3:
          //pantalla actual = highscores
          limpiar(2);
          break;
      }
    }
  }

  if (btnB.estado()) {
    if (gameScreen.pantallaActual != 0 && gameScreen.pantallaActual != 5 && gameScreen.pantallaActual != 6) {
      limpiar(0);
      gameScreen.colPolla = NUM_COLS;
      gameScreen.rowPolla = 2;
      gameScreen.score = 0;
      musica.detener();
      gameScreen.delayPollas = 120;
      cancionReproducida = false;
    }
    else if (gameScreen.pantallaActual == 5) {
      gameScreen.nickname[0] = 'A';
      gameScreen.nickname[1] = 'A';
      gameScreen.nickname[2] = 'A';
      gameScreen.cursorPos = 0;
      gameScreen.indiceAbecedario = 0;
    }
  }

  if (btnA.estado()) {
    switch (gameScreen.pantallaActual) {
      case 1:
        musica.pausa();
        limpiar(3);
        break;
      
      case 3:
        musica.pausa();
        limpiar(1);
        break;

      case 5:
        highscores[gameScreen.posNuevoHighscore].username = gameScreen.nickname;
        highscores[gameScreen.posNuevoHighscore].score = gameScreen.score;
        MSD.SaveJSONFile(highscores);
        limpiar(2);
        break;
    }
  }
}

#endif