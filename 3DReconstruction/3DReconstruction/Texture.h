#pragma once

#include <stdio.h>
#include <string>
#include <iostream>

#include <GL\glew.h>

#include "stb_image.h"

#include "utils.h"

class Texture
{
public:
	Texture();
	Texture(int _xDim, int _yDim, int _zDim, int _numImages, const char* fileLoc);

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

	~Texture();

private:
	GLuint textureID;
	int xDim, yDim, zDim, bitDepth, numImages;

	const char* fileLocation;
};

