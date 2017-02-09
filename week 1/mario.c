#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("Height: ");
    int height = get_int();
    
    for(int i = 1; i < height + 1; i++) {
        for(int j = 0; j < height - i; j++) {
            printf("%c", ' ');
        }
        for(int j = 0; j < i; j++) {
            printf("#");
        }
        printf("%c", ' ');
        for(int j = 0; j < i; j++) {
            printf("#");
        }
        printf("\n");
    }
}