
# 2do Trabajo práctico - Sistemas Operativos - 2022 2Q - ITBA

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
    

