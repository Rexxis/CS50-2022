// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file

    // initiate array to hold header values
    uint8_t header_temp[HEADER_SIZE];

    // read header file size of 44 byte
    fread(header_temp, HEADER_SIZE, 1, input);
    // write to output file from header_temp
    fwrite(header_temp, HEADER_SIZE, 1, output);

    // TODO: Read samples from input file and write updated data to output file

    // initiate a buffer to hold 2 byte value (int16_t)
    int16_t buffer;
    // while loop to go through all data in the input file
    while (fread(&buffer, sizeof(int16_t), 1, input))
    {
        // multiply buffer by factor to modify volume
        buffer *= factor;
        // write the modified buffer to the output file
        fwrite(&buffer, sizeof(int16_t), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}
