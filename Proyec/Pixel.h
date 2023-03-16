#ifndef PIXEL_H
#define PIXEL_H

#include <cstdint>
#include <vector>

class Pixel// colores de los pixeles 
{
    public:

        Pixel();
        void setR(int r);
        void setG(int g);
        void setB(int b);
        void setGris(int gris);
        int getR();
        int getG();
        int getB();
        int getGris();

    protected:
        int r;
        int g;
        int b;
        int gris;
};



#endif