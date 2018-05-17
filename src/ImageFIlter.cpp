#include "ImageFilter.h"
#include <math.h>

void convolve2D (uchar Y_buff[], int xSize, int ySize, double filterCoeff[], int N)
{
	//TO DO
	int sum, m, n;
	int delta = floor(N / 2);
	uchar* temp = new uchar[(xSize + 2 * delta) * (ySize + 2 * delta)];

	extendBorders(Y_buff, xSize, ySize, temp, delta);

	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			sum = 0;
			for (m = 0; m < N; m++) {
				for (n = 0; n < N; n++) {
					sum += temp[i + m + (j + n) * xSize] * filterCoeff[N - m - 1 + (N - n - 1) * N];
				}
			}
			Y_buff[j * xSize + i] = abs(sum);
		}
	}
}

void extendBorders(uchar input[], int xSize, int ySize, uchar output[], int delta)
{
	//TO DO
	for (int i = 0; i < ((xSize + 2 * delta) * (ySize + 2 * delta)); i++) {
		output[i] = 0;
	}

	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			output[i + delta + (j + delta) * xSize] = input[i + j * xSize];
		}
	}
}
	
void performNFFilter (uchar input[], int xSize, int ySize)
{
	//TO DO
	double NFCoeffs[9];
	for (int i = 0; i < 9; i++)
		NFCoeffs[i] = 1.0 / 9.0;
	convolve2D(input, xSize, ySize, NFCoeffs, 3);
}

void performVFFilter (uchar input[], int xSize, int ySize)
{
	//TO DO
	double VFCoeffs[9] = { 0.0, -1.0/4.0, 0.0, -1.0/4.0, 4.0/4.0, -1.0/4.0, 0.0, -1.0/4.0, 0.0 };
	convolve2D(input, xSize, ySize, VFCoeffs, 3);

}

void performSuccessiveNFFilter (uchar input[], int xSize, int ySize, int stages)
{
	//TO DO
	double NFCoeffs[9];
	for (int i = 0; i < 9; i++)
		NFCoeffs[i] = 1.0 / 9.0;

	for (int i = 0; i < stages; i++) {
		convolve2D(input, xSize, ySize, NFCoeffs, 3);
	}
}

void performSobelEdgeDetection(uchar input[], int xSize, int ySize, double threshold)
{
	//TO DO
	uchar* temp = new uchar[(xSize) * (ySize)];
	for (int i = 0; i < xSize*ySize; i++) {
			temp[i] = input[i];
	}

	
	double sobel_v[9] = { -1.0/4.0, -2.0/4.0, -1.0/4.0, 0.0, 0.0, 0.0, 1.0/4.0, 2.0/4.0, 1.0/4.0 };
	double sobel_h[9] = { -1.0/4.0, 0.0, 1.0/4.0, -2.0/4.0, 0.0, 2.0/4.0, -1.0/4.0, 0.0, 1.0/4.0 };

	convolve2D(input, xSize, ySize, sobel_h, 3);	  // u input mi je Horizontal
	convolve2D(temp, xSize, ySize, sobel_v, 3);		  // u temp mi je Vertical

	for (int i = 0; i < xSize*ySize; i++) {
		auto res =sqrt((input[i]* input[i])+(temp[i]* temp[i]));
		if (res >= threshold) {
			input[i] = 255;
		}
		else{
			input[i] = 0;
		}
	}
}

void performNFplusSobelEdgeDetection(uchar input[], int xSize, int ySize, int stages, uchar threshold)
{

	//TO DO
	double NFCoeffs[9];
	for (int i = 0; i < 9; i++)
		NFCoeffs[i] = 1.0 / 9.0;

	for (int i = 0; i < stages; i++) {
		convolve2D(input, xSize, ySize, NFCoeffs, 3);
	}

	uchar* temp = new uchar[(xSize) * (ySize)];
	for (int i = 0; i < xSize*ySize; i++) {
		temp[i] = input[i];
	}


	double sobel_v[9] = { -1.0 / 4.0, -2.0 / 4.0, -1.0 / 4.0, 0.0, 0.0, 0.0, 1.0 / 4.0, 2.0 / 4.0, 1.0 / 4.0 };
	double sobel_h[9] = { -1.0 / 4.0, 0.0, 1.0 / 4.0, -2.0 / 4.0, 0.0, 2.0 / 4.0, -1.0 / 4.0, 0.0, 1.0 / 4.0 };

	convolve2D(input, xSize, ySize, sobel_h, 3);	  // u input mi je Horizontal
	convolve2D(temp, xSize, ySize, sobel_v, 3);		  // u temp mi je Vertical

	for (int i = 0; i < xSize*ySize; i++) {
		auto res = sqrt((input[i] * input[i]) + (temp[i] * temp[i]));
		if (res >= threshold) {
			input[i] = 255;
		}
		else {
			input[i] = 0;
		}
	}
	
}
