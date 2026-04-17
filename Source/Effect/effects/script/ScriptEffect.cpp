/*
  ==============================================================================

    ScriptEffect.cpp
    Created: 20 Nov 2021 10:06:39am
    Author:  bkupe

  ==============================================================================
*/

#include "Effect/EffectIncludes.h"
#include "Common/Helpers/FastNoiseLite.h"

namespace
{
  float getNormalisedNoiseValue(float x, float y, int octaves = 1)
    {
        fnl_state noiseState = fnlCreateState();
        noiseState.frequency = 1.0f;
        noiseState.noise_type = FNL_NOISE_OPENSIMPLEX2;
        noiseState.fractal_type = octaves > 1 ? FNL_FRACTAL_FBM : FNL_FRACTAL_NONE;
        noiseState.octaves = jmax(octaves, 1);
        return jlimit(0.0f, 1.0f, fnlGetNoise2D(&noiseState, x, y) * 0.5f + 0.5f);
    }

    class ScriptEffectScript :
        public Script
    {
    public:
        ScriptEffectScript(ScriptTarget* parentTarget, bool canBeDisabled, bool canBeRemoved) :
            Script(parentTarget, canBeDisabled, canBeRemoved)
        {
           scriptObject.getDynamicObject()->setMethod("noise", &ScriptEffectScript::noiseFromScript);
        }

        static var noiseFromScript(const var::NativeFunctionArgs& args)
        {
            if (args.numArguments == 2)
            {
                return getNormalisedNoiseValue((float)args.arguments[0], (float)args.arguments[1]);
            }

            if (args.numArguments < 6) return var();

            const int count = jmax(0, (int)args.arguments[0]);
            const float time = (float)args.arguments[1];
            const float scale = (float)args.arguments[2];
            const int octaves = jmax(1, (int)args.arguments[3]);
            const float amplitude = (float)args.arguments[4];
            const float offset = (float)args.arguments[5];

            var result;
            for (int i = 0; i < count; ++i)
            {
                result.append(getNormalisedNoiseValue((offset + i) * scale, time, octaves) * amplitude);
            }

            return result;
        }

        void onContainerParameterChangedInternal(Parameter* p) override
        {
            if (p == filePath)
            {
                ObjectManager::ScopedProcessSuspender processSuspender("Script effect reload");
                Script::onContainerParameterChangedInternal(p);
                return;
            }

            Script::onContainerParameterChangedInternal(p);
        }

        void onContainerTriggerTriggered(Trigger* t) override
        {
            if (t == reload)
            {
                ObjectManager::ScopedProcessSuspender processSuspender("Script effect reload");
                Script::onContainerTriggerTriggered(t);
                return;
            }

            Script::onContainerTriggerTriggered(t);
        }

        void loadJSONDataInternal(var data) override
        {
            ObjectManager::ScopedProcessSuspender processSuspender("Script effect reload");
            Script::loadJSONDataInternal(data);
        }

        void endLoadFile() override
        {
            ObjectManager::ScopedProcessSuspender processSuspender("Script effect reload");
            Script::endLoadFile();
        }

        void checkScriptFile() override
        {
            File f = filePath->getFile();
            if (f.getLastModificationTime() != fileLastModTime)
            {
                ObjectManager::ScopedProcessSuspender processSuspender("Script effect reload");
                Script::loadScript();
            }
        }
    };

