#pragma once

#include <Windows.h>
#include <NuiApi.h>
#include <opencv2\core\mat.hpp>
#include <opencv\highgui.h>
#include <iostream>

void convertNuiToMatRGB(NUI_LOCKED_RECT& lockedRect, cv::Mat3b& mat);
void convertNuiToMatDEPTH(NUI_LOCKED_RECT& lockedRect, cv::Mat4b& mat);