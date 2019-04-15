#include <stdio.h>
#include <stdint.h>
#include "stb_image_write.h"

#define WIDTH 320
#define HEIGHT 240

uint8_t pixel[4];
uint8_t buffer[320];

int main()
{
    char file[] = "image.bmp";

    int8_t ret;
    printf("hello");

    ret = stbi_write_bmp(file, WIDTH, HEIGHT, 1, &buffer);

    if(ret !=0) {
        printf("error");
        return 1;
    }

    return 0;
}
