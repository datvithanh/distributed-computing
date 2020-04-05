#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <stdlib.h>

#define MAX_ITER 80
//figure size
#define WIDTH 600
#define HEIGHT 400
//plot window
#define RE_START -2
#define RE_END 1
#define IM_START -1
#define IM_END 1

FILE *fp;

int mandelbrot(double x, double y){
    double zx = 0, zy = 0, tx, ty;
    int n = 0;
    while(zx*zx + zy*zy < 4 && n < MAX_ITER){
        double tx, ty;
        tx = zx*zx - zy*zy + x;
        ty = 2*zx*zy + y;
        zx = tx; zy = ty;
        n++;
    }
    return n;
}

void generate_fractal() {
    int *fig;
    int x, y;

    fig = (int *)malloc(WIDTH * HEIGHT * sizeof(int));
    #pragma omp parallel shared(fig) private(x, y)
    {
        #pragma omp master
        {
            printf("Number of processors available: %d\n", omp_get_num_procs());
            printf("Number of threads being used: %d\n", omp_get_num_threads());
            printf("Maximum number of threads available: %d\n", omp_get_max_threads());
            printf("This line is in pragma omp. In parallel region: %s\n", omp_in_parallel() ? "true" : "false");
            printf("Dynamic threads are enabled: %s\n", omp_get_dynamic() ? "true" : "false");
            printf("Nested parallelism is supported: %s\n", omp_get_nested() ? "true" : "false");
        }
        
        #pragma omp for schedule(static)
        for(int x = 0; x < WIDTH; ++x)
            for(int y = 0; y < HEIGHT; ++y){
                double n = mandelbrot(RE_START + (1.0*x / WIDTH) * (RE_END - RE_START), \
                        IM_START + (1.0*y / HEIGHT) * (IM_END - IM_START));
                *(fig + x*HEIGHT + y) = 255 - (int)(n * 255 / MAX_ITER);
            }
    }

    fp = fopen("pixels.txt", "w");
    for(int x = 0; x < WIDTH; ++x){
        for(int y = 0; y < HEIGHT; ++y) 
            fprintf(fp, "%d ", *(fig + x*HEIGHT + y));
        fprintf(fp, "\n");
    }
}

int main(int argc, char const *argv[])
{
    generate_fractal();
}