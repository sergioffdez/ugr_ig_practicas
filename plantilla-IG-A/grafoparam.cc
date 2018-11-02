// #############################################################################
//
// Informática Gráfica (Grado Informática)
//
// Archivo: GrafoParam.cc
// -- implementación del objeto jerárquico de la práctica 3
//
// #############################################################################

#include "grafoparam.h"

// -----------------------------------------------------------------------------
// constructor: crea mallas indexadas en los nodos terminales del grafo

GrafoParam::GrafoParam()
{
    cilindro = new Cilindro("plys/cilindro.ply");
    cubo = new Cubo();
}
// -----------------------------------------------------------------------------
// actualizar valor efectivo de un parámetro (a partir de su valor no acotado)

void GrafoParam::actualizarValorEfe(const unsigned iparam, const float valor_na)
{
    assert(iparam < num_parametros);

    using namespace std;
    //cout << "GrafoParam::actualizarValorEfe( " << iparam << ", " << valor_na << " )" << endl ;

    constexpr float vp = 2.5;

    switch (iparam)
    {
    case 0:
        // altura 1: oscila entre 0.7 y 1.3, a 0.5 oscilaciones por segundo
        // inicialmente es 1.0 (media de los valores extremos)
        altura_1 = 1.0 + 0.3 * sin(0.5 * (2.0 * M_PI * valor_na));
        break;
    case 1:
        // altura 2: oscila a 0.3 oscilaciones por segundo
        altura_2 = 0.7 + 0.7 * sin(0.3 * (2.0 * M_PI * valor_na));
        break;
    case 2:
        // angulo en grados de rotacion 1 (cubo rojoaa)
        // crece linealmente a 150 grados por segundo, inicialmente es 20 grados
        ag_rotacion_1 = 20.0 + 100.0 * valor_na;
        break;
    case 3:
        // ángulo en grados de rotacion 2 (cubo azul)
        // oscila entre -45 y 30 grados, a 1.5 oscilaciones por segundo
        // (inicialmente es -7.5 grados)
        ag_rotacion_2 = -7.5 + 37.5 * sin(0.5 * (2.0 * M_PI * valor_na));
        break;

    case 4:
        desplazamiento = -0.3 + 0.27 * sin(0.3 * (2.0 * M_PI * valor_na));
        break;

    case 5:
        ag_rotacion_3 = 35.0 + 20 * sin(0.3 * (2.0 * M_PI * valor_na));
        break;
    }
}

// -----------------------------------------------------------------------------
// visualización del objeto Jerárquico con OpenGL,
// mediante llamadas a los métodos 'draw' de los sub-objetos

void GrafoParam::draw(const int p_modo_vis, const bool p_usar_diferido)
{
    // asegurarnos que está inicializado ok

    assert(cubo != nullptr);
    assert(cilindro != nullptr);

    // guardar parametros de visualización para esta llamada a 'draw'
    // (modo de visualización, modo de envío)

    modo_vis = p_modo_vis;
    usar_diferido = p_usar_diferido;

    // dibujar objetos

    constexpr float
        sep = 1.5,                    // separación entre centros de las columnas
        radio_cil = 0.5,              // radio del cilindro más fino
        radio_cil2 = radio_cil + 0.1; // radio del cilindro más grueso

    grua();
}

void GrafoParam::rueda()
{
    glPushMatrix();
        glTranslatef(0.0, -0.00, -0.00);
        glRotatef(90, 1.0, 0.0, 0.0);
        glScalef(0.2, 0.1, 0.2);
        cilindro->draw(modo_vis, usar_diferido);
    glPopMatrix();
}

void GrafoParam::base()
{

    glPushMatrix();
        glScalef(2.0, 0.2, 1.0);
        cubo->draw(modo_vis, usar_diferido);
    glPopMatrix();
}

