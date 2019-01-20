//Sergio Fernández Fernández
//DNI: 09083723-B

#include "aux.h"
#include "ply_reader.h"
#include "malla.h"
#define cimg_display 0
#include "CImg.h"

using namespace cimg_library;

// *****************************************************************************
//
// Clase ObjMallaIndexada
//
// *****************************************************************************

// Visualización en modo inmediato con 'glDrawElements'

void ObjMallaIndexada::draw_ModoInmediato(int modo_vis)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices.data());

	if (glIsEnabled(GL_LIGHTING) == GL_TRUE){
		glEnableClientState(GL_NORMAL_ARRAY);
    	glNormalPointer(GL_FLOAT, 0, normalesVertices.data());
	}

	/*if (glIsEnabled(GL_TEXTURE_2D) == GL_TRUE){
    	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    	glTexCoordPointer( 2, GL_FLOAT, 0, texturas.data());
  	}*/
	// habilitar uso de un array de vértices

	// indicar el formato y la dirección de memoria del array de vértices
	// (son tuplas de 3 valores float, sin espacio entre ellas)

	if (modo_vis == 3)
		ModoAjedrez();
	else
		// visualizar, indicando: tipo de primitiva, número de índices,
		// tipo de los índices, y dirección de la tabla de índices
		glDrawElements(GL_TRIANGLES, triangulos.size() * 3, GL_UNSIGNED_INT, triangulos.data());

	/*if (glIsEnabled(GL_TEXTURE_2D) == GL_TRUE)
  	{
    	glDisable(GL_TEXTURE_2D);
    	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }*/

	// deshabilitar array de vértices
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}
// -----------------------------------------------------------------------------
// Visualización en modo diferido con 'glDrawElements' (usando VBOs)

