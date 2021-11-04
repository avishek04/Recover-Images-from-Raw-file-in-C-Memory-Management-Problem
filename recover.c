#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Usage: ./recover image");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if(file == NULL)
    {
        printf("File cannot be opened");
        return 1;
    }

    BYTE fileCopy[512];
    bool foundfirstJPG = false;
    FILE *newJPGPtr;
    int fileCounter = 0;

    while(fread(fileCopy, 1, 512, file) != 0x00)
    {
        if(fileCopy[0] == 0xff && fileCopy[1] == 0xd8 && fileCopy[2] == 0xff && (fileCopy[3] & 0xf0) == 0xe0)
        {
            if(!foundfirstJPG)
            {
                foundfirstJPG = true;
            }
            char filename[8];
            sprintf(filename, "%03i.jpg", fileCounter);
            fileCounter += 1;
            newJPGPtr = fopen(filename, "w");
            if(newJPGPtr == NULL)
            {
                return 2;
            }
            fwrite(fileCopy, 1, 512, newJPGPtr);
        }
        else
        {
            if(foundfirstJPG)
            {
                fwrite(fileCopy, 1, 512, newJPGPtr);
            }
        }
    }
    fclose(newJPGPtr);
    fclose(file);
}
