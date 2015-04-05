#include <iostream>
#include <assert.h>
#include <cstdlib>
#include <thread>
using namespace std;


// Point computation (serial)
// Implemented in smdb.h
int ComputeMandelbrotPoint(int x, int y, int dimX, int dimY);

// Implement the thread function which will be called by the spawned threads
//
//
// This function is called by the main thread
// It will spawn and join threads, calling a thread function that
// you'll implement
//

// If the return code is false, disable plotting and reporting which won't
// work if the multithraded code hasn't been implemented
bool Mandelbrot_multiThreaded(int** pts, int dimX, int dimY, int numThreads, int chunkSize) 
{
    cerr << "Not yet implemented.. " << endl;
    // Change the return code to true when you've imlemented this code
    // A return code of false will disable plotting, as plotting
    // won't function correctly with an empty image.
    return false;
  //////////// add your code here //////////////////////////////////
}


