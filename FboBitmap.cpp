// Copyright 2014
//   ville.kankainen@gmail.com
// 
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "FboBitmap.h"

static const char gVertexShader[] =
        "attribute vec2 coord2d;\n"
		"attribute vec2 texcoord;"
		"varying vec2 f_texcoord;"
		"uniform mat4 anim;"
		"void main() {\n"
		"  gl_Position = anim * vec4(coord2d.x, coord2d.y, 0.0, 1.0);"
		"  f_texcoord = texcoord;"
		"}\n";

static const char gFragmentShader[] =
        "precision mediump float;\n"
		"varying vec2 f_texcoord;"
		"uniform sampler2D mytexture;"
		"uniform vec4 color;"

		"void main(void) {"
		"  vec2 flipped_texcoord = vec2(f_texcoord.x, f_texcoord.y);"
		"  gl_FragColor = texture2D(mytexture, flipped_texcoord)*color;"
		"}"
		;
FboBitmap::FboBitmap(int x,int y, int width, int height)
:Bitmap(x,y,width,height,gVertexShader, gFragmentShader),
 virgin(1)
{
	pBitmapWidth = width;
	pBitmapHeight = height;

//	pBitmapWidth = 512;
//	pBitmapHeight = 512;

}

FboBitmap::~FboBitmap() {
	// TODO Auto-generated destructor stub
}

bool FboBitmap::initVariables() {
	if(!super::initVariables()) {
		return false;
	}

	glGenFramebuffers(1, &fbo);
    checkGlError("glGenFramebuffers");

    glGenTextures(1, &texture_id);
    checkGlError("glGenTextures");

    glBindTexture(GL_TEXTURE_2D, texture_id);
    checkGlError("glBindTexture");
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    checkGlError("glTexParameterf");
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
	        GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			GL_CLAMP_TO_EDGE);
    checkGlError("glTexParameterf");

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, /*GL10.GL_REPLACE*/ GL_MODULATE);

//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, // target
			0,  // level, 0 = base, no minimap,
			GL_RGBA, // internalformat
			pBitmapWidth,  // width
			pBitmapHeight,  // height
			0,  // border, always 0 in OpenGL ES
			GL_RGBA,  // format
			GL_UNSIGNED_BYTE, // type
 			NULL);
    checkGlError("glTexImage2D");
//	GL_EXT_FUNC glUniform1i(uniform_mytexture, 0);
//    checkGlError("glUniform1i");

	glBindTexture(GL_TEXTURE_2D, 0);
    checkGlError("glBindTexture");

    glGenRenderbuffers(1, &rbo_depth);
    checkGlError("glGenRenderbuffers");
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
    checkGlError("glBindRenderbuffer");
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, pBitmapWidth, pBitmapHeight);
    checkGlError("glRenderbufferStorage");
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
    checkGlError("glBindRenderbuffer");

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    checkGlError("glBindFramebuffer");
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);
    checkGlError("glFramebufferRenderbuffer");
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);
    checkGlError("glFramebufferTexture2D");
	GLenum status;
	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "glCheckFramebufferStatus: error 0x%x", status);
		return 0;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    checkGlError("glBindFramebuffer");

//	GLfloat square_texcoords[] = {
//			// front
//			0.0, 0.0,
//			1.0, 0.0,
//			1.0, 1.0,
//			0.0, 1.0,
//	};
//	GLfloat square_texcoords[] = {
//			// front
//			-1, -1,
//			 1,  -1,
//			-1,  1,
//			 1,  1,
//	};
//
//	GL_EXT_FUNC glGenBuffers(1, &vbo_square_texcoords);
//	checkGlError("glGenBuffers");
//
//    GL_EXT_FUNC glBindBuffer(GL_ARRAY_BUFFER, vbo_square_texcoords);
//	checkGlError("glBindBuffer");
//
//    GL_EXT_FUNC glBufferData(GL_ARRAY_BUFFER, sizeof(square_texcoords), square_texcoords, GL_STATIC_DRAW);
//	checkGlError("glBufferData");
//
//	GL_EXT_FUNC glBindBuffer(GL_ARRAY_BUFFER, 0);
//	checkGlError("glBindBuffer");

	return true;
}


void FboBitmap::enableAttributes() const
{
	super::enableAttributes();

    GL_EXT_FUNC glEnableVertexAttribArray(attribute_texcoord);


}

void FboBitmap::disableAttributes() const
{
    GL_EXT_FUNC glDisableVertexAttribArray(attribute_texcoord);
	super::disableAttributes();
}

void FboBitmap::SetAsActiveFrameBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    checkGlError("glBindFramebuffer");

//	glViewport(0,0,pBitmapWidth,pBitmapHeight);
//    checkGlError("glBindFramebuffer");

}
