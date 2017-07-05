#pragma once

#include <Windows.h>
#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>

class Sensor {
public:
	Sensor();
	~Sensor();

	void initialize();
	void updateTextureRGB();
	void updateTextureDEPTH();

	void releaseFrameRGB();
	void releaseFrameDEPTH();
	void setCameraAngle(LONG Angle);

	INuiFrameTexture* getTextureRGB();
	INuiFrameTexture* getTextureDEPTH();
	NUI_LOCKED_RECT getLockedRectRGB();
	NUI_LOCKED_RECT getLockedRectDEPTH();
private:
	INuiSensor* kinect;
	HANDLE rgbStream;
	HANDLE depthStream;

	NUI_IMAGE_FRAME imageFrameRGB;
	NUI_IMAGE_FRAME imageFrameDEPTH;
	NUI_LOCKED_RECT lockedRectRGB;
	NUI_LOCKED_RECT lockedRectDEPTH;
};
