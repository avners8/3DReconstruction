#include "Object.h"

#define	COMPUTE_TRIANGLE_VERTICES																																																																																					\
		if (indices[0] < (xDim * yDim * zDim / (pow(samplingStep, 2))) && indices[1] < (xDim * yDim * zDim / (pow(samplingStep, 2))) && indices[2] < (xDim * yDim * zDim / (pow(samplingStep, 2)))) {																																												\
				triVertices[0]  = vertices[fixedOffset2 * indices[0]]; triVertices[1]  = vertices[fixedOffset2 * indices[0] + 1]; triVertices[2]  = vertices[fixedOffset2 * indices[0] + 2]; triVertices[3]  = vertices[fixedOffset2 * indices[0] + 3]; triVertices[4]  = vertices[fixedOffset2 * indices[0] + 4]; triVertices[5]  = vertices[fixedOffset2 * indices[0] + 5];		\
				triVertices[6]  = vertices[fixedOffset2 * indices[1]]; triVertices[7]  = vertices[fixedOffset2 * indices[1] + 1]; triVertices[8]  = vertices[fixedOffset2 * indices[1] + 2]; triVertices[9]  = vertices[fixedOffset2 * indices[1] + 3]; triVertices[10] = vertices[fixedOffset2 * indices[1] + 4]; triVertices[11] = vertices[fixedOffset2 * indices[1] + 5];		\
				triVertices[12] = vertices[fixedOffset2 * indices[2]]; triVertices[13] = vertices[fixedOffset2 * indices[2] + 1]; triVertices[14] = vertices[fixedOffset2 * indices[2] + 2]; triVertices[15] = vertices[fixedOffset2 * indices[2] + 3]; triVertices[16] = vertices[fixedOffset2 * indices[2] + 4]; triVertices[17] = vertices[fixedOffset2 * indices[2] + 5];		\
																																																																																													\
				Mesh *obj = new Mesh();																																																																																				\
				obj->CreateMesh(triVertices, triIndices, 18, 3);																																																																													\
				meshList.push_back(obj);																																																																																			\
			}

Object::Object()
{
}

