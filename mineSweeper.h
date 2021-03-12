#include <stdio.h>
#define length 9
#define MAX length * length + 1
#define FLAG 7

unsigned int seed;
int mines;
int freeplaces = MAX;
char playspace[MAX];
char display[MAX];
int pos;

//RANDOMLY PLACES MINES
void placemines()
{
    void correct(unsigned int *);
    unsigned int temp;
    int i, j;
    fseek(stdin, 0, 0);
    printf("\nEnter seed: ");
    scanf("%u", &seed);
    temp = seed;
    for(i = 0, j = 0; i < MAX - 1; i++)
    {
        temp *= temp + temp / 7;
        correct(&temp);
        if(temp % 17 < 2)
        {
            playspace[j] = '1';
            mines++;
        }
        else
            playspace[j] = '0';
        j++;
         
    }
    playspace[j] = '\0';
}
void correct(unsigned int *n)
{
    int i, j;
    unsigned int temp;
    temp = *n;
    for(i = 0; temp > 0; i++)
    {
        temp /= 10;
    }

    if(i > 4)
        for(j = 0; j < i - 4; j++)
            *n /= 10;
    else
        for(j = 0; j < 4 - i; j++)
        {
            *n = *n * 13;
        }
}

//COUNTS ADJACENT MINES
int minecounter(int p, int iterations)
{
    int i;
    int minecount = 0;
    int temp = 0, u = 0; 
    for(i = 0; i < iterations; i++)
    {
        switch(i)
        {
            case 0:
                if(playspace[p - 1] == '1' && p % length != 0)
                    minecount++;
                break;
            case 1:
                if(playspace[p + 1] == '1' && (p + 1) % length != 0)
                    minecount++;
                break;
            case 2:
                if(p + length < MAX && playspace[p + length] == '1')
                    minecount++;
                temp += minecounter(p + length, 2);
                break;
            case 3:
                if(p - length > -1 && playspace[p - length] == '1')
                    minecount++;
                temp += minecounter(p - length, 2);
                break;
        }
    }
    return minecount + temp;
}

//SHOWS THE INTERFACE
void board()
{
    int i, j;
    printf("\n    ");
    for(i = 0; i < length ; i++)
        printf(" %d  ", i + 1);
    for(i = 0; i < MAX - 1; i++)
    {
        if(i % length == 0)
        {
            printf("\n    ");
            if(i != 0)
                for(j = 0; j < length; j++)
                {
                    if(j < length - 1)
                        printf(" - +");
                    else 
                        printf(" - ");
                }
            printf("\n%d   ", i / length + 1);
        }
        if((i + 1) % length == 0)
            printf(" %c ", display[i]);
        else
            printf(" %c |", display[i]);
    }
    printf("\n");
}

//CHECKS FOR AN ERROR IN THE COMMANDS
int error(char *cmd, int flag)
{
    int row, col, i = 0;
    int rval = 0;
    if(cmd[0] == 'x' && cmd[1] == '\n')
        return -1;
    if(cmd[0] == 'f' || flag == 1)
    {
        rval = FLAG;
        if(cmd[0] == 'f')
            i = 1;
    }
//-------------------------
    //Reading the position
    if(cmd[i] < '0' || cmd[i] > '9')
        return 1;
    else
        row = cmd[i] - '0';
    if(cmd[i + 1] != ',')
        return 1;
    if(cmd[i + 2] < '0' || cmd[2] > '9')
        return 1;
    else
        col = cmd[i + 2] - '0';
//-------------------------
    if(cmd[i + 3] != '\0' && cmd[i + 3] != '+')
        return 1;
//-------------------------
    if(row < 1 || row > length)
        return 1;
    if(col < 1|| col > length)
        return 1;
    pos = length * row - (length - col) - 1;
    return rval;
}

//ADJACENT FILLER
void adjacentfiller(int p, int x, int hcall, int vcall)
{
    if(p < 0 || p > MAX - 1)
        return;
    int i, j;
    if((j = minecounter(p, 5)) != 0 && x > 1)
    {
        if(display[p] == ' ')
            --freeplaces;
        display[p] = j + '0';
        return;
    }
    if(display[p] == ' ')
        --freeplaces;
    display[p] = j + '0'; 
    for(i = 0; i < x; i++)
    {
        switch(i)
        {
            case 0:
                if(p % length != 0 && playspace[p - 1] == '0' && hcall != 2)
                {
                    if((j = minecounter(p - 1, 5)) != 0)
                    {
                        if(display[p - 1] == ' ')
                            --freeplaces;
                        display[p - 1] = j + '0';
                    }
                    else if(x > 1)
                        adjacentfiller(p - 1, 2, 1, 0);
                }
                if((p + 1) % length != 0 && playspace[p + 1] == '0' && hcall != 1)
                {
                    if((j = minecounter(p + 1, 5)) != 0)
                    {
                        if(display[p + 1] == ' ')
                            --freeplaces;
                        display[p + 1] = j + '0';
                    }
                    else if(x > 1)
                        adjacentfiller(p + 1, 2, 2, 0);
                }  
                break; 
            case 1:
                if((p - length) > -1 && playspace[p - length] == '0' && vcall != 2)
                {
                    if((j = minecounter(p - length, 5)) != 0)
                    {
                        if(display[p - length] == ' ')
                            --freeplaces;
                        display[p - length] = j + '0';
                    }
                    else
                        adjacentfiller(p - length, 2, hcall, 1);
                    adjacentfiller(p - length, 1, 0, 0);
                } 
                if((p + length) < MAX && playspace[p + length] == '0' && vcall != 1)
                {
                    if((j = minecounter(p + length, 5)) != 0)
                    {
                        if(display[p + length] == ' ')
                            --freeplaces;
                        display[p + length] = j + '0';
                    }
                    else
                        adjacentfiller(p + length, 2, hcall, 2);
                    adjacentfiller(p + length, 1, 0, 0);
                } 
        }
    }
}

//WHEN YOU GO TO A MINE AND DON'T FLAG IT, THIS IS WHAT YOU SEE
void explode()
{
    int i = 0;
    for(i = 0; i < MAX; i++)
        if(playspace[i] == '1')
            display[i] = 'X';
    board();
}

//BOARD UPDATER
int update(int i, int flag)
{
    int k, j, p;
    if(i == FLAG || flag == 1)
    {
        if(display[pos] == ' ')
        {
            display[pos] = 'F';
            freeplaces--;
            if(playspace[pos] == '1')
                mines--;
        }
        else if(display[pos] == 'F')
        {
            freeplaces++;
            mines++;
            display[pos] = ' ';
        }
    }
    else
    {
        if(playspace[pos] == '0' && display[pos] == ' ')
            adjacentfiller(pos, 3, 0, 0);
        else if(display[pos] != ' ')
            printf("\nYou've already covered position (%d, %d).\n", (pos + length) / length, (pos + 1) % length);
        else
        {
            explode();
            printf("\nYou were blown to smithereens. R. I. P.\n\n");
            return -2;
        }
    }
    return 0;
}