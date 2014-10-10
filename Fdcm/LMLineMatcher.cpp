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
#include "LMLineMatcher.h"


LMLineMatcher::LMLineMatcher()
{
	dbImages_ = NULL;
	bias_ = 1.0;

	nDirections_ = 60;	
	directionCost_ = 0.5;
	maxCost_ = 30;	
	scale_ = 1.0;
	db_scale_ = 1.0;

	// search paperters
	minCostRatio_ = 1.0;

	baseSearchScale_ = 1.2;
	minSearchScale_ = 0;
	maxSearchScale_ = 0;

	baseSearchAspect_ = 1.1;
	minSearchAspect_ = 0;
	maxSearchAspect_ = 0;

	searchStepSize_ = 2;
	searchBoundarySize_ = 5;
};

LMLineMatcher::~LMLineMatcher()
{
	SafeRelease();
};

void LMLineMatcher::SafeRelease()
{
	if (dbImages_)
		delete[] dbImages_;
	dbImages_ = NULL;
};



void LMLineMatcher::Match(LFLineFitter &lf,vector<LMDetWind> &detWind)
{
	// initialize the memory for storing the information of the best hypothesis.
	double *distances = new double[MAX_NUM_HYPOTHESES];
	int *indices = new int[MAX_NUM_HYPOTHESES];
	int *xIndices = new int[MAX_NUM_HYPOTHESES];
	int *yIndices = new int[MAX_NUM_HYPOTHESES];
	double *sIndices = new double[MAX_NUM_HYPOTHESES];
	double *dIndices = new double[MAX_NUM_HYPOTHESES];
	int *iindices = new int[MAX_NUM_HYPOTHESES];
	int *wIndices = new int[MAX_NUM_HYPOTHESES];
	int *hIndices = new int[MAX_NUM_HYPOTHESES];

	//LARGE_INTEGER t1, t2, f;
	////QueryPerformanceFrequency(&f);
	////QueryPerformanceCounter(&t1);

	// structure computation
	queryImage_.SetNumDirections(nDirections_);
	queryImage_.Read(lf);
	queryImage_.Scale(scale_);
	queryDistanceImage_.Configure(directionCost_,maxCost_);
	queryDistanceImage_.SetImage(queryImage_);


	////QueryPerformanceCounter(&t2);
	//std::cout<<"IDT3 Computation Time "<<setiosflags(ios::fixed)<<setprecision(6)<<(t2.QuadPart - t1.QuadPart)/(1.0*f.QuadPart)<<std::endl;	

	////QueryPerformanceFrequency(&f);
	////QueryPerformanceCounter(&t1);

	double minCost = 1e+10;
	
	// mathcing
	int counter = 0;
	for (int i=0 ; i<ndbImages_ ; i++)
	{		
		MatchBruteForce(dbImages_[i], i, iindices, indices, xIndices, yIndices, dIndices, sIndices, distances, counter, minCost);		
	}

	////QueryPerformanceCounter(&t2);
	//std::cout<<"Fast Directional Chamfer Matching Time "<<setiosflags(ios::fixed)<<setprecision(6)<<(t2.QuadPart - t1.QuadPart)/(1.0*f.QuadPart)<<std::endl;	

	MMFunctions::ISort(distances, counter, iindices);

	// Compute the best hypothesis
	LMDetWind wind;
	wind.x_ = (int)ceil(xIndices[iindices[0]]/scale_-0.5);
	wind.y_ = (int)ceil(yIndices[iindices[0]]/scale_-0.5);
	
	double scale = sIndices[iindices[0]];
	wind.width_ = (int)(dbImages_[indices[iindices[0]]].width_*scale/scale_);
	wind.height_ = (int)(dbImages_[indices[iindices[0]]].height_*scale/scale_);
	
	detWind.push_back(wind);


	delete []	distances;
	delete []	indices;
	delete []	xIndices;
	delete []	yIndices;
	delete []	dIndices;
	delete []	iindices;
	delete []	sIndices;
	delete []	wIndices;
	delete []	hIndices;
}



