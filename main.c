#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#define MLD 1000000000.0

typedef struct pixel
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
}
Pixel;

enum MaskSize
{
    MASK_3,
    MASK_5,
    MASK_7
};

int main(int argc, char **args)
{
    char fileName[128];
    
    int values[4];
    int maxValue;
    int io_result;
    
    struct timespec ts1;
    struct timespec ts2;
    
    unsigned int sizeX;
    unsigned int sizeY;
    
    double time1;
    double time2;
    double time3;
    
    int i, j, k;
    
    
    strcpy(fileName, args[1]);
    sizeX = atoi(args[2]);
    sizeY = atoi(args[3]);
    
    
    char **image  = malloc(sizeof(char *) * sizeY);
    char **result = malloc(sizeof(char *) * sizeY);
    
    for (i = 0; i < sizeY; i++)
    {
        image[i]  = malloc(sizeof(Pixel) * sizeX);
        result[i] = malloc(sizeof(Pixel) * sizeX);
        
        memset(result[i], 0, sizeX * sizeof(Pixel));
    }
    
    clock_gettime(CLOCK_REALTIME, &ts1);

    int file = open(fileName, O_RDONLY);
    
    for (i = 0; i < sizeY; i++)
    {
        io_result = read(file, image[i], sizeX * sizeof(Pixel));
        
        if (io_result == -1)
        {
            fputs("Read error\n", stderr);
        }
    }
    
    close(file);
    
    clock_gettime(CLOCK_REALTIME, &ts2);
    
    time1 = (ts2.tv_sec + ts2.tv_nsec / MLD) - (ts1.tv_sec + ts1.tv_nsec / MLD);
    
    clock_gettime(CLOCK_REALTIME, &ts1);
    
    
    
    clock_gettime(CLOCK_REALTIME, &ts2);
    
    time2 = (ts2.tv_sec + ts2.tv_nsec / MLD) - (ts1.tv_sec + ts1.tv_nsec / MLD);
    
    clock_gettime(CLOCK_REALTIME, &ts1);
    
    strcat(fileName, "-mask");
    
    file = open(fileName, O_WRONLY | O_CREAT, S_IRUSR);
    
    for (i = 0; i < sizeY; i++)
    {
        write(file, result[i], sizeX * sizeof(Pixel));
    }
    
    close(file);
    
    clock_gettime(CLOCK_REALTIME, &ts2);
    
    time3 = (ts2.tv_sec + ts2.tv_nsec / MLD) - (ts1.tv_sec + ts1.tv_nsec / MLD);
    
    printf("Read Time  = %3.10lf\n", time1);
    printf("Calc Time  = %3.10lf\n", time2);
    printf("Write Time = %3.10lf\n", time3);
    
    return 0;
}
