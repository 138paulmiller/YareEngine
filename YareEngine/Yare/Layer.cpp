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
		_quad.reset(0);
		_shader = 0;
		_target = 0;
	}
	
	void Layer::setQuad(const glm::vec2 & pos, const glm::vec2 & size)
	{
		_quad.reset(Quad::CreateVertexArray(pos.x, pos.y, size.x, size.y).release());
	}
	void Layer::setShader(Shader * shader)
	{
		_shader = shader;
	}

	void Layer::addInput(RenderTarget * target)
	{
		_inputs.push_back(target);
	}
	void Layer::clearInputs()
	{
		_inputs.clear();
	}

	
	void Layer::setTarget(RenderTarget * target)
	{
		_target = target;
	}
	void Layer::render(Renderer * renderer)
	{
		if(_target)
			_target->bind(RenderTargetMode::Draw);
		_shader->bind();

		int offsetTextureUnit = 0;
		for (RenderTarget * input : _inputs) 
		{
			if (input)
			{
				input->bind(RenderTargetMode::Read);
				input->bindTextures(offsetTextureUnit);
				input->unloadUniforms(_uniforms);
				offsetTextureUnit += input->getNumberOfAttachments();
			}
		}
		_uniforms.load(_shader);

		_quad->bind();

		renderer->pushState(_state);
		renderer->renderMesh(_quad.get());
		renderer->popState();

		for (RenderTarget* input : _inputs)
		{
			if (input)
			{
				input->unbind(RenderTargetMode::Read);
			}
		}

		if (_target)
			_target->unbind(RenderTargetMode::Draw);
	}
	

}