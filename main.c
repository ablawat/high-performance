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
    const size_t dataSize = sizeof(char) * SIZE_X * SIZE_Y * 3;
    
    char *image  = malloc(dataSize);
    char *result = malloc(dataSize);
    
    memset(result, 0, dataSize);
    
    int file = open("/mnt/adam-2/dane/image-1-128-128.raw", O_RDONLY);
    
    read(file, image, dataSize);
    
    return 0;
}
