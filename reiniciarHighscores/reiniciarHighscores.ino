#include "MicroSD.h"

MicroSD MSD;
Highscore* highscores;

void setup() {
  Serial.begin(115200);
  MSD.MicroSD_init();

  highscores = MSD.ReadJSONFile();

  Serial.println("HIGHSCORES ANTIGUOS");
  for (int i = 0; i < MAX_HIGHSCORES; i++) {
    Serial.print("Usuario: ");
    Serial.print(highscores[i].username);
    Serial.print(" - Puntaje: ");
    Serial.println(highscores[i].score);
  }

  highscores[0] = {"VAS", 100};
  highscores[1] = {"FAK", 20};
  highscores[2] = {"ACO", 15};
  highscores[3] = {"VRG", 10};

  MSD.SaveJSONFile(highscores);

  Serial.println("HIGHSCORES GUARDADOS");
  for (int i = 0; i < MAX_HIGHSCORES; i++) {
    Serial.print("Usuario: ");
    Serial.print(highscores[i].username);
    Serial.print(" - Puntaje: ");
    Serial.println(highscores[i].score);
  }
}

void loop() {
  
}
