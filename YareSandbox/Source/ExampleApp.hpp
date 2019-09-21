
#pragma once

#include <Yare/App.hpp>
#include <Yare/OS/FileSystem.hpp>
#include <Yare/Graphics/SkyBox.hpp>

#include <GLFW/glfw3.h>


//YARE_PROJECT_DIR defined by cmake
#define YARESANDBOX_ASSET(path) YARESANDBOX_PROJECT_DIR "/Assets/" path


using namespace yare;
using namespace yare::graphics;
using namespace yare::os;

enum PhongTextureSlot
{
	Diffuse = 0, Specular, 
	/*Shininess,*/
	Count
};


class PhongMesh : public Mesh
{
public:
	PhongMesh();
	void preRender()override;
	void postRender()override;

private:
	
	std::unique_ptr<Texture>	_textures[(int)PhongTextureSlot::Count];
	float _shininess;
};

class ExampleApp : public App {
 public:
  ExampleApp();
  ~ExampleApp() = default;

 protected:
  
	void onRender() override;
	void onEnter() override;
	void onExit() override;


 private:
	float _time = 0.f;
	const int _size = 100;

	// shader


	// shader matrix uniform
	glm::mat4 _projection = glm::mat4(1.0);
	glm::mat4 _model = glm::mat4(1.0); 
	
	Scene _scene;
	Camera _camera;
	std::unique_ptr<SkyBox>		_skybox;

	std::unique_ptr<PhongMesh>		_phongMesh;



};
