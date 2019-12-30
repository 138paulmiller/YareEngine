#pragma once
#include <stack>
#include <queue>

#include "Graphics/Error.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/RenderTarget.hpp"
#include "Graphics/VertexArray.hpp"
#include "Graphics/TextureBlock.hpp"
#include "Graphics/UniformBlock.hpp"

namespace yare {
	// End Api


	//Test functions to stencil, depth, test etc....
	enum class RenderTestFunc
	{
		Less = 0,
		LessEqual,
		Greater,
		GreaterEqual,
		Equal,
		NotEqual,
		Never, //Always fails.
		Always,//Always passes
		Disabled //Will disable before draw
	};

	enum class RenderWinding {
		Clockwise = 0, CounterClockwise
	};

	enum class RenderCullFace {
		Back = 0, Front
	};

	enum class RenderPrimitive {
		Triangles = 0
	};


	enum class RenderMode {
		Mesh=0, //NonIndexed MEsh - Using VertexArray (No Index Buffer Is Set. VertexCount is required however)
		IndexedMesh 
	};

	enum class RenderLighting {
		Unlit = 0, Phong, 
	};

	//overload == to only update if changed 
	struct RenderState
	{
		RenderCullFace cullFace = RenderCullFace::Back;
		RenderWinding  winding = RenderWinding::Clockwise;
		RenderTestFunc depthFunc = RenderTestFunc::Less;
		RenderTestFunc stencilFunc = RenderTestFunc::Less;
		RenderPrimitive primitive = RenderPrimitive::Triangles;

		bool wireframe = 0;
	};


	//Should order render data.
	struct RenderCommand
	{
		//Viewport
		graphics::UniformBlock uniforms;
		graphics::TextureBlock textures;

		graphics::VertexArray * vertexArray = 0;
		graphics::Shader * shader = 0;

		RenderMode mode = RenderMode::IndexedMesh;
		RenderLighting lighting = RenderLighting::Unlit;
		RenderState state;
	};

	
	struct Renderable
	{
		virtual ~Renderable() = default;
		virtual void onBind() = 0;
		RenderCommand command;
	};

}
