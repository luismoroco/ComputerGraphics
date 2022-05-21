#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <random>

bool changue_color = false;

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
    changue_color = true;
  }   
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
    changue_color = false;
  } 
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
  glViewport(0, 0, width, height);
}

const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main()\n"
  "{\n"
  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "uniform vec4 ourColor;\n"
  "void main()\n"
  "{\n"
  "   FragColor = ourColor;\n"
  "}\n\0";

int main(int, char**) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Laboratorio 3: Presiona C para cambiar de colores y R para parar", nullptr, nullptr);
  if (!window) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
    
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  float vertices[] = {
    0.0f,  0.5f, 0.0f,  
   -0.5f, -0.0f, 0.0f,  
    0.0f, -0.5f, 0.0f,  
    0.5f,  0.0f, 0.0f,
    0.5f,  0.5f, 0.0f,
   -0.5f,  0.5f, 0.0f,
    0.0f,  1.0f, 0.0f,  
    1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, -1.0f, 0.0f,
   -0.5f, -0.5f, 0.0f,
   -1.0f, 0.0f, 0.0f
  };
  unsigned int indices[] = {  
    0, 1, 2, 
    0, 2, 3,  
    4, 5, 6,   
    4, 7, 8,  
    8, 9, 10,  
    5, 11, 10
  };
  
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0); 
  glBindVertexArray(0); 

  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    int probablidad = rand() % 10;
    if (probablidad == 5) {
      float RW = static_cast<float>(rand() / (RAND_MAX + 1.0));
      float GW = static_cast<float>(rand() / (RAND_MAX + 1.0));
      float BW = static_cast<float>(rand() / (RAND_MAX + 1.0));
      float XW = static_cast<float>(rand() / (RAND_MAX + 1.0));
      glClearColor(RW, GW, BW, XW);
    }
    
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO); 

    if (changue_color) {
      float R = static_cast<float>(rand() / (RAND_MAX + 1.0));
      float G = static_cast<float>(rand() / (RAND_MAX + 1.0));
      float B = static_cast<float>(rand() / (RAND_MAX + 1.0));
      float X = static_cast<float>(rand() / (RAND_MAX + 1.0));

      int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
      glUniform4f(vertexColorLocation, R, G, B, X);
    }

    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  
  return 0;
}
