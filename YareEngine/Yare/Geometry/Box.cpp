#include "Box.hpp"

namespace yare {
	namespace geometry {


		struct BoxVertex {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 uv;
		};

		std::unique_ptr<graphics::VertexArray> Box::CreateVertexArray(const glm::vec3& extent)
		{

			/*
			Extent is half the width
			  1_________ 2
			  \         |\
			  |\5_______|_\6
			 0\ |_______|3|
			   \|________\|
			    4         7

					   ^ yextent
					   |
			-xextent <----> +xextent
			
			*/
			float ex = extent.x, ey = extent.y, ez = extent.z;
			
			std::vector<BoxVertex>	vertices = {
				//face -z
				{{-ex, -ey, -ez}, {0.0f,  0.0f, -1.0f},  {0.0f, 0.0f}},
				{{ ex, -ey, -ez}, {0.0f,  0.0f, -1.0f},  {1.0f, 0.0f}},
				{{ ex,  ey, -ez}, {0.0f,  0.0f, -1.0f},  {1.0f, 1.0f}},
				
				{{ ex,  ey, -ez}, {0.0f,  0.0f, -1.0f},  {1.0f, 1.0f}},
				{{-ex,  ey, -ez}, {0.0f,  0.0f, -1.0f},  {0.0f, 1.0f}},
				{{-ex, -ey, -ez}, {0.0f,  0.0f, -1.0f},  {0.0f, 0.0f}},
				//face +z
				{{-ex, -ey,  ez}, {0.0f,  0.0f,  1.0f},  {0.0f, 0.0f}},
				{{ ex,  ey,  ez}, {0.0f,  0.0f,  1.0f},  {1.0f, 1.0f}},
				{{ ex, -ey,  ez}, {0.0f,  0.0f,  1.0f},  {1.0f, 0.0f}},
				
				{{ ex,  ey,  ez}, {0.0f,  0.0f,  1.0f},  {1.0f, 1.0f}},
				{{-ex, -ey,  ez}, {0.0f,  0.0f,  1.0f},  {0.0f, 0.0f}},
				{{-ex,  ey,  ez}, {0.0f,  0.0f,  1.0f},  {0.0f, 1.0f}},
				//face -x
				{{-ex,  ey,  ez}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},
				{{-ex, -ey, -ez}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},
				{{-ex,  ey, -ez}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}},

				{{-ex, -ey, -ez}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},
				{{-ex,  ey,  ez}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},
				{{-ex, -ey,  ez}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},

				//face +x
				{{ ex,  ey,  ez}, {1.0f,  0.0f,  0.0f},  {1.0f, 0.0f}},
				{{ ex,  ey, -ez}, {1.0f,  0.0f,  0.0f},  {1.0f, 1.0f}},
				{{ ex, -ey, -ez}, {1.0f,  0.0f,  0.0f},  {0.0f, 1.0f}},
				
				{{ ex, -ey, -ez}, {1.0f,  0.0f,  0.0f},  {0.0f, 1.0f}},
				{{ ex, -ey,  ez}, {1.0f,  0.0f,  0.0f},  {0.0f, 0.0f}},
				{{ ex,  ey,  ez}, {1.0f,  0.0f,  0.0f},  {1.0f, 0.0f}},
				//face -y
				{{-ex, -ey, -ez}, {0.0f, -1.0f,  0.0f},  {0.0f, 1.0f}},
				{{ ex, -ey,  ez}, {0.0f, -1.0f,  0.0f},  {1.0f, 0.0f}},
				{{ ex, -ey, -ez}, {0.0f, -1.0f,  0.0f},  {1.0f, 1.0f}},
				
				{{ ex, -ey,  ez}, {0.0f, -1.0f,  0.0f},  {1.0f, 0.0f}},
				{{-ex, -ey, -ez}, {0.0f, -1.0f,  0.0f},  {0.0f, 1.0f}},
				{{-ex, -ey,  ez}, {0.0f, -1.0f,  0.0f},  {0.0f, 0.0f}},
				//face +y
				{{-ex,  ey, -ez}, {0.0f,  1.0f,  0.0f},  {0.0f, 1.0f}},
				{{ ex,  ey, -ez}, {0.0f,  1.0f,  0.0f},  {1.0f, 1.0f}},
				{{ ex,  ey,  ez}, {0.0f,  1.0f,  0.0f},  {1.0f, 0.0f}},
				
				{{ ex,  ey,  ez}, {0.0f,  1.0f,  0.0f},  {1.0f, 0.0f}},
				{{-ex,  ey,  ez}, {0.0f,  1.0f,  0.0f},  {0.0f, 0.0f}},
				{{-ex,  ey, -ez}, {0.0f,  1.0f,  0.0f},  {0.0f, 1.0f}}


			};				  




			graphics::VertexArray* vertexArray = graphics::VertexArray::Create();
			vertexArray->bind();

			graphics::BufferLayout vertexLayout = {
				{graphics::BufferElementType::Float3, "position"},
				{graphics::BufferElementType::Float3, "normal"},
				{graphics::BufferElementType::Float2, "uv"},
			};
			graphics::VertexBuffer* vertexBuffer = graphics::VertexBuffer::Create(vertexLayout);
			//vertexBuffer->load(&vertices[0], sizeof(vertices));
			vertexBuffer->load(&vertices[0], vertices.size() * sizeof(vertices[0]));
			vertexArray->addVertexBuffer(vertexBuffer);

			//Not Indexed 
			vertexArray->setVertexCount(vertices.size());

			std::vector<unsigned int> indices = {
				//size +z
				0,1,2,
				2,3,0,
				//side +x
				3,2,6,
				6,7,3,
				//side -z
				7,6,5,
				5,4,7,
				//side -x
				4,5,1,
				1,0,4,
				//side +y
				1,5,6,
				6,2,1,
				//-y
				4,0,3,
				3,7,4
			};
			graphics::IndexBuffer* indexBuffer = graphics::IndexBuffer::Create();
			//indexBuffer->load(&indices[0], sizeof(indices));
			indexBuffer->load(&indices[0], indices.size() * sizeof(indices[0]));
			vertexArray->setIndexBuffer(indexBuffer);
	

			vertexArray->unbind();
			return std::unique_ptr<graphics::VertexArray>(vertexArray);
		}


	}
}