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

char* Optimise(char* file)
{
    unsigned long long int length = strlen(file);

    unsigned long long int size = 0;
    unsigned long long int realSize = 0;

    for (unsigned long long int n = 0; n < length; n++)
    {
        if (file[n] == '<' || 
            file[n] == '>' || 
            file[n] == '+' || 
            file[n] == '-' )
        {
            size += 2;
            if (file[n] != file[n - 1])
                realSize += 2;
        } 
        else if (file[n] == '[' ||
                 file[n] == ']' ||
                 file[n] == '.' ||
                 file[n] == ',')
        {
            size++;
            realSize++;
        }
    }
    char* optimizedPrg = (char*)malloc(size + 1);

    optimizedPrg[size] = '\0';

    unsigned long long int prgWriteHead = 0;

    unsigned long long int n = 0;

    while (n <= size)
    {
        if (file[n] == '[' ||
            file[n] == ']' ||
            file[n] == '.' ||
            file[n] == ',')
        {
            optimizedPrg[prgWriteHead] = file[n];
            n++;
            prgWriteHead++;
        }
        else if (file[n] == '<' || 
                 file[n] == '>' || 
                 file[n] == '+' || 
                 file[n] == '-' )
        {

            optimizedPrg[prgWriteHead] = file[n];
            prgWriteHead++;
            optimizedPrg[prgWriteHead] = 1;

            n++;

            while (file[n] == file[n - 1] && n <= size && optimizedPrg[prgWriteHead] < 90)
            {
                optimizedPrg[prgWriteHead]++;
                n++;
            }
            prgWriteHead++;
        }
        else
        {
            n++;
        }
    }
    optimizedPrg[realSize + 1] = '\0';

    return optimizedPrg; 
    
}

unsigned long long int GetPrgSize(char* prg)
{
    unsigned long long int ret = 0;

    while (prg[ret] != 0)
    {
        ret++;
    }

    return ret;
}

void Interpriter(char* prg) 
{
    unsigned long long int length = GetPrgSize(prg);
    #define TAPE_LENGTH 1000000
    char* tape = malloc(TAPE_LENGTH);
    unsigned long long int memPointer = 0;
    unsigned long long int prgPointer = 0; //the instruction we are on
    unsigned int numStackedLoops = 0;
    
    while (prgPointer < length)
    {
        
        //printf("not here!\n");
        switch (prg[prgPointer])
        {
        case '+':
            tape[memPointer] += prg[prgPointer + 1];
            prgPointer++;
            break;
        case '-':
            tape[memPointer] -= prg[prgPointer + 1];
            prgPointer++;
            break;
        case '<':
            memPointer -= prg[prgPointer + 1];
            if (memPointer > TAPE_LENGTH)
            {
                printf("mem pointer out of bounds <\n\n EXITING...\n");
                exit(-2);
            }
            prgPointer++;
            break;
        case '>':
            memPointer += prg[prgPointer + 1];
            if (memPointer > TAPE_LENGTH)
            {
                printf("mem pointer out of bounds >\n\n EXITING...\n");
                exit(-2);
            }
            prgPointer++;
            break;
        case '.':
            printf("%c", tape[memPointer]);
            break;
        case '[':
            numStackedLoops++;

            if (tape[memPointer] == 0)
            {
                unsigned int numStackedLoopsEnd = numStackedLoops;
                while (numStackedLoopsEnd != numStackedLoops - 1)
                {
                    prgPointer++;
                    if (prg[prgPointer] == '[')
                    {
                        numStackedLoopsEnd++;
                    } 
                    else if (prg[prgPointer] == ']')
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
                    if (prg[prgPointer] == '[')
                    {
                        numStackedLoopsEnd++;
                    } 
                    else if (prg[prgPointer] == ']')
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

    char* optimizedPrg = Optimise(file);

    free(file);

    Interpriter(optimizedPrg); 

    free(optimizedPrg);
    free(length);

    return 0;
}