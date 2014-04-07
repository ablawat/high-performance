#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#define MLD 1000000000.0

int main(int argc, char **args)
{
    char fileName[128];
    
    int values[4];
    int maxValue;
    
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
        image[i]  = malloc(sizeof(char) * sizeX * 3);
        result[i] = malloc(sizeof(char) * sizeX * 3);
        
        memset(result[i], 0, sizeX * 3);
    }
    
    clock_gettime(CLOCK_REALTIME, &ts1);

    int file = open(fileName, O_RDONLY);
    
    for (i = 0; i < sizeY; i++)
    {
        read(file, image[i], sizeX * 3);
    }
    
    close(file);
    
    clock_gettime(CLOCK_REALTIME, &ts2);
    
    time1 = (ts2.tv_sec + ts2.tv_nsec / MLD) - (ts1.tv_sec + ts1.tv_nsec / MLD);
    
    clock_gettime(CLOCK_REALTIME, &ts1);
    
    for (i = 1; i < sizeY - 1; i++)
    {
        for (j = 3; j < sizeX * 3 - 3; j++)
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
    
    strcat(fileName, "-edges");
    
    file = open(fileName, O_WRONLY | O_CREAT, S_IRUSR);
    
    for (i = 0; i < sizeY; i++)
    {
        write(file, result[i], sizeX * 3);
    }
    
    close(file);
    
    clock_gettime(CLOCK_REALTIME, &ts2);
    
    time3 = (ts2.tv_sec + ts2.tv_nsec / MLD) - (ts1.tv_sec + ts1.tv_nsec / MLD);
    
    printf("Read Time  = %3.10lf\n", time1);
    printf("Calc Time  = %3.10lf\n", time2);
    printf("Write Time = %3.10lf\n", time3);
    
    return 0;
}
