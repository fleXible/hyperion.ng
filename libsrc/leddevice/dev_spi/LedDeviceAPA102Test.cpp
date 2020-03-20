#include "LedDeviceAPA102Test.h"
#include <cmath>

LedDeviceAPA102Test::LedDeviceAPA102Test(const QJsonObject &deviceConfig)
	: ProviderSpi(deviceConfig)
	, _brightness(32)
{
}

LedDevice* LedDeviceAPA102Test::construct(const QJsonObject &deviceConfig)
{
	return new LedDeviceAPA102Test(deviceConfig);
}

bool LedDeviceAPA102Test::init(const QJsonObject &deviceConfig)
{
	bool isInitOK = false;

	// Initialise sub-class
	if ( ProviderSpi::init(deviceConfig) )
	{
		_brightness = deviceConfig["brightness"].toInt(_brightness);
		Debug(_log, "brightness=%d", _brightness);

		const unsigned int endFrameSize = qMax<unsigned int>(((_ledCount + 15) / 16), 4);
		const unsigned int APAbufferSize = (_ledCount * 4) + startFrameSize + endFrameSize;

		_ledBuffer.resize(APAbufferSize, 0xFF);
		_ledBuffer[0] = 0x00;
		_ledBuffer[1] = 0x00;
		_ledBuffer[2] = 0x00;
		_ledBuffer[3] = 0x00;

		isInitOK = true;
	}
	return isInitOK;
}

int LedDeviceAPA102Test::write(const std::vector<ColorRgb> &ledValues)
{
	// adjust range from 1-32 to 0-31 (lower 5 bits)
	const unsigned int globalBrightness = _brightness - 1;

	for (unsigned currentLed = 0; currentLed < (unsigned)_ledCount; ++currentLed) {

		const ColorRgb& color = ledValues[currentLed];
		const unsigned int pos = startFrameSize + currentLed * 4;

		// global brightness - 0xE0 (highest 3 bits)
		_ledBuffer[pos + 0] = 0xE0 | globalBrightness;

		_ledBuffer[pos + 1] = color.red;
		_ledBuffer[pos + 2] = color.green;
		_ledBuffer[pos + 3] = color.blue;
	}

	return writeBytes(_ledBuffer.size(), _ledBuffer.data());
}
