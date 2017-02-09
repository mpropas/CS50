/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    int n = atoi(argv[1]);
    if (argc != 4 || n < 1 || n > 100 || n % 1 != 0)
    {
        fprintf(stderr, "Usage: ./copy n (1 >= n >= 100) infile outfile\n");
        return 1;
    }

    // remember filenames and get n;
    //int n = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    //first take note of original width and height and padding
    int oldWidth = bi.biWidth;
    int oldHeight = bi.biHeight;
    int oldPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    // change width and height
    bi.biWidth *= n;
    bi.biHeight *= n;
    // determine padding for scanlines based on new width
    int newPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    //now change the bi.biSizeImage
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + newPadding) * abs(bi.biHeight);
    //now change the bf.bfSize
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(oldHeight); i < biHeight; i++)
    {
        //repeat each row a number of n times
        for(int q = 0; q < n; q++) {
            // iterate over pixels in scanline
            for (int j = 0; j < oldWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile
                //repeat each pixel n times
                for(int k = 0; k < n; k++) {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            
            }
            
            // then add padding
            for (int k = 0; k < newPadding; k++)
            {
                fputc(0x00, outptr);
            }

            // if have copied row n times then skip past padding otherwise move cursor to beginning of line to repeat
            if(q == n - 1) {
                fseek(inptr, oldPadding, SEEK_CUR);
            }
            else {
                fseek(inptr, -1 * oldWidth * (long)sizeof(RGBTRIPLE), SEEK_CUR);
            }
        }

        
        
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