void ObjMallaIndexada::draw_ModoDiferido()
{

	if (glIsEnabled(GL_LIGHTING) == GL_TRUE){
		glEnableClientState(GL_NORMAL_ARRAY);
    	glNormalPointer(GL_FLOAT, 0, normalesVertices.data());
	}
	/*if (glIsEnabled(GL_TEXTURE_2D) == GL_TRUE){
    	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    	glTexCoordPointer( 2, GL_FLOAT, 0, texturas.data());
	}	*/
	  
	if (id_vbo_ver == 0)
		id_vbo_ver = CrearVBO(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), vertices.data());

	if (id_vbo_tri == 0)
		id_vbo_tri = CrearVBO(GL_ELEMENT_ARRAY_BUFFER, triangulos.size() * 3 * sizeof(float), triangulos.data());

	// especificar localización y formato de la tabla de vértices, habilitar tabla
	glBindBuffer(GL_ARRAY_BUFFER, id_vbo_ver); // activar VBO de vértices
	glVertexPointer(3, GL_FLOAT, 0, 0);		   // especifica formato y offset (=0)
	glBindBuffer(GL_ARRAY_BUFFER, 0);		   // desactivar VBO de vértices.
	glEnableClientState(GL_VERTEX_ARRAY);	  // habilitar tabla de vértices

	// visualizar triángulos con glDrawElements (puntero a tabla == 0)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_vbo_tri); // activar VBO de triángulos
	glDrawElements(GL_TRIANGLES, 3 * triangulos.size(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // desactivar VBO de triángulos

	/*if (glIsEnabled(GL_TEXTURE_2D) == GL_TRUE){
    	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    	glTexCoordPointer( 2, GL_FLOAT, 0, texturas.data());
  	}*/
	// desactivar uso de array de vértices
	glDisableClientState(GL_VERTEX_ARRAY);
}

GLuint ObjMallaIndexada::CrearVBO(GLuint tipo_vbo, GLuint tamanio_bytes, GLvoid *puntero_ram)
{
	GLuint id_vbo; // resultado: identificador de VBO

	glGenBuffers(1, &id_vbo);		// crear nuevo VBO, obtener identificador (nunca 0)
	glBindBuffer(tipo_vbo, id_vbo); // activar el VBO usando su identificador

	// esta instrucción hace la transferencia de datos desde RAM hacia GPU
	glBufferData(tipo_vbo, tamanio_bytes, puntero_ram, GL_STATIC_DRAW);
	glBindBuffer(tipo_vbo, 0); // desactivación del VBO (activar 0)

	return id_vbo; // devolver el identificador resultado
}

// -----------------------------------------------------------------------------
// Función de visualización de la malla,
// puede llamar a  draw_ModoInmediato o bien a draw_ModoDiferido

void ObjMallaIndexada::draw(int modo_vis, bool selecciona)
{
	//glColorPointer(3, GL_FLOAT, 0, colores.data());

	if (!selecciona)
		draw_ModoInmediato(modo_vis);
	else
		draw_ModoDiferido();
}

void ObjMallaIndexada::ModoAjedrez()
{
	std::vector<Tupla3i> tri_1, tri_2;
	std::vector<Tupla3f> color_impar, color_par;

	for (int i = 0; i < triangulos.size(); i++)
	{
		if (i % 2 == 0)
			tri_1.push_back(triangulos[i]);
		else
			tri_2.push_back(triangulos[i]);
	}

	for (int i = 0; i < vertices.size(); i++)
	{
		color_impar.push_back({0.0, 1.0, 0.0});
		color_par.push_back({0.0, 0.0, 1.0});
	}

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices.data());
	glColorPointer(3, GL_FLOAT, 0, color_impar.data());
	glDrawElements(GL_TRIANGLES, tri_1.size() * 3, GL_UNSIGNED_INT, tri_1.data());
	glColorPointer(3, GL_FLOAT, 0, color_par.data());
	glDrawElements(GL_TRIANGLES, tri_2.size() * 3, GL_UNSIGNED_INT, tri_2.data());

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

//Funcion para rotar los vertices
inline Tupla3f ObjMallaIndexada::RotacionEjeY(Tupla3f vertice, int i, int angulo)
{
	const double PI = 3.141592653589;
	Tupla3f vertice_rotado;
	float x, y, z;
	float angulo_rotacion = ((2 * PI * i) / angulo);

	y = vertice[1];
	x = ((cos(angulo_rotacion) * vertice[0]) + (vertice[2] * sin(angulo_rotacion)));
	z = ((-sin(angulo_rotacion) * vertice[0]) + (vertice[2] * cos(angulo_rotacion)));

	vertice_rotado = {x, y, z};

	return vertice_rotado;
}

//Función crear Malla
void ObjMallaIndexada::CrearMalla(const std::vector<Tupla3f> perfil, const int rotaciones, const std::vector<Tupla3f> &vertices,
								  const std::vector<Tupla3i> &triangulos)
{
	//Declaramos el numero de vertices total y el numero de vertices del perfil
	int aux;

	this->vertices.clear();
	this->triangulos.clear();

	//Calculamos la tabla de vertices.
	TablaVertices(perfil, rotaciones, vertices);

	//Calculamos la tabla de triangulos
	TablaTriangulos(perfil, rotaciones, triangulos);

	//Capa inferior
	Tupla3f ver1 = vertices[0];
	if (ver1[0] != 0)
	{
		this->vertices.push_back({0.0, ver1[1], 0.0});

		for (auto i = 0; i + perfil.size() < vertices.size(); i += perfil.size())
			this->triangulos.push_back({i, vertices.size() - 1, (i + perfil.size()) % (vertices.size() - 1)});
	}

	//Tapa superior
	Tupla3f ver2 = vertices[vertices.size() - 2];
	if (ver2[0] != 0)
	{
		this->vertices.push_back({0.0, ver2[1], 0.0});

		for (auto i = perfil.size() - 1; i < vertices.size() - 1; i += perfil.size())
		{
			this->triangulos.push_back({vertices.size() - 1, i, (i + perfil.size()) % (vertices.size() - 2)});
			aux = i;
		}
	}
}

//Funcion para colorear objetos
void ObjMallaIndexada::ColorearObjeto()
{
	for (int i = 0; i < vertices.size(); i++)
	{
		colores.push_back({1.0, 0.0, 1.0});
		//colores.push_back({1.0, 1.0, 0.0});
		//colores.push_back({0.0, 1.0, 1.0});
	}
}

std::vector<Tupla3f> ObjMallaIndexada::getColores()
{
	return colores;
}

// -----------------------------------------------------------------------------
// Recalcula la tabla de normales de vértices (el contenido anterior se pierde)

void ObjMallaIndexada::TablaVertices(const std::vector<Tupla3f> perfil, int rotaciones,
										 const std::vector<Tupla3f> &vertices)
{
	//this->vertices.clear();

	for (int i = 0; i < rotaciones; i++)
	{
		for (int j = 0; j < perfil.size(); j++)
		{
			Tupla3f aux = RotacionEjeY(perfil[j], i, rotaciones);
			this->vertices.push_back(aux);
		}
	}
}

//Calcular tabla de triangulos
void ObjMallaIndexada::TablaTriangulos(const std::vector<Tupla3f> perfil, int rotaciones,
									   const std::vector<Tupla3i> &triangulos)
{
	int v1, v2, v3, v4;

	//this->triangulos.clear();

	for (int i = 0; i < rotaciones; i++)
	{
		for (int j = 0; j < perfil.size() - 1; j++)
		{
			v1 = perfil.size() * i + j;
			v2 = perfil.size() * ((i + 1) % rotaciones) + j;
			v3 = perfil.size() * i + (j + 1);
			v4 = perfil.size() * ((i + 1) % rotaciones) + (j + 1);
			this->triangulos.push_back({v1, v2, v4});
			this->triangulos.push_back({v1, v4, v3});
		}
	}
}

/*void ObjMallaIndexada::calcular_normales()
{
	normalesVertices.clear();
	normalesVertices.resize(triangulos.size());

	Tupla3f vector_1, vector_2, prod_vect;

	//Obtener 2 vectores en la cara y hacer producto vectorial
	for (unsigned int i=0;i<triangulos.size();i++)
  	{
		vector_1 = vertices[triangulos[i][1]] - vertices[triangulos[i][0]];
		vector_2 = vertices[triangulos[i][2]] - vertices[triangulos[i][0]];
		prod_vect = vector_1.cross(vector_2);

		normalesVertices[triangulos[i][0]]= normalesVertices[triangulos[i][0]] + prod_vect;
    	normalesVertices[triangulos[i][1]]= normalesVertices[triangulos[i][1]] + prod_vect;
    	normalesVertices[triangulos[i][2]]= normalesVertices[triangulos[i][2]] + prod_vect;
	}

	for (unsigned int i=0;i<vertices.size(); i++)
    	normalesVertices[i] = normalesVertices[i].normalized();

}*/

// *****************************************************************************
//
// Clase Cubo (práctica 1)
//
// *****************************************************************************

Cubo::Cubo()
{

	// inicializar la tabla de vértices
	vertices = {
		{-0.5, -0.5, -0.5}, // 0
		{-0.5, -0.5, +0.5}, // 1
		{-0.5, +0.5, -0.5}, // 2
		{-0.5, +0.5, +0.5}, // 3
		{+0.5, -0.5, -0.5}, // 4
		{+0.5, -0.5, +0.5}, // 5
		{+0.5, +0.5, -0.5}, // 6
		{+0.5, +0.5, +0.5}  // 7
	};

	// inicializar la tabla de caras o triángulos:
	// (es importante en cada cara ordenar los vértices en sentido contrario
	//  de las agujas del reloj, cuando esa cara se observa desde el exterior del cubo)

	triangulos = {{0, 2, 4}, {4, 2, 6}, {1, 5, 3}, {3, 5, 7}, {1, 3, 0}, {0, 3, 2}, {5, 4, 7}, {7, 4, 6}, {1, 0, 5}, {5, 0, 4}, {3, 7, 2}, {2, 7, 6}};

	//ColorearObjeto();

	//calcular_normales();
}

// *****************************************************************************
//
// Clase Tetraedro (práctica 1)
//
// *****************************************************************************

Tetraedro::Tetraedro()
{
	vertices =
		{
			{0, +1.5, 0},		//0
			{+0.75, 0, 0},		//1
			{0, 0, +0.75},		//2
			{-0.325, 0, -0.325} //3
		};

	triangulos = {{0, 1, 2}, {0, 3, 1}, {0, 2, 3}, {1, 3, 2}};

	//ColorearObjeto();

	//calcular_normales();
}

// *****************************************************************************
//
// Clase ObjPLY (práctica 2)
//
// *****************************************************************************

ObjPLY::ObjPLY(const std::string &nombre_archivo)
{
	// leer la lista de caras y vértices
	ply::read(nombre_archivo, vertices, triangulos);

	//ColorearObjeto();

	//calcular_normales();
}

std::vector<Tupla3f> ObjPLY::getVertices()
{
	return this->vertices;
}

// *****************************************************************************
//
// Clase ObjRevolucion (práctica 2)
//
// *****************************************************************************

// *****************************************************************************
// objeto de revolución obtenido a partir de un perfil (en un PLY)

ObjRevolucion::ObjRevolucion(const std::string &nombre_ply_perfil)
{
	std::vector<Tupla3f> perfil; //Vector que contiene los vertices del perfil

	ply::read_vertices(nombre_ply_perfil, perfil); //Lee los vertices y los almacena en el perfil

	CrearMalla(perfil, 50, vertices, triangulos);

	//ColorearObjeto();

	//calcular_normales();
}

Cilindro::Cilindro(const std::string &nombre_ply_perfil) : ObjRevolucion(nombre_ply_perfil) {}

Cono::Cono(const std::string &nombre_ply_perfil) : ObjRevolucion(nombre_ply_perfil) {}

Esfera::Esfera(const std::string &nombre_ply_perfil) : ObjRevolucion(nombre_ply_perfil) {}

/*Luz::Luz(Tupla4f luzPunto, Tupla4f luzAmbiente, Tupla4f luzDifusa, Tupla4f luzEspecular)
{
	ParametrosLuz pm;
	for(int i=0; i<4; i++)
	{
		pm.luz_punto[i] = luzPunto(i);
 		pm.luz_ambiente[i] = luzAmbiente(i);
  		pm.luz_difusa[i] = luzDifusa(i);
  		pm.luz_especular[i] = luzEspecular(i);
	}

	param_luz.push_back(pm);
}

void Luz::activar()
{
	glEnable(GL_LIGHTING);

	for (int i=0; i<param_luz.size(); i++)
	{
		glEnable(indice_luz[i]);

		glLightfv(indice_luz[i], GL_AMBIENT, (GLfloat*) &param_luz[i].luz_ambiente);
		glLightfv(indice_luz[i], GL_DIFFUSE, (GLfloat*) &param_luz[i].luz_difusa);
		glLightfv(indice_luz[i], GL_SPECULAR, (GLfloat*) &param_luz[i].luz_especular);

	if(i == 1)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();
			glMultMatrixd(matriz_identidad);
			glRotatef(angulo, 0.0, 1.0, 0.0);
			glLightfv(indice_luz[i], GL_POSITION, (GLfloat*) &param_luz[i].luz_punto);
		glPopMatrix();
	}
	else
		glLightfv(indice_luz[i], GL_POSITION, (GLfloat*) &param_luz[i].luz_punto);
	}
}

void Luz::desactivar()
{
	glDisable(GL_LIGHTING);
}

void Luz::incrementarAngulo()
{
	angulo = angulo + 0.25;
}

void Luz::addLuz(Tupla4f luzPunto, Tupla4f luzAmbiente, Tupla4f luzDifusa, Tupla4f luzEspecular)
{
	ParametrosLuz pm;
	for(int i=0; i<4; i++)
	{
		pm.luz_punto[i] = luzPunto(i);
 		pm.luz_ambiente[i] = luzAmbiente(i);
  		pm.luz_difusa[i] = luzDifusa(i);
  		pm.luz_especular[i] = luzEspecular(i);
	}

	param_luz.push_back(pm);
}

void ObjMallaIndexada::activarMaterial(int indice)
{
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, material[indice].ambiente);
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, material[indice].difuso);
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, material[indice].especular);
	glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, material[indice].brillo * 128.0);
}

void ObjMallaIndexada::addMaterial(Tupla4f mat_difuso, Tupla4f mat_especular, Tupla4f mat_ambiente, float mat_brillo)
{
	Material mat;
	for(int i=0; i<4; i++)
	{
		mat.difuso[i] = mat_difuso(i);
 		mat.especular[i] = mat_especular(i);
  		mat.ambiente[i] = mat_ambiente(i);
	}
  		mat.brillo = mat_brillo;

	material.push_back(mat);
}*/

Cuadro::Cuadro()
{
	vertices = {
		{-0.5, -0.5, 0.0},
		{0.5, -0.5, 0.0},
		{0.5, 0.5, 0.0},
		{-0.5, 0.5, 0.0}
	};

	triangulos = {{0, 2, 3}, {0, 1, 2}};

	//ColorearObjeto();
	//calcular_normales();
	//cargarImagen();

	//texturas = {{0.0, 1.0}, {1.0, 1.0}, {1.0, 0.0}, {0.0, 0.0}};
}

/*void ObjMallaIndexada::cargarImagen()
{
	CImg<unsigned char> imagen;
	unsigned char *x, *y, *z;

	imagen.load("imagenes/imagen.jpg");

	for(int i=0; i<imagen.height(); i++)
	{
		for(int j=0; j<imagen.width(); j++)
		{
			unsigned char *x=imagen.data(j, i, 0, 0);
			unsigned char *y=imagen.data(j, i, 0, 1);
			unsigned char *z=imagen.data(j, i, 0, 2);

			pixel.push_back(*x);
			pixel.push_back(*y);
			pixel.push_back(*z);
		}
	}

	this->ancho = imagen.width();
	this->alto = imagen.height();
}

void ObjMallaIndexada::textura()
{
  glEnable(GL_TEXTURE_2D);

  glGenTextures(1, &textura_id);
  glBindTexture(GL_TEXTURE_2D, textura_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);

  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
  // TRASFIERE LOS DATOS A GPU
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel.data());
}*/



