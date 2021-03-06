#include "Noclip.hpp"

#include <glm\gtc\quaternion.hpp>
#include <SDL.h>

Noclip::Noclip(float speed, float sensitivity){
	_speed = speed;
	_sensitivity = sensitivity;
}

void Noclip::load(){
	_transform = getComponent<Transform>();
	_input = getComponent<Input>();
}

void Noclip::update(double dt){
	// Mouse movements
	int mouseRX, mouseRY;

	// Get normalized mouse movement
	SDL_GetRelativeMouseState(&mouseRX, &mouseRY);

	// Apply sensitivity to movements in seperate quaternions 
	glm::quat lookX(glm::vec3(0.f, 0.f, glm::radians(-(float)(mouseRX * _sensitivity))));
	glm::quat lookY(glm::vec3(glm::radians((float)(mouseRY * _sensitivity)), 0.f, 0.f));

	// Rotate left and right globally on z axis
	_transform->rotate(lookX);
	// Rotate up and down locally on x axis
	_transform->localRotate(lookY);


	// Key bindings and conditions
	
	/*

	W, A, S, D	=	Move forward, left, back, right

	Space, CTRL	=	Translate up, down

	Shift		=	Speed boost modifier

	*/

	// Move left locally
	if (_input->isDown("a")){
		left(dt);

		// Shift key is speed boost
		if (_input->isDown("shift"))
			left(dt);
	}

	// Move right locally
	if (_input->isDown("d")){
		right(dt);

		if (_input->isDown("shift"))
			right(dt);
	}

	// Move forward locally
	if (_input->isDown("w")){
		if (_input->isDown("space") || _input->isDown("ctrl")) // Globally if moving up or down
			globalForward(dt);
		else
			forward(dt);

		if (_input->isDown("shift")){
			if (_input->isDown("space") || _input->isDown("ctrl"))
				globalForward(dt);
			else
				forward(dt);
		}
	}

	// Move back locally
	if (_input->isDown("s")){
		if (_input->isDown("space") || _input->isDown("ctrl"))
			globalBack(dt);
		else
			back(dt);

		if (_input->isDown("shift")){
			if (_input->isDown("space") || _input->isDown("ctrl"))
				globalBack(dt);
			else
				back(dt);
		}
	}

	// Move up globally
	if (_input->isDown("space")){
		globalUp(dt);

		if (_input->isDown("shift"))
			globalUp(dt);
	}

	// Move down globally
	if (_input->isDown("ctrl")){
		globalDown(dt);

		if (_input->isDown("shift"))
			globalDown(dt);
	}
}

void Noclip::left(double dt){
	_transform->localTranslate(glm::vec3(-(float)(_speed * dt), 0, 0));
}

void Noclip::right(double dt){
	_transform->localTranslate(glm::vec3((float)(_speed * dt), 0, 0));
}

void Noclip::forward(double dt){
	_transform->localTranslate(glm::vec3(0, (float)(_speed * dt), 0));
}

void Noclip::back(double dt){
	_transform->localTranslate(glm::vec3(0, -(float)(_speed * dt), 0));
}

void Noclip::globalForward(double dt){
	float z = glm::eulerAngles(_transform->rotation()).z;

	_transform->translate(glm::vec3(0, (float)(_speed * dt), 0) * glm::inverse(glm::quat(glm::vec3(0, 0, z))));
}

void Noclip::globalBack(double dt){
	float z = glm::eulerAngles(_transform->rotation()).z;

	_transform->translate(glm::vec3(0, -(float)(_speed * dt), 0) * glm::inverse(glm::quat(glm::vec3(0, 0, z))));
}

void Noclip::globalUp(double dt){
	_transform->translate(glm::vec3(0, 0, -(float)(_speed * dt)));
}

void Noclip::globalDown(double dt){
	_transform->translate(glm::vec3(0, 0, (float)(_speed * dt)));
}

void Noclip::setSpeed(float speed){
	_speed = speed;
}

void Noclip::setSensitivity(float sensitivity){
	_sensitivity = sensitivity;
}