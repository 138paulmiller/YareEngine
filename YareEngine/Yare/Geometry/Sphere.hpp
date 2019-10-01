#pragma once 
#include "Geometry.hpp"
#include "../Renderer.hpp"

namespace yare {
	namespace geometry {



		class Sphere
		{
		public:
			static std::unique_ptr<graphics::VertexArray> CreateVertexArray(float radius, int xSectors, int ySectors);
			//bool intersects(const Line & line);
			//bool intersects(const Sphere & sphere);

			glm::vec3 _position;
			float  _radius;
			//
		};

	}
}