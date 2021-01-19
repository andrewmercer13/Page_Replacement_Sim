/*
 * File:	source.c
 * Author:	Andrew Merer
 * Date:	2020-07-19
 * Version:	1.0
 *
 * Purpose:	To simulate various page replacement algrithms
 * 
 * Notes:   Please use the echo command to pipe in values ie:
 *          echo 1 2 3 4 5 6 7 | ./A5 5 
 *          use make to compile. 
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#define MAX_READ_IN     10000
#define DEFAULT_OUTPUT  -1
#define STRTOL_BASE     10
#define MAX_MEM_VAL     5000
#define INITAL_FREQ     999999
#define INITAL_LOWEST   100000
void FIFO(int read_in_count,int num_of_frames);
void LRU(int read_in_count, int num_of_frames);
void LFU(int read_in_count, int num_of_frames);
void MFU(int read_in_count, int num_of_frames);
void SECOND_CHANCE(int read_in_count, int num_of_frames);
void optimal(int read_in_count,int num_of_frames);

int array[MAX_READ_IN];

int page_fault_count = DEFAULT_OUTPUT;

/*
 * Name:	main()
 * Purpose:	Calling page replacement algrithm functions and 
 *          printing the number of page faults 
 * Arguments:	argc, argv and an array to hold the values in.
 * Outputs:	Error messages only.
 * Modifies:	The array 'array'.
 * Returns:	0 on success.
 * Bugs:	
 * Notes: page fault values printed as -1 by default.
 */
int main(int argc, char *argv[])
{
    int read_in_count = 0;
    /* Converts string to int*/
    int num_of_frames = strtol(argv[1],NULL,STRTOL_BASE); 
    
    while(!feof(stdin))
    {
        scanf("%d",&array[read_in_count]);
        read_in_count++;
    }
    /* Reads in one extra value */
    read_in_count--;
    printf("For a system with %d frames:\n", num_of_frames);
    
    optimal(read_in_count,num_of_frames);
    printf("Optimal page faults: %d\n", page_fault_count);
    page_fault_count = DEFAULT_OUTPUT;
    
    FIFO(read_in_count,num_of_frames);
    printf("FIFO page faults: %d\n", page_fault_count);
    page_fault_count = DEFAULT_OUTPUT;
    
    LRU(read_in_count,num_of_frames);
    printf("LRU page faults: %d\n", page_fault_count);
    page_fault_count = DEFAULT_OUTPUT;
    
    LFU(read_in_count,num_of_frames);
    printf("LFU page faults: %d\n", page_fault_count);
    page_fault_count = DEFAULT_OUTPUT;
    
    MFU(read_in_count,num_of_frames);
    printf("MFU page faults: %d\n", page_fault_count);
    page_fault_count = DEFAULT_OUTPUT;
    
    SECOND_CHANCE(read_in_count,num_of_frames);
    printf("Second chance page faults: %d\n", page_fault_count);
    page_fault_count = DEFAULT_OUTPUT;

    
    return EXIT_SUCCESS;

}
/*
 * Name:	optimal()
 * Purpose:	To simulate the optimal page replacement algrithm
 * Arguments:	read_in_count and num_of_frames.
 * Outputs:	.
 * Modifies:	page_fault_count.
 * Returns:	0 on success.
 * Bugs:  Not fully implemented.
 * Notes: page fault values printed as -1 by default.
 */
void optimal(int read_in_count,int num_of_frames)
{
    /* Struct for keeping track of frame table values */
    typedef struct 
    {
        int frame;
        int times_used;

    }frame_table;
    /* Struct for keeping track of memory values */
    typedef struct 
    {
        bool checkarray;
        int index;
    }memory_table;
    /* Used to remove remove -1 default value */
    page_fault_count++; 

    frame_table frame[num_of_frames + 1];
    memory_table mem[MAX_MEM_VAL];
    int count = 0; 
    int current_lowest ;   
    int i = 0;
    /* Used to set all bool values to false*/
    while(i <= MAX_MEM_VAL)
    {
        mem[i].checkarray = false;
        i++;
    }
    i = 0;
    /* Loops for all values echo'd in */
    while(count < read_in_count)
    {
        /* Checks if value is in memory */
        if(mem[array[count]].checkarray == false)
        {
            current_lowest = num_of_frames;

            while(i < num_of_frames)
            {
                if(frame[i].times_used < frame[current_lowest].times_used)
                {
                    current_lowest = i;
                }
                i++;
            }

            /* Updates all values in memory and bool array*/
            mem[array[count]].index = current_lowest;
            mem[array[current_lowest]].checkarray= false;
            mem[array[count]].checkarray= true;
            frame[current_lowest].times_used = count;
            frame[current_lowest].frame = array[count];
            page_fault_count++;
        }
        else
        {
            /* Increases number of times used*/
            frame[mem[array[count]].index].times_used = count;
        }
        count++;
        
    }
    return;
}
/*
 * Name:	FIFO()
 * Purpose:	To simulate the FIFO page replacement algrithm
 * Arguments:	read_in_count and num_of_frames.
 * Outputs:	.
 * Modifies:	page_fault_count.
 * Returns:	0 on success.
 * Bugs:
 * Notes: page fault values printed as -1 by default.
 */
