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

#ifndef _lm_distance_image_h_
#define _lm_distance_image_h_

#include "Image/Image.h"
#include "Image/DistanceTransform.h"
#include "EIEdgeImage.h"
#include "LMDirectionalIntegralDistanceImage.h"


class LMDistanceImage 
{
public:

	LMDistanceImage();
	~LMDistanceImage();
	void Configure(float directionCost,double maxCost){ directionCost_ = directionCost; maxCost_ = maxCost; };
	void SetImage(EIEdgeImage& ei);
	

private:

	void SafeRelease();
	void ConstructDTs(EIEdgeImage& ei);
	void UpdateCosts();
	void ConstructDIntegrals();
	
	vector<LMDirectionalIntegralDistanceImage> idtImages_;
	int nDirections_;
	int width_;
	int height_;
	
	vector< Image<float> > dtImages_;
	float directionCost_;
	double maxCost_;
	friend class LMLineMatcher;
};

#endif