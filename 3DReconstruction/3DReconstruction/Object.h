#pragma once

#include <vector>
#include <algorithm>

#include <GL\glew.h>
#include <glm\glm.hpp>

#include "Mesh.h"
#include "Camera.h"
#include "Texture.h"
#include "Material.h"

class Object
{
public:
	Object();
	Object(const int xDim, const int yDim, const int zDim, const int numImages, const int zDimTex, const int zScale, const int samplingStep,
		const char* fileLoc, GLfloat specularIntensity, GLfloat shininess);

	void RenderObject(Camera camera, GLuint uniformSpecularIntensity, GLuint uniformShininess, bool sortRequired);

	~Object();

private:
	std::vector<Mesh*> meshList;
	const char* fileLocation;
	Texture texture;
	Material material;
};

