#include <stdio.h>
#include <stdint.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

#define WIDTH 640
#define HEIGHT 480

#define PIXELSIZE 4

#define TOTAL_FRAMES 2400

#define COLOR_INTENSITY 2000

#define GRAV_LIMIT 0.10

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

void updateForces(struct Meta meta[3])
{
        uint32_t iter2 = 0;
        uint32_t iter3 = 0;

        int32_t xDelta;
        int32_t yDelta;

        while(iter2 < 3)
        {
            // vector calc
            // P1P2 = (x2-x1)i + (y2-y1)j
            
            iter3 = 0; 
            while(iter3 < 3)
            {
                if(iter2 == iter3)
                {
                    iter3++;
                    continue;
                }

                xDelta = (meta[iter3].pos.x - meta[iter2].pos.x);
                yDelta = (meta[iter3].pos.y - meta[iter2].pos.y);

                int32_t i; // direction vector for x
                int32_t j; // direction vector for y

                float gravity = metaCalc(xDelta, yDelta)*1000; // *1000 makes gravity 1 when overlapping

                if(gravity > GRAV_LIMIT)
                    gravity = GRAV_LIMIT;

                if(xDelta > 0)
                    i = 1;
                else
                    i = -1;

                if(yDelta > 0)
                    j = 1;
                else
                    j = -1;

                meta[iter2].pos.x += meta[iter2].v.x + i*gravity/2.0f; // calculate position
                meta[iter2].v.x += i*gravity; // calc speed

                meta[iter2].pos.y += meta[iter2].v.y + j*gravity/2.0f; // calculate position
                meta[iter2].v.y += j*gravity; // calc speed

                printf("\nX:%fY:%f v.x: %f, v.y: %f, i: %d, j: %d, xDelta: %d yDelta: %d Gravity: %f", meta[iter2].pos.x, meta[iter2].pos.y, meta[iter2].v.x, meta[iter2].v.y, i, j, xDelta, yDelta, gravity);

                iter3++;
            }

            iter2++;
        }
}

int main()
{
    int8_t c;

    float amplitude1;
    float amplitude2;
    float amplitude3;
    float amplitudeSum;

    char file[32];

    uint32_t x;
    uint32_t y;

    struct Meta meta[3];

    meta[0].pos.x = 250;
    meta[0].pos.y = 200;
    meta[0].v.x = 0; 
    meta[0].v.y = 0; 

    meta[1].pos.x = 300;
    meta[1].pos.y = 250;
    meta[1].v.x = 0; 
    meta[1].v.y = 0; 

    meta[2].pos.x = 350;
    meta[2].pos.y = 225;
    meta[2].v.x = 0; 
    meta[2].v.y = 0; 

    uint32_t iter = 0;

    uint8_t color = 0;

    int8_t ret;
    printf("hello");

    while(iter < TOTAL_FRAMES)
    {
        clear(buffer);

        for(y=0;y<HEIGHT;y++)
        {
            for(x=0;x<WIDTH;x++)
            {
                //setPixel(x,y,0xFF,0xFF,0x00,0xFF, buffer);

                amplitude1 = metaCalc(x-meta[0].pos.x,y-meta[0].pos.y); 
                amplitude2 = metaCalc(x-meta[1].pos.x,y-meta[1].pos.y); 
                amplitude3 = metaCalc(x-meta[2].pos.x,y-meta[2].pos.y); 

                amplitudeSum = amplitude1 + amplitude2 + amplitude3;             

                color = (uint8_t)((float)0xFF*amplitudeSum*20000);
    
                if(amplitudeSum > 0.000052)
                {
                    setPixel(x,y,(uint8_t)((float)color*amplitude3*COLOR_INTENSITY),(uint8_t)((float)color*amplitude1*COLOR_INTENSITY),(uint8_t)((float)color*amplitude2*COLOR_INTENSITY),0xFF, buffer);
                }
            }
        }

        snprintf(file, sizeof(char) * 32, "frames/image%i.png", iter);
        printf("\nRendering frame %s", file);
        ret = stbi_write_png(file, WIDTH, HEIGHT, 4, &buffer, WIDTH*PIXELSIZE);


        updateForces(meta);

        iter++;
    }

    if(ret !=0) {
        printf("error");
        return 1;
    }
    else printf("ok");

    return 0;
}
