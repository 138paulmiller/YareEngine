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
	

	void Scene::add(const std::string& name, Renderable* renderable)
	{
		_renderables[name] = renderable;
	}

	void Scene::remove(const std::string& name)
	{
		_renderables.erase(name);
	}

	void Scene::unloadUniforms(UniformBlock& uniforms) const
	{
		_lights.unloadUniforms(uniforms);
		_camera->unloadUniforms(uniforms);
	}
	void Scene::render(Renderer* renderer)
	{
		renderer->begin(this);
		for (const std::pair<std::string, Renderable* >& pair : _renderables)
		{
			renderer->submit(pair.second);
		}
		//present the render data
		renderer->render();
		renderer->end();
	}
}
