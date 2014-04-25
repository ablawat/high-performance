#include "main.h"

void calculation(Pixel **image, Pixel **result, int **mask, unsigned int size_x, unsigned int size_y, MaskType mask_type)
{
	int new_pixel_red;
    int new_pixel_green;
    int new_pixel_blue;
    
    int mask_x_index;
    int mask_y_index;
    int mask_x_value;
    int mask_y_value;
    
    int mask_size = 0;
    int index = 0;
    
    int i, j;
    
    switch (mask_type)
    {
        case MASK_3: index = 1;
                     mask_size = 3;
                     break;
        case MASK_5: index = 2;
                     mask_size = 5;
                     break;
        case MASK_7: index = 3;
                     mask_size = 7;
                     break;
    }
    
    // Sumuje wszystkie wagi z maski
    int mask_sum = 0;
    
    for (i = 0; i < mask_size; i++)
    {
        for (j = 0; j < mask_size; j++)
        {
        	mask_sum += mask[i][j];
        }
    }
    
    for (i = index; i < size_y + index; i++)
    {
    	for (j = index; j < size_x + index; j++)
    	{
    		new_pixel_red   = 0;
    		new_pixel_green = 0;
    		new_pixel_blue  = 0;
    		
    		mask_y_index = 0;
    		mask_y_value = -index;
    		
    		while (mask_y_value <= index)
    		{
    			mask_x_index = 0;
    			mask_x_value = -index;
    			
    			while (mask_x_value <= index)
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
    		
    		result[i - index][j - index].red   = new_pixel_red   / mask_sum;
    		result[i - index][j - index].green = new_pixel_green / mask_sum;
    		result[i - index][j - index].blue  = new_pixel_blue  / mask_sum;
    	}
    }
}