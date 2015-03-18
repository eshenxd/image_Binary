#include "image_Binary.h"

using namespace Binary;

ImageBinary::ImageBinary(IplImage* src)
{
	
	image_input = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
	cvCopy(src, image_input);

	img_tmp = cvCreateImage(cvGetSize(src), 8, 1);

	for (int i = 0; i < CV_MAX_HASH_SIZE; ++i)
	{
		Hash[i] = 0;
	}

}

ImageBinary::~ImageBinary()
{
	if (img_tmp != NULL)
		cvReleaseImage(&img_tmp);
}

IplImage* ImageBinary::image_binary()
{
	//IplImage *img_tmp;
	//int x, y, key;
	//int Hash[CV_MAX_HASH_SIZE];
	img_tmp = cvCreateImage(cvGetSize(image_input), 8, 1);
	//转换成灰度图
	cvCvtColor(image_input, img_tmp, CV_RGB2GRAY);

	
	//计算二值化的阀值
	//for (x = 0; x < CV_MAX_HASH_SIZE; ++x)
	//	Hash[x] = 0;
	//int pixel_sum = 0, pixel_average = 0;
	////图像像素的灰度值总和，图像像素的总平均灰度
	//for (x = 0; x < image_input->height; ++x)
	//{
	//	uchar * temp = (unsigned char *)(img_tmp->imageData + x*img_tmp->widthStep);
	//	for (y = 0; y < image_input->width; ++y)
	//	{


	//		pixel_sum += temp[y];
	//		++Hash[(int)temp[y]];
	//	}
	//}
	//pixel_average = pixel_sum / (image_input->height*image_input->depth);
	////阀值：threshold 前景像素灰度总和：fg_sum 前景像素灰度总和：bg_sum 背景像素灰度总和
	//int threshold = 0, fg_sum, bg_sum;
	//int fg_num, bg_num, pixel_num;
	//pixel_num = fg_num = image_input->height*image_input->width;
	//fg_sum = pixel_sum;
	//bg_num = bg_sum = 0;
	//double deltaMax = 0.0;
	//for (key = 0; key < CV_MAX_HASH_SIZE; ++key)
	//{
	//	double deltaTmp;
	//	double w0, w1, u0, u1;
	//	bg_num += Hash[key];
	//	bg_sum += key*Hash[key];
	//	fg_num -= Hash[key];
	//	fg_sum -= key*Hash[key];
	//	if (fg_num != 0 && bg_num != 0)
	//	{
	//		w0 = 1.0*fg_num / pixel_num;
	//		w1 = 1.0*bg_num / pixel_num;
	//		u0 = 1.0*fg_sum / fg_num;
	//		u1 = 1.0*bg_sum / bg_num;
	//		deltaTmp = w0*w1*(u0 - u1)*(u0 - u1);
	//	}
	//	if (deltaTmp > deltaMax)
	//	{
	//		deltaMax = deltaTmp;
	//		threshold = key;
	//	}
	//}
	//均值补偿二值化（OSTU）
	int threshold = ostu(img_tmp);
	cvThreshold(img_tmp, img_tmp, threshold, 255, CV_THRESH_BINARY);
	return img_tmp;
}

/*
 *   均值补偿二值化（OSTU）
 */
int Binary::ImageBinary::ostu(IplImage* img)
{
	int pixel_sum = 0, pixel_average = 0;
	//图像像素的灰度值总和，图像像素的总平均灰度
	for (int x = 0; x < img->height; ++x)
	{
		uchar * temp = (unsigned char *)(img->imageData + x*img->widthStep);
		for (int y = 0; y < img->width; ++y)
		{


			pixel_sum += temp[y];
			++Hash[(int)temp[y]];
		}
	}
	pixel_average = pixel_sum / (image_input->height*image_input->depth);
	//阀值：threshold 前景像素灰度总和：fg_sum 前景像素灰度总和：bg_sum 背景像素灰度总和
	int threshold = 0, fg_sum, bg_sum;
	int fg_num, bg_num, pixel_num;
	pixel_num = fg_num = image_input->height*image_input->width;
	fg_sum = pixel_sum;
	bg_num = bg_sum = 0;
	double deltaMax = 0.0;
	for (int key = 0; key < CV_MAX_HASH_SIZE; ++key)
	{
		double deltaTmp=0;
		double w0, w1, u0, u1;
		bg_num += Hash[key];
		bg_sum += key*Hash[key];
		fg_num -= Hash[key];
		fg_sum -= key*Hash[key];
		if (fg_num != 0 && bg_num != 0)
		{
			w0 = 1.0*fg_num / pixel_num;
			w1 = 1.0*bg_num / pixel_num;
			u0 = 1.0*fg_sum / fg_num;
			u1 = 1.0*bg_sum / bg_num;
			deltaTmp = w0*w1*(u0 - u1)*(u0 - u1);
		}
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			threshold = key;
		}
	}

	return threshold;
}