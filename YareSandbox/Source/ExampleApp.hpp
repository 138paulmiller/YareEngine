/**
 * MyApp.hpp skeleton
 * Contributors:
 *      * Arthur Sonzogni
 * Licence:
 *      * MIT
 */

#ifndef OPENGL_CMAKE_SKELETON_MYApp
#define OPENGL_CMAKE_SKELETON_MYApp

#include <Yare/App.hpp>
#include <Yare/GL/Shader.hpp>

class ExampleApp : public App {
 public:
  ExampleApp();

 protected:
  virtual void loop();

 private:
  float time = 0.f;
  const int size = 100;

  // shader

  ShaderProgram shaderProgram;

  // shader matrix uniform
  glm::mat4 projection = glm::mat4(1.0);
  glm::mat4 view = glm::mat4(1.0);

  // VBO/VAO/ibo
  GLuint vao, vbo, ibo;
};

#endif  // OPENGL_CMAKE_SKELETON_MYApp
