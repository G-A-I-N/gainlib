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


#include <png.h>
#include "PngBitmap.h"


namespace Gain {

static PngBitmapCache* gBitmapCache = 0;

PngBitmap::PngBitmap(float x,float y, float width, float height, const char* aPngFile) :
Bitmap(x,y,width,height)
{
	readFile(aPngFile);
}

PngBitmap::PngBitmap(int x,int y, int width, int height, const char* aPngFile) :
Bitmap(x,y,width,height)
{
	readFile(aPngFile);
}

PngBitmap::PngBitmap(const char* aPngFile) :
Bitmap()
{
	readFile(aPngFile);
}

PngBitmap::~PngBitmap() {
}

void PngBitmap::readFile(const char* aFileName)
{
	if(!gBitmapCache)
	{
		gBitmapCache = new PngBitmapCache();
	}

#ifdef IOS
    const char* new_path = get_asset_filepath(aFileName);
	BitmapCacheData data = gBitmapCache->loadBitmap(new_path);
#else
	BitmapCacheData data = gBitmapCache->loadBitmap(aFileName);
#endif


    pBitmapWidth = data.width;
    pBitmapHeight = data.height;
    pBitmapSlotSize = data.bitsPerPixel;
    pBitmap = data.bitmap;
    pPngLiteral = data.literal;

}

bool PngBitmap::initVariables() {
	if(!super::initVariables()) {
		return false;
	}

	const char* attribute_name;
	const char* uniform_name;

	BitmapCacheData data;
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




void PngBitmap::updateG(float time, float timeDelta)
{
	super::updateG( time,  timeDelta);
}


void PngBitmap::enableAttributes() const
{
	super::enableAttributes();
}

void PngBitmap::disableAttributes() const
{
    GL_EXT_FUNC glDisableVertexAttribArray(attribute_texcoord);
	super::disableAttributes();
}




BitmapCacheData PngBitmapCache::loadBitmap(std::string filename)
{
	std::map<std::string,BitmapCacheData>::iterator it = pBitmapMap.find(filename);

	if(it != pBitmapMap.end())
	{
		return it->second;
	}

	BitmapCacheData data;

    unsigned char header[8];    // 8 is the maximum size that can be checked

    int x, y;

    png_byte color_type;
    png_byte bit_depth;

    png_structp png_ptr;
    png_infop info_ptr;
    int number_of_passes;
    png_bytep * row_pointers;

    /* open file and test for it being a png */
    FILE *fp = fopen(filename.c_str(), "rb");
    if (!fp)
            printf("[read_png_file] File (%s) could not be opened for reading", filename.c_str());
    fread(header, 1, 8, fp);

    if (png_sig_cmp(header, 0, 8))
            printf("[read_png_file] File %s is not recognized as a PNG file",  filename.c_str());


    /* initialize stuff */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr)
            printf("[read_png_file] png_create_read_struct failed");

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
            printf("[read_png_file] png_create_info_struct failed");

    if (setjmp(
    		png_jmpbuf( png_ptr ) )
        )
            printf("[read_png_file] Error during init_io");

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    data.width = png_get_image_width(png_ptr, info_ptr);
    data.height = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    number_of_passes = png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);


    /* read file */
    if (setjmp(png_jmpbuf(png_ptr)))
            printf("[read_png_file] Error during read_image");

    int row_bytes = png_get_rowbytes(png_ptr,info_ptr);
    data.bitsPerPixel = row_bytes*8/data.width;
    data.bitmap = (png_byte*) malloc(data.height*row_bytes);

    row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * data.height);
    for (y=0; y<data.height; y++)
            row_pointers[y] = (png_byte*) &data.bitmap[y*row_bytes];

    png_read_image(png_ptr, row_pointers);

    fclose(fp);
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

GLuint PngBitmapCache::openglId(BitmapCacheData* data)
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
