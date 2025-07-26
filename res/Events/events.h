#ifndef EVENTS_H
#define EVENTS_H
#include "../../module.h"
#include "../../src/include/GLFW/glfw3.h"
#include"../../src/include/glm/glm.hpp"
#include"../../src/include/glm/gtc/matrix_transform.hpp"
#include"../../src/include/glm/gtc/type_ptr.hpp"


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void processInput(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

#endif
