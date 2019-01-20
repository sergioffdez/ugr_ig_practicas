//Sergio Fernández Fernández
//DNI: 09083723-B

// #############################################################################
//
// Informática Gráfica (Grado Informática)
//
// Archivo: ObjMallaIndexada.h
// -- declaraciones de clase ObjMallaIndexada (mallas indexadas) y derivados
//
// #############################################################################

#ifndef OBJETOS_H_INCLUDED
#define OBJETOS_H_INCLUDED

#include "aux.h"
#include <cmath>
#define cimg_display 0
#include "CImg.h"

using namespace cimg_library;

// *****************************************************************************
//
// clase para objetos 3D (mallas indexadas)
//
// *****************************************************************************

struct Material
{
  Tupla4f difuso;	
  Tupla4f especular;
  Tupla4f ambiente;
  float brillo;
};

struct ParametrosLuz
{
  Tupla4f luz_punto; //posicion luz, según sea el ultimo componente 1 o 0
  Tupla4f luz_ambiente; //componente ambiental 
  Tupla4f luz_difusa; //componente difuso
  Tupla4f luz_especular; //componente especular
};

class ObjMallaIndexada
{
public:
  
  // dibuja el objeto en modo inmediato
  void draw_ModoInmediato(int modo_vis);

  // dibuja el objeto en modo diferido (usando VBOs)
  void draw_ModoDiferido();

  GLuint CrearVBO(GLuint tipo_vbo, GLuint tamanio_bytes, GLvoid *puntero_ram);

  // función que redibuja el objeto
  // está función llama a 'draw_MI' (modo inmediato)
  // o bien a 'draw_MD' (modo diferido, VBOs)
  void draw(int modo_vis, bool selecciona);

  void activarMaterial(int indice);
  void addMaterial(Tupla4f mat_difuso, Tupla4f mat_especular, Tupla4f mat_ambiente, float mat_brillo);
  //void cargarImagen();
  //void textura();

  //Dibuja el modo ajedrez
  void ModoAjedrez();

  //Funcion crear malla
  void CrearMalla(const std::vector<Tupla3f> perfil, const int rotaciones, const std::vector<Tupla3f> &vertices,
                  const std::vector<Tupla3i> &triangulos);

  //Función que permite rotar los vertices
  inline Tupla3f RotacionEjeY(Tupla3f vertice, int i, int angulo);

  //Funcion que colorea los objetos;
  void ColorearObjeto();

  //Get colores
  std::vector<Tupla3f> getColores();

protected:
  GLuint id_vbo_ver = 0;
  GLuint id_vbo_tri = 0;

  void TablaVertices(const std::vector<Tupla3f> perfil, int rotaciones,
                         const std::vector<Tupla3f> &vertices); 
  void TablaTriangulos(const std::vector<Tupla3f> perfil, int rotaciones,
                       const std::vector<Tupla3i> &triangulos);

  //void calcular_normales();

  std::vector<Tupla3f> vertices;   // tabla de coordenadas de vértices (una tupla por vértice, con tres floats)
  std::vector<Tupla3i> triangulos; // una terna de 3 enteros por cada cara o triángulo
  std::vector<Tupla3f> colores;
  std::vector<Tupla3f> normalesVertices;
  std::vector<Tupla2f> texturas;
  std::vector<Material> material;

  GLuint textura_id;
  int alto;
  int ancho;
  std::vector<unsigned char> pixel;
  

  // completar: tabla de colores, tabla de normales de vértices
};

// *****************************************************************************
//
// clases derivadas de ObjMallaIndexada (definen constructores específicos)
//
// *****************************************************************************

// *****************************************************************************
// Cubo con centro en el origen y lado unidad
// (tiene 9 vertices y 6 caras)

class Cuadro : public ObjMallaIndexada
{
  public: 
    Cuadro();
};

class Cubo : public ObjMallaIndexada
{
public:
  Cubo();
};

class Tetraedro : public ObjMallaIndexada
{
public:
  Tetraedro();
};

// *****************************************************************************
// objeto leído de un archivo PLY

class ObjPLY : public ObjMallaIndexada
{
public:
  ObjPLY(const std::string &nombre_archivo);
  std::vector<Tupla3f> getVertices();
};

// *****************************************************************************
// objeto de revolución obtenido a partir de un perfil (en un PLY)

class ObjRevolucion : public ObjMallaIndexada
{
public:
  ObjRevolucion(const std::string &nombre_ply_perfil);
};

class Cilindro : public ObjRevolucion
{
public:
  Cilindro(const std::string &nombre_ply_perfil);
};

class Cono : public ObjRevolucion
{
public:
  Cono(const std::string &nombre_ply_perfil);
};

class Esfera : public ObjRevolucion
{
public:
  Esfera(const std::string &nombre_ply_perfil);
};

/*class Luz {
  public:
      Luz(Tupla4f luzPunto, Tupla4f luzAmbiente, Tupla4f luzDifusa, Tupla4f luzEspecular);
      void activar();
      void desactivar();
      void incrementarAngulo();
      void addLuz(Tupla4f luzPunto, Tupla4f luzAmbiente, Tupla4f luzDifusa, Tupla4f luzEspecular);

   protected:
      std::vector<ParametrosLuz> param_luz;
      std::vector<GLenum> indice_luz = {GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7}; //indice de la luz, LIGHT0-LIGHT7
      GLdouble matriz_identidad[16] = {1,0,0,0,0,1,0,0,0,0,1,0,1,0,0,1};
      float angulo;

};*/


#endif
