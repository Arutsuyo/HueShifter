#include <string>
#include "stb_image.h"

class Image
{
public:
    Image(std::string imageName);
    ~Image();

    void dumpImage();

private:
    int width;
    int height;
    int comp;
    unsigned char *data;
    std::string imageType_;
};