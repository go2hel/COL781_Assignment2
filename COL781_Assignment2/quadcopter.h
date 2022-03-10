#ifndef QUADCOPTER_H
#define QUADCOPTER_H

#include "camera.h"
#include "light.h"
#include "model.h"
#include <string>
#include <queue>
using namespace std;

class quadParts {
public:
	model *_model;
	shader *_shader;

	quadParts(string path, string vertexShaderPath, string fragmentShaderPath) {
		_model = new model(path);
		_shader = new shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
	}

	void draw(light& _light) {
		_model->Draw(*_shader, _light);
	}
};

class quadcopter
{
public:
	
	camera quadCamera;
	vector<vector<quadParts>> drone;

	quadcopter(string directory, string vertexShaderPath, string fragmentShaderPath) {
		if (!endsWith(directory, '/')) {
			directory += '/';
		}
		loadDrone(directory, vertexShaderPath, fragmentShaderPath);
		quadCamera = camera(true, glm::vec3(0.0f, 0.0f, 0.0f));
	}

	void draw(light& _light, glm::mat4 projection, glm::mat4 view) {

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, quadCamera.Position);
		drone[0][0]._shader->use();
		drone[0][0]._shader->setMat4("model", model);
		drone[0][0]._shader->setMat4("projection", projection);
		drone[0][0]._shader->setMat4("view", view);
		drone[0][0].draw(_light);
	}

private:

	bool endsWith(string& s, char c) {
		return (s[s.size() - 1] == c);
	}

	void loadDrone(string directory, string vertexShaderPath, string fragmentShaderPath) {
		
		string path = directory + "model1.obj";
		vector<quadParts> v;

		v.push_back(quadParts(path, vertexShaderPath, fragmentShaderPath));

		drone.push_back(v);
		v.clear();

		path = directory + "model2.obj";
		for (int i = 0; i < 4; i++)
		{
			v.push_back(quadParts(path, vertexShaderPath, fragmentShaderPath));
		}
		
		drone.push_back(v);
		v.clear();

		path = directory + "model3.obj";
		for (int i = 0; i < 4; i++)
		{
			v.push_back(quadParts(path, vertexShaderPath, fragmentShaderPath));
		}

		drone.push_back(v);
		v.clear();

		path = directory + "model4.obj";
		for (int i = 0; i < 4; i++)
		{
			v.push_back(quadParts(path, vertexShaderPath, fragmentShaderPath));
		}

		drone.push_back(v);
		v.clear();
	}
};

#endif // !QUADCOPTER_H