void FIFO(int read_in_count,int num_of_frames)
{
    /* This is used to look up if a value is in memory */
    bool checkarray[MAX_MEM_VAL]; 
    page_fault_count++;    
    /* Initalize memory frame array */         
    int framearray[num_of_frames];
    int count = 0;
    int q_index = 0;
    int i = 0;

    while(i <= MAX_MEM_VAL)
    {
        checkarray[i] = false;
        i++;
    }
    /* Loops for all values input at run time */
    while(count < read_in_count)
    {
        /* Checks to see if current value is not in memory */
        if(checkarray[array[count]] == false)
        {   
            /* Sets current value's index to true in array*/
            checkarray[array[count]] = true;
            /* Sets previous value in memory array's index to false */
            checkarray[framearray[q_index]] = false;
            /* Adds new value to memory array */
            framearray[q_index] = array[count];
            page_fault_count++;
            /* Circular queue index increase */
            q_index++;
            
            /* Moves index to 0 once it reaches the total number of frames*/
            if(q_index >= num_of_frames)
            {
                q_index = 0;
            }
        }
        /* Testing print statement */
        count++;
        
    }
    return;
}
/*
 * Name:	LRU()
 * Purpose:	To simulate the LRU page replacement algrithm
 * Arguments:	read_in_count and num_of_frames.
 * Outputs:	.
 * Modifies:	page_fault_count.
 * Returns:	0 on success.
 * Bugs:
 * Notes: page fault values printed as -1 by default.
 */
void LRU(int read_in_count, int num_of_frames)
{
    /* Struct for keeping track of frame table values */
    typedef struct 
    {
        int frame;
        int times_used;

    }frame_table;
    /* Struct for keeping track of memory values */
    typedef struct 
    {
        bool checkarray;
        int index;
    }memory_table;
    /* Brings starting values up to 0 */
    page_fault_count++; 

    frame_table frame[num_of_frames + 1];
    memory_table mem[MAX_MEM_VAL];
    int count = 0; 
    int current_lowest ;   
    int i = 0;
    /* Sets all values in bool array to false*/
    while(i <= MAX_MEM_VAL)
    {
        mem[i].checkarray = false;
        i++;
    }
    i = 0;
    /* Loops for all values read in */
    while(count < read_in_count)
    {
        /* If value not in memory*/
        if(mem[array[count]].checkarray == false)
        {
            current_lowest = num_of_frames;
            
            while(i < num_of_frames)
            {
                /* Compares previous to new value of times used*/
                if(frame[i].times_used < frame[current_lowest].times_used)
                {
                    current_lowest = i;
                }
                i++;
            }

            /* Sets value in memory to new value and updates bool array */
            mem[array[count]].index = current_lowest;
            mem[array[current_lowest]].checkarray= false;
            mem[array[count]].checkarray= true;
            frame[current_lowest].times_used = count;
            frame[current_lowest].frame = array[count];
            page_fault_count++;
        }
        else
        {
            /* Increases used time if value is already in memory */
            frame[mem[array[count]].index].times_used = count;
        }
        count++;
        
    }
    return;
}
/*
 * Name:	LFU()
 * Purpose:	To simulate the LFU page replacement algrithm
 * Arguments:	read_in_count and num_of_frames.
 * Outputs:	.
 * Modifies:	page_fault_count.
 * Returns:	0 on success.
 * Bugs:
 * Notes: page fault values printed as -1 by default.
 */
