#ifndef SKYBOX_H
#define SKYBOX_H
#include <string>
#include <GL/glew.h>

using namespace std;

class SkyBoxTexture 
{
    public:
        SkyBoxTexture(const string& dir,
                      const string& PosXFileName, 
                      const string& NegXFileName, 
                      const string& PosYFileName, 
                      const string& NegYFileName, 
                      const string& PosZFileName, 
                      const string& NegZFileName);
        ~SkyBoxTexture();
        bool Load();

        boid Bind(GLenum TextureUnit); 

    private:
        string m_fileNames[6];
        GLuint m_textureObj; 
};

#endif
