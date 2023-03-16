#include <iostream>
#include <stdio.h>
//#include <conio.h>
#include <string.h>
#include "Pixel.cxx"

const int maxsize = 100;
const int null = 0;

using namespace std;
struct huff_node
{
  int symbol;
  int freq;
  huff_node *parent;
  char childtype;
  string codigo;
};

typedef huff_node *ptr;
ptr node[maxsize];

void create(int k,int f); 
void twosmall(ptr &p, ptr &q, int numnodes);   
                                              
char* encriptar(std::vector<std::vector<Pixel>>misDatos); 
char* letra(int k);

int decodificar(string codigo,ptr node[])
{
  for (int i = 0; i < maxsize; i++)
  {
    if (codigo==node[i]->codigo)
    {
      return node[i]->symbol;
    }
  }

  return -999999;
}
std::vector<std::vector<Pixel>> HuffmanArbol(std::vector<std::pair<int,int>>miSimbol,string bits,int ancho, int alto)
{
    int numsymbols = miSimbol.size();

    std::vector<std::vector<Pixel>> resultado;
    ptr p, q, r;
    
    cout << endl << "ola antes del for create";
    for (int i = 0; i < numsymbols; i++)
        create(miSimbol.at(i).first, miSimbol.at(i).second);

    cout << endl << "ola antes del otro for que no se xd";
    for (int j = numsymbols; j < 2 * numsymbols - 1; j++)
    {
        r = new huff_node;
        node[j] = r;
        r->parent = NULL;
        twosmall(p, q, j);
        p->parent = r;
        q->parent = r;
        p->childtype = 'L';
        q->childtype = 'R';
        r->symbol = ' ';
        r->freq = p->freq + q->freq;
    }

    std::vector<Pixel> fila;
    Pixel aux;
    string simbolaux;

    cout << endl << "ola antes de for pushback fila";
    for (int i = 0; i < ancho; i++)
    {
        fila.push_back(aux);
    }

    cout << endl << "ola antes de el ultimo for";
    int tamanio = int(bits.length() / 255);
    for (int k = 0; k < alto; k++) {
        for (int i = 0; i < ancho; i++)
        {

            for (int j = 0; j < tamanio; j++) {
                simbolaux += bits[j + i * tamanio + k * tamanio];
            }
            aux.setGris(decodificar(simbolaux, node));
            fila[i] = aux;
        }
        resultado.push_back(fila);
    }

    return resultado;
}


char *HuffmanTree(std::vector<std::pair<int,int>>miSimbol,std::vector<std::vector<Pixel>>misDatos)
{
  int numsymbols = miSimbol.size();
  ptr p, q, r;

  for (int i = 0; i < numsymbols; i++)
    create(miSimbol.at(i).first,miSimbol.at(i).second);


  for (int j = numsymbols; j < 2*numsymbols - 1; j++)
    {
      r = new huff_node;
      node[j] = r;
      r->parent = NULL;
      twosmall(p, q, j);  
      p->parent = r;
      q->parent = r;
      p->childtype = 'L';
      q->childtype = 'R';
      r->symbol = ' ';
      r->freq = p->freq + q->freq;
    }
  return encriptar(misDatos);
}

char *encriptar(std::vector<std::vector<Pixel>>misDatos)
{
    string resultado = "";
    for(std::vector<Pixel> i:misDatos)
    {
        for(Pixel j:i)
        {
            string temp = letra(j.getGris());
            resultado += temp;
        }
    }

    char* dest = new char[resultado.length() + 1];
    std::copy(resultado.begin(), resultado.end(), dest);

    return dest;
}
void create(int k,int f)
{
  ptr t = new huff_node;
  t->symbol=k;
  t->freq=f;
  t->parent = NULL;
  node[k] = t;
}
char* letra(int k)
{
    ptr t = node[k];
    string code = "";
    int size = 0;

    while (t->parent != NULL)
    {
        if (t->childtype == 'L'){
            code += '0';
            node[k]->codigo+='0';
        }
        else{
            code += '1';
            node[k]->codigo+="1";
        }
        t = t->parent;
        size++;
    }

    char* dest = new char[code.length() + 1];
    std::copy(code.begin(), code.end(), dest);

    return dest;
}

void twosmall(ptr &p, ptr &q, int numnodes)
{
  int min1 = 9999;
  int min2 = 9999;
  p = NULL;
  q = NULL;

  for (int i = 0; i < numnodes; i++)
    {
      if (node[i]->parent == NULL)
            {
              if (node[i]->freq < min1)
                {
                  min2 = min1;
                  min1 = node[i]->freq;
                  q = p;
                  p = node[i];
                }
              else
                if (node[i]->freq < min2)
                  {
                        min2 = node[i]->freq;
                        q = node[i];
                  }
            }
    }

}