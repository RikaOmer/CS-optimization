#include <stdlib.h>
#include <stdbool.h>
#include "myfunction1.h"
#include "showBMP.h"

void charsToPixels(Image *charsImg, pixel *pixels)
{

	int row, col;
	int pos = 0;
	int tPos = 0;
	for (row = 0; row < m; row++)
	{
		for (col = 0; col < n; col++)
		{
			pixels[pos].red = image->data[tPos];
			pixels[pos].green = image->data[tPos + 1];
			pixels[pos].blue = image->data[tPos + 2];
			pos++;
			tPos = tPos + 3;
		}
	}
}

void pixelsToChars(pixel *pixels, Image *charsImg)
{

	int row, col;
	int pos = 0;
	int tPos = 0;
	for (row = 0; row < m; row++)
	{
		for (col = 0; col < n; col++)
		{

			image->data[tPos] = pixels[pos].red;
			image->data[tPos + 1] = pixels[pos].green;
			image->data[tPos + 2] = pixels[pos].blue;
			pos++;
			tPos = tPos + 3;
		}
	}
}

void copyPixels(pixel *src, pixel *dst)
{

	int row, col;
	int pos = 0;
	for (row = 0; row < m; row++)
	{
		for (col = 0; col < n; col++)
		{

			dst[pos].red = src[pos].red;
			dst[pos].green = src[pos].green;
			dst[pos].blue = src[pos].blue;
			pos++;
		}
	}
}

void smoothB(int dim, Image* image)
{
		int x = m * n * 3;
	pixel *dst = malloc(x);
	pixel *src = malloc(x);
	charsToPixels(image, dst);
	copyPixels(dst, src);

	int i, j;
	for (i = 1; i < dim - 1; i++)
	{
		for (j = 1; j < dim - 1; j++)
		{
			int sumR = 0;
			int sumG = 0;
			int sumB = 0;
			int ii, jj;
			// pixel_sum sum;
			pixel loop_pixel;
			int pos = (i - 1) * dim + j - 1;
			for (ii = 0; ii < 3; ii++)
			{
				for (jj = 0; jj < 3; jj++)
				{
					loop_pixel = src[pos];
					// apply kernel on pixel at [ii,jj]
					sumR += ((int)loop_pixel.red);
					sumG += ((int)loop_pixel.green);
					sumB += ((int)loop_pixel.blue);

					pos++;
				}
				pos = pos - 3 + dim;
			}
			pos =i * dim + j;
			// assign kernel's result to pixel at [i,j]
			// divide by kernel's weight
			sumR = sumR / 9;
			sumG = sumG / 9;
			sumB = sumB / 9;

			// truncate each pixel's color values to match the range [0,255]
			dst[pos].red = (unsigned char)(min(max(sumR, 0), 255));
			dst[pos].green = (unsigned char)(min(max(sumG, 0), 255));
			dst[pos].blue = (unsigned char)(min(max(sumB, 0), 255));

			// dst[i * dim + j] = current_pixel;
		}
	}
	pixelsToChars(dst, image);
	free(dst);
	free(src);
}

