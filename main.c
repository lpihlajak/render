#include <stdio.h>
#include <stdint.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

#define WIDTH 320
#define HEIGHT 240
#define PIXELSIZE 4 // 4 bytes rgba

//uint8_t buffer[WIDTH*HEIGHT*PIXELSIZE];
uint8_t buffer[WIDTH][HEIGHT][PIXELSIZE];

enum rgba {
    R=0,
    G=1,
    B=2,
    A=3,
};

int main()
{
    int16_t x,y;
    int8_t c;

    char file[] = "image2.bmp";

    int8_t ret;
    printf("hello");

    uint32_t size = sizeof(buffer);
    uint32_t iterator = 0;

    for(y=0; y<HEIGHT; y++)
    {
        for(x=0; x<WIDTH; x++)
        {
            c = x^y;
            buffer[x][y][R] = c;
            buffer[x][y][G] = c;
            buffer[x][y][B] = c;
            buffer[x][y][A] = 0xFF;
        }
    }

    ret = stbi_write_bmp(file, WIDTH, HEIGHT, 4, &buffer);

    if(ret !=0) {
        printf("error");
        return 1;
    }

    return 0;
}
