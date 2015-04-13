#include <iostream>
#include <assert.h>
#include <cstdlib>
#include <thread>
using namespace std;


// Point computation (serial)
// Implemented in smdb.h
int ComputeMandelbrotPoint(int x, int y);

// Implement the thread function which will be called by the spawned threads
//
//
// This function is called by the main thread
// It will spawn and join threads, calling a thread function that
// you'll implement
//

int ComputeMandelbrotPointXY(int **pts, int startY, int endY, int dimX){
    for(int i = 0; i < dimX; i++){
        for(int j = startY; j < endY; j++){
            pts[j][i] = ComputeMandelbrotPoint(i,j);
        }
    }

}

// If the return code is false, disable plotting and reporting which won't
// work if the multithraded code hasn't been implemented
bool Mandelbrot_multiThreaded(int** pts, int dimX, int dimY, int numThreads, int chunkSize) 
{
    //cerr << "Not yet implemented.. " << endl;
    // Change the return code to true when you've imlemented this code
    // A return code of false will disable plotting, as plotting
    // won't function correctly with an empty image.


    std::thread *thrds = new thread[numThreads];

    //checking whether chunkSize is too big or too small
    if(chunkSize > dimX || chunkSize > dimY || chunkSize < 0){
    	cerr << " Chunksize exceed limits" << endl;
    	return false;
    }

    int block = dimY/numThreads;
    //BLOCK case
    if(chunkSize == 0 || chunkSize == block){
    	for(int i = 0; i < numThreads; i++){
            std::thread thrds[i] = thread(ComputeMandelbrotPointXY, pts, i*block, (i+1)*block, dimX);
        }
    }
    //CYCLIC case
    else if(chunkSize > 0 && chunkSize < block){
        int i = 0, j = 0;
        while(j*chunkSize >= dimY){
            std::thread thrds[i++] = thread(ComputeMandelbrotPointXY, pts, j*chunkSize, (j+1)*chunkSize, dimX);
            if(i >= numThreads){
                i = 0;
            }
        }        
    }

    for(int i = 0; i < numThreads; i++){
        thrds[i].join();
    }


    return false;
  //////////// add your code here //////////////////////////////////
}


