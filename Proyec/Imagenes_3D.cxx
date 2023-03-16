#include <string>
#include <math.h>  
#include "Imagenes_3D.h"

Imagenes_3D::Imagenes_3D()
{
    
}
void Imagenes_3D::setCantidadImagenes(int cantidadImagenes)
{
    this->cantidadImagenes = cantidadImagenes;
};
int Imagenes_3D::getLargo()
{
    return this->largo;
}
int Imagenes_3D::getCantidadImagenes()
{
    return this->cantidadImagenes;
}
std::string Imagenes_3D::getNombre()
{
    return this->nombreVol;
}
void Imagenes_3D::setNombre(std::string nom)
{
    this->nombreVol = nom;
}
void Imagenes_3D:: setLargo(int largo)
{
    this->largo=largo;
}

void Imagenes_3D::infoImagen_3d()
{
    std::cout << std::endl << "Volumen cargado en memoria: " << this->nombreVol << std::endl;
    std::cout << "Tamanio: " << this->cantidadImagenes << std::endl;
    std::cout << "Ancho: " << this->ancho << std::endl;
    std::cout << "Alto: " << this->largo << std::endl;
}

void Imagenes_3D::leerArchivo3D(std::string nombre, int nImg)
{
    int auxAncho,auxLargo;
    int maxPixel;  
    int cantidadPixel;
    std::stringstream aux;
    Pixel auxPixel;

    std::string auxFolderName = nombre;
    
    if (nombre.find("-ppm") != std::string::npos)
    {
        nombre.erase(nombre.length() - 4);
    }
    else if (nombre.find("ppm") != std::string::npos)
    {
        nombre.erase(nombre.length() - 3);
    }

    this->nombreVol = nombre;
    this->cantidadImagenes = nImg;

    for (int i = 1; i <= nImg; i++)
    {
        Imagenes_2D imgAux2D;

        std::string numerito = (i < 10) ? "0" + std::to_string(i) : std::to_string(i);

        std::ifstream archivo("Fuentes/Imagenes 3D/" + auxFolderName + "/" + nombre + numerito + ".ppm");
        std::string inputLine = " ";
        getline(archivo, inputLine);

        if (inputLine.compare("P3") != 0)
        {
            std::cout << "El volumen " << nombre << numerito << ".ppm" << " no ha podido ser cargado" << std::endl;
            archivo.close();
            return;
        }
        else
        {
            aux << archivo.rdbuf();
            aux >> ancho >> largo;

            auxAncho = ancho;
            auxLargo = largo;

            this->ancho = auxAncho;
            this->largo = auxLargo;

            imgAux2D.setLargo(auxAncho);
            imgAux2D.setAncho(auxLargo);

            std::vector<Pixel> miVectAux(auxLargo);

            aux >> maxPixel;

            for (int i = 0; i < auxLargo; i++)
            {
                miVectAux.clear();
                for (int j = 0; j < auxAncho; j++)
                {
                    aux >> cantidadPixel;
                    auxPixel.setR(cantidadPixel);

                    aux >> cantidadPixel;
                    auxPixel.setG(cantidadPixel);

                    aux >> cantidadPixel;
                    auxPixel.setB(cantidadPixel);
                    miVectAux.push_back(auxPixel);
                }
                imgAux2D.miVect.push_back(miVectAux);
            }

            this->vecImag.push_back(imgAux2D);
        }
        archivo.close();
    }

    std::cout << "El volumen " << nombre << " ha sido cargado" << std::endl; 
    return;
}

