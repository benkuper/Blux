/*
  ==============================================================================

    ColorSourceFactory.cpp
    Created: 7 Nov 2020 5:26:47pm
    Author:  bkupe

  ==============================================================================
*/

juce_ImplementSingleton(ColorSourceFactory)

ColorSourceFactory::ColorSourceFactory()
{
    updateDefinitions();
}

ColorSourceFactory::~ColorSourceFactory()
{
}

void ColorSourceFactory::updateDefinitions()
{
    defs.clear();
    Image defaultImg = ImageCache::getFromMemory(BinaryData::icon2_png, BinaryData::icon2_pngSize);

    //Add natives
    defs.add(Factory<ColorSource>::Definition::createDef("Generic", "Solid Color", &SolidColorSource::create));
    defs.add(Factory<ColorSource>::Definition::createDef("Generic", "Rainbow", &RainbowColorSource::create));
    defs.add(Factory<ColorSource>::Definition::createDef("Generic", "Strobe", &StrobeColorSource::create));
    defs.add(Factory<ColorSource>::Definition::createDef("Generic", "Noise", &NoiseColorSource::create));
    defs.add(Factory<ColorSource>::Definition::createDef("Generic", "Point", &PointColorSource::create));
    defs.add(Factory<ColorSource>::Definition::createDef("Generic", "Multipoint", &MultiPointColorSource::create));
    defs.add(Factory<ColorSource>::Definition::createDef("Generic", "Gradient", &GradientColorSource::create));

    defs.add(Factory<ColorSource>::Definition::createDef("Feed", "Picture", &PictureColorSource::create));
    defs.add(Factory<ColorSource>::Definition::createDef("Feed", "Video File", &VideoColorSource::create));
    defs.add(Factory<ColorSource>::Definition::createDef("Feed", "Spout/Syphon", &SpoutSyphonColorSource::create));

    defs.add(Factory<ColorSource>::Definition::createDef("Advanced", "Node", &NodeColorSource::create));
    defs.add(Factory<ColorSource>::Definition::createDef("Advanced", "Script", &ScriptColorSource::create));


    for (auto& d : defs)
    {
        int iSize = 0;
        String iconName = d->type.replace(" ", "").replace("/", "").toLowerCase() + "_png";
        const char* imgData = BinaryData::getNamedResource(iconName.getCharPointer(), iSize);
        Image i = ImageCache::getFromMemory(imgData, iSize);
        d->addIcon(i);
    }


    //Custom

    buildPopupMenu();
}
