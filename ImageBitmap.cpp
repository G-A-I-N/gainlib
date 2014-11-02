// Copyright 2014
//   ville.kankainen@gmail.com
//   ville.kankainen@kangain.com
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

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "ImageBitmap.h"


namespace Gain {

static ImageBitmapCache* gBitmapCache = 0;

ImageBitmap::ImageBitmap(float x,float y, float width, float height, const char* aImageFile) :
Bitmap(x,y,width,height)
{
	readFile(aImageFile);
}

ImageBitmap::ImageBitmap(int x,int y, int width, int height, const char* aImageFile) :
Bitmap(x,y,width,height)
{
	readFile(aImageFile);
}

ImageBitmap::ImageBitmap(const char* aImageFile) :
Bitmap()
{
	readFile(aImageFile);
}

ImageBitmap::~ImageBitmap() {
}

void ImageBitmap::readFile(const char* aFileName)
{
	if(!gBitmapCache)
	{
		gBitmapCache = new ImageBitmapCache();
	}

#ifdef IOS
    const char* new_path = get_asset_filepath(aFileName);
	ImageBitmapCacheData data = gBitmapCache->loadBitmap(new_path);
#else
	ImageBitmapCacheData data = gBitmapCache->loadBitmap(aFileName);
#endif


    pBitmapWidth = data.width;
    pBitmapHeight = data.height;
    pBitmapSlotSize = data.bitsPerPixel;
    pBitmap = data.bitmap;
    pPngLiteral = data.literal;

}

bool ImageBitmap::initVariables() {
	if(!super::initVariables()) {
		return false;
	}

	const char* attribute_name;
	const char* uniform_name;

	ImageBitmapCacheData data;
	data.width = pBitmapWidth;
	data.height = pBitmapHeight;
	data.bitsPerPixel = pBitmapSlotSize;
	data.bitmap = pBitmap;
	data.literal = pPngLiteral;
	texture_id = gBitmapCache->openglId(&data);

	GLfloat square_texcoords[] = {
			// front
			0.0, 0.0,
			1.0, 0.0,
			1.0, 1.0,
			0.0, 1.0,
	};
    GL_EXT_FUNC glGenBuffers(1, &vbo_square_texcoords);
	checkGlError("glGenBuffers");

    GL_EXT_FUNC glBindBuffer(GL_ARRAY_BUFFER, vbo_square_texcoords);
	checkGlError("glBindBuffer");

    GL_EXT_FUNC glBufferData(GL_ARRAY_BUFFER, sizeof(square_texcoords), square_texcoords, GL_STATIC_DRAW);
	checkGlError("glBufferData");

	return true;
}




void ImageBitmap::updateG(float time, float timeDelta)
{
	super::updateG( time,  timeDelta);
}


void ImageBitmap::enableAttributes() const
{
	super::enableAttributes();
}

void ImageBitmap::disableAttributes() const
{
    GL_EXT_FUNC glDisableVertexAttribArray(attribute_texcoord);
	super::disableAttributes();
}




ImageBitmapCacheData ImageBitmapCache::loadBitmap(std::string filename)
{
	std::map<std::string,ImageBitmapCacheData>::iterator it = pBitmapMap.find(filename);

	if(it != pBitmapMap.end())
	{
		return it->second;
	}

	ImageBitmapCacheData data;

	int width, height, comp;
    unsigned char *image_data;

	FILE *file = fopen(filename.c_str(), "rb");
	if (!file) {
	return data;
	}

	image_data = stbi_load_from_file(file, &width, &height, &comp, 0);

	fclose(file);

    unsigned char header[8];    // 8 is the maximum size that can be checked


    data.width = width;
    data.height = height;
    data.bitsPerPixel = comp*8;
    data.bitmap = image_data;
	data.literal = filename;

    pBitmapMap[filename] = data;

	return data;

}

static unsigned long sdbm(uint8_t* data, int len )
{
	unsigned long hash = 0;

	//65599
	while (--len)
		hash = data[len] + (hash << 6) + (hash << 16) - hash;

	return hash;
}

GLuint ImageBitmapCache::openglId(ImageBitmapCacheData* data)
{
	std::map<std::string,unsigned int>::iterator it = pOpenglCache.find(data->literal);

	if(it != pOpenglCache.end())
	{
		return it->second;
	}

	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
	        GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			GL_CLAMP_TO_EDGE);

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, /*GL10.GL_REPLACE*/ GL_MODULATE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, // target
			0,  // level, 0 = base, no minimap,
			GL_RGBA, // internalformat
			data->width,  // width
			data->height,  // height
			0,  // border, always 0 in OpenGL ES
			GL_RGBA,  // format
			GL_UNSIGNED_BYTE, // type
			data->bitmap);

	pOpenglCache.insert(std::pair<std::string, unsigned int>(data->literal,texture_id));
	return texture_id;
}


} /* namespace Gain */