void GrafoParam::base_ruedas()
{

    //rueda 1
    glPushMatrix();
        glTranslatef(0.9, 0.0, 0.5);
        glRotatef(ag_rotacion_1,0.0,0.0,1.0);
        rueda();
    glPopMatrix();

    //rueda 2
    glPushMatrix();
        glTranslatef(0.9, 0.0, -0.6);
        glRotatef(ag_rotacion_1,0.0,0.0,1.0);
        rueda();
    glPopMatrix();

    //rueda 3
    glPushMatrix();
        glTranslatef(-0.9, 0.0, 0.5);
        glRotatef(ag_rotacion_1,0.0,0.0,1.0);
        rueda();
    glPopMatrix();

    //rueda 4
    glPushMatrix();
        glTranslatef(-0.9, 0.0, -0.6);
        glRotatef(ag_rotacion_1,0.0,0.0,1.0);
        rueda();
    glPopMatrix();

    glPushMatrix();
        base();
    glPopMatrix();
}

void GrafoParam::pie(){

	glPushMatrix();
	
		glTranslatef(0.0,0.1,0.0);
		glPushMatrix();
			base_ruedas();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(0.0,0.1,0.0);
			glScalef(0.05,1.5,0.05);
			cilindro->draw(modo_vis,usar_diferido);
		glPopMatrix();
	glPopMatrix();
}

void GrafoParam::base_giratoria()
{
    glPushMatrix();
        glTranslatef(0.0, 0.2, 0.0);
        glScalef(0.5, 0.15, 0.5);
        cilindro->draw(modo_vis, usar_diferido);
    glPopMatrix();
}

void GrafoParam::cabina()
{
    glPushMatrix();
        base_giratoria();
    glPopMatrix();

     glPushMatrix();
        glTranslatef(0.27, 0.55, 0.0);
        glScalef(0.4, 0.4, 1.2);
        cubo->draw(modo_vis, usar_diferido);
    glPopMatrix();
}

void GrafoParam::cuba()
{
    glPushMatrix();
        glTranslatef(-0.21, 0.0, 0.0);
        glRotatef(45, 0.0, 0.0, 1.0);
        glScalef(0.3, 0.3, 0.4);
        cubo->draw(modo_vis, usar_diferido);
    glPopMatrix();
}

void GrafoParam::ext1()
{
    glPushMatrix();
        glRotatef(ag_rotacion_2, 0.0, 0.0, 1.0);
        cuba();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.4, 0.0, 0.0);
        glScalef(0.8, 0.1, 0.1);
        cubo->draw(modo_vis, usar_diferido);
    glPopMatrix();
}

void GrafoParam::ext2()
{
    glPushMatrix();
        glRotatef(ag_rotacion_3, 0.0, 0.0, 1.0);
        glTranslatef(-0.8, 0.00, 0.0);
        ext1();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.29, 0.0, 0.0);
        glScalef(0.6, 0.1, 0.1);
        cubo->draw(modo_vis, usar_diferido);
    glPopMatrix();
}

void GrafoParam::ext3()
{
    glPushMatrix();
    glTranslatef(desplazamiento, 0.0, 0.0);
        ext2();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.3, 0.0, 0.0);
        glScalef(0.6, 0.15, 0.15);
        cubo->draw(modo_vis, usar_diferido);
    glPopMatrix();
}

void GrafoParam::ext4()
{
    glPushMatrix();
    glRotatef(ag_rotacion_3, 0.0, 0.0, 1.0);
    glTranslatef(-0.6, 0.0, 0.0);
        ext3();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(0.39, 0.0, 0.0);
        glScalef(0.8, 0.15, 0.15);
        cubo->draw(modo_vis, usar_diferido);
    glPopMatrix();
}

void GrafoParam::cabina_brazo()
{
    glPushMatrix();
        glRotatef(ag_rotacion_1,0.0,1.0,0.0);
        glTranslatef(0.0,altura_2,0.0);
        glPushMatrix();
            glTranslatef(-0.8, 0.94, 0.0);
            glRotatef(-45, 0.0, 0.0, 1.0);
            ext4();
        glPopMatrix();

        glPushMatrix();
            cabina();
        glPopMatrix();
    glPopMatrix();
}

void GrafoParam::grua()
{
    glPushMatrix();
        cabina_brazo();
    glPopMatrix();

    glPushMatrix();
        pie();
    glPopMatrix();
}