double LMLineMatcher::MatchBruteForce(EIEdgeImage& dbImage, int index, int* iindices, int* indices, int* xIndex, int* yIndex, double* dIndex, double* sIndex, double* distances, int& counter, double& minCost)
{	
	LFLineSegment dbLine, queryLine, line;
	int count =0,i=0,k=0;
	double ltrans[2];
	double factor = 1.0;
	double cost, sum, scale;
	double minx, miny, maxx, maxy;
	int currentcount;

	for(int s = minSearchScale_  ; s<= maxSearchScale_ ; s++)	
	{		
		scale = pow(baseSearchScale_,1.0*s);
		EIEdgeImage tdbImage;
		tdbImage = dbImage;
		tdbImage.Scale(scale);
		factor = 1.0/pow(dbImage.Length(),bias_);
		tdbImage.Boundary(minx, miny, maxx, maxy);
		tdbImage.SetDirectionIndices();

		for (int y=-(int)miny; y<queryImage_.height_-(int)maxy; y += searchStepSize_)
		{
			for (int x=-(int)minx ; x<queryImage_.width_-(int)maxx ; x += searchStepSize_)	
			{
		
				ltrans[0] = (double)x;
				ltrans[1] = (double)y;				
				cost = 0;				

				if (minx + ltrans[0] <=searchBoundarySize_ || minx + ltrans[0] >=queryImage_.width_-searchBoundarySize_ || 
					maxx + ltrans[0] <=searchBoundarySize_ || maxx + ltrans[0] >=queryImage_.width_-searchBoundarySize_ ||
					miny + ltrans[1] <=searchBoundarySize_ || miny + ltrans[1] >=queryImage_.height_-searchBoundarySize_ || 
					maxy + ltrans[1] <=searchBoundarySize_ || maxy + ltrans[1] >=queryImage_.height_-searchBoundarySize_ )
				{
					cost = 1e+10;
					continue;
				}
				else
				{
					count++;

					for (k=0 ; k<tdbImage.nLines_ ; k++)
					{
						line = tdbImage.lines_[k];
						line.Translate(ltrans);	


						sum = queryDistanceImage_.idtImages_[tdbImage.directionIndices_[k]].Sum((int)line.sx_,(int)line.sy_,(int)line.ex_,(int)line.ey_, currentcount);
						cost+= sum*factor;
						//cost+=sum*factor;
						if (cost > minCost*minCostRatio_)
						{
							cost = 1e+10;
							break;
						}
					}
				}


				if (cost<minCost*minCostRatio_)
				{
					xIndex[counter] = (int)ltrans[0];
					yIndex[counter] = (int)ltrans[1];
					dIndex[counter] = (i*M_PI)/nDirections_;
					sIndex[counter] = scale;
					distances[counter] = cost;
					iindices[counter] = counter;
					indices[counter++] = index;

					if (cost<minCost)
						minCost = cost;
				}
			}
		}
		
	}
	return minCost;
}

void LMLineMatcher::MatchCostMap(LFLineFitter &lf,const char *filename)
{
	//LARGE_INTEGER t1, t2, f;
	////QueryPerformanceFrequency(&f);
	////QueryPerformanceCounter(&t1);


	queryImage_.SetNumDirections(nDirections_);
	queryImage_.Read(lf);
	queryImage_.Scale(scale_);
	queryDistanceImage_.Configure(directionCost_,maxCost_);
	queryDistanceImage_.SetImage(queryImage_);


	////QueryPerformanceCounter(&t2);
	//std::cout<<"IDT3 Computation Time "<<setiosflags(ios::fixed)<<setprecision(6)<<(t2.QuadPart - t1.QuadPart)/(1.0*f.QuadPart)<<std::endl;	


	////QueryPerformanceFrequency(&f);
	////QueryPerformanceCounter(&t1);

	double minCost = 1e+10;
	

	vector<double> costMap(queryImage_.width_*queryImage_.height_,1e10);


	int counter = 0;
	for (int i=0 ; i<ndbImages_ ; i++)
	{		
		MatchBruteForceCostMap(dbImages_[i],costMap);
	}

	////QueryPerformanceCounter(&t2);
	//std::cout<<"Fast Directional Chamfer Matching Time "<<setiosflags(ios::fixed)<<setprecision(6)<<(t2.QuadPart - t1.QuadPart)/(1.0*f.QuadPart)<<std::endl;	

	// store matching cost
	ofstream file;
	file.open(filename);
	for(int y=0;y<queryImage_.height_;y++)
	{
		for(int x=0;x<queryImage_.width_;x++)
		{
			int index = x+y*queryImage_.width_;
			file<<costMap[index]<<" ";
		}
		file<<::endl;
	}
	file.close();
}


