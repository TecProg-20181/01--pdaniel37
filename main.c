#include <stdio.h>
#define R 0
#define G 1
#define B 2


typedef struct _pixel {
    unsigned short int red;
    unsigned short int green;
    unsigned short int blue;
} Pixel;

typedef struct _image {
    // [width][height][rgb]
    // 0 -> red
    // 1 -> green
    // 2 -> blue
    unsigned short int pixel[512][512][3];
    unsigned int width;
    unsigned int height;
} Image;

//Functions prototypes
Image menu(Image imageAux);
Image gray_scale(Image imageAux);
Image blur(Image imageAux);
Image rotate90right(Image imageAux);
Image invert_colors(Image imageAux);
Image crop_image(Image imageAux);
Image readImage(Image imageAux);
Image sepia(Image imageAux);
Image mirror(Image imageAux);
void printImage(Image imageAux);


int main() {
    Image image;

    image = readImage(image);

    image = menu(image);

    printImage(image);

    return 0;
}

//Aplly a gray scale
Image gray_scale(Image imageAux) {

    for (unsigned int column = 0; column < imageAux.height; ++column) {
        for (unsigned int row = 0; row < imageAux.width; ++row) {
            int media = imageAux.pixel[column][row][R] +
                        imageAux.pixel[column][row][G] +
                        imageAux.pixel[column][row][B];
            media /= 3;
            imageAux.pixel[column][row][R] = media;
            imageAux.pixel[column][row][G] = media;
            imageAux.pixel[column][row][B] = media;
        }
    }

    return imageAux;
}

//Blur the image
Image blur(Image imageAux) {
    int size_blur = 0;
    scanf("%d", &size_blur);

    for (unsigned int column = 0; column < imageAux.height; ++column) {
        for (unsigned int row = 0; row < imageAux.width; ++row) {
            Pixel media = {0, 0, 0};

            int menor_h = (imageAux.height - 1 > column + size_blur/2) ? column + size_blur/2 : imageAux.height - 1;
            int min_w = (imageAux.width - 1 > row + size_blur/2) ? row + size_blur/2 : imageAux.width - 1;
            for(unsigned int columnAux = (0 > column - size_blur/2 ? 0 : column - size_blur/2); columnAux <= menor_h; ++columnAux) {
                for(unsigned int rowAux = (0 > row - size_blur/2 ? 0 : row - size_blur/2); rowAux <= min_w; ++rowAux) {
                    media.red += imageAux.pixel[columnAux][rowAux][R];
                    media.green += imageAux.pixel[columnAux][rowAux][G];
                    media.blue += imageAux.pixel[columnAux][rowAux][B];
                }
            }

            media.red /= size_blur * size_blur;
            media.green /= size_blur * size_blur;
            media.blue /= size_blur * size_blur;

            imageAux.pixel[column][row][R] = media.red;
            imageAux.pixel[column][row][G] = media.green;
            imageAux.pixel[column][row][B] = media.blue;
        }
    }
    return imageAux;
}

//Rotates 90 degrees to the right
Image rotate90right(Image imageAux) {
    Image rotated;

    rotated.width = imageAux.height;
    rotated.height = imageAux.width;

    for (unsigned int column = 0, columnMax = 0; column < rotated.height; ++column, ++columnMax) {
        for (int row = rotated.width - 1, rowAux = 0; row >= 0; --row, ++rowAux) {
            rotated.pixel[column][row][R] = imageAux.pixel[rowAux][columnMax][R];
            rotated.pixel[column][row][G] = imageAux.pixel[rowAux][columnMax][G];
            rotated.pixel[column][row][B] = imageAux.pixel[rowAux][columnMax][B];
        }
    }

    return rotated;
}

//Invert colors of the image
Image invert_colors(Image imageAux) {
    for (unsigned int column = 0; column < imageAux.height; ++column) {
        for (unsigned int row = 0; row < imageAux.width; ++row) {
            imageAux.pixel[column][row][R] = 255 - imageAux.pixel[column][row][R];
            imageAux.pixel[column][row][G] = 255 - imageAux.pixel[column][row][G];
            imageAux.pixel[column][row][B] = 255 - imageAux.pixel[column][row][B];
        }
    }
    return imageAux;
}

//Crop the image, the first two parameters are from the starting point and the next two are the size of the crop.
Image crop_image(Image imageAux) {
    Image cropped;

    int initialPointX, initialPointY;
    scanf("%d %d", &initialPointX, &initialPointY);
    int widthCrop, heightCrop;
    scanf("%d %d", &widthCrop, &heightCrop);

    cropped.width = widthCrop;
    cropped.height = heightCrop;

    for(int column = 0; column < heightCrop; ++column) {
        for(int row = 0; row < widthCrop; ++row) {
            cropped.pixel[column][row][R] = imageAux.pixel[column + initialPointY][row + initialPointX][R];
            cropped.pixel[column][row][G] = imageAux.pixel[column + initialPointY][row + initialPointX][G];
            cropped.pixel[column][row][B] = imageAux.pixel[column + initialPointY][row + initialPointX][B];
        }
    }
    return cropped;
}

