//as per CS50X's top challenge for week 1, this C program validates credit cards as per Luhn’s algorithm. The code below has passed Stanfords built in test.

#include <stdio.h>
#include <cs50.h>
#include <string.h>

int main(void)
{
    printf("Number: ");

    unsigned long num = get_long_long();
    char input[256];
    sprintf(input, "%lu", num);
    unsigned long length = strlen(input);
    
    long double_sum = 0, single_sum = 0;
    for(int i = 0; i < length; i++) {
        if((i % 2 == 0 && length % 2 != 0) || (i % 2 != 0 && length % 2 == 0)) {
            single_sum += (input[i] - '0');
        }
        else {
            switch(2 * (input[i] - '0')) {
                case 10:
                case 12:
                case 14:
                case 16:
                case 18:
                    double_sum += 1 + (2 * (input[i] - '0') - 10);
                    break;
                default:
                    double_sum += 2 * (input[i] - '0');
                    break;
            }
        }
    }

    if((single_sum + double_sum) % 10 == 0) {
        if(length == 15 && input[0] == '3' && (input[1] == '4' || input[1] == '7')) {
            printf("AMEX\n");
        }
        else if(length == 16 && input[0] == '5' && (input[1] == '1' || input[1] == '2' || input[1] == '3' || input[1] == '4' || input[1] == '5')) {
            printf("MASTERCARD\n");
        }
        else if((length == 13 || length == 16) && input[0] == '4') {
            printf("VISA\n");
        }
        else {
            printf("INVALID\n");
        }
    }
    else {
        printf("INVALID\n");
    }
}