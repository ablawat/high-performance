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
    char file_name[128];
    
    int values[4];
    int max_value;
    int io_result;
    int extended;
    
    struct timespec ts1;
    struct timespec ts2;
    
    unsigned int size_x;
    unsigned int size_y;
    
    unsigned int extended_size_x;
    unsigned int extended_size_y;
    
    MaskSize mask = MASK_3;
    
    double time1;
    double time2;
    double time3;
    
    int i, j, k;
    
    
    strcpy(file_name, args[1]);
    size_x = atoi(args[2]);
    size_y = atoi(args[3]);
    
    
    switch (mask)
    {
    	case MASK_3: extended = 2;
    	             break;
    	case MASK_5: extended = 4;
    	             break;
    	case MASK_7: extended = 6;
    	             break;
    }
    
    extended_size_x = size_x + extended;
    extended_size_y = size_y + extended;
    
    Pixel **image  = malloc(sizeof(Pixel *) * (size_y + extended));
    Pixel **result = malloc(sizeof(Pixel *) * size_y);
    
    for (i = 0; i < size_y + extended; i++)
    {
        image[i]  = malloc(sizeof(Pixel) * (size_x + extended));
        result[i] = malloc(sizeof(Pixel) * size_x);
    }
    
    clock_gettime(CLOCK_REALTIME, &ts1);

    int file = open(file_name, O_RDONLY);
    
    for (i = extended / 2; i < size_y; i++)
    {
        io_result = read(file, image[i] + (extended / 2), size_x * sizeof(Pixel));
        
        if (io_result == -1)
        {
            fputs("Read error\n", stderr);
        }
    }
    
    close(file);
    
    // Kopiowanie skrajnych pikseli do obszaru rozszerzonego
    for (i = 0; i < extended / 2; i++)
    {
    	memcpy(image[i], image[extended / 2], sizeof(Pixel) * (size_x + extended));
    	//memcpy()
    }
    
    clock_gettime(CLOCK_REALTIME, &ts2);
    
    time1 = (ts2.tv_sec + ts2.tv_nsec / MLD) - (ts1.tv_sec + ts1.tv_nsec / MLD);
    
    clock_gettime(CLOCK_REALTIME, &ts1);
    
    
    
    clock_gettime(CLOCK_REALTIME, &ts2);
    
    time2 = (ts2.tv_sec + ts2.tv_nsec / MLD) - (ts1.tv_sec + ts1.tv_nsec / MLD);
    
    clock_gettime(CLOCK_REALTIME, &ts1);
    
    strcat(file_name, "-mask");
    
    file = open(file_name, O_WRONLY | O_CREAT, S_IRUSR);
    
    for (i = 0; i < size_y; i++)
    {
        io_result = write(file, result[i], size_x * sizeof(Pixel));
        
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
