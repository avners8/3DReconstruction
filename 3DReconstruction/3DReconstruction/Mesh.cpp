#include "Mesh.h"

Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
	center = 0;
}

void Mesh::CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	indexCount = numOfIndices;

	center = new GLfloat[3];
	for (int i = 0; i < 3; i++) {
		center[i] = (vertices[9 * i] + vertices[9 * i + 1] + vertices[9 * i + 2]) / 3;
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 6, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 6, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 9, (void*)(sizeof(vertices[0]) * 6));
	//glEnableVertexAttribArray(2); 
	checkGLError();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Mesh::RenderMesh()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

GLfloat* Mesh::getMeshCenter() const {
	return center;
}

void Mesh::UpdateCameraPosition(glm::vec3 newCameraPosition) {
	cameraPosition = newCameraPosition;
}

bool Mesh::operator > (const Mesh &other) {
	return (pow(abs(center[0] - cameraPosition.x), 2) + pow(abs(center[1] - cameraPosition.y), 2) + pow(abs(center[2] - cameraPosition.z), 2)) >
		(pow(abs(other.getMeshCenter()[0] - cameraPosition.x), 2) + pow(abs(other.getMeshCenter()[1] - cameraPosition.y), 2) + pow(abs(other.getMeshCenter()[2] - cameraPosition.z), 2));
}

void Mesh::ClearMesh()
{
	if (IBO != 0) {
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0) {
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;

	if (center) {
		delete[] center;
	}
}

Mesh::~Mesh()
{
	ClearMesh();
}