void smoothS(int dim, Image* image)
{
	int x = m * n * 3;
	pixel *dst = malloc(x);
	pixel *src = malloc(x);
	charsToPixels(image, dst);
	copyPixels(dst, src);
	int i, j;
	for (i = 1; i < dim - 1; i++)
	{
		for (j = 1; j < dim - 1; j++)
		{
			int sumR = 0;
			int sumG = 0;
			int sumB = 0;
			int ii, jj;
			// pixel_sum sum;
			pixel loop_pixel;
			int pos = (i - 1) * dim + j - 1;
			for (ii = 0; ii < 1; ii++)
			{
				for (jj = 0; jj < 3; jj++)
				{
					loop_pixel = src[pos];
					// apply kernel on pixel at [ii,jj]
					sumR += ((int)loop_pixel.red) * -1;
					sumG += ((int)loop_pixel.green) * -1;
					sumB += ((int)loop_pixel.blue) * -1;

					pos++;
				}
				pos = pos - 3 + dim;
			}
			loop_pixel = src[pos];
			// apply kernel on pixel at [ii,jj]
			sumR += ((int)loop_pixel.red) * -1;
			sumG += ((int)loop_pixel.green) * -1;
			sumB += ((int)loop_pixel.blue) * -1;

			pos++;
			loop_pixel = src[pos];
			// apply kernel on pixel at [ii,jj]
			sumR += ((int)loop_pixel.red) * 9;
			sumG += ((int)loop_pixel.green) * 9;
			sumB += ((int)loop_pixel.blue) * 9;

			pos++;
			loop_pixel = src[pos];
			// apply kernel on pixel at [ii,jj]
			sumR += ((int)loop_pixel.red) * -1;
			sumG += ((int)loop_pixel.green) * -1;
			sumB += ((int)loop_pixel.blue) * -1;

			pos = pos - 2 + dim;
			for (ii = 2; ii < 3; ii++)
			{
				for (jj = 0; jj < 3; jj++)
				{
					loop_pixel = src[pos];
					// apply kernel on pixel at [ii,jj]
					sumR += ((int)loop_pixel.red) * -1;
					sumG += ((int)loop_pixel.green) * -1;
					sumB += ((int)loop_pixel.blue) * -1;

					pos++;
				}
				pos = pos - 3 + dim;
			}
			pos = i * dim + j;
			// truncate each pixel's color values to match the range [0,255]
			dst[pos].red = (unsigned char)(min(max(sumR, 0), 255));
			dst[pos].green = (unsigned char)(min(max(sumG, 0), 255));
			dst[pos].blue = (unsigned char)(min(max(sumB, 0), 255));

			// dst[i * dim + j] = current_pixel;
		}
	}
	pixelsToChars(dst, image);
	free(dst);
	free(src);
}

void smoothFB(int dim, Image* image)
{
	int x = m * n * 3;
	pixel *dst = malloc(x);
	pixel *src = malloc(x);
	charsToPixels(image, dst);
	copyPixels(dst, src);

	int i, j;
	for (i = 1; i < dim - 1; i++)
	{
		for (j = 1; j < dim - 1; j++)
		{
			int sumR = 0;
			int sumG = 0;
			int sumB = 0;
			int ii, jj;
			// pixel_sum sum;
			int min_loc;
			int max_loc;
			int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
			int max_intensity = -1;	 // arbitrary value that is lower than minimum possible intensity, which is 0
			pixel loop_pixel;
			int pos = (i - 1) * dim + j - 1;
			for (ii = 0; ii < 3; ii++)
			{
				for (jj = 0; jj < 3; jj++)
				{
					loop_pixel = src[pos];
					// apply kernel on pixel at [ii,jj]
					sumR += ((int)loop_pixel.red);
					sumG += ((int)loop_pixel.green);
					sumB += ((int)loop_pixel.blue);

					int sumColor = (int)loop_pixel.red + (int)loop_pixel.green + (int)loop_pixel.blue;
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
					pos++;
				}
				pos = pos - 3 + dim;
			}
			pos = pos - 2 * dim + 1;
			// filter out min and max
			loop_pixel = src[min_loc];
			sumR += ((int)loop_pixel.red) * -1;
			sumG += ((int)loop_pixel.green) * -1;
			sumB += ((int)loop_pixel.blue) * -1;
			loop_pixel = src[max_loc];
			sumR += ((int)loop_pixel.red) * -1;
			sumG += ((int)loop_pixel.green) * -1;
			sumB += ((int)loop_pixel.blue) * -1;

			// assign kernel's result to pixel at [i,j]
			// divide by kernel's weight
			sumR = sumR / 7;
			sumG = sumG / 7;
			sumB = sumB / 7;

			// truncate each pixel's color values to match the range [0,255]
			dst[pos].red = (unsigned char)(min(max(sumR, 0), 255));
			dst[pos].green = (unsigned char)(min(max(sumG, 0), 255));
			dst[pos].blue = (unsigned char)(min(max(sumB, 0), 255));

			// dst[i * dim + j] = current_pixel;
		}
	}
	pixelsToChars(dst, image);
	free(dst);
	free(src);
}

