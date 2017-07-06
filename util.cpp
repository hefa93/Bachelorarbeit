#include <util.h>

void convertNuiToMatRGB(NUI_LOCKED_RECT& lockedRect, cv::Mat3b& mat) {
	if (lockedRect.Pitch != 0)
	{
		BYTE* upperLeftCorner = (BYTE*)lockedRect.pBits;
		BYTE* pointerToTheByteBeingRead = upperLeftCorner;

		for (int i = 0; i < 480; i++)
		{
			cv::Vec3b *pointerToRow = mat.ptr<cv::Vec3b>(i);

			for (int j = 0; j < 640; j++)
			{
				unsigned char r = *pointerToTheByteBeingRead;
				pointerToTheByteBeingRead += 1;
				unsigned char g = *pointerToTheByteBeingRead;
				pointerToTheByteBeingRead += 1;
				unsigned char b = *pointerToTheByteBeingRead;
				pointerToTheByteBeingRead += 2; // Skip the alpha channel
				pointerToRow[j] = cv::Vec3b(r, g, b);
			}

		}
		flip(mat, mat, 1);
	}
}

void convertNuiToMatDEPTH(NUI_LOCKED_RECT& lockedRect, cv::Mat4b& mat) {
	if (lockedRect.Pitch != 0)
	{
		const USHORT* upperLeftCorner = (USHORT*)lockedRect.pBits;
		const USHORT* pointerToTheByteBeingRead = upperLeftCorner;
		int x = 1;
		USHORT depth;

		for (int i = 0; i < 480; i++)
		{
			cv::Vec4b *pointerToRow = mat.ptr<cv::Vec4b>(i);

			for (int j = 0; j < 640; j++)
			{
				depth = NuiDepthPixelToDepth(*pointerToTheByteBeingRead);

				unsigned char r = (depth*x)%256;
				
				unsigned char g = r;
				
				unsigned char b = r;
				
				unsigned char alpha = 0xFF ;

				pointerToTheByteBeingRead+=1;
				
				pointerToRow[j] = cv::Vec4b(r, g, b, alpha);

				if (i == 250 & j == 300) {

					std::cout << depth*x;
					std::cout << "depth\n";

				}
			}
		
			

		}
		flip(mat, mat, 1);
	}
}













