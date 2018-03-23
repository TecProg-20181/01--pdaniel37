#include <stdio.h>

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
            int media = imageAux.pixel[column][row][0] +
                        imageAux.pixel[column][row][1] +
                        imageAux.pixel[column][row][2];
            media /= 3;
            imageAux.pixel[column][row][0] = media;
            imageAux.pixel[column][row][1] = media;
            imageAux.pixel[column][row][2] = media;
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
                    media.red += imageAux.pixel[columnAux][rowAux][0];
                    media.green += imageAux.pixel[columnAux][rowAux][1];
                    media.blue += imageAux.pixel[columnAux][rowAux][2];
                }
            }

            media.red /= size_blur * size_blur;
            media.green /= size_blur * size_blur;
            media.blue /= size_blur * size_blur;

            imageAux.pixel[column][row][0] = media.red;
            imageAux.pixel[column][row][1] = media.green;
            imageAux.pixel[column][row][2] = media.blue;
        }
    }
    return imageAux;
}

//Rotates 90 degrees to the right
Image rotate90right(Image imageAux) {
    Image rotated;

    rotated.width = imageAux.height;
    rotated.height = imageAux.width;

    for (unsigned int i = 0, y = 0; i < rotated.height; ++i, ++y) {
        for (int j = rotated.width - 1, x = 0; j >= 0; --j, ++x) {
            rotated.pixel[i][j][0] = imageAux.pixel[x][y][0];
            rotated.pixel[i][j][1] = imageAux.pixel[x][y][1];
            rotated.pixel[i][j][2] = imageAux.pixel[x][y][2];
        }
    }

    return rotated;
}

//Invert colors of the image
Image invert_colors(Image imageAux) {
    for (unsigned int i = 0; i < imageAux.height; ++i) {
        for (unsigned int j = 0; j < imageAux.width; ++j) {
            imageAux.pixel[i][j][0] = 255 - imageAux.pixel[i][j][0];
            imageAux.pixel[i][j][1] = 255 - imageAux.pixel[i][j][1];
            imageAux.pixel[i][j][2] = 255 - imageAux.pixel[i][j][2];
        }
    }
    return imageAux;
}

//Crop the image, the first two parameters are from the starting point and the next two are the size of the crop.
Image crop_image(Image imageAux) {
    Image cropped;
    cropped = imageAux;

    int x, y;
    scanf("%d %d", &x, &y);
    int width, height;
    scanf("%d %d", &width, &height);

    cropped.width = width;
    cropped.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            cropped.pixel[i][j][0] = imageAux.pixel[i + y][j + x][0];
            cropped.pixel[i][j][1] = imageAux.pixel[i + y][j + x][1];
            cropped.pixel[i][j][2] = imageAux.pixel[i + y][j + x][2];
        }
    }
    printf("%d\t%d\t%d\t%d", imageAux.width, imageAux.height, cropped.width, cropped.height);
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
          scanf("%hu %hu %hu", &imageAux.pixel[i][j][0],
                               &imageAux.pixel[i][j][1],
                               &imageAux.pixel[i][j][2]);

      }
  }
  return imageAux;
}

//Aplly a sepia filter
Image sepia(Image imageAux){
  for (unsigned int column = 0; column < imageAux.height; ++column) {
      for (unsigned int row = 0; row < imageAux.width; ++row) {
          unsigned short int pixel[3];
          pixel[0] = imageAux.pixel[column][row][0];
          pixel[1] = imageAux.pixel[column][row][1];
          pixel[2] = imageAux.pixel[column][row][2];

          int pixelUnity =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
          int menor_r = (255 >  pixelUnity) ? pixelUnity : 255;
          imageAux.pixel[column][row][0] = menor_r;

          pixelUnity =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
          menor_r = (255 >  pixelUnity) ? pixelUnity : 255;
          imageAux.pixel[column][row][1] = menor_r;

          pixelUnity =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
          menor_r = (255 >  pixelUnity) ? pixelUnity : 255;
          imageAux.pixel[column][row][2] = menor_r;
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
          pixelAux.red = imageAux.pixel[column][row][0];
          pixelAux.green = imageAux.pixel[column][row][1];
          pixelAux.blue = imageAux.pixel[column][row][2];

          imageAux.pixel[column][row][0] = imageAux.pixel[columnAux][rowAux][0];
          imageAux.pixel[column][row][1] = imageAux.pixel[columnAux][rowAux][1];
          imageAux.pixel[column][row][2] = imageAux.pixel[columnAux][rowAux][2];

          imageAux.pixel[columnAux][rowAux][0] = pixelAux.red;
          imageAux.pixel[columnAux][rowAux][1] = pixelAux.green;
          imageAux.pixel[columnAux][rowAux][2] = pixelAux.blue;
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
          printf("%hu %hu %hu ", imageAux.pixel[i][j][0],
                                 imageAux.pixel[i][j][1],
                                 imageAux.pixel[i][j][2]);

      }
      printf("\n");
  }
}
