#include <stdlib.h>
#include <stdio.h>

#include <GLFW/glfw3.h>

GLFWwindow* window;

#define WIDTH  800
#define HEIGHT 800

void error_callback(int error, const char* description) {
  fprintf(stderr, "Error: %s\n", description);
}

int main(void) {
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    return -1;
  }

  window = glfwCreateWindow(WIDTH, HEIGHT, "Laboratorio 2", NULL, NULL);
  if (!window) {
    fprintf(stderr, "Error\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}