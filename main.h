#ifndef MAIN_H
	
	#define MAIN_H
	
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

#endif