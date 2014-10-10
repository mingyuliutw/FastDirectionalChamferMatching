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


#ifndef _lm_det_wind_h_
#define _lm_det_wind_h_

#include <iostream>
#include <fstream>

using namespace std;

class LMDetWind
{
public:
	int x_;
	int y_;
	int width_;
	int height_;
	double cost_;
	int count_;

	inline LMDetWind &operator=(const LMDetWind &rhs);
	inline bool operator==(const LMDetWind &other) const;
	inline bool operator!=(const LMDetWind &other) const;
	inline bool operator>=(const LMDetWind &other) const;
	inline bool operator<=(const LMDetWind &other) const;
	inline bool operator>(const LMDetWind &other) const;
	inline bool operator<(const LMDetWind &other) const;
	inline friend ostream &operator<<(ostream &output,LMDetWind &e);
};


ostream &operator<<(ostream &output,LMDetWind &e)
{
	//output<< "(" <<  e.x_ <<", " << e.y_ <<", " << e.width_ <<", " << e.height_ <<", " << e.cost_ <<", " << e.count_ <<")";
	output<< e.x_ <<" "<< e.y_ <<" "<< e.width_ <<" " << e.height_ <<" " << e.cost_ <<" " << e.count_;
    return output;
}


LMDetWind &LMDetWind::operator=(const LMDetWind &rhs)
{
	x_ = rhs.x_;
	y_ = rhs.y_;
	width_ = rhs.width_;
	height_ = rhs.height_;
	cost_ = rhs.cost_;
	count_ = rhs.count_;
	return *this;
}

bool LMDetWind::operator==(const LMDetWind &other) const
{
	return cost_ == other.cost_;
}

bool LMDetWind::operator!=(const LMDetWind &other) const
{
	return cost_ != other.cost_;
}

bool LMDetWind::operator>=(const LMDetWind &other) const
{
	return cost_ >= other.cost_;
}

bool LMDetWind::operator<=(const LMDetWind &other) const
{
	return cost_ <= other.cost_;
}

bool LMDetWind::operator>(const LMDetWind &other) const
{
	return cost_ > other.cost_;
}

bool LMDetWind::operator<(const LMDetWind &other) const
{
	return cost_ < other.cost_;
}

#endif