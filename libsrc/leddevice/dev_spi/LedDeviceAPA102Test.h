#pragma once

// hyperion includes
#include "ProviderSpi.h"


///
/// Implementation of the LedDevice interface for writing to APA102 led device.
///
class LedDeviceAPA102Test : public ProviderSpi
{
public:
	static const unsigned int startFrameSize = 4;

	///
	/// Constructs specific LedDevice
	///
	explicit LedDeviceAPA102Test(const QJsonObject &deviceConfig);

	///
	/// @brief Constructs the LED-device
	///
	/// @param[in] deviceConfig Device's configuration as JSON-Object
	/// @return LedDevice constructed
	static LedDevice* construct(const QJsonObject &deviceConfig);

private:
	
	///
	/// @brief Initialise the device's configuration
	///
	/// @param[in] deviceConfig the JSON device configuration
	/// @return True, if success
	///
	bool init(const QJsonObject &deviceConfig) override;

	///
	/// @brief Writes the RGB-Color values to the LEDs.
	///
	/// @param[in] ledValues The RGB-color per LED
	/// @return Zero on success, else negative
	///
	int write(const std::vector<ColorRgb> & ledValues) override;

	/// global led brightness level from 1-32
	int _brightness;
};
