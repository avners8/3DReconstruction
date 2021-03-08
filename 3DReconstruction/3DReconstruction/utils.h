#pragma once

#include <stdio.h>
#include <string>
#include <iostream>

#include <GL\glew.h>

static void checkGLError() {
	switch (glGetError()) {
	case GL_INVALID_ENUM: printf("GL error: GL_INVALID_ENUM\n"); break;
	case GL_INVALID_VALUE: printf("GL error: GL_INVALID_VALUE\n"); break;
	case GL_INVALID_OPERATION: printf("GL error: GL_INVALID_OPERATION\n"); break;
	case GL_STACK_OVERFLOW: printf("GL error: GL_STACK_OVERFLOW\n"); break;
	case GL_STACK_UNDERFLOW: printf("GL error: GL_STACK_UNDERFLOW\n"); break;
	case GL_OUT_OF_MEMORY: printf("GL error: GL_OUT_OF_MEMORY\n"); break;
	case GL_INVALID_FRAMEBUFFER_OPERATION: printf("GL error: GL_INVALID_FRAMEBUFFER_OPERATION\n"); break;
	case GL_CONTEXT_LOST: printf("GL error: GL_CONTEXT_LOST\n"); break;
	case GL_TABLE_TOO_LARGE: printf("GL error: GL_TABLE_TOO_LARGE\n"); break;
	}
}
