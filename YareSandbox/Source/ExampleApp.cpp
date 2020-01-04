
#include "ExampleApp.hpp"
#include <Yare/Geometry/Box.hpp>
#include <Yare/Geometry/Quad.hpp>
#include <Yare/Geometry/Sphere.hpp>
#include <Yare/AssetManager.hpp>
#include <iostream>
#include <vector>


struct VertexType {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
};

float heightMap(const glm::vec2 position) {
  return  2 * sin(position.x) * sin(position.y);
}

VertexType getHeightMap(const glm::vec2 position, const glm::vec2 uv) {
  const glm::vec2 dx(1.0, 0.0);
  const glm::vec2 dy(0.0, 1.0);

  VertexType v;
  float h = heightMap(position);
  float hx = 100.f * (heightMap(position + 0.01f * dx) - h);
  float hy = 100.f * (heightMap(position + 0.01f * dy) - h);

  v.position = glm::vec3(position.x, h, position.y);
  v.normal = glm::normalize(glm::vec3(-hx, -hy, 1.0));

  float c = sin(h * 5.f) * 0.5 + 0.5;
  v.uv = uv;
  return v;
}

void CreateHeightMap(
	int xsize, int ysize, 
	std::vector<VertexType> & vertices,
	std::vector<unsigned int> & indices)
{
	BufferLayout vertexLayout = {

			{BufferElementType::Float3, "position"},
			{BufferElementType::Float3, "normal"},
			{BufferElementType::Float2, "uv"},
	};
	vertices.resize(xsize * ysize);
	indices.resize(xsize * ysize * 6);

	for (int y = 0; y <= ysize; ++y)
		for (int x = 0; x <= xsize; ++x) {
			float xx = (x - xsize / 2.0) / xsize * 10;
			float yy = (y - ysize / 2.0) / ysize * 10;


			vertices.push_back(getHeightMap({ xx, yy }, { x / (float)xsize, y / (float)ysize }));
		}

	for (int y = 0; y < ysize; ++y)
		for (int x = 0; x < xsize; ++x) {
			indices.push_back((x + 0) + (xsize + 1) * (y + 0));
			indices.push_back((x + 1) + (xsize + 1) * (y + 0));
			indices.push_back((x + 1) + (xsize + 1) * (y + 1));

			indices.push_back((x + 1) + (xsize + 1) * (y + 1));
			indices.push_back((x + 0) + (xsize + 1) * (y + 1));
			indices.push_back((x + 0) + (xsize + 1) * (y + 0));
		}
	std::cout << "vertices=" << vertices.size() << std::endl;
	std::cout << "index=" << indices.size() << std::endl;

}






ExampleApp::ExampleApp() 
{
}


void ExampleApp::onEnter()
{

	//Create an instance of the phong material. To be shared among meshes

	Texture* diffuse = AssetManager::GetInstance().get<Texture>("container_diffuse");
	Texture * specular = AssetManager::GetInstance().get<Texture>("container_specular");
	
	//All materials should set all textures as empty to default. Move this to texture class to get empty texture
	Texture* empty = Texture::Create();
	TexturePixels pixels = { 0, 0, 0};
	pixels.height = pixels.height = 1;
	pixels.format = TextureFormat::RGB8;
	empty->update(TextureWrap::Repeat);
	empty->load(pixels);
	empty->generateMipMaps();


	diffuse->update(TextureWrap::Repeat);
	specular->update(TextureWrap::Repeat);

	_phongMaterial.reset(new PhongMaterial());
	_phongMaterial->setDiffuseTexture(diffuse);
	_phongMaterial->setSpecularTexture(specular);
	_phongMaterial->setEmissiveTexture(empty);
	_phongMaterial->setShininess(2);

	//Create an instance of the flat material. To be shared among meshes
	_flatMaterial.reset(new FlatMaterial());
	_flatMaterial->setBase(glm::vec3(1, 0, 0));

	_boxMeshes.resize(BOX_COUNT);
	for (int i = 0; i < BOX_COUNT; i++)
	{ //phong mesh
		_boxMeshes[i].reset(new Mesh());
		_boxMeshes[i]->loadVertexArray(geometry::Box::CreateVertexArray({ 1,1,1 }));

		_boxMeshes[i]->setMaterial(_phongMaterial.get());
		RenderCommand& data = _boxMeshes[i]->command;

		data.shader = AssetManager::GetInstance().get<Shader>("phong_mesh");
		data.mode = RenderMode::Mesh;
		data.lighting = RenderLighting::Surface;

	}

	_pointLights.resize(LIGHT_COUNT);
	_pointLightMeshes.resize(LIGHT_COUNT);
	for (int i = 0; i < LIGHT_COUNT; i++)
	{	
		//Create the light and the mesh
		_pointLights[i].reset(new PointLight());
		_pointLights[i]->setAmbient(glm::vec3(0.05f, 0.05f, 0.05f));
		_pointLights[i]->setDiffuse(glm::vec3(0.654f, 0.652f, 0.652f));
		_pointLights[i]->setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
		_pointLights[i]->setAttenuation(glm::vec3(1, 0.5, 0.00004 ));
		
		_pointLightMeshes[i].reset(new Mesh());

		_pointLightMeshes[i]->loadVertexArray(geometry::Box::CreateVertexArray({ 0.15,0.15,0.15 }));
		_pointLightMeshes[i]->setMaterial(_flatMaterial.get());
		
		//Setup render config
		RenderCommand& data = _pointLightMeshes[i]->command;
		data.shader   = AssetManager::GetInstance().get<Shader>("unlit_mesh");
		data.mode     = RenderMode::Mesh;
		data.lighting = RenderLighting::Unlit;
	}
	//Set  up Sky box
	_skybox.reset(new SkyBox());
	Texture* skyboxTexture = AssetManager::GetInstance().get<Cubemap>("skybox_cubemap");
	_skybox->setCubemap(skyboxTexture);


	//Set up the scene
	_scene.setCamera(&_camera);
	_scene.add("Skybox", _skybox.get());
	
	int index = 0;
	for (const std::unique_ptr<PointLight>& pointLight : _pointLights)
	{
		_scene.getLights().setPointLight("PointLight_" + std::to_string(index), pointLight.get());
		index++;
	}
	index = 0;
	for (const std::unique_ptr<Mesh>& boxMesh: _boxMeshes)
	{
		_scene.add("PhongMesh_" + std::to_string(index), boxMesh.get());
		index++;
	}

	index = 0;
	for (const std::unique_ptr<Mesh>& mesh : _pointLightMeshes)
	{
		_scene.add("PointLightMesh_" + std::to_string(index), mesh.get());
		index++;
	}

	
}


