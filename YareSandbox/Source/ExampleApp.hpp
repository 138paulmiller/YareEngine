
#pragma once

#include <Yare/App.hpp>
#include <Yare/OS/FileSystem.hpp>
#include <Yare/SkyBox.hpp>

#include <Yare/Materials/PhongMaterial.hpp>
#include <Yare/Materials/FlatMaterial.hpp>
#include <GLFW/glfw3.h>


//YARE_PROJECT_DIR defined by cmake
#define YARESANDBOX_ASSET(path) YARESANDBOX_PROJECT_DIR "/Assets/" path


using namespace yare;
using namespace yare::graphics;
using namespace yare::os;



class ExampleApp : public App 
{
 public:
  ExampleApp();
  ~ExampleApp() = default;

 protected:
  
	void onRender() override;
	void onEnter() override;
	void onExit() override;

	//move these to update method in ECS like system
	void updateCamera(); 
	void updateSkybox();
	void updateBoxes();


 private:
	//FPS calc
	float _elapsedTime = 0.f;
	int _frames = 0;
	
	const int BOX_COUNT = 80;
	const int LIGHT_COUNT = 3;

	Scene _scene;
	std::unique_ptr<PerspectiveCamera >_camera;

	//Use asset manager to load these class from files and manage their gc 
	std::unique_ptr<SkyBox>		_skybox;
	std::unique_ptr<Mesh >       _floorMesh;
	
	std::vector<std::unique_ptr<DirectionalLight >> _directionalLights;
	std::vector<std::unique_ptr<Mesh >>       _directionalLightMeshes;


	std::vector< std::unique_ptr<Mesh>	>	_boxMeshes;

	std::unique_ptr<PhongMaterial> _floorMaterial;
	std::unique_ptr<FlatMaterial> _flatMaterial;
	std::unique_ptr<PhongMaterial> _phongMaterial;


};