void LFU(int read_in_count, int num_of_frames)
{
    /* Struct for keeping track of frame table values */
    typedef struct 
    {
        int frame;
        int times_used;

    }frame_table;
    /* Struct for keeping track of memory values */
    typedef struct 
    {
        bool checkarray;
        int freq;
        int index;
    }memory_table;
    page_fault_count++; 
    int i = 0;
    int count = 0;
    int j;
    int current_lowest;
    
    frame_table frame[num_of_frames];
    memory_table mem[MAX_MEM_VAL];
    /* Sets frequency and bool array values to default values */
    while(i < MAX_MEM_VAL)
    {
        mem[i].checkarray = false;
        mem[i].freq = INITAL_FREQ;
        i++;

    }
    i = 0;
    while(count < num_of_frames)
    {
        /* Checks if value is in memory */
        if(mem[array[i]].checkarray == false)
        {
            frame[i].frame = array[i];
            count++;
            mem[array[i]].checkarray = true;
            page_fault_count++;
        }
        mem[array[i]].freq++;
        i++;
        
    }
    j = i;
    while(j < read_in_count)
    {
        if(mem[array[j]].checkarray == false)
        {
            /* Sets inital value to be outside of acceptable range to avoid errors*/
            current_lowest = INITAL_LOWEST;
            i = 0;
            while(i < num_of_frames)
            {
                /* Checks if current frequency is less than previous*/
                if(mem[frame[i].frame].freq < current_lowest)
                {
                    current_lowest = mem[frame[i].frame].freq;
                    count = i;
                }
                i++;
            }
            /* Sets values in memory and updating bool array */
            mem[array[j]].index = i;
            mem[frame[count].frame].checkarray = false;
            mem[array[j]].checkarray = true;
            frame[count].frame = array[j];
            page_fault_count++;
        }
        mem[array[j]].freq++;
        j++;
    }
    return;
}
/*
 * Name:	MFU()
 * Purpose:	To simulate the LRU page replacement algrithm
 * Arguments:	read_in_count and num_of_frames.
 * Outputs:	.
 * Modifies:	page_fault_count.
 * Returns:	0 on success.
 * Bugs:
 * Notes: page fault values printed as -1 by default.
 */
void MFU(int read_in_count, int num_of_frames)
{
    /* Struct for keeping track of frame table values */
    typedef struct 
    {
        int frame;
        int times_used;

    }frame_table;
    /* Struct for keeping track of memory values */
    typedef struct 
    {
        bool checkarray;
        int freq;
        int index;
    }memory_table;
    page_fault_count++; 
    int i = 0;
    int count = 0;
    int j;
    int current_lowest;
    
    frame_table frame[num_of_frames];
    memory_table mem[MAX_MEM_VAL];
    /* Sets frequency and bool array values to default values */
    while(i < MAX_MEM_VAL)
    {
        mem[i].checkarray = false;
        mem[i].freq = 0;
        i++;

    }
    i = 0;
    /* Loops for all values input */
    while(count < num_of_frames)
    {
        /* Checks if value is in memory */
        if(mem[array[i]].checkarray == false)
        {
            frame[count].frame = array[i];
            count++;
            mem[array[i]].checkarray = true;
            page_fault_count++;
        }
        mem[array[i]].freq++;
        i++;
        
    }
    j = i;
    while(j < read_in_count)
    {
        if(mem[array[j]].checkarray == false)
        {
            current_lowest = 0;
            i = 0;/* Struct for keeping track of memory values */
            while(i < num_of_frames)
            {
                /* Checks if new frequency is greater than previous*/
                if(mem[frame[i].frame].freq > current_lowest)
                {
                    current_lowest = mem[frame[i].frame].freq;
                    count = i;
                }
                i++;
            }
            /* Updates value in memory and updates bool array*/
            mem[array[j]].index = i;
            mem[frame[count].frame].checkarray = false;
            mem[array[j]].checkarray = true;
            frame[count].frame = array[j];
            page_fault_count++;
        }
        mem[array[j]].freq++;
        j++;
    }
    return;
}
/*
 * Name:	SECOND_CHANCE()
 * Purpose:	To simulate the SECOND_CHANCE page replacement algrithm
 * Arguments:	read_in_count and num_of_frames.
 * Outputs:	.
 * Modifies:	page_fault_count.
 * Returns:	0 on success.
 * Bugs:
 * Notes: page fault values printed as -1 by default.
 */
void SECOND_CHANCE(int read_in_count, int num_of_frames)
{
    /* Struct for keeping track of frame table values */
    typedef struct 
    {
        int frame;
        int second;
        int times_used;

    }frame_table;
    /* Struct for keeping track of memory values */
    typedef struct 
    {
        bool checkarray;
        int index;
    }memory_table;
    page_fault_count++;
    frame_table frame[num_of_frames];
    memory_table mem[MAX_MEM_VAL];
    int i = 0;
    int j = 0;
    int count = 0;
    /* Sets frequency and bool array values to default values */
    while(i < MAX_MEM_VAL)
    {
        mem[i].checkarray = false;
        i++;
    }
    /* Loops for all values read in */
    while(j < read_in_count)
    {
        /* Checks if value is in memory */
        if(mem[array[j]].checkarray == false)
        {
            /* Checks if on second or first check */
            while(frame[count].second == 1)
            {
                /* Resets check count */
                frame[count].second = 0;
                count++;
                if(count == num_of_frames)
                {
                    count = 0;
                }
            }
            /* Updates value in memory and bool array*/
            mem[frame[count].frame].checkarray = false;
            mem[array[j]].checkarray = true;
            mem[array[j]].index = count;
            frame[count].second = 1;
            frame[count].frame = array[j];
            page_fault_count++;
        }
        j++;
    }

    return;
}