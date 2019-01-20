#ifndef _CAMARA_H
#define _CAMARA_H

#include "aux.h"

class Camara
{
    private: 
        Tupla3f eye;
        Tupla3f punto;
        Tupla3f orientacion;

        int tipo_camara;
        float izda, dcha, bottom, tope, cerca, lejos;

        float angulo_observador_x, angulo_observador_y;
        float xmouse, ymouse, zoom_mouse;

        float zoom_ortho;

        void rotarX(float angulo);
        void rotarY(float angulo);

    public:     
        Camara(Tupla3f pos, int tipo);
        void mover(float x, float y);
        void zoom(float fact);
        void configObservador();
        void configProyeccion(const float radio_xy);
};

#endif