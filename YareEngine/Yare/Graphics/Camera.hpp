#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/transform.hpp>

namespace yare { namespace graphics {

	class Camera
	{
	public:
		Camera(
			const glm::vec3& up = { 0,1,0 },
			const glm::vec3& right = { 1,0,0 }
		) :_up(up), _right(right) {}

		void setPosition(const glm::vec3& position)
		{
			_position = position;
		}

		void setProjection(const glm::mat4& projection)
		{
			_projection = projection;
		}

		const glm::vec3& getPosition()const { return _position; }

		const glm::mat4& getProjection() const { return _projection; }
		const glm::mat4& getView() const { return _view; }
		void lookAt(const glm::vec3& target) { _view = glm::lookAt(_position, target, _up); }

	private:
		glm::vec3 _position;
		glm::vec3 _up;
		glm::vec3 _right;
		glm::mat4 _view;
		glm::mat4 _projection;
	};

} }