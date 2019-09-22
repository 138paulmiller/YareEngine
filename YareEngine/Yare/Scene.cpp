#include "Scene.hpp"
#include "Renderer.hpp"
namespace yare
{
	using namespace graphics;

	LightBlock& Scene::getLights()
	{
		return _lights;
	}

	void Scene::setCamera(const Camera* camera)
	{
		_camera = camera;
	}
	const Camera* Scene::getCamera() const
	{
		return _camera;
	}
	
	void Scene::loadUniforms(UniformBlock& uniforms, RenderLighting lighting) const
	{
		//use UBOs and render views for this
		uniforms.setUniform("view", _camera->getView());
		uniforms.setUniform("projection", _camera->getProjection());
		switch (lighting)
		{
		case RenderLighting::Unlit:
			break;
		case RenderLighting::Phong:
			//load lighting for phong shading
			_lights.loadUniforms(uniforms);
			uniforms.setUniform("view_pos", _camera->getPosition());

			break;
		}

	}

	void Scene::add(const std::string& name, Renderable* renderable)
	{
		_renderables[name] = renderable;
	}

	void Scene::remove(const std::string& name)
	{
		_renderables.erase(name);
	}


	void Scene::render(Renderer* renderer)
	{
		renderer->beginScene(this);
		for (const std::pair<std::string, Renderable* >& pair : _renderables)
		{
			renderer->submit(pair.second);
		}
		renderer->endScene();
	}
}