    class ColourScriptData :
        public ScriptTarget
    {
    public:
        ColourScriptData(Array<Colour, CriticalSection>* sourceColors) :
            ScriptTarget("colors", this),
            sourceColors(sourceColors)
        {
            scriptObject.getDynamicObject()->setMethod("getArray", &ColourScriptData::getArrayFromScript);
            scriptObject.getDynamicObject()->setMethod("get", &ColourScriptData::getColorFromScript);
            scriptObject.getDynamicObject()->setMethod("set", &ColourScriptData::updateColorRGBFromScript);
            scriptObject.getDynamicObject()->setMethod("setHSV", &ColourScriptData::updateColorHSVFromScript);
            scriptObject.getDynamicObject()->setMethod("setColors", &ColourScriptData::updateColorsRGBFromScript);
            scriptObject.getDynamicObject()->setMethod("setColorsHSV", &ColourScriptData::updateColorsHSVFromScript);
            scriptObject.getDynamicObject()->setMethod("fill", &ColourScriptData::fillRGBFromScript);
            scriptObject.getDynamicObject()->setMethod("fillHSV", &ColourScriptData::fillHSVFromScript);
            scriptObject.getDynamicObject()->setMethod("point", &ColourScriptData::pointRGBFromScript);
            scriptObject.getDynamicObject()->setMethod("pointHSV", &ColourScriptData::pointHSVFromScript);
            scriptObject.getDynamicObject()->setMethod("gradient", &ColourScriptData::gradientRGBFromScript);
            scriptObject.getDynamicObject()->setMethod("gradientHSV", &ColourScriptData::gradientHSVFromScript);
            scriptObject.getDynamicObject()->setMethod("lerpColor", &ColourScriptData::lerpColorFromScript);
            scriptObject.getDynamicObject()->setMethod("getHSV", &ColourScriptData::getHSVColorFromScript);
        }

        Array<Colour, CriticalSection>* sourceColors;
        Array<Colour, CriticalSection> colorArray;

        static Colour getColorFromArgs(const var* a, int numArgs, int offset, bool useHSV = false)
        {
            Colour c;
            if (numArgs <= offset) return c;
            if (a[offset].isArray() && a[offset].size() >= 3)
            {
                if (useHSV) c = Colour::fromHSV((float)a[offset][0], (float)a[offset][1], (float)a[offset][2], a[offset].size() > 3 ? (float)a[offset][3] : (numArgs > offset + 1 ? (float)a[offset + 1] : 1.f));
                else c = Colour::fromFloatRGBA((float)a[offset][0], (float)a[offset][1], (float)a[offset][2], a[offset].size() > 3 ? (float)a[offset][3] : (numArgs > offset + 1 ? (float)a[offset + 1] : 1.f));
            }
            else if (numArgs >= offset + 3)
            {
                if (useHSV) c = Colour::fromHSV((float)a[offset], (float)a[offset + 1], (float)a[offset + 2], numArgs > offset + 3 ? (float)a[offset + 3] : 1.f);
                else c = Colour::fromFloatRGBA((float)a[offset], (float)a[offset + 1], (float)a[offset + 2], numArgs > offset + 3 ? (float)a[offset + 3] : 1.f);
            }

            return c;
        }

        static var updateColorRGBFromScript(const var::NativeFunctionArgs& args)
        {
            ColourScriptData* p = getObjectFromJS<ColourScriptData>(args);
            if (p == nullptr) return var();
            if (args.numArguments < 2) return var();

            int index = args.arguments[0];
            if (index < 0 || index >= p->colorArray.size()) return var();

            p->colorArray.set(index, getColorFromArgs(args.arguments, args.numArguments, 1, false));
            return var();
        }

        static var updateColorHSVFromScript(const var::NativeFunctionArgs& args)
        {
            ColourScriptData* p = getObjectFromJS<ColourScriptData>(args);
            if (p == nullptr) return var();
            if (args.numArguments < 2) return var();

            int index = args.arguments[0];
            if (index < 0 || index >= p->colorArray.size()) return var();

            p->colorArray.set(index, getColorFromArgs(args.arguments, args.numArguments, 1, true));
            return var();
        }

        static var updateColorsRGBFromScript(const var::NativeFunctionArgs& args)
        {
            ColourScriptData* p = getObjectFromJS<ColourScriptData>(args);
            if (p == nullptr) return var();
            if (args.numArguments == 0 || !args.arguments[0].isArray()) return var();

            var colors = args.arguments[0];
            const int numColors = jmin(p->colorArray.size(), colors.size());
            for (int i = 0; i < numColors; i++) p->colorArray.set(i, getColorFromArgs(&colors[i], colors[i].size(), 0, false));
            return colors;
        }

        static var updateColorsHSVFromScript(const var::NativeFunctionArgs& args)
        {
            ColourScriptData* p = getObjectFromJS<ColourScriptData>(args);
            if (p == nullptr) return var();
            if (args.numArguments == 0 || !args.arguments[0].isArray()) return var();

            var colors = args.arguments[0];
            const int numColors = jmin(p->colorArray.size(), colors.size());
            for (int i = 0; i < numColors; i++) p->colorArray.set(i, getColorFromArgs(&colors[i], colors[i].size(), 0, true));
            return colors;
        }

        static var fillRGBFromScript(const var::NativeFunctionArgs& args)
        {
            ColourScriptData* p = getObjectFromJS<ColourScriptData>(args);
            if (p == nullptr || args.numArguments == 0) return var();

            Colour c = getColorFromArgs(args.arguments, args.numArguments, 0, false);
            if (args.numArguments < 5)
            {
                p->colorArray.fill(c);
            }
            else
            {
                float start = (float)args.arguments[args.numArguments - 2];
                float end = (float)args.arguments[args.numArguments - 1];
                const int resolution = p->colorArray.size();
                if (resolution == 0) return var();

                const int s = roundToInt(jmax(jmin(start, end), 0.f) * jmax(resolution - 1, 0));
                const int e = roundToInt(jmin(jmax(start, end), 1.f) * jmax(resolution - 1, 0));
                for (int i = s; i <= e; i++) p->colorArray.set(i, c);
            }

            return var();
        }

        static var fillHSVFromScript(const var::NativeFunctionArgs& args)
        {
            ColourScriptData* p = getObjectFromJS<ColourScriptData>(args);
            if (p == nullptr || args.numArguments == 0) return var();

            Colour c = getColorFromArgs(args.arguments, args.numArguments, 0, true);
            if (args.numArguments < 5)
            {
                p->colorArray.fill(c);
            }
            else
            {
                float start = (float)args.arguments[args.numArguments - 2];
                float end = (float)args.arguments[args.numArguments - 1];
                const int resolution = p->colorArray.size();
                if (resolution == 0) return var();

                const int s = roundToInt(jmax(jmin(start, end), 0.f) * jmax(resolution - 1, 0));
                const int e = roundToInt(jmin(jmax(start, end), 1.f) * jmax(resolution - 1, 0));
                for (int i = s; i <= e; i++) p->colorArray.set(i, c);
            }

            return var();
        }

        static var pointRGBFromScript(const var::NativeFunctionArgs& args)
        {
            ColourScriptData* p = getObjectFromJS<ColourScriptData>(args);
            if (p == nullptr || args.numArguments < 3) return var();

            Colour c = getColorFromArgs(args.arguments, args.numArguments, 2, false);
            float pos = (float)args.arguments[0];
            float radius = (float)args.arguments[1];
            if (radius == 0) return var();

            const int resolution = p->colorArray.size();
            if (resolution <= 1)
            {
                if (resolution == 1) p->colorArray.set(0, p->colorArray[0].overlaidWith(c));
                return var();
            }

            for (int i = 0; i < resolution; i++)
            {
                float rel = i * 1.0f / (resolution - 1);
                float fac = jmax(1 - (std::abs(pos - rel) / radius), 0.f);
                Colour ic = c.withMultipliedAlpha(fac);
                p->colorArray.set(i, p->colorArray[i].overlaidWith(ic));
            }

            return var();
        }

        static var pointHSVFromScript(const var::NativeFunctionArgs& args)
        {
            ColourScriptData* p = getObjectFromJS<ColourScriptData>(args);
            if (p == nullptr || args.numArguments < 3) return var();

            Colour c = getColorFromArgs(args.arguments, args.numArguments, 2, false);
            float pos = (float)args.arguments[0];
            float radius = (float)args.arguments[1];
            if (radius == 0) return var();

            const int resolution = p->colorArray.size();
            if (resolution <= 1)
            {
                if (resolution == 1) p->colorArray.set(0, p->colorArray[0].overlaidWith(c));
                return var();
            }

            for (int i = 0; i < resolution; i++)
            {
                float rel = i * 1.0f / (resolution - 1);
                float fac = jmax(1 - (std::abs(pos - rel) / radius), 0.f);
                Colour ic = c.withMultipliedAlpha(fac);
                p->colorArray.set(i, p->colorArray[i].overlaidWith(ic));
            }

            return var();
        }

        static var gradientRGBFromScript(const var::NativeFunctionArgs& args)
        {
            ColourScriptData* p = getObjectFromJS<ColourScriptData>(args);
            if (p == nullptr || args.numArguments < 7) return var();

            Colour c1 = getColorFromArgs(args.arguments, args.numArguments, 2, false);
            Colour c2 = getColorFromArgs(args.arguments, args.numArguments, 6, false);
            float start = (float)args.arguments[0];
            float end = (float)args.arguments[1];

            const int resolution = p->colorArray.size();
            if (resolution == 0) return var();

            const int s = roundToInt(jmax(jmin(start, end), 0.f) * jmax(resolution - 1, 0));
            const int e = roundToInt(jmin(jmax(start, end), 1.f) * jmax(resolution - 1, 0));
            if (s == e)
            {
                p->colorArray.set(s, c1);
                return var();
            }

            for (int i = s; i <= e; i++)
            {
                float rel = (i - s) * 1.0f / (e - s);
                p->colorArray.set(i, c1.interpolatedWith(c2, rel));
            }

            return var();
        }

        static var gradientHSVFromScript(const var::NativeFunctionArgs& args)
        {
            ColourScriptData* p = getObjectFromJS<ColourScriptData>(args);
            if (p == nullptr || args.numArguments < 2) return var();

            Colour c1 = getColorFromArgs(args.arguments, args.numArguments, 0, true);
            Colour c2 = getColorFromArgs(args.arguments, args.numArguments, 1, true);
            float start = args.numArguments > 2 ? (float)args.arguments[args.numArguments - 2] : 0.f;
            float end = args.numArguments > 3 ? (float)args.arguments[args.numArguments - 1] : 1.f;

            const int resolution = p->colorArray.size();
            if (resolution == 0) return var();

            const int s = roundToInt(jmax(jmin(start, end), 0.f) * jmax(resolution - 1, 0));
            const int e = roundToInt(jmin(jmax(start, end), 1.f) * jmax(resolution - 1, 0));
            if (s == e)
            {
                p->colorArray.set(s, c1);
                return var();
            }

            for (int i = s; i <= e; i++)
            {
                float rel = (i - s) * 1.0f / (e - s);
                p->colorArray.set(i, c1.interpolatedWith(c2, rel));
            }

            return var();
        }

        static var lerpColorFromScript(const var::NativeFunctionArgs& args)
        {
            ColourScriptData* p = getObjectFromJS<ColourScriptData>(args);
            if (p == nullptr || args.numArguments < 3) return var();

            Colour c1 = getColorFromArgs(args.arguments, args.numArguments, 0, false);
            Colour c2 = getColorFromArgs(args.arguments, args.numArguments, 1, false);
            float rel = (float)args.arguments[args.numArguments - 1];

            Colour cr = c1.interpolatedWith(c2, rel);
            var result;
            result.append(cr.getFloatRed());
            result.append(cr.getFloatGreen());
            result.append(cr.getFloatBlue());
            result.append(cr.getFloatAlpha());
            return result;
        }

        static var getHSVColorFromScript(const var::NativeFunctionArgs& args)
        {
            ColourScriptData* p = getObjectFromJS<ColourScriptData>(args);
            if (p == nullptr || args.numArguments == 0) return var();

            Colour c = getColorFromArgs(args.arguments, args.numArguments, 0, false);
            var result;
            result.append(c.getHue());
            result.append(c.getSaturation());
            result.append(c.getBrightness());
            result.append(c.getFloatAlpha());
            return result;
        }

        static var getColorFromScript(const var::NativeFunctionArgs& args)
        {
            ColourScriptData* p = getObjectFromJS<ColourScriptData>(args);
            if (p == nullptr || args.numArguments == 0) return var();

            int index = args.arguments[0];
            if (p->sourceColors == nullptr || index < 0 || index >= p->sourceColors->size()) return var();

            Colour col = p->sourceColors->getUnchecked(index);
            var c;
            c.append(col.getFloatRed());
            c.append(col.getFloatGreen());
            c.append(col.getFloatBlue());
            c.append(col.getFloatAlpha());
            return c;
        }

        static var getArrayFromScript(const var::NativeFunctionArgs& args)
        {
            ColourScriptData* p = getObjectFromJS<ColourScriptData>(args);
            if (p == nullptr || p->sourceColors == nullptr) return var();

            var result;
            for (auto& col : *p->sourceColors)
            {
                var c;
                c.append(col.getFloatRed());
                c.append(col.getFloatGreen());
                c.append(col.getFloatBlue());
                c.append(col.getFloatAlpha());
                result.append(c);
            }

            return result;
        }
    };
}

