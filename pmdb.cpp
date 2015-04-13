#include <iostream>
#include <assert.h>
#include <cstdlib>
#include <thread>
using namespace std;


// Point computation (serial)
// Implemented in smdb.h
int ComputeMandelbrotPoint(int x, int y);



/*
 *  This function is the thread function of Mandelbrot_multiThreaded function.
 *  It calls function ComputeMandelbrotPoint in order to have the data in
 *  each grid of the image.
 *  It has 4 paramaters;
 *  pts is a double pointer of integer representing the image
 *  startY indicates the beginning of the Y bound for each thread
 *  endY indicates the end of the Y bound for each thread
 *  dimX represents the length of X dimension for each thread
 */
int ComputeMandelbrotPointXY(int **pts, int startY, int endY, int dimX){
    //For each grid in image, calcualte the data 
    //by calling function ComputeMandelBrotpoint
    for(int i = 0; i < dimX; i++){
        for(int j = startY; j < endY; j++){
            pts[j][i] = ComputeMandelbrotPoint(i,j);
        }
    }

}

/*
 *  This function's purpose is to have the same result of serial program 
 *  in a faster way by running parallel program. It has 5 paramaters:
 *  pts is a double pointer of integer representing the image
 *  dimX represents the length of x dimension
 *  dimY represents the length of y dimension
 *  numThreads represents the number of threads we use in this funcion
 *  chunkSize represents whether we should use block or cyclic method
 */
bool Mandelbrot_multiThreaded(int** pts, int dimX, 
    int dimY, int numThreads, int chunkSize) 
{
    //a set of threads that we will spawn for parallel programming
    std::thread *thrds = new thread[numThreads];

    //a variable indicating how many blocks that we should seperate the image
    int block = dimY/numThreads;

    //checking whether chunkSize is too big or too small
    //if it is, then print out error and return false;
    if(chunkSize > dimX || chunkSize > dimY || chunkSize < 0){
    	cerr << " Chunksize exceed limits" << endl;
    	return false;
    }

    //start parallel computing
    //BLOCK case
    if(chunkSize == 0 || chunkSize == block){
        //spawn threads
    	for(int i = 0; i < numThreads; i++){
            //assign works into different threads evenly
            thrds[i] = thread(ComputeMandelbrotPointXY, pts, 
                i*block, (i+1)*block, dimX);
        }
    }
    //CYCLIC case   
    else if(chunkSize > 0 && chunkSize < block){
        // i indicates which thread we are working on
        // j indicates which bound we should choose for each thread
        int i = 0, j = 0;
        //spawn threads
        while(j*chunkSize >= dimY){
            //assign works into different threads evenly
            thrds[i++] = thread(ComputeMandelbrotPointXY, pts, 
                (j++)*chunkSize, j*chunkSize, dimX);
            // make sure we dont run out of threads to get out of bound error
            if(i >= numThreads){
                i = 0;
            }
        }        
    }

    //after running all the threats, we make them join 
    //in order to terminate the threads safely 
    for(int i = 0; i < numThreads; i++){
        thrds[i].join();
    }

    //indicate we have implemented parallel function
    return true;
}


