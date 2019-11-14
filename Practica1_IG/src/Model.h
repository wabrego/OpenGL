#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <ogl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
    
    public:

        Model(const char* modelFile);

        void render(GLenum mode);
         
        virtual ~Model();
         
    private:
        
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> textureCoords;
        std::vector<GLushort>  indices;
    
        GLuint vao;
        GLuint vboPositions;
        GLuint vboNormals;
        GLuint vboTextureCoords;
        GLuint eboIndices;

};

#endif /* MODEL_H */
