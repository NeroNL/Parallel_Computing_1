    //////////// DO NOT MODIFY THIS FILE ////////////////////////

#include <stdlib.h>
#include <time.h>
#include <vector>
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include "Plotter.h"

    using namespace std;

    double getTime();
    void Mandelbrot_thread(int** points, int dimX, int dimY, int numThreads, int chunkSize );
    void ComputeMandelbrot(int **points, int dimX, int dimY);
    void WriteImage(int** image, int maxValue, int dimX, int dimY, char* filename);
    void Report(ofstream& logfile, bool runSerial, int threads, int chunkSize, bool verify, int** pts, int dimX, int dimY, double ptime, int maxIterations, double *Window);
    void splotPNG(char* filename);
    void splotPNG2(char* filename, int dimX, int dimY);
    void splotMdb(int** image, int maxValue, int dimX, int dimY);
    //void splot(int **image, int x, int y);
    int **alloc2D(int m,int n);


    int cmdLine(int argc, char** argv, int &numThreads, bool &runSerial, int &imgX, int &imgY, float& re_min, float& im_min, float& re_max, float& im_max, int &doPlot, int &chunkSize, char *filename, bool &verify, int &maxIterations, string& cLine);

    //global variables for the range of Mandelbrot calculation
    int maxIterations = 200;

    float scaleRe, scaleIm;
    float re_min, im_min, re_max, im_max;

    int main(int argc, char* argv[]) 
    {
            // number of worker threads for ComputeMandelbrot_thread
            int numThreads = 1;	
            bool runSerial = false;
            bool verify = false;
            int imgX = 1024;  // size of x/real dimension
            int imgY = 1024;  // size of y/imaginary dimension
            int doPlot = false;
            int chunkSize = 0; // 0 -> BLOCK partitioning
                            // other values -> chunk size for cylic paritioning

            double sTime = 0.0;
            double pTime = 0.0;
            double t1, t2;
            char filename[100] = "\0";

            re_min=-2.5;
            im_min = -1;
            re_max=1;
            im_max = 1;
            string cLine;

            // processing command line arguments
            if(!cmdLine(argc, argv, numThreads, runSerial, imgX, imgY, re_min, im_min, re_max, im_max, doPlot, chunkSize, filename, verify, maxIterations, cLine) )
            return 0;

        Plotter *plotter = NULL;
        if (doPlot){
            plotter = new Plotter();
        }
	scaleRe = (re_max - re_min)/imgX;
	scaleIm = (im_max - im_min)/imgY;
      
	// print out parameters
	cout << "max iterations: " << maxIterations << endl;
	cout << "image size: " << imgX << " x " << imgY << endl;
        cout << "Window: [" << re_min << " : " << re_max << "] x [";
        cout << im_min << " : " << im_max << "]" << endl;

	int **image = alloc2D(imgY,imgX);

	// parallel Mandelbrot
	t1 = getTime();

	if(runSerial)
	  ComputeMandelbrot(image, imgX, imgY);
	else
	//////////////////////////////////////////////////////////////////////
	//
	// Implement this routine, contained in pmdb.cpp
	// Do not change the function signature, which is defined in this file
	//
	  Mandelbrot_thread(image, imgX, imgY, numThreads, chunkSize);
	//////////////////////////////////////////////////////////////////////
	t2 = getTime();
	pTime += t2 - t1;
	
	ofstream logfile("log");
        double Window[4] = { re_min , re_max , im_min,im_max};
	Report(logfile, runSerial, numThreads, chunkSize, verify, image, imgX, imgY, pTime,maxIterations,Window);
	
	// write the visualization of Mandelbrot set to a png image file
	if(filename[0] != 0) {
	  WriteImage(image, maxIterations, imgX, imgY, filename);
	  // plot the image with gnuplot
	  // splotPNG2(filename, imgX, imgY);
	}
	if (doPlot){
//	  splot(image, imgX, imgY);
          plotter->Plot(image, imgX, imgY, cLine);
	}

	free(image);

	return 0;
}
