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
    unsigned int mask_size;
    
    MaskSize mask_type = MASK_3;
    
    double time1;
    double time2;
    double time3;
    
    int i, j, k;
    
    
    strcpy(file_name, args[1]);
    size_x = atoi(args[2]);
    size_y = atoi(args[3]);
    
    
    switch (mask_type)
    {
    	case MASK_3: extended = 2;
    	             mask_size = 3;
    	             break;
    	case MASK_5: extended = 4;
    	             mask_size = 5;
    	             break;
    	case MASK_7: extended = 6;
    	             mask_size = 7;
    	             break;
    }
    
    extended_size_x = size_x + extended;
    extended_size_y = size_y + extended;
    
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
    
    // Wczytuje maskę
    for (i = 0; i < mask_size; i++)
    {
    	for (j = 0; j < mask_size; j++)
    	{
    		mask[i][j] = 1;
    	}
    }
    
    clock_gettime(CLOCK_REALTIME, &ts1);
    
    // Wczytuje macierz wejściową z pliku
    int file = open(file_name, O_RDONLY);
    
    for (i = extended / 2; i < size_y + extended / 2; i++)
    {
        io_result = read(file, image[i] + (extended / 2), size_x * sizeof(Pixel));
        
        if (io_result == -1)
        {
            fputs("Read error\n", stderr);
        }
    }
    
    close(file);
    
    // Kopiuje skrajne piksele do obszaru rozszerzonego
    unsigned int index = extended / 2;
    
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
    int new_pixel_red;
    int new_pixel_green;
    int new_pixel_blue;
    
    int mask_x_index;
    int mask_y_index;
    int mask_x_value;
    int mask_y_value;
    puts("ok");
    for (i = extended / 2; i < size_y + extended / 2; i++)
    {
    	for (j = extended / 2; j < size_x + extended / 2; j++)
    	{
    		new_pixel_red   = 0;
    		new_pixel_green = 0;
    		new_pixel_blue  = 0;
    		
    		mask_y_index = 0;
    		mask_y_value = -(extended / 2);
    		
    		while (mask_y_value <= extended / 2)
    		{
    			mask_x_index = 0;
    			mask_x_value = -(extended / 2);
    			
    			while (mask_x_value <= extended / 2)
    			{
    				new_pixel_red   += image[i + mask_y_value][j + mask_x_value].red   * mask[mask_y_index][mask_x_index];
    				new_pixel_green += image[i + mask_y_value][j + mask_x_value].green * mask[mask_y_index][mask_x_index];
    				new_pixel_blue  += image[i + mask_y_value][j + mask_x_value].blue  * mask[mask_y_index][mask_x_index];
    				
    				mask_x_index++;
    				mask_x_value++;
    			}
    			
    			mask_y_index++;
    			mask_y_value++;
    		}
    		
    		result[i - extended / 2][j - extended / 2].red   = (unsigned char)(new_pixel_red   / ((extended + 1) * (extended + 1)));
    		result[i - extended / 2][j - extended / 2].green = (unsigned char)(new_pixel_green / ((extended + 1) * (extended + 1)));
    		result[i - extended / 2][j - extended / 2].blue  = (unsigned char)(new_pixel_blue  / ((extended + 1) * (extended + 1)));
    	}
    }
    
    clock_gettime(CLOCK_REALTIME, &ts2);
    
    time2 = (ts2.tv_sec + ts2.tv_nsec / MLD) - (ts1.tv_sec + ts1.tv_nsec / MLD);
    
    
    clock_gettime(CLOCK_REALTIME, &ts1);
    
    strcat(file_name, "-test");
    
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
