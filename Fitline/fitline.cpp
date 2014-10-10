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

#include <iostream>
#include <string>
#include "Image/Image.h"
#include "Image/ImageIO.h"
#include "LFLineFitter.h"

using namespace std;

void main(int argc, char *argv[])
{
	//IplImage *inputImage=NULL;
	Image<uchar> *inputImage=NULL;
	LFLineFitter lf;

	if(argc != 4)
	{
		std::cerr<<"[Syntax] fitline   input_edgeMap.pgm   output_line.txt   output_edgeMap.pgm"<<std::endl;
		exit(0);
	}

	string imageName(argv[1]);
	string outFilename(argv[2]);
	string outputImageName(argv[3]);
	
	//string imageName("data/hat_edges.pgm");
	//string outFilename("data/hat_edges.txt");
	//string outputImageName("data/hat_edges_display.pgm");

	
	//inputImage = cvLoadImage(imageName.c_str(),0);
	inputImage = ImageIO::LoadPGM(imageName.c_str());
	if(inputImage==NULL)
	{
		std::cerr<<"[ERROR] Fail in reading image "<<imageName<<std::endl;
		exit(0);
	}
	
	lf.Init();
	
	lf.Configure("para_template_line_fitter.txt");
	
	lf.FitLine(inputImage);

	lf.DisplayEdgeMap(inputImage,outputImageName.c_str());	

	lf.SaveEdgeMap(outFilename.c_str());

	//cvReleaseImage(&inputImage);
	delete inputImage;
};