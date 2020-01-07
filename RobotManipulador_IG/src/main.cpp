#include <iostream>
#include <ogl/glew.h>
#include <ogl/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shaders.h"
#include "Model.h"

#include <sstream>
std::string toString(const int &i) {
    std::stringstream ss;
    ss << i;
    return ss.str();
}

#define I glm::mat4(1.0f)

void funInit();
void funDestroy();
void funReshape(int w, int h);
void funDisplay();

//dibujo
void drawObjectMat(Model *object, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawObjectTex(Model *object, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void setLights(glm::mat4 P, glm::mat4 V);
void drawPlane(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawBase(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCylinder(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSphere(glm::mat4 P, glm::mat4 V, glm::mat4 M);

//animacion
void mouseFunc(int button, int state, int x, int y);
void camara(int x, int y);
void moverMano(int value);
void keyBoardFunc(unsigned char key , int x , int y);
void specialFun(int key, int x, int y);

// Shaders
   Shaders* shaders;
   
// Modelos
   Model* plane;
   Model* sphere;
   Model* cylinder;

// Viewport
   int w = 600;
   int h = 600;
   
// Color 0.75
   float x = 0.0f;
   float y = 0.0f;
   float z = 0.0f;
   float fov = 30.0f;
   float giroPyD = 0.0f;
   float rotBase = 0.0f;
   float rotAzul = 0.0f;
   float rotVerde = 0.0f;
   float dR   = 0.0f;
   float dX   = 0.0f;
   float dZ   = 0.0f;
   float alphaY = 0.0f;
   float alphaX = 0.0f;
   float LuzD = 0.7f;
   float LuzF = 0.090f;
   
   // Luces
   #define  NLD 1
   #define  NLP 1
   #define  NLF 1
   Light    lightG;
   Light    lightD[NLD];
   Light    lightP[NLP];
   Light    lightF[NLF];
   
   // Materiales
   Material matRuby;
   Material matLuces;
   Material matEmerald;
   Material matEmeraldEsfera;
   
   // Texturas
   Texture  *texNoEmissive;
   Textures texSuelo;
   Textures texBrazo;
   Textures texBase;
   Textures texLight;

   
int main(int argc, char** argv) {

 // Inicializamos GLUT
    glutInit(&argc, argv);
    glutInitContextVersion(3,3);   
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE); 
    glutInitContextProfile(GLUT_CORE_PROFILE); 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(50,50);
    glutCreateWindow("Sesion 5");
    
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
    glutMouseFunc(mouseFunc);
    glutMotionFunc(camara);
    glutTimerFunc(30,moverMano,1);
    glutKeyboardFunc(keyBoardFunc);
    glutSpecialFunc(specialFun);
         
 // Bucle principal
    glutMainLoop();
    
 // Liberamos memoria
    funDestroy();
    
    return 0;
}

void funInit() {
   
 // Test de profundidad
    glEnable(GL_DEPTH_TEST); 
    
 // Transparencias
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
   
    
 // Shaders
    shaders = new Shaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");

 // Modelos
    plane    = new Model("resources/models/plane.obj");
    sphere   = new Model("resources/models/sphere.obj");
    cylinder = new Model("resources/models/cylinder.obj");
              
 // Materiales
    
    matRuby.ambient    = glm::vec4(0.174500f, 0.011750f, 0.011750f, 1.0f);
    matRuby.diffuse    = glm::vec4(0.614240f, 0.041360f, 0.041360f, 1.0f);
    matRuby.specular   = glm::vec4(0.727811f, 0.626959f, 0.626959f, 1.0f);
    matRuby.emissive   = glm::vec4(0.0f);
    matRuby.shininess  = 50.0f;
    
    matEmerald.ambient    = glm::vec4(0.0215f, 0.1745f, 0.0215f, 0.75f );
    matEmerald.diffuse    = glm::vec4(0.07568f, 0.61424f, 0.07568f, 0.75f );
    matEmerald.specular   = glm::vec4(0.633f, 0.727811f, 0.633f, 0.75f );
    matEmerald.emissive   = glm::vec4(0.0f);
    matEmerald.shininess  = 50.0f;
    
    matEmeraldEsfera.ambient    = glm::vec4(0.0215f, 0.1745f, 0.0215f, 1.0f );
    matEmeraldEsfera.diffuse    = glm::vec4(0.07568f, 0.61424f, 0.07568f, 1.0f );
    matEmeraldEsfera.specular   = glm::vec4(0.633f, 0.727811f, 0.633f, 1.0f );
    matEmeraldEsfera.emissive   = glm::vec4(0.0f);
    matEmeraldEsfera.shininess  = 50.0f;
    
 // Texturas
    texNoEmissive     = new Texture("resources/textures/imgNoEmissive.png");

    texSuelo.diffuse   = new Texture("resources/textures/imgSueloDiffuse.png");   
    texSuelo.specular  = new Texture("resources/textures/imgSueloSpecular.png");
    texSuelo.emissive  = new Texture("resources/textures/imgSueloEmissive.png"); 
    texSuelo.normal    = NULL;
    texSuelo.shininess = 50.0f;
    
    texBrazo.diffuse   = new Texture("resources/textures/imgBrazoDiffuse.png");   
    texBrazo.specular  = texBrazo.diffuse;
    texBrazo.emissive  = texNoEmissive; 
    texBrazo.normal    = NULL;
    texBrazo.shininess = 50.0f;
    
    texBase.diffuse   = new Texture("resources/textures/imgBaseDiffuse.png");
    texBase.specular  = texBase.diffuse;
    texBase.emissive  = texNoEmissive; 
    texBase.normal   = NULL;
    texBase.shininess = 50.0f;
    
    texLight.diffuse   = new Texture("resources/textures/imgLuces.png");   
    texLight.specular  = texLight.diffuse;
    texLight.emissive  = texNoEmissive; 
    texLight.normal    = NULL;
    texLight.shininess = 50.0f;
    
    glutPostRedisplay();
}

void funDestroy() {
      
    delete shaders;
    delete plane,cylinder, sphere;
    delete texNoEmissive;
    delete texSuelo.diffuse, texSuelo.specular, texSuelo.emissive;
    delete texBrazo.diffuse, texBrazo.specular, texBrazo.emissive;
    delete texBase.diffuse, texBase.specular, texBase.emissive;
    delete texLight.diffuse, texLight.specular, texLight.emissive;
    
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
    float fovy   = fov;
    float nplane = 0.1f;
    float fplane = 20.0f;
    float aspectRatio = (float)w/(float)h;
    glm::mat4 P = glm::perspective(glm::radians(fovy), aspectRatio, nplane, fplane); 

 // Matriz de Vista V (Cámara)
    
    float x = 10.0f*glm::cos(alphaY)*glm::sin(alphaX);
    float y = 10.0f*glm::sin(alphaY);
    float z = 10.0f*glm::cos(alphaY)*glm::cos(alphaX);
    glm::vec3 pos   (   x,    y,    z);
    glm::vec3 lookat(0.0f, 0.0f,  0.0f);
    glm::vec3 up    (0.0f, 1.0f,  0.0f);
    glm::mat4 V = glm::lookAt(pos, lookat, up);
    shaders->setVec3 ("ucpos",pos);
    
// Luz ambiental global
    lightG.ambient      = glm::vec3(0.7f, 0.7f, 0.7f);    
    
// Luces direccionales
    lightD[0].direction = glm::vec3(0.0f, -1.0f, 0.0f);
    lightD[0].ambient   = glm::vec3(0.1f, 0.1f, 0.1f);
    lightD[0].diffuse   = glm::vec3(LuzD, LuzD, LuzD);
    lightD[0].specular  = glm::vec3(LuzD, LuzD, LuzD); 
    
// Luces focales
    lightF[0].position    = glm::vec3(3.0f,  3.0f, -3.0f);
    lightF[0].direction   = glm::vec3(3.0f, 3.0f, 3.0f);
    lightF[0].ambient     = glm::vec3(0.1f, 0.1f, 0.1f);
    lightF[0].diffuse     = glm::vec3(0.9f, 0.9f, 0.9f);
    lightF[0].specular    = glm::vec3(0.9f, 0.9f, 0.9f);
    lightF[0].innerCutOff = 10.0f;
    lightF[0].outerCutOff = 11.0f;
    lightF[0].c0          = 1.000f;
    lightF[0].c1          = LuzF;
    lightF[0].c2          = LuzF;    
    
 // Luces posicionales
    lightP[0].position  = glm::vec3(0.0f, 0.2f, -1.5f);
    lightP[0].ambient   = glm::vec3(0.1f, 0.1f, 0.1f);
    lightP[0].diffuse   = glm::vec3(0.9f, 0.9f, 0.9f);
    lightP[0].specular  = glm::vec3(0.9f, 0.9f, 0.9f);
    lightP[0].c0        = 1.000f;
    lightP[0].c1        = 0.220f;
    lightP[0].c2        = 0.200f;   
    
 // Fijamos las luces
    setLights(P,V);

 // Dibujamos la escena
    drawPlane(P,V,I);
    drawBase(P,V,I);
    drawCylinder(P,V,I);
    drawSphere(P,V,I);
    
    
 // Intercambiamos los buffers
    glutSwapBuffers();
    
}

void drawObjectMat(Model* object, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M) {
     
    shaders->setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders->setMat4("uM"  ,M);
    shaders->setMat4("uPVM",P*V*M);
    shaders->setBool("umaterials",true);
    shaders->setMaterial("umaterial",material);
    object->render(GL_FILL);
    
}

void drawObjectTex(Model* object, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders->setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders->setMat4("uM"  ,M);
    shaders->setMat4("uPVM",P*V*M);
    shaders->setBool("umaterials",false);
    shaders->setTextures("utextures",textures);
    object->render(GL_FILL);

}

void setLights(glm::mat4 P, glm::mat4 V) {
    
    shaders->setLight("ulightG",lightG);
    for(int i=0; i<NLD; i++) shaders->setLight("ulightD["+toString(i)+"]",lightD[i]);
    for(int i=0; i<NLP; i++) shaders->setLight("ulightP["+toString(i)+"]",lightP[i]);    
    for(int i=0; i<NLF; i++) shaders->setLight("ulightF["+toString(i)+"]",lightF[i]);
    
    for(int i=0; i<NLP; i++) {
        glm::mat4 M = glm::scale(glm::translate(I,lightP[i].position),glm::vec3(0.025f));
        drawObjectTex(sphere,texLight,P,V,M);
    }

    for(int i=0; i<NLF; i++) {
        glm::mat4 M = glm::scale(glm::translate(I,lightF[i].position),glm::vec3(0.025f));
        drawObjectTex(sphere,texLight,P,V,M);
    }
    
}

void drawPlane(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    
    //Escalado del plano
    glm::mat4 Sp = glm::scale(I, glm::vec3(2.0f,2.0f,2.0f));
    
    //Dibujar plano
    drawObjectTex(plane,texSuelo,P,V,M*Sp);
     
}

void drawBase(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    
    //Escalados
    glm::mat4 Sb = glm::scale(I, glm::vec3(0.25,0.05f,0.25f));
    
    //Translaciones
    glm::mat4 Ty  = glm::translate(I, glm::vec3(0.0f,0.05f,0.0f));
    glm::mat4 Dx  = glm::translate(I, glm::vec3(dX,0.0f,0.0f));
    glm::mat4 Dy  = glm::translate(I, glm::vec3(0.0f,0.0f,dZ));
    
    //Rotaciones
    glm::mat4 Rbase = glm::rotate   (I, glm::radians(rotBase),  glm::vec3(0.0f, 1.0f, 0.0f));
    
    //Base
    drawObjectTex(cylinder,texBase,P,V,M*Dx*Dy*Rbase*Ty*Sb);
     
}

void drawCylinder(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    
    //Escalados
    glm::mat4 Sr = glm::scale    (I, glm::vec3(0.025f, 0.25f, 0.025f));
    glm::mat4 Sv = glm::scale    (I, glm::vec3(0.050f, 0.25f, 0.050f)); 
    glm::mat4 Sa = glm::scale    (I, glm::vec3(0.075f, 0.25f, 0.075f));
    glm::mat4 Sc = glm::scale    (I, glm::vec3(0.1f, 0.025f, 0.1f));
    glm::mat4 Sm = glm::scale    (I, glm::vec3(0.01f, 0.05f, 0.01f));
    
    //Translaciones
    glm::mat4 Ty       = glm::translate(I, glm::vec3(0.0f,0.25f,0.0f));
    glm::mat4 Tzrojo1  = glm::translate(I, glm::vec3(0.0f,0.0f,0.5f));
    glm::mat4 Tzrojo2  = glm::translate(I, glm::vec3(0.0f,0.0f,0.5f));
    glm::mat4 Tzverde1 = glm::translate(I, glm::vec3(0.0f,0.0f,0.25f));
    glm::mat4 Tzverde2 = glm::translate(I, glm::vec3(0.0f,0.0f,0.5f));
    glm::mat4 Tzazul   = glm::translate(I, glm::vec3(0.0f,0.0f,0.25f));
    glm::mat4 Tzcyan1  = glm::translate(I, glm::vec3(0.0f,0.0f,0.775f));
    glm::mat4 Tzcyan2  = glm::translate(I, glm::vec3(0.0f,0.0f,0.5f));
    glm::mat4 Tzmor1   = glm::translate(I, glm::vec3(0.0f,0.0f,0.850f));
    glm::mat4 Tzmor2   = glm::translate(I, glm::vec3(0.0f,0.0f,0.50f));
    glm::mat4 Trojo    = glm::translate(I, glm::vec3(0.0f,0.0f,dR));
    glm::mat4 Tr1      = glm::translate(I, glm::vec3(0.0f,0.09f,0.0f));
    glm::mat4 Tr2      = glm::translate(I, glm::vec3(0.0f,-0.09f,0.0f));
    glm::mat4 Tr3      = glm::translate(I, glm::vec3(0.09f,0.0f,0.0f));
    glm::mat4 Tr4      = glm::translate(I, glm::vec3(-0.09f,0.0f,0.0f));
    glm::mat4 Dx       = glm::translate(I, glm::vec3(dX,0.0f,0.0f));
    glm::mat4 Dy       = glm::translate(I, glm::vec3(0.0f,0.0f,dZ));
    
    //Rotaciones
    glm::mat4 Ry90    = glm::rotate   (I, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 Rz90    = glm::rotate   (I, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 Rz      = glm::rotate   (I, glm::radians(giroPyD)  , glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 Rbase   = glm::rotate   (I, glm::radians(rotBase) , glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 Razul   = glm::rotate   (I, glm::radians(rotAzul) , glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 Rverde  = glm::rotate   (I, glm::radians(rotVerde) , glm::vec3(1.0f, 0.0f, 0.0f));
    
    //Cylinder ruby
    drawObjectMat(cylinder,matRuby,P,V,M*Dx*Dy*Rbase*Ty*Razul*Tzrojo2*Rverde*Tzrojo1*Trojo*Ry90*Rz90*Sr);
    
    //Cylinder esmeralda
    drawObjectMat(cylinder,matEmerald,P,V,M*Dx*Dy*Rbase*Ty*Razul*Tzverde2*Rverde*Tzverde1*Ry90*Rz90*Sv);
    
    //Cilindro Blue
    drawObjectTex(cylinder,texBrazo,P,V,M*Dx*Dy*Rbase*Ty*Razul*Tzazul*Ry90*Rz90*Sa);
    
    //Cylinder cyan
    drawObjectTex(cylinder,texBase,P,V,M*Dx*Dy*Rbase*Ty*Razul*Tzcyan2*Rverde*Tzcyan1*Rz*Trojo*Ry90*Rz90*Sc);
    
    //Cylinders ruby
    drawObjectMat(cylinder,matRuby,P,V,M*Dx*Dy*Rbase*Ty*Razul*Tzmor2*Rverde*Tzmor1*Trojo*Rz*Tr1*Ry90*Rz90*Sm);
    drawObjectMat(cylinder,matRuby,P,V,M*Dx*Dy*Rbase*Ty*Razul*Tzmor2*Rverde*Tzmor1*Trojo*Rz*Tr2*Ry90*Rz90*Sm);
    drawObjectMat(cylinder,matRuby,P,V,M*Dx*Dy*Rbase*Ty*Razul*Tzmor2*Rverde*Tzmor1*Trojo*Rz*Tr3*Ry90*Rz90*Sm);
    drawObjectMat(cylinder,matRuby,P,V,M*Dx*Dy*Rbase*Ty*Razul*Tzmor2*Rverde*Tzmor1*Trojo*Rz*Tr4*Ry90*Rz90*Sm);
    
}

void drawSphere(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    
    //Escalados
    glm::mat4 Sv = glm::scale(I, glm::vec3(0.05f,0.05f,0.05f));
    glm::mat4 Sa = glm::scale(I, glm::vec3(0.075f ,0.075f ,0.075f));
    
    //Rotaciones
    glm::mat4 Rbase  = glm::rotate   (I, glm::radians(rotBase),  glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 Razul  = glm::rotate   (I, glm::radians(rotAzul),  glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 Rverde = glm::rotate   (I, glm::radians(rotVerde),  glm::vec3(1.0f, 0.0f, 0.0f));
    
    //Translaciones
    glm::mat4 Ty = glm::translate(I, glm::vec3(0.0f,0.25f,0.0f));
    glm::mat4 Tz = glm::translate(I, glm::vec3(0.0f,0.0f,0.5f));
    glm::mat4 Dx = glm::translate(I, glm::vec3(dX,0.0f,0.0f));
    glm::mat4 Dy = glm::translate(I, glm::vec3(0.0f,0.0f,dZ));
    
    //Sphere esmeralda
    drawObjectMat(sphere,matEmeraldEsfera,P,V,M*Dx*Dy*Rbase*Ty*Razul*Tz*Rverde*Sv);
    
    //Sphere Blue
    drawObjectTex(sphere,texBrazo,P,V,M*Dx*Dy*Rbase*Ty*Razul*Sa);
     
}
// Zoom
void mouseFunc(int button, int state, int x, int y){
    
    if (button==3){
        if (fov>10){
            fov = fov - 1.0f;
        }
            
    }else if (button==4){
        if (fov<60){
            fov = fov + 1.0f;
        }
    }  
    
    glutPostRedisplay();
}

void camara(int x, int y){       
    
    float pi   = glm::pi<float>();
    float limY = glm::radians( 89.0f);
    
    alphaX = pi*((float)x/(float)w - 0.5f);
    alphaY = pi*(0.5f - (float)y/(float)h);
    if(alphaY<-limY) alphaY = -limY;      
    if(alphaY> limY) alphaY =  limY;
    glutPostRedisplay();
    
}
// Configurar timer para mover la mano completa
void moverMano(int value) {
    
    giroPyD = giroPyD +5.0;// Rotar 5 grados
    glutTimerFunc(30,moverMano,1);// Cada 30 ms
    glutPostRedisplay();
    
}
//Utilizar glutKeyBoardFunc para rotar todo el manipulador
void keyBoardFunc(unsigned char key , int x , int y){
    
    switch(key){
        
        case 'b':
            rotBase = rotBase + 5.0;
            break;
        
        case 'B':
            rotBase = rotBase - 5.0;
            break;  
            
        case 'a':
            if (rotAzul>-180.0){
                 rotAzul = rotAzul - 5.0;
            }
            break;
        
        case 'A':
            if (rotAzul < 0.0){
                rotAzul = rotAzul + 5.0;
            }    
            break;  
        
        case 'v':
            if(rotVerde > -90.0){
                rotVerde = rotVerde - 5.0;
            }  
            break;
        
        case 'V':
            if(rotVerde < 90.0){
                rotVerde = rotVerde + 5.0;
            }
            break;
        
        case 'r':
            if(dR > -0.25){
                dR = dR - 0.01;
            }  
            break;
        
        case 'R':
            if(dR < 0.25){
                dR = dR + 0.01;
            }
            break;
            
        case 's':
            if(LuzD > -0.5){
                LuzD = LuzD - 0.1;
            }  
            break;
       
        case 'S':
            if(LuzD < 1.0){
                LuzD = LuzD + 0.1;
            }
            break; 
        case 'f':
            if(LuzF == 0.090f){
                LuzF = 1.0f;
            }else{
                LuzF = 0.090f;
            } 
            break;  
    }
    
}
void specialFun(int key, int x, int y) {
   
    switch(key) {
        
        case GLUT_KEY_LEFT: 
            if (dX < 1.70)
                dX += 0.05f;   
            break;
            
        case GLUT_KEY_RIGHT:
            if (dX > -1.70)
                dX -= 0.05f;   
            break;
        
        case GLUT_KEY_UP: 
            if (dZ < 1.70)
                dZ += 0.05f;   
            break;
        
        case GLUT_KEY_DOWN:  
            if (dZ > -1.70)
                dZ -= 0.05f;   
            break;
            
    }
    glutPostRedisplay();
    
}






