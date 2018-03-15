#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#include <assert.h>
#include "Image.h"
#include "color/color.hpp"

using namespace std;

Image::Image(std::string imageName) : RenderObject()
{
	is_main = false;
    interactable = true;
    int lastSlash = imageName.find_last_of('/');
    iName = imageName.substr(
        lastSlash == string::npos ? 0 : lastSlash + 1, 
        imageName.length() - 4);
    iType = imageName.substr(imageName.length() - 3);
    cout << "loading image name: " << iName << endl;
    cout << "loading image type: " << iType << endl;
    data = stbi_load(imageName.c_str(), &iwidth, &iheight, &cmp, 0);
    if (data == NULL)
    {
        cerr << "Error: Image failed to load: " << imageName << endl;
        assert(true);
    }
    
    cout << "Width: " << iwidth << " Height: " << iheight << endl;
	loc = {0, iwidth, 0, iheight};
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

void Image::setIsMain() {
	is_main = true;
}

void Image::generateHSL(int w, int h)
{
	//HSL_arr = image(w * h);
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {

		}
	}
}

void Image::setH(int dif)
{
}

void Image::setS(int dif)
{
}

void Image::setL(int diff)
{
}

void Image::dumpImage()
{
    string temp = iName + ".png";
    stbi_write_png(temp.c_str(), iwidth, iheight, cmp, data, iwidth);
}

int Image::getWidth()
{
	return iwidth;
}

int Image::getHeight()
{
	return iheight;
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

string Image::getName()
{
    return iName;
}

Image::~Image()
{
    stbi_image_free(data);
}