void smoothRB(int dim, Image* image)
{
	int x = m * n * 3;
	pixel *dst = malloc(x);
	pixel *src = malloc(x);
	charsToPixels(image, dst);
	copyPixels(dst, src);
	int i, j;
	for (i = 1; i < dim - 1; i++)
	{
		for (j = 1; j < dim - 1; j++)
		{
			int sumR = 0;
			int sumG = 0;
			int sumB = 0;
			int ii, jj;
			// pixel_sum sum;
			pixel loop_pixel;
			int pos = i * dim + j - 1;

			loop_pixel = src[pos];

			// apply kernel on pixel at [ii,jj]
			sumR += ((int)loop_pixel.red);
			sumG += ((int)loop_pixel.green);
			sumB += ((int)loop_pixel.blue);
			pos++;
			loop_pixel = src[pos];

			sumR += ((int)loop_pixel.red) * 2;
			sumG += ((int)loop_pixel.green) * 2;
			sumB += ((int)loop_pixel.blue) * 2;

			pos++;
			loop_pixel = src[pos];

			sumR += ((int)loop_pixel.red);
			sumG += ((int)loop_pixel.green);
			sumB += ((int)loop_pixel.blue);

			// divide by kernel's weight
			sumR = sumR / 4;
			sumG = sumG / 4;
			sumB = sumB / 4;
			pos = i * dim + j;
			// truncate each pixel's color values to match the range [0,255]
			dst[pos].red = (unsigned char)(min(max(sumR, 0), 255));
			dst[pos].green = (unsigned char)(min(max(sumG, 0), 255));
			dst[pos].blue = (unsigned char)(min(max(sumB, 0), 255));

			// dst[i * dim + j] = current_pixel;
		}
	}
	pixelsToChars(dst, image);
	free(dst);
	free(src);
}

void smoothRS(int dim, Image* image)
{
	int x = m * n * 3;
	pixel *dst = malloc(x);
	pixel *src = malloc(x);
	charsToPixels(image, dst);
	copyPixels(dst, src);
	int i, j;
	for (i = 1; i < dim - 1; i++)
	{
		for (j = 1; j < dim - 1; j++)
		{
			int sumR = 0;
			int sumG = 0;
			int sumB = 0;
			int ii, jj;
			// pixel_sum sum;
			int sumColor;
			pixel loop_pixel;
			int pos = i * dim + j - 1;

			loop_pixel = src[pos];

			// apply kernel on pixel at [ii,jj]
			sumR += ((int)loop_pixel.red) * (-1);
			sumG += ((int)loop_pixel.green) * (-1);
			sumB += ((int)loop_pixel.blue) * (-1);

			pos++;
			loop_pixel = src[pos];

			// apply kernel on pixel at [ii,jj]
			sumR += ((int)loop_pixel.red) * 3;
			sumG += ((int)loop_pixel.green) * 3;
			sumB += ((int)loop_pixel.blue) * 3;

			pos++;
			loop_pixel = src[pos];

			// apply kernel on pixel at [ii,jj]
			sumR += ((int)loop_pixel.red) * (-1);
			sumG += ((int)loop_pixel.green) * (-1);
			sumB += ((int)loop_pixel.blue) * (-1);
			pos = i * dim + j;
			// truncate each pixel's color values to match the range [0,255]
			dst[pos].red = (unsigned char)(min(max(sumR, 0), 255));
			dst[pos].green = (unsigned char)(min(max(sumG, 0), 255));
			dst[pos].blue = (unsigned char)(min(max(sumB, 0), 255));

			// dst[i * dim + j] = current_pixel;
		}
	}
	pixelsToChars(dst, image);
	free(dst);
	free(src);
}
