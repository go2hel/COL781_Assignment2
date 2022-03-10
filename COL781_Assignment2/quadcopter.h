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
	}

	void draw(light &_light, glm::vec3 projection, glm::vec3 view) {
		
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
