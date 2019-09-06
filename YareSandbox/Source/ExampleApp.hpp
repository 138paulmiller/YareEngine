
#pragma once

#include <Yare/App.hpp>
#include <Yare/System/FileSystem.hpp>
#include <Yare/Graphics/Shader.hpp>
#include <Yare/Graphics/Platform.hpp>
#include <Yare/Graphics/Texture.hpp>
#include <Yare/Graphics/VertexArray.hpp>


#include <Yare/SkySphere.hpp>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/transform.hpp>

//YARE_PROJECT_DIR defined by cmake
#define YARESANDBOX_ASSET(path) YARESANDBOX_PROJECT_DIR "/Assets/" path


using namespace yare;
using namespace yare::graphics;
using namespace yare::system;


class Camera
{
public:
	Camera(
		const glm::vec3& up = {0,1,0},
		const glm::vec3& right = {1,0,0}
	) :_up(up), _right(right){}

	void setPosition(const glm::vec3& position)
	{
		_position = position;
	}
	const glm::vec3& getPosition() { return _position; }

	const glm::mat4& getView() { return _view; }
	void lookAt(const glm::vec3& target) { _view = glm::lookAt(_position, target, _up); }

private:
	glm::vec3 _position;
	glm::vec3 _up;
	glm::vec3 _right;
	glm::mat4 _view;
	//Have camera handle projection as well

};

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
