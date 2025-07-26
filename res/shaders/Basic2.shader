#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoord; 

out vec3 ourColor; 
out vec2 TextCoord; 
out vec3 FragPos;  
out vec3 Normal;

//uniform mat4 transform;  were not using this now
uniform mat4 model; 
uniform mat4 projection; 
uniform mat4 view; 

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor; 
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TextCoord = aTexCoord; 
}

// -------------------------- //

#shader fragment
#version 330 core

out vec4 FragColor; 
in vec3 ourColor; 
in vec2 TextCoord; 
in vec3 Normal; 
in vec3 FragPos;  
uniform sampler2D ourTexture;
uniform vec3 lightPos; 
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform float ambientStr;
uniform float specularStr;
uniform float shininessStr;

void main() 
{

    vec3 texelColor = texture(ourTexture, TextCoord).rgb; 
    // normalize the normal vector
    vec3 norm = normalize(Normal);

    // calculate light direction
    vec3 lightDir = normalize(lightPos - FragPos);
    
    vec3 color = vec3(1.0,0.5,0.0);

    // ambient 
    float ambientStrength = ambientStr;// 0.2;
    vec3 ambient = lightColor * ambientStrength * texelColor; 

    // diffuse shit    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * texelColor * diff; 

    // specular shit 
    float specularStrength = specularStr;//0.6; 
    float shininess = shininessStr;//32.0;       
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = lightColor * specularStrength * spec;

    // final combined light 
    vec3 finalLight = ambient + diffuse + specular;


    FragColor = vec4(finalLight * objectColor, 1.0); // multiply finalLight by objectColor as a tint
}



