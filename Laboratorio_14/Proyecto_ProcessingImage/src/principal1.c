/*******************************************************
  * Para probar con la versión en paralelo con N hilos
  * debes definir la constante durante la compilación
  * gracias a la opción -D:
  * gcc principal.c -otest -DPARALLEL = N -fopenmp -Wall

********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <omp.h>

#define NB_PIXELS 256

typedef struct color_pixel_struct {    // colores basicos del pixel
    unsigned char r,g,b;
} color_pixel_type;

typedef struct color_image_struct
{
  int width, height;    		// atributos de la imagen
  color_pixel_type * pixels;     // vector de pixeles
} color_image_type;

typedef struct grey_image_struct    //Es una estructura que captura los atributos de la imagen
{
  int width, height;
  unsigned char * pixels;          // vector de pixeles
} grey_image_type;


/**********************************************************************/
color_image_type * loadColorImage(char *filename){
  int i, width,height,max_value;
  char format[8];
  color_image_type * image;
  FILE * f = fopen(filename,"r");
  if (!f){
    fprintf(stderr,"Cannot open file %s...\n",filename);
    exit(-1);
  }
  fscanf(f,"%s\n",format);
  assert( (format[0]=='P' && format[1]=='3'));  // check P3 format
  while(fgetc(f)=='#') // commentaire
    {
      while(fgetc(f) != '\n'); // ir al final de la línea
    }
  fseek( f, -1, SEEK_CUR);
  fscanf(f,"%d %d\n", &width, &height);
  fscanf(f,"%d\n", &max_value);
  image = malloc(sizeof(color_image_type));
  assert(image != NULL);
  image->width = width;
  image->height = height;
  image->pixels = malloc(width*height*sizeof(color_pixel_type));
  assert(image->pixels != NULL);

  for(i=0 ; i<width*height ; i++){
      int r,g,b;
      fscanf(f,"%d %d %d", &r, &g, &b);
      //printf("\n Pixel %d",i);
      image->pixels[i].r = (unsigned char) r;
      image->pixels[i].g = (unsigned char) g;
      image->pixels[i].b = (unsigned char) b;
    }
  fclose(f);
  return image;
}

/**********************************************************************/

//Crea una representacion en la memoria

grey_image_type * createGreyImage(int width, int height){
  grey_image_type * image = malloc(sizeof(grey_image_type));
  assert(image != NULL);
  image->width = width;
  image->height = height;
  image->pixels = malloc(width*height*sizeof(unsigned char));
  assert(image->pixels != NULL);
  return(image);
}

/**********************************************************************/

void saveGreyImage(char * filename, grey_image_type *image){
  int i;
  FILE * f = fopen(filename,"w");
  if (!f){
    fprintf(stderr,"Cannot open file %s...\n",filename);
    exit(-1);
  }
  fprintf(f,"P2\n%d %d\n255\n",image->width,image->height);
  for(i=0 ; i<image->width*image->height ; i++){
    fprintf(f,"Pixel %d %d\n",i,image->pixels[i]);
  }
  fclose(f);
}

/*******************************************************
 * Transforms a color image PPM to grey PGM
********************************************************/
grey_image_type* colorImageToGrey(color_image_type *colorImage){  //devuelve variable tipo grey_image_
  double t, start, stop;  											// atributos de la imagen
  start = omp_get_wtime();
  int width=colorImage->width, height=colorImage->height;
  grey_image_type *greyImage=createGreyImage(width, height);     //Crea un tipo grey_image_type (pixeles)
  int r, g, b;
#ifdef PARALLEL
#pragma omp parallel for private(r, g, b) num_threads(PARALLEL)
#endif
  for (int i = 0; i < width * height; i++){
    r=colorImage->pixels[i].r;              //obtiene el color rgb
    g=colorImage->pixels[i].g;
    b=colorImage->pixels[i].b;
    greyImage->pixels[i] = (299 * r +587 * g + 114 * b) / 1000;  //Crea un vector de tipo grey_image_type (pixeles)
    															//Lo trasforma a un color gris y lo guarda en la imagen gris
  }
  stop = omp_get_wtime();
  t = stop - start;
  printf("TEMPS-colorToGrey: %lf\n", t);
  //FILE * f = fopen("src/image0.ppm","r");
  return greyImage;    						//Retorna la imagen en gris

}

int main(int argc, char* argv[]){

  saveGreyImage("src/resize/image0_grey.pgm",colorImageToGrey(loadColorImage("src/image0.ppm")));
  saveGreyImage("src/resize/image2_grey.pgm",colorImageToGrey(loadColorImage("src/image2.ppm")));
  //color_image_type nuevo;
  //colorImageToGrey(nuevo);

  return 0;
}
