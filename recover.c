#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // Check for correct progrm usage
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open input file
    FILE *raw_file = fopen(argv[1], "r");
    if (raw_file == NULL)
    {
        printf("Could not open %s\n", argv[1]);
        return 1;
    }

    // Prepare for reading raw file
    const int BLOCK_SIZE = 512; // Size in bytes of one memory block in FAT
    typedef uint8_t BYTE;
    BYTE *buffor = malloc(BLOCK_SIZE * sizeof(BYTE));
    int image_number = 0;
    char out_filename[8];
    FILE *out_file;

    // Read input file in blocks
    while (fread(buffor, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        // Search for JPEG file beginning
        if (buffor[0] == 0xff && buffor[1] == 0xd8 && buffor[2] == 0xff && buffor[3] >= 0xe0 && buffor[3] <= 0xef)
        {
            // Close previous file if not first
            if (image_number > 0)
            {
                fclose(out_file);
            }

            // Generate new file name
            image_number++;
            if (image_number < 11)
            {
                sprintf(out_filename, "00%i.jpg", image_number - 1);
            }
            else
            {
                sprintf(out_filename, "0%i.jpg", image_number - 1);
            }

            // Create a new JPEG file to write to
            out_file = fopen(out_filename, "w");
            if (out_file == NULL)
            {
                printf("Failed to create a new file %s\n", out_filename);
                fclose(raw_file);
                free(buffor);
                return 1;
            }
        }

        // Copy bytes if image found
        if (image_number > 0)
        {
            fwrite(buffor, 1, BLOCK_SIZE, out_file);
        }
    }

    // Close the last generated file
    if (image_number > 0)
    {
        fclose(out_file);
    }

    // free memory, close input file
    fclose(raw_file);
    free(buffor);

    return 0;
}