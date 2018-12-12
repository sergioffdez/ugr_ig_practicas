//Sergio Fernández Fernández
//DNI: 09083723-B

#include "aux.h" // includes de OpenGL/glut/glew, windows, y librería std de C++
#include "escena.h"
#include "malla.h" // objetos: Cubo y otros....

//**************************************************************************
// constructor de la escena (no puede usar ordenes de OpenGL)
//**************************************************************************

Escena::Escena()
{
    Front_plane = 0.1;
    Back_plane = 2000.0;
    Observer_distance = 2.0;
    Observer_angle_x = 0.0;
    Observer_angle_y = 0.0;

    ejes.changeAxisSize(5000);

    Tupla4f difusa = {1.0, 1.0, 1.0, 1.0};
    Tupla4f magenta = {1.0, 0.0, 1.0, 1.0};


    // crear los objetos de las prácticas: Mallas o Jerárquicos....

    cubo = new Cubo();

    tetraedro = new Tetraedro();

    ply = new ObjPLY("plys/big_dodge.ply");

    cilindro = new Cilindro("plys/cilindro.ply");

    esfera = new Esfera("plys/esfera.ply");

    cono = new Cono("plys/cono.ply");

    peon = new ObjRevolucion("plys/peon.ply");

    obj = new ObjJerarquico();

    luces = new Luz({30.0, 30.0, 30.0, 0.0}, difusa, difusa, difusa);

    luces->addLuz({30.0, 3.0, 3.0, 1.0}, {0.0, 0.0, 1.0, 1.0}, magenta, magenta);

    cuadro = new Cuadro();

    malla.addMaterial({0.0, 0.50980392, 0.50980392, 1.0}, {0.50196078, 0.50196078, 0.50196078, 1.0}, {0.0, 0.1, 0.06, 1.0}, 0.25);
    malla.addMaterial({0.50754, 0.50754, 0.50754, 1.0}, {0.508273, 0.508273, 0.508273, 1.0}, {0.19225, 0.19225, 0.19225, 1.0}, 0.4);
    malla.addMaterial({0.75164, 0.60648, 0.22648, 1.0}, {0.628281, 0.555802, 0.366065, 1.0}, {0.24725, 0.1995, 0.0745, 1.0}, 0.4);
    nMateriales = 3;
    // .......completar: ...
    // .....

    num_objetos = 9; // se usa al pulsar la tecla 'O' (rotar objeto actual)
}

//**************************************************************************
// inicialización de la escena (se ejecuta cuando ya se ha creado la ventana, por
// tanto sí puede ejecutar ordenes de OpenGL)
// Principalmemnte, inicializa OpenGL y la transf. de vista y proyección
//**************************************************************************

void Escena::inicializar(int UI_window_width, int UI_window_height)
{
    glClearColor(1.0, 1.0, 1.0, 1.0); // se indica cual sera el color para limpiar la ventana	(r,v,a,al)

    glEnable(GL_DEPTH_TEST); // se habilita el z-bufer

    redimensionar(UI_window_width, UI_window_height);
}

// **************************************************************************
// Funcion que dibuja el objeto activo actual, usando su método 'draw'
// (llamada desde Escena::dibujar)
// ***************************************************************************

void Escena::dibujar_objeto_actual()
{
    using namespace std;

    if(modo_draw && modo_dibujo==3)
        modo_dibujo = modo_dibujo-2;

    if(activar_luz)
        luces->activar();
    else
        luces->desactivar();

    if(activar_textura)
        glEnable(GL_TEXTURE_2D);
    else
        glDisable(GL_TEXTURE_2D);

    switch (modo_dibujo)
    {
    case 0:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Pinta en modo lineas
        break;
    case 1:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Pinta en modo relleno
        glEnableClientState(GL_COLOR_ARRAY);

        if(!modo_shade)
            glShadeModel(GL_SMOOTH);
        else
            glShadeModel(GL_FLAT);

        break;
    case 2:
        
        glDisableClientState(GL_COLOR_ARRAY);
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); //Pinta en modo puntos
        glPointSize(5);
        break;
    case 3:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Se elige entre tetraedro o cubo y se pinta su modo ajedrez
        break;
    }

    // (2) dibujar el objeto actual usando método 'draw' del objeto asociado al
    // valor entero en 'objeto_actual'

    switch (objeto_actual)
    {
    case 0:
        if (cubo != nullptr){
            glDisable(GL_TEXTURE_2D);
            cubo->draw(modo_dibujo, modo_draw);
        }
        break;
    case 1:
        if (tetraedro != nullptr)
            tetraedro->draw(modo_dibujo, modo_draw); //Se dibuja el tetraedro
        break;
    case 2:
        if (ply != nullptr)
            ply->draw(modo_dibujo, modo_draw);
        break;
    case 3:
        if (cilindro != nullptr){
            cilindro->draw(modo_dibujo, modo_draw);
        }
        break;
    case 4:
        if (esfera != nullptr){
            esfera->draw(modo_dibujo, modo_draw);
        }
        break;
    case 5:
        if (cono != nullptr)
            cono->draw(modo_dibujo, modo_draw);
        break;
    case 6:
        if (peon != nullptr)
            peon->draw(modo_dibujo, modo_draw);
        break;
    case 7:
        if (obj != nullptr)
            obj->draw(modo_dibujo, modo_draw);
        break;
    case 8:
        if (cuadro != nullptr){
            cuadro->draw(modo_dibujo, modo_draw);
            cuadro->textura();
        }
        break;

    default:
        cout << "draw_object: el número de objeto actual (" << objeto_actual << ") es incorrecto." << endl;
        break;
    }
}