void Imagenes_3D::ComparadorX(char condicion, int ancho, int largo, std::vector<std::vector<Pixel>>& auxresultado)
{
    Pixel resultPixel;

    for (int i = 0; i < this->vecImag.size(); i++)
    {
        for (int j = 0; j < ancho; j++)
        {
            if (condicion == '-') //criterio mediana
            {
                auxresultado[j][i].setR(this->vecImag[i].miVect[j][(largo / 2)].getR());
                auxresultado[j][i].setG(this->vecImag[i].miVect[j][(largo / 2)].getG());
                auxresultado[j][i].setB(this->vecImag[i].miVect[j][(largo / 2)].getB());
                continue;
            }

            resultPixel.setR(0);
            resultPixel.setG(0);
            resultPixel.setB(0);

            for (int k = 0; k < largo; k++)
            {
                if (condicion == '~') //criterio promedio
                {
                    resultPixel.setR(resultPixel.getR() + this->vecImag[i].miVect[j][k].getR());
                    resultPixel.setG(resultPixel.getG() + this->vecImag[i].miVect[j][k].getG());
                    resultPixel.setB(resultPixel.getB() + this->vecImag[i].miVect[j][k].getB());
                    continue;
                }

                bool Rcorrect, Gcorrect, Bcorrect;

                switch (condicion)
                {
                    case '<': //criterio minimo
                        Rcorrect = (this->vecImag[i].miVect[j][k].getR() < resultPixel.getR()) || (resultPixel.getR() == 0);
                        Gcorrect = (this->vecImag[i].miVect[j][k].getG() < resultPixel.getG()) || (resultPixel.getG() == 0);
                        Bcorrect = (this->vecImag[i].miVect[j][k].getB() < resultPixel.getB()) || (resultPixel.getB() == 0);
                        break;
                    case '>': //criterio maximo
                        Rcorrect = (this->vecImag[i].miVect[j][k].getR() > resultPixel.getR());
                        Gcorrect = (this->vecImag[i].miVect[j][k].getG() > resultPixel.getG());
                        Bcorrect = (this->vecImag[i].miVect[j][k].getB() > resultPixel.getB());
                        break;
                }
                 
                if (Rcorrect && Gcorrect && Bcorrect)
                {
                    resultPixel.setR(this->vecImag[i].miVect[j][k].getR());
                    resultPixel.setG(this->vecImag[i].miVect[j][k].getG());
                    resultPixel.setB(this->vecImag[i].miVect[j][k].getB());
                }
            }

            if (condicion == '~') //criterio promedio
            {
                auxresultado[j][i].setR((resultPixel.getR() / largo));
                auxresultado[j][i].setG((resultPixel.getG() / largo));
                auxresultado[j][i].setB((resultPixel.getB() / largo));
                continue;
            }
            auxresultado[j][i].setR(resultPixel.getR());
            auxresultado[j][i].setG(resultPixel.getG());
            auxresultado[j][i].setB(resultPixel.getB());
        }
    }
}
void Imagenes_3D::ComparadorY(char condicion, int ancho, int largo, std::vector<std::vector<Pixel>>& auxresultado)
{
    Pixel resultPixel;

    for (int i = 0; i < this->vecImag.size(); i++)
    {
        for (int j = 0; j < largo; j++)
        {
            if (condicion == '-') //criterio mediana
            {
                auxresultado[j][i].setR(this->vecImag[i].miVect[j][(ancho / 2)].getR());
                auxresultado[j][i].setG(this->vecImag[i].miVect[j][(ancho / 2)].getG());
                auxresultado[j][i].setB(this->vecImag[i].miVect[j][(ancho / 2)].getB());
                continue;
            }

            resultPixel.setR(0);
            resultPixel.setG(0);
            resultPixel.setB(0);

            for (int k = 0; k < ancho; k++)
            {
                if (condicion == '~') //criterio promedio
                {
                    resultPixel.setR(resultPixel.getR() + this->vecImag[i].miVect[k][j].getR());
                    resultPixel.setG(resultPixel.getG() + this->vecImag[i].miVect[k][j].getG());
                    resultPixel.setB(resultPixel.getB() + this->vecImag[i].miVect[k][j].getB());
                    continue;
                }

                bool Rcorrect, Gcorrect, Bcorrect;

                switch (condicion)
                {
                case '<': //criterio minimo
                    Rcorrect = (this->vecImag[i].miVect[k][j].getR() < resultPixel.getR()) || (resultPixel.getR() == 0);
                    Gcorrect = (this->vecImag[i].miVect[k][j].getG() < resultPixel.getG()) || (resultPixel.getG() == 0);
                    Bcorrect = (this->vecImag[i].miVect[k][j].getB() < resultPixel.getB()) || (resultPixel.getB() == 0);
                    break;
                case '>': //criterio maximo
                    Rcorrect = (this->vecImag[i].miVect[k][j].getR() > resultPixel.getR());
                    Gcorrect = (this->vecImag[i].miVect[k][j].getG() > resultPixel.getG());
                    Bcorrect = (this->vecImag[i].miVect[k][j].getB() > resultPixel.getB());
                    break;
                }

                if (Rcorrect && Gcorrect && Bcorrect)
                {
                    resultPixel.setR(this->vecImag[i].miVect[j][k].getR());
                    resultPixel.setG(this->vecImag[i].miVect[j][k].getG());
                    resultPixel.setB(this->vecImag[i].miVect[j][k].getB());
                }
            }

            if (condicion == '~') //criterio promedio
            {
                auxresultado[i][j].setR((resultPixel.getR() / largo));
                auxresultado[i][j].setG((resultPixel.getG() / largo));
                auxresultado[i][j].setB((resultPixel.getB() / largo));
                continue;
            }
            auxresultado[i][j].setR(resultPixel.getR());
            auxresultado[i][j].setG(resultPixel.getG());
            auxresultado[i][j].setB(resultPixel.getB());
        }
    }
}
void Imagenes_3D::ComparadorZ(char condicion, int ancho, int largo, int cantidad, std::vector<std::vector<Pixel>>& auxresultado)
{
    Pixel resultPixel;
    resultPixel.setR(0);
    resultPixel.setG(0);
    resultPixel.setB(0);

    if (condicion == '-') //criterio mediana
    {
        for (int i = 0; i < ancho; i++)
        {
            for (int j = 0; j < largo; j++)
            {
                auxresultado[i][j].setR(this->vecImag[(cantidad / 2)].miVect[i][j].getR());
                auxresultado[i][j].setG(this->vecImag[(cantidad / 2)].miVect[i][j].getG());
                auxresultado[i][j].setB(this->vecImag[(cantidad / 2)].miVect[i][j].getB());
            }
        }

        return;
    }
    else if (condicion == '~') //criterio promedio
    {
        for (int i = 0; i < ancho; i++)
        {
            for (int j = 0; j < largo; j++)
            {
                for (int k = 0; k < this->vecImag.size(); k++) 
                {
                    resultPixel.setR(resultPixel.getR() + this->vecImag[k].miVect[i][j].getR());
                    resultPixel.setG(resultPixel.getG() + this->vecImag[k].miVect[i][j].getG());
                    resultPixel.setB(resultPixel.getB() + this->vecImag[k].miVect[i][j].getB());                
                }

                auxresultado[i][j].setR(resultPixel.getR() / cantidad);
                auxresultado[i][j].setG(resultPixel.getG() / cantidad);
                auxresultado[i][j].setB(resultPixel.getB() / cantidad);
            }
        }

        return;
    }

    for (int i = 0; i < this->vecImag.size(); i++)
    {
        for (int j = 0; j < ancho; j++)
        {
            resultPixel.setR(0);
            resultPixel.setG(0);
            resultPixel.setB(0);

            for (int k = 0; k < largo; k++)
            {
                bool Rcorrect, Gcorrect, Bcorrect;

                switch (condicion)
                {
                case '<': //criterio minimo
                    Rcorrect = (this->vecImag[i].miVect[j][k].getR() < auxresultado[j][k].getR());
                    Gcorrect = (this->vecImag[i].miVect[j][k].getG() < auxresultado[j][k].getG());
                    Bcorrect = (this->vecImag[i].miVect[j][k].getB() < auxresultado[j][k].getB());
                    break;
                case '>': //criterio maximo
                    Rcorrect = (this->vecImag[i].miVect[j][k].getR() > auxresultado[j][k].getR());
                    Gcorrect = (this->vecImag[i].miVect[j][k].getG() > auxresultado[j][k].getG());
                    Bcorrect = (this->vecImag[i].miVect[j][k].getB() > auxresultado[j][k].getB());
                    break;
                }

                if (Rcorrect && Gcorrect && Bcorrect)
                {
                    resultPixel.setR(this->vecImag[i].miVect[j][k].getR());
                    resultPixel.setG(this->vecImag[i].miVect[j][k].getG());
                    resultPixel.setB(this->vecImag[i].miVect[j][k].getB());

                    auxresultado[j][k].setR(this->vecImag[i].miVect[j][k].getR());
                    auxresultado[j][k].setG(this->vecImag[i].miVect[j][k].getG());
                    auxresultado[j][k].setB(this->vecImag[i].miVect[j][k].getB());
                   
                }
            }
        }
    }
}