void ExampleApp::onExit()
{
}

void ExampleApp::onRender() {

	float time = getTime();

	if (_elapsedTime > 1.0) //for every one second.
	{
		std::cout << "\nFPS = " << _frames / (_elapsedTime);
		_elapsedTime = 0;
		_frames = 0;
	}
	else
	{
		_elapsedTime += getDeltaTime();
		_frames++;
	}
	// set matrix : projection + view
	_projection = glm::perspective(45.0f,getWindowRatio(), 1.0f, 200.0f);
	
	_camera.setProjection(_projection);
	_camera.setPosition({ 0, 0, 10 });
	_camera.setForward(glm::normalize( glm::vec3(0) -  _camera.getPosition() ));
	

	////////// Update State ////////////////
 	 _model = glm::translate(_camera.getPosition());
	 _model *= glm::scale(glm::vec3( 100, 100, 100 ));
	_skybox->setModel(_model);
	
	if (BOX_COUNT > 0)
	{
		if (BOX_COUNT > 1)
		{
			demoMovingBoxesAndLights(time);
		}
		else
		{
			demoRotatingBoxes(time);
		}

	}

	//Submit Scene to be drawn - TODO - SceneRenderer will manage /sort/ cull this process of drawing
	_scene.render(App::getRenderer());
} 

//////////////////////// Varying demoes ////////////////////////////////

void ExampleApp::demoMovingBoxesAndLights(float time)
{

	float yaw = 0, pitch = 0, roll = 0;
	const glm::vec3 up = { 0, 1, 0 };
	const glm::vec3 forward = { 0, 0, 1 };
	const glm::vec3 right = { 1, 0, 0 };
	glm::vec3 position;
	int i = 0;
	float t;
	float amp = 10;
	float speed = 0.025;

	for (i = 0; i < BOX_COUNT; i++)
	{
		t = (float)i / (float)(BOX_COUNT) * 360.0f * 3.14156f / 180.0f;

		position = {
			cos(t + time * speed) * sin(t + time * speed) * amp ,
			sin(t + time * speed) * cos(t + time * speed) * amp ,
			cos(t + time * speed) * amp
		};


		yaw =   time *  (speed + 0.0158) + position.x;
		pitch = time *  (speed + 0.0125) + position.y;
		roll =  time *  (speed + 0.0150) + position.z;
		glm::mat4 transform = glm::mat4(1);
		transform = glm::translate(transform, position);
		transform = glm::rotate(transform, yaw, up);
		transform = glm::rotate(transform, roll, forward);
		transform = glm::rotate(transform, pitch, right);


		_boxMeshes[i]->setModel(transform);

	}

	amp = 15;
	speed = 0.04;
	for (i = 0; i < LIGHT_COUNT; i++)
	{
		t = (float)i / (float)(LIGHT_COUNT) * 360.0f * 3.14156f / 180.0f;
		position = {
			cos(t + time * speed) * amp ,
			sin(t + time * speed) * cos(t + time * speed) * amp ,
			sin(t + time * speed) * amp
		};
		_pointLights[i]->setPosition(position);
		_pointLightMeshes[i]->setModel(glm::translate(position));
	}
}


void ExampleApp::demoRotatingBoxes(float time)
{
	float yaw = 0;
	const glm::vec3 up = { 0, 1, 0 };
	
	glm::vec3 position;
	int i = 0;
	float t;
	float amp = 1;
	float speed = 0.25;

	for (i = 0; i < BOX_COUNT; i++)
	{
		
		position = { 0,0,0};
		yaw = time * (speed ) ;
		glm::mat4 transform = glm::mat4(1);
		transform = glm::translate(transform, position);
		transform = glm::rotate(transform, yaw, up);
		
		_boxMeshes[i]->setModel(transform);

	}

	amp = 5;
	speed = 0.4;
	for (i = 0; i < LIGHT_COUNT; i++)
	{
		t = (float)i / (float)(LIGHT_COUNT) * 360.0f * 3.14156f / 180.0f;
		position = {
			cos(t + time * speed) * amp ,
			sin(t + time * speed) * cos(t + time * speed) * amp ,
			sin(t + time * speed) * amp
		};
		_pointLights[i]->setPosition(position);
		_pointLightMeshes[i]->setModel(glm::translate(position));
	}
}

