#pragma once
#include <stack>
#include <queue>

#include "Graphics/Error.hpp"
#include "Graphics/Shader.hpp"
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
		Always //Always passes
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
		IndexedMesh = 0
	};

	enum class RenderLighting {
		Unlit = 0, Flat, Phong, 
	};

	//overload == to only update if changed 
	struct RenderState
	{
		RenderCullFace cullFace = RenderCullFace::Back;
		RenderWinding  winding = RenderWinding::Clockwise;
		RenderTestFunc depthFunc = RenderTestFunc::Less;
		RenderTestFunc stencilFunc = RenderTestFunc::Less;
		bool wireframe = 0;
	};


	//Should order render data.
	struct RenderData
	{
		//Render Targets 
		//Viewport
		graphics::UniformBlock uniforms;
		graphics::TextureBlock textures;

		graphics::VertexArray * vertexArray = 0;
		graphics::Shader * shader = 0;

		RenderMode mode = RenderMode::IndexedMesh;
		RenderPrimitive primitive = RenderPrimitive::Triangles;
		RenderLighting lighting = RenderLighting::Unlit;
		RenderState state;
	};

	
	struct Renderable
	{
		virtual ~Renderable() = default;
		virtual void preRender() = 0;
		virtual void postRender() = 0;
		RenderData renderData;
	};

}
