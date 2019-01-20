#include "camara.h"

Camara::Camara(Tupla3f pos, int tipo)
{
    this->eye = pos;
    this->punto = {0.0, 0.0, 0.0};
    this->orientacion = {0.0, 1.0, 0.0};
    this->angulo_observador_x = 0.0;
    this->angulo_observador_y = 0.0;
    this->xmouse = 0.0;
    this->ymouse = 0.0;
    this->zoom_mouse = 0.0;
    this->tipo_camara = tipo;

    if(tipo_camara == 0)
    {
        this->izda = -0.084;
        this->dcha = 0.084;
        this->bottom = -0.084;
        this->tope = 0.084;
        this->cerca = 0.1;
        this->lejos = 2000.0;

        zoom_ortho = std::abs(eye(2));
    }
    else
    {
        this->izda = -pos(2);
        this->dcha = pos(2);
        this->bottom = -pos(2);
        this->tope = pos(2);
        this->cerca = -20.0;
        this->lejos = 20.0;

        zoom_ortho = std::abs(eye(2));
    }
}

void Camara::configProyeccion(const float radio_xy)
{
    if(tipo_camara == 0)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, radio_xy, cerca, lejos);
        glMatrixMode(GL_MODELVIEW);
        
    }
    else
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(izda, dcha, bottom, tope, cerca, lejos);
        glMatrixMode(GL_MODELVIEW);
    }
    
}

void Camara::configObservador()
{
    std::cout << "Angulo X: " << angulo_observador_x << std::endl;
    std::cout << "Angulo Y: " << angulo_observador_y << std::endl;

    gluLookAt(eye(0), eye(1), eye(2), punto(0), punto(1), punto(2), orientacion(0), orientacion(1), orientacion(2));
    
    if(tipo_camara == 0)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-zoom_ortho/100, +zoom_ortho/100, -zoom_ortho/100, +zoom_ortho/100, cerca, lejos);
        glMatrixMode(GL_MODELVIEW); 
    }
    else
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-zoom_ortho, +zoom_ortho, -zoom_ortho, +zoom_ortho, cerca, lejos);
        glMatrixMode(GL_MODELVIEW);    
    }
    this->rotarX(angulo_observador_x);
    this->rotarY(angulo_observador_y);
}

void Camara::rotarX(float angulo)
{
    glRotatef(angulo, 0.0, 1.0, 0.0);
}

void Camara::rotarY(float angulo)
{
    glRotatef(angulo, 1.0, 0.0, 0.0);
}

void Camara::zoom(float fact)
{
    float aumento = 0.1;

    if(tipo_camara == 0)
    {
        if(zoom_mouse > fact)
            zoom_ortho -= aumento;
        else
            zoom_ortho += aumento;
    }
    else
    {
        if(zoom_mouse > fact)
            zoom_ortho += aumento;
        else
            zoom_ortho -= aumento;
    }

    zoom_mouse = fact;
}

void Camara::mover(float x, float y)
{
    float dy = std::abs(ymouse - y);
    float dx = std::abs(xmouse - x);

    if(xmouse > x)
        angulo_observador_x = angulo_observador_x + dx;
    else
        angulo_observador_x = angulo_observador_x - dx;
    
    if(ymouse > y)
        angulo_observador_y = angulo_observador_y + dy;
    else
        angulo_observador_y = angulo_observador_y - dy;

    xmouse = x;
    ymouse = y;
}
