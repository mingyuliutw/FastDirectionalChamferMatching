/********************************************************************
*			Fast directional chamfer matching code					*
*																	*	
*	Ming-Yu Liu (UMD), Oncel Tuzel (MERL), 							*
*	Ashok Veeraraghavan (MERL) and Rama Chellappa (UMD)				*
*     						                                        *
/********************************************************************

/* Copyright 2011, Ming-Yu Liu

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

/********************************************************************
* 						General information 						*
/********************************************************************
This is a MATLAB wrapper of the Fast Directional Chamfer Matching algorithm. The project webpage can be found in http://www.umiacs.umd.edu/~mingyliu. 
For questions concerning the code please contact Ming-Yu Liu via <mingyliu AT umiacs DOT umd DOT edu>. 

If you find this software useful for your research, please cite the following paper. 

@inproceedings{liu_cvpr2010,
    title = {Fast Directional Chamfer Matching},
    author = {Ming-Yu Liu and Oncel Tuzel and Ashok Veeraraghavan and Rama Chellappa},
    booktitle = {Proc. IEEE Conf. on Computer Vision and Pattern Recognition},
    year = {2010},
    publisher = {IEEE Computer Society}
}


/********************************************************************
* 			Installation 											*
/********************************************************************


1) Unzipped the files to $HOME_DIR   

2) We have provided pre-compiled mex functions. You may not need to recompile the mex files. If you need to recompile the codes, type >>make in the matlab command prompt. This should do the job. We have tested the code on 
		MATLAB 2008 + Windows 7 32-bit
		MATLAB 2008 + Windows 7 64-bit
		MATLAB 2010 + Redhat4.1.2 64-bit
You should not encounter any compiling problem. But if any, please feel free to contact me.

3) Type demo to see a demonstration of the usage of the code.

4) !!!!! IMPORTANT !!!!! Though we demonstrate the algorithm using real images, it actually takes pre-computed edge maps as input. In order to compute the edge map, you can use (a) Canny edge detector, (b) Edison software, or (c) Berkeley PB edge detectors. (The provided edge maps are computed with PB edge detector).

(a) If you have MATLAB or OpenCV softwares, they have built-in Canny edge detector.
(b) Edison is a software developed by the vision research group in Rutgers University. You can find the link via http://coewww.rutgers.edu/riul/research/code.html
(c) Berkeley probability boundary edge detector is probably the most robust edge detector developed so far. But it takes a significant amount of time to compute the edge map. To download the edge detector, please go to http://www.eecs.berkeley.edu/Research/Projects/CS/vision/bsds/.


/********************************************************************
* 			Usage		 											*
/********************************************************************
1) mex_fitline : convert an edge map to a linear representation

	usage: [lineRep lineMap] = mex_fitline( templateEdgeMap, lineFittingPara);
		Input: 
			templateEdgeMap: edge map in double format
			lineFittingPara: structure for the fitting parameters
			
		Output:
			lineRep: an array of size ( # of lines ) x 4
			lineMap: approximation of the edge map with the line representation.


2) mex_fdcm_detection : detect object instances in the query image using 
						the template shape.

	usage: [detWinds] = mex_fdcm_detect(query,tempate,threshold,...
								lineFittingPara2,lineMatchingPara);
		Input:
			query: the edge map of the query image
			tempate: a cell array of the templates. Each entry of the cell 
					is the lineRep computed from mex_fitline.
			threshold: detection threshold. The larger the more the detections.
			lineFittingPara2: structure for configuring the line-fitting parameters
			lineMatchingPara: structure for configuring the matching parameters
		
		Output:
			detWinds: an array of (# of detecions) x 6
					The first and second columns are the x and y coordinates of the 
					top-left corner of the detection window. The 3rd and 4th columns are
					the width and height respectively. The 5th column contains the matching
					cost. The 6th column is the number of detection windows covered by the 
					current detection window in the nonmaximum-suppression process.
					
			
	
	
/********************************************************************
* 			Release notes:											*
/********************************************************************
2011, January 1: release version 0.2

