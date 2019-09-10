#include "Renderer.hpp"

YARE_GRAPHICS_MODULE_BEG

void Renderer::submit(const RenderCommand & command)
{
	_commandQueue.push(command);
}

void Renderer::present()
{
	//TODO OPTIMIZE!! - cull, sort, ubo, shader management
	while (!_commandQueue.empty())
	{
		const RenderCommand  & command = _commandQueue.front();
		_commandQueue.pop();
		
		command.shader->bind();
		//use UBOs and render views for this
		command.shader->setUniform("view", _camera->getView());
		command.shader->setUniform("projection", _camera->getProjection());
		
		Texture * texture = command.textures[0];
		int i = 0;
		while (texture)
		{
			texture->bind(i);
			i++;
		}
		command.uniformBuffer.load(command.shader);

		command.vertexArray->bind();
		switch (command.mode)
		{
		case RenderMode::IndexedMesh:
			renderIndexedMesh(command.vertexArray);
		break;
		}
		command.vertexArray->unbind();

		command.shader->unbind();
	}
}

void Renderer::beginScene(const Camera * camera)
{
	_camera = camera;
}

void Renderer::endScene()
{

	_camera = 0;
}

void Renderer::pushState(RenderState & state)
{
	updateState(state);
	_stateStack.push(state);
}

void Renderer::popState()
{
	RenderState & state = _stateStack.top();
	updateState(state);
	_stateStack.pop();
}


YARE_GRAPHICS_MODULE_END