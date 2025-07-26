#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 texCoords;

//uniform mat4 model; 
uniform mat4 projection; 
uniform mat4 view; 

void main() 
{
    // remove translation from view matrix so it looks infinite 
    mat4 skyboxView = mat4(mat3(view));
    vec4 pos = projection * skyboxView * vec4(aPos, 1.0);
    
    // force depth to far plane (z = w)
    gl_Position = pos.xyww;
    
    // pass raw position as texture coords 
    texCoords = vec3(aPos.x, aPos.y, -aPos.z);}
// -------------------------- //

#shader fragment
#version 330 core
out vec4 FragColor; 
in vec3 texCoords;

uniform samplerCube skybox; 

void main() 
{
    FragColor = texture(skybox, texCoords);
    //FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}
