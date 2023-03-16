#ifndef __GRAFO__CXX__
#define __GRAFO__CXX__

#include "Grafo.h"

Imagenes_2D TransformToImagen(vector<vector<int>> matrix)
{
    Imagenes_2D newImg;

    for (int i = 0; i < matrix.size(); i++)
    {
        vector<Pixel> aux;

        for (int j = 0; j < matrix[i].size(); j++)
        {
            Pixel pix;
            pix.setGris(matrix[i][j]);
            aux.push_back(pix);
        }

        newImg.miVect.push_back(aux);
    }

    return newImg;
}

std::pair<Grafo, vector<vector<int>>> GrafoImagen(Imagenes_2D img)
{
    Grafo grafo;
    vector <vector<int>> etiquetas;

    for (int i = 0; i < img.miVect.size(); i++)
    {
        vector<int> columna;

        for (int j = 0; j < img.miVect[i].size(); j++)
        {
            grafo.insertarVertice(img.miVect[i][j]);
            columna.push_back(-1);
        }

        etiquetas.push_back(columna);
    }

    for (int i = 0; i < img.miVect.size(); i++)
    {
        for (int j = 0; j < img.miVect[i].size(); j++)
        {
            Pixel aRev = img.miVect[i][j];

            if (i > 0)
            {
                Pixel izquierdo = img.miVect[i - 1][j];
                int costo = abs(izquierdo.getGris() - aRev.getGris());

                grafo.insertarArista(aRev, izquierdo, costo);
            }
            if (i < img.miVect.size() - 1)
            {
                Pixel derecho = img.miVect[i + 1][j];
                int costo = abs(derecho.getGris() - aRev.getGris());

                grafo.insertarArista(aRev, derecho, costo);
            }
            if (j > 0)
            {
                Pixel arriba = img.miVect[i][j - 1];
                int costo = abs(arriba.getGris() - aRev.getGris());

                grafo.insertarArista(aRev, arriba, costo);
            }
            if (j < img.miVect[i].size() - 1)
            {
                Pixel abajo = img.miVect[i][j + 1];
                int costo = abs(abajo.getGris() - aRev.getGris());

                grafo.insertarArista(aRev, abajo, costo);
            }
        }
    }

    std::pair<Grafo, vector<vector<int>>> aux;
    aux.first = grafo;
    aux.second = etiquetas;

    return aux;
}

void segmentar(std::vector<std::string> args, Imagenes_2D img)
{
    std::string nombreArchivo = args[1];

    if ((args.size() - 2) % 3 != 0)
    {
        std::cout << "La imagen en memoria no pudo ser segmentada" << std::endl;
        return;
    }

    vector<Semilla> semillas;

    for (int i = 2; i < args.size(); i += 3)
    {
        Semilla semilla;

        semilla.x = atoi(args[i].c_str());
        semilla.y = atoi(args[i + 1].c_str());
        semilla.l = atoi(args[i + 2].c_str());
        semilla.myPix = img.miVect[semilla.x][semilla.y];
        semillas.push_back(semilla);
    }

    std::pair<Grafo, vector<vector<int>>> aux = GrafoImagen(img);

    Grafo grafo = aux.first;
    vector<vector<int>> etiquetas = aux.second;

    for (int i = 0; i < img.miVect.size(); i++)
    {
        for (int j = 0; j < img.miVect[i].size(); j++)
        {
            Pixel aRev = img.miVect[i][j];

            int minCost = INT_MAX;
            Semilla minSemilla;

            for (int k = 0; k < semillas.size(); k++)
            {
                int costo = grafo.Dijkstra(semillas[k].myPix, aRev);
                if (costo < minCost)
                {
                    minCost = costo;
                    minSemilla = semillas[k];
                }
            }

            etiquetas[i][j] = minSemilla.l;
        }
    }

    Imagenes_2D result = TransformToImagen(etiquetas);

    std::string filename(nombreArchivo);
    std::fstream file_out;

    file_out.open(filename, std::ios_base::out);

    if (!file_out.is_open())
    {
        std::cout << "La imagen en memoria no pudo ser segmentada" << std::endl;
    }
    else
    {
        std::cout << "La imagen en memoria fue segmentada correctamente y guardada en " << nombreArchivo << std::endl;

        file_out << "P2" << std::endl;
        file_out << result.getLargo() << " " << result.getAncho() << std::endl;
        file_out << 255 << std::endl;

        for (int i = 0; i < result.getAncho(); i++)
        {
            for (int j = 0; j < result.getLargo(); j++)
            {
                file_out << result.miVect[i][j].getGris() << " ";
            }

            file_out << std::endl;
        }
    }
}

int Grafo::verticesCount()
{
    return vertices.size();
};

int Grafo::aristasCount()
{
    int sum = 0;
    for (int i = 0; i < verticesCount(); i++)
    {
        sum += aristas[i].size();
    }
    return sum;
}

int Grafo::buscarVertice(Pixel vert)
{
    int ind = -1;
    for (int i = 0; i < verticesCount(); i++)
    {
        if (this->vertices[i].getGris() == vert.getGris())
        {
            ind = i;
        }
    }
    return ind;
}

