#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char* LoadFile(const char* fileName, size_t* len){
    char * buffer = 0;
    unsigned long long length;
    FILE * f = fopen (fileName, "rb");

    if (f)
    {
      fseek (f, 0, SEEK_END);
      length = ftell (f);
	  *len = length;
      fseek (f, 0, SEEK_SET);
      buffer = malloc (length);
      if (buffer)
      {
        fread (buffer, 1, length, f);
      }
      fclose (f);
    }

    if (buffer)
    {
      return buffer;
    }
    return 0;
}

void interpriter(char* file) 
{
    unsigned long long int length = strlen(file);
    #define TAPE_LENGTH 1000000
    char* tape = malloc(TAPE_LENGTH);
    unsigned long long int memPointer = 0;
    unsigned long long int prgPointer = 0; //the instruction we are on
    unsigned int numStackedLoops = 0;
    
    while (prgPointer < length + 1)
    {
        
        //printf("not here!\n");
        switch (file[prgPointer])
        {
        case '+':
            tape[memPointer] += 1;
            break;
        case '-':
            tape[memPointer] -= 1;
            break;
        case '<':
            memPointer -= 1;
            if (memPointer > TAPE_LENGTH)
            {
                printf("mem pointer out of bounds\n\n EXITING...\n");
                exit(-2);
            }
            break;
        case '>':
            memPointer += 1;
            if (memPointer > TAPE_LENGTH)
            {
                printf("mem pointer out of bounds\n\n EXITING...\n");
                exit(-2);
            }
            break;
        case '.':
            printf("%c", tape[memPointer]);
        case '[':
            numStackedLoops++;

            if (tape[memPointer] == 0)
            {
                unsigned int numStackedLoopsEnd = numStackedLoops;
                while (numStackedLoopsEnd != numStackedLoops - 1)
                {
                    prgPointer++;
                    if (file[prgPointer] == '[')
                    {
                        numStackedLoopsEnd++;
                    } 
                    else if (file[prgPointer] == ']')
                    {
                        numStackedLoopsEnd--;
                    }

                }
                numStackedLoops--;
            }
            break;
        case ']':
            numStackedLoops--;

            if (tape[memPointer] != 0)
            {
                unsigned int numStackedLoopsEnd = numStackedLoops;
                while (numStackedLoopsEnd != numStackedLoops + 1)
                {
                    prgPointer--;
                    if (file[prgPointer] == '[')
                    {
                        numStackedLoopsEnd++;
                    } 
                    else if (file[prgPointer] == ']')
                    {
                        numStackedLoopsEnd--;
                    }

                }
                numStackedLoops++;
            }

            break;
        case ',':;

            char c = getchar();

            tape[memPointer] = c;

            break;
        default:
            break;
        }
        prgPointer++;
    }
    printf("\nPROG EXITING EXECUTION DONE \n");
    free(tape);
    return;
}

int main(int argc, char *argv[]) 
{

    if (argc != 2) 
    {
        printf("THIS PROGRAM NEEDS ONLY 1 ARGUMENT");

        return -1;
    }

    size_t* length = (size_t*)malloc(sizeof(size_t)); 

    char* file = LoadFile(argv[1], length);

    

    interpriter(file); 

    free(file);
    free(length);

    return 0;
}