#include <list>
#include <algorithm>
#include <bitset>
#include <fstream>
#include "Imagenes_2D.h"

std::string toBinary(int num);

Imagenes_2D::Imagenes_2D()
{
    
}

Imagenes_2D::Imagenes_2D(int largo,int ancho)
{
    this->largo = largo;
    this->ancho = ancho;
}
int Imagenes_2D::getLargo()
{
    return this->largo;
}
int Imagenes_2D::getAncho()
{
    return this->ancho;
}
std::string Imagenes_2D::getNombre()
{
    return this->nombreImg;
}
void Imagenes_2D::setLargo(int largo)
{
    this->largo = largo;
}
void Imagenes_2D::setAncho(int ancho)
{
    this->ancho = ancho;
}
void Imagenes_2D::setNombre(std::string nom)
{
    this->nombreImg = nom;
}

void Imagenes_2D::info_Imagen()
{
    std::cout << std::endl << "Imagen cargada en memoria: " << this->nombreImg << std::endl;
    std::cout << "Ancho: " << this->ancho << std::endl;
    std::cout << "Alto: " << this->largo << std::endl;
};

Imagenes_2D Imagenes_2D::leerArchivo2D(std::string nombre)
{
    int auxAncho,auxLargo;
    int maxPixel;  
    int cantidadPixel;
    
    std::stringstream aux;
    Imagenes_2D imagenAux;
    Pixel auxPixel;
    std::ifstream archivo("Fuentes/Imagenes 2D/"+ nombre);
    std::string inputLine = " ";
    getline(archivo, inputLine);

    if(inputLine.compare("P2")!=0) 
    {
        std::cout << "La imagen " << nombre << " no ha podido ser cargada" << std::endl;
    }
    else
    { 
        std::cout << "La imagen " << nombre << " ha sido cargada" << std::endl;

        imagenAux.setNombre(nombre);

        aux<<archivo.rdbuf();  

        aux>>ancho>>largo;
        auxAncho=ancho;
        auxLargo=largo;
        std::vector<Pixel> miVectAux (auxAncho);
        aux>>maxPixel;
            for (int i=0;i<auxLargo;i++)
            {
                 for (int j=0;j<auxAncho;j++)
                 {
                    aux>>cantidadPixel;
                    auxPixel.setGris(cantidadPixel);
                    miVectAux.push_back(auxPixel);
                 }
                    imagenAux.miVect.push_back(miVectAux);
            }
    }
    archivo.close();
    return imagenAux;
}

std::vector<std::pair<int,int>> Imagenes_2D::frecuencias()
{
    std::vector<std::pair<int,int>> frecuenciaVector;
    frecuenciaVector.resize(255);

    for (int i = 0; i < 255; i++)
    {
        frecuenciaVector.at(i).first = i;
    }

    for (int i = 0; i < this->miVect.size(); i++)
    {
        for (int j = 0; j < this->miVect[i].size(); j++)
        {
            Pixel vectorxd = this->miVect[i][j];

            int gris = vectorxd.getGris();

            std::pair<int, int> pair = frecuenciaVector[gris];

            pair.second++;
        }
    }

    std::sort(frecuenciaVector.begin(), frecuenciaVector.end(),[] (const pair <int,int> &x, const pair <int,int> &y) { return x.second < y.second; });
        
    return frecuenciaVector;
}

struct Datos
{
    int ancho;
    int largo;
    int tamanio;
    vector<int> frecuencias;
    char* bits;
};

void Imagenes_2D::codificar (std::string nombreArchivo)
{
    std::vector<std::pair<int, int>> frecuenciasResult = this->frecuencias();
    char* aux = HuffmanTree(frecuenciasResult,this->miVect);

    Datos data;
    data.ancho = this->ancho;
    data.largo = this->largo;
    data.tamanio = sizeof(aux);

    for (int i = 0; i < frecuenciasResult.size(); i++)
    {
        data.frecuencias.push_back(frecuenciasResult[i].second);
    }

    data.bits = aux;

    std::ofstream archivoBinario(nombreArchivo, ofstream::binary);
    archivoBinario.write((char*)& data, sizeof(Datos));
    archivoBinario.close();
}

void Imagenes_2D::decodificar(std::string nombreHuffman, std::string nombreOutput)
{
    Datos readedData;

    cout << endl << "ola antes de crear archivo";

    std::fstream archivoBinario(nombreHuffman, ios::in | ios::binary);
    archivoBinario.read((char*)& readedData, sizeof(Datos));
    archivoBinario.close();

    std::vector<std::pair<int,int>> frecuencias2;
    
    for (int i = 0; i <256; i++)
    {
        frecuencias2.push_back(std::pair<int,int>(i, readedData.frecuencias[i]));
    }

    this->ancho = readedData.ancho;
    this->largo = readedData.largo;
    
    cout << endl << "ola antes de huffman arbol";
    std::vector<std::vector<Pixel>> h= HuffmanArbol(frecuencias2, readedData.bits, this->ancho, this->largo);
    
    this->miVect = h;
    this->nombreImg = nombreOutput;
    
    cout << endl << "ola antes de crear output";
    std::fstream file_out;
    file_out.open(nombreOutput, std::ios_base::out);
    
    if (!file_out.is_open())
    {
        std::cout << "Se ha generado un error en la creacion del archivo" << std::endl;
    }
    else
    {
        std::cout << "Se ha generado el archivo correctamente" << std::endl;

        file_out << "P2"<<std::endl;
        file_out << this->getLargo() << " "<<this->getAncho()<<std::endl;
        file_out << 255 << std::endl;

        for(int i=0;i<this->getAncho();i++)
        {
            for(int j=0;j<this->getLargo();j++)
            {
                file_out << this->miVect[i][j].getGris() << " ";
            }
            file_out << std::endl;
        }

    }
}