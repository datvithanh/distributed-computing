/**
 * URL: https://www.geeksforgeeks.org/fractals-in-cc/
 */

#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <graphics.h>

#define MAXCOUNT 50

void omp_generate(float left, float top, float xside, float yside)
{
    float xscale, yscale, zx, zy, cx, tempx, cy;
    int x, y, i, j;
    int maxx, maxy, count;

    maxx = getmaxx();
    maxy = getmaxy();
    // maxx = 1000;
    // maxy = 1000;
    xscale = xside / maxx;
    yscale = yside / maxy;
#pragma omp parallel for
    for (y = 1; y <= maxy - 1; y++)
    {
        for (x = 1; x <= maxx - 1; x++)
        {
            cx = x * xscale + left;
            cy = y * yscale + top;
            zx = 0;
            zy = 0;
            count = 0;
            while ((zx * zx + zy * zy < 4) && (count < MAXCOUNT))
            {
                tempx = zx * zx - zy * zy + cx;
                zy = 2 * zx * zy + cy;
                zx = tempx;
                count = count + 1;
            }
            putpixel(x, y, count);
        }
    }
}

void normal_generate(float left, float top, float xside, float yside)
{
    float xscale, yscale, zx, zy, cx, tempx, cy;
    int x, y, i, j;
    int maxx, maxy, count;

    maxx = getmaxx();
    maxy = getmaxy();
    // maxx = 1000;
    // maxy = 1000;
    xscale = xside / maxx;
    yscale = yside / maxy;
    for (y = 1; y <= maxy - 1; y++)
    {
        for (x = 1; x <= maxx - 1; x++)
        {
            cx = x * xscale + left;
            cy = y * yscale + top;
            zx = 0;
            zy = 0;
            count = 0;
            while ((zx * zx + zy * zy < 4) && (count < MAXCOUNT))
            {
                tempx = zx * zx - zy * zy + cx;
                zy = 2 * zx * zy + cy;
                zx = tempx;
                count = count + 1;
            }
            putpixel(x, y, count);
        }
    }
}

int main(int argc, char const *argv[])
{
    omp_set_num_threads(4);
    clock_t begin, end;
    int gd = DETECT, gm, errorcode;
    float left, top, xside, yside;
    left = -1.75;
    top = -0.25;
    xside = 0.25;
    yside = 0.45;
    char driver[200] = "";

    begin = clock();
    initgraph(&gd, &gm, driver);
    normal_generate(left, top, xside, yside);
    getch();
    closegraph();
    end = clock();
    printf("Normal time taken: \t%f\n", (double)(end - begin) / CLOCKS_PER_SEC);

    begin = clock();
    initgraph(&gd, &gm, driver);
    omp_generate(left, top, xside, yside);
    getch();
    closegraph();
    end = clock();
    printf("OMP time taken: \t%f\n", (double)(end - begin) / CLOCKS_PER_SEC);

    return 0;
}