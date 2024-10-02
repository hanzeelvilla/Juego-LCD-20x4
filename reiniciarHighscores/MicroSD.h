#ifndef MicroSD_h
#define MicroSD_h
/* Librerías fabricante necesarias para utilizar la MicroSD */
#include <SD.h>
#include <SPI.h>
#include <ArduinoJson.h>

#define MAX_HIGHSCORES 4

struct Highscore {
  String username;
  int score;
};

#define MICROSD_PIN 5    /* Pin de control MicroSD */  
File MicroSD_File;       /* Archivo para escribir  o leer en la MicroSD */
class MicroSD  {
  // VARIABLES
  public:
    uint8_t contador = 0;      /* */
    String filename = "/highscores.json";   /* Nombre de archivo que incluye "/" que indica la ubicación en raíz */
  // MÉTODOS
  public:
   void MicroSD_init ( void );
   void SaveFile ( void );
   String* ReadFile ( void );
   Highscore* ReadJSONFile(void);
   void SaveJSONFile(Highscore* highscores);

};

void MicroSD::MicroSD_init(void) {
    Serial.println(F("Iniciando MicroSD..."));
    if (!SD.begin(MICROSD_PIN)) {
        Serial.println(F("Falló la inicialización del módulo MicroSD"));
        while (1);  // Detén el programa si falla la inicialización
    }
    Serial.println(F("MicroSD inicializada correctamente"));
}

void MicroSD :: SaveFile ( void ) {
    
    // Abrir o crear en caso de que no exista.
    MicroSD_File = SD.open ( filename, FILE_APPEND );
    if ( MicroSD_File ) {

        // Almacenar JSON en el archivo.
        //serializeJson(*doc, JSON);
        MicroSD_File.print ( F ( "Contador en: " ) );
        MicroSD_File.println ( contador );
        contador++;
        MicroSD_File.close ( );
      
    } else {
        // if the file didn't open, print an error:
        Serial.print ( F ( "Error opening " ) );
        Serial.println ( filename );

    }
    Serial.println ( "Verificando si existe el archivo: ");
    if ( SD.exists ( filename ) ) {
    Serial.println ( filename );
    Serial.println( " existe" );
  } else {
        Serial.println ( filename );
    Serial.println( " no existe" );
  }
     // Esperar un segundo entre lecturas
      delay ( 1000 );
}

String* MicroSD :: ReadFile (void) {
  static String highscores[MAX_HIGHSCORES];
  int numLineas = 0;

  MicroSD_File = SD.open(filename);
  if (MicroSD_File) {
    Serial.print("Archivo: ");
    Serial.println(filename);

    // read from the file until there's nothing else in it:
    while (MicroSD_File.available() && numLineas < MAX_HIGHSCORES) {
      highscores[numLineas] = MicroSD_File.readStringUntil('\n');
      numLineas++;
    }
    // close the file:
    MicroSD_File.close();
  } 
  else {
    // if the file didn't open, print an error:
    Serial.println ("Error abriendo el archivo");
  }

  return highscores;
}

void MicroSD::SaveJSONFile(Highscore* highscores) {
  // Crear o abrir el archivo en modo escritura
  MicroSD_File = SD.open(filename, FILE_WRITE);

  if (MicroSD_File) {
    // Crear un documento JSON con capacidad suficiente
    const size_t capacity = JSON_ARRAY_SIZE(MAX_HIGHSCORES) + MAX_HIGHSCORES * JSON_OBJECT_SIZE(2);
    StaticJsonDocument<capacity> doc;

    // Añadir los highscores al JSON
    for (int i = 0; i < MAX_HIGHSCORES; i++) {
      JsonObject obj = doc.createNestedObject();
      obj["username"] = highscores[i].username;
      obj["score"] = highscores[i].score;
    }

    // Serializar el JSON en el archivo
    if (serializeJson(doc, MicroSD_File) == 0) {
      Serial.println(F("Error al escribir en el archivo"));
    } else {
      Serial.println(F("Archivo JSON guardado correctamente"));
    }

    // Cerrar el archivo
    MicroSD_File.close();
  } else {
    // Si el archivo no pudo abrirse, imprimir un error
    Serial.println(F("Error abriendo el archivo para escritura"));
  }
}


Highscore* MicroSD::ReadJSONFile(void) {
  // guardar todo el JSON en un string
  String jsonContent = "";
  
  // Abrir el archivo en modo lectura
  MicroSD_File = SD.open(filename);
  if (MicroSD_File) {
    Serial.print("Archivo: ");
    Serial.println(filename);

    // Leer el contenido del archivo
    while (MicroSD_File.available()) {
      jsonContent += MicroSD_File.readStringUntil('\n');
    }

    // Cerrar el archivo
    MicroSD_File.close();
  } 
  else {
    Serial.println("Error abriendo el archivo JSON");
    return nullptr; // Devuelve un puntero nulo en caso de error
  }
  
  //Serial.println(jsonContent);

  // Establecer un tamaño máximo para el JSON
  const size_t capacity = JSON_ARRAY_SIZE(4) + 4 * JSON_OBJECT_SIZE(2) + 100;
  /*
  JSON_ARRAY_SIZE(4) =  4 elementos en el array
  4 * JSON_OBJECT_SIZE(2) = 4 objetos/json con 2 campos cada json
  100 = margen adicional

  Suma de cuanta memoria reservar en el buffer para guardar
  un array de 4 elementos (4 objetos o json), cada json con 2 campos (username y score)
  */

  StaticJsonDocument<capacity> doc;

  // Deserializar el JSON
  DeserializationError error = deserializeJson(doc, jsonContent);
  // Si hay error
  if (error) {
    Serial.print(F("Error en deserialización: "));
    Serial.println(error.f_str());
    return nullptr;
  }

  // Crear un array dinámico para almacenar los highscores
  static Highscore highscores[MAX_HIGHSCORES];

  // Recorrer el array JSON y extraer los datos
  for (int i = 0; i < MAX_HIGHSCORES; i++) {
    highscores[i].username = doc[i]["username"].as<String>();
    highscores[i].score = doc[i]["score"].as<int>();
  }

  // Devolver el array
  return highscores;
}

#endif