double LMLineMatcher::MatchBruteForceCostMap(EIEdgeImage& dbImage,vector<double> &costMap)
{	
	LFLineSegment dbLine, queryLine, line;
	int count =0,i=0,k=0;
	double ltrans[2];
	double factor = 1.0;
	double cost, sum, scale;
	double minx, miny, maxx, maxy;
	int currentcount;
	double minCost = 1e10;

	for(int s = minSearchScale_  ; s<= maxSearchScale_ ; s++)	
	{		
		scale = pow(baseSearchScale_,1.0*s);
		EIEdgeImage tdbImage;
		tdbImage = dbImage;
		tdbImage.Scale(scale);
		factor = 1.0/dbImage.Length();
		tdbImage.Boundary(minx, miny, maxx, maxy);
		tdbImage.SetDirectionIndices();

		int y0,x0,imageIndex;
		y0 = 0;
		for (int y=-(int)miny; y<queryImage_.height_-(int)maxy; y += searchStepSize_)
		{
			x0 = 0;
			for (int x=-(int)minx ; x<queryImage_.width_-(int)maxx ; x += searchStepSize_)	
			{
		
				ltrans[0] = (double)x;
				ltrans[1] = (double)y;				
				cost = 0;				

				if (minx + ltrans[0] <=searchBoundarySize_ || minx + ltrans[0] >=queryImage_.width_-searchBoundarySize_ || 
					maxx + ltrans[0] <=searchBoundarySize_ || maxx + ltrans[0] >=queryImage_.width_-searchBoundarySize_ ||
					miny + ltrans[1] <=searchBoundarySize_ || miny + ltrans[1] >=queryImage_.height_-searchBoundarySize_ || 
					maxy + ltrans[1] <=searchBoundarySize_ || maxy + ltrans[1] >=queryImage_.height_-searchBoundarySize_ )
				{
					cost = 1e+10;
					continue;
				}
				else
				{
					count++;
					for (k=0 ; k<tdbImage.nLines_ ; k++)
					{
						line = tdbImage.lines_[k];
						line.Translate(ltrans);	
						sum = queryDistanceImage_.idtImages_[tdbImage.directionIndices_[k]].Sum((int)line.sx_,(int)line.sy_,(int)line.ex_,(int)line.ey_, currentcount);		
						cost+=sum*factor;
					}
					imageIndex = x0 + y0*queryImage_.width_;
					costMap[imageIndex] = cost;
				}

				if (cost<minCost)
					minCost = cost;

				//if (cost<minCost*minCostRatio_)
				//{
				//	xIndex[counter] = (int)ltrans[0];
				//	yIndex[counter] = (int)ltrans[1];
				//	dIndex[counter] = (i*M_PI)/nDirections_;
				//	sIndex[counter] = scale;
				//	distances[counter] = cost;
				//	iindices[counter] = counter;
				//	indices[counter++] = index;
				//	if (cost<minCost)
				//		minCost = cost;
				//}
				x0++;
			}
			y0++;
		}
		
	}
	return minCost;
}


