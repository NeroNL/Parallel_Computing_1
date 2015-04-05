//
// Do not change the code in this file, as doing so
// could cause your submission to be graded incorrectly
//
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>

using namespace std;

double getTime();
void ComputeMandelbrot(int** points, int dimX, int dimY);
int **alloc2D(int m, int n);

int CompareResults(int** a, int** b, int dimX, int dimY)
{
  int error = 0;
  for(int i=0; i < dimY; ++i) {
    for(int j=0; j < dimX; ++j) {
      if(a[i][j] != b[i][j] )
	error++;
    }
  }
  return error;
}

void Report(ofstream& logfile, bool runSerial, int numThreads, int chunkSize, bool verify, int** pts, int dimX, int dimY, double ptime, int maxIterations, double *Window)

{
  int error = 0;
  char chunkMsg[100];
  if( runSerial) {
    cout << "running serial Mandelbrot... "<< endl; 
    logfile << "running serial Mandelbrot... "<< endl; 
  }
  else {
    chunkSize == 0 ? sprintf(chunkMsg, "BLOCK partitioning") : sprintf(chunkMsg, "cyclic partitioning, chunk size = %i", chunkSize);
    cout << "running parallel Mandelbrot with "<< numThreads << " threads, " << chunkMsg << endl;
    logfile << "running parallel Mandelbrot with "<< numThreads << " threads, " << chunkMsg << endl;
  }

  if (!runSerial && verify) {
    int** ref = alloc2D(dimY, dimX);

    cout << "Verifying results : " << flush;
    logfile << "Verifying results : " << flush;
    
    double sTime = -getTime();
    ComputeMandelbrot(ref, dimX, dimY);
    sTime += getTime();

    error = CompareResults(ref, pts, dimX, dimY);

    logfile <<  error << " errors(s)" << endl;
    cout <<  error << " errors(s)" << endl;

    cout << "Serial run time: " << sTime << " sec" << endl;

    delete [] ref;
  }

  if(runSerial) {
    logfile << "Serial run time: " << ptime << endl;
    cout << "Serial run time: " << ptime << " sec" <<  endl;
  }
  else {
    logfile << "Parallel run time: " << ptime << endl;
    cout << "Parallel run time: " << ptime << " sec" <<  endl;
  }

  string OneLiner;
  OneLiner = "@> ";
  OneLiner += to_string(maxIterations);
  OneLiner += " ";
  OneLiner += to_string(dimX); OneLiner += " ";
  OneLiner += to_string(dimY); OneLiner += " ";
  for (int k=0; k<4; k++){
    OneLiner += to_string(Window[k]);
  OneLiner += " ";
  }
  OneLiner += to_string(Window[1]);
  OneLiner += " ";
  if(runSerial) {
      OneLiner += "S ";
  }
  else{
    OneLiner += "P ";
    OneLiner += to_string(numThreads); OneLiner += " ";
    if (chunkSize == 0) 
        OneLiner += "B";
    else 
        OneLiner += "C ";
        OneLiner += to_string(chunkSize); OneLiner += " ";
  }
  OneLiner += " ";

  char * cstr = new char [OneLiner.length()+1];
  std::strcpy (cstr, OneLiner.c_str());
  logfile << cstr << endl;
  cout << cstr << endl;

}

