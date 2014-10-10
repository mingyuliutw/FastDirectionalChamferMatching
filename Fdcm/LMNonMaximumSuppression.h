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

#ifndef _lm_non_maximum_suppression_h_
#define _lm_non_maximum_suppression_h_

#include "MatchingCostMap.h"
#include "LMDetWind.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
using namespace std;

class LMNonMaximumSuppression
{
public:

	// Use the detection threshold and the overlapping threshold to find the local minima in the matching cost map.
	static void ComputeDetection(MatchingCostMap &matchingCostMap,double threshold,double overlapThresh,vector<LMDetWind> &wind,int varyingQuerySize=1);

	// Check if the current window ( curWind ) has a significant overlap with any previous windows stored in wind.
	static bool IsOverlapping(LMDetWind &curWind,vector<LMDetWind> &wind,double overlapThresh);

	// Compute the overlapping ratio, the intersection region over the union region, between the two detection windows.
	static double OverlapRatio(LMDetWind &curWind,LMDetWind &refWind);

	// Check which windows are valid detections and construct the window dimension for varying query image size.
	static void ComputeValidWindVaryingQuerySize(MatchingCostMap &matchingCostMap,vector<LMDetWind> &detWinds,double threshold);

	// Check which windows are valid detections and construct the window dimension for varying template size.
	static void ComputeValidWindVaryingTemplateSize(MatchingCostMap &matchingCostMap,vector<LMDetWind> &detWinds,double threshold);

};

#endif