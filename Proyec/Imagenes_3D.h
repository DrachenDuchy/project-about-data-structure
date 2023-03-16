#ifndef IMAGENES_3D_H
#define IMAGENES_3D_H

#include <math.h> 
#include <vector>
#include "Imagenes_2D.cxx"

class Imagenes_3D
{
    public:
        Imagenes_3D();
        int getLargo();
        void setLargo(int largo);
        void setCantidadImagenes(int cantidadImagenes);
        void setNombre(std::string nom);
        int getCantidadImagenes();
        std::vector<Imagenes_2D> vecImag;  
        void infoImagen_3d();
        Imagenes_2D proyeccion2D(char eje, std::string criterio, std::string resultante);
        void leerArchivo3D(std::string nombre, int nImg);
        std::string getNombre();
    protected:
        int ancho;
        int largo;
        int cantidadImagenes;
        std::string nombreVol;
        void ComparadorX(char condicion, int ancho, int largo, std::vector<std::vector<Pixel>>& auxresultado);
        void ComparadorY(char condicion, int ancho, int largo, std::vector<std::vector<Pixel>>& auxresultado);
        void ComparadorZ(char condicion, int ancho, int largo, int cantidad, std::vector<std::vector<Pixel>>& auxresultado);
};
#endif