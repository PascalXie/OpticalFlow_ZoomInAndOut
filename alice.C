#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

struct Pixel {
	int		Counter;
	float	Sum;
	float	GrayValue;
};

const int imgPixelNumber = 25;
const int nRow = 5;
const int nCol = 5;
const float PI = 3.1415926;

int main()
{
	void readImg(string fileName, float *img);
	void toPole(float *img, float *imgPole);
	void my_LK_Plus_2D(float *gray_ref, float *gray_est, float scale, float &ofx, float &ofy);
	cout<<"Hello"<<endl;

	/*
	// testing

	struct Pixel a; 
	a.Counter = 0;
	a.Sum = 0;
	a.GrayValue = 0;

	ifstream fread1, fread2;
	fread1.open("pic1.txt");
	fread2.open("pic2.txt");

	float gray[5];
	while(!fread1.eof())
	{
		fread1>>gray[0]>>gray[1]>>gray[2]>>gray[3]>>gray[4];
		cout<<gray[0]<<"	";
		cout<<gray[1]<<"	";
		cout<<gray[2]<<"	";
		cout<<gray[3]<<"	";
		cout<<gray[4]<<"	"<<endl;
	}
	*/


	// get image
	float img1[imgPixelNumber];
	float img2[imgPixelNumber];
	readImg("pic1.txt", img1);
	readImg("pic2.txt", img2);

	/*
	for(int i=0; i<imgPixelNumber; i++)
		cout<<"pic1.txt : "<<img1[i]<<endl;
	for(int i=0; i<imgPixelNumber; i++)
		cout<<"pic2.txt : "<<img2[i]<<endl;
	*/

	float img1_pole[nCol*2*nCol];
	float img2_pole[nCol*2*nCol];
	toPole(img1, img1_pole);
	toPole(img2, img2_pole);

	/*
	// 
	float Vx = 0;
	float Vy = 0;
	my_LK_Plus_2D(img1, img2, 100, Vx, Vy);
	cout<<"Velocity : Vx = "<<Vx<<", Vy = "<<Vy<<endl;
	*/


	
	return 0;
}

void readImg(string fileName, float *img)
{
	/*
	// array testing
	img[0] = 0;
	for(int i=0; i<25; i++)
		img[i] = 1.*i;
	*/

	ifstream fread;
	fread.open(fileName.c_str());

	for(int i=0; i<imgPixelNumber; i++)
		fread>>img[i];
}

void toPole(float *img, float *imgPole)
{
	int rows = nRow;
	int cols = nCol;

	float P_C_ImgORG_X = -1.*rows/2.;
	float P_C_ImgORG_Y = -1.*cols/2.;

	for(int i=0; i<rows; i++)
	for(int j=0; j<cols; j++)
	{
		float x = float(i)+P_C_ImgORG_Y;
		float y = float(j)+P_C_ImgORG_X;
		//float theta = atan2(float(i)+P_C_ImgORG_Y, float(j)+P_C_ImgORG_X);
		float theta = atan2(x, y);
		float r = sqrt(x*x+y*y);
		cout<<"i,j : "<<i<<" , "<<j<<"; theta = "<<(theta)/PI*180.<<";	r = "<<r<<endl;
		imgPole[i*cols+j]
	}
	
}


/*********************************************************************/
//	LK_Plus_2D (char version)
//	This function computes optical flow between two images curr_img //	and last_img using a version of Lucas-Kanade's algorithm
//	This algorithm assumes that displacements are generally on 
//	the order of one pixel or less. 
//
//	VARIABLES:
//	curr_img,last_img: first and second images
//	numpix: number of pixels in line image
//	rows: number of rows in image
//	cols: number of cols in image
//	scale: value of one pixel of motion (for scaling output)
//	ofx: pointer to integer value for X shift.
//	ofy: pointer to integer value for Y shift.
/*********************************************************************/
void my_LK_Plus_2D(float *gray_ref, float *gray_est, float scale, float &ofx, float &ofy)
{
	int rows = nRow;
	int cols = nCol;

	double  A11=0, A12=0, A22=0, b1=0, b2=0;
	double  F2F1, F4F3, FCF0;

	// set up pointers
	for(int i=1; i<rows-1; i++)
	for(int j=1; j<cols-1; j++)
	{
		//cout<<"i,j : "<<i<<" , "<<j<<endl;

		// compute differentials, then increment pointers (post	// increment)
		F2F1 = gray_est[i*cols+j-1]		- gray_est[i*cols+j+1]; //horizontal differential
		F4F3 = gray_est[(i-1)*cols+j]	- gray_est[(i+1)*cols+j]; //vertical differential
		FCF0 = gray_ref[i*cols+j]		- gray_est[i*cols+j];   //time differential
		//cout<<F2F1<<endl;
		//cout<<F4F3<<endl;
		//cout<<FCF0<<endl;

		// update summations
		A11 += (F2F1 * F2F1);
		A12 += (F4F3 * F2F1);
		A22 += (F4F3 * F4F3);
		b1  += (FCF0 * F2F1);                   
		b2  += (FCF0 * F4F3);                                   
	}

	//determinant
	double detA = A11*A22 - A12*A12;

	// Compute final output. Note use of "scale" here to multiply 2*top   
	// to a larger number so that it may be meaningfully divided using 
	// fixed point arithmetic
	ofx = b1*A22 - b2*A12 * scale / detA;
	ofy = b2*A11 - b1*A12 * scale / detA;
	cout<<ofx<<"	"<<ofy<<endl;
}
