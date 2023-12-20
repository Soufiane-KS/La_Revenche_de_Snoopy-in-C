#include <stdio.h>
#include <stdlib.h>
#include <windows.h> // for Sleep function
#include <conio.h>
#include <stdbool.h>
#define HAUTEUR 10
#define LARGEUR 20

bool GameOver = false ;
typedef struct
{
    int x;
    int y;
} Position;

typedef struct
{
    Position pos;
    int vie;
    int score;
    char v;
} Snoopy;

typedef struct
{
    Position pos;
    int vivant;
} Oiseaux;

typedef struct
{
    Position pos;
    int vivant;
    char v; //(trace de la balle)
    int dirX;
    int dirY;
} Balle;

char terrain[HAUTEUR][LARGEUR];

void ClearTerrain()
{
    for (int i = 0; i < HAUTEUR; i++)
    {
        for (int j = 0; j < LARGEUR; j++)
        {
            if (i == 0 || i == HAUTEUR - 1 || j == 0 || j == LARGEUR - 1)
            {
                terrain[i][j] = '#'; // Fill border with '#' character
            }
            else
            {
                terrain[i][j] = ' ';
            }
        }
    }
}

void MoveBall(Balle *balle)
{
    // Update the ball's position based on its current direction
    balle->pos.x += balle->dirX;
    balle->pos.y += balle->dirY;

    // Bounce off the walls
    if (balle->pos.x >= LARGEUR - 1 || balle->pos.x <= 0)
    {
        // Reverse the x-direction when hitting the left or right wall
        balle->dirX = -balle->dirX;

        // Adjust the position to stay within bounds
        balle->pos.x = (balle->pos.x >= LARGEUR - 1) ? LARGEUR - 1 : 0;
    }

    if (balle->pos.y >= HAUTEUR - 1 || balle->pos.y <= 0)
    {
        // Reverse the y-direction when hitting the top or bottom wall
        balle->dirY = -balle->dirY;

        // Adjust the position to stay within bounds
        balle->pos.y = (balle->pos.y >= HAUTEUR - 1) ? HAUTEUR - 1 : 0;
    }
}

void Setup(Balle *balle, Snoopy *snoopy)
{
    // Initialize terrain with border
    ClearTerrain();

    // Initialize ball
    balle->pos.x = 1; // start from the top left
    balle->pos.y = 1;
    balle->vivant = 1;
    balle->v = 'O';  // Ball trace character
    balle->dirX = 1; // initial x-direction
    balle->dirY = 1; // initial y-direction
    // initialise snoopy
    snoopy->pos.y = HAUTEUR / 2;
    snoopy->pos.x = LARGEUR / 2;
    snoopy->score = 0;
    snoopy->vie = 3;
    snoopy->v = 'S';
}

void Draw(Balle *balle, Snoopy *snoopy)
{
    system("cls"); // Clear the console in Windows
    ClearTerrain();
    for (int i = 0; i < HAUTEUR; i++)
    {
        for (int j = 0; j < LARGEUR; j++)
        {
            terrain[snoopy->pos.y][snoopy->pos.x] = snoopy->v;
            if (i == balle->pos.y && j == balle->pos.x)
            {
                printf("%c", balle->v); // Display the ball
            }
            else
            {
                printf("%c", terrain[i][j]);
            }
        }
        printf("\n");
    }
}

void MoveSnoopy(Snoopy *snoopy)
{
    if (_kbhit())
    {
        // Move Snoopy based on input
        switch (_getch())
        {
        case 'Z': // Up
            snoopy->pos.y--;
            break;
        case 'S': // Down
            snoopy->pos.y++;
            break;
        case 'D': // Right
            snoopy->pos.x++;
            break;
        case 'Q': // Left
            snoopy->pos.x--;
            break;
        }
    }
}

void ShowGameOver()
{
    system("cls"); // Clear the console in Windows

    for (int i = 0; i < HAUTEUR; i++)
    {
        for (int j = 0; j < LARGEUR; j++)
        {
            if (i == HAUTEUR / 2 && j == LARGEUR / 2 - 5)
            {
                printf("GAME OVER\n");
                j += 8; // Skip the characters for "GAME OVER"
            }
            else
            {
                printf("%c", terrain[i][j]);
            }
        }
        printf("\n");
    }
}

void Logic(Balle *ball,Snoopy * snoopy)
{
    if(snoopy->pos.x == ball->pos.x && snoopy->pos.y == ball->pos.y){
        GameOver = true;
        ShowGameOver();
        Sleep(5000);
        ball->vivant = false;
    }
}



int main()
{
    Balle balle;
    Snoopy snoopy;

    Setup(&balle, &snoopy);

    while (balle.vivant)
    {
        // Move the ball and draw
        MoveSnoopy(&snoopy);
        MoveBall(&balle);
        Draw(&balle, &snoopy);
        Logic(&balle,&snoopy);
        Sleep(70); // Sleep for 100 milliseconds
    }
    ShowGameOver();

    return 0;
}