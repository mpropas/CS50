//this script has mined 50 images from card.raw

#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        //print error if user uses incorrectly
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    

    //open the file
    FILE *card = fopen(argv[1], "r");
    if (card == NULL) {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }
    //open the write file
    //set the number for the jpg to 0 and create first output file
    int fileCount = 0;
    char filename[256];
    sprintf(filename, "%03i.jpg", fileCount);
    FILE *img = fopen(filename, "w");


    int mysize = 512;
    do {
        //setup buffer variable an array of bytes
        BYTE buffer[512] = {0};
        mysize = fread(&buffer, 1, 512, card);

        //if mataches the beginning of jpg then add 1 to filecount, convert to filename string, print to console and open new file,
        //checking for errors first
        if(buffer[0] == 255 && buffer[1] == 216 && buffer[2] == 255 && (buffer[3] & 0xf0 ) == 0xe0) {
            fclose(img);
            fileCount++;
            sprintf(filename, "%03i.jpg", fileCount);
            printf("%s\n", filename);
            
            img = fopen(filename, "w");
            if (img == NULL) {
                fprintf(stderr, "Could not open %s.\n", filename);
                return 3;
            }
        }
        
        //write to file as long as we have found a beginning already
        if(fileCount != 0) {
            fwrite(&buffer, 1, 512, img);
        }
    } while(mysize == 512); //continue until the size returned is not 512

    //just close file;
    fclose(card);
    fclose(img);

    //return
    return 0;
}