ScriptEffect::ScriptEffect(var params) :
    ColorEffect(getTypeString(), params)
{
    script.reset(new ScriptEffectScript(this, false, false));
    script->setCustomShortName("script");
    script->setParamsContainer(&effectParams, false, false);
    addChildControllableContainer(script.get());
}

ScriptEffect::~ScriptEffect()
{
}


void ScriptEffect::processedEffectColorsInternal(Array<Colour, CriticalSection>& colors, Object* o, ColorComponent* c, int id, float time)
{
    if (script == nullptr || script->state != Script::SCRIPT_LOADED) return;

    ColourScriptData colorData(&colors);
    colorData.colorArray = colors;

    Array<var> args;
    args.add(colorData.getScriptObject());
    args.add(id);
    args.add(colors.size());
    args.add(time);
    args.add(o != nullptr ? o->getScriptObject() : var());
    args.add(c != nullptr ? c->getScriptObject() : var());

    var scriptResult = script->callFunction(updateColorsIdentifier, args);
    if (scriptResult.isArray())
    {
        const int numColors = jmin(colors.size(), scriptResult.size());
        for (int i = 0; i < numColors; i++)
        {
            if (!scriptResult[i].isArray() || scriptResult[i].size() < 3) continue;
            colors.set(i, Colour::fromFloatRGBA((float)scriptResult[i][0], (float)scriptResult[i][1], (float)scriptResult[i][2], scriptResult[i].size() > 3 ? (float)scriptResult[i][3] : 1.f));
        }
    }
    else
    {
        colors.swapWith(colorData.colorArray);
    }
}
