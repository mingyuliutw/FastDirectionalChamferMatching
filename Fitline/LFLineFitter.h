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
#ifndef _lf_line_fitter_h_
#define _lf_line_fitter_h_

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <map>
//#include <cxcore.h>
//#include <highgui.h>

#include "LFLineSegment.h"
#include "Fdcm/MMFunctions.h"
#include "Image/Image.h"
#include "Image/ImageIO.h"
#include "Image/ImageDraw.h"

using namespace std;
#define LF_NUM_LAYER 2

class LFLineFitter
{
public:
	LFLineFitter();
	~LFLineFitter();

	// configuration and initialization
	void Init();
	void PrintParameter();
	void Configure(const char *filename);
	void Configure(double sigmaFitALine,double sigmaFindSupport,double maxGap,int nLayer,int *nLinesToFitInStage,int *nTrialsPerLineInStage);


	void FitLine(Image<uchar> *inputImage);


	Image<uchar> * ComputeOuputLineImage(Image<uchar> *inputImage);
	void DisplayEdgeMap(Image<uchar> *inputImage,const char *outputImageName=NULL);
	void SaveEdgeMap(const char *filename);
	void LoadEdgeMap(const char *filename);


	int	rWidth() {return width_;};
	int rHeight() {return height_;};
	int rNLineSegments() {return nLineSegments_;};
	LFLineSegment* rOutputEdgeMap() {return outEdgeMap_;};

	LFLineSegment *outEdgeMap_;

private:

	int FitALine(const int nWindPoints,Point<int> *windPoints,const double sigmaFitALine,Point<double> &lnormal);
	int SampleAPixel(map<int,Point<int> > *edgeMap,Image<unsigned char> *inputImage,int nPixels);
	void FindSupport(const int nWindPoints,Point<int> *windPoints,Point<double> &lnormal,double sigmaFindSupport,double maxGap,LFLineSegment &ls,Point<int> *proposedKillingList,int &nProposedKillingList,int x0,int y0);
	void Find(int x0,int y0,Point<int> *windPoints,int &nWindPoints,Image<unsigned char> *inputImage,int localWindSize);
	void Find(map<int,Point<int> > *edgeMap,int x0,int y0,Point<int> *windPoints,int &nWindPoints,Image<unsigned char> *inputImage,int localWindSize);
	void SafeRelease();

private:
	int width_;
	int height_;

	// Output
	
	int nLineSegments_;
	int nInputEdges_;


	// Fitting parameters
	int nLinesToFitInStage_[LF_NUM_LAYER];
	int nTrialsPerLineInStage_[LF_NUM_LAYER];
	double sigmaFitALine_;
	double sigmaFindSupport_;
	double maxGap_;
	int minLength_;


	// Program parameters
	int nMaxWindPoints_;
	int nMinEdges_;
	int localWindSize_;
	int smallLocalWindowSize_;

	// temporary storage use
	Point<int> *rpoints_;
	double *rProjection_;
	double *absRProjection_;
	int *idx_;
};

#endif