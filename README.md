# Juego LCD 20x4

Juego creado por Fernando Villaseñor y Hanzeel Villa en la materia IoT. Empezó como una broma 
termino como un proyecto y se ve GOD.

## Componentes necesarios

* ESP32
* Cable USB a micro USB
* Pantalla LCD 20x4
* Módulo Joystick
* 2 botones
* Módulo Micro SD
* Micro SD no mayor a 32 GB
* Amplificador max98357a
* Bocina 8 Ohms 0.5 Watts
* Placa expansora para la esp (opcional)

## Características

1. Intro con una canción de fondo mostrando el nombre del juego y los creadores
2. Menú de selección entre jugar y ver los puntajes más altos
3. Pausa y reinicio de partida
4. Obstaculos en filas aleatorias
5. Almacenamiento en la Micro SD de los puntajes más altos con su respectivo jugador
6. Música de fondo para la intro y el gameplay

## Cómo jugar

1. Selecciona Play dentro del menú haciendo clic al joystick
2. Esquiva los obstaculos moviéndote arriba y abajo
3. Puedes reiniciar/volver al menú con el botón izq o poner pausa con el botón derecho
4. Después de 10 obstáculos esquivados la velocidad aumenta
5. Si chocas con algún obstaculo el juego acaba
6. Si consigues uno de los **mejores 4 marcadores** puedes registrar tu nombre para guardar tu puntuación
7. Tienes un **máximo de 3 carácteres**, presiona el botón del joystick para confirmar tu selección de letra, presiona el botón izq para reiniciar el nombre o el botón der para confirmar tu nombre
8. DIVIERTETE NENA!

## Mejoras

1. Eliminar variables/funciones inecesarias
2. Crear una clase para los obstaculos
3. Usar correctamente variables públicas y privadas
4. Hacer más intuitiva la interacción
5. Agregar comentarios XD
6. Solucionar el problema de la música funcionando en veces si y en veces no (tal vez con dual core)
7. Abierto a feedback