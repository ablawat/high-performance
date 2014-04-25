#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#include "main.h"
#include "calculation.h"

int main(int argc, char **args)
{
    char image_file_name[128];
    char mask_file_name[128];
    
    int io_result;
    int extended;
    int index;
    int mask_int;
    
    struct timespec ts1;
    struct timespec ts2;
    
    unsigned int size_x;
    unsigned int size_y;
    
    unsigned int extended_size_x;
    unsigned int extended_size_y;
    unsigned int mask_size;
    
    MaskType mask_type = MASK_3;
    
    double time1;
    double time2;
    double time3;
    
    int i, j;
    
    
    strcpy(image_file_name, args[1]);
    strcpy(mask_file_name, args[2]);
    size_x = atoi(args[3]);
    size_y = atoi(args[4]);
    mask_int = atoi(args[5]);
    
    
    switch (mask_int)
    {
        case 3: extended = 2;
                mask_size = 3;
                mask_type = MASK_3;
                break;
        case 5: extended = 4;
                mask_size = 5;
                mask_type = MASK_5;
                break;
        case 7: extended = 6;
                mask_size = 7;
                mask_type = MASK_7;
                break;
    }
    
    extended_size_x = size_x + extended;
    extended_size_y = size_y + extended;
    
    index = extended / 2;
    
    // Tworzy macierz wejściową i macierz wyjściową
    Pixel **image  = malloc(sizeof(Pixel *) * extended_size_y);
    Pixel **result = malloc(sizeof(Pixel *) * size_y);
    
    for (i = 0; i < extended_size_y; i++)
    {
        image[i]  = malloc(sizeof(Pixel) * extended_size_x);
        result[i] = malloc(sizeof(Pixel) * size_x);
    }
    
    // Tworzy macierz maski
    int **mask = malloc(sizeof(int *) * mask_size);
    
    for (i = 0; i < mask_size; i++)
    {
        mask[i] = malloc(sizeof(int) * mask_size);
    }
    
    
    clock_gettime(CLOCK_REALTIME, &ts1);
    
    // Wczytuje macierz wejściową z pliku
    int file = open(image_file_name, O_RDONLY);
    
    for (i = index; i < size_y + index; i++)
    {
        io_result = read(file, image[i] + index, size_x * sizeof(Pixel));
        
        if (io_result == -1)
        {
            fputs("Read error\n", stderr);
        }
    }
    
    close(file);
    
    // Wczytuje maskę
    FILE *mask_file = fopen(mask_file_name, "r");
    
    for (i = 0; i < mask_size; i++)
    {
        for (j = 0; j < mask_size; j++)
        {
            fscanf(mask_file, "%d", &mask[i][j]);
        }
    }
    
    //Test
    /*
    for (i = 0; i < mask_size; i++)
    {
        for (j = 0; j < mask_size; j++)
        {
            printf("%d ", mask[i][j]);
        }
        
        printf("\n");
    }
    */
    
    // Kopiuje skrajne piksele do obszaru rozszerzonego
    for (i = 0; i < index; i++)
    {
        memcpy(image[i], image[index], sizeof(Pixel) * extended_size_x);
        
        unsigned int src_index = extended_size_y - index - 1;
        unsigned int des_index = extended_size_y - i - 1;
        
        memcpy(image[des_index], image[src_index], sizeof(Pixel) * extended_size_x);
    }
    
    for (i = 0; i < extended_size_y; i++)
    {
        for (j = 0; j < index; j++)
        {
            memcpy(image[i] + j, image[i] + index, sizeof(Pixel));
            
            unsigned int src_index = extended_size_x - index - 1;
            unsigned int des_index = extended_size_x - j - 1;
            
            memcpy(image[i] + des_index, image[i] + src_index, sizeof(Pixel));
        }
    }
    //
    
    clock_gettime(CLOCK_REALTIME, &ts2);
    
    time1 = (ts2.tv_sec + ts2.tv_nsec / MLD) - (ts1.tv_sec + ts1.tv_nsec / MLD);
    
    
    clock_gettime(CLOCK_REALTIME, &ts1);
    
    calculation(image, result, mask, size_x, size_y, mask_type);
    
    clock_gettime(CLOCK_REALTIME, &ts2);
    
    time2 = (ts2.tv_sec + ts2.tv_nsec / MLD) - (ts1.tv_sec + ts1.tv_nsec / MLD);
    
    
    clock_gettime(CLOCK_REALTIME, &ts1);
    
    // Zapisuje macierz wyjściową do pliku
    strcat(image_file_name, "-test");
    
    file = open(image_file_name, O_WRONLY | O_CREAT, S_IRUSR);
    
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
    
    // Wyświetla czasy
    printf("Read Time  = %3.10lf\n", time1);
    printf("Calc Time  = %3.10lf\n", time2);
    printf("Write Time = %3.10lf\n", time3);
    
    return 0;
}