Imagenes_2D Imagenes_3D::proyeccion2D(char eje, std::string criterio, std::string resultante)
{
    Imagenes_2D resultado;
        
    std::vector<Pixel> vectorPixeles; // Guarda un pixel en cada posicion del vector.
    int ancho = this->vecImag[0].miVect.size();
    int largo = this->vecImag[0].miVect[0].size();

    int resultR = 0, resultG = 0, resultB = 0;

    int cantidad = this->getCantidadImagenes();
    
    std::vector<std::vector<Pixel>> auxresultado; // Matriz de pixeles

    if(eje == 'x')
    {
        auxresultado.resize(ancho);        

        for(int i = 0; i < ancho; i++)
        {
            auxresultado[i].resize(cantidad);
            for(int j = 0; j < cantidad; j++)
            {   
                auxresultado[i][j].setR(0);
                auxresultado[i][j].setG(0);
                auxresultado[i][j].setB(0);
            }
        }

        if(criterio == "minimo") this->ComparadorX('<', ancho, largo, auxresultado);
        else if(criterio == "maximo") this->ComparadorX('>', ancho, largo, auxresultado);
        else if(criterio == "promedio") this->ComparadorX('~', ancho, largo, auxresultado);
        else if(criterio == "mediana") this->ComparadorX('-', ancho, largo, auxresultado);
        else
        {
            std::cout << "La proyecci�n 2D del volumen en memoria no ha podido ser generada" << std::endl;
            return resultado;
        }

        for(int i = 0; i < ancho; i++)
        {
            vectorPixeles.clear();
            
            for(int j=0; j<cantidad;j++) vectorPixeles.push_back(auxresultado[i][j]);

            resultado.miVect.push_back(vectorPixeles);
        }
        resultado.setLargo(cantidad);
        resultado.setAncho(ancho);
    }
    else if(eje == 'y')
    {
        auxresultado.resize(largo);

        for (int i = 0; i < largo; i++)
        {
            auxresultado[i].resize(cantidad);
            for (int j = 0; j < cantidad; j++)
            {
                auxresultado[i][j].setR(0);
                auxresultado[i][j].setG(0);
                auxresultado[i][j].setB(0);
            }
        }

        int contador=0;

        if (criterio == "minimo") this->ComparadorY('<', ancho, largo, auxresultado);
        else if (criterio == "maximo") this->ComparadorY('>', ancho, largo, auxresultado);
        else if (criterio == "promedio") this->ComparadorY('~', ancho, largo, auxresultado);
        else if (criterio == "mediana") this->ComparadorY('-', ancho, largo, auxresultado);
        else
        {
            std::cout << "La proyeccion 2D del volumen en memoria no ha podido ser generada" << std::endl;
            return resultado;
        }

        for (int i = 0; i < largo; i++)
        {
            vectorPixeles.clear();

            for (int j = 0; j < cantidad; j++) vectorPixeles.push_back(auxresultado[i][j]);

            resultado.miVect.push_back(vectorPixeles);
        }
        resultado.setLargo(cantidad);
        resultado.setAncho(largo);
    }
    else if(eje == 'z')
    {
        auxresultado.resize(ancho);
        for (int i = 0; i < auxresultado.size(); i++) auxresultado[i].resize(largo);

        if (criterio == "minimo") this->ComparadorZ('<', ancho, largo, cantidad, auxresultado);
        else if (criterio == "maximo") this->ComparadorZ('>', ancho, largo, cantidad, auxresultado);
        else if (criterio == "promedio") this->ComparadorZ('~', ancho, largo, cantidad, auxresultado);
        else if (criterio == "mediana") this->ComparadorZ('-', ancho, largo, cantidad, auxresultado);
        else
        {
            std::cout << "La proyeccion 2D del volumen en memoria no ha podido ser generada" << std::endl;
            return resultado;
        }
        
        for (int i = 0; i < ancho; i++)
        {
            vectorPixeles.clear();

            for (int j = 0; j < largo; j++) vectorPixeles.push_back(auxresultado[i][j]);

            resultado.miVect.push_back(vectorPixeles);
        }
        resultado.setLargo(largo);
        resultado.setAncho(ancho);
    }
    else
    {
        std::cout << "La proyeccion 2D del volumen en memoria no ha podido ser generada" << std::endl;
        return resultado;
    }

    std::string filename(resultante);
    std::fstream file_out;
    file_out.open(filename, std::ios_base::out);
    if (!file_out.is_open())
    {
        std::cout << "La proyeccion 2D del volumen en memoria no ha podido ser generada" << std::endl;
    }
    else
    {
        std::cout << "La proyeccion 2D del volumen en memoria ha sido generada" << std::endl;

        file_out << "P2"<<std::endl;
        file_out << resultado.getLargo() << " "<<resultado.getAncho()<<std::endl;
        file_out << 255 << std::endl;

        for(int i=0;i<resultado.getAncho();i++)
        {
            for(int j=0;j<resultado.getLargo();j++)
            {
                int toGris = (resultado.miVect[i][j].getR() + resultado.miVect[i][j].getG() + resultado.miVect[i][j].getB()) / 3;
                file_out << toGris << " ";
            }
            file_out << std::endl;
        }

    }
    return resultado;
};