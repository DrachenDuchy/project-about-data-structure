#ifndef IMAGENES_2D_H
#define IMAGENES_2D_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Huffman.cxx"

class Imagenes_2D
{
    public:
        Imagenes_2D();
        Imagenes_2D(int ancho, int largo);
        std::vector<std::vector<Pixel>> miVect;
        void setAncho(int ancho);
        void setLargo(int largo);
        void setNombre(std::string nom);
        int getLargo();
        int getAncho();
        std::string getNombre();
        void info_Imagen();
        Imagenes_2D leerArchivo2D(std::string nombre);
        std::vector<std::pair<int,int>> frecuencias();
        void codificar(std::string nombreArchivo);
        void decodificar(std::string nombreHuffman, std::string nombreOutput);

    protected:
        int ancho;
        int largo;
        std::string nombreImg;
};


#endif
