/*
 * Base.cpp
 *
 *  Created on: Feb 2, 2013
 *      Author: villekankainen
 */

#include "Base.h"

namespace Gain {

Base::Base()
:pState(NOT_INITIALIZED)
{
}

Base::~Base()
{
}

void Base::updateG(float /*time*/, float /*deltaTime*/)
{
}


GLuint Base::createProgram(const char* pVertexSource, const char* pFragmentSource) {
	static const char* lastLoaded_vertex;
	static const char* lastLoaded_fragment;
	static GLuint lastLoaded_program;

	if (pVertexSource == lastLoaded_vertex &&
		pFragmentSource == lastLoaded_fragment)
	{

		return lastLoaded_program;
	}



	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
	if (!vertexShader) {
		return 0;
	}

	GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
	if (!pixelShader) {
		return 0;
	}

    GLuint program = GL_EXT_FUNC glCreateProgram();
	if (program) {
        GL_EXT_FUNC glAttachShader(program, vertexShader);
		checkGlError("glAttachVShader");
        GL_EXT_FUNC glAttachShader(program, pixelShader);
		checkGlError("glAttachFShader");
        GL_EXT_FUNC glLinkProgram(program);
		GLint linkStatus = GL_FALSE;
        GL_EXT_FUNC glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE) {
			GLint bufLength = 0;
            GL_EXT_FUNC glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
			if (bufLength) {
				char* buf = (char*) malloc(bufLength);
				if (buf) {
                    GL_EXT_FUNC glGetProgramInfoLog(program, bufLength, NULL, buf);
					LOGE("Could not link program:\n%s\n", buf);
					free(buf);
				}
			}
            GL_EXT_FUNC glDeleteProgram(program);
			program = 0;
		}
	}
	lastLoaded_vertex = pVertexSource;
	lastLoaded_fragment = pFragmentSource;

	lastLoaded_program = program;
	return program;
}


GLuint Base::loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = GL_EXT_FUNC glCreateShader(shaderType);
	if (shader) {
        GL_EXT_FUNC glShaderSource(shader, 1, &pSource, NULL);
        GL_EXT_FUNC glCompileShader(shader);
		GLint compiled = 0;
        GL_EXT_FUNC glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			GLint infoLen = 0;
            GL_EXT_FUNC glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen) {
				char* buf = (char*) malloc(infoLen);
				if (buf) {
                    GL_EXT_FUNC glGetShaderInfoLog(shader, infoLen, NULL, buf);
					LOGE("Could not compile shader %d:\n%s\n",
							shaderType, buf);
					free(buf);
				}
                GL_EXT_FUNC glDeleteShader(shader);
				shader = 0;
			}
		}
	}
	return shader;
}

} /* namespace Gain */
