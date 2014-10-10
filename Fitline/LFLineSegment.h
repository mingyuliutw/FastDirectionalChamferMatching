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

#ifndef _lf_line_segment_h_
#define _lf_line_segment_h_

#include <stdio.h>
#define	_USE_MATH_DEFINES
#include <math.h>
#undef	_USE_MATH_DEFINES

//#include <cxcore.h>
#include <iostream>
#include <vector>
#include "Point.h"

using namespace std;

class LFLineSegment
{
public:
	LFLineSegment() {};
	~LFLineSegment() {};

	double sx_,sy_,ex_,ey_;
	int nSupport_;
	double len_;
	Point<double> normal_;

	void Read(FILE* fin);	
	void Center(double *center);
	void Translate(double *vec);
	
	double Theta();

	void Aspect(double a);
	void Rotate(double theta);
	void Scale(double s);
	double Length();



	inline LFLineSegment& operator=(const LFLineSegment &rhs);	
	inline bool operator==(const LFLineSegment &other) const;
	inline bool operator!=(const LFLineSegment &other) const;
	inline bool operator>=(const LFLineSegment &other) const;
	inline bool operator<=(const LFLineSegment &other) const;
	inline bool operator>(const LFLineSegment &other) const;
	inline bool operator<(const LFLineSegment &other) const;
	inline friend ostream &operator<<(ostream &output,LFLineSegment &e);
};


inline LFLineSegment& LFLineSegment::operator=(const LFLineSegment &rhs)
{
	sx_ = rhs.sx_;
	sy_ = rhs.sy_;
	ex_ = rhs.ex_;
	ey_ = rhs.ey_;
	len_ = rhs.len_;

	nSupport_ = rhs.nSupport_;
	normal_ = rhs.normal_;
	return (*this);
}

ostream &operator<<(ostream &output,LFLineSegment &e)
{
	output<< "(" <<  e.sx_ <<", " << e.sy_ <<"), (" << e.ex_ <<", " << e.ey_ <<"), ";
    return output;
}


bool LFLineSegment::operator==(const LFLineSegment &other) const
{
	return len_ == other.len_;
}

bool LFLineSegment::operator!=(const LFLineSegment &other) const
{
	return len_ != other.len_;
}

bool LFLineSegment::operator>=(const LFLineSegment &other) const
{
	return len_ >= other.len_;
}

bool LFLineSegment::operator<=(const LFLineSegment &other) const
{
	return len_ <= other.len_;
}

bool LFLineSegment::operator>(const LFLineSegment &other) const
{
	return len_ > other.len_;
}

bool LFLineSegment::operator<(const LFLineSegment &other) const
{
	return len_ < other.len_;
}


#endif