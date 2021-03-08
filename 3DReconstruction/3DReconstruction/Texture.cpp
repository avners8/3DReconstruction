#include "Texture.h"

Texture::Texture()
{
	textureID = 0;
	xDim = 0; yDim = 0; zDim = 0;
	numImages = 0;
	bitDepth = 0;
	fileLocation = "";
}

Texture::Texture(int _xDim, int _yDim, int _zDim, int _numImages, const char* fileLoc) {
	textureID = 0;
	xDim = _xDim; yDim = _yDim; zDim = _zDim;
	numImages = _numImages;
	bitDepth = 0;
	fileLocation = fileLoc;

}

void Texture::LoadTexture() {

	int width, height;
	const long size = 4 * xDim * yDim * zDim;
	const long sizeImage = xDim * yDim;

	//load data into a 3D texture
	glGenTextures(1, &textureID); checkGLError();
	glBindTexture(GL_TEXTURE_3D, textureID); checkGLError();

	// set the texture parameters
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); checkGLError();

	// Initializing a 3D texture
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, xDim, yDim, zDim, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	checkGLError();

	for (int im = 0; im < numImages; im++) {
		char fileName[100];
		char suffix[100];
		sprintf_s(suffix, "%03d", im + 1); strcpy_s(fileName, fileLocation); strcat_s(fileName, suffix); strcat_s(fileName, ".png");

		// Filling each 2D texture with an image
		unsigned char *textData = stbi_load(fileName, &width, &height, &bitDepth, 0);
		glTexSubImage3D(GL_TEXTURE_3D, 0, 0 /* x offset */, 0 /* y offset */, im + 1, xDim, yDim, 1, GL_RGBA, GL_UNSIGNED_BYTE, textData);
		stbi_image_free(textData);
	}

	glGenerateMipmap(GL_TEXTURE_3D);

	glBindTexture(GL_TEXTURE_3D, 0);
}

void Texture::UseTexture() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, textureID);
}

void Texture::ClearTexture() {
	glDeleteTextures(1, &textureID);
	textureID = 0;
	xDim = 0; yDim = 0; zDim = 0;
	numImages = 0;
	bitDepth = 0;
	fileLocation = "";
}

Texture::~Texture()
{
	ClearTexture();
}