Object::Object(const int xDim, const int yDim, const int zDim, const int numImages, const int zDimTex, const int zScale, const int samplingStep,
	const char* fileLoc, GLfloat specularIntensity, GLfloat shininess) {

	const int fixedOffset1 = 18;
	const int fixedOffset2 = 6;
	const long long indicesCount = fixedOffset1 * xDim * yDim * zDim / (pow(samplingStep, 2));
	const long long verticesCount = fixedOffset2 * xDim * yDim *zDim / (pow(samplingStep, 2));

	// First three elements are the 3D coordinates of a grid of xDim x yDim x zDim
	// The next elements are the 3D texture coordinate
	GLfloat* vertices = new GLfloat[verticesCount];
	for (int k = 0; k < zDim; k++) {
		for (int i = 0; i < (xDim / samplingStep); i++) {
			for (int j = 0; j < (yDim / samplingStep); j++) {
				vertices[fixedOffset2 * (k * xDim*yDim / (int)(pow(samplingStep, 2)) + i * yDim / (samplingStep)+j)] = i * samplingStep - xDim / 2;
				vertices[fixedOffset2 * (k * xDim*yDim / (int)(pow(samplingStep, 2)) + i * yDim / (samplingStep)+j) + 1] = j * samplingStep - yDim / 2;
				vertices[fixedOffset2 * (k * xDim*yDim / (int)(pow(samplingStep, 2)) + i * yDim / (samplingStep)+j) + 2] = zScale * (k - zDim / 2);
				vertices[fixedOffset2 * (k * xDim*yDim / (int)(pow(samplingStep, 2)) + i * yDim / (samplingStep)+j) + 3] = (0.0 + (float)i * samplingStep) / (float)(xDim);
				vertices[fixedOffset2 * (k * xDim*yDim / (int)(pow(samplingStep, 2)) + i * yDim / (samplingStep)+j) + 4] = (0.0 + (float)j * samplingStep) / (float)(yDim);
				vertices[fixedOffset2 * (k * xDim*yDim / (int)(pow(samplingStep, 2)) + i * yDim / (samplingStep)+j) + 5] = (0.5 + (float)k) / (float)(zDim);
			}
		}
	}

	// The indices of triangles needed to cover the grid
	unsigned long* indices = new unsigned long[3];
	unsigned int* triIndices = new unsigned int[3];
	triIndices[0] = 0; triIndices[1] = 1; triIndices[2] = 2;
	GLfloat* triVertices = new GLfloat[18];
	for (int k = 0; k < zDim; k++) {
		for (int i = 0; i < (xDim / samplingStep); i++) {
			for (int j = 0; j < (yDim / samplingStep); j++) {

				indices[0] = (k*xDim*yDim / (pow(samplingStep, 2)) + i * yDim / samplingStep + j);
				indices[1] = ((k + 1)*xDim*yDim / (pow(samplingStep, 2)) + i * yDim / samplingStep + j);
				indices[2] = ((k + 1)*xDim*yDim / (pow(samplingStep, 2)) + (i + 1) * yDim / samplingStep + j);
				COMPUTE_TRIANGLE_VERTICES

					indices[0] = (k*xDim*yDim / (pow(samplingStep, 2)) + i * yDim / samplingStep + j);
				indices[1] = (k*xDim*yDim / (pow(samplingStep, 2)) + (i + 1) * yDim / samplingStep + j);
				indices[2] = ((k + 1)*xDim*yDim / (pow(samplingStep, 2)) + (i + 1) * yDim / samplingStep + j);
				COMPUTE_TRIANGLE_VERTICES

					indices[0] = (k*xDim*yDim / (pow(samplingStep, 2)) + i * yDim / samplingStep + j);
				indices[1] = ((k + 1)*xDim*yDim / (pow(samplingStep, 2)) + i * yDim / samplingStep + j);
				indices[2] = ((k + 1)*xDim*yDim / (pow(samplingStep, 2)) + i * yDim / samplingStep + (j + 1));
				COMPUTE_TRIANGLE_VERTICES

					indices[0] = (k*xDim*yDim / (pow(samplingStep, 2)) + i * yDim / samplingStep + j);
				indices[1] = (k*xDim*yDim / (pow(samplingStep, 2)) + i * yDim / samplingStep + (j + 1));
				indices[2] = ((k + 1)*xDim*yDim / (pow(samplingStep, 2)) + i * yDim / samplingStep + (j + 1));
				COMPUTE_TRIANGLE_VERTICES

					indices[0] = (k*xDim*yDim / (pow(samplingStep, 2)) + i * yDim / samplingStep + j);
				indices[1] = (k*xDim*yDim / (pow(samplingStep, 2)) + (i + 1) * yDim / samplingStep + j);
				indices[2] = (k*xDim*yDim / (pow(samplingStep, 2)) + (i + 1) * yDim / samplingStep + (j + 1));
				COMPUTE_TRIANGLE_VERTICES

					indices[0] = (k*xDim*yDim / (pow(samplingStep, 2)) + i * yDim / samplingStep + j);
				indices[1] = (k*xDim*yDim / (pow(samplingStep, 2)) + i * yDim / samplingStep + (j + 1));
				indices[2] = (k*xDim*yDim / (pow(samplingStep, 2)) + (i + 1) * yDim / samplingStep + (j + 1));
				COMPUTE_TRIANGLE_VERTICES
			}
		}
	}

	//calcAverageNormals(indices, indicesCount, vertices, verticesCount, 9, 6);

	delete[] vertices;
	delete[] indices;
	delete[] triVertices;
	delete[] triIndices;

	// Loading the texture and material of the object
	fileLocation = fileLoc;
	texture = Texture(xDim, yDim, zDimTex, numImages, fileLoc);
	texture.LoadTexture();

	material = Material(specularIntensity, shininess);
}

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset) {
	for (size_t i = 0; i < indiceCount; i += 3) {
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;

		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::abs(glm::cross(v1, v2));
		if (glm::length(normal)) {
			normal = glm::normalize(normal);
		}

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++) {
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		if (glm::length(vec)) {
			vec = glm::normalize(vec);
		}
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void Object::RenderObject(Camera camera, GLuint uniformSpecularIntensity, GLuint uniformShininess, bool sortRequired) {

	// Using the texture and material of the objects
	texture.UseTexture();
	material.UseMaterial(uniformSpecularIntensity, uniformShininess);
	if (sortRequired) {
		// Sorting all triangles according to their distance from the camera
		for (std::vector<Mesh*>::iterator it = meshList.begin(); it != meshList.end(); ++it) {
			(*it)->UpdateCameraPosition(camera.getCameraPosition());
		}
		std::sort(meshList.begin(), meshList.end());
	}

	// Rendering the triangles from the farest to the nearest
	for (std::vector<Mesh*>::iterator it = meshList.begin(); it != meshList.end(); ++it) {
		(*it)->RenderMesh();
	}
}

Object::~Object()
{
	for (std::vector<Mesh*>::iterator it = meshList.begin(); it != meshList.end(); ++it) {
		(*it)->ClearMesh();
	}
	meshList.clear();
	texture.ClearTexture();
}
