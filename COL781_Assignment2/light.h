#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

// Light object
class light
{
public:

	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	light(glm::vec3 Position, glm::vec3 Ambient, glm::vec3 Specular, glm::vec3 Diffuse, float Shininess) {
		position = Position;
		ambient = Ambient;
		diffuse = Diffuse;
		specular = Specular;
		shininess = Shininess;
	}

};


#endif // !LIGHT_H
