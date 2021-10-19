#include <stdio.h>
#include <stdlib.h>
#include "mineSweeper.h"
int main() 
{
    int i;
    char c, cmd[10];
    do 
    {
        for(i = 0; i < MAX; i++)
            display[i] = ' ';
        system("clear");
        printf("\nDEADLY MINEFIELD\n(a. k. a. minesweeper)\n\n");
        printf("You are trapped in a dungeon filled with hidden mines ready to detonate. Find them all!\n");
        printf("\nEnter ? to know the game\nEnter x to exit\nEnter anything else to play\n");
        fseek(stdin, 0, 0);
        while((c = getchar()) == ' ' || c == '\t');
        if(c == 'x')
        {
            while((c = getchar()) == ' ' || c == '\t');
            if(c == '\n')
                return 0;
        }
        else if(c == '?')
        {
            while((c = getchar()) == ' ' || c == '\t');
            if(c == '\n')
            {
                printf("\nHOW TO PLAY\n");
                printf("\nThe numbers in the boxed postions show the number of mines around that position.\n");
                printf("\nTo select a postion, enter the row and column separated by a comma, like\n");
                printf("<row_number> , <column_number>\ne.g. 2,3\n");
               
                printf("\nYou can select multiple positions by separating each position with a '+' sign, like\n");
                printf("2,3 + 4,5\n");

                printf("\nIf the selected position doesn't have a mine, then you will see\n");
                printf("1. The number of mines around the selected position\n");
                printf("2. The number of mines around the adjacent positions, if they don't contain mines\n");
                printf("\nIf you want to mark a place that you think has a mine, prefix the position with an 'f'.\n");
                printf("e.g. f2,3\n");
                printf("\nYou can flag multiple positions like\n");
                printf("f2,3 + 4,5\nThis will flag both (2,3) and (4,5)\n");
                printf("\nDon't worry about spacing, just be mindful of the order.\n");
                
                printf("\nIf you uncover all positions without selecting a mine and mark all the mines, you win.\n\n");
                goto END;
            }
        }
        else
        {
            //GAME BEGINS
            placemines();
            int flag = 0;
            do
            {
                board();
                do
                {
                    flag = 0;
                    printf("\n--> ");
                    fseek(stdin, 0, 0);
                    for(i = 0; i < 10; i++)
                    {
                        while((c = getchar()) == ' ' || c == '\t');
                        if(c == '\n')
                        { 
                            cmd[i] = '\0';
                            if((i = error(cmd, flag)) == 1)
                            {
                                printf("\nInvalid command.\n");
                                break;
                            }
                            else if(i == -1)
                                goto END;
                            if(update(i, flag) == -2)
                                goto END;
                            break;
                        }
                        cmd[i] = c; 
                        if(c == '+')
                        {
                            if((i = error(cmd, flag)) == 1)
                            {
                                printf("\nInvalid command.\n");
                                break;
                            }
                            else if(i == -1)
                                goto END;
                            else if(i == FLAG)
                                flag = 1;
                            if(update(i, flag) == -2)
                                goto END;
                            i = -1;
                        } 
                    }
                } while(i == 1);
                if(freeplaces == 0)
                {
                    printf("\nYou lived to see another day!\n");
                    goto END;
                }
            } while(1);
        }
        END:
        fseek(stdin, 0, 0);
        getchar();
    } while(1);
    return 0;
}
