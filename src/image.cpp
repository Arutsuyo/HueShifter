#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#include <assert.h>
#include "image.h"
using namespace std;

Image::Image(std::string imageName) : RenderObject()
{
	interactable = true;
    iType = imageName.substr(imageName.length() - 4);
    iName = imageName.substr(0, imageName.length() - 4);
    cout << "loading image type: " << iType << endl;
    data = stbi_load(imageName.c_str(), &iwidth, &iheight, &cmp, 0);
    if (data == NULL)
    {
        cerr << "Error: Image failed to load: " << imageName << endl;
        assert(true);
    }
    
    cout << "Image: " << iName << " Width: " << iwidth << " height: " << iheight << endl;
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

Image::~Image()
{
    stbi_image_free(data);
}
