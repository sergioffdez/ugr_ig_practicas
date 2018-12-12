//Sergio Fernández Fernández
//DNI: 09083723-B

#ifndef _ESCENA_H
#define _ESCENA_H

#include "ejes.h"
#include "malla.h"
#include "jerarquico.h"

class Escena
{

  private:
    Ejes ejes;
    bool animacion = false;
    bool luz = false;
    bool textura_activada = false;

    // variables que definen la posicion de la camara en coordenadas polares
    GLfloat Observer_distance;
    GLfloat Observer_angle_x;
    GLfloat Observer_angle_y;

    // variables que controlan la ventana y la transformacion de perspectiva
    GLfloat Width, Height, Front_plane, Back_plane;

    void clear_window();
    void dibujar_objeto_actual();

    // Transformación de cámara
    void change_projection(const float ratio_xy);
    void change_observer();

    void conmutarAnimaciones();
    void conmutarAnimacionesLuz();

    int objeto_actual = 0, // objeto actual (el que se visualiza)
        num_objetos = 0,   // número de objetos (actualizado al crear los objetos en el constructor)
        modo_dibujo = 0,
        num_material = 0,
        nMateriales = 0;

    ObjMallaIndexada malla;

    bool modo_draw = false;
    bool modo_shade = false;
    bool activar_luz = false;
    bool activar_textura = false;

    // Objetos de la escena
    Cubo *cubo = nullptr; // es importante inicializarlo a 'nullptr'
    Tetraedro *tetraedro = nullptr;
    ObjPLY *ply = nullptr;
    Cilindro *cilindro = nullptr;
    Cono *cono = nullptr;
    Esfera *esfera = nullptr;
    ObjRevolucion *peon = nullptr;
    ObjJerarquico *obj = nullptr;
    Luz *luces = nullptr;
    Cuadro *cuadro = nullptr;

  public:
    Escena();
    void inicializar(int UI_window_width, int UI_window_height);
    void redimensionar(int newWidth, int newHeight);

    // Dibujar
    void dibujar();

    // Interacción con la escena
    bool teclaPulsada(unsigned char Tecla1, int x, int y);
    void teclaEspecial(int Tecla1, int x, int y);

    void mgeDesocupado();
};
#endif
