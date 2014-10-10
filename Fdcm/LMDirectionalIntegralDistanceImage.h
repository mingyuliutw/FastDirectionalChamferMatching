/*
Copyright 2011, Ming-Yu Liu

All Rights Reserved 

Permission to use, copy, modify, and distribute this software and 
its documentation for any non-commercial purpose is hereby granted 
without fee, provided that the above copyright notice appear in 
all copies and that both that copyright notice and this permission 
notice appear in supporting documentation, and that the name of 
the author not be used in advertising or publicity pertaining to 
distribution of the software without specific, written prior 
permission. 

THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, 
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
ANY PARTICULAR PURPOSE. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR 
ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES 
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN 
AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING 
OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
*/


#ifndef _lm_directional_integral_distance_iamge_h_
#define _lm_directional_integral_distance_iamge_h_
//#include <cxcore.h>
#include <iostream>
#include <cmath>
#include "Image/Image.h"

using namespace std;

class LMDirectionalIntegralDistanceImage
{
public:
	LMDirectionalIntegralDistanceImage();
	~LMDirectionalIntegralDistanceImage();

	void CreateImage(int width, int height);
	void Construct(Image<float> *image, float dx, float dy);
	inline float Sum(int x1, int y1, int x2, int y2, int& count);

private:
	void SafeRelease();
	void ComputeIndices();
	void ComputeII(Image<float>* image);

	Image<float> iimage_;
	float ds_;
	int xindexed_;
	int* indices_;
	float factor_;
	int width_;
	int height_;

	friend class LMDistanceImage;
};

inline float LMDirectionalIntegralDistanceImage::Sum(int x1, int y1, int x2, int y2, int& count)
{
	double value = -1;
	Image<float> *tiimage = &iimage_;

	if (xindexed_)
	{
		if (x1 <= x2)
		{
			//value = cvGetReal2D(iimage_,y1+indices_[x2]-indices_[x1], x2)- cvGetReal2D(iimage_,y1-indices_[x1]+indices_[x1-1], x1-1);
			value = imRef(tiimage,x2,y1+indices_[x2]-indices_[x1]) - imRef(tiimage,x1-1,y1-indices_[x1]+indices_[x1-1]);
			count = x2-x1+1;
		}
		else
		{
			//value = cvGetReal2D(iimage_, y2+indices_[x1]-indices_[x2], x1)- cvGetReal2D(iimage_, y2-indices_[x2]+indices_[x2-1], x2-1);
			value = imRef(tiimage,x1,y2+indices_[x1]-indices_[x2])- imRef(tiimage,x2-1,y2-indices_[x2]+indices_[x2-1]);
			count = x1-x2+1;
		}

	}
	else
	{
		if (y1 <= y2)
		{
			//value = cvGetReal2D(iimage_, y2, x1+indices_[y2]-indices_[y1])- cvGetReal2D(iimage_, y1-1, x1-indices_[y1]+indices_[y1-1]);
			value = imRef(tiimage,x1+indices_[y2]-indices_[y1],y2) - imRef(tiimage,x1-indices_[y1]+indices_[y1-1],y1-1);
			count = y2-y1+1;

		}
		else
		{
			//value = cvGetReal2D(iimage_, y1, x2+indices_[y1]-indices_[y2])- cvGetReal2D(iimage_, y2-1, x2-indices_[y2]+indices_[y2-1]);
			value = imRef(tiimage, x2+indices_[y1]-indices_[y2], y1)- imRef(tiimage, x2-indices_[y2]+indices_[y2-1], y2-1);
			count = y1-y2+1;

		}		
	}

	return (float)(value*factor_);
}

#endif