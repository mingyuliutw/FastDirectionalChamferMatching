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

#ifndef _point_h_
#define _point_h_


#include <stdio.h>
#define	_USE_MATH_DEFINES
#include <math.h>
#undef	_USE_MATH_DEFINES
#include <iostream>
#include <vector>

using namespace std;

template <class T>
class Point
{
public:
	Point() {};
	Point(T xInput,T yInput): x(xInput), y(yInput) {};
	T x;
	T y;
	inline Point& operator=(const Point &rhs);	
};

template <class T> 
Point<T>& Point<T>::operator=(const Point &rhs)
{
	x = rhs.x;
	y = rhs.y;
	return (*this);
}

#endif