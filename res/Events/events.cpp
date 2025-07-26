#include "events.h"
#include "../../module.h"
#include <thread>
#include <chrono>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_Q && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if(canJump && (key == GLFW_KEY_SPACE && action == GLFW_PRESS)){
       velocityY += jumpStr;
    }
    if(key == GLFW_KEY_B && action == GLFW_PRESS){
        gravity = 0.0f;
        jumpStr = 0.0f;
        velocityY = 0.0f;
    }

   if(key == GLFW_KEY_N && action == GLFW_PRESS){
       gravity = -12.7f;
       jumpStr = 5.0;
   }
   if (key == GLFW_KEY_K && action == GLFW_PRESS){
        ShowCursor = !ShowCursor; 
    }
   
   if (key >= 0 && key < 1024) { // WE AINT GON HAVE NO BUFFER OVERFLOWS IN THIS HOUSE 
       if (action == GLFW_PRESS) {
           keys[key] = true;
       } else if (action == GLFW_RELEASE) {
           keys[key] = false;
       }
   }
}

void processInput(GLFWwindow* window){

    if (keys[GLFW_KEY_W]){
        cameraPos += cameraSpeed * cameraFront * deltaTime;
    }
    if (keys[GLFW_KEY_S]){
        cameraPos -= cameraSpeed * cameraFront * deltaTime;
    }
    if (keys[GLFW_KEY_A]){
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
    }
    if (keys[GLFW_KEY_D]){
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
    }

        

     if (keys[GLFW_KEY_W] && keys[GLFW_KEY_LEFT_SHIFT])
        cameraPos += spdboost * cameraFront * deltaTime;
    
    if (keys[GLFW_KEY_S]&& keys[GLFW_KEY_LEFT_SHIFT])
        cameraPos -= spdboost * cameraFront * deltaTime;
    
    if (keys[GLFW_KEY_A]&& keys[GLFW_KEY_LEFT_SHIFT])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * spdboost * deltaTime;
    
    if (keys[GLFW_KEY_D]&& keys[GLFW_KEY_LEFT_SHIFT])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * spdboost * deltaTime;
   
    if(keys[GLFW_KEY_R])
        fov = 80.0f;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds screen doesn't get flipped
     
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    fov -= (float)yoffset;
    if (fov < 30.0f)
        fov = 30.0f;
    if (fov > 120.0f)
        fov = 120.0f;
}

