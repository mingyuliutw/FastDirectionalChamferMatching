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
#include "LFLineSegment.h"

void LFLineSegment::Read(FILE* fin)
{
	fscanf(fin, "%lf %lf", &sx_, &sy_);
	fscanf(fin, "%lf %lf", &ex_, &ey_);
}

double LFLineSegment::Theta()
{
	double theta = atan2(ey_-sy_,ex_-sx_);
	if (theta<0)
		theta += M_PI;
	return theta;
}

void LFLineSegment::Aspect(double a)
{
	sx_ *= a;
	ex_ *= a;
}

void LFLineSegment::Center(double *center)
{
	center[0] = (sx_ + ex_) / 2;
	center[1] = (sy_ + ey_) / 2;
}

void LFLineSegment::Translate(double *vec)
{
	sx_ += vec[0];
	sy_ += vec[1];

	ex_ += vec[0];
	ey_ += vec[1];
}


void LFLineSegment::Rotate(double theta)
{
	double x, y;
	x = sx_, y = sy_;

	double sinTheta;
	double cosTheta;
	double mat[2][2];

	sinTheta = sin(theta);
	cosTheta = cos(theta);
	mat[0][0] = cosTheta;
	mat[0][1] = -sinTheta;
	mat[1][0] = sinTheta;
	mat[1][1] = cosTheta;

	sx_ = x*mat[0][0] + y*mat[0][1];
	sy_ = x*mat[1][0] + y*mat[1][1];

	x = ex_, y = ey_;
	ex_ = x*mat[0][0] + y*mat[0][1];
	ey_ = x*mat[1][0] + y*mat[1][1];
}

void LFLineSegment::Scale(double s)
{
	sx_ *= s;
	sy_ *= s;
	ex_ *= s;
	ey_ *= s;

}

double LFLineSegment::Length()
{
	double x, y;
	x = ex_ - sx_;
	y = ey_ - sy_;

	len_ = sqrt(x*x+y*y);
	return len_;
}