bool Grafo::insertarVertice(Pixel vert)
{
    bool res = false;

    if (buscarVertice(vert) == -1)
    {
        this->vertices.push_back(vert);
        std::vector<std::pair<int, int>>* nvector = new std::vector < std::pair < int, int >>;

        this->aristas.push_back(*nvector);
        res = true;
    }
    return res;
}

Pixel Grafo::GetVertice(int ind)
{
    return this->vertices[ind];
}

bool Grafo::insertarArista(Pixel origen, Pixel destino, int costo)
{
    bool res = false;
    int indiceOrigen = buscarVertice(origen);
    int indiceDestino = buscarVertice(destino);

    if (indiceOrigen != -1 && indiceDestino != -1)
    {
        bool esta = false;
        typename std::vector<std::pair<int, int> >::iterator it;
        for (it = aristas[indiceOrigen].begin(); it != aristas[indiceOrigen].end(); it++)
        {
            if (it->first == indiceDestino)
                esta = true;

        }
        if (!esta)
        {
            std::pair<int, int>* n_par = new std::pair<int, int>;
            n_par->first = indiceDestino;
            n_par->second = costo;
            aristas[indiceOrigen].push_back(*n_par);

            res = true;
        }
    }
    return res;
}

int Grafo::buscarArista(Pixel origen, Pixel destino)
{
    int res = -1;

    int indiceOrigen = buscarVertice(origen);
    int indiceDestino = buscarVertice(destino);

    if (indiceOrigen != -1 && indiceDestino != -1)
    {
        typename std::vector<std::pair<int, int> >::iterator it = aristas[indiceOrigen].begin();
        for (; it != aristas[indiceOrigen].end(); it++)
        {
            if (it->first == indiceDestino)
            {
                res = it->second;
            }
        }
    }

    return res;
}

bool Grafo::eliminarVertice(Pixel vert)
{
    bool res = false;
    int indiceVertice = buscarVertice(vert);
    if (indiceVertice != -1)
    {
        typename std::vector< std::vector<std::pair<int, int> > >::iterator itA;
        typename std::vector< std::vector<std::pair<int, int> > >::iterator itPos;

        int ind = 0;

        for (itA = aristas.begin(); itA != aristas.end(); itA++)
        {
            ind += 1;
            if (ind == indiceVertice)
            {
                itPos = itA;
            }
            else if (!itA->empty())
            { 
                typename std::vector<std::pair<int, int>>::iterator it;
                typename std::vector<std::pair<int, int>>::iterator itPos;
                if (itA->size() > 1)
                {
                    for (it = itA->begin(); it != itA->end(); it++)
                    {
                        if (it->first == indiceVertice)
                        {
                            itPos = it;
                        }
                    }

                    itA->erase(itPos);
                }
            }
        }

        vertices.erase(vertices.begin() + indiceVertice);
    }
    return res;
}

bool Grafo::eliminarArista(Pixel origen, Pixel destino)
{
    bool res = false;

    int indiceOrigen = buscarVertice(origen);
    int indiceDestino = buscarVertice(destino);

    if (indiceOrigen != -1 && indiceDestino != -1)
    {
        typename std::vector<std::pair<int, int>>::iterator it;
        typename std::vector<std::pair<int, int>>::iterator itPos;

        for (it = aristas[indiceOrigen].begin(); it != aristas[indiceOrigen].end(); it++)
        {
            if (it->first == indiceDestino)
                itPos = it;
        }

        aristas[indiceOrigen].erase(itPos);
    }

    return res;
}

int Grafo::Dijkstra(Pixel VerticeO, Pixel verticeD)
{
    int indiceOrigen = buscarVertice(VerticeO); 
    int indiceDestino = buscarVertice(verticeD); 

    if (indiceOrigen != -1 && indiceDestino != -1)
    {
        std::vector<int> distancias;  
        std::vector<int> visitados;   
        std::vector<int> padres;     

        for (int i = 0; i < verticesCount(); i++)
        {
            distancias.push_back(INT_MAX);
            visitados.push_back(0);  
            padres.push_back(-1);
        }

        distancias[indiceOrigen] = 0;
        visitados[indiceOrigen] = 1; 

        for (int i = 0; i < verticesCount(); i++)
        {
            int min = INT_MAX;
            int ind = -1;  
            for (int j = 0; j < verticesCount(); j++)
            {
                if (visitados[j] == 0 && distancias[j] < min)
                {
                    min = distancias[j];
                    ind = j;
                }
            }

            visitados[ind] = 1;

            for (int j = 0; j < verticesCount(); j++)
            {
                if (visitados[j] == 0 && distancias[j] > distancias[ind] + buscarArista
                (VerticeO, vertices[j]))
                {
                    distancias[j] = distancias[ind] + buscarArista(VerticeO, vertices[j]);
                    padres[j] = ind;
                }
            }
        }

        return distancias[indiceDestino];
    }

    return -1;
}


#endif



