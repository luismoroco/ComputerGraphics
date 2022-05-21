#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

const char* vertexShaderSource = 
  "#version 330 core\n"
  "layout (location = 0) in vec4 position;\n"
  "void main()\n"
  "{\n"
  " gl_Position = position;\n"
  "}\n\0";

const char* fragmentShaderSource = 
  "#version 330 core\n"
  "out vec4 color;"
  "void main()\n"
  "{\n"
  " color = vec4(0.2f, 0.7f, 0.4f, 1.0f);\n"
  "}\n\0";

int main(int, char**) {
  glfwInit();
  // Standard -> OpenGL 3.3 Core (MAJOR, MINOR) Puro
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL", nullptr, nullptr);
  if (!window) {
    std::cout << "Error GLFW!\n";
    glfwTerminate();
    return -1;
  } 
  glfwMakeContextCurrent(window); // el hilo de ejecución se encarga de la ventana

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Error! GLAD\n";
    glfwTerminate();
    return -1;
  }
  
  float positions[] = {
    -0.5f, -0.5,     // vertice 1
     0.5f, -0.5f,    // vertice 2  
     0.0f,  0.5f     // vertice 3 
  };
  

  // VerteArrayObject
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // creamos un buffer para el objeto
  unsigned int buffer;
  glGenBuffers(1, &buffer); // numero de  buffers / direccion de buffer
  glBindBuffer(GL_ARRAY_BUFFER, buffer); // que queremos dibujar / en que buffer quieres escribir la info
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); // en que buffer se escribirá la data

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
  glEnableVertexAttribArray(0);

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);

  int success;
  char log[100];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 100, nullptr, log);
    std::cout << "Error VertexShader!\n";
    return -1;
  }

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 100, nullptr, log);
    std::cout << "Error fragmentShader!\n";
    return -1;
  }

  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shaderProgram, 100, nullptr, log);
    std::cout << "Error shaderProgram!\n";
    return -1;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  glUseProgram(shaderProgram);

  // render loop
  while (!glfwWindowShouldClose(window)) {
    // actualizar
    
    //dibujas
    glClearColor(0.3f, 0.2f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteProgram(shaderProgram);
  glDeleteBuffers(1, &buffer);
  glDeleteVertexArrays(1, &VAO);

  glfwTerminate();
  return 0;
}
