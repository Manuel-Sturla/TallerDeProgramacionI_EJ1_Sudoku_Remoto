# EJ1 - Sudoku Remoto - Taller de Programación I
Este trabajo muestra un Sudoku que corre a través de internet con un cliente conectándose a un servidor.  
El enunciado correspondiente a este ejercicio puedo encontrarse en `2019.02.ejercicio.1.sudoku-remoto.pdf`.  

En este trabajo el foco se encuentra en implementar la arquitectura cliente servidor para ver un ejemplo más completo de uso ver [MicroMachines](https://github.com/hugomlb/MicroMachines)

## Ejecución
### Servidor
Para correr al servidor se debe ejecutar:  
```bash 
$ ./tp server 7777
```
Este utiliza un archivo `board.txt` que debe estar en el mismo directorio que donde se corre el programa. Este tiene el tablero iniciar del sudoku de la forma:
```
0 1 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0
0 0 0 0 0 8 0 0 0
0 0 0 0 0 0 0 0 0
0 0 0 0 0 9 0 0 0
0 4 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0
0 7 0 0 0 0 0 9 0
0 0 0 0 0 0 0 0 0
```

### Cliente
Para ejecutarlo utilice:
```bash
$ ./tp client 127.0.0.1 7777
```
Luego para jugar tendrá los siguientes comandos:  
* `get`: muestra el estado del tablero.  
* `put <n> in <f>,<c>`: pondrá el número `n` en la celda correspondiente a la fila `f` y la columna `c`.  
* `verify`: verifica que los números ingresados cumplan las reglas.  
* `reset`: vuelve a comenzar el tablero en su estado inicial.  
* `exit`: finaliza el juego.   

## Realizado por
 * Manuel Sturla