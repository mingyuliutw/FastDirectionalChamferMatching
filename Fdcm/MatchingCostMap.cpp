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

#include "MatchingCostMap.h"

MatchingCostMap::MatchingCostMap()
{
	costMap_ = NULL;
	nCostMap_ = 0;

}

MatchingCostMap::~MatchingCostMap()
{
	Release();	
}
	
void MatchingCostMap::Release()
{
	if(costMap_)
	{
		for(int i=0;i<nCostMap_;i++)
			costMap_[i].clear();
		delete [] costMap_;
		nCostMap_ = 0;
	}
	templateWidth_.clear();
	templateHeight_.clear();
	width_.clear();
	height_.clear();
	stepSize_.clear();
	x0_.clear();
	y0_.clear();
	scale_.clear();
	aspect_.clear();
}

void MatchingCostMap::Init(int nCostMap)
{
	nCostMap_ = nCostMap;
	costMap_ = new vector<double> [nCostMap_];
	templateWidth_.resize(nCostMap_);
	templateHeight_.resize(nCostMap_);
	width_.resize(nCostMap_);
	height_.resize(nCostMap_);
	stepSize_.resize(nCostMap_);
	x0_.resize(nCostMap_);
	y0_.resize(nCostMap_);
	scale_.resize(nCostMap_);
	aspect_.resize(nCostMap_);
}