void LMLineMatcher::Configure(const char *filename)
{
	ifstream file;
	file.open(filename);
	string line;
	string column(":");
	size_t found;

	if( !file.is_open() )
	{
		cerr<<"Cannot open file "<<filename<<endl;
		exit(-1);
	}

	while( getline(file,line) )
	{
		if( string::npos != line.find("MODEL_SIZE_BIAS") )
		{
			found = line.find(column);
			string substring = line.substr(found+1,line.length()-found).c_str();
			bias_ = atof(substring.c_str());
		}
		if( string::npos != line.find("NUMBER_DIRECTION") )
		{
			found = line.find(column);
			string substring = line.substr(found+1,line.length()-found).c_str();
			nDirections_ = atoi(substring.c_str());
		}
		else if(string::npos != line.find("DIRECTIONAL_COST") )
		{
			found = line.find(column);
			string substring = line.substr(found+1,line.length()-found).c_str();
			directionCost_ = (float)atof(substring.c_str());
		}
		else if(string::npos != line.find("MAXIMUM_EDGE_COST") )
		{
			found = line.find(column);
			string substring = line.substr(found+1,line.length()-found).c_str();
			maxCost_ = atof(substring.c_str());
		}
		else if(string::npos != line.find("MATCHING_SCALE") )
		{
			found = line.find(column);
			string substring = line.substr(found+1,line.length()-found).c_str();
			scale_ = atof(substring.c_str());
		}
		else if(string::npos != line.find("TEMPLATE_SCALE") )
		{
			found = line.find(column);
			string substring = line.substr(found+1,line.length()-found).c_str();
			db_scale_ = atof(substring.c_str());
		}
		else if(string::npos != line.find("BASE_SEARCH_SCALE") )
		{
			found = line.find(column);
			string substring = line.substr(found+1,line.length()-found).c_str();
			baseSearchScale_ = atof(substring.c_str());
		}
		else if(string::npos != line.find("MIN_SEARCH_SCALE") )
		{
			found = line.find(column);
			string substring = line.substr(found+1,line.length()-found).c_str();
			minSearchScale_ = atoi(substring.c_str());
		}
		else if(string::npos != line.find("MAX_SEARCH_SCALE") )
		{
			found = line.find(column);
			string substring = line.substr(found+1,line.length()-found).c_str();
			maxSearchScale_ = atoi(substring.c_str());
		}
		else if(string::npos != line.find("SEARCH_STEP_SIZE") )
		{
			found = line.find(column);
			string substring = line.substr(found+1,line.length()-found).c_str();
			searchStepSize_ = atoi(substring.c_str());
		}
		else if(string::npos != line.find("SEARCH_BOUNDARY_SIZE") )
		{
			found = line.find(column);
			string substring = line.substr(found+1,line.length()-found).c_str();
			searchBoundarySize_ = atoi(substring.c_str());
		}
		else if(string::npos != line.find("MIN_COST_RATIO") )
		{
			found = line.find(column);
			string substring = line.substr(found+1,line.length()-found).c_str();
			minCostRatio_ = atof(substring.c_str());
		}
		else if(string::npos != line.find("BASE_SEARCH_ASPECT") )
		{
			found = line.find(column);
			string substring = line.substr(found+1,line.length()-found).c_str();
			baseSearchAspect_ = atof(substring.c_str());
		}
		else if(string::npos != line.find("MIN_SEARCH_ASPECT") )
		{
			found = line.find(column);
			string substring = line.substr(found+1,line.length()-found).c_str();
			minSearchAspect_ = atoi(substring.c_str());
		}
		else if(string::npos != line.find("MAX_SEARCH_ASPECT") )
		{
			found = line.find(column);
			string substring = line.substr(found+1,line.length()-found).c_str();
			maxSearchAspect_ = atoi(substring.c_str());
		}
	}
	file.close();

	PrintParameter();
}


void LMLineMatcher::PrintParameter()
{
	cout<<"/* =========================================================="<<endl;
	cout<<"* LMLineMatching parameters"<<endl;
	cout<<"* ==========================================================="<<endl;
	cout<<"* NUMBER_DIRECTION:"<<"\t"<<nDirections_<<endl;
	cout<<"* DIRECTIONAL_COST:"<<"\t"<<directionCost_<<endl;
	cout<<"* MAXIMUM_EDGE_COST:"<<"\t"<<maxCost_<<endl;
	cout<<"* MATCHING_SCALE:"<<"\t"<<scale_<<endl;
	cout<<"* TEMPLATE_SCALE:"<<"\t"<<db_scale_<<endl;
	cout<<"* BASE_SEARCH_SCALE:"<<"\t"<<baseSearchScale_<<endl;
	cout<<"* MIN_SEARCH_SCALE:"<<"\t"<<minSearchScale_<<endl;
	cout<<"* MAX_SEARCH_SCALE:"<<"\t"<<maxSearchScale_<<endl;
	cout<<"* BASE_SEARCH_ASPECT:"<<"\t"<<baseSearchAspect_<<endl;
	cout<<"* MIN_SEARCH_ASPECT:"<<"\t"<<minSearchAspect_<<endl;
	cout<<"* MAX_SEARCH_ASPECT:"<<"\t"<<maxSearchAspect_<<endl;
	cout<<"* SEARCH_STEP_SIZE:"<<"\t"<<searchStepSize_<<endl;
	cout<<"* SEARCH_BOUNDARY_SIZE:"<<"\t"<<searchBoundarySize_<<endl;
	cout<<"* MIN_COST_RATIO:"<<"\t"<<minCostRatio_<<endl;
	cout<<"* MODEL_SIZE_BIAS:"<<"\t"<<bias_<<endl;
	cout<<"* ==========================================================="<<endl;
	cout<<"*/"<<endl;
}

