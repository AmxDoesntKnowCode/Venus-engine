#include "shader.h"
#include<GLFW/glfw3.h>

ShaderProgramSource ParseShader(const std::string& filepath){
    std::ifstream stream(filepath);
    
    enum class ShaderType {
        NONE = -1,
        VERTEX = 0, 
        FRAGMENT = 1, 
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while(getline(stream, line)){

        if(line.find("#shader") != std::string::npos){
            if(line.find("vertex") != std::string::npos) 
                type = ShaderType::VERTEX;
            else if(line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;  

        }else{
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

unsigned int CompileShader(unsigned int type, const std::string& source){
    unsigned int id = glCreateShader(type); 
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr); 
    glCompileShader(id); 

    // TODO: Error handling 
    int result; 
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE){
        int length; 
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << message << '\n';
        glDeleteShader(id);
        return 0; 
    }
    

    return id; 
} 

unsigned int CreateShader(const std::string& vertexShader, const std::string fragmentShader){
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    glAttachShader(program, vs); 
    glAttachShader(program, fs); 
    glLinkProgram(program); 
    glValidateProgram(program); 

    glDeleteShader(vs); 
    glDeleteShader(fs);

    return program; 
}


/*
    vec4 texColor = texture(ourTexture, TextCoord); // sample the tex first
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos); 

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float ambientStrength = 0.3;
    vec3 ambient = lightColor * ambientStrength; 

    float specularStrength = 0.6;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = texColor * vec4(result, 1.0);
    //FragColor = vec4(1.0, 0.5, 0.0 ,1.0);

    //FragColor = texColor; 
*/

