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

    // crear los objetos de las prácticas: Mallas o Jerárquicos....
    cubo = new Cubo();

    tetraedro = new Tetraedro();

    ply = new ObjPLY("plys/big_dodge.ply");

    cilindro = new Cilindro("plys/cilindro.ply");
    //cilindro = new CilindroPrueba();

    esfera = new Esfera("plys/esfera.ply");

    cono = new Cono("plys/cono.ply");

    // .......completar: ...
    // .....

    num_objetos = 6; // se usa al pulsar la tecla 'O' (rotar objeto actual)
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

    //Vector de colores para dibujar el cubo y el tetraedro
    /*float gl_color[] =
        {
            1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0,
            1.0, 1.0, 0.0,
            1.0, 0.0, 1.0,
            0.0, 1.0, 1.0,
            1.0, 0.5, 0.0,
            0.3, 0.8, 0.6};*/

    switch (modo_dibujo)
    {
    case 0:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Pinta en modo lineas
        break;
    case 1:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Pinta en modo relleno
        glEnableClientState(GL_COLOR_ARRAY);

        switch (objeto_actual)
        {
        case 0:
            glColorPointer(3, GL_FLOAT, 0, cubo->getColores().data());
            break;
        case 1:
            glColorPointer(3, GL_FLOAT, 0, tetraedro->getColores().data());
            break;
        case 2:
            glColorPointer(3, GL_FLOAT, 0, ply->getColores().data());
            break;
        case 3:
            glColorPointer(3, GL_FLOAT, 0, cilindro->getColores().data());
            break;   
        case 4:
            glColorPointer(3, GL_FLOAT, 0, esfera->getColores().data());
            break; 
        case 5:
            glColorPointer(3, GL_FLOAT, 0, cono->getColores().data());
            break; 
        }
        break;
    case 2:
        glDisableClientState(GL_COLOR_ARRAY);
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); //Pinta en modo puntos
        glPointSize(5);
        break;
    case 3:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Se elige entre tetraedro o cubo y se pinta su modo ajedrez
        switch (objeto_actual)
        {
        case 0:
            cubo->ModoAjedrez();
            break;
        case 1:
            tetraedro->ModoAjedrez();
            break;
        case 2:
            ply->ModoAjedrez();
            break;
        case 3:
            cilindro->ModoAjedrez();
            break;
        case 4:
            esfera->ModoAjedrez();
            break;
         case 5:
            cono->ModoAjedrez();
            break;
        }
    }

    // (2) dibujar el objeto actual usando método 'draw' del objeto asociado al
    // valor entero en 'objeto_actual'

    switch (objeto_actual)
    {
    case 0:
        if (cubo != nullptr)
            cubo->draw(modo_draw);
        break;
    case 1:
        if (tetraedro != nullptr)
            tetraedro->draw(modo_draw); //Se dibuja el tetraedro
        break;
    case 2:
        if (ply != nullptr)
            ply->draw(modo_draw);
        break;
    case 3:
        if (cilindro != nullptr)
            cilindro->draw(modo_draw);
        break;
    case 4:
        if (esfera != nullptr)
            esfera->draw(modo_draw);
        break;
    case 5:
        if (cono != nullptr)
            cono->draw(modo_draw);
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

    switch (toupper(tecla))
    {
    case 'Q':
        // salir
        return true;
        break;
    case 'O':
        // activar siguiente objeto
        objeto_actual = (objeto_actual + 1) % num_objetos;
        cout << "objeto actual == " << objeto_actual << endl;
        break;
    case 'M':
        //Se cambia el modo de dibujo de lineas a puntos a relleno, o ajedrez
        modo_dibujo = (modo_dibujo + 1) % 4;
        break;
    case 'V':
        //Se cambia el modo de dibujo de inmediato a diferido
        if (!modo_draw)
        {
            cout << "Se cambia el modo de dibujo a inmediato " << endl;
            modo_draw = true;
            cubo->draw(modo_draw);
            tetraedro->draw(modo_draw);
            ply->draw(modo_draw);
            cilindro->draw(modo_draw);
            esfera->draw(modo_draw);
        }
        else
        {
            cout << "Se cambia el modo de dibujo a diferido" << endl;
            modo_draw = false;
            cubo->draw(modo_draw);
            tetraedro->draw(modo_draw);
            ply->draw(modo_draw);
            cilindro->draw(modo_draw);
            esfera->draw(modo_draw);
            cono->draw(modo_draw);
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