void LMLineMatcher::Init(const char* fileName)
{

	fstream file;
	file.open(fileName);

	// read the input template list
	if(!file.is_open())
	{
		cerr<<"Cannot read file "<<fileName<<"\n!!!";
		exit(-1);
	}
	string line;
	getline(file,line);
	ndbImages_ = atoi(line.c_str());	
	dbImages_ = new EIEdgeImage [ndbImages_];

	// read the templates
	cout<<"Number of templates = "<<ndbImages_<<endl;
	for (int i=0;i<ndbImages_ ;i++)
	{
		getline(file,line);
		dbImages_[i].SetNumDirections(nDirections_);
		dbImages_[i].Read(line.c_str());		
		dbImages_[i].Scale(scale_*db_scale_);
	}
	file.close();
}


void LMLineMatcher::SingleShapeDetectionWithVaryingTemplateSizeForROC(LFLineFitter &lf,double minThreshold,double gap,double maxThreshold,vector< vector<LMDetWind> > &detWindArrays)
{
	//LARGE_INTEGER t1, t2, f;
	////QueryPerformanceFrequency(&f);


	//
	// Structure computation
	//
	////QueryPerformanceCounter(&t1);

	queryImage_.SetNumDirections(nDirections_);
	queryImage_.Read(lf);
	queryImage_.Scale(scale_);
	queryDistanceImage_.Configure(directionCost_,maxCost_);
	queryDistanceImage_.SetImage(queryImage_);

	////QueryPerformanceCounter(&t2);
	//cout<<"Structure computation ";
	//cout<<"taking "<<(t2.QuadPart - t1.QuadPart)/(1.0*f.QuadPart)<<" seconds"<<endl;

	MatchingCostMap matchingCostMap;
	int nCostMap = (maxSearchScale_-minSearchScale_+1)*(maxSearchAspect_-minSearchAspect_+1);
	matchingCostMap.Init(nCostMap);
	//
	// Detection
	//
	////QueryPerformanceCounter(&t1);
	int counter = 0;
	int i=0;		
	DetectBruteForceVaryingTemplateSize(dbImages_[i],maxThreshold,matchingCostMap);

	////QueryPerformanceCounter(&t2);
	//cout<<"Detection Computation";
	//cout<<" ( "<<maxSearchScale_-minSearchScale_+1<<" scales )"<<" taking "<<(t2.QuadPart - t1.QuadPart)/(1.0*f.QuadPart)<<" seconds"<<endl;

	//
	// Nonmaximum suppression
	//
	////QueryPerformanceCounter(&t1);	
	double overlapThreshold = 0.2;
	int nInterval = 0;
	for(double t=minThreshold;t<=maxThreshold;t+=gap)
		nInterval++;

	int index = 0;
	int varyingQuerySize = 0;
	detWindArrays.resize(nInterval);	
	for(double t=minThreshold;t<=maxThreshold;t+=gap)
	{
		LMNonMaximumSuppression::ComputeDetection(matchingCostMap,t,overlapThreshold,detWindArrays[index],varyingQuerySize);
		index++;
	}
	////QueryPerformanceCounter(&t2);
	//cout<<"Non-maximum suppression ";
	//cout<<"taking "<<setiosflags(ios::fixed)<<setprecision(6)<<(t2.QuadPart - t1.QuadPart)/(1.0*f.QuadPart)<<endl;
}

