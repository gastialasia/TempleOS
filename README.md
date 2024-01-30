
# TempleOS

Sistema operativo cli, single thread, con sopoprte para multiprocesos y division entre kernel y userland. Cuenta con  memory manager, scheduler, procesos, IPC, y manejo de syscalls. Para más información, leer el archivo 'Informe TP2 - Grupo 2.pdf'

## Características principales

* Doble Memory Manager, a elección del usuario al momento de compilación
* Scheduler Round Robin basado en prioridades, con sistema de envejecimiento para evitar inanición
* Ofrece funcionalidad para crear procesos, finalizarlos y cambiarles la prioridad
* Implementación de semáforos basados en POSIX con syscalls para abrirlos y cerrarlos
* Pipes basados en POSIX con sus correspondientes syscalls
* Lectura y escritura desde file descriptors en background o foreground
* Terminal interactiva con comandos para testear las características
  
## Tecnologías utilizadas

Desarrollado mayoritariamente en C y Assembler

## Intrucciones de instalación y ejecución

 Luego de descargar el repositorio, navegar al mismo. 

 ```
  cd TP2-SO
 ```

 En la misma ruta, abrir el contenedor Docker con la imágen provista por la cátedra:

 ```
  docker run -v "${PWD}:/root" --privileged -ti agodio/itba-so:1.0
 ```
    
 Luego, ejecutar un make all en la carpeta Toolchain:

```
  cd Toolchain; make all
 ```

 Para compilar el proyecto con el Memory Manager estándar, ejecutar un make all en la carpeta raíz del mismo:

```
  make all
```

 En cambio, para compilar con el Buddy System:

```
  make MM=BUDDY
```

 Para ejecutar el sistema, ejecutar el script de bash run.sh ubicado en la carpeta raíz:

```
  ./run.sh
```
## Terminación de un programa

Para cerrar forzadamente un proceso presionar la tecla F2. Para más detalles y lista de comandos dispobles consultar el informe del repositorio.
    

