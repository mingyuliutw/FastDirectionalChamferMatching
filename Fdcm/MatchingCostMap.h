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

#ifndef _matching_cost_map_h_
#define _matching_cost_map_h_

#include "MMFunctions.h"
#include <vector>
using namespace std;

class MatchingCostMap
{
public:
	MatchingCostMap();
	~MatchingCostMap();
	void Release();
	void Init(int nCostMap);




	vector<double> *costMap_;
	int nCostMap_;

	vector<int> width_;
	vector<int> height_;
	vector<int> templateWidth_;
	vector<int> templateHeight_;
	vector<int> stepSize_;
	vector<int> x0_;
	vector<int> y0_;
	vector<double> scale_;
	vector<double> aspect_;


};


#endif