void LMLineMatcher::SingleShapeDetectionWithVaryingQuerySizeForROC(LFLineFitter &lf,double minThreshold,double gap,double maxThreshold,vector< vector<LMDetWind> > &detWindArrays)
{
	//LARGE_INTEGER t1, t2, f;
	////QueryPerformanceFrequency(&f);
	double scale;
	int i=0;
	double structureCompTime = 0;
	double matchingCostCompTime = 0;
	MatchingCostMap matchingCostMap;
	int nCostMap = (maxSearchScale_-minSearchScale_+1)*(maxSearchAspect_-minSearchAspect_+1);
	int costMapIndex = 0;
	matchingCostMap.Init(nCostMap);

	cout<<"[";
	for(int s=minSearchScale_;s<=maxSearchScale_;s++)
	{
		cout<<".";

		//
		// structure computation
		//		
		////QueryPerformanceCounter(&t1);

		scale = pow(baseSearchScale_,1.0*s);
		queryImage_.SetNumDirections(nDirections_);
		queryImage_.Read(lf);
		queryImage_.Scale(scale_*scale);
		queryDistanceImage_.Configure(directionCost_,maxCost_);
		queryDistanceImage_.SetImage(queryImage_);

		////QueryPerformanceCounter(&t2);
		//structureCompTime += (t2.QuadPart - t1.QuadPart)/(1.0*f.QuadPart);


		//
		// compute matching cost
		//		
		////QueryPerformanceCounter(&t1);

		DetectBruteForce(dbImages_[i], maxThreshold, scale, matchingCostMap, costMapIndex);

		////QueryPerformanceCounter(&t2);
		//matchingCostCompTime += (t2.QuadPart - t1.QuadPart)/(1.0*f.QuadPart);

	}

	cout<<"]"<<endl;
	//cout<<"Structure computation";
	//cout<<" ( "<<maxSearchScale_-minSearchScale_+1<<" scales )"<<" taking "<<structureCompTime<<" seconds"<<endl;

	//cout<<"Detection Computation";
	//cout<<" ( "<<maxSearchScale_-minSearchScale_+1<<" scales )"<<" taking "<<matchingCostCompTime<<" seconds"<<endl;

		
	////QueryPerformanceCounter(&t1);

	
	double overlapThreshold = 0.2;
	int nInterval = 0;
	for(double t=minThreshold;t<=maxThreshold;t+=gap)
		nInterval++;

	int index = 0;
	int varyingQuerySize = 1;
	detWindArrays.resize(nInterval);	
	for(double t=minThreshold;t<=maxThreshold;t+=gap)
	{
		LMNonMaximumSuppression::ComputeDetection(matchingCostMap,t,overlapThreshold,detWindArrays[index],varyingQuerySize);
		index++;
	}
	////QueryPerformanceCounter(&t2);
	//cout<<"Non-maximum suppression ";
	//cout<<"taking "<<setiosflags(ios::fixed)<<setprecision(6)<<(t2.QuadPart - t1.QuadPart)/(1.0*f.QuadPart)<<endl;	
}


void LMLineMatcher::DetectBruteForce(EIEdgeImage& dbImage, double detectionThreshold,double scale,MatchingCostMap &matchingCostMap,int &costMapIndex)
{	
	
	int count=0;
	int currentcount=0;
	int imageIdx;
	int x0;
	int y0;
	int width;
	int height;
	double ltrans[2];
	double factor;
	double cost;
	double sum;
	double aspect;
	double minx, miny, maxx, maxy;
	LFLineSegment dbLine, queryLine, line;
	
	
	// detection with different aspect ratios
	for(int a = minSearchAspect_  ; a<= maxSearchAspect_ ; a++)	
	{			
		EIEdgeImage tdbImage;
		tdbImage = dbImage;

		aspect = pow(baseSearchAspect_,1.0*a);
		tdbImage.Aspect(aspect);
		tdbImage.Boundary(minx, miny, maxx, maxy);
		tdbImage.Tight(minx,miny,maxx,maxy);
		tdbImage.SetDirectionIndices();

		factor = 1.0/pow(tdbImage.Length(),bias_);
		detectionThreshold *= ( queryDistanceImage_.maxCost_/factor);

		width = (queryImage_.width_-(int)(maxx-minx) )/searchStepSize_;
		height = (queryImage_.height_-(int)(maxy-miny) )/searchStepSize_;
		width = max(0,width);
		height = max(0,height);
		matchingCostMap.templateWidth_[costMapIndex] = tdbImage.width_;
		matchingCostMap.templateHeight_[costMapIndex] = tdbImage.height_;
		matchingCostMap.width_[costMapIndex] = width;
		matchingCostMap.height_[costMapIndex] = height;
		matchingCostMap.x0_[costMapIndex] = 0;
		matchingCostMap.y0_[costMapIndex] = 0;
		matchingCostMap.stepSize_[costMapIndex] = searchStepSize_;
		matchingCostMap.scale_[costMapIndex] = scale;
		matchingCostMap.aspect_[costMapIndex] = aspect;
		matchingCostMap.costMap_[costMapIndex].resize(width*height,1e+10);

		if(min(width,height)==0)
			continue;


		y0 = 0;
		for (int y=-(int)miny; y<queryImage_.height_-(int)maxy; y += searchStepSize_)
		{
			x0 = 0;
			for (int x=-(int)minx ; x<queryImage_.width_-(int)maxx ; x += searchStepSize_)
			{
				ltrans[0] = (double)x;
				ltrans[1] = (double)y;				
				cost = 0;				

				if (minx + ltrans[0] <=searchBoundarySize_ || minx + ltrans[0] >=queryImage_.width_-searchBoundarySize_ || 
					maxx + ltrans[0] <=searchBoundarySize_ || maxx + ltrans[0] >=queryImage_.width_-searchBoundarySize_ ||
					miny + ltrans[1] <=searchBoundarySize_ || miny + ltrans[1] >=queryImage_.height_-searchBoundarySize_ || 
					maxy + ltrans[1] <=searchBoundarySize_ || maxy + ltrans[1] >=queryImage_.height_-searchBoundarySize_ )
				{
					x0++;
					continue;
				}
				else
				{
					count++;
					for (int k=0 ; k<tdbImage.nLines_ ; k++)
					{
						line = tdbImage.lines_[k];
						line.Translate(ltrans);
						sum = queryDistanceImage_.idtImages_[tdbImage.directionIndices_[k]].Sum((int)line.sx_,(int)line.sy_,(int)line.ex_,(int)line.ey_, currentcount);
						cost+=sum;
						if (cost > detectionThreshold)
						{
							cost = 1e+10;
							break;
						}
					}				
					cost *= factor;
					cost /= queryDistanceImage_.maxCost_;

				}
				imageIdx = x0 + y0*width;
				matchingCostMap.costMap_[costMapIndex][imageIdx] = cost;
				x0++;
			}
			y0++;
		}
		costMapIndex++;
	}
}




