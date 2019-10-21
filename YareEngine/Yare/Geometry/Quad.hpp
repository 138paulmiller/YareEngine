#include "Geometry.hpp"
#include "../Renderer.hpp"


//Implement https://en.wikipedia.org/wiki/Geometry#Important_concepts_in_geometry

namespace yare {
	namespace geometry {
		/*
		2dquad
		*/
		class Quad
		{
		public:

			//Creates a 2D Quad
			static std::unique_ptr<graphics::VertexArray> CreateVertexArray(float x, float y, float w, float h);
			//bool intersects(const Line & line);
			//bool intersects(const Sphere & sphere);

			//
		};

	}
}