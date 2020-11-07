/*
  ==============================================================================

    PictureColorSource.cpp
    Created: 2 Nov 2020 6:20:22pm
    Author:  bkupe

  ==============================================================================
*/

#include "PictureColorSource.h"

PictureColorSource::PictureColorSource(var params) :
    TimedColorSource(getTypeString(), params)
{
	pictureFile = new FileParameter("Picture File", "The file, the picture, the thing", "");
	addParameter(pictureFile);

	brightness = addFloatParameter("Brightness", "The brightness", 1, 0, 1);
	saturation = addFloatParameter("Saturation", "The saturation", 1, 0, 3);
	hue = addFloatParameter("Hue", "The hue", 0, -1, 1);
}

PictureColorSource::~PictureColorSource()
{
}

void PictureColorSource::onContainerParameterChangedInternal(Parameter *p)
{
	TimedColorSource::onContainerParameterChangedInternal(p);

	if (p == pictureFile)
	{
		File f = pictureFile->getFile();
		picture = ImageCache::getFromFile(f);
	}
}

void PictureColorSource::fillColorsForObjectTimeInternal(Array<Colour>& colors, Object* o, ColorComponent* comp, int id, float time)
{
	if (picture.getWidth() == 0) return;

	int numPixelsH = picture.getHeight();
	float txRel = fmodf(time, 1);
	int  tx = jmin<int>(txRel * picture.getWidth(), picture.getWidth() - 1);

	const int resolution = colors.size();
	for (int i = 0; i < resolution; i++)
	{
		float ty = jmin(i * numPixelsH / resolution, numPixelsH - 1);

		float h = 0, s = 0, b = 0;
		picture.getPixelAt(tx, ty).getHSB(h, s, b);
		colors.set(i, Colour::fromHSV(h + hue->floatValue(), jmin(1.0f, s * saturation->floatValue()), jmin(1.0f, b * brightness->floatValue()), 1));
	}
}
