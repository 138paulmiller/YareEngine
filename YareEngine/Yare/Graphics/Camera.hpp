#pragma once
#include "Graphics.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/transform.hpp>
YARE_GRAPHICS_MODULE_BEG

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
	const glm::vec3& getPosition() { return _position; }

	const glm::mat4& getView() { return _view; }
	void lookAt(const glm::vec3& target) { _view = glm::lookAt(_position, target, _up); }

private:
	glm::vec3 _position;
	glm::vec3 _up;
	glm::vec3 _right;
	glm::mat4 _view;
	//Have camera handle projection as well
};
YARE_GRAPHICS_MODULE_END