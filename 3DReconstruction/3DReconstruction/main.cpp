#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <algorithm>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"
#include "Material.h"
#include "Object.h"

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Light mainLight;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

GLfloat alphaPower = 4;
GLfloat alphaSpeed = 2;
GLfloat alphaThresh = 0.0;
GLfloat alphaThreshSpeed = 0.4;
GLfloat upperThresh = 2;
GLfloat upperThreshSpeed = 0.3;
GLfloat brightness = 2.0f;
GLfloat brightnessSpeed = 0.6f;

bool transparentMode = true;

static const char* instructions =
"3D reconstruction from medical images\n"
"Using CT or MRI images to construct a 3D model of the body or other organs.\n"
"Controls:\n"
"	- camera movment : w/a/s/d + mouse \n"
"	- brightness     : page up/down \n"
"	- alpha power    : up/down arrows \n"
"	- lower threshold: right/left arrows \n"
"	- upper threshold: o/p \n"
"	- visual mode    : space \n"
"	- exit           : esc \n";

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

const float pi = 3.141592653589793238462643383279502884197169399375105820974944592307816406;

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void generalKeyControl(bool* keys, GLfloat deltaTime) {
	GLfloat velocity = alphaSpeed * deltaTime;
	GLfloat velocityThresh = alphaThreshSpeed * deltaTime;
	GLfloat velocityBrightness = brightnessSpeed * deltaTime;
	GLfloat velocityUpperThresh = upperThreshSpeed * deltaTime;

	if (keys[GLFW_KEY_UP]) {
		alphaPower += velocity;
	}

	if (keys[GLFW_KEY_DOWN]) {
		alphaPower = std::max(0.0f, alphaPower - velocity);
	}

	if (keys[GLFW_KEY_RIGHT]) {
		alphaThresh += velocityThresh;
	}

	if (keys[GLFW_KEY_LEFT]) {
		alphaThresh = std::max(0.0f, alphaThresh - velocityThresh);
	}

	if (keys[GLFW_KEY_PAGE_UP]) {
		brightness += velocityBrightness;
	}

	if (keys[GLFW_KEY_PAGE_DOWN]) {
		brightness = std::max(0.0f, brightness - velocityBrightness);
	}

	if (keys[GLFW_KEY_P]) {
		upperThresh += velocityUpperThresh;
	}

	if (keys[GLFW_KEY_O]) {
		upperThresh = std::max(0.0f, upperThresh - velocityUpperThresh);
	}

	if (keys[GLFW_KEY_SPACE]) {
		transparentMode = !transparentMode;
	}
}

struct objectStruct {
	const char* fileLoc;
	const int xDim, yDim, zDim, numImages, zDimTex, zScale, samplingStep;
};

int main()
{

	printf("%s\n", instructions);

	mainWindow = Window(1366, 768);
	mainWindow.Initialise();

	GLfloat specularIntensity = 1.0f, shininess = 32;

	//							    fileLoc                     xDim, yDim, zDim, numImages, zDimTex, zScale, samplingStep
	objectStruct CTBodyStruct = { "Textures/CT_Full_Body/1-", 512,  512,  140,  239,       256,     8,      60 };
	objectStruct CTLungStruct = { "Textures/CT_Lung/1-",		512,  512,  55,   237,       256,     7,      32 };
	objectStruct MRBrainStruct = { "Textures/MR_Brain/1-",     288,  288,  120,  310,       512,     4,      32 };

	objectStruct currentObjStruct = CTBodyStruct;
	bool rotate_x = (currentObjStruct.fileLoc == MRBrainStruct.fileLoc);

	Object currentObject = Object(currentObjStruct.xDim, currentObjStruct.yDim, currentObjStruct.zDim,
		currentObjStruct.numImages, currentObjStruct.zDimTex, currentObjStruct.zScale,
		currentObjStruct.samplingStep, currentObjStruct.fileLoc, specularIntensity, shininess);

	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 100.0f, 0.4f);

	mainLight = Light(1.0f, 1.0f, 1.0f, 0.99f,
		1.0f, 1.0f, 1.0f, 0.1f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformAmbientIntensity = 0, uniformAmbientColour = 0,
		uniformDirection = 0, uniformDiffuseIntensity = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0,
		uniformAlphaPower = 0, uniformAlphaThresh = 0, uniformBrightness = 0,
		uniformTransparentMode = 0, uniformUpperThresh = 0;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 300.0f, 4000.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + Handle user input events
		glfwPollEvents();

		bool keyChanged = camera.keyControl(mainWindow.getsKeys(), deltaTime);
		bool mouseChanged = camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		bool sortRequired = keyChanged || mouseChanged;

		generalKeyControl(mainWindow.getsKeys(), deltaTime);
		if (transparentMode) {
			glDisable(GL_DEPTH_TEST);
		}
		else {
			glEnable(GL_DEPTH_TEST);
		}

		// Clear window
		glClearColor(0.14f, 0.12f, 0.12f, 0.001f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformAmbientColour = shaderList[0].GetAmbientColourLocation();
		uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();
		uniformDirection = shaderList[0].GetDirectionLocation();
		uniformDiffuseIntensity = shaderList[0].GetDiffuseIntensityLocation();
		uniformEyePosition = shaderList[0].GetEyePosition();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();
		uniformAlphaPower = shaderList[0].GetAlphaPowerLocation();
		uniformAlphaThresh = shaderList[0].GetAlphaThreshLocation();
		uniformBrightness = shaderList[0].GetBrightnessLocation();
		uniformTransparentMode = shaderList[0].GetTransparentModeLocation();
		uniformUpperThresh = shaderList[0].GetUpperThreshLocation();

		mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColour,
			uniformDiffuseIntensity, uniformDirection);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		glUniform1f(uniformAlphaPower, alphaPower);
		glUniform1f(uniformAlphaThresh, alphaThresh);
		glUniform1f(uniformBrightness, brightness);
		glUniform1f(uniformTransparentMode, transparentMode);
		glUniform1f(uniformUpperThresh, upperThresh);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -600.0f));
		model = glm::rotate(model, pi / 2, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, pi, glm::vec3(0.0f, 0.0f, 1.0f));
		if (rotate_x) model = glm::rotate(model, pi / 2, glm::vec3(1.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		currentObject.RenderObject(camera, uniformSpecularIntensity, uniformShininess, sortRequired);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}
	return 0;
}