void LMLineMatcher::DetectBruteForceVaryingTemplateSize(EIEdgeImage& dbImage, double detectionThreshold,MatchingCostMap &matchingCostMap)
{	
	
	int costMapIndex = 0;
	int count=0;
	int currentcount=0;
	int imageIdx;
	int x0;
	int y0;
	int width;
	int height;
	double ltrans[2];
	double factor;
	double cost;
	double sum;
	double aspect;
	double scale;
	double minx, miny, maxx, maxy;
	LFLineSegment dbLine, queryLine, line;
	
	for(int s = minSearchScale_; s<= maxSearchScale_; s++)
	{
		scale = pow(baseSearchScale_,1.0*s);
		// detection with different aspect ratios
		for(int a = minSearchAspect_  ; a<= maxSearchAspect_ ; a++)	
		{			
			EIEdgeImage tdbImage;
			tdbImage = dbImage;

			aspect = pow(baseSearchAspect_,1.0*a);
			tdbImage.Scale(scale);
			tdbImage.Aspect(aspect);
			tdbImage.Boundary(minx, miny, maxx, maxy);
			tdbImage.Tight(minx,miny,maxx,maxy);
			tdbImage.SetDirectionIndices();

			factor = 1.0/pow(tdbImage.Length(),bias_);
			detectionThreshold *= ( queryDistanceImage_.maxCost_/factor);

			width = (queryImage_.width_-(int)(maxx-minx) )/searchStepSize_;
			height = (queryImage_.height_-(int)(maxy-miny) )/searchStepSize_;
			width = max(0,width);
			height = max(0,height);
			matchingCostMap.templateWidth_[costMapIndex] = tdbImage.width_;
			matchingCostMap.templateHeight_[costMapIndex] = tdbImage.height_;
			matchingCostMap.width_[costMapIndex] = width;
			matchingCostMap.height_[costMapIndex] = height;
			matchingCostMap.x0_[costMapIndex] = 0;
			matchingCostMap.y0_[costMapIndex] = 0;
			matchingCostMap.stepSize_[costMapIndex] = searchStepSize_;
			matchingCostMap.scale_[costMapIndex] = scale;
			matchingCostMap.aspect_[costMapIndex] = aspect;
			matchingCostMap.costMap_[costMapIndex].resize(width*height,1e+10);
			if(min(width,height)==0)
				continue;

			y0 = 0;
			for (int y=-(int)miny; y<queryImage_.height_-(int)maxy; y += searchStepSize_)
			{
				x0 = 0;
				for (int x=-(int)minx ; x<queryImage_.width_-(int)maxx ; x += searchStepSize_)
				{
					ltrans[0] = (double)x;
					ltrans[1] = (double)y;				
					cost = 0;				

					if (minx + ltrans[0] <=searchBoundarySize_ || minx + ltrans[0] >=queryImage_.width_-searchBoundarySize_ || 
						maxx + ltrans[0] <=searchBoundarySize_ || maxx + ltrans[0] >=queryImage_.width_-searchBoundarySize_ ||
						miny + ltrans[1] <=searchBoundarySize_ || miny + ltrans[1] >=queryImage_.height_-searchBoundarySize_ || 
						maxy + ltrans[1] <=searchBoundarySize_ || maxy + ltrans[1] >=queryImage_.height_-searchBoundarySize_ )
					{
						x0++;
						continue;
					}
					else
					{
						count++;
						for (int k=0 ; k<tdbImage.nLines_ ; k++)
						{
							line = tdbImage.lines_[k];
							line.Translate(ltrans);
							sum = queryDistanceImage_.idtImages_[tdbImage.directionIndices_[k]].Sum((int)line.sx_,(int)line.sy_,(int)line.ex_,(int)line.ey_, currentcount);
							cost+=sum;
							if (cost > detectionThreshold)
							{
								cost = 1e+10;
								break;
							}
						}				
						cost *= factor;
						cost /= queryDistanceImage_.maxCost_;

					}
					imageIdx = x0 + y0*width;
					matchingCostMap.costMap_[costMapIndex][imageIdx] = cost;
					x0++;
				}
				y0++;
			}
			costMapIndex++;
		}
	}
}

