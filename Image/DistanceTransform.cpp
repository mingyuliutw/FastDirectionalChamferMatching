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
#include "DistanceTransform.h"

// dt of binary image using squared distance
void DistanceTransform::CompDT(const Image<uchar> *input,Image<float> *output,bool onEqualOne,Image<int> *nn)
{

	uchar on;
	on = (onEqualOne) ? 255 : 0;

	int imageWidth = input->width();
	int imageHeight = input->height();

	// Initialize distance transform value
	//for (int k = 0; k < imageHeight*imageWidth; k++) 
	//{
	//	if( cvGetReal1D(input,k) == on )
	//	{
	//		cvSetReal1D( output, k, 0);
	//		//cvSet2D( output, y, x, cvScalar( 0 ) );
	//	}
	//	else
	//	{
	//		cvSetReal1D( output, k, INF);
	//		//cvSet2D( output, y, x, cvScalar( INF ) );
	//	}	
	//}

	for(int y=0;y<imageHeight;y++)
	{
		for(int x=0;x<imageWidth;x++)
		{
			if( imRef(input,x,y) == on )
				imRef(output,x,y) = 0;
			else
				imRef(output,x,y) = INF;
		}
	}

	if(nn)
	{
		Update2DDTCostNN(output,nn);
	}
	else
	{
		Update2DDTCost(output);
	}

	// square root
	for (int y = 0; y < imageHeight; y++) 
	{
		for (int x = 0; x < imageWidth; x++) 
		{
			//cvSetReal2D( output, y, x, sqrt( cvGetReal2D( output, y, x) ) );
			imRef(output,x,y) = sqrt( imRef(output,x,y) ) ;
		}
	}
}


/* dt of 2d function using squared distance */
void DistanceTransform::Update2DDTCostNN(Image<float> *output,Image<int> *nn)
{
	int imageWidth = output->width();
	int imageHeight = output->height();
	float *f = new float[ max( imageWidth, imageHeight ) ];
	int *ind = new int[ max( imageWidth, imageHeight ) ]; 

	// transform along columns
	for (int x = 0; x < imageWidth; x++) 
	{
		for (int y = 0; y < imageHeight; y++) 
		{
			//f[ y ] = (double)cvGetReal2D( output, y, x);
			f[ y ] = imRef(output,x,y);
			ind[ y ] = y*imageWidth + x;
		}
		float *d = Update1DDTCostNN(f, imageHeight, ind);
		for (int y = 0; y < imageHeight; y++) 
		{
			//cvSetReal2D( output, y, x, d[y]);
			imRef(output,x,y) = d[y];
			//cvSetReal2D( nn, y, x, (double)ind[ y ]);
			imRef(nn,x,y) = ind[y];
		}
		delete [] d;
	}


	// transform along rows
	for (int y=0; y<imageHeight; y++) 
	{
		for( int x = 0; x<imageWidth; x++) 
		{
			//f[ x ] = (double)cvGetReal2D( output, y, x);
			f[y] = imRef(output,x,y);
			//ind[ x ] = (int) cvGetReal2D( nn, y, x);
			ind[ x ] = imRef(nn,x,y);
		}
		float *d = Update1DDTCostNN(f, imageWidth, ind);
		for (int x = 0; x < imageWidth; x++) 
		{
			//cvSetReal2D( output, y, x, d[x]);
			imRef( output, x, y) = d[x];
			//cvSetReal2D( nn, y, x, (double)ind[ x ]);
			imRef( nn, x, y) = ind[x];
		}
		delete [] d;
	}


	delete [] ind;
	delete [] f;

}


// dt of 1d function using squared distance
float* DistanceTransform::Update1DDTCostNN(float *f, const int n, int *ind)
{
	float *d = new float [n];
	int *v = new int[n];
	float *z = new float[n+1];

	int *tmpInd = new int [n];

	int k = 0;
	v[0] = 0;
	z[0] = -INF;
	z[1] = +INF;

	for (int q=1; q<=n-1; q++) 
	{
		float s = ((f[q]+square(q)) - (f[v[k]]+square(v[k]))) / (2*q - 2*v[k]);
		while ( s <= z[k] ) 
		{
			k--;
			s = ((f[q]+square(q)) - (f[v[k]]+square(v[k]))) / (2*q - 2*v[k]);
		}
		k++;
		v[k] = q;
		z[k] = s;
		z[k+1] = INF;

	}

	k = 0;
	for (int q=0; q<=n-1; q++) 
	{
		while ( z[k+1] < q )
		{
			k++;
		}
		d[q] = square(q-v[k]) + f[v[k]];
		tmpInd[q] = ind[v[k]];
	}
	memcpy(ind,tmpInd,sizeof(int)*n);

	delete [] tmpInd;
	delete [] v;
	delete [] z;
	return d;
}
































// dt of 2d function using squared distance
void DistanceTransform::Update2DDTCost(Image<float> *output)
{
	int imageWidth = output->width();
	int imageHeight = output->height();
	float *f = new float[ max( imageWidth, imageHeight ) ];
	int *ind = NULL;


	// transform along columns
	for (int x = 0; x < imageWidth; x++) 
	{
		for (int y = 0; y < imageHeight; y++) 
		{
			//f[ y ] = (double)cvGetReal2D( output, y, x);
			f[y] = imRef(output,x,y);
		}
		float *d = Update1DDTCost(f, imageHeight);
		for (int y = 0; y < imageHeight; y++) 
		{
			//cvSetReal2D( output, y, x, d[y]);
			imRef(output,x,y) = d[y];
		}
		delete [] d;
	}

	for (int y=0; y<imageHeight; y++) 
	{
		for( int x = 0; x<imageWidth; x++) 
		{
			//f[ x ] = (double)cvGetReal2D( output, y, x);
			f[ x ] = imRef(output,x,y);
		}
		float *d = Update1DDTCost(f, imageWidth);
		for (int x = 0; x < imageWidth; x++) 
		{
			//cvSetReal2D( output, y, x, d[x]);
			imRef( output, x, y) = d[x];
		}
		delete [] d;
	}

	delete [] f;

}

// dt of 1d function using squared distance
float* DistanceTransform::Update1DDTCost(float *f, const int n)
{
	float *d = new float [n];
	int *v = new int[n];
	float *z = new float[n+1];

	int k = 0;
	v[0] = 0;
	z[0] = -INF;
	z[1] = +INF;

	for (int q=1; q<=n-1; q++) 
	{
		float s = ((f[q]+square(q)) - (f[v[k]]+square(v[k]))) / (2*q - 2*v[k]);
		while ( s <= z[k] ) 
		{
			k--;
			s = ((f[q]+square(q)) - (f[v[k]]+square(v[k]))) / (2*q - 2*v[k]);
		}
		k++;
		v[k] = q;
		z[k] = s;
		z[k+1] = INF;

	}

	k = 0;
	for (int q=0; q<=n-1; q++) 
	{
		while ( z[k+1] < q )
		{
			k++;
		}
		d[q] = square(q-v[k]) + f[v[k]];

	}

	delete [] v;
	delete [] z;
	return d;
}
