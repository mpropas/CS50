#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

int main(void)
{
    //prompt user for name
    string input = get_string();
    //iterate through input string
    for(int i = 0, n = strlen(input); i < n; i++) {
        //if the char is not a space and this is the first letter or if the char before was a space, print a capital of this char to the console
        if(input[i] != ' ' && (i == 0 || input[i - 1] == ' ')) {
            printf("%c", toupper(input[i]));
        }
    }
    printf("\n");
}