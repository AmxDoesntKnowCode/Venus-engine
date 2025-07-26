//TODO: Texture Loading to Models
// Most of which is boiler-plate code ig i will just copy smth off of AI 
// or StackOverFlow or smth idfk
//TODO: Materials

// glfw and glew
#include "src/include/GL/glew.h"
#include "src/include/GLFW/glfw3.h"

// imGui
#include "src/include/imgui/imgui.h"
#include "src/include/imgui/imgui_spectrum.h"
#include "src/include/imgui/backends/imgui_impl_glfw.h"
#include "src/include/imgui/backends/imgui_impl_opengl3.h"


// stb_image
#define STB_IMAGE_IMPLEMENTATION
#include "src/include/stb_image.h"


// glm 
#include"src/include/glm/glm.hpp"
#include"src/include/glm/gtc/matrix_transform.hpp"
#include"src/include/glm/gtc/type_ptr.hpp"

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<thread>
#include<chrono>
#include<cmath>
#include<vector>

#include"res/shaders/shader.h"
#include"module.h"
#include"res/Events/events.h"
#include"res/ImGui_functions/ImGui_Func.h"
#include"res/Handle/Models/OBJ_Loader.h"


struct ModelMesh {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int indexCount; // how many indices to draw
    // yep thats a TODO
    // objl::Material material;
};



int main(int argc, char* argv[]){
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Venus", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);


    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwWindowHint(GLFW_SAMPLES, 4);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "retard the error is: " << glewGetErrorString(err) << '\n';
    }
    

    /* we need to initialize it after making the context since that basically
     * makes a openGL context which is required
    */
    
    std::vector<ModelMesh> temple;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    InitImGui(window); // initialize ImGui
    SetImGuiStyle();
    ImGui::StyleColorsLight();

    glfwSetKeyCallback(window, key_callback); // a function that handles key presses ig
    glfwSetScrollCallback(window, scroll_callback);

    
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the VAO then bind and set vertex buffers and then configure vertex attributes...yep no fucking idea thats what the opengl said
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // aPos (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // aNormal (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // aColor (location = 2) - vertex data layout for colors (3 floats)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // aTexCoord (location = 3) - vertex data layout for texture coordinates (2 floats)
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glBindVertexArray(0);

    unsigned int light_VAO, light_VBO;
    glGenVertexArrays(1, &light_VAO);
    glGenBuffers(1, &light_VBO);

    glBindVertexArray(light_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, light_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);

    // position attribute for light source (only 3 floats per vertex)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // unbind lightCubeVAO
   

    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);    
    glGenBuffers(1, &skyboxEBO);

    // bind VAO first (all subsequent calls will be stored in this VAO)
    glBindVertexArray(skyboxVAO);

    // bind and fill VBO (vertex data)
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    // bind and fill EBO (index data)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), skyboxIndices, GL_STATIC_DRAW);

    // set vertex attribute pointer (position, 3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // unbind VBO and VAO (EBO stays bound because it's stored in VAO)
    glBindBuffer(GL_ARRAY_BUFFER, 0);               
    glBindVertexArray(0); // unbind VAO (EBO is automatically unbound from VAO)
    // i think im getting the hang of it  

    // yes i stole your shader code mr cherno and i have no shame in doing so
    // also yes i dont have a shader class...yep i can feel your blood pressure rising
    ShaderProgramSource source = ParseShader("res/shaders/Basic2.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);
    unsigned int location = glGetUniformLocation(shader, "FragColor");

    // retrieve and set view and projection uniform locations ONCE per frame
    unsigned int viewLoc  = glGetUniformLocation(shader, "view");
    unsigned int projectionLoc = glGetUniformLocation(shader, "projection");
    unsigned int lightColorLoc = glGetUniformLocation(shader, "lightColor");
    unsigned int objectColorLoc = glGetUniformLocation(shader, "objectColor");
    unsigned int modelLoc = glGetUniformLocation(shader, "model");
    unsigned int lightPosLoc = glGetUniformLocation(shader, "lightPos");
    unsigned int viewPosLoc = glGetUniformLocation(shader, "viewPos");
    unsigned int ambientLoc = glGetUniformLocation(shader, "ambientStr");
    unsigned int specularLoc = glGetUniformLocation(shader, "specularStr");
    unsigned int shininessLoc = glGetUniformLocation(shader, "shininessStr");

    // light-source shader
    ShaderProgramSource lightShader_source = ParseShader("res/shaders/light.shader");
    unsigned int light_shader = CreateShader(lightShader_source.VertexSource, lightShader_source.FragmentSource);
    glUseProgram(light_shader);
    unsigned int light_viewLoc  = glGetUniformLocation(light_shader, "view");
    unsigned int light_projectionLoc = glGetUniformLocation(light_shader, "projection");
    unsigned int light_modelLoc = glGetUniformLocation(light_shader, "model");

    //skybox shader 
    ShaderProgramSource skybox_source = ParseShader("res/shaders/skybox.shader");
    unsigned int skybox_shader = CreateShader(skybox_source.VertexSource, skybox_source.FragmentSource);
    glUseProgram(skybox_shader);
    unsigned int skybox_tex = glGetUniformLocation(skybox_shader, "skybox");
    unsigned int skybox_projectionLoc = glGetUniformLocation(skybox_shader, "projection");
    unsigned int skybox_viewLoc  = glGetUniformLocation(skybox_shader, "view");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("res/textures/dcontainer.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else  std::cout << "Failed to load texture" << '\n';
    stbi_image_free(data);

   // skybox texture
    unsigned int SkyBoxT_texture;
    glGenTextures(1, &SkyBoxT_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBoxT_texture);
    
    // set the texture wrapping/filtering options 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    std::string facesCubemap[6] = {
        "res/textures/right.jpg",
        "res/textures/left.jpg",
        "res/textures/top.jpg",
        "res/textures/bottom.jpg",
        "res/textures/front.jpg",
        "res/textures/back.jpg",    

    };
 
    for(unsigned int i = 0; i < 6; i++){
        int SkyBoxT_width, SkyBoxT_height, SkyBoxT_nrChannels;
        unsigned char *SkyBoxT_data = stbi_load(facesCubemap[i].c_str(), &SkyBoxT_width, &SkyBoxT_height, &SkyBoxT_nrChannels, 0);

        if (SkyBoxT_data)
        {
            stbi_set_flip_vertically_on_load(false);
            glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0,
                    GL_RGB,
                    SkyBoxT_width,
                    SkyBoxT_height,
                    0,
                    GL_RGB,
                    GL_UNSIGNED_BYTE,
                    SkyBoxT_data
                );
            stbi_image_free(SkyBoxT_data);
        }else{
            std::cout << "Failed to load texture" << '\n'; 
            return -1;
        }
    }
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    objl::Loader loader;
    if(loader.LoadFile("res/media/models/sponza.obj")) {
       for(int i = 0; i < loader.LoadedMeshes.size(); i++) {
           objl::Mesh curMesh = loader.LoadedMeshes[i];

           ModelMesh renderMesh; // a new instance for this mesh

           // access vertices:
           std::vector<objl::Vertex> vertices = curMesh.Vertices;

           // access indices:
           std::vector<unsigned int> indices = curMesh.Indices;
           
           renderMesh.indexCount = indices.size(); // store the number of indices

           glGenVertexArrays(1, &renderMesh.VAO);
           glBindVertexArray(renderMesh.VAO);

           glGenBuffers(1, &renderMesh.VBO);
           glBindBuffer(GL_ARRAY_BUFFER, renderMesh.VBO);
           glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(objl::Vertex), &vertices[0], GL_STATIC_DRAW);

           glGenBuffers(1, &renderMesh.EBO);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderMesh.EBO);
           glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

           glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (void*)offsetof(objl::Vertex, Position));
           glEnableVertexAttribArray(0);

           glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (void*)offsetof(objl::Vertex, Normal));
           glEnableVertexAttribArray(1);

           glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(objl::Vertex), (void*)offsetof(objl::Vertex, TextureCoordinate));
           glEnableVertexAttribArray(2);

           glBindVertexArray(0);
           glBindBuffer(GL_ARRAY_BUFFER, 0);
           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

           // add the prepared mesh to the list of renderable meshes
           temple.push_back(renderMesh);

       }

    }

    

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glEnable(GL_CULL_FACE);
        canJump = false;
        DrawImGui(window); // draw all the menues and shit
        glClearColor(SkyRGB[0], SkyRGB[1], SkyRGB[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear both color and depth
        processInput(window);


        glUseProgram(shader);
        glBindVertexArray(VAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);


        // calculate delta time so the program has a stable performance
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //calculate fps
        if(currentFrame - lastFpsUpdate >= FPS_DISPLAY_FREQ){
            fps = 1 / deltaTime; 
            DisplayFps = static_cast<int>(fps); 

            // reset the timer for the next FPS display update
            lastFpsUpdate = currentFrame; 

        }

        if(VSYNC) glfwSwapInterval(1); // enable V-Sync
        else if(!VSYNC) glfwSwapInterval(0);
   
       // TODO: fix MSSA not working 
        if(MSSA) glEnable(GL_MULTISAMPLE);
        else if(!MSSA) glDisable(GL_MULTISAMPLE);

        if(ShowCursor) 
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            sensitivity = 0.0f;
        }else if(!ShowCursor){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            sensitivity = 0.5f;
        }
        
    
       //std::cout << "Camera Pos: x=" << cameraPos.x << ", y=" << cameraPos.y << ", z=" << cameraPos.z << std::endl;

        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)1280 / (float)720, 0.1f, 1000.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glUniform1f(ambientLoc, ambientSlider);
        glUniform1f(specularLoc, specularSlider);
        glUniform1f(shininessLoc, Material_Shininess);
       
        // here i break Acerola's rule of graphics programming
        // DO NOT PARSE VARIABLES BETWEEN THE CPU AND GPU 
        // but i will do it anyways since i really dont see any other option 
        glm::vec3 lightColor(Light_Color[0], Light_Color[1], Light_Color[2]);  
        glm::vec3 objectColor(1.0f , 1.0f , 1.0f);
        glm::vec3 lightPos(LightXPos, LightYPos, LightZPos);

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glUniform3f(lightColorLoc, lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(objectColorLoc, objectColor.x, objectColor.y, objectColor.z);

        glUseProgram(light_shader);
        glBindVertexArray(light_VAO);

        glUniformMatrix4fv(light_viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(light_projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        velocityY += gravity * deltaTime;
        cameraPos.y += velocityY * deltaTime;

        bool onGround = false;
        float oldCameraY = cameraPos.y;
        // players bounding box
        glm::vec3 playerMin = cameraPos - glm::vec3(playerRadius, 0.0f, playerRadius);
        glm::vec3 playerMax = cameraPos + glm::vec3(playerRadius, playerHeight, playerRadius);

        // you bet ya ass i stole that shi from learnopengl.com
        if(LightAnim)
        {
            lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
            lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
        }
 

        glUseProgram(shader);

        // ive noticed learnopengl.com doesnt update the 
        // light's pos so the lighting is static
        // tho here i made it dynamic
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z); // update the light's pos
        glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z); // update the camera's pos
                             
        glm::mat4 templeModel = glm::mat4(1.0f);
        templeModel = glm::translate(templeModel, glm::vec3(1.0f, -1.0f, 1.0f));
        templeModel = glm::scale(templeModel, glm::vec3(TempleModelScale));


        glUseProgram(light_shader);
        glm::mat4 light_model = glm::mat4(1.0f);
        light_model = glm::translate(light_model, lightPos);
        light_model = glm::scale(light_model, glm::vec3(0.2f));
        glUniformMatrix4fv(light_modelLoc, 1, GL_FALSE, glm::value_ptr(light_model));
        glBindVertexArray(light_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glUseProgram(shader);
        glBindVertexArray(VAO);

        glBindVertexArray(0);
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(templeModel));

        for (const auto& mesh : temple) {
            glBindVertexArray(mesh.VAO);
           
            glBindVertexArray(mesh.VAO); // bind the VAO for the mesh
            glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0); 
            glBindVertexArray(0); // unbind VAO 
        }

        glDepthFunc(GL_LEQUAL);  // change depth func so skybox passes depth test at 1.0
        glDisable(GL_CULL_FACE); // disable face culling so skybox works :D

        glUseProgram(skybox_shader);
        glActiveTexture(GL_TEXTURE0);

        // remove translation from the view matrix for skybox
        glm::mat4 skyboxView = glm::mat4(glm::mat3(view)); 

        glUniformMatrix4fv(skybox_viewLoc, 1, GL_FALSE, glm::value_ptr(skyboxView));
        glUniformMatrix4fv(skybox_projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBoxT_texture);
        glUniform1i(skybox_tex, 0);  

        glBindVertexArray(skyboxVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glDepthFunc(GL_LESS);  // set depth function back to default

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

    }


    // clean up the shader
    glDeleteProgram(shader);
    glDeleteProgram(light_shader);
    glDeleteProgram(skybox_shader);


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glDeleteVertexArrays(1, &light_VAO);
    glDeleteBuffers(1, &light_VBO);


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;


}