//Upload image
Image readImage(Image imageAux){
  // read type of image
  char p3[4];
  scanf("%s", p3);

  // read width height and color of image
  int max_color;
  scanf("%u %u %d", &imageAux.width, &imageAux.height, &max_color);

  // read all pixels of image
  for (unsigned int i = 0; i < imageAux.height; ++i) {
      for (unsigned int j = 0; j < imageAux.width; ++j) {
          scanf("%hu %hu %hu", &imageAux.pixel[i][j][R],
                               &imageAux.pixel[i][j][G],
                               &imageAux.pixel[i][j][B]);

      }
  }
  return imageAux;
}

//Aplly a sepia filter
Image sepia(Image imageAux){
  for (unsigned int column = 0; column < imageAux.height; ++column) {
      for (unsigned int row = 0; row < imageAux.width; ++row) {
          unsigned short int pixel[3];
          pixel[R] = imageAux.pixel[column][row][R];
          pixel[G] = imageAux.pixel[column][row][G];
          pixel[B] = imageAux.pixel[column][row][B];

          int pixelUnity =  pixel[R] * .393 + pixel[G] * .769 + pixel[B] * .189;
          int menor_r = (255 >  pixelUnity) ? pixelUnity : 255;
          imageAux.pixel[column][row][R] = menor_r;

          pixelUnity =  pixel[R] * .349 + pixel[G] * .686 + pixel[B] * .168;
          menor_r = (255 >  pixelUnity) ? pixelUnity : 255;
          imageAux.pixel[column][row][G] = menor_r;

          pixelUnity =  pixel[R] * .272 + pixel[G] * .534 + pixel[B] * .131;
          menor_r = (255 >  pixelUnity) ? pixelUnity : 255;
          imageAux.pixel[column][row][B] = menor_r;
      }
  }
  return imageAux;
}

//Mirror the image, the degree of horizontal inclination must be reported.
Image mirror(Image imageAux){
  int horizontal = 0;
  scanf("%d", &horizontal);

  int width = imageAux.width, height = imageAux.height;

  if (horizontal == 1){
    width /= 2;
  } else{
    height /= 2;
  }

  for (int column = 0; column < height; ++column) {
      for (int row = 0; row < width; ++row) {
          int columnAux = column, rowAux = row;

          if (horizontal == 1) rowAux = imageAux.width - 1 - row;
          else columnAux = imageAux.height - 1 - column;

          Pixel pixelAux;
          pixelAux.red = imageAux.pixel[column][row][R];
          pixelAux.green = imageAux.pixel[column][row][G];
          pixelAux.blue = imageAux.pixel[column][row][B];

          imageAux.pixel[column][row][R] = imageAux.pixel[columnAux][rowAux][R];
          imageAux.pixel[column][row][G] = imageAux.pixel[columnAux][rowAux][G];
          imageAux.pixel[column][row][B] = imageAux.pixel[columnAux][rowAux][B];

          imageAux.pixel[columnAux][rowAux][R] = pixelAux.red;
          imageAux.pixel[columnAux][rowAux][G] = pixelAux.green;
          imageAux.pixel[columnAux][rowAux][B] = pixelAux.blue;
      }
  }
  return imageAux;
}

//Program menu
Image menu(Image imageAux){
  int n_options;
  scanf("%d", &n_options);

  for(int i = 0; i < n_options; ++i) {
      int options;
      scanf("%d", &options);

      switch(options) {
          case 1: { // Gray Scale
              imageAux = gray_scale(imageAux);
              break;
          }
          case 2: { // Sepia Filter
              imageAux = sepia(imageAux);
              break;
          }
          case 3: { // Blur
              imageAux = blur(imageAux);
              break;
          }
          case 4: { // Rotation
              int how_many_times = 0;
              scanf("%d", &how_many_times);
              how_many_times %= 4;
              for (int j = 0; j < how_many_times; ++j) {
                  imageAux = rotate90right(imageAux);
              }
              break;
          }
          case 5: { // Mirroring
              imageAux = mirror(imageAux);
              break;
          }
          case 6: { // Invert Colors
              imageAux = invert_colors(imageAux);
              break;
          }
          case 7: { // Crop Image
              imageAux = crop_image(imageAux);
              break;
          }
      }

  }
  return imageAux;
}

//Prints the image at * .ppm.
void printImage(Image imageAux){
  // print type of image
  printf("P3\n");
  // print width height and color of image
  printf("%u %u\n255\n", imageAux.width, imageAux.height);

  // print pixels of image
  for (unsigned int i = 0; i < imageAux.height; ++i) {
      for (unsigned int j = 0; j < imageAux.width; ++j) {
          printf("%hu %hu %hu ", imageAux.pixel[i][j][R],
                                 imageAux.pixel[i][j][G],
                                 imageAux.pixel[i][j][B]);

      }
      printf("\n");
  }
}
