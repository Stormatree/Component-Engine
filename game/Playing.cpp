#include "Playing.hpp"

#include <Component\Camera.hpp>
#include <Component\Light.hpp>
#include <Component\Model.hpp>
#include <Component\Camera.hpp>
#include <Component\Follow.hpp>

#include "Input.hpp"
#include "Velocity.hpp"

void Playing::on(){
	Entity* sky = EntityManager::createEntity("sky");
	sky->getComponent<Transform>()->setScale(glm::vec3(500, 500, 500));
	sky->addComponent(new Model("relaxed_cube.obj", "grid.png"));

	Entity* player = EntityManager::createEntity("player");
	player->getComponent<Transform>()->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
	player->addComponent(new Velocity(0.01f));
	player->addComponent(new Input(200.f, 0.075f));
	player->addComponent(new Model("ship.obj", "ship.png"));
	
	Entity* camera = EntityManager::createEntity("camera");
	camera->getComponent<Transform>()->setPosition(glm::vec3(0.f, -10.f, -50.f));
	camera->addComponent(new Follow(player->ID(), 15.f, glm::vec3(0.f, -1.f, -5.f)));
	camera->addComponent(new Camera);
	camera->addComponent(new Light);
		

	EntityManager::invokeAll(&Component::load);
	EntityManager::invokeAll(&Component::lateLoad);
}

void Playing::off(){
	EntityManager::destroyAll();
	EntityManager::deleteDestroyed();
}