#include "mex.h"

#include "image/Image.h"
#include "image/ImageIO.h"
#include "fitline/LFLineFitter.h"
#include "fdcm/LMLineMatcher.h"
#include <iostream>
#include <string>

using namespace std;

void mexFunction(int nlhs, mxArray *plhs[ ],int nrhs, const mxArray *prhs[ ]) 
{
	if(nrhs!=5)
		mexErrMsgTxt("Syntax error [detWind] = mex_fdcm_detect(edgeMap,template,threshold,lf_para,lm_para) .\n");

	// if(nlhs > 1)
		// mexErrMsgTxt("Too many output arguments.");
	
	// Convert the image representation
	size_t width,height;
	double *data;
	width = mxGetN(prhs[0]);
	height = mxGetM(prhs[0]);
	data = mxGetPr(prhs[0]);
	
	
	// Create Iamge
	Image<uchar> inputImage;
	inputImage.Resize(width,height,false);
	//mexPrintf("( width , height ) = ( %d , %d )\n",width,height);
	int row,col; 
	for (col=0; col < mxGetN(prhs[0]); col++)
	{
		for (row=0; row < mxGetM(prhs[0]); row++)
		{			
			inputImage.Access(col,row) = mxGetPr(prhs[0])[row+col*mxGetM(prhs[0])];
		}	
	}
	
	
	// Parse the parameter
	mxArray *tmp;
	int nfields;
	mwSize NStructElems;
	mxClassID  *classIDflags;
	
    nfields = mxGetNumberOfFields(prhs[3]);
    NStructElems = mxGetNumberOfElements(prhs[3]);
    // allocate memory  for storing classIDflags
    classIDflags = (mxClassID *)mxCalloc(nfields, sizeof(mxClassID));
	

	
	double *sigma_fit_a_line;
	double *sigma_find_support;
	double *max_gap;
	double *nLinesToFitInStage1;
	double *nTrialsPerLineInStage1;
	double *nLinesToFitInStage2;
	double *nTrialsPerLineInStage2;
	
	tmp = mxGetField(prhs[3],0,"SIGMA_FIT_A_LINE");
	if(tmp)
		sigma_fit_a_line = mxGetPr(tmp);

	tmp = mxGetField(prhs[3],0,"SIGMA_FIND_SUPPORT");
	if(tmp)
		sigma_find_support = mxGetPr(tmp);
	
	tmp = mxGetField(prhs[3],0,"MAX_GAP");
	if(tmp)
		max_gap = mxGetPr(tmp);


	tmp = mxGetField(prhs[3],0,"N_LINES_TO_FIT_IN_STAGE_1");
	if(tmp)
		nLinesToFitInStage1 = mxGetPr(tmp);
	
	tmp = mxGetField(prhs[3],0,"N_TRIALS_PER_LINE_IN_STAGE_1");
	if(tmp)
		nTrialsPerLineInStage1 = mxGetPr(tmp);
	
	tmp = mxGetField(prhs[3],0,"N_LINES_TO_FIT_IN_STAGE_2");
	if(tmp)
		nLinesToFitInStage2 = mxGetPr(tmp);
		
	tmp = mxGetField(prhs[3],0,"N_TRIALS_PER_LINE_IN_STAGE_2");
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
	

	
	
	double *nDirection;
	double *directionCost;
	double *maxCost;
	double *matchingScale;
	double *dbScale;
	double *baseSearchScale;
	double *minSearchScale;
	double *maxSearchScale;
	double *baseSearchAspect;
	double *minSearchAspect;
	double *maxSearchAspect;
	double *searchStepSize;
	double *searchBoundarySize;
	double *minCostRatio;
	
	//mexPrintf("\n\n/* Read line matching parameters */\n");	
	tmp = mxGetField(prhs[4],0,"NUMBER_DIRECTION");
	if(tmp)
		nDirection = mxGetPr(tmp);
	//mexPrintf("NUMBER_DIRECTION = %lf\n",*nDirection);

	tmp = mxGetField(prhs[4],0,"DIRECTIONAL_COST");
	if(tmp)
		directionCost = mxGetPr(tmp);
	//mexPrintf("DIRECTIONAL_COST = %lf\n",*directionCost);	
	
	tmp = mxGetField(prhs[4],0,"MAXIMUM_EDGE_COST");
	if(tmp)
		maxCost = mxGetPr(tmp);
	//mexPrintf("MAXIMUM_EDGE_COST = %lf\n",*maxCost);	
	
	tmp = mxGetField(prhs[4],0,"MATCHING_SCALE");
	if(tmp)
		matchingScale = mxGetPr(tmp);
	//mexPrintf("MATCHING_SCALE = %lf\n",*matchingScale);	
		
	tmp = mxGetField(prhs[4],0,"TEMPLATE_SCALE");
	if(tmp)
		dbScale = mxGetPr(tmp);
	//mexPrintf("TEMPLATE_SCALE = %lf\n",*dbScale);	

	tmp = mxGetField(prhs[4],0,"BASE_SEARCH_SCALE");
	if(tmp)
		baseSearchScale = mxGetPr(tmp);
	//mexPrintf("BASE_SEARCH_SCALE = %lf\n",*baseSearchScale);	

	tmp = mxGetField(prhs[4],0,"MIN_SEARCH_SCALE");
	if(tmp)
		minSearchScale = mxGetPr(tmp);
	//mexPrintf("MIN_SEARCH_SCALE = %lf\n",*minSearchScale);	

	tmp = mxGetField(prhs[4],0,"MAX_SEARCH_SCALE");
	if(tmp)
		maxSearchScale = mxGetPr(tmp);
	//mexPrintf("MAX_SEARCH_SCALE = %lf\n",*maxSearchScale);		

	tmp = mxGetField(prhs[4],0,"BASE_SEARCH_ASPECT");
	if(tmp)
		baseSearchAspect = mxGetPr(tmp);
	//mexPrintf("BASE_SEARCH_ASPECT = %lf\n",*baseSearchAspect);	

	tmp = mxGetField(prhs[4],0,"MIN_SEARCH_ASPECT");
	if(tmp)
		minSearchAspect = mxGetPr(tmp);
	//mexPrintf("MIN_SEARCH_ASPECT = %lf\n",*minSearchAspect);	

	tmp = mxGetField(prhs[4],0,"MAX_SEARCH_ASPECT");
	if(tmp)
		maxSearchAspect = mxGetPr(tmp);
	//mexPrintf("MAX_SEARCH_ASPECT = %lf\n",*maxSearchAspect);		
	
	tmp = mxGetField(prhs[4],0,"SEARCH_STEP_SIZE");
	if(tmp)
		searchStepSize = mxGetPr(tmp);
	//mexPrintf("SEARCH_STEP_SIZE = %lf\n",*searchStepSize);
	
	tmp = mxGetField(prhs[4],0,"SEARCH_BOUNDARY_SIZE");
	if(tmp)
		searchBoundarySize = mxGetPr(tmp);
	//mexPrintf("SEARCH_BOUNDARY_SIZE = %lf\n",*searchBoundarySize);	

	tmp = mxGetField(prhs[4],0,"MIN_COST_RATIO");
	if(tmp)
		minCostRatio = mxGetPr(tmp);
	//mexPrintf("MIN_COST_RATIO = %lf\n",*minCostRatio);
		

	double *maxThreshold = mxGetPr(prhs[2]);
	LMLineMatcher lm;
	
	// Load configuration
	lm.nDirections_ = (int)(*nDirection);
	lm.directionCost_ = (float)(*directionCost);
	lm.maxCost_ = *maxCost;
	lm.scale_ = *matchingScale;
	lm.db_scale_ = *dbScale;
	lm.baseSearchScale_ = *baseSearchScale;
	lm.minSearchScale_ = (int)(*minSearchScale);
	lm.maxSearchScale_ = (int)(*maxSearchScale);
	lm.baseSearchAspect_ = *baseSearchAspect;
	lm.minSearchAspect_ = (int)(*minSearchAspect);
	lm.maxSearchAspect_ = (int)(*maxSearchAspect);
	lm.searchStepSize_ = (int)(*searchStepSize);
	lm.searchBoundarySize_ = (int)(*searchBoundarySize);
	lm.minCostRatio_ = *minCostRatio;
	
		
	// Load tempalte files
	// The following codes replace the functionality of the code
	// lm.Init(templateFileName.c_str());
	int m = mxGetM(prhs[1]);
	int n = mxGetN(prhs[1]);	
	int nTemplate = max(m,n);
	lm.ndbImages_ = nTemplate;
	lm.dbImages_ = new EIEdgeImage[lm.ndbImages_];
	
	
	mxArray *tempShape;
	for(int i=0;i<nTemplate;i++)
	{
		tempShape = mxGetCell(prhs[1],i);
		double *pTempShape = mxGetPr(tempShape);
		int nRow = mxGetM(tempShape);
		int nCol = mxGetN(tempShape);
		//mexPrintf("( m , n ) = ( %d , %d )\n",nRow,nCol);
		lm.dbImages_[i].SetNumDirections(lm.nDirections_);
		lm.dbImages_[i].Read( pTempShape, nRow );
		lm.dbImages_[i].Scale(lm.scale_*lm.db_scale_);
	}
		
	vector< vector<LMDetWind> > detWindArrays;
	detWindArrays.clear();
	lm.SingleShapeDetectionWithVaryingQuerySize(lf,*maxThreshold,detWindArrays);	
	
	int last = detWindArrays.size()-1;
	int nDetWindows = detWindArrays[last].size();
		
	//mexPrintf("\n\n/* Matching */\n");
	//mexPrintf("Num of Template = %d; ",nTemplate);
	//mexPrintf("Threshold = %lf; nDetWindows = %d\n",*maxThreshold,nDetWindows);
	
	double *out;
	
	plhs[0] = mxCreateDoubleMatrix(nDetWindows, 6, mxREAL);
	out =  mxGetPr(plhs[0]);
	for(int i=0;i<nDetWindows;i++)
	{
		out[i+0*nDetWindows] = 1.0*detWindArrays[last][i].x_;
		out[i+1*nDetWindows] = 1.0*detWindArrays[last][i].y_;
		out[i+2*nDetWindows] = 1.0*detWindArrays[last][i].width_;
		out[i+3*nDetWindows] = 1.0*detWindArrays[last][i].height_;
		out[i+4*nDetWindows] = 1.0*detWindArrays[last][i].cost_;
		out[i+5*nDetWindows] = 1.0*detWindArrays[last][i].count_;
	}	

	/*
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
	*/
	
    mxFree(classIDflags);

    return;	
	
}
