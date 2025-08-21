#!/bin/bash

# Nombre del proyecto
BASE="$HOME/workspace"

# Crear carpeta base si no existe
mkdir -p "$BASE"

# Buscar ultimo folder de practica
maxid=0
for d in "$BASE"/Practica*; do
	if [ -d "$d" ]; then
		folder="${d##*/}"
		num=${folder##Practica}
		if [ "$num" -gt "$maxid" ]; then
			maxid=$num
		fi
	fi
done
echo $maxid
#Numero de practica
mpid=$((maxid+1))
NEW_DIR="$BASE/Practica$mpid"

# Crear carpeta c
mkdir -p "$NEW_DIR"
cd "$NEW_DIR" || exit

# Clonar repositorio

# mover los archivos main.c y Makefile a esta ultima carpeta y borrar la carpeta del repositorio
