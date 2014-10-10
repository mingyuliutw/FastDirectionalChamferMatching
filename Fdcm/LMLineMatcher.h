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


#ifndef _lm_line_matcher_h_
#define _lm_line_matcher_h_


#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <vector>
#include "Fitline/LFLineFitter.h"
#include "EIEdgeImage.h"
#include "LMDistanceImage.h"
#include "LMDetWind.h"
#include "LMDisplay.h"
#include "LMNonMaximumSuppression.h"
#include "MatchingCostMap.h"
//#include <windows.h>

using namespace std;

#define MAX_NUM_HYPOTHESES 5000000

class LMLineMatcher {
public:
	LMLineMatcher();
	~LMLineMatcher();

	// Initialization
	void Configure(const char* filename);



	void PrintParameter();

	void Init(const char* fileName);

	// find the best matcher
	void Match(LFLineFitter &lf,vector<LMDetWind> &detWind);

	// compute detection windows
	//void Detect(LFLineFitter &lf,double detectionThreshold,vector<LMDetWind> &detWinds);

	// compute detection
	void SingleShapeDetectionWithVaryingQuerySize(LFLineFitter &lf,double maxThreshold,vector< vector<LMDetWind> > &detWindArrays);


	// compute ROC curves
	void SingleShapeDetectionWithVaryingQuerySizeForROC(LFLineFitter &lf,double minThreshold,double gap,double maxThreshold,vector< vector<LMDetWind> > &detWindArrays);
	void SingleShapeDetectionWithVaryingTemplateSizeForROC(LFLineFitter &lf,double minThreshold,double gap,double maxThreshold,vector< vector<LMDetWind> > &detWindArrays);


	// compute the matching cost map (no early termination).
	void MatchCostMap(LFLineFitter &lf,const char *filename);


private:
	void SafeRelease();

	// brute force scan
	void DetectBruteForce(EIEdgeImage& dbImage, double detectionThreshold, double scale, MatchingCostMap &matchingCostMap,int &costMapIndex);

	// brute force scan with varying template size
	void DetectBruteForceVaryingTemplateSize(EIEdgeImage& dbImage, double detectionThreshold,MatchingCostMap &matchingCostMap);

	// brute force scan.
	double MatchBruteForce(EIEdgeImage& dbImage, int index, int* iindices, int* indices, int* xIndex, int* yIndex, double* dIndex, double* sIndex, double* distances, int& counter, double& minCost);

	// brute force scan (no eraly termination)
	double MatchBruteForceCostMap(EIEdgeImage& dbImage,vector<double> &costMap);

	// the query image
	EIEdgeImage	queryImage_;
	LMDistanceImage queryDistanceImage_;

public:
	// the tempalte set
	int ndbImages_;
	EIEdgeImage* dbImages_;	

	// matching parameter
	int nDirections_;	
	float directionCost_;
	double maxCost_;	

	double bias_;
	double scale_;
	double db_scale_;

	// search paperters
	double minCostRatio_;

	double baseSearchScale_;
	int minSearchScale_;
	int maxSearchScale_;

	double baseSearchAspect_;
	int minSearchAspect_;
	int maxSearchAspect_;

	int searchStepSize_;
	int searchBoundarySize_;

};

#endif