// **************************************************************************
//
// función de dibujo de la escena: limpia ventana, fija cámara, dibuja ejes,
// y dibuja los objetos
//
// **************************************************************************

void Escena::dibujar()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpiar la pantalla
    glEnable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    change_observer();
    ejes.draw();
    dibujar_objeto_actual();
    
}

//**************************************************************************
//
// función que se invoca cuando se pulsa una tecla
// Devuelve true si se ha pulsado la tecla para terminar el programa (Q),
// devuelve false en otro caso.
//
//**************************************************************************

bool Escena::teclaPulsada(unsigned char tecla, int x, int y)
{
    using namespace std;
    cout << "Tecla pulsada: '" << tecla << "'" << endl;

    switch (tecla)
    {
    case 'q':
    case 'Q':
        // salir
        return true;
        break;

    case 'o':
    case 'O':
        // activar siguiente objeto
        objeto_actual = (objeto_actual + 1) % num_objetos;
        cout << "objeto actual == " << objeto_actual << endl;
        break;

    case 'm':
    case 'M':
        //Se cambia el modo de dibujo de lineas a puntos a relleno, o ajedrez
        modo_dibujo = (modo_dibujo + 1) % 4;
        break;

    case 'p':
    case 'P':
        //Se activa el parametro siguiente al actual
        obj->siguienteParametro();
        break;

    case 'a':
    case 'A':
        //Activa o desactiva las animaciones
        if(obj != nullptr && objeto_actual == 7)
        {
            animacion? animacion=false : animacion=true;
            this->conmutarAnimaciones();
        }
        break;
    
    case 'i':
    case 'I':
        luz? luz=false : luz=true;
        this->conmutarAnimacionesLuz();
        break;

    case 'x':
    case 'X':
            num_material = (num_material + 1) % nMateriales;
            malla.activarMaterial(num_material);
        break;

    case 'l':
    case 'L':
            activar_luz = !activar_luz;
        break;

    case 't':
    case 'T':
            if(objeto_actual==8)
                activar_textura = !activar_textura;
        break;

    case 's':
    case 'S':
        modo_shade = !modo_shade;

    case 'Z':
        //Se incrementa el valor del parámetro actual del objeto jerárquico
        obj->incrementaParamAct();
        break;

    case 'z':
        //Se incrementa el valor del parámetro actual del objeto jerárquico
        obj->decrementaParamAct();
        break;

    case '>':
        //Se incrementa el valor de incremento/decremento usado para las animaciones y las modificaciones del parámetro actual
        obj->acelerar();
        break;

    case '<':
        //Se decrementa el valor de incremento/decremento usado para las animaciones y las modificaciones del parámetro actual
        obj->decelerar();
        break;
        break;

    case 'v':
    case 'V':
         if(!modo_draw){
            cout << "Se cambia el modo de dibujo a diferido " << endl;
            modo_draw = true;
         }
        else{
            cout << "Se cambia el modo de dibujo a inmediato " << endl;
            modo_draw=false;
        }
        break;
    }

    return false;
}
//**************************************************************************

void Escena::teclaEspecial(int Tecla1, int x, int y)
{
    switch (Tecla1)
    {
    case GLUT_KEY_LEFT:
        Observer_angle_y--;
        break;
    case GLUT_KEY_RIGHT:
        Observer_angle_y++;
        break;
    case GLUT_KEY_UP:
        Observer_angle_x--;
        break;
    case GLUT_KEY_DOWN:
        Observer_angle_x++;
        break;
    case GLUT_KEY_PAGE_UP:
        Observer_distance *= 1.2;
        break;
    case GLUT_KEY_PAGE_DOWN:
        Observer_distance /= 1.2;
        break;
    }

    //std::cout << Observer_distance << std::endl;
}

//**************************************************************************
// Funcion para definir la transformación de proyeccion
//
// ratio_xy : relacción de aspecto del viewport ( == ancho(X) / alto(Y) )
//
//***************************************************************************

void Escena::change_projection(const float ratio_xy)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    const float wy = 0.84 * Front_plane,
                wx = ratio_xy * wy;
    glFrustum(-wx, +wx, -wy, +wy, Front_plane, Back_plane);
}
//**************************************************************************
// Funcion que se invoca cuando cambia el tamaño de la ventana
//***************************************************************************

void Escena::redimensionar(int newWidth, int newHeight)
{
    Width = newWidth;
    Height = newHeight;
    change_projection(float(Width) / float(Height));
    glViewport(0, 0, Width, Height);
}

//**************************************************************************
// Funcion para definir la transformación de vista (posicionar la camara)
//***************************************************************************

void Escena::change_observer()
{
    // posicion del observador
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -Observer_distance);
    glRotatef(Observer_angle_x, 1.0, 0.0, 0.0);
    glRotatef(Observer_angle_y, 0.0, 1.0, 0.0);
}

void Escena::mgeDesocupado()
{
    if (objeto_actual == 7 && animacion)
        obj->actualizarEstado();

    if (glIsEnabled(GL_LIGHTING) == GL_TRUE)
        luces->incrementarAngulo();

    glutPostRedisplay();
}

void Escena::conmutarAnimaciones()
{
    if (animacion)
    {
        obj->inicioAnimaciones();
        glutIdleFunc(funcion_desocupado);
    }
    else
    {
        glutIdleFunc(nullptr);
    }
}

void Escena::conmutarAnimacionesLuz()
{
    if (luz)
        glutIdleFunc(funcion_desocupado);
    else
        glutIdleFunc(nullptr);
}

