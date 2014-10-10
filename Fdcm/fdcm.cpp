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


//#include <cxcore.h>
#include "Image/Image.h"
#include "Image/ImageIO.h"
#include "fitline/LFLineFitter.h"
#include "LMLineMatcher.h"

#include <iostream>
#include <string>


void main(int argc, char *argv[])
{
	
	//if(argc < 4)
	//{
	//	//std::cerr<<"[Syntax] fdcm template.txt input_edgeMap.pgm input_realImage.jpg"<<std::endl;
	//	std::cerr<<"[Syntax] fdcm template.txt input_edgeMap.pgm input_realImage.ppm"<<std::endl;
	//	exit(0);
	//}

	LFLineFitter lf;
	LMLineMatcher lm;
	lf.Configure("para_line_fitter.txt");
	lm.Configure("para_line_matcher.txt");

	
	//Image *inputImage=NULL;
	Image<uchar> *inputImage=NULL;
	
	string templateFileName(argv[1]);
	string edgeMapName(argv[2]);
	string displayImageName(argv[3]);
	
	//string templateFileName("Exp_Smoothness/template_list.txt");
	//string edgeMapName("Exp_Smoothness/device5-20_edge_cluttered.pgm");
	//string displayImageName("Exp_Smoothness/device5-20_edge_cluttered.pgm");	

	//string templateFileName("data/template_giraffe.txt");
	//string edgeMapName("data/looking_edges.pgm");
	//string displayImageName("data/looking.ppm");
	
	//string templateFileName("data/template_applelogo.txt");
	//string edgeMapName("data/hat_edges.pgm");
	//string displayImageName("data/hat.jpg");

	
	//inputImage = cvLoadImage(edgeMapName.c_str(),0);
	inputImage = ImageIO::LoadPGM(edgeMapName.c_str());

	
	if(inputImage==NULL)
	{
		std::cerr<<"[ERROR] Fail in reading image "<<edgeMapName<<std::endl;
		exit(0);
	}
	

	lf.Init();
	
	lm.Init(templateFileName.c_str());

	
	// Line Fitting
	lf.FitLine(inputImage);


	
	// FDCM Matching
	vector<LMDetWind> detWind;
	lm.Match(lf,detWind);
	//lm.MatchCostMap(lf,outputCostMapName.c_str());
	// Display best matcher in edge map
	if(displayImageName.c_str())
	{
		Image<RGBMap> *debugImage = ImageIO::LoadPPM(displayImageName.c_str());
		LMDisplay::DrawDetWind(debugImage,detWind[0].x_,detWind[0].y_,detWind[0].width_,detWind[0].height_,RGBMap(0,255,0),4);
		char outputname[256];
		sprintf(outputname,"%s.output.ppm",displayImageName.c_str());
		ImageIO::SavePPM(debugImage,outputname);
		delete debugImage;
	}

	//cvReleaseImage(&inputImage);
	delete inputImage;
};
