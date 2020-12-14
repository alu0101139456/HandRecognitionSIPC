## Práctica de interacción por gestos
---
### Autores:
- Iris Estefanía  Pereira Domínguez
- Ángel Julián Bolaño Campos

---
### Dependencias
Se debe tener instalado OpenVC 
~~~
~ $ sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev

~ $ sudo apt update && sudo apt install -y cmake g++ wget unzip

~ $ wget -O opencv.zip https://github.com/opencv/opencv/archive/master.zip

~ $ unzip opencv.zip

~ $ mkdir -p build && cd build

~ $ cmake ../opencv-master

~ $ cmake --build .

~ $ sudo make install

~~~
### Precaución respecto a OpenCV:
El proceso de instalación solo copia archivos en ubicaciones predefinidas. La biblioteca instalada con este método no está integrada en el registro de paquetes del sistema y no se puede desinstalar automáticamente. Por defecto, OpenCV se instalará en /usr/local , todos los archivos se copiarán en las siguientes localizaciones:

- /usr/local/bin - executable files
- /usr/local/lib - libraries (.so)
- /usr/local/cmake/opencv4 - cmake package
- /usr/local/include/opencv4 - headers
- /usr/local/share/opencv4 - other files (e.g. trained cascades in XML format)

---


ddddddd