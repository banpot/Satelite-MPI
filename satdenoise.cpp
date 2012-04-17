/* 
############################################################
# Program by : Banpot Dolwithayakul
#
# Compile With : mpicc satdenoise.cpp -I/usr/include/opencv -lopencv_core -lopencv_highgui -lopencv_ml -lm
############################################################
*/

#include <stdio.h>
#include "mpi.h"
#include <omp.h>
#include <cv.h>
#include <highgui.h>
#include <string.h>

#define ALPHA 6
#define MAXITE 30
#define BETA 0.0001
#define h 1
#define FILENAME "nitz.jpg"
#define NOISELEVEL 0.00

double z_r[2000][2000];
double z_g[2000][2000];
double z_b[2000][2000];

double r[2000][2000];
double g[2000][2000];
double b[2000][2000];

double ux_r[2000][2000];
double ux_g[2000][2000];
double ux_b[2000][2000];

double uy_r[2000][2000];
double uy_g[2000][2000];
double uy_b[2000][2000];

double d_r[2000][2000];
double d_g[2000][2000];
double d_b[2000][2000];

double dd_r[2000][2000];
double dd_g[2000][2000];
double dd_b[2000][2000];

int width, height;

//##### WGN Generating Function ######//
float gennoise(){
/* random number in range 0 - 1 not including 1 */
float random = 0;

/* the white noise */
float noise = 0;

long int q = 15;
float c1 = (1 << q) - 1;
float c2 = ((int)(c1 / 3)) + 1;
float c3 = 1.f / c1;


for (int i = 0; i < 15; i++)
{
    random = ((float)rand() / (float)(RAND_MAX));
    noise = (2 * ((random * c2) + (random * c2) + (random * c2)) - 3.f * (c2 - 1.f)) * c3;
}

return noise;
}

//##### Dynamically Load Image ######//
void loadImage(){
	//string pPath;
	//pPath = getenv ("HOME");
	cv::Mat_<cv::Vec3b> img = cv::imread(FILENAME, 1);
	printf("Data size is (#cols, #rows) = (%d, %d)\n", img.cols, img.rows);
	cv::Mat_<int> labels(width * height, 1);
	width = img.cols;
	height = img.rows;

// Create and populate color matrix
	cv::Mat_<float> color_matrix(width*height, 3);
	cv::Vec3b vec3b;
	int index = 0;
	for (int y = 0; y < img.rows; ++y) {
		for (int x = 0; x < img.cols; ++x)
		{
			vec3b = img(y, x);
			r[y][x] = float( vec3b[0]);
			g[y][x] = float( vec3b[1]);
			b[y][x] = float( vec3b[2]);

			//z_r[y][x] = float( vec3b[0]);
			//z_g[y][x] = float( vec3b[1]);
			//z_b[y][x] = float( vec3b[2]);

			r[y][x] += (gennoise()*NOISELEVEL*255);	
			if (r[y][x]>255){r[y][x]=255;}
			if (r[y][x]<0){r[y][x]=0;}
			z_r[y][x] = r[y][x];


			g[y][x] += (gennoise()*NOISELEVEL*255);	
			if (g[y][x]>255){g[y][x]=255;}
			if (g[y][x]<0){g[y][x]=0;}
			z_g[y][x] = g[y][x];


			b[y][x] += (gennoise()*NOISELEVEL*255);	
			if (b[y][x]>255){b[y][x]=255;}
			if (b[y][x]<0){b[y][x]=0;}
			z_b[y][x] = b[y][x];

		}
	}
} // End function


int main()
{
// Init MPI
MPI::Init();

// Declare Internal Variable
int p, id;

//  Get the number of processes.
p = MPI::COMM_WORLD.Get_size();

//  Get the individual process ID.
id = MPI::COMM_WORLD.Get_rank();

// MPI::COMM_WORLD.Send ( &h[1], 1, MPI::DOUBLE, id-1, tag );

// Terminate
MPI::Finalize();

}
