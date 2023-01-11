#include <stdlib.h>
#include <stdbool.h>
#include "myfunction1.h"
#include "showBMP.h"
/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
void initialize_pixel_sum(pixel_sum *sum)
{
	sum->red = sum->green = sum->blue = 0;
	// sum->num = 0;
	return;
}

/*
 * assign_sum_to_pixel - Truncates pixel's new value to match the range [0,255]
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum, int kernelScale)
{

	// divide by kernel's weight
	sum.red = sum.red / kernelScale;
	sum.green = sum.green / kernelScale;
	sum.blue = sum.blue / kernelScale;

	// truncate each pixel's color values to match the range [0,255]
	current_pixel->red = (unsigned char)(min(max(sum.red, 0), 255));
	current_pixel->green = (unsigned char)(min(max(sum.green, 0), 255));
	current_pixel->blue = (unsigned char)(min(max(sum.blue, 0), 255));
	return;
}

/*
 * sum_pixels_by_weight - Sums pixel values, scaled by given weight
 */
static void sum_pixels_by_weight(pixel_sum *sum, pixel p, int weight)
{
	sum->red += ((int)p.red) * weight;
	sum->green += ((int)p.green) * weight;
	sum->blue += ((int)p.blue) * weight;
	// sum->num++;
	return;
}
/*
 * Apply the kernel over each pixel.
 * Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than kernelSize/2 and/or
 * column index smaller than kernelSize/2
 */
void smooth(int dim, pixel *src, pixel *dst, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter)
{

	int i, j;
	int sumR = 0;
	int sumG = 0;
	int sumB = 0;

	for (i = 1; i < dim - 1; i++)
	{
		for (j = 1; j < dim - 1; j++)
		{
			sumR = 0;
			sumG = 0;
			sumB = 0;
			int ii, jj;
			// pixel_sum sum;
			int min_loc;
			int max_loc;
			int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
			int max_intensity = -1;	 // arbitrary value that is lower than minimum possible intensity, which is 0
			int sumColor;
			pixel loop_pixel;
			int pos = (i - 1) * dim + j - 1;
			for (ii = 0; ii < 3; ii++)
			{
				for (jj = 0; jj < 3; jj++)
				{
					loop_pixel = src[pos];
					// apply kernel on pixel at [ii,jj]
					sumR += ((int)loop_pixel.red) * kernel[ii][jj];
					sumG += ((int)loop_pixel.green) * kernel[ii][jj];
					sumB += ((int)loop_pixel.blue) * kernel[ii][jj];

					if(filter){
						sumColor = (int)loop_pixel.red + (int)loop_pixel.green + (int)loop_pixel.blue;
						if (sumColor <= min_intensity)
						{
							min_intensity = sumColor;
							min_loc = pos;
						}
						if (sumColor > max_intensity)
						{
							max_intensity = sumColor;
							max_loc = pos;
						}
					}


					pos++;
				}
				pos = pos - 3 + dim;
			}
				if (filter)
			{
				// filter out min and max
				sumR += ((int)src[min_loc].red) * -1;
				sumG += ((int)src[min_loc].green) * -1;
				sumB += ((int)src[min_loc].blue) * -1;
				sumR += ((int)src[max_loc].red) * -1;
				sumG += ((int)src[max_loc].green) * -1;
				sumB += ((int)src[max_loc].blue) * -1;

			}

			// assign kernel's result to pixel at [i,j]
			// divide by kernel's weight
			sumR = sumR / kernelScale;
			sumG = sumG / kernelScale;
			sumB = sumB / kernelScale;

			// truncate each pixel's color values to match the range [0,255]
			dst[i * dim + j].red = (unsigned char)(min(max(sumR, 0), 255));
			dst[i * dim + j].green = (unsigned char)(min(max(sumG, 0), 255));
			dst[i * dim + j].blue = (unsigned char)(min(max(sumB, 0), 255));

			// dst[i * dim + j] = current_pixel;
		}
	}
}

void charsToPixels(Image *charsImg, pixel *pixels)
{

	int row, col;
	for (row = 0; row < m; row++)
	{
		for (col = 0; col < n; col++)
		{
			pixels[row * n + col].red = image->data[3 * row * n + 3 * col];
			pixels[row * n + col].green = image->data[3 * row * n + 3 * col + 1];
			pixels[row * n + col].blue = image->data[3 * row * n + 3 * col + 2];
		}
	}
}

void pixelsToChars(pixel *pixels, Image *charsImg)
{

	int row, col;
	for (row = 0; row < m; row++)
	{
		for (col = 0; col < n; col++)
		{

			image->data[3 * row * n + 3 * col] = pixels[row * n + col].red;
			image->data[3 * row * n + 3 * col + 1] = pixels[row * n + col].green;
			image->data[3 * row * n + 3 * col + 2] = pixels[row * n + col].blue;
		}
	}
}

void copyPixels(pixel *src, pixel *dst)
{

	int row, col;
	for (row = 0; row < m; row++)
	{
		for (col = 0; col < n; col++)
		{

			dst[row * n + col].red = src[row * n + col].red;
			dst[row * n + col].green = src[row * n + col].green;
			dst[row * n + col].blue = src[row * n + col].blue;
		}
	}
}

void doConvolution(Image *image, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter)
{

	pixel *pixelsImg = malloc(m * n * sizeof(pixel));
	pixel *backupOrg = malloc(m * n * sizeof(pixel));

	charsToPixels(image, pixelsImg);
	copyPixels(pixelsImg, backupOrg);

	smooth(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale, filter);

	pixelsToChars(pixelsImg, image);

	free(pixelsImg);
	free(backupOrg);
}
