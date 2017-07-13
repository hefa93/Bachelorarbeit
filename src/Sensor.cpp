#include <Sensor.h>
#include <stdexcept>

Sensor::Sensor() {}

Sensor::~Sensor() {
	kinect->Release();
}

void Sensor::initialize() {


	// Get a working kinect sensor
	int numSensors;
	NuiGetSensorCount(&numSensors);
	NuiCreateSensorByIndex(0, &kinect);

	if (kinect == nullptr) {
		throw std::runtime_error("Cannot initialize kinect");
	}

	// Initialize sensor
	kinect->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR);
	
	if (kinect->NuiImageStreamOpen(
		NUI_IMAGE_TYPE_COLOR,            // Depth camera or rgb camera?
		NUI_IMAGE_RESOLUTION_640x480,    // Image resolution
		0,        // Image stream flags, e.g. near mode
		2,        // Number of frames to buffer
		NULL,   // Event handle
		&rgbStream) != S_OK)
	{
	}
	if (kinect->NuiImageStreamOpen(
		NUI_IMAGE_TYPE_DEPTH,            // Depth camera or rgb camera?
		NUI_IMAGE_RESOLUTION_640x480,    // Image resolution
		0,        // Image stream flags, e.g. near mode
		2,        // Number of frames to buffer
		NULL,   // Event handle
		&depthStream) != S_OK)
	{
	}
}

void Sensor::updateTextureRGB() {
	HRESULT hr = E_NUI_FRAME_NO_DATA;
	while (hr == E_NUI_FRAME_NO_DATA) {
		hr = kinect->NuiImageStreamGetNextFrame(rgbStream, 0, &imageFrameRGB);
	}
}

void Sensor::updateTextureDEPTH() {
	HRESULT hr = E_NUI_FRAME_NO_DATA;
	while (hr == E_NUI_FRAME_NO_DATA) {
		hr = kinect->NuiImageStreamGetNextFrame(depthStream, 0, &imageFrameDEPTH);
	}
}

INuiFrameTexture* Sensor::getTextureRGB() {
	return imageFrameRGB.pFrameTexture;
}

INuiFrameTexture* Sensor::getTextureDEPTH() {
	return imageFrameDEPTH.pFrameTexture;
}

NUI_LOCKED_RECT Sensor::getLockedRectRGB() {
	return lockedRectRGB;
}

NUI_LOCKED_RECT Sensor::getLockedRectDEPTH() {
	return lockedRectDEPTH;
}

void Sensor::releaseFrameRGB() {
	kinect->NuiImageStreamReleaseFrame(rgbStream, &imageFrameRGB);
}

void Sensor::releaseFrameDEPTH() {
	kinect->NuiImageStreamReleaseFrame(depthStream, &imageFrameDEPTH);
}

void Sensor::setCameraAngle(LONG Angle) {
	NuiCameraElevationSetAngle(Angle);
};


