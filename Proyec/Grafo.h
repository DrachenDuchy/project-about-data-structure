#ifndef __GRAFO__H__
#define __GRAFO__H__

#include <vector>
#include <climits>
#include <queue>
#include <string>
#include <cmath>
#include "Imagenes_3D.cxx"

class Grafo
{
	public:
		bool insertarVertice(Pixel vertice);
		bool insertarArista(Pixel, Pixel, int);
		int verticesCount();
		int aristasCount();
		int buscarVertice(Pixel vertice);
		int buscarArista(Pixel, Pixel);
		bool eliminarVertice(Pixel Vertice);
		bool eliminarArista(Pixel, Pixel);
		int Dijkstra(Pixel, Pixel);
		Pixel GetVertice(int ind); //devuelve el vertice en la posicion ind

	private:
		std::vector<Pixel> vertices;
		std::vector <std::vector<std::pair<int, int>>> aristas;

};
struct Semilla
{
	int x, y, l;
	Pixel myPix;
};
#include "Grafo.cxx"

#endif 