#include "Quad.hpp"

namespace yare {
	namespace geometry {


		struct QuadVertex {
			glm::vec3 position;
			glm::vec2 uv;
		};

		std::unique_ptr<graphics::VertexArray> Quad::CreateVertexArray(float x, float y, float w, float h)
		{

			std::vector<QuadVertex>	vertices = {
				//face -z
				{{x, -y,  0.0f},        {0.0f, 0.0f}},
				{{x + w, y,  0.0f},     {1.0f, 0.0f}},
				{{x+w,  y+h,  0.0f},    {1.0f, 1.0f}},
									   
				{{x + w,  y+h,  0.0f},   {1.0f, 1.0f}},
				{{x,  y+h,  0.0f},      {0.0f, 1.0f}},
				{{x, y,  0.0f},         {0.0f, 0.0f}},

			};

			graphics::VertexArray* vertexArray = graphics::VertexArray::Create();
			vertexArray->bind();

			graphics::BufferLayout vertexLayout =
			{
				{graphics::BufferElementType::Float3, "position"},
				{graphics::BufferElementType::Float2, "uv"},
			};
			graphics::VertexBuffer* vertexBuffer = graphics::VertexBuffer::Create(vertexLayout);
			vertexBuffer->load(&vertices[0], vertices.size() * sizeof(vertices[0]));
			vertexArray->addVertexBuffer(vertexBuffer);
			vertexArray->setVertexCount(vertices.size());

			vertexArray->unbind();

			return std::unique_ptr<graphics::VertexArray>(vertexArray);
		}


	}
}