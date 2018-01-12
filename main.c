#include <stdio.h>
#include <stdlib.h>

typedef enum
{
    false = ( 1 == 0 ),
    true = ( ! false )
} bool;


bool solutionFound = false;

void findSafeBanker( int processes, int resources, int currentProcess, int *processOrder, int *available, int *maximum, int *need)
{
    int i, i2; //iterator
    bool safe;
    for( i = 0; i < processes; i++ )
    {
        safe = true;

        //is the process in the process order already?
        for(i2 = 0; i2 < processes; i2++)
        {
            if(i+1 == processOrder[i2])
            {
                safe = false;
                break;
            }
        }
        //process was allready accouted for, try the next one
        if(safe == false)
            continue;

        //iterate processes to find safe state
        for( i2 = 0; i2 < resources; i2++ )
        {
            //subtact need from available
            available[i2]-= need[i*resources + i2];
            if(available[i2] <0)
                safe = false;
        }

        if(safe == true)
        {
            //is last process in order?
            processOrder[currentProcess] = i +1;
            if (currentProcess +1 == processes)
            {
                //we finished all processes
                printf("Safe Order: ");
                for(i2 =0; i2 < processes; i2++)
                {
                    printf("%i ",processOrder[i2]);
                }
                printf("\n");
                //return resources for next process
                for( i2 = 0; i2 < resources; i2++ )
                {
                    available[i2]+= need[i*resources + i2];
                }
                solutionFound =true;
            }
            else
            {
                //return resources
                for( i2 = 0; i2 < resources; i2++ )
                {
                    available[i2]+= maximum[i*resources + i2];
                }
                findSafeBanker(processes, resources, currentProcess +1, processOrder, available, maximum, need);
                //remove returned resourse for next process
                for( i2 = 0; i2 < resources; i2++ )
                {
                    available[i2]-= maximum[i*resources + i2];
                    available[i2]+= need[i*resources + i2];
                }
            }
            //clear order
            processOrder[currentProcess] = 0;
        }
        else
        {
            //return resources
            for( i2 = 0; i2 < resources; i2++ )
            {
                available[i2]+= need[i*resources + i2];
            }
        }

    }//try next process

}

int main()
{
    int i, i2; //iterators
    int processes;
    int resources;

    int *allocated;
    int *maximum;
    int *need;
    int *available;
    int *processOrder;

    char fileName[20];
    printf("Please enter a filename: ");
    gets(fileName);
    printf("\n");

    //read file. I know, this is a slow way of doing things
    //but I wanted to make it somewhat human readable.
    FILE *file = fopen (fileName, "r");
    {
        if(file == NULL)
        {
            printf("could not open file.\n");
            return -1;
        }

        //get processes
        fscanf(file, "%i", &processes);
        fscanf(file, "%i", &resources);

        //alocate memory
        allocated = malloc( sizeof (*allocated) * processes * resources );
        maximum = malloc( sizeof (*maximum) * processes * resources );
        need = malloc( sizeof (*need) * processes * resources );
        available = malloc( sizeof (*need) * resources );
        processOrder = malloc( sizeof (*processOrder) * processes );
        memset(processOrder ,0, sizeof (*processOrder) * processes);

        //get available
        for(i = 0; i < resources; i++)
        {
            fscanf(file, "%i", &available[i]);
        }

        //get allocated
        for(i = 0; i < processes; i++)
        {
            for(i2 = 0; i2 < resources; i2++)
            {
                fscanf(file, "%i", &allocated[i*resources + i2]);
            }
        }

        //get maximums
        for(i = 0; i < processes; i++)
        {
            for(i2 = 0; i2 < resources; i2++)
            {
                fscanf(file, "%i", &maximum[i*resources + i2]);
            }
        }
    }
    fclose(file);


    //calculate need
    for(i = 0; i < processes * resources; i++)
    {
        need[i] = maximum[i] - allocated[i];
        //printf("Need: %i\n",need[i]);
    }

    findSafeBanker(processes, resources, 0, processOrder, available, maximum, need);
    if(solutionFound == false)
    {
        printf("Sorry, no solutions.\n");
    }
    free(processOrder);
    free(available);
    free(need);
    free(maximum);
    free(allocated);

    return 0;
}
