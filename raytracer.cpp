#include <iostream>
#include <string>
#include <sstream>


#include <time.h>
#include <math.h>
using namespace std;

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;

//Résolution de l'image par défaut
#define XDIM 1000
#define YDIM 1000

#define INF 2e10f
#define SPHERES 20

#define rnd(x) (x*rand()/RAND_MAX)

typedef struct pixel {
	int r,g,b;
} t_pixels;

struct Sphere {
	float r,g,b;
	float radius;
	float x,y,z;

	float hit (float ox, float oy, float *n) {
		float dx = ox - x;
		float dy = oy - y;
		if (dx*dx +dy*dy < radius*radius) {
			float dz = sqrtf (radius*radius-dx*dx-dy*dy);
			*n = dz/ sqrtf (radius*radius);
			return dz+z;
		}
		return -INF;
	}
};

Sphere * initSpheres (int n, int xDim, int yDim) {

	Sphere * tmp = (Sphere*)malloc(SPHERES*sizeof(Sphere));
	for (int i = 0;i<SPHERES;i++) {
		tmp[i].r = rnd(1.0f);
		tmp[i].g = rnd(1.0f);
		tmp[i].b = rnd(1.0f);
		tmp[i].x = rnd(1000.0f)-1000.0f/2;
		tmp[i].y = rnd(1000.0f)-1000.0f/2;
		tmp[i].z = rnd(1000.0f)-500.0f;
		tmp[i].radius = rnd(100.0f)+20;	
	}
	return tmp;
}


//Kernel de raytracing
void kernel (t_pixels *image, Sphere *s, int nx, int ny) {

	for (int idx = 0; idx < nx*ny; idx++) { 
		int x = idx/nx;
		int y = idx%nx;

		float ox = (x-nx/2);
		float oy = (y-ny/2);
		float r=0,g=0,b=0;
		float maxz = -INF;

		for (int i=0;i<SPHERES;i++) {
			float n;
			float t=s[i].hit(ox,oy,&n);
			if (t>maxz) {
				float fscale = n;
				r = s[i].r*fscale;
				g = s[i].g*fscale;
				b = s[i].b*fscale;
				maxz = t;
			}
		}
		image [idx].r = (int)(r*255);
		image [idx].g = (int)(g*255);
		image [idx].b = (int)(b*255);
	}
}



int main (int argc, char * argv[]) {

	// Initialisation du générateur aléatoire
	time_t t;
	srand((unsigned) time(&t)); 
	
	// Nom du fichier de sauvegarde de l'image
	if (argc <2) {
		cout <<"Vous n'avez pas donné de nom de fichier" << endl;
		exit (EXIT_FAILURE);
	}

	//Dimensions de l'écran
	int xDim = XDIM; 
	int yDim = YDIM;

	if (argc == 4) {
		xDim = atoi(argv[2]);
		yDim = atoi(argv[3]);
	}

	Mat M(xDim,yDim,CV_8UC3,Scalar(0,0,0));

	//création des objets
	
	Sphere * h_sphere = initSpheres (SPHERES,xDim,yDim);

	
	const int size = xDim*yDim; 
	t_pixels *h_image = NULL;
	h_image = (t_pixels*)malloc(size*sizeof(t_pixels)); 

	kernel (h_image,h_sphere,xDim,yDim);
	int ix,iy;
	for (int i=0;i<size;i++) {
		ix=i/xDim;
		iy=i%yDim;
		M.at<Vec3b>(ix,iy)[0] = h_image[i].r;
		M.at<Vec3b>(ix,iy)[1] = h_image[i].g;
		M.at<Vec3b>(ix,iy)[2] = h_image[i].b;
	}
	imshow ("Rendu",M);
	//waitKey(0);
	free(h_image);
	free(h_sphere);
	return EXIT_SUCCESS;
}