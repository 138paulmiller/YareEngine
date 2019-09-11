
#pragma once

#include <Yare/App.hpp>
#include <Yare/System/FileSystem.hpp>
#include <Yare/Graphics/Shader.hpp>
#include <Yare/Graphics/Camera.hpp>
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
  ~ExampleApp() = default;

 protected:
  
	void onRender(Renderer* renderer) override;
	void onEnter() override;
	void onExit() override;


 private:
	float _time = 0.f;
	const int _size = 100;

	// shader


	// shader matrix uniform
	glm::mat4 _projection = glm::mat4(1.0);
	glm::mat4 _model = glm::mat4(1.0); 
	Camera _camera;

	std::shared_ptr<Shader > _simpleShader;
	std::shared_ptr<VertexArray> _vertexArray;
	std::shared_ptr<Texture> texture;
	std::unique_ptr<SkySphere> _skySphere;
	RenderCommand _command;


};
