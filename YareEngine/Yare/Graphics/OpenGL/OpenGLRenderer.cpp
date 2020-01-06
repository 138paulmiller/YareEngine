#pragma once
#include "OpenGLRenderer.hpp"
#include "OpenGLError.hpp"
#include <GL/glew.h>

namespace yare { namespace graphics {  

void OpenGLRenderer::renderMesh(const VertexArray* vertexArray)
{

	glDrawArrays(_primitive, 0, vertexArray->getVertexCount());
	OpenGLCheckError();

}

void OpenGLRenderer::resizeViewport(int width, int height)
{
	Renderer::resizeViewport(width, height);
	glViewport(0, 0, width, height);
	OpenGLCheckError();

}

void OpenGLRenderer::renderIndexedMesh(const VertexArray * vertexArray)
{
	glDrawElements(_primitive,         // mode
		vertexArray->getIndexBuffer()->getIndexCount(),  // count
		GL_UNSIGNED_INT,      // type
		NULL                  // element array buffer offset
	);
	OpenGLCheckError();

}

void OpenGLRenderer::updateState(const RenderState & state)
{
	OpenGLCheckError();

	glDepthMask(GL_TRUE);
	OpenGLCheckError();
	glEnable(GL_DEPTH_TEST);
	OpenGLCheckError();

	switch (state.depthFunc)
	{
	case  RenderTestFunc::Less:
		glDepthFunc(GL_LESS);
		break;
	case  RenderTestFunc::LessEqual:
		glDepthFunc(GL_LEQUAL);
		break;
	case  RenderTestFunc::Greater:
		glDepthFunc(GL_GREATER);
		break;
	case  RenderTestFunc::GreaterEqual:
		glDepthFunc(GL_GEQUAL);
		break;
	case  RenderTestFunc::Always:
		glDepthFunc(GL_ALWAYS);
		break;
	case  RenderTestFunc::Never:
		glDepthFunc(GL_NEVER);
		break;
	case  RenderTestFunc::Equal:
		glDepthFunc(GL_EQUAL);
		break;
	case  RenderTestFunc::NotEqual:
		glDepthFunc(GL_NOTEQUAL);
		break;
	case RenderTestFunc::Disabled:
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		break;
	}
	OpenGLCheckError();

	glColorMask(
		(char)( state.colorMask & RenderColorMask::R ), 
		(char)( state.colorMask & RenderColorMask::B ),
		(char)( state.colorMask & RenderColorMask::B ),
		(char)( state.colorMask & RenderColorMask::A )
		);
	OpenGLCheckError();

	glEnable(GL_CULL_FACE);
	switch (state.cullFace)
	{
	case RenderCullFace::Back:
		glCullFace(GL_BACK);
		break;
	case RenderCullFace::Front:
		glCullFace(GL_FRONT);
		break;
	}
	OpenGLCheckError();


	switch (state.winding)
	{
	case RenderWinding::Clockwise:
		glFrontFace(GL_CW);
		break;
	case RenderWinding::CounterClockwise:
		glFrontFace(GL_CCW);
		break;
	}
	OpenGLCheckError();

	switch (state.primitive)
	{
	case RenderPrimitive::Triangles:
		_primitive = GL_TRIANGLES;
		break;
	}

	//glStencilFunc(RenderTestFuncToGLFunc(state.stencilFunc), ref, mask);

	if(state.wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}
	OpenGLCheckError();
}

void OpenGLRenderer::clear(RenderBufferFlag mask) 
{
	int glmask = 0;
	if(  (int)(mask & RenderBufferFlag::Color) )
	{
		glmask |= GL_COLOR_BUFFER_BIT;
	}
	if ((int)(mask & RenderBufferFlag::Depth))
	{
		glmask |= GL_DEPTH_BUFFER_BIT;
	}
	glClear(glmask);
}

} } 