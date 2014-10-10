#include "mex.h"

#include <iostream>
#include <string>
#include "Image/Image.h"
#include "Image/ImageIO.h"
#include "fitline/LFLineFitter.h"

using namespace std;

void mexFunction(int nlhs, mxArray *plhs[ ],int nrhs, const mxArray *prhs[ ]) 
{
	if(nrhs!=2)
		mexErrMsgTxt("Syntax error [lineRep lineMap] = mex_fitline(edgeMap,para) .\n");

	if(nlhs > 2)
		mexErrMsgTxt("Too many output arguments.");
	
	// parse the parameter

	
	mxArray *tmp;
	int nfields;
	mwSize NStructElems;
	mxClassID  *classIDflags;
	
    nfields = mxGetNumberOfFields(prhs[1]);
    NStructElems = mxGetNumberOfElements(prhs[1]);
    // allocate memory  for storing classIDflags
    classIDflags = (mxClassID *)mxCalloc(nfields, sizeof(mxClassID));
	
	
	size_t width,height;
	double *data;
	width = mxGetN(prhs[0]);
	height = mxGetM(prhs[0]);
	data = mxGetPr(prhs[0]);
	
	
	// Create Iamge
	Image<uchar> inputImage;
	inputImage.Resize(width,height,false);

	size_t index1,index2;
	index2 = 0;
	int    row,col; 
	for (col=0; col < mxGetN(prhs[0]); col++)
	{
		for (row=0; row < mxGetM(prhs[0]); row++)
		{			
			inputImage.Access(col,row) = mxGetPr(prhs[0])[row+col*mxGetM(prhs[0])];
		}	
	}
	

	
	
	double *sigma_fit_a_line;
	double *sigma_find_support;
	double *max_gap;
	double *nLinesToFitInStage1;
	double *nTrialsPerLineInStage1;
	double *nLinesToFitInStage2;
	double *nTrialsPerLineInStage2;
	

	tmp = mxGetField(prhs[1],0,"SIGMA_FIT_A_LINE");
	if(tmp)
		sigma_fit_a_line = mxGetPr(tmp);

	tmp = mxGetField(prhs[1],0,"SIGMA_FIND_SUPPORT");
	if(tmp)
		sigma_find_support = mxGetPr(tmp);

		tmp = mxGetField(prhs[1],0,"MAX_GAP");
	if(tmp)
		max_gap = mxGetPr(tmp);


	tmp = mxGetField(prhs[1],0,"N_LINES_TO_FIT_IN_STAGE_1");
	if(tmp)
		nLinesToFitInStage1 = mxGetPr(tmp);
	

	tmp = mxGetField(prhs[1],0,"N_TRIALS_PER_LINE_IN_STAGE_1");
	if(tmp)
		nTrialsPerLineInStage1 = mxGetPr(tmp);
	

	tmp = mxGetField(prhs[1],0,"N_LINES_TO_FIT_IN_STAGE_2");
	if(tmp)
		nLinesToFitInStage2 = mxGetPr(tmp);
	
	
	tmp = mxGetField(prhs[1],0,"N_TRIALS_PER_LINE_IN_STAGE_2");
	if(tmp)
		nTrialsPerLineInStage2 = mxGetPr(tmp);
	

	
	int nLayer = 2;
	int nLinesToFitInStage[2];
	int nTrialsPerLineInStage[2];
	nLinesToFitInStage[0] = (int)floor(*nLinesToFitInStage1);
	nLinesToFitInStage[1] = (int)floor(*nLinesToFitInStage2);
	nTrialsPerLineInStage[0] = (int)floor(*nTrialsPerLineInStage1);
	nTrialsPerLineInStage[1] = (int)floor(*nTrialsPerLineInStage2);
	

	// mexPrintf("\n\n /* Read line fitting parameters */\n");		
	// mexPrintf("SIGMA_FIT_A_LINE = %lf\n",*sigma_fit_a_line);		
	// mexPrintf("SIGMA_FIND_SUPPORT = %lf\n",*sigma_find_support);
	// mexPrintf("MAX_GAP = %lf\n",*max_gap);		
	// mexPrintf("N_LINES_TO_FIT_IN_STAGE_1 = %d\n",nLinesToFitInStage[0]);	
	// mexPrintf("N_LINES_TO_FIT_IN_STAGE_2 = %d\n",nLinesToFitInStage[1]);	
	// mexPrintf("N_TRIALS_PER_LINE_IN_STAGE_1 = %d\n",nTrialsPerLineInStage[0]);	
	// mexPrintf("N_TRIALS_PER_LINE_IN_STAGE_2 = %d\n",nTrialsPerLineInStage[1]);	
	
	
	LFLineFitter lf;		
	lf.Configure(*sigma_fit_a_line,*sigma_find_support,*max_gap,nLayer,nLinesToFitInStage,nTrialsPerLineInStage);
	lf.Init();	
	lf.FitLine(&inputImage);
	
	double *out;
	plhs[0] = mxCreateDoubleMatrix(lf.rNLineSegments(), 4, mxREAL);
	out =  mxGetPr(plhs[0]);
	for(int i=0;i<lf.rNLineSegments();i++)
	{
		out[i+0*lf.rNLineSegments()] = lf.outEdgeMap_[i].sx_;
		out[i+1*lf.rNLineSegments()] = lf.outEdgeMap_[i].sy_;
		out[i+2*lf.rNLineSegments()] = lf.outEdgeMap_[i].ex_;
		out[i+3*lf.rNLineSegments()] = lf.outEdgeMap_[i].ey_;	
	}
	
	
	
	
	Image<uchar> *debugImage = lf.ComputeOuputLineImage(&inputImage);	
	plhs[1] = mxCreateDoubleMatrix(mxGetM(prhs[0]), mxGetN(prhs[0]), mxREAL);	
	out =  mxGetPr(plhs[1]);

	for (col=0; col < mxGetN(plhs[1]); col++)
	{
		for (row=0; row < mxGetM(plhs[1]); row++)
		{			
				out[row+col*mxGetM(plhs[1])] = debugImage->Access(col,row);
		}	
	}

	
	delete debugImage;
	
    mxFree(classIDflags);

    return;	
	
}
