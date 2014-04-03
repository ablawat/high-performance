#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define SIZE_X 128
#define SIZE_Y 128

int main()
{
    int i, j;
    
    char **image  = malloc(sizeof(char *) * SIZE_Y);
    char **result = malloc(sizeof(char *) * SIZE_Y);
    
    for (i = 0; i < SIZE_Y; i++)
    {
        image[i]  = malloc(sizeof(char) * SIZE_X * 3);
        result[i] = malloc(sizeof(char) * SIZE_X * 3);
        
        memset(result[i], 0, SIZE_X * 3);
    }
    
    int file = open("/mnt/adam-2/dane/image-1-128-128.raw", O_RDONLY);
    
    for (i = 0; i < SIZE_Y; i++)
    {
        read(file, image[i], SIZE_X * 3);
    }
    
    close(file);
    
    for (i = 0; i < SIZE_Y; i++)
    {
        for (j = 3; j < SIZE_X * 3; j++)
        {
            result[i][j] = (char)abs(image[i][j] - image[i][j - 3]);
        }
    }
    
    file = open("/mnt/adam-2/dane/image-1-128-128-edges.raw", O_WRONLY | O_CREAT, S_IRUSR);
    
    for (i = 0; i < SIZE_Y; i++)
    {
        write(file, result[i], SIZE_X * 3);
    }
    
    close(file);
    
    return 0;
}
