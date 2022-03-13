#ifndef HELPER_H
#define HELPER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "light.h"

#include <iostream>

// screen settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera attached to drone
camera cam(true, glm::vec3(0.0f, 0.0f, 1.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// fixed camera
camera fixcam(false, glm::vec3(0.0f, 1.5f, 8.0f), glm::vec3(5.0f, 5.0f, 0.0f), -90.0f, 0.0f, -45.0f);

// timing for smooth calculation
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// For drawing parts of drone in symmetric way
void getABC(int type, int& a, int& b, int& c) {
	if (type == 1) {
		a = 1;
		b = 1;
		c = -1;
	}
	else if(type == 2)
	{
		a = -1;
		b = -1;
		c = -1;
	}
	else if (type == 3) {
		a = -1;
		b = 1;
		c = 1;
	}
	else if (type == 4) {
		a = 1;
		b = -1;
		c = 1;
	}
}

// Draw light present in the scene using lightshader
void drawLights(unsigned int &lightVAO, Shader& lightShader, glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
	lightShader.use();

	lightShader.setMat4("model", model);
	lightShader.setMat4("projection", projection);
	lightShader.setMat4("view", view);

	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

// Draw main rectangular base box of the drone
void drawMainBody(Shader& shader, glm::mat4 model, glm::mat4 projection, glm::mat4 view, Model& model_, light &onlyLight) {
	shader.use();

	shader.setMat4("model", model);
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);

	model_.Draw(shader, onlyLight);
}

// Draw different parts of the drone
void drawParts(Shader& shader, glm::mat4 model_b, glm::mat4 projection, glm::mat4 view, Model model2
	, Model model3, Model model4, int type, light &onlyLight) 
{
	shader.use();
	int a, b, c;

	// Use a, b, c to draw the parts in the drone
	getABC(type, a, b, c);

	// Rendering cylindrical pipe of the drone
	glm::mat4 model2T = glm::mat4(1.0f);
	model_b = glm::translate(model_b, glm::vec3(a * 1.25f, 0.0f, b * 0.7f));
	model2T = model_b;
	model_b = glm::rotate(model_b, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model_b = glm::rotate(model_b, glm::radians(c * 45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model_b = glm::scale(model_b, glm::vec3(0.1f, 0.5f, 0.1f));
	shader.setMat4("model", model_b);
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	model2.Draw(shader, onlyLight);

	// Rendering ring in which the fan will be rotating
	glm::mat4 model3T = glm::mat4(1.0f);
	model2T = glm::scale(model2T, glm::vec3(2.0f, 1.0f, 0.8f));
	model2T = glm::translate(model2T, glm::vec3(a * 0.15f, 0.12f, b * 0.4f));
	model3T = model2T;
	model2T = glm::scale(model2T, glm::vec3(0.1f, 0.1f, 0.1f));
	shader.setMat4("model", model2T);
	model3.Draw(shader, onlyLight);

	// Rendering the base on which fan will be mounted on
	model2T = glm::scale(model2T, glm::vec3(0.2f, 0.4f, 0.2f));
	shader.setMat4("model", model2T);
	model2.Draw(shader, onlyLight);

	// Rendering the fan
	float angle = static_cast<float>(glfwGetTime()) * cam.rotation[type - 1];
	model3T = glm::rotate(model3T, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	model3T = glm::scale(model3T, glm::vec3(0.01f, 0.01f, 0.01f));
	shader.setMat4("model", model3T);
	model4.Draw(shader, onlyLight);

}

#endif // !HELPER_H
