/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

  //this code has passed check50 2016.find.less helpers.c and ./generate 1000 50 | ./find 127 and ./generate 1000 50 | ./find 128
 
#include <cs50.h>
#include <stdio.h>


#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    if (n <= 0) {
        return false;
    }
    
    
    int left = 0, right = n - 1, middle = 0;
    while(true) {
        middle = (left + right) / 2;
        if(values[middle] == value) {
            //printf("found %i at position %i\n", value, middle);
            return true;
        }
        else if (left == right) {
            //printf("exiting with count = %i\n",count);
            return false;
        }
        else if (values[middle] < value) {
            left = middle + 1;
        }
        else if (values[middle] > value) {
            right = middle - 1;
        }
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    //assume numbers can be from 0 to 65535
    int MAX = 65535;
    int counterArray[MAX + 1];
    //initialize counter array manually
    for(int i = 0; i < MAX + 1; i++) {
        counterArray[i] = 0;
    }
    
    for(int i = 0; i < n; i++) {
        counterArray[values[i]] ++;
    }
    int index = 0;
    for(int i = 0; i < MAX + 1; i++) {
        for(int j = 0; j < counterArray[i]; j++) {
            values[index] = i;
            index++;
        }
    }
    //look at sorted array
    /*
    for(int i = 0; i < n; i++) {
        printf("%i, ", values[i]);
    }
    */
    
    return;
}
