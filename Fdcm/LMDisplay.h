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


#ifndef _lm_display_h_
#define _lm_display_h_

#include "Image/Image.h"
#include "Image/ImageIO.h"
#include "Image/ImageDraw.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include "LMDetWind.h"
#include "EIEdgeImage.h"
using namespace std;

class LMDisplay
{
public:

	static void DrawDetWind(Image<RGBMap> *image,int x,int y,int detWindWidth,int detWindHeight,const RGBMap scalar,int thickness=1);
	static void DrawDetWindCost(Image<RGBMap> *image,LMDetWind &wind,const RGBMap scalar,int thickness=2);
	static void DrawDetWindWind(Image<RGBMap> *image,LMDetWind &wind,const RGBMap scalar,int thickness=2);
	static void DrawMatchTemplate(Image<RGBMap> *image,EIEdgeImage &ei,int x,int y,double scale,const RGBMap scalar,int thickness=1);


	static void StoreDetWind(const char *filename,vector< vector<LMDetWind> > &detWindArrays);

};


#endif