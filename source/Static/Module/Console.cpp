#include "Console.hpp"

#include <iostream>
#include <Static\EntityManager.hpp>
#include <Static\Renderer.hpp>
#include <Static\AssetLoader.hpp>

// Currently this is a regex mess, but will eventually be replaced with Lua

void setEntityPosition(const std::string& entityName, const glm::vec3& position){
	Entity* entity = EntityManager::getEntity(entityName);

	if (entity){
		entity->getComponent<Transform>()->setPosition(position);
		return;
	}
}

Console::Console(){
	_patternMap["null"] = std::regex("^ *$", std::regex_constants::basic);
	_patternMap["exit"] = std::regex("^(?:exit|quit|close)$", std::regex_constants::icase);
	_patternMap["help"] = std::regex("^help *(\\S*) *$", std::regex_constants::icase | std::regex_constants::ECMAScript);
	_patternMap["hello"] = std::regex("^(?:hello|hey|hi)(?: +.*$|$)", std::regex_constants::icase);
	_patternMap["you"] = std::regex("^(?:you|yourself)$", std::regex_constants::icase);
	_patternMap["position"] = std::regex("^(?:position|pos|move) +(\\S+) +([-+]?(?:[0-9]*\\.[0-9]+|[0-9]+)) +([-+]?(?:[0-9]*\\.[0-9]+|[0-9]+)) +([-+]?(?:[0-9]*\\.[0-9]+|[0-9]+))$", std::regex_constants::icase | std::regex_constants::ECMAScript);
}

void Console::setPrefix(const std::string& prefix){
	_prefix = prefix;
}

std::string Console::getInput(){
	if (_prefix != "")
		std::cout << _prefix << " ";

	std::string input;

	std::getline(std::cin, input);

	return input;
}

int Console::interpretInput(const std::string& input){
	std::smatch results;

	if (std::regex_match(input, results, _patternMap["null"])){
		return NULL_CODE;
	}
	else if (std::regex_match(input, results, _patternMap["exit"])){
		return EXIT_CODE;
	}
	else if (std::regex_match(input, results, _patternMap["help"])){
		std::smatch subResults;

		if (std::regex_match(results[1].str(), subResults, _patternMap["you"])){
			std::cout << "I don't want to do that." << std::endl;
		}
		else if (results[1].str() != ""){
			std::cout << "I can't help " << results[1].str() << "." << std::endl;
		}
		else{
			std::cout << "Help who?" << std::endl;
		}

		return VALID_CODE;
	}
	else if (std::regex_match(input, results, _patternMap["position"])){
		setEntityPosition(results[1].str(), glm::vec3(std::stof(results[2].str()), std::stof(results[3].str()), std::stof(results[4].str())));
		return VALID_CODE;
	}
	else if (std::regex_match(input, results, _patternMap["hello"])){
		std::cout << "Hello." << std::endl;
		return VALID_CODE;
	}

	std::cout << "I don't understand." << std::endl;
	return INVALID_CODE;
}