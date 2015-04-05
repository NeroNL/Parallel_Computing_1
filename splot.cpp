/* **********************************************************
 *  Author : Urvashi R.V. [04/06/2004]
 * 	Modified by Scott Baden [10/8/06]
 * 	Modified by Pietro Cicoctti [10/8/08]
 * 
 *************************************************************/

#include <stdio.h>
#include <math.h>

#include <algorithm>
#include <fstream>
#include "util.h"
#include "pngwriter.h"

/* Function to plot the 2D array
 * 'gnuplot' is instantiated via a pipe and 
 * the values to be plotted are passed through, along 
 * with gnuplot commands */

using namespace std;

extern float re_min, re_max, im_min, im_max;

void ConvertMdbToRGB(int** image, int max, int x, int y,ColorB** rgb);

FILE* gnu = NULL;

void splotMdb(int** image, int max, int x, int y)
{
  ColorB** rgb = new ColorB*[y];
  for(int i=0; i < y; ++i)
    rgb[i] = new ColorB[x];

  ConvertMdbToRGB(image, max, x, y, rgb);
  ofstream f("temp.rgb", ios::out | ios::binary);
  for(int i=0; i < y; ++i)   
    f.write((char*)&rgb[i][0], x * sizeof(ColorB) );  
  
  f.close();
 
  for(int i=0; i < y; ++i)
    delete [] rgb[i];
  delete [] rgb;

  if (!gnu)
    gnu = popen("gnuplot -persist","w");
  
  fprintf(gnu,"plot \"temp.rgb\" binary array=%ix%i format='%%uchar' with rgbimage \n", x, y);
  
  //  remove("temp.rgb"); 
}

void splotPNG2(char* filename, int x, int y)
{
  float fb = 255 / (float)(65535);
  pngwriter png;
  png.readfromfile(filename);

  ColorB** rgb = new ColorB*[y];
  for(int i=0; i < y; ++i)
    rgb[i] = new ColorB[x];

  for( int i=0; i < y; ++i)
    for(int j=0; j < x; ++j) {
      rgb[i][j].r = (unsigned char)(png.read(j, i, 1) * fb);
      rgb[i][j].g = (unsigned char)(png.read(j, i, 2) * fb);
      rgb[i][j].b = (unsigned char)(png.read(j, i, 3) * fb);
    }

  ofstream f("temp.rgb", ios::out | ios::binary);
  for(int i=0; i < y; ++i)   
    f.write((char*)&rgb[i][0], x * sizeof(ColorB) );  
  
  f.close();
  png.close();

  for(int i=0; i < y; ++i)
    delete [] rgb[i];
  delete [] rgb;

  // plot in gnuplot
  if (!gnu)
    gnu = popen("gnuplot -persist","w");

  //  fprintf(gnu, "set xlabel 'Real'\n");
  // fprintf(gnu, "set ylabel 'Imaginary'\n");
  fprintf(gnu,"set key off\n");
  fprintf(gnu,"set pm3d map\n");

  fprintf(gnu,"plot \"temp.rgb\" binary array=%ix%i format='%%uchar' with rgbimage \n", x, y);
}

void splotPNG(char* filename)
{
  if (!gnu)
    gnu = popen("gnuplot -persist","w");

    //fprintf(gnu,"set title ");
    //fprintf(gnu,"set size square\n");
    fprintf(gnu,"set term png\n");
    fprintf(gnu,"unset tics\n");

    fprintf(gnu,"plot \"%s\" binary filetype=png w rgbimage\n", filename);
   
    fflush(gnu);

    return;
}

#if 0
void splot(int **image, int x, int y)
{
  if(!gnu) gnu = popen("gnuplot -persist","w");
  auto mx= -1, mn = 32768;
  for (auto j=0; j<y; j++)
     for (auto i=0; i<x; i++){
        if (image[j][i] > mx)
            mx = image[j][i];
        if (image[j][i] < mn)
            mn = image[j][i];
  }
  fprintf(gnu,"\n");
  fprintf(gnu,"\n");
  fprintf(gnu,"set size square\n");
  fprintf(gnu,"set key off\n");
  fprintf(gnu,"set pm3d map\n");
  fprintf(gnu,"set palette defined (-3 \"blue\", 0 \"white\", 1 \"red\")\n");
  // fprintf(gnu, "set xlabel 'Real'\n");
  //fprintf(gnu, "set ylabel 'Imaginary'\n");
    /* Various color schemes
     * fprintf(gnu,"set palette rgbformulae 22, 13, 31\n");
     * fprintf(gnu,"set palette rgbformulae 30, 31, 32\n");
    */

  fprintf(gnu,"splot [0:%d] [0:%d][%d:%d] \"-\"\n",x-1,y-1,mn,mx);
  for (auto j=0; j<y; j++){
       for (auto i=0; i<x; i++)
           fprintf(gnu,"%d %d %d\n", i, j, image[j][i]);
       fprintf(gnu,"\n");
  }
  fprintf(gnu,"e\n");
  fflush(gnu);
  return;
}
#endif
