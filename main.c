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

int main() {
    Image image;

    image = readImage(image);

    image = menu(image);

    printImage(image);

    return 0;
}

Image gray_scale(Image imageAux) {

    for (unsigned int i = 0; i < imageAux.height; ++i) {
        for (unsigned int j = 0; j < imageAux.width; ++j) {
            int media = imageAux.pixel[i][j][0] +
                        imageAux.pixel[i][j][1] +
                        imageAux.pixel[i][j][2];
            media /= 3;
            imageAux.pixel[i][j][0] = media;
            imageAux.pixel[i][j][1] = media;
            imageAux.pixel[i][j][2] = media;
        }
    }

    return imageAux;
}

void blur(unsigned int height, unsigned short int pixel[512][512][3], unsigned int width) {
    int size_blur = 0;
    scanf("%d", &size_blur);

    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            Pixel media = {0, 0, 0};

            int menor_h = (height - 1 > i + size_blur/2) ? i + size_blur/2 : height - 1;
            int min_w = (width - 1 > j + size_blur/2) ? j + size_blur/2 : width - 1;
            for(unsigned int x = (0 > i - size_blur/2 ? 0 : i - size_blur/2); x <= menor_h; ++x) {
                for(unsigned int y = (0 > j - size_blur/2 ? 0 : j - size_blur/2); y <= min_w; ++y) {
                    media.red += pixel[x][y][0];
                    media.green += pixel[x][y][1];
                    media.blue += pixel[x][y][2];
                }
            }

            media.red /= size_blur * size_blur;
            media.green /= size_blur * size_blur;
            media.blue /= size_blur * size_blur;

            pixel[i][j][0] = media.red;
            pixel[i][j][1] = media.green;
            pixel[i][j][2] = media.blue;
        }
    }
}

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

void invert_colors(unsigned short int pixel[512][512][3],
                    unsigned int width, unsigned int height) {
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            pixel[i][j][0] = 255 - pixel[i][j][0];
            pixel[i][j][1] = 255 - pixel[i][j][1];
            pixel[i][j][2] = 255 - pixel[i][j][2];
        }
    }
}

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

Image sepia(Image imageAux){
  for (unsigned int x = 0; x < imageAux.height; ++x) {
      for (unsigned int j = 0; j < imageAux.width; ++j) {
          unsigned short int pixel[3];
          pixel[0] = imageAux.pixel[x][j][0];
          pixel[1] = imageAux.pixel[x][j][1];
          pixel[2] = imageAux.pixel[x][j][2];

          int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
          int menor_r = (255 >  p) ? p : 255;
          imageAux.pixel[x][j][0] = menor_r;

          p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
          menor_r = (255 >  p) ? p : 255;
          imageAux.pixel[x][j][1] = menor_r;

          p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
          menor_r = (255 >  p) ? p : 255;
          imageAux.pixel[x][j][2] = menor_r;
      }
  }
  return imageAux;
}

Image mirror(Image imageAux){
  int horizontal = 0;
  scanf("%d", &horizontal);

  int width = imageAux.width, height = imageAux.height;

  if (horizontal == 1){
    width /= 2;
  } else{
    height /= 2;
  }

  for (int i2 = 0; i2 < height; ++i2) {
      for (int j = 0; j < width; ++j) {
          int x = i2, y = j;

          if (horizontal == 1) y = imageAux.width - 1 - j;
          else x = imageAux.height - 1 - i2;

          Pixel aux1;
          aux1.red = imageAux.pixel[i2][j][0];
          aux1.green = imageAux.pixel[i2][j][1];
          aux1.blue = imageAux.pixel[i2][j][2];

          imageAux.pixel[i2][j][0] = imageAux.pixel[x][y][0];
          imageAux.pixel[i2][j][1] = imageAux.pixel[x][y][1];
          imageAux.pixel[i2][j][2] = imageAux.pixel[x][y][2];

          imageAux.pixel[x][y][0] = aux1.red;
          imageAux.pixel[x][y][1] = aux1.green;
          imageAux.pixel[x][y][2] = aux1.blue;
      }
  }
  return imageAux;
}

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
              blur(imageAux.height, imageAux.pixel, imageAux.width);
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
              invert_colors(imageAux.pixel, imageAux.width, imageAux.height);
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
