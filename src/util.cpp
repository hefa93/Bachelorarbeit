#include <util.h>

void convertNuiToMat(NUI_LOCKED_RECT& lockedRect, Mat3b& mat) {
	if (lockedRect.Pitch != 0)
	{
		BYTE* upperLeftCorner = (BYTE*)lockedRect.pBits;
		BYTE* pointerToTheByteBeingRead = upperLeftCorner;

		for (int i = 0; i < 480; i++)
		{
			Vec3b *pointerToRow = mat.ptr<Vec3b>(i);

			for (int j = 0; j < 640; j++)
			{
				unsigned char r = *pointerToTheByteBeingRead;
				pointerToTheByteBeingRead += 1;
				unsigned char g = *pointerToTheByteBeingRead;
				pointerToTheByteBeingRead += 1;
				unsigned char b = *pointerToTheByteBeingRead;
				pointerToTheByteBeingRead += 2; // Skip the alpha channel
				pointerToRow[j] = Vec3b(r, g, b);
			}
		}
	}
}