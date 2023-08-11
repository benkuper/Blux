/*
  ==============================================================================

	ColorHelpers.cpp
	Created: 11 Aug 2023 5:07:09pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/CommonIncludes.h"


HashMap<int, Colour> ColorHelpers::temperatureColorMap;

void ColorHelpers::init()
{
	temperatureColorMap.set(1000, Colour(255, 56, 0));
	temperatureColorMap.set(1100, Colour(255, 71, 0));
	temperatureColorMap.set(1200, Colour(255, 83, 0));
	temperatureColorMap.set(1300, Colour(255, 93, 0));
	temperatureColorMap.set(1400, Colour(255, 101, 0));
	temperatureColorMap.set(1500, Colour(255, 109, 0));
	temperatureColorMap.set(1600, Colour(255, 115, 0));
	temperatureColorMap.set(1700, Colour(255, 121, 0));
	temperatureColorMap.set(1800, Colour(255, 126, 0));
	temperatureColorMap.set(1900, Colour(255, 131, 0));
	temperatureColorMap.set(2000, Colour(255, 138, 18));
	temperatureColorMap.set(2100, Colour(255, 142, 33));
	temperatureColorMap.set(2200, Colour(255, 147, 44));
	temperatureColorMap.set(2300, Colour(255, 152, 54));
	temperatureColorMap.set(2400, Colour(255, 157, 63));
	temperatureColorMap.set(2500, Colour(255, 161, 72));
	temperatureColorMap.set(2600, Colour(255, 165, 79));
	temperatureColorMap.set(2700, Colour(255, 169, 87));
	temperatureColorMap.set(2800, Colour(255, 173, 94));
	temperatureColorMap.set(2900, Colour(255, 177, 101));
	temperatureColorMap.set(3000, Colour(255, 180, 107));
	temperatureColorMap.set(3100, Colour(255, 184, 114));
	temperatureColorMap.set(3200, Colour(255, 187, 120));
	temperatureColorMap.set(3300, Colour(255, 190, 126));
	temperatureColorMap.set(3400, Colour(255, 193, 132));
	temperatureColorMap.set(3500, Colour(255, 196, 137));
	temperatureColorMap.set(3600, Colour(255, 199, 143));
	temperatureColorMap.set(3700, Colour(255, 201, 148));
	temperatureColorMap.set(3800, Colour(255, 204, 153));
	temperatureColorMap.set(3900, Colour(255, 206, 159));
	temperatureColorMap.set(4000, Colour(255, 209, 163));
	temperatureColorMap.set(4100, Colour(255, 211, 168));
	temperatureColorMap.set(4200, Colour(255, 213, 173));
	temperatureColorMap.set(4300, Colour(255, 215, 177));
	temperatureColorMap.set(4400, Colour(255, 217, 182));
	temperatureColorMap.set(4500, Colour(255, 219, 186));
	temperatureColorMap.set(4600, Colour(255, 221, 190));
	temperatureColorMap.set(4700, Colour(255, 223, 194));
	temperatureColorMap.set(4800, Colour(255, 225, 198));
	temperatureColorMap.set(4900, Colour(255, 227, 202));
	temperatureColorMap.set(5000, Colour(255, 228, 206));
	temperatureColorMap.set(5100, Colour(255, 230, 210));
	temperatureColorMap.set(5200, Colour(255, 232, 213));
	temperatureColorMap.set(5300, Colour(255, 233, 217));
	temperatureColorMap.set(5400, Colour(255, 235, 220));
	temperatureColorMap.set(5500, Colour(255, 236, 224));
	temperatureColorMap.set(5600, Colour(255, 238, 227));
	temperatureColorMap.set(5700, Colour(255, 239, 230));
	temperatureColorMap.set(5800, Colour(255, 240, 233));
	temperatureColorMap.set(5900, Colour(255, 242, 236));
	temperatureColorMap.set(6000, Colour(255, 243, 239));
	temperatureColorMap.set(6100, Colour(255, 244, 242));
	temperatureColorMap.set(6200, Colour(255, 245, 245));
	temperatureColorMap.set(6300, Colour(255, 246, 247));
	temperatureColorMap.set(6400, Colour(255, 248, 251));
	temperatureColorMap.set(6500, Colour(255, 249, 253));
	temperatureColorMap.set(6600, Colour(254, 249, 255));
	temperatureColorMap.set(6700, Colour(252, 247, 255));
	temperatureColorMap.set(6800, Colour(249, 246, 255));
	temperatureColorMap.set(6900, Colour(247, 245, 255));
	temperatureColorMap.set(7000, Colour(245, 243, 255));
	temperatureColorMap.set(7100, Colour(243, 242, 255));
	temperatureColorMap.set(7200, Colour(240, 241, 255));
	temperatureColorMap.set(7300, Colour(239, 240, 255));
	temperatureColorMap.set(7400, Colour(237, 239, 255));
	temperatureColorMap.set(7500, Colour(235, 238, 255));
	temperatureColorMap.set(7600, Colour(233, 237, 255));
	temperatureColorMap.set(7700, Colour(231, 236, 255));
	temperatureColorMap.set(7800, Colour(230, 235, 255));
	temperatureColorMap.set(7900, Colour(228, 234, 255));
	temperatureColorMap.set(8000, Colour(227, 233, 255));
	temperatureColorMap.set(8100, Colour(225, 232, 255));
	temperatureColorMap.set(8200, Colour(224, 231, 255));
	temperatureColorMap.set(8300, Colour(222, 230, 255));
	temperatureColorMap.set(8400, Colour(221, 230, 255));
	temperatureColorMap.set(8500, Colour(220, 229, 255));
	temperatureColorMap.set(8600, Colour(218, 229, 255));
	temperatureColorMap.set(8700, Colour(217, 227, 255));
	temperatureColorMap.set(8800, Colour(216, 227, 255));
	temperatureColorMap.set(8900, Colour(215, 226, 255));
	temperatureColorMap.set(9000, Colour(214, 225, 255));
	temperatureColorMap.set(9100, Colour(212, 225, 255));
	temperatureColorMap.set(9200, Colour(211, 224, 255));
	temperatureColorMap.set(9300, Colour(210, 223, 255));
	temperatureColorMap.set(9400, Colour(209, 223, 255));
	temperatureColorMap.set(9500, Colour(208, 222, 255));
	temperatureColorMap.set(9600, Colour(207, 221, 255));
	temperatureColorMap.set(9700, Colour(207, 221, 255));
	temperatureColorMap.set(9800, Colour(206, 220, 255));
	temperatureColorMap.set(9900, Colour(205, 220, 255));
	temperatureColorMap.set(10000, Colour(207, 218, 255));
	temperatureColorMap.set(10100, Colour(207, 218, 255));
	temperatureColorMap.set(10200, Colour(206, 217, 255));
	temperatureColorMap.set(10300, Colour(205, 217, 255));
	temperatureColorMap.set(10400, Colour(204, 216, 255));
	temperatureColorMap.set(10500, Colour(204, 216, 255));
	temperatureColorMap.set(10600, Colour(203, 215, 255));
	temperatureColorMap.set(10700, Colour(202, 215, 255));
	temperatureColorMap.set(10800, Colour(202, 214, 255));
	temperatureColorMap.set(10900, Colour(201, 214, 255));
	temperatureColorMap.set(11000, Colour(200, 213, 255));
	temperatureColorMap.set(11100, Colour(200, 213, 255));
	temperatureColorMap.set(11200, Colour(199, 212, 255));
	temperatureColorMap.set(11300, Colour(198, 212, 255));
	temperatureColorMap.set(11400, Colour(198, 212, 255));
	temperatureColorMap.set(11500, Colour(197, 211, 255));
	temperatureColorMap.set(11600, Colour(197, 211, 255));
	temperatureColorMap.set(11700, Colour(197, 210, 255));
	temperatureColorMap.set(11800, Colour(196, 210, 255));
	temperatureColorMap.set(11900, Colour(195, 210, 255));
	temperatureColorMap.set(12000, Colour(195, 209, 255));
}

var ColorHelpers::getRGBWFromRGB(Colour col, int temperature)
{
	if (temperatureColorMap.size() == 0) init();

	int temp = jlimit(1000, 12000, temperature);
	temp = roundToInt(temp / 100) * 100;

	// Constructor
	// 
	Colour tempColor = temperatureColorMap[temp];

	float r = col.getFloatRed();
	float g = col.getFloatGreen();
	float b = col.getFloatBlue();

	float tempRed = tempColor.getFloatRed();
	float tempGreen = tempColor.getFloatGreen();
	float tempBlue = tempColor.getFloatBlue();

	// Calculate all of the color's white values corrected taking into account the white color temperature.
	float wRed = r * 1 / tempRed;
	float wGreen = g * 1 / tempGreen;
	float wBlue = b * 1 / tempBlue;

	// Determine the smallest white value from above.
	float wMin = jmin(wRed, jmin(wGreen, wBlue));

	// Make the color with the smallest white value to be the output white value
	float wOut;
	if (wMin == wRed)  wOut = r;
	else if (wMin == wGreen)  wOut = g;
	else wOut = b;

	// Calculate the output red, green and blue values, taking into account the white color temperature.
	float rOut = r - wOut * tempRed;
	float gOut = g - wOut * tempGreen;
	float bOut = b - wOut * tempBlue;

	var result;
	result.append(rOut);
	result.append(gOut);
	result.append(bOut);
	result.append(wOut);
	result.append(0);

	return result;
}


var ColorHelpers::getRGBWAFromRGB(Colour col)
{
	var result;
	result.append(col.getFloatRed());
	result.append(col.getFloatGreen());
	result.append(col.getFloatBlue());
	result.append(0);
	result.append(0);

	return result;
}
