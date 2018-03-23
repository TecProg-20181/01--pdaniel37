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


int max(int a, int b) {
    if (a > b)
        return a;
    return b;
}

int pixel_igual(Pixel p1, Pixel p2) {
    if (p1.red == p2.red &&
        p1.green == p2.green &&
        p1.blue == p2.blue)
        return 1;
    return 0;
}


Image escala_de_cinza(Image image) {
    /*for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            print("%u", image.pixel[i][j][0] + image.pixel[i][j][1] + image.pixel[i][j][2]);
        }
    }*/

    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            int media = image.pixel[i][j][0] +
                        image.pixel[i][j][1] +
                        image.pixel[i][j][2];
            media /= 3;
            image.pixel[i][j][0] = media;
            image.pixel[i][j][1] = media;
            image.pixel[i][j][2] = media;
        }
    }

    return image;
}

void blur(unsigned int height, unsigned short int pixel[512][512][3], int size, unsigned int width) {
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            Pixel media = {0, 0, 0};

            unsigned long int menor_h = (height - 1 > i + size/2) ? i + size/2 : height - 1;
            unsigned long int min_w = (width - 1 > j + size/2) ? j + size/2 : width - 1;
            for(int x = (0 > i - size/2 ? 0 : i - size/2); x <= menor_h; ++x) {
                for(int y = (0 > j - size/2 ? 0 : j - size/2); y <= min_w; ++y) {
                    media.red += pixel[x][y][0];
                    media.green += pixel[x][y][1];
                    media.blue += pixel[x][y][2];
                }
            }

            // printf("%u", media.red)
            media.red /= size * size;
            media.green /= size * size;
            media.blue /= size * size;

            pixel[i][j][0] = media.red;
            pixel[i][j][1] = media.green;
            pixel[i][j][2] = media.blue;
        }
    }
}

Image rotacionar90direita(Image image) {
    Image rotacionada;

    rotacionada.width = image.height;
    rotacionada.height = image.width;

    for (unsigned int i = 0, y = 0; i < rotacionada.height; ++i, ++y) {
        for (int j = rotacionada.width - 1, x = 0; j >= 0; --j, ++x) {
            rotacionada.pixel[i][j][0] = image.pixel[x][y][0];
            rotacionada.pixel[i][j][1] = image.pixel[x][y][1];
            rotacionada.pixel[i][j][2] = image.pixel[x][y][2];
        }
    }

    return rotacionada;
}

void inverter_cores(unsigned short int pixel[512][512][3],
                    unsigned int width, unsigned int height) {
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            pixel[i][j][0] = 255 - pixel[i][j][0];
            pixel[i][j][1] = 255 - pixel[i][j][1];
            pixel[i][j][2] = 255 - pixel[i][j][2];
        }
    }
}

Image crop_image(Image image, int x, int y, int width, int height) {
    Image cropped;

    cropped.width = width;
    cropped.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            cropped.pixel[i][j][0] = image.pixel[i + y][j + x][0];
            cropped.pixel[i][j][1] = image.pixel[i + y][j + x][1];
            cropped.pixel[i][j][2] = image.pixel[i + y][j + x][2];
        }
    }

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

Image menu(Image imageAux){
  int n_opcoes;
  scanf("%d", &n_opcoes);

  for(int i = 0; i < n_opcoes; ++i) {
      int opcao;
      scanf("%d", &opcao);

      switch(opcao) {
          case 1: { // Escala de Cinza
              imageAux = escala_de_cinza(imageAux);
              break;
          }
          case 2: { // Filtro Sepia
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

              break;
          }
          case 3: { // Blur
              int tamanho = 0;
              scanf("%d", &tamanho);
              blur(imageAux.height, imageAux.pixel, tamanho, imageAux.width);
              break;
          }
          case 4: { // Rotacao
              int quantas_vezes = 0;
              scanf("%d", &quantas_vezes);
              quantas_vezes %= 4;
              for (int j = 0; j < quantas_vezes; ++j) {
                  imageAux = rotacionar90direita(imageAux);
              }
              break;
          }
          case 5: { // Espelhamento
              int horizontal = 0;
              scanf("%d", &horizontal);

              int width = imageAux.width, height = imageAux.height;

              if (horizontal == 1) width /= 2;
              else height /= 2;

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
              break;
          }
          case 6: { // Inversao de Cores
              inverter_cores(imageAux.pixel, imageAux.width, imageAux.height);
              break;
          }
          case 7: { // Cortar Imagem
              int x, y;
              scanf("%d %d", &x, &y);
              int width, height;
              scanf("%d %d", &width, &height);

              imageAux = crop_image(imageAux, x, y, width, height);
              break;
          }
      }

  }
  return imageAux;
}

int main() {
    Image image;

    image = readImage(image);

    image = menu(image);

    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", image.width, image.height);

    // print pixels of image
    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            printf("%hu %hu %hu ", image.pixel[i][j][0],
                                   image.pixel[i][j][1],
                                   image.pixel[i][j][2]);

        }
        printf("\n");
    }
    return 0;
}
