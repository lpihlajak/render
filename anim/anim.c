#include <stdio.h>
#include <stdint.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

#define WIDTH 640
#define HEIGHT 480

#define PIXELSIZE 4

#define TOTAL_FRAMES 2400

uint8_t buffer[WIDTH*HEIGHT*PIXELSIZE];
//uint8_t buffer[WIDTH][HEIGHT][PIXELSIZE];

enum rgba {
    R=0,
    G=1,
    B=2,
    A=3,
};

void setPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint8_t *buf)
{
    uint32_t addr = (x*PIXELSIZE) + y*PIXELSIZE*WIDTH;
    if(addr < WIDTH*HEIGHT*PIXELSIZE)
    {
        buf[addr+R] = r; 
        buf[addr+G] = g; 
        buf[addr+B] = b; 
        buf[addr+A] = a; 
    }
    else
    {
        printf("memory error");
    }   
    return;
}
void clear(uint8_t *buffer)
{
    uint32_t x;
    uint32_t y;
    for(y=0;y<HEIGHT;y++)
    {
        for(x=0;x<WIDTH;x++)
        {
            setPixel(x,y,0x00,0x00,0x00,0xFF, buffer);
        }
    }
}

float metaCalc(uint32_t x, uint32_t y)
{
    return (1.0 / (x*x + y*y)); 
}
// objects

struct Vec2 {
  float x;
  float y;
} Vec2;

typedef struct Meta {
  uint32_t x;
  uint32_t y;
  struct Vec2 v;  // Velocity
} Rect;

int main()
{
    int8_t c;

    float amplitude;

    char file[32];

    uint32_t x;
    uint32_t y;

    struct Meta meta[2];

    meta[0].x = 150;
    meta[0].y = 150;
    meta[0].v.x = 1; 
    meta[0].v.y = 1; 

    meta[1].x = 450;
    meta[1].y = 250;
    meta[0].v.x = 1; 
    meta[0].v.y = 1; 

    uint32_t i = 0;

    int8_t ret;
    printf("hello");

    while(i< TOTAL_FRAMES)
    {
        clear(buffer);

        for(y=0;y<HEIGHT;y++)
        {
            for(x=0;x<WIDTH;x++)
            {
                //setPixel(x,y,0xFF,0xFF,0x00,0xFF, buffer);
                amplitude = metaCalc(x-meta[0].x,y-meta[0].y); 
                amplitude = amplitude + metaCalc(x-meta[1].x,y-meta[1].y); 
                //printf("\namplitude %f", amplitude);
                if(amplitude > 0.000052)
                {
                    setPixel(x,y,0x00,(uint8_t)((float)0xFF*amplitude*20000),0x00,0xFF, buffer);
                }
            }
        }

        snprintf(file, sizeof(char) * 32, "frames/image%i.png", i);
        printf("\nRendering frame %s", file);
        ret = stbi_write_png(file, WIDTH, HEIGHT, 4, &buffer, WIDTH*PIXELSIZE);

        //int32_t xdir = meta[0].x - meta[1].x;
        //int32_t ydir = meta[0].y - meta[1].y;
        int32_t xdir = 1;
        int32_t ydir = 1;

        meta[0].x += xdir*meta[0].v.x; 
        meta[0].y += xdir*meta[0].v.y; 

        meta[1].x += xdir*meta[1].v.x; 
        meta[1].y += xdir*meta[1].v.y; 

        i++;
    }

    if(ret !=0) {
        printf("error");
        return 1;
    }
    else printf("ok");

    return 0;
}
