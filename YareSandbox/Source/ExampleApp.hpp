
#ifndef OPENGL_CMAKE_SKELETON_MYApp
#define OPENGL_CMAKE_SKELETON_MYApp

#include <Yare/App.hpp>
#include <Yare/FileSystem.hpp>
#include <Yare/Renderer/Shader.hpp>
#include <Yare/Renderer/Platform.hpp>
#include <Yare/Renderer/VertexArray.hpp>

using namespace yare;

class ExampleApp : public App {
 public:
  ExampleApp();

 protected:
  virtual void onRender() override;

 private:
  float time = 0.f;
  const int size = 100;

  // shader

  std::shared_ptr<Shader > simpleShader;

  // shader matrix uniform
  glm::mat4 projection = glm::mat4(1.0);
  glm::mat4 view = glm::mat4(1.0);

  // VBO/VAO/ibo
  std::shared_ptr<VertexArray> vertexArray;
};

#endif  // OPENGL_CMAKE_SKELETON_MYApp
