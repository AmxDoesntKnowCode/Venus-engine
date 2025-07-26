#ifndef SHADER_H
#define SHADER_H

#include "../../src/include/GL/glew.h"
#include "../../src/include/GLFW/glfw3.h"
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

struct ShaderProgramSource
{
    std::string VertexSource; 
    std::string FragmentSource; 
};

ShaderProgramSource ParseShader(const std::string& filepath);

unsigned int CompileShader(unsigned int type, const std::string& source);

unsigned int CreateShader(const std::string& vertexShader, const std::string fragmentShader);

#endif