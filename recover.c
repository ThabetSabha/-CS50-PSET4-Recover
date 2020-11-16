#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;


int main(int argc, char *argv[])
{
    //Check for 2 arguments;
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    
    FILE *image = fopen(argv[1], "r");
    FILE *output = NULL;
    char fileName[8];
    
    //Check if the input exists
    if (image == NULL)
    {
        printf("sorry, couldn't find %s\n", argv[1]);
        return 1;
    }
    
    //our buffer
    BYTE jpeg[512];
    int counter = 0;
    
    
    //Only takes 512 byte-sized blocks
    while (fread(jpeg, 1, 512, image) == 512)       
    {   
        //Check to see if it's a JPEG
        if (jpeg[0] == 0xff && jpeg[1] == 0xd8 && jpeg[2] == 0xff && ((jpeg[3] & 0xf0) == 0xe0))
        {   
            if (counter > 0)
            {
                fclose(output);
            }
            sprintf(fileName, "%03i.jpg", counter);
            counter++;
            output = fopen(fileName, "w");
            fwrite(jpeg, 1, 512, output);
        }
        
        else if (output != NULL)   
        {   
            sprintf(fileName, "%03i.jpg", counter);
            fwrite(jpeg, 1, 512, output);
        }
    }
    
    fclose(output);
    fclose(image);
}