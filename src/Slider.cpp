#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#include <assert.h>
#include <iostream>
#include "Slider.h"
using namespace std;

Slider::Slider(Quad place, std::string barName, std::string pointerName) :
    bar(barName), point(pointerName)
{
    bar.setInteractable(false);
    bar.setQuad(place);

    point.setInteractable(true);
    place.x1 += (bar.getWidth() / 2) - point.getWidth() / 6;
    place.x2 = place.x1 + point.getWidth() / 3;
    place.y1 = place.y2 - (place.y2 - place.y1) / 2;
    place.y2 += point.getHeight() / 4;
    point.setQuad(place);
}

void Slider::render()
{
    bar.render();
    point.render();
}

bool Slider::hovering(double x, double y)
{
    return point.hovering(x, y);
}

void Slider::slide(int x, int y = 0)
{
    point.slide(x, y);
}

void Slider::setx(int x)
{
    Quad ploc = point.getQuad();
    Quad bloc = bar.getQuad();
    int w = ploc.x2 - ploc.x1;
    ploc.x1 = x - w / 2 <= bloc.x1 - w / 2 ? 
        bloc.x1 - w / 2
        : x - w / 2 >= bloc.x2 - w/2 ? 
            bloc.x2 - w / 2 
            : x - w / 2;
    ploc.x2 = ploc.x1 + w;
    point.setQuad(ploc);
#ifndef NDEBUG
    cout << "Bar: " << bar.getName() << " Value: " << getSliderValue() << endl;
#endif
}

float Slider::getSliderValue()
{
    Quad bq = bar.getQuad();
    Quad pq = point.getQuad();

    int midx = (pq.x2 - pq.x1) / 2;
    midx += pq.x1;
    return ((midx - bq.x1) / ((float)(bq.x2 - bq.x1))) - 0.5f;
}

Slider::~Slider()
{

}
