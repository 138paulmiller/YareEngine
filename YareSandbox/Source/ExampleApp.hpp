
#pragma once

#include <Yare/App.hpp>
#include <Yare/System/FileSystem.hpp>
#include <Yare/Graphics/Shader.hpp>
#include <Yare/Graphics/Camera.hpp>
#include <Yare/Graphics/Platform.hpp>
#include <Yare/Graphics/Texture.hpp>
#include <Yare/Graphics/VertexArray.hpp>


#include <Yare/SkySphere.hpp>

#include <GLFW/glfw3.h>


//YARE_PROJECT_DIR defined by cmake
#define YARESANDBOX_ASSET(path) YARESANDBOX_PROJECT_DIR "/Assets/" path


using namespace yare;
using namespace yare::graphics;
using namespace yare::system;


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
  glm::mat4 model = glm::mat4(1.0); 
  Camera camera;

  // VBO/VAO/ibo
  std::shared_ptr<VertexArray> vertexArray;
  std::shared_ptr<Texture> texture;
  std::unique_ptr<SkySphere> _skySphere;

};
