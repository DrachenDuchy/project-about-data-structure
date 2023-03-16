
// Yeraldi,sebastian;juan soto

#include <iostream>
#include <sstream>
#include "Grafo.cxx"

Imagenes_2D img;
Imagenes_3D img3D;

std::string getUserInput() 
{
    std::string userComando;
    std::getline(std::cin, userComando);

    return userComando;
}

vector<string> getCommandArgs(std::string command, int* argCount)
{
    int count = 1;

    for (int i = 0; i < command.size(); i++)
        if (command[i] == ' ') count++;

    vector<string> args;
    args.resize(count);

    int currentArg = 0;
    std::string argTxt = "";

    for (int i = 0; i < command.size(); i++)
    {
        if (command[i] == ' ')
        {
            args[currentArg] = argTxt;
            argTxt = "";
            currentArg++;
        }
        else
        {
            argTxt += command[i];
            if (i == command.size() - 1)
            {
                args[currentArg] = argTxt;
            }
        }
    }

    *argCount = count;
    return args;
}

void ayuda()
{
    std::cout << "  _______________________________________" << std::endl;
    std::cout << " |                                       |" << std::endl;
    std::cout << " |       BIENVENIDO AL PROGRAMA ^W^      |" << std::endl;
    std::cout << " |_______________________________________|" << std::endl;

    std::cout << "$cargar_imagen" << std::endl;
    std::cout << "$cargar_volumen nombre_base n_im" << std::endl;
    std::cout << "$info_imagen             " << std::endl;
    std::cout << "$info_volumen            " << std::endl;
    std::cout << "$proyeccion2D dirección criterio nombre_archivo.pgm" << std::endl;
    std::cout << "$codificar_imagen nombre_archivo.huffman" << std::endl;
    std::cout << "$decodificar_archivo nombre_archivo.huffman nombre_imagen.pgm" << std::endl;
    std::cout << "$segmentar salida_imagen.pgm sx1 sy1 sl1 sx2 sy2 sl2..." << std::endl;
    std::cout << "$salir                   " << std::endl;
}

void ayuda(std::string nombreComando)
{
    if (nombreComando == "ayuda")
    {
        std::cout << "$ayuda <--- para obtener un listado completo de los comandos" << std::endl;
        std::cout << "$ayuda \"comando\" <--- para obtener detalles de implementacion  de un comando" << std::endl;
    }
    else if (nombreComando == "cargar_imagen")
    {
        std::cout << "$cargar_imagen nombre_imagen.pgm" << std::endl;
    }
    else if (nombreComando == "cargar_volumen")
    {
        std::cout << "$cargar_volumen nombre_base n_im" << std::endl;
    }
    else if (nombreComando == "info_imagen")
    {
        std::cout << "$info_imagen <--- El comando no necesita ningun argumento" << std::endl;
    }
    else if (nombreComando == "info_volumen")
    {
        std::cout << "$info_volumen <--- El comando no necesita ningun argumento" << std::endl;
    }
    else if (nombreComando == "proyeccion2D")
    {
        std::cout << "$proyeccion2D direccion criterio nombre_archivo.pgm" << std::endl;
    }
    else if (nombreComando == "codificar_imagen")
    {
        std::cout << "$codificar_imagen nombre_archivo.bin" << std::endl;
    }
    else if (nombreComando == "decodificar_archivo")
    {
        std::cout << "$decodificar_archivo nombre_archivo.huffman nombre_imagen.pgm" << std::endl;
    }
    else if (nombreComando == "segmentar")
    {
        std::cout << "$segmentar salida_imagen.pgm sx1 sy1 sl1 sx2 sy2 sl2..." << std::endl;
    }
    else if (nombreComando == "salir")
    {
        std::cout << "$salir <--- El comando no necesita ningun argumento" << std::endl;
    }
}

void badArgsMessage(std::string nombreComando)
{
    std::cout << "El comando " + nombreComando + " es erroneo, la forma correcta de llamarlo es: " << std::endl;
    ayuda(nombreComando);
}

int main(int argc, char* argv[])
{
    std::string userComando = "";
    std::cout << '$';

    if (argc < 2)
    {
        userComando = getUserInput();
    }
    else
    {
        userComando = argv[1];
    }

    std::string imagenCargada = "", volumenCargado = "";
    int volNum = 0;

    do
    {
        std::cout << '$';
        if (userComando == "") userComando = getUserInput();

        int argCount = 0;
        vector<string> args = getCommandArgs(userComando, &argCount);

        if (args[0] == "ayuda")
        {
            if (argCount != 2) ayuda();
            else ayuda(args[1]);
        }
        else if (args[0] == "cargar_imagen")
        {
            userComando = "";

            if (argCount != 2) badArgsMessage(args[0]);
            else
            {
                img.leerArchivo2D(args[1]);
            }
        }
        else if (args[0] == "cargar_volumen")
        {
            if (argCount != 3) badArgsMessage(args[0]);
            else
            {
                img3D.leerArchivo3D(args[1], atoi(args[2].c_str()));
            }
        }
        else if (args[0] == "info_imagen")
        {
            if (argCount != 1) badArgsMessage(args[0]);
            else
            {
                if (img.getLargo() == 0) std::cout << std::endl << "No hay una imagen cargada en memoria" << std::endl;
                else img.info_Imagen();
            }
        }
        else if (args[0] == "info_volumen")
        {
            if (argCount != 1) badArgsMessage(args[0]);
            else
            {
                if (img3D.getLargo() == 0) std::cout << std::endl << "No hay un volumen cargado en memoria" << std::endl;
                else img3D.infoImagen_3d();
            }
        }
        else if (args[0] == "proyeccion2D")
        {
            if (argCount != 4) badArgsMessage(args[0]);
            else
            {
                if (img3D.getLargo() == 0) std::cout << std::endl << "No hay un volumen cargado en memoria" << std::endl;
                else img3D.proyeccion2D(args[1][0], args[2], args[3]);
            }
        }
        else if (args[0] == "codificar_imagen")
        {
            if (argCount != 2) badArgsMessage(args[0]);
            else
            {
                if (img.getLargo() != 0)
                {
                    img.codificar(args[1]);
                    std::cout << "La imagen en memoria ha sido codificada exitosamente" << std::endl;
                }
                else std::cout << "No hay una imagen cargada en memoria" << std::endl;
            }
        }
        else if (args[0] == "decodificar_archivo")
        {
            if (argCount != 3) badArgsMessage(args[0]);
            else
            {
                img.decodificar(args[1], args[2]);
            }
        }
        else if (args[0] == "segmentar")
        {
            if (argCount < 3) badArgsMessage(args[0]);
            else
            {
                if (img.getLargo() != 0) segmentar(args, img);
                else std::cout << "No hay una imagen cargada en memoria" << std::endl;
            }
        }
        else if (args[0] == "salir")
        {
            break;
        }
        else
        {
            std::cout << "El comando " << args[0] << " no fue reconocido por el programa" << std::endl;
            std::cout << "si no recuerda los comandos porfavor use el comando |ayuda| para volver a los comando existentes" << std::endl;
        }

        userComando = "";
    } while (userComando != "$salir");

    return 0;
}