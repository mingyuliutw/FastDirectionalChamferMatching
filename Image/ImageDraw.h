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
#ifndef _image_draw_h_
#define _image_draw_h_

#include <cstdlib>
#include <climits>
#include <cstring>
#include <fstream>
#include <cmath>
#include "Image.h"

template <class T>
class ImageDraw
{
public:
	inline static void Line(Image<T> *im,int sx,int sy,int ex,int ey,const T scalar,int thickness=1);
};

template <class T>
inline void ImageDraw<T>::Line(Image<T> *im,int sx,int sy,int ex,int ey,const T scalar,int thickness)
{
	double dx = 1.0*ex-sx;
	double dy = 1.0*ey-sy;
	double ds;
	int x,y;
	int xindexed = 0;
	int count = -1;

	if (abs(dx) > abs(dy))
	{
		ds = dy / (dx + 1e-10);
		xindexed = 1;		
	}
	else
	{
		ds = dx / (dy + 1e-10);
		xindexed = 0;
	}

	int tsx,tex,tsy,tey;
	tsx = sx;
	tsy = sy;
	tex = ex;
	tey = ey;

	for(int t=0;t<thickness;t++)
	{


		if (xindexed)
		{
			sx = tsx;
			sy = tsy + (t - thickness/2);
			ex = tex;
			ey = tey + (t - thickness/2);
			if(ex>sx)
			{
				for (int i=sx ; i<=ex;i++)
				{
					count++;
					y = sy+(int)ceil(count*ds-0.5);
					if(y<0||y>=im->height()||i<0||i>=im->width())
						continue;
					imRef(im,i,y) = scalar;
				}
			}
			else
			{
				for (int i=ex ; i<=sx;i++)
				{
					count++;
					y = ey+(int)ceil(count*ds-0.5);
					if(y<0||y>=im->height()||i<0||i>=im->width())
						continue;
					imRef(im,i,y) = scalar;
				}
			}
		}
		else
		{
			sx = tsx + (t - thickness/2);
			sy = tsy;
			ex = tex + (t - thickness/2);
			ey = tey;

			if(ey>sy)
			{
				for (int i=sy ; i<=ey;i++)
				{
					count++;
					x = sx+(int)ceil(count*ds-0.5);
					if(x<0||x>=im->width()||i<0||i>=im->height())
						continue;
					imRef(im,x,i) = scalar;
				}
			}
			else
			{
				for (int i=ey ; i<=sy;i++)
				{
					count++;
					x = ex+(int)ceil(count*ds-0.5);
					if(x<0||x>=im->width()||i<0||i>=im->height())
						continue;
					imRef(im,x,i) = scalar;
				}
			}
		}
	}
}

#endif