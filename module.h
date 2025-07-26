// tbh idc what you say this is such a flexible way for variables
// i can define a variable here and use it anywhere


#ifndef MODULE_H
#define MODULE_H
#include"src/include/glm/glm.hpp"
#include"src/include/glm/gtc/matrix_transform.hpp"
#include"src/include/glm/gtc/type_ptr.hpp"
#include "src/include/GLFW/glfw3.h"


extern float currentFrame;
extern float lastFrame;
extern float deltaTime;
extern float fps; 
extern int DisplayFps; 
extern float lastFpsUpdate; 

extern bool keys[1024]; // we will track keys with this mf 
extern bool firstMouse;
extern bool canJump; 
extern bool ShowCursor; 
extern bool LightAnim;
extern bool VSYNC;
extern bool MSSA;
extern float FPS_DISPLAY_FREQ; 
              
extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;

extern float yaw;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
extern float pitch ;
extern float lastX;
extern float lastY;
extern float sensitivity;
extern float fov;
extern float gravity;
extern float velocityY;
extern float jumpStr;
extern float cameraSpeed; 
extern float spdboost;

extern float playerHeight;
extern float playerRadius; // radius for horizontal collision


extern float Material_Shininess; 
extern float ambientSlider;
extern float specularSlider;
extern float LightXPos;
extern float LightYPos;
extern float LightZPos;

extern float TempleModelScale;

extern float SkyRGB[3];
extern float Light_Color[3];

extern float texCoords[6];
extern float vertices[396]; // 288
extern float lightVertices[108];
extern float skyboxVertices[108];
extern unsigned int skyboxIndices[36];


#endif

