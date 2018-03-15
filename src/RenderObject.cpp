#include "RenderObject.h"
#include <assert.h>
using namespace std;


/* RenderObject::RenderObject( Quad loc )
Arguments: Quad loc [in]
loc: Quad location for the texture to be rendered. x1 < x2, y1 < y2
*/
RenderObject::RenderObject()
{
	interactable = false;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &tex);
    glDisable(GL_TEXTURE_2D);
}

void RenderObject::render()
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);

    // texture draw points
    int w[] = { 0, 1, 1, 0 };
    int h[] = { 1, 1, 0, 0};

    glTexCoord2d(w[0], h[0]);
    glVertex2d(loc.x1, loc.y1);
    glTexCoord2d(w[1], h[1]);
    glVertex2d(loc.x2, loc.y1);
    glTexCoord2d(w[2], h[2]);
    glVertex2d(loc.x2, loc.y2);
    glTexCoord2d(w[3], h[3]);
    glVertex2d(loc.x1, loc.y2);

    glEnd();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

    if (glGetError() == GL_NO_ERROR)
    {
        assert(true);
    }
}

bool RenderObject::hovering(double x, double y)
{
    if (interactable 
        && x > loc.x1
        && x < loc.x2
        && y > loc.y1
        && y < loc.y2)
        return true;
    else
        return false;
}

void RenderObject::slide(int x, int y = 0)
{
    loc.x1 += x;
    loc.x2 += x;
    loc.y1 += y;
    loc.y2 += y;
}

void RenderObject::setx(int x)
{
    int w = loc.x2 - loc.x1;
    loc.x1 = x;
    loc.x2 = x + w;
}

void RenderObject::setQuad(Quad newQ)
{
    loc = newQ;
}

Quad RenderObject::getQuad()
{
    return loc;
}

RenderObject::~RenderObject()
{
    glDeleteTextures(1, &tex);
}
