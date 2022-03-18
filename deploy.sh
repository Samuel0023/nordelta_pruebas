#!/bin/bash
COMPILADOR="testmake.sh"

#crear directorios necesarios
mkdir home
cd home
mkdir utnso
cd utnso
mkdir dumps
cd dumps
mkdir tlb

#permiso de ejecucion
cd
cd tp-2021-2c-Nordelta
chmod 777 ${COMPILADOR}
#clonar commons
# https://github.com/sisoputnfrba/so-commons-library.git

#compilar y instalar commons
cd 
cd so-commons-library
make
make install

#compilar Nordelta
cd 
cd tp-2021-2c-Nordelta
./${COMPILADOR}

# importar la matelib
sudo cp -u /home/utnso/tp-2021-2c-Nordelta/matelib/lib_implementation/libmatelib.so /usr/lib

#clonar PruebasCarpincho
#git clone https://github.com/sisoputnfrba/carpinchos-pruebas.git

#compilar pruebas
cd
cd carpinchos-pruebas
make
