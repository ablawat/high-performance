#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define SIZE_X 1920
#define SIZE_Y 1080

int main()
{
    const char inFileName[]  = "/mnt/adam-2/dane/nature-2-1920-1080.raw";
    const char outFileName[] = "/mnt/adam-2/dane/nature-2-1920-1080-edges.raw";
    
    int values[4];
    int maxValue;
    
    int i, j, k;
    
    char **image  = malloc(sizeof(char *) * SIZE_Y);
    char **result = malloc(sizeof(char *) * SIZE_Y);
    
    for (i = 0; i < SIZE_Y; i++)
    {
        image[i]  = malloc(sizeof(char) * SIZE_X * 3);
        result[i] = malloc(sizeof(char) * SIZE_X * 3);
        
        memset(result[i], 0, SIZE_X * 3);
    }
    
    int file = open(inFileName, O_RDONLY);
    
    for (i = 0; i < SIZE_Y; i++)
    {
        read(file, image[i], SIZE_X * 3);
    }
    
    close(file);
    
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
    
    file = open(outFileName, O_WRONLY | O_CREAT, S_IRUSR);
    
    for (i = 0; i < SIZE_Y; i++)
    {
        write(file, result[i], SIZE_X * 3);
    }
    
    close(file);
    
    return 0;
}
