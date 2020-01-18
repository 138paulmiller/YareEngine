#pragma once
#include <stack>
#include <queue>

#include "Error.hpp"
#include "Shader.hpp"
#include "RenderTarget.hpp"
#include "VertexArray.hpp"
#include "UniformBlock.hpp"

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
	enum class RenderColorMask : char {
		None  = 0b0000,
		R     = 0b0001,
		G     = 0b0010,
		B     = 0b0100,
		A     = 0b1000,
		RG    = 0b0011,
		RGB   = 0b0111,
		RGBA  = 0b1111,
	};

	RenderColorMask operator|(RenderColorMask l, RenderColorMask r);
	RenderColorMask operator&(RenderColorMask l, RenderColorMask r);

	enum class RenderMode {
		Mesh=0, //NonIndexed Mesh - Using VertexArray (No Index Buffer Is Set. VertexCount is required however)
		IndexedMesh 
	};
	//Lighting type
	enum class RenderLighting {
		Unlit = 0, Surface, 
		//Translucent, UI
	};

	//overload == to only update if changed 
	struct RenderState
	{
		RenderCullFace cullFace = RenderCullFace::Back;
		RenderWinding  winding = RenderWinding::Clockwise;
		RenderTestFunc depthFunc = RenderTestFunc::Less;
		RenderColorMask colorMask = RenderColorMask::RGBA;
		RenderTestFunc stencilFunc = RenderTestFunc::Less;
		RenderPrimitive primitive = RenderPrimitive::Triangles;

		bool wireframe = 0;
	};


	//Should order render data.
	struct RenderCommand
	{
		//Viewport
		graphics::UniformBlock uniforms;

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
