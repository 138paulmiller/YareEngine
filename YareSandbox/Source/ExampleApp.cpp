
#include "ExampleApp.hpp"
#include <Yare/Geometry/Box.hpp>
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

PhongMesh::PhongMesh()
{
	//Default render data config
	RenderData& data = Mesh::renderData;
	data.shader = AssetManager::GetInstance().get<Shader>("Shader_Phong");
	data.mode = RenderMode::IndexedMesh;
	data.lighting = RenderLighting::Phong;
	data.primitive = RenderPrimitive::Triangles;
	data.state.cullFace = RenderCullFace::Back;
	data.state.winding = RenderWinding::Clockwise;
	data.state.depthFunc = RenderTestFunc::Less;
	data.state.stencilFunc = RenderTestFunc::Less;
}

void PhongMesh::preRender()
{
	
	Mesh::preRender();
}

void PhongMesh::postRender()
{
	Mesh::postRender();
}




ExampleApp::ExampleApp() 
{
}


void ExampleApp::onEnter()
{
	//Load default engine assets
	AssetManager::GetInstance().loadEngineContent();


	//Create an instance of the phong material. To be shared among meshes

	Texture* diffuse = AssetManager::GetInstance().get<Texture>("Image_Container_Diffuse");
	Texture * specular = AssetManager::GetInstance().get<Texture>("Image_Container_Specular");

	diffuse->update(TextureWrap::Repeat);
	specular->update(TextureWrap::Repeat);

	_phongMaterial.reset(new PhongMaterial());
	_phongMaterial->setDiffuseTexture(diffuse);
	_phongMaterial->setSpecularTexture(specular);
	_phongMaterial->setShininess(2);

	//Create an instance of the flat material. To be shared among meshes
	_flatMaterial.reset(new FlatMaterial());
	_flatMaterial->setBase(glm::vec3(1, 0, 0));

	_phongMeshes.resize(BOX_COUNT);
	for (int i = 0; i < BOX_COUNT; i++)
	{ //phong mesh
		_phongMeshes[i].reset(new PhongMesh());
		_phongMeshes[i]->loadVertexArray(geometry::Box::CreateVertexArray({ 1,1,1 }));
		_phongMeshes[i]->setMaterial(_phongMaterial.get());
	}

	_pointLights.resize(LIGHT_COUNT);
	_pointLightMeshes.resize(LIGHT_COUNT);
	for (int i = 0; i < LIGHT_COUNT; i++)
	{	
		//Create the light and the mesh
		_pointLights[i].reset(new PointLight());
		_pointLights[i]->setAmbient(glm::vec3(0.05f, 0.05f, 0.05f));
		_pointLights[i]->setDiffuse(glm::vec3(0.54f, 0.52f, 0.52f));
		_pointLights[i]->setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
		_pointLights[i]->setAttenuation(glm::vec3(1, 0.5, 0.00004 ));
		
		_pointLightMeshes[i].reset(new Mesh());
		_pointLightMeshes[i]->loadVertexArray(geometry::Box::CreateVertexArray({ 0.15,0.15,0.15 }));
		_pointLightMeshes[i]->setMaterial(_flatMaterial.get());
		RenderData& data = _pointLightMeshes[i]->renderData;
		data.shader = AssetManager::GetInstance().get<Shader>("Shader_Flat");
		data.lighting = RenderLighting::Flat;
	}
	//Set  up Sky box
	_skybox.reset(new SkyBox());
	Texture* skyboxTexture = AssetManager::GetInstance().get<Cubemap>("Image_SkyBox");
	_skybox->setCubemap(skyboxTexture);


	//Set up the scene
	_scene.setCamera(&_camera);

	_scene.add("Skybox", _skybox.get());

	int index = 0;
	for (const std::unique_ptr<PointLight> & pointLight : _pointLights)
	{
		_scene.getLights().setPointLight("PointLight_" + std::to_string(index), pointLight.get());
		index++;
	}
	index = 0;
	for (const std::unique_ptr<PhongMesh> & phongMesh : _phongMeshes)
	{
		_scene.add("PhongMesh_" + std::to_string(index), phongMesh.get());
		index++;
	}

	index = 0;
	for (const std::unique_ptr<Mesh> & mesh  : _pointLightMeshes)
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
	float speed = 0.15;

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
	_camera.setPosition({ 0, 0, 30 });
	_camera.setForward({0,0,-1});
	// clear
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	////////// Update State ////////////////
 	 _model = glm::translate(_camera.getPosition());
	 _model *= glm::scale(glm::vec3( 100, 100, 100 ));
	_skybox->setModel(_model);
	

	glm::vec3 position;
	int i = 0;
	float t;
	float p= 4; // periodicity 
	float amp = 20; 
	for (i = 0; i < BOX_COUNT; i++)
	{
		t = i / (float)BOX_COUNT * 360 * 3.14156 / 180;

		position = { 
			cos(t + time * speed) * sin(t*p + time * speed) * amp ,
			sin(t + time * speed) * cos(t*p + time * speed) * amp ,
			cos(t + time * speed) * amp
		};
		_phongMeshes[i]->setModel(glm::translate(position));
		
	}

	p = 4; // periodicity 
	amp = 25;
	for (i = 0; i < LIGHT_COUNT; i++)
	{
		t =  i/(float)LIGHT_COUNT * 360 * 3.14156 / 180;
		position = { 
			cos(t + time * speed  ) * sin(t*p + time * speed  ) * amp ,
			sin(t + time * speed  ) * cos(t*p + time * speed  ) * amp ,
			cos(t + time * speed  ) * amp
		};
		_pointLights[i]->setPosition(position);
		_pointLightMeshes[i]->setModel(glm::translate(position));
	}


	//Submit Scene to be drawn - TODO - SceneRenderer will manage /sort/ cull this process of drawing
	_scene.render(App::getRenderer());
} 