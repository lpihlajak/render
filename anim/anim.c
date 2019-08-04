#include <stdio.h>
#include <stdint.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

#define WIDTH 640
#define HEIGHT 480

#define PIXELSIZE 4

#define TOTAL_FRAMES 2400

uint8_t buffer[WIDTH*HEIGHT*PIXELSIZE];

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
    struct Vec2 pos; 
    struct Vec2 v;  // Velocity
} Meta;

int main()
{
    int8_t c;

    float amplitude;

    char file[32];

    uint32_t x;
    uint32_t y;

    struct Meta meta[2];

    meta[0].pos.x = 150;
    meta[0].pos.y = 150;
    meta[0].v.x = 1; 
    meta[0].v.y = 1; 

    meta[1].pos.x = 450;
    meta[1].pos.y = 250;
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
                amplitude = metaCalc(x-meta[0].pos.x,y-meta[0].pos.y); 
                amplitude = amplitude + metaCalc(x-meta[1].pos.x,y-meta[1].pos.y); 
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

        // vector calc
        // P1P2 = (x2-x1)i + (y2-y1)j
        
        int32_t xDelta = (meta[1].pos.x - meta[0].pos.x);
        int32_t yDelta = (meta[1].pos.y - meta[0].pos.y);

        int32_t i;
        int32_t j;


        float gravity = metaCalc(xDelta, yDelta)*1000; // *1000 makes gravity 1 when overlapping

        if(xDelta > 0)
            i = 1;
        else
            i = -1;

        if(yDelta > 0)
            j = 1;
        else
            j = -1;

        meta[0].v.x += i*gravity; // calc speed
        meta[0].pos.x += meta[0].v.x; // calculate position

        meta[0].v.y += j*gravity; // calc speed
        meta[0].pos.y += meta[0].v.y; // calculate position

        printf("\nv.x: %f, v.y: %f, i: %d, j: %d, Delta: %d yDelta: %d Gravity: %f", meta[0].v.x, meta[0].v.y, i, j, xDelta, yDelta, gravity);

        i++;
    }

    if(ret !=0) {
        printf("error");
        return 1;
    }
    else printf("ok");

    return 0;
}
