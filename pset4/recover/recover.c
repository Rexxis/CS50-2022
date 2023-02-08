#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover [filename]\n");
        return 1;
    }

    // open input file
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        printf("Could not read file: %s\n", argv[1]);
        return 1;
    }

    // create new datatype BYTE
    typedef uint8_t BYTE;

    // create constant block size
    const int BLOCK_SIZE = 512;

    // create a buffer to store data
    BYTE buffer[BLOCK_SIZE];

    // variable to count jpg names
    int count = 0;

    // create first file name
    char *filename = malloc(8);
    sprintf(filename, "%03i.jpg", count);

    // assign first output file
    FILE *outptr = fopen(filename, "w");

    // read the input file per 512 byte blocks
    while (fread(buffer, 1, BLOCK_SIZE, inptr) == BLOCK_SIZE)
    {
        // if current buffer the start of new jpg
        if ((buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff) && ((buffer[3] & 0xf0) == 0xe0))
        {
            // if it's the first file
            if (count == 0)
            {
                // increment count for jpg names
                count++;

                // write to output file
                fwrite(buffer, 512, 1, outptr);

            }
            // if it's not the first file
            else
            {
                // close output file pointer
                fclose(outptr);

                // create filename for output JPG files
                sprintf(filename, "%03i.jpg", count);

                // increment count for jpg names
                count++;

                // open new output file
                outptr = fopen(filename, "w");
                fwrite(buffer, 512, 1, outptr);

            }
        }
        // if block is not start of a jpg
        else if (count >= 1)
        {
            // continue writing output
            fwrite(buffer, 512, 1, outptr);
        }
    }

    // close all file pointers
    fclose(inptr);
    fclose(outptr);

    // free memory allocated for filename
    free(filename);
}