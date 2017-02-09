#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("Shower Time (minutes): ");
    int minutes = get_int();
    printf("Bottles of Water Used: %i\n", 12 * minutes);
}