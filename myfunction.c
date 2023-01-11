#include <stdbool.h> 
#include "myutil.h"
#include "myfunction1.h"
#include "myfunction.h"
#include "writeBMP.h"


void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* rowBlurRsltImgName, char* rowSharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {

	/*
	* [1, 1, 1]
	* [1, 1, 1]
	* [1, 1, 1]
	*/
	int blurKernel[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

	/*
	* [-1, -1, -1]
	* [-1, 9, -1]
	* [-1, -1, -1]
	*/
	int sharpKernel[3][3] = {{-1,-1,-1},{-1,9,-1},{-1,-1,-1}};

	/*
	* [0, 0, 0]
	* [1, 2, 1]
	* [0, 0, 0]
	*/
	int rowBlurKernel[3][3] = {{0, 0, 0}, {1, 2, 1}, {0, 0, 0}};

	/*
	* [0, 0, 0]
	* [-2, 6, -2]
	* [0, 0, 0]
	*/
	int rowSharpKernel[3][3] = {{0, 0, 0},{-2,6,-2},{0, 0, 0}};

	if (flag == '1') {	
		// blur image
		smoothB(image->sizeX, image);
		//doConvolution(image , 0); //blurKernel

		// write result image to file
		writeBMP(image, srcImgpName, blurRsltImgName);	

		// sharpen the resulting image
		smoothS(image->sizeX, image);
		
		// write result image to file
		writeBMP(image, srcImgpName, sharpRsltImgName);	
    } else if (flag == '2') {	
		// blur image with row-blurring kernel
		smoothRB(image->sizeX, image);

		// write result image to file
		writeBMP(image, srcImgpName, rowBlurRsltImgName);

		// sharpen the resulting image with row-sharpening kernel
		smoothRS(image->sizeX, image);

		// write result image to file
		writeBMP(image, srcImgpName, rowSharpRsltImgName);	
	} else {
		// apply extermum filtered kernel to blur image
		smoothFB(image->sizeX, image);

		// write result image to file
		writeBMP(image, srcImgpName, filteredBlurRsltImgName);

		// sharpen the resulting image
		smoothS(image->sizeX, image);

		// write result image to file
		writeBMP(image, srcImgpName, filteredSharpRsltImgName);	
	}
}

