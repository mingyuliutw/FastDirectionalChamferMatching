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
#include "LMDirectionalIntegralDistanceImage.h"

LMDirectionalIntegralDistanceImage::LMDirectionalIntegralDistanceImage()
{	
	//iimage_->data = NULL;
	//iimage_->access = NULL;
	indices_ = NULL;
}

LMDirectionalIntegralDistanceImage::~LMDirectionalIntegralDistanceImage()
{
	SafeRelease();
}

void LMDirectionalIntegralDistanceImage::SafeRelease()
{
	if(indices_)
	{
		delete [] indices_;
	}


	//if(iimage_)
	//{
	//	//cvReleaseImage(tiimage);
	//	delete iimage_;
	//}



}

void LMDirectionalIntegralDistanceImage::CreateImage(int width, int height)
{ 
	width_ = width;
	height_ = height;
	//iimage_ = cvCreateImage(cvSize(width+1,height+1),IPL_DEPTH_32F,1);
	iimage_.Resize(width+1,height+1);
}


void LMDirectionalIntegralDistanceImage::Construct(Image<float> *image, float dx, float dy)
{
	if (abs(dx) > abs(dy))
	{
		ds_ = dy / (dx + 1e-9f);
		xindexed_ = 1;		
	}
	else
	{
		ds_ = dx / (dy + 1e-9f);
		xindexed_ = 0;
	}
	// Compute secant
	factor_ = sqrt(ds_*ds_ + 1);



	ComputeIndices();
	ComputeII(image);

}


void LMDirectionalIntegralDistanceImage::ComputeIndices()
{

	if (indices_)
	{
		delete[] indices_;
	}


	if (xindexed_)
	{
		indices_ = new int[width_];
		indices_[0] = 0;

		for (int i=0 ; i<width_;i++)
		{
			indices_[i] = (int)ceil(i*ds_-0.5);
		}
	}
	else
	{
		indices_ = new int[height_];
		indices_[0] = 0;

		for (int i=0 ; i<height_;i++)
		{
			indices_[i] = (int)ceil(i*ds_-0.5);
		}
	}
}




void LMDirectionalIntegralDistanceImage::ComputeII(Image<float>* image)
{
	int x, y;
	Image<float> *tiimage = &iimage_;
	for (x = 0 ; x <= width_ ; x++)
	{
		imRef(tiimage,x,0) = 0;
		//cvSetReal2D(iimage_,0,x,0);
		//WriteFloat(iimage_,x,0) = 0;
	}

	for (y = 0 ; y <= height_ ; y++)
	{
		imRef(tiimage,0,y) = 0;
		//cvSetReal2D(iimage_,y,0,0);
		//WriteFloat(iimage_,0,y) = 0;
	}		


	if (xindexed_)
	{
		int miny, maxy;
		int py, cy;

		if (indices_[width_-1]> 0 )
		{
			miny = -indices_[width_-1];
			maxy = height_;
		}
		else
		{
			miny = 0;
			maxy = height_-indices_[width_-1];
		}

		for (y=miny ; y<=maxy ; y++)
		{
			for (x=1 ; x<width_ ; x++)
			{
				py = y+indices_[x-1];
				cy = y+indices_[x];

				if (cy > 0 &&  cy < height_-1)
				{
					imRef(tiimage,x,cy) = imRef(tiimage,x-1,py)+imRef(image,x,cy);
					//cvSetReal2D( iimage_, cy, x,cvGetReal2D(iimage_,py,x-1) + cvGetReal2D(image,cy,x));
					//WriteFloat( iimage_, x , cy) = ReadFloat(iimage_,x-1,py) + ReadFloat(image,x,cy);
				}
			}
		}
	}
	else
	{
		int minx, maxx;
		int px, cx;

		if (indices_[height_-1]> 0 )
		{
			minx = -indices_[height_-1];
			maxx = width_;
		}
		else
		{
			minx = 0;
			maxx = width_-indices_[height_-1];
		}

		for (x=minx ; x<=maxx ; x++)
		{
			for (y=1 ; y<height_; y++)
			{
				px = x+indices_[y-1];
				cx = x+indices_[y];

				if (cx > 0 &&  cx < width_-1)
				{
					imRef(tiimage,cx,y) = imRef(tiimage,px,y-1) + imRef(image,cx,y);
					//cvSetReal2D( iimage_, y, cx,cvGetReal2D(iimage_,y-1,px) + cvGetReal2D(image,y,cx));
					//WriteFloat( iimage_, cx , y) = ReadFloat(iimage_,px,y-1) + ReadFloat(image,cx,y);
				}
			}
		}
		

	}

	
}