#define __USE_MINGW_ANSI_STDIO 0

#include <iostream>
#include <ogl/glew.h>
#include <ogl/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shaders.h"
#include "Model.h"

#define I glm::mat4(1.0f)

void funInit();
void funDestroy();
void funReshape(int w, int h);
void funDisplay();
//void funSpecial(int key, int x, int y);

void drawObject(Model* object, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSuelo (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCylinder(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSphereGreen(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSphereBlue(glm::mat4 P, glm::mat4 V, glm::mat4 M);
  
// Shaders
   Shaders* shaders;
   
// Modelos
   Model* plane;
   Model* sphere;
   Model* cylinder;

// Viewport
   int w = 500;
   int h = 500;
   
// Animaciones
   float desZ = 0.0f;
   float rotZ = 0.0f;
   
int main(int argc, char** argv) {

 // Inicializamos GLUT
    glutInit(&argc, argv);
    glutInitContextVersion(3,3);   
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE); 
    glutInitContextProfile(GLUT_CORE_PROFILE); 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(50,50);
    glutCreateWindow("Practica 1");
    
 // Inicializamos GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return false;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    const GLubyte* oglVersion = glGetString(GL_VERSION);
    std::cout <<"This system supports OpenGL Version: " << oglVersion << std::endl;
    
 // Inicializaciones específicas
    funInit();
    
 // Configuración CallBacks
    glutReshapeFunc(funReshape);
    glutDisplayFunc(funDisplay);
   // glutSpecialFunc(funSpecial);
         
 // Bucle principal
    glutMainLoop();
    
 // Liberamos memoria
    funDestroy();
    
    return 0;
}

void funInit() {
   
 // Test de profundidad
    glEnable(GL_DEPTH_TEST); 
    glPolygonOffset(1.0f,1.0f);
    
 // Shaders
    shaders = new Shaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");

 // Modelos
    plane    = new Model("resources/models/plane.obj");
    sphere   = new Model("resources/models/sphere.obj");
    cylinder = new Model("resources/models/cylinder.obj");
    
}

void funDestroy() {
      
    delete shaders;
    delete plane,cylinder, sphere;
    
}

void funReshape(int wnew, int hnew) {
    
 // Configuración del Viewport
    glViewport(0, 0, wnew, hnew);

 // Captura de w y h
    w = wnew;
    h = hnew;
    
}

void funDisplay() {
    
 // Borramos el buffer de color
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
 // Indicamos los shaders a utilizar
    shaders->use();
    
 // Matriz de Proyección P (Perspectiva)
    float fovy   = 30.0f;
    float nplane =  0.1f;
    float fplane = 20.0f;
    float aspectRatio = (float)w/(float)h;
    glm::mat4 P = glm::perspective(glm::radians(fovy), aspectRatio, nplane, fplane); 

 // Matriz de Vista V (Cámara)
    glm::vec3 pos   (-3.0f, 2.0f, 3.0f);
    glm::vec3 lookat(0.0f, 0.0f,  0.0f);
    glm::vec3 up    (0.0f, 1.0f,  0.0f);
    glm::mat4 V = glm::lookAt(pos, lookat, up);   

 // Dibujamos la escena
    drawSuelo(P,V,I);
    drawCylinder(P,V,I);
    drawSphereGreen(P,V,I);
    drawSphereBlue(P,V,I);
    
 // Intercambiamos los buffers
    glutSwapBuffers();
    
}

void drawObject(Model* object, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    
    shaders->setMat4("uPVM",P*V*M);
    glEnable(GL_POLYGON_OFFSET_FILL);
        shaders->setVec3("uColor",color);
        object->render(GL_FILL);
    glDisable(GL_POLYGON_OFFSET_FILL);
    shaders->setVec3("uColor",glm::vec3((color.x)*(0.75),(color.y)*(0.75),(color.z)*(0.75)));
    object->render(GL_LINE);
    
}
void drawSuelo(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(2.0f,2.0f,2.0f));
    drawObject(plane,glm::vec3(0.4f,0.4f,0.4f),P,V,M*S);
     
}
void drawBase(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    
    glm::mat4 Tz = glm::translate(I, glm::vec3(0.0f,0.0f,1.0f));
    glm::mat4 Ry = glm::rotate   (I, glm::radians(-90.f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 T = glm::translate(I, glm::vec3(0.0f,0.05f,0.0f));
    glm::mat4 S = glm::scale(I, glm::vec3(0.25,0.0514f,0.25f));
    drawObject(cylinder,glm::vec3(1.0f,1.0f,0.0f),P,V,M*Tz*Ry*T*S);
     
}
void drawSphereGreen(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    //drawEsferaAzul(P,V,M);
    glm::mat4 Tz = glm::translate(I, glm::vec3(0.0f,0.0f,1.0f));
    glm::mat4 Ry = glm::rotate   (I, glm::radians(-90.f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 S = glm::scale(I, glm::vec3(0.101947f,0.101947f,0.101947f));
    glm::mat4 T = glm::translate(I, glm::vec3(0.0f,0.25f,0.0f));
    glm::mat4 T1 = glm::translate(I, glm::vec3(0.0f,0.0f,0.5f));
    drawObject(sphere,glm::vec3(0.0f,1.0f,0.0f),P,V,M*Tz*Ry*T1*T*S);
     
}

void drawSphereBlue(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    
    glm::mat4 Tz = glm::translate(I, glm::vec3(0.0f,0.0f,1.0f));
    glm::mat4 Ry = glm::rotate   (I, glm::radians(-90.f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 S = glm::scale(I, glm::vec3(0.1529f,0.1529f,0.1529f));
    glm::mat4 T = glm::translate(I, glm::vec3(0.0f,0.25f,0.0f));
    //glm::mat4 T1 = glm::translate(I, glm::vec3(0.0f,0.0f,0.5f));
    drawObject(sphere,glm::vec3(0.0f,0.0f,1.0f),P,V,M*Tz*Ry*T*S);
     
}

void drawCylinderBlue(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    
    glm::mat4 Tz = glm::translate(I, glm::vec3(0.0f,0.0f,1.0f));
    glm::mat4 R = glm::rotate   (I, glm::radians(90.f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 Ry = glm::rotate   (I, glm::radians(-90.f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 S = glm::scale    (I, glm::vec3(0.075, 0.257f, 0.075f));
    glm::mat4 R90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 T = glm::translate(I, glm::vec3(0.0f,0.25f,0.0f));
    glm::mat4 T1 = glm::translate(I, glm::vec3(0.0f,0.0f,0.25f));
     M = M*Tz*Ry*T*T1*R*R90*S;
    drawObject(cylinder,glm::vec3(0.0f,0.0f,1.0f),P,V,M);
    
}

void drawCylinderGreen(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    
    glm::mat4 Tz = glm::translate(I, glm::vec3(0.0f,0.0f,1.0f));
    glm::mat4 Ry = glm::rotate   (I, glm::radians(-90.f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 R = glm::rotate   (I, glm::radians(90.f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 S = glm::scale    (I, glm::vec3(0.050f, 0.257f, 0.050f));
    glm::mat4 R90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 T = glm::translate(I, glm::vec3(0.0f,0.25f,0.0f));
    glm::mat4 T1 = glm::translate(I, glm::vec3(0.0f,0.0f,0.75f));
     M = M*Tz*Ry*T*T1*R*R90*S;
    drawObject(cylinder,glm::vec3(0.0f,1.0f,0.0f),P,V,M); 
    
}
void drawCylinderRed(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 Tz = glm::translate(I, glm::vec3(0.0f,0.0f,1.0f));
    glm::mat4 Ry = glm::rotate   (I, glm::radians(-90.f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 R = glm::rotate   (I, glm::radians(90.f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 S = glm::scale    (I, glm::vec3(0.025f, 0.257f, 0.025f));
    glm::mat4 R90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 T = glm::translate(I, glm::vec3(0.0f,0.25f,0.0f));
    glm::mat4 T1 = glm::translate(I, glm::vec3(0.0f,0.0f,1.0f));
    M = M*Tz*Ry*T*T1*R*R90*S;
    drawObject(cylinder,glm::vec3(1.0f,0.0f,0.0f),P,V,M);
}

void drawCylinder(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    //Llamadas a los cilindros verde, azul y rojo 
    drawCylinderGreen(P,V,M);
    drawCylinderBlue(P,V,M);
    drawCylinderRed (P,V,M);
    //Base Cilindro Amarilla
    drawBase(P,V,M);
    
   
     
}
