#ifndef _IMAGE_BINARY_H_
#define _IMAGE_BINARY_H_

#include "header.h"

#define CV_MAX_HASH_SIZE 256

namespace Binary
{
	class ImageBinary
	{
	public:
		ImageBinary(IplImage*);
		~ImageBinary();
		IplImage* image_binary();

	private:
		IplImage* img_tmp;
		IplImage* image_input;

		int ostu(IplImage* img);
		//int key;
		int Hash[CV_MAX_HASH_SIZE];
		//int pixel_sum, pixel_average;
		//int threshold, fg_sum, bg_sum;
		//int fg_num, bg_num, pixel_num;

	};
}

#endif /*_IMAGE_BINARY_H_*/