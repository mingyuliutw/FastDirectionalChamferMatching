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
#include "LMDisplay.h"

void LMDisplay::DrawDetWind(Image<RGBMap> *image,int x,int y,int detWindWidth,int detWindHeight,const RGBMap scalar,int thickness)
{
	ImageDraw<RGBMap>::Line(image,x,y,x+detWindWidth,y,scalar,thickness);
	ImageDraw<RGBMap>::Line(image,x+detWindWidth,y,x+detWindWidth, y+detWindHeight,scalar,thickness);
	ImageDraw<RGBMap>::Line(image,x+detWindWidth,y+detWindHeight,x, y+detWindHeight,scalar,thickness);
	ImageDraw<RGBMap>::Line(image,x, y+detWindHeight,x, y,scalar,thickness);
}

void LMDisplay::DrawMatchTemplate(Image<RGBMap> *image,EIEdgeImage &ei,int x,int y,double scale,const RGBMap scalar,int thickness)
{
	EIEdgeImage tdbImage;
	tdbImage = ei;
	tdbImage.Scale(scale);
	LFLineSegment line;
	double ltrans[2];
	ltrans[0] = 1.0*x;
	ltrans[1] = 1.0*y;
	for (int k=0 ; k<tdbImage.nLines_ ; k++)
	{
		line = tdbImage.lines_[k];
		line.Translate(ltrans);	
		ImageDraw<RGBMap>::Line(image,(int)line.sx_,(int)line.sy_,(int)line.ex_,(int)line.ey_,scalar,thickness);
	}
}


void LMDisplay::DrawDetWindWind(Image<RGBMap> *image,LMDetWind &wind,const RGBMap scalar,int thickness)
{
	//CvScalar scalar2;
	//if(scalar==NULL)
	//	scalar2 = cvScalar(rand()&255,rand()&255,rand()&255);
	//else
	//	scalar2 = *scalar;

	ImageDraw<RGBMap>::Line(image, wind.x_,wind.y_, wind.x_+wind.width_,wind.y_,scalar,thickness);
	ImageDraw<RGBMap>::Line(image, wind.x_+wind.width_,wind.y_, wind.x_+wind.width_, wind.y_+wind.height_,scalar,thickness);
	ImageDraw<RGBMap>::Line(image, wind.x_+wind.width_,wind.y_+wind.height_, wind.x_, wind.y_+wind.height_,scalar,thickness);
	ImageDraw<RGBMap>::Line(image, wind.x_,wind.y_+wind.height_, wind.x_, wind.y_,scalar,thickness);
}

void LMDisplay::DrawDetWindCost(Image<RGBMap> *image,LMDetWind &wind,const RGBMap scalar,int thickness)
{
	//CvScalar scalar2;
	//if(scalar==NULL)
	//	scalar2 = cvScalar(rand()&255,rand()&255,rand()&255);
	//else
	//	scalar2 = *scalar;

	ImageDraw<RGBMap>::Line(image, wind.x_,wind.y_, wind.x_+wind.width_,wind.y_,scalar,thickness);
	ImageDraw<RGBMap>::Line(image, wind.x_+wind.width_,wind.y_, wind.x_+wind.width_, wind.y_+wind.height_,scalar,thickness);
	ImageDraw<RGBMap>::Line(image, wind.x_+wind.width_,wind.y_+wind.height_, wind.x_, wind.y_+wind.height_,scalar,thickness);
	ImageDraw<RGBMap>::Line(image, wind.x_,wind.y_+wind.height_, wind.x_, wind.y_,scalar,thickness);
	//CvFont font;
	//cvInitFont(&font,CV_FONT_VECTOR0,0.5,0.5,0.0,2);
	//char str[255];
	//sprintf(str,"%4.4lf/%d",wind.cost_,wind.count_);
	//cvPutText(image,str,cvPoint(wind.x_,wind.y_-5),&font,scalar2);
}


void LMDisplay::StoreDetWind(const char *filename,vector< vector<LMDetWind> > &detWindArrays)
{
	ofstream file;
	file.open(filename);
	int nIntervals = detWindArrays.size();	
	file<<nIntervals<<endl;
	for(int i=0;i<nIntervals;i++)
	{
		int nWinds = detWindArrays[i].size();
		file<<nWinds<<endl;
		for(int j=0;j<nWinds;j++)
		{
			file<<detWindArrays[i][j]<<endl;
		}
	}
	file.close();
}