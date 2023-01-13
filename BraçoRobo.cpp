/*
    Universidade Federal Do Piauí - UFPI
    Computação Gráfica - Braço Robótico
    Profº: Dr. Laurindo de Sousa Britto Neto
    Autor: Neemias Calebe Pereira Freire
*/

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#define ESC 27

static int ombro = 0, cotovelo = 0, mao = 0, indicador = 0, eixo = 0,
           anelar = 0, polegar = 0;

void init(void);
void keyboard(unsigned char key, int x, int y);
void display(void);
void reshape(int w, int h);

void base(void);
void arm(void);
void elbow(void);
void forearm(void);
void hand(void);
void shoulder(void);
void indicator(void);
void ring(void);
void thumb(void);

int main(int argc, char **argv)
{
    // inicia o GLUT
    glutInit(&argc, argv);

    // inicia o display usando RGB, double-buffering e z-buffering
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Computacao Grafica: Braco Robotico");

    // Funcao com alguns comandos para a inicializacao do OpenGL
    init();

    // define as funcoes de callback
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return EXIT_SUCCESS;
}

void init(void)
{
    glClearColor(1.0, 0.6, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // define o tamanho da area de desenho da janela
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    // Define a forma do volume de visualizacao para termos
    // uma projecao perspectiva (3D).
    gluPerspective(60, (float)w / (float)h, 1.0, 9.0); //(angulo, aspecto, ponto_proximo, ponto distante)
    gluLookAt(0.0, 0.0, 7.0,                           // posicao da camera (olho)
              0.0, 1.0, 0.0,                           // direcao da camera (geralmente para centro da cena)
              0.0, 1.0, 0.0);                          // sentido ou orientacao da camera (de cabeca para cima)
    // muda para o modo GL_MODELVIEW para desenhar na tela
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'o':
        ombro = (ombro - 5) % 360;
        break; // sentido horario
    case 'O':
        ombro = (ombro + 5) % 360;
        break; // sentido anti-horario
    case 'c':
        cotovelo = (cotovelo - 5) % 360;
        break; // sentido horario
    case 'C':
        cotovelo = (cotovelo + 5) % 360;
        break; // sentido anti-horario
    case 'm':
        mao = (mao - 5) % 360;
        break;
    case 'M':
        mao = (mao + 5) % 360;
        break;
    case 'y':
        eixo = (eixo - 5) % 360;
        break;
    case 'Y':
        eixo = (eixo + 5) % 360;
        break;
    case 'i':
        indicador = (indicador + 5) % 360;
        break;
    case 'I':
        indicador = (indicador - 5) % 360;
        break;
    case 'a':
        anelar = (anelar + 5) % 360;
        break;
    case 'A':
        anelar = (anelar - 5) % 360;
        break;
    case 'p':
        polegar = (polegar - 5) % 360;
        break;
    case 'P':
        polegar = (polegar + 5) % 360;
        break;
    case ESC:
        exit(EXIT_SUCCESS);
        break; // sai do programa
    }
    glutPostRedisplay();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa o Buffer de Cores
    glLoadIdentity();
    
    glRotatef((GLfloat) eixo, 0.0, 1.0, 0.0);
    
    glPushMatrix();
        // Base
        glPushMatrix();
        base();
        glPopMatrix();

        // Ombro
        glPushMatrix();
        shoulder(); 
        glPopMatrix();

        // Movimento do braco
        glPushMatrix();
            // origem posicionada no ombro
            glTranslatef(0.0, -1.0, 0.0);
            glRotatef((GLfloat)ombro, 0.0, 0.0, 1.0);
            // origem posicionada no centro do braco
            glTranslatef(0.0, 1.0, 0.0);
            // Braco
            glPushMatrix();
            arm();
            glPopMatrix();

            // Cotovelo
            glPushMatrix();
            elbow(); // raio, fatias, pilhas
            glPopMatrix();

            // Movimento do antebraco
            glPushMatrix();
                // origem posicionada no cotovelo
                glTranslatef(0.0, 1.0, 0.0);
                glRotatef((GLfloat)cotovelo, 0.0, 0.0, 1.0);
                glTranslatef(0.0, 1.0, 0.0);
                
                // Antebraco
                glPushMatrix();
                forearm(); 
                glPopMatrix();

                glTranslatef(0.0, 1.0, 0.0);
                glRotatef((GLfloat) mao, 0.0, 0.0, 1.0);
                glTranslatef(0.0, -1.0, 0.0);
                
                // Mao
                glPushMatrix();
                hand();
                glPopMatrix();

                // Dedo
                glPushMatrix();
                glTranslatef(-0.3, 1.5, 0.07);
                glRotatef((GLfloat) indicador, 0.0, 0.0, 1.0);
                glTranslatef(0.3, -1.5, 0.07);
                indicator();
                glPopMatrix();

                // Anelar
                glPushMatrix();
                glTranslatef(-0.3, 1.5, -0.07);
                glRotatef((GLfloat) anelar, 0.0, 0.0, 1.0);
                glTranslatef(0.3, -1.5, -0.07);
                ring();
                glPopMatrix();

                // Polegar
                glPushMatrix();
                glTranslatef(0.3, 1.5, 0.0);
                glRotatef((GLfloat) polegar, 0.0, 0.0, 1.0);
                glTranslatef(-0.3, -1.5, 0.0);
                thumb();
                glPopMatrix();

            glPopMatrix();
            // origem volta para o sistema de coordenadas original
        glPopMatrix();

    glPopMatrix();
    // Troca os buffers, mostrando o que acabou de ser desenhado
    glutSwapBuffers();
}

void base(void)
{
    glTranslatef(0.0, -2.0, 0.0);
    glRotatef(-90, 1, 0, 0);
    glColor3f(0.0, 0.0, 0.0);
    glutWireCone(1.0, 1.0, 30, 30);
}

void shoulder(void)
{
    glTranslatef(0.0, -1.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    glutWireSphere(0.5, 30, 30);
}

void arm(void)
{
    glRotatef(90, 0, 0, 1);
    glScalef(2.0, 0.5, 0.5);
    glColor3f(0.8, 0.5, 0.3);
    glutSolidCube(1.0); // Tamanho
}

void elbow(void)
{
    glTranslatef(0.0, 1.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    glutWireSphere(0.4, 30, 30);
}

void forearm(void)
{
    glRotatef(90, 0, 0, 1);
    glScalef(2.0, 0.4, 0.4);
    glColor3f(0.0, 0.0, 1.0);
    glutSolidCube(1.0);
}

void hand(void)
{
    glTranslatef(0.0, 1.28, 0.0);
    glScalef(0.8, 0.5, 0.5);
    glColor3f(0.8, 0.0, 0.0);
    glutSolidCube(1.0);
}

void indicator(void){
    glTranslatef(0.0, 1.5, 0.0);
    glScalef(0.5, 0.2, 0.2);
    glColor3f(0.0, 0.5, 0.7);
    glutSolidCube(1.0);
}

void ring(void){
    glTranslatef(0.0, 1.5, 0.0);
    glScalef(0.5, 0.2, 0.2);
    glColor3f(0.0, 0.5, 0.7);
    glutSolidCube(1.0);
}

void thumb(void){
    glTranslatef(0.0, 1.5, 0.0);
    glScalef(0.5, 0.2, 0.2);
    glColor3f(0.0, 0.5, 0.7);
    glutSolidCube(1.0);
}