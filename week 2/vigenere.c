//this code has passed check50 2016.vigenere vigenere.c

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
    else {
        char letter;
        //cycle through key checking for non letters, if one is found exit the program with code 1
        for(int i = 0, n = strlen(argv[1]); i < n; i++) {
            letter = argv[1][i];
            if(letter < 65 || letter > 122 || (letter >= 91 && letter <= 96)) {
                printf("Please provide a single input string!\n");
                return 1;
            }
        }
    }
    //ask for and receive plaintext
    printf("plaintext: ");
    string input = get_string();
    char letter;
    printf("ciphertext: ");
    //iterate through inputed string
    for(int i = 0, j = 0, n = strlen(input), m = strlen(argv[1]), k; i < n; i++) {
        letter = input[i];
        //set k to be the jth character in the key, recycling through with the % operator
        k = argv[1][j % m];
        //if uppercase k shift by 65
        if(k >= 65 && k <= 65 + 26) {
            k -= 65;
        }
        //if lowercase k shift by 97
        else if(k >= 97 && k <= 97 + 26) {
            k -= 97;
        }
        //if letter is uppercase shift by k and increment j
        if(letter >= 65 && letter <= 65 + 26) {
            printf("%c", ((letter - 65 + k) % 26) + 65);
            j++;
        }
        //else if lowercase letter also shift by k and increment j
        else if(letter >= 97 && letter <= 97 + 26) {
            printf("%c", ((letter - 97 + k) % 26) + 97);
            j++;
        }
        //otherwise just print the letter as per original input
        else {
            printf("%c", letter);
        }
    }
    printf("\n");
    return 0;
}