#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#include <assert.h>
#include "Image.h"
using namespace std;

int Image::sWidth = -1;
int Image::sHeight = -1;

Image::Image(std::string imageName) : RenderObject()
{

#ifndef NDEBUG
    if (sWidth == -1 || sHeight == -1)
        cerr << "ERROR: Screen details must be set first" << endl;
#endif

    interactable = true;
    int lastSlash = imageName.find_last_of('/');
    iName = imageName.substr(
        lastSlash == string::npos ? 0 : lastSlash + 1,
        imageName.length() - 4);
    iType = imageName.substr(imageName.length() - 3);
#ifndef NDEBUG
    cout << "loading image name: " << iName << endl;
    cout << "loading image type: " << iType << endl;
#endif
    data = stbi_load(imageName.c_str(), &iwidth, &iheight, &cmp, 0);
    o_data = stbi_load(imageName.c_str(), &iwidth, &iheight, &cmp, 0);
    if (data == NULL)
    {
        cerr << "Error: Image failed to load: " << imageName << endl;
        assert(true);
    }

#ifndef NDEBUG
    cout << "Width: " << iwidth << " Height: " << iheight << endl;
#endif
    
    loc = { 0, 
        iwidth > sWidth ? sWidth : iwidth, 
        0, 
        iheight > sHeight ? sHeight : iheight};
    
    glEnable(GL_TEXTURE_2D);
    if (cmp == 3)
    {
        cmp_type = GL_RGB;
    }
    else if (cmp == 4)
    {
        cmp_type = GL_RGBA;
    }

    // Bind texture we're putting data into
    glBindTexture(GL_TEXTURE_2D, tex);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // Draw to texture
    glTexImage2D(GL_TEXTURE_2D, 0, cmp_type, iwidth, iheight, 0, cmp_type, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    if (glGetError() == GL_NO_ERROR)
    {
        assert(true);
    }
}

void Image::generateHSL()
{
	if (HSL_arr.size() == 0) {
		HSL_arr.resize(iheight * iwidth);
	}
	for (int i = 0; i < iheight * iwidth * cmp; i += cmp) {
		rgb_t temp_rgb({ data[i+0], data[i+1], data[i+2] });
		hsl_t temp_hsl;
		temp_hsl = temp_rgb;
		HSL_arr[i / cmp] = temp_hsl;
	}
}

void Image::updateTexture()
{
	for (int i = 0; i < iwidth * iheight * cmp; i += cmp) {
		rgb_t temp_rgb;
		hsl_t temp_hsl = HSL_arr[i / cmp];
		temp_rgb = temp_hsl;
		data[i] = temp_rgb[0];
		data[i + 1] = temp_rgb[1];
		data[i + 2] = temp_rgb[2];
	}
	

	// Bind texture we're putting data into
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);

	// Draw to texture
    glTexImage2D(GL_TEXTURE_2D, 0, cmp_type, iwidth, iheight, 0, cmp_type, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

}

void Image::dumpImage()
{
    string temp = iName + "Dump.png";
    stbi_write_png(temp.c_str(), iwidth, iheight, cmp, data, iwidth);
}

void Image::setH(float h)
{
    for (int i = 0; i < iwidth * iheight; i++)
        HSL_arr[i][0] += HSL_arr[i][0] * h;
}

void Image::setS(float s)
{
    for (int i = 0; i < iwidth * iheight; i++)
        HSL_arr[i][1] += HSL_arr[i][1] * s;
}

void Image::setL(float l)
{
    for (int i = 0; i < iwidth * iheight; i++)
        HSL_arr[i][2] += HSL_arr[i][2] * l;
}

int Image::getWidth()
{
    return iwidth;
}

int Image::getHeight()
{
    return iheight;
}

void Image::getMaxImageWindowSize(int &w, int &h)
{
    w = iwidth > sWidth ? sWidth : iwidth;
    h = iheight > sHeight ? sHeight : iheight;
}

void Image::setInteractable(bool inter)
{
    interactable = inter;
}

void Image::scale(float xs, float ys)
{
    loc.x2 = loc.x1 + iwidth / xs;
    loc.y2 = loc.y1 + iheight / ys;
}

#ifndef NDEBUG
string Image::getName()
{
    return iName;
}
#endif

char Image::button()
{
    return iName[0];
}

void Image::resetImageData()
{
    memcpy(data, o_data, sizeof(unsigned char) * cmp * iwidth * iheight);
    HSL_arr.clear();
    generateHSL();
    // Bind texture we're putting data into
    glBindTexture(GL_TEXTURE_2D, tex);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // Draw to texture
    glTexImage2D(GL_TEXTURE_2D, 0, cmp_type, iwidth, iheight, 0, cmp_type, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void Image::SetScreenDetails(int w, int h)
{
    sWidth = w - 400;
    sHeight = h - 400;
}

Image::~Image()
{
    stbi_image_free(data);
    stbi_image_free(o_data);
}
