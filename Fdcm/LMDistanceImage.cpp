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
#include "LMDistanceImage.h"

LMDistanceImage::LMDistanceImage()
{

}

LMDistanceImage::~LMDistanceImage()
{
	SafeRelease();
}


void LMDistanceImage::SafeRelease()
{
	dtImages_.clear();
	idtImages_.clear();
}


void LMDistanceImage::SetImage(EIEdgeImage& ei)
{
	SafeRelease();
	width_ = ei.width_;
	height_ = ei.height_;
	nDirections_ = ei.nDirections_;

	ConstructDTs(ei);
	UpdateCosts();
	ConstructDIntegrals();
}

void LMDistanceImage::ConstructDTs(EIEdgeImage& ei)
{
	Image<uchar> image(width_,height_,false);
	dtImages_.resize(nDirections_);

	for (int i=0 ; i<ei.nDirections_ ; i++)
	{
		dtImages_[i].Resize(width_,height_,false);
		ei.ConstructDirectionImage(i, &image);
		DistanceTransform::CompDT(&image, &dtImages_[i], false);
	}
}


void LMDistanceImage::UpdateCosts()
{
	float* costs;
	costs = new float[nDirections_];

	float **buffers = new float*[nDirections_];
	for (int i=0;i<nDirections_ ; i++)
	{
		buffers[i] = (float*) dtImages_[i].data;
	}
	
	
	int wh = width_*height_;
	for (int k=0 ; k<wh ; k++)
	{
		for (int i=0 ; i<nDirections_ ; i++)
		{
			costs[i] = buffers[i][k];
			if (costs[i] > maxCost_)
				costs[i] = (float)maxCost_;
		}

		//forward pass
		if (costs[0] > costs[nDirections_-1] + directionCost_)
			costs[0] = costs[nDirections_-1] + directionCost_;
		for (int i=1 ; i<nDirections_ ; i++)
		{
			if (costs[i] > costs[i-1] + directionCost_)
				costs[i] = costs[i-1] + directionCost_;
		}

		if (costs[0] > costs[nDirections_-1] + directionCost_)
			costs[0] = costs[nDirections_-1] + directionCost_;
		for (int i=1 ; i<nDirections_ ; i++)
		{
			if (costs[i] > costs[i-1] + directionCost_)
				costs[i] = costs[i-1] + directionCost_;
			else
				break;
		}

		//backward pass
		if (costs[nDirections_-1] > costs[0] + directionCost_)
			costs[nDirections_-1] = costs[0] + directionCost_;
		for (int i=nDirections_-1 ; i>0 ; i--)
		{
			if (costs[i-1] > costs[i] + directionCost_)
				costs[i-1] = costs[i] + directionCost_;
		}

		if (costs[nDirections_-1] > costs[0] + directionCost_)
			costs[nDirections_-1] = costs[0] + directionCost_;
		for (int i=nDirections_-1 ; i>0 ; i--)
		{
			if (costs[i-1] > costs[i] + directionCost_)
				costs[i-1] = costs[i] + directionCost_;
			else
				break;
		}

		for (int i=0 ; i<nDirections_ ; i++)
		{
			buffers[i][k] =  costs[i];
		}

	}

	delete[] costs;
	delete[] buffers;
}


void LMDistanceImage::ConstructDIntegrals()
{
	double theta;
	idtImages_.resize(nDirections_);
	for (int i=0 ; i<nDirections_ ; i++)
	{
		theta = (i*M_PI)/nDirections_ + M_PI/(2*nDirections_);
		idtImages_[i].CreateImage(width_,height_);		
		idtImages_[i].Construct(&dtImages_[i], (float)cos(theta), (float)sin(theta));
	}
}