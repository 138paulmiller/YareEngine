#include "Layer.hpp"
#include "Renderer.hpp"

#include "Geometry/Quad.hpp"
namespace yare
{
	using namespace graphics;
	using namespace geometry;


	
	Layer::Layer()
	{
		_quad.reset(geometry::Quad::CreateVertexArray(0, 0, 1, 1).release());
		_target = 0;
		_shader = 0;
		/*
		Each layer is a 2d pass
		*/
	}
	Layer::~Layer()
	{
	}
	
	void Layer::setQuad(const glm::vec2 & pos, const glm::vec2 & size)
	{

		_quad.reset(Quad::CreateVertexArray(pos.x, pos.y, size.x, size.y).release());
	}
	void Layer::setShader(Shader * shader)
	{
		_shader = shader;
	}
	
	void Layer::setTarget(RenderTarget * target)
	{
		_target = target;
	}
	void Layer::render(Renderer * renderer)
	{
		if (_target)
		{
			_target->bind();
		}
		_shader->bind();
		renderer->renderMesh(_quad.get());
	}
	

}