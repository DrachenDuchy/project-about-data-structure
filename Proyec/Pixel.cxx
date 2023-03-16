#include "Pixel.h"

Pixel::Pixel() //color pixeles
{
	
}
void Pixel::setR(int num)
{
	this->r = num;
}
void Pixel::setG(int num)
{
	this->g = num;
}
void Pixel::setB(int num)
{
	this->b = num;
}
void Pixel::setGris(int gris)
{
	this->gris = gris;
}

int Pixel::getR()
{
	return this->r;
}
int Pixel::getG()
{
	return this->g;
}
int Pixel::getB()
{
	return this->b;
}
int Pixel::getGris()
{
	return this->gris;
}
