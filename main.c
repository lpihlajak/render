#include <stdio.h>
#include <stdint.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

#define WIDTH 320
#define HEIGHT 240

uint8_t pixel[4];
uint8_t buffer[WIDTH*HEIGHT*sizeof(pixel)];

int main()
{
    char file[] = "image2.bmp";

    int8_t ret;
    printf("hello");

    uint32_t size = sizeof(buffer);
    uint32_t iterator = 0;

    while(iterator < size)
    {
        // try blue color
        buffer[iterator] = iterator ^ (iterator/HEIGHT);

        iterator++;
    } 

    ret = stbi_write_bmp(file, WIDTH, HEIGHT, 4, &buffer);

    if(ret !=0) {
        printf("error");
        return 1;
    }

    return 0;
}