void LMLineMatcher::SingleShapeDetectionWithVaryingQuerySize(LFLineFitter &lf,double maxThreshold,vector< vector<LMDetWind> > &detWindArrays)
{
	//LARGE_INTEGER t1, t2, f;
	////QueryPerformanceFrequency(&f);
	double scale;
	int i=0;
	//double structureCompTime = 0;
	//double matchingCostCompTime = 0;
	MatchingCostMap matchingCostMap;
	int nCostMap = (maxSearchScale_-minSearchScale_+1)*(maxSearchAspect_-minSearchAspect_+1);
	int costMapIndex = 0;
	matchingCostMap.Init(nCostMap);

	cout<<"[";
	for(int s=minSearchScale_;s<=maxSearchScale_;s++)
	{
		cout<<".";

		//
		// structure computation
		//		
		////QueryPerformanceCounter(&t1);

		scale = pow(baseSearchScale_,1.0*s);
		queryImage_.SetNumDirections(nDirections_);
		queryImage_.Read(lf);
		queryImage_.Scale(scale_*scale);
		queryDistanceImage_.Configure(directionCost_,maxCost_);
		queryDistanceImage_.SetImage(queryImage_);

		////QueryPerformanceCounter(&t2);
		//structureCompTime += (t2.QuadPart - t1.QuadPart)/(1.0*f.QuadPart);

		//mexPrintf("Scale = %lf\n",scale);
		//
		// compute matching cost
		//		
		////QueryPerformanceCounter(&t1);

		DetectBruteForce(dbImages_[i], maxThreshold, scale, matchingCostMap, costMapIndex);

		////QueryPerformanceCounter(&t2);
		//matchingCostCompTime += (t2.QuadPart - t1.QuadPart)/(1.0*f.QuadPart);

	}

	//cout<<"]"<<endl;
	//cout<<"Structure computation";
	//cout<<" ( "<<maxSearchScale_-minSearchScale_+1<<" scales )"<<" taking "<<structureCompTime<<" seconds"<<endl;

	//cout<<"Detection Computation";
	//cout<<" ( "<<maxSearchScale_-minSearchScale_+1<<" scales )"<<" taking "<<matchingCostCompTime<<" seconds"<<endl;

		
	////QueryPerformanceCounter(&t1);

	
	double overlapThreshold = 0.2;
	int nInterval = 1;
	int index = 0;
	int varyingQuerySize = 1;
	detWindArrays.resize(nInterval);	
	LMNonMaximumSuppression::ComputeDetection(matchingCostMap,maxThreshold,overlapThreshold,detWindArrays[index],varyingQuerySize);
	index++;
	////QueryPerformanceCounter(&t2);
	//cout<<"Non-maximum suppression ";
	//cout<<"taking "<<setiosflags(ios::fixed)<<setprecision(6)<<(t2.QuadPart - t1.QuadPart)/(1.0*f.QuadPart)<<endl;	
}
