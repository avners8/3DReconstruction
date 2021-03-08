#pragma once

#include <stdio.h>
#include <string>
#include <iostream>

#include <GL\glew.h>
#include <glm/glm.hpp>

#include "utils.h"

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderMesh();
	void ClearMesh();

	GLfloat* getMeshCenter() const;
	void UpdateCameraPosition(glm::vec3 newCameraPosition);
	bool operator > (const Mesh &other);

	~Mesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
	GLfloat* center;
	glm::vec3 cameraPosition;
};

