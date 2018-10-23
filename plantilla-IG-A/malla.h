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
#include<cmath>

// *****************************************************************************
//
// clase para objetos 3D (mallas indexadas)
//
// *****************************************************************************

class ObjMallaIndexada
{
public:
  // dibuja el objeto en modo inmediato
  void draw_ModoInmediato();

  // dibuja el objeto en modo diferido (usando VBOs)
  void draw_ModoDiferido();

  GLuint CrearVBO(GLuint tipo_vbo, GLuint tamanio_bytes, GLvoid *puntero_ram);

  // función que redibuja el objeto
  // está función llama a 'draw_MI' (modo inmediato)
  // o bien a 'draw_MD' (modo diferido, VBOs)
  void draw(bool selecciona);

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

  void calcular_normales(const std::vector<Tupla3f> perfil, int rotaciones,
                         const std::vector<Tupla3f> &vertices);               // calcula tabla de normales de vértices (práctica 3)
  void TablaTriangulos(const std::vector<Tupla3f> perfil, int rotaciones,
                       const std::vector<Tupla3i> &triangulos);

  std::vector<Tupla3f> vertices;   // tabla de coordenadas de vértices (una tupla por vértice, con tres floats)
  std::vector<Tupla3i> triangulos; // una terna de 3 enteros por cada cara o triángulo
  std::vector<Tupla3f> colores;    // una terna de 3 floats para cada color RGB

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

class CilindroPrueba : public ObjMallaIndexada
{
  public: 
    CilindroPrueba();
};

#endif
