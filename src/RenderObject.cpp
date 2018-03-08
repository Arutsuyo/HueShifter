#include "RenderObject.h"
using namespace std;


/* RenderObject::RenderObject( Quad loc )
Arguments: Quad loc [in]
loc: Quad location for the texture to be rendered. x1 < x2, y1 < y2
*/
RenderObject::RenderObject(Quad renderLocation) : loc(renderLocation)
{
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &tex);
    
    glDisable(GL_TEXTURE_2D);
}

void RenderObject::render()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);

    // texture draw points
    int w[] = { 0, 0, 1, 1 };
    int h[] = { 0, 1, 1, 0};

    glTexCoord2d(0, 0);
    glVertex2d(loc.x1, loc.y1);
    glTexCoord2d(0, 1);
    glVertex2d(loc.x1, loc.y2);
    glTexCoord2d(1, 1);
    glVertex2d(loc.x2, loc.y2);
    glTexCoord2d(1, 0);
    glVertex2d(loc.x2, loc.y1);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}

RenderObject::~RenderObject()
{
    glDeleteTextures(1, &tex);
}
