#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

int main(int argc, string argv[])
{
    //exit program with code 1 if only one additional argument is not given
    if(argc != 2) {
        printf("Please provide a single input string!\n");
        return 1;
    }
    //ask for and receive plaintext
    printf("plaintext: ");
    string input = get_string();
    //convert argument to an integer k
    int k = atoi (argv[1]);
    char letter;
    printf("ciphertext: ");
    //inerate through inputed string
    for(int i = 0, n = strlen(input); i < n; i++) {
        letter = input[i];
        //if uppercase letter shift by k
        if(letter >= 65 && letter <= 65 + 26) {
            printf("%c", ((letter - 65 + k) % 26) + 65);
        }
        //else if lowercase letter also shift by k
        else if(letter >= 97 && letter <= 97 + 26) {
            printf("%c", ((letter - 97 + k) % 26) + 97);
        }
        //otherwise just print the letter as per original input
        else {
            printf("%c", letter);
        }
    }
    printf("\n");
    return 0;
}