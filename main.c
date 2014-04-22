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

typedef enum mask_size
{
    MASK_3,
    MASK_5,
    MASK_7
}
MaskSize;

int main(int argc, char **args)
{
    char fileName[128];
    
    int values[4];
    int maxValue;
    int io_result;
    int extended;
    
    struct timespec ts1;
    struct timespec ts2;
    
    unsigned int sizeX;
    unsigned int sizeY;
    
    unsigned int extendedSizeX;
    unsigned int extendedSizeY;
    
    MaskSize mask = MASK_3;
    
    double time1;
    double time2;
    double time3;
    
    int i, j, k;
    
    
    strcpy(fileName, args[1]);
    sizeX = atoi(args[2]);
    sizeY = atoi(args[3]);
    
    
    switch (mask)
    {
    	case MASK_3: extended = 2;
    	             break;
    	case MASK_5: extended = 4;
    	             break;
    	case MASK_7: extended = 6;
    	             break;
    }
    
    
    
    Pixel **image  = malloc(sizeof(Pixel *) * (sizeY + extended));
    Pixel **result = malloc(sizeof(Pixel *) * sizeY);
    
    for (i = 0; i < sizeY + extended; i++)
    {
        image[i]  = malloc(sizeof(Pixel) * (sizeX + extended));
        result[i] = malloc(sizeof(Pixel) * sizeX);
    }
    
    clock_gettime(CLOCK_REALTIME, &ts1);

    int file = open(fileName, O_RDONLY);
    
    for (i = extended / 2; i < sizeY; i++)
    {
        io_result = read(file, image[i] + (extended / 2), sizeX * sizeof(Pixel));
        
        if (io_result == -1)
        {
            fputs("Read error\n", stderr);
        }
    }
    
    close(file);
    
    // Kopiowanie skrajnych pikseli do obszaru rozszerzonego
    for (i = 0; i < extended / 2; i++)
    {
    	memcpy(image[i], image[extended / 2], sizeof(Pixel) * (sizeX + extended));
    	//memcpy()
    }
    
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
        io_result = write(file, result[i], sizeX * sizeof(Pixel));
        
        if (io_result == -1)
        {
            fputs("Write error\n", stderr);
        }
    }
    
    close(file);
    
    clock_gettime(CLOCK_REALTIME, &ts2);
    
    time3 = (ts2.tv_sec + ts2.tv_nsec / MLD) - (ts1.tv_sec + ts1.tv_nsec / MLD);
    
    printf("Read Time  = %3.10lf\n", time1);
    printf("Calc Time  = %3.10lf\n", time2);
    printf("Write Time = %3.10lf\n", time3);
    
    return 0;
}
