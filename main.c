#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#define MLD 1000000000.0

#define SIZE_X 1920
#define SIZE_Y 1080

int main()
{
    const char inFileName[]  = "/mnt/adam-2/dane/nature-2-1920-1080.raw";
    const char outFileName[] = "/mnt/adam-2/dane/nature-2-1920-1080-edges.raw";
    
    int values[4];
    int maxValue;
    
    struct timespec ts1;
    struct timespec ts2;
    
    double time1;
    double time2;
    double time3;
    
    int i, j, k;
    
    char **image  = malloc(sizeof(char *) * SIZE_Y);
    char **result = malloc(sizeof(char *) * SIZE_Y);
    
    for (i = 0; i < SIZE_Y; i++)
    {
        image[i]  = malloc(sizeof(char) * SIZE_X * 3);
        result[i] = malloc(sizeof(char) * SIZE_X * 3);
        
        memset(result[i], 0, SIZE_X * 3);
    }
    
    clock_gettime(CLOCK_REALTIME, &ts1);

    int file = open(inFileName, O_RDONLY);
    
    for (i = 0; i < SIZE_Y; i++)
    {
        read(file, image[i], SIZE_X * 3);
    }
    
    close(file);
    
    clock_gettime(CLOCK_REALTIME, &ts2);
    
    time1 = (ts2.tv_sec + ts2.tv_nsec / MLD) - (ts1.tv_sec + ts1.tv_nsec / MLD);
    
    clock_gettime(CLOCK_REALTIME, &ts1);
    
    for (i = 1; i < SIZE_Y - 1; i++)
    {
        for (j = 3; j < SIZE_X * 3 - 3; j++)
        {
            values[0] = abs(image[i][j] - image[i][j - 3]);
            values[1] = abs(image[i][j] - image[i - 1][j - 3]);
            values[2] = abs(image[i][j] - image[i - 1][j]);
            values[3] = abs(image[i][j] - image[i - 1][j + 3]);
            
            maxValue = values[0];
            
            for (k = 1; k < 4; k++)
            {
                if (values[k] > maxValue)
                {
                    maxValue = values[k];
                }
            }
            
            result[i][j] = (char)maxValue;
        }
    }
    
    clock_gettime(CLOCK_REALTIME, &ts2);
    
    time2 = (ts2.tv_sec + ts2.tv_nsec / MLD) - (ts1.tv_sec + ts1.tv_nsec / MLD);
    
    clock_gettime(CLOCK_REALTIME, &ts1);
    
    file = open(outFileName, O_WRONLY | O_CREAT, S_IRUSR);
    
    for (i = 0; i < SIZE_Y; i++)
    {
        write(file, result[i], SIZE_X * 3);
    }
    
    close(file);
    
    clock_gettime(CLOCK_REALTIME, &ts2);
    
    time3 = (ts2.tv_sec + ts2.tv_nsec / MLD) - (ts1.tv_sec + ts1.tv_nsec / MLD);
    
    printf("Read Time  = %3.10lf\n", time1);
    printf("Calc Time  = %3.10lf\n", time2);
    printf("Write Time = %3.10lf\n", time3);
    
    return 0;
}
