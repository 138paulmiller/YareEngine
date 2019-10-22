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

void OpenGLRenderer::renderIndexedMesh(const VertexArray * vertexArray)
{
	glDrawElements(_primitive,         // mode
		vertexArray->getIndexBuffer()->getIndexCount(),  // count
		GL_UNSIGNED_INT,      // type
		NULL                  // element array buffer offset
	);
}

unsigned int RenderTestFuncToGLFunc(RenderTestFunc func)
{
	switch (func)
	{
	case  RenderTestFunc::Less:
		return GL_LESS;
	case  RenderTestFunc::LessEqual:
		return GL_LEQUAL;
	case  RenderTestFunc::Greater:
		return GL_GREATER;
	case  RenderTestFunc::GreaterEqual:
		return GL_GEQUAL;
	case  RenderTestFunc::Always:
		return GL_ALWAYS;
	case  RenderTestFunc::Never:
		return GL_NEVER;
	case  RenderTestFunc::Equal:
		return GL_EQUAL;
	case  RenderTestFunc::NotEqual:
		return GL_NOTEQUAL;

	}
}


void OpenGLRenderer::updateState(const RenderState & state)
{
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
	switch (state.winding)
	{
	case RenderWinding::Clockwise:
		glFrontFace(GL_CW);
		break;
	case RenderWinding::CounterClockwise:
		glFrontFace(GL_CCW);
		break;
	}

	switch (state.primitive)
	{
	case RenderPrimitive::Triangles:
		_primitive = GL_TRIANGLES;
		break;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(RenderTestFuncToGLFunc(state.depthFunc));
	//glStencilFunc(RenderTestFuncToGLFunc(state.stencilFunc), ref, mask);

	if(state.wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}

}

} } 