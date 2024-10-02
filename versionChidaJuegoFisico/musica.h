#ifndef MUSICA_H
#define MUSICA_H

#include "Arduino.h"
#include "Audio.h"
#include "SD.h"
#include "FS.h"

#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26

Audio audio;

class Musica {
  public:
    void inicializar();
    void seleccionarCancion(String cancion);
    void reproducir();
    void detener();
    void pausa();
};

void Musica::inicializar() {
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(20);
}

void Musica::seleccionarCancion(String cancion) {
  String ruta = "/" + cancion + ".mp3";

   if (!SD.exists(ruta.c_str())) {
    Serial.print("El archivo ");
    Serial.print(ruta);
    Serial.println(" no existe.");
    return;
  }
    
  Serial.print("Reproduciendo: ");
  Serial.println(ruta);

  audio.connecttoFS(SD, ruta.c_str());
}

void Musica::reproducir() {
  audio.loop();
}

void Musica::detener() {
  audio.stopSong();  
}

void Musica::pausa() {
  audio.pauseResume();
}

#endif