#include <stdio.h>
#include <stdlib.h>
#include <windows.h> // for Sleep function
#include <conio.h>
#include <stdbool.h>
#define HAUTEUR 10
#define LARGEUR 20
#define NUM_COLLECTIBLES 4 // Number of collectibles (B letters)

bool GameOver = false;
bool Paused = false;

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

typedef struct
{
    Position pos;
    int vivant;
    char v; //(collectible letter)
} Collectible;

char terrain[HAUTEUR][LARGEUR];

// Array to store collectibles
Collectible collectibles[NUM_COLLECTIBLES];

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

    // Add three blocks in the middle horizontally
    int middleRow = HAUTEUR / 2;
    int middleColumn = LARGEUR / 2;
    terrain[middleRow][middleColumn - 1] = '#';
    terrain[middleRow][middleColumn] = '#';
    terrain[middleRow][middleColumn + 1] = '#';
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
        if (balle->pos.x >= LARGEUR - 1)
        {
            balle->pos.x = LARGEUR - 1;
        }
        else
        {
            balle->pos.x = 0;
        }
    }

    if (balle->pos.y >= HAUTEUR - 1 || balle->pos.y <= 0)
    {
        // Reverse the y-direction when hitting the top or bottom wall
        balle->dirY = -balle->dirY;

        // Adjust the position to stay within bounds
        if (balle->pos.y >= HAUTEUR - 1)
        {
            balle->pos.y = HAUTEUR - 1;
        }
        else
        {
            balle->pos.y = 0;
        }
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

    // Initialise snoopy
    snoopy->pos.y = HAUTEUR / 2 + 1;
    snoopy->pos.x = LARGEUR / 2 + 1;
    snoopy->score = 0;
    snoopy->vie = 3;
    snoopy->v = 'S';

    // Initialize collectibles
    for (int i = 0; i < NUM_COLLECTIBLES; i++)
    {
        collectibles[i].vivant = 1;
        collectibles[i].v = 'B';
        // Place collectibles at random positions
        collectibles[i].pos.x = rand() % (LARGEUR - 2) + 1;
        collectibles[i].pos.y = rand() % (HAUTEUR - 2) + 1;
    }
}

void Draw(Balle *balle, Snoopy *snoopy)
{
    system("cls"); // Clear the console in Windows
    ClearTerrain();

    // Draw collectibles
    for (int i = 0; i < NUM_COLLECTIBLES; i++)
    {
        if (collectibles[i].vivant)
        {
            terrain[collectibles[i].pos.y][collectibles[i].pos.x] = collectibles[i].v;
        }
    }

    // Draw Snoopy
    terrain[snoopy->pos.y][snoopy->pos.x] = snoopy->v;

    // Draw Ball
    if (balle->vivant)
    {
        terrain[balle->pos.y][balle->pos.x] = balle->v;
    }

    // Display the entire terrain
    for (int i = 0; i < HAUTEUR; i++)
    {
        for (int j = 0; j < LARGEUR; j++)
        {
            printf("%c", terrain[i][j]);
        }
        printf("\n");
    }

    // Display Score and Lives
    printf("Score: %d\tLives: %d\n", snoopy->score, snoopy->vie);
}

void ShowPauseMenu()
{
    system("cls"); // Clear the console in Windows

    printf("##############################\n");
    printf("#                            #\n");
    printf("#         Game Paused        #\n");
    printf("#                            #\n");
    printf("#   1. Resume               #\n");
    printf("#   2. Exit                 #\n");
    printf("#                            #\n");
    printf("##############################\n");
}

void MoveSnoopy(Snoopy *snoopy)
{
    if (_kbhit())
    {
        // Store the current position in case the next move is invalid
        int prevX = snoopy->pos.x;
        int prevY = snoopy->pos.y;

        // Move Snoopy based on input
        switch (_getch())
        {
        case 'z': // Up
            snoopy->pos.y--;
            break;
        case 's': // Down
            snoopy->pos.y++;
            break;
        case 'd': // Right
            snoopy->pos.x++;
            break;
        case 'q': // Left
            snoopy->pos.x--;
            break;
        case 'p': // Pause
            Paused = true;
            while (Paused)
            {
                ShowPauseMenu();
                char choice = _getch();
                switch (choice)
                {
                case '1':
                    Paused = false;
                    break;
                case '2':
                    GameOver = true;
                    exit(0);
                    break;
                }
            }
            break;
        }

        // Check if the next position is valid (not on a wall)
        if (terrain[snoopy->pos.y][snoopy->pos.x] == '#')
        {
            // If the next position is a wall, restore the previous position
            snoopy->pos.x = prevX;
            snoopy->pos.y = prevY;
        }
    }
}

void ShowGameOver(Snoopy *snoopy)
{
    system("cls"); // Clear the console in Windows

    printf("##############################\n");
    printf("#                            #\n");
    printf("#          GAME OVER         #\n");
    printf("#                            #\n");
    printf("##############################\n");
    printf("\n");
    printf("Score: %d\tLives Left: %d\n", snoopy->score, snoopy->vie);
    Sleep(5000);
}

void ShowYouWon(Snoopy *snoopy)
{
    system("cls"); // Clear the console in Windows

    printf("##############################\n");
    printf("#                            #\n");
    printf("#         YOU WON!           #\n");
    printf("#                            #\n");
    printf("##############################\n");
    printf("\n");
    printf("Score: %d\tLives Left: %d\n", snoopy->score, snoopy->vie);
}


void Logic(Balle *ball, Snoopy *snoopy)
{
    // Check if Snoopy ate a collectible
    for (int i = 0; i < NUM_COLLECTIBLES; i++)
    {
        if (collectibles[i].vivant && snoopy->pos.x == collectibles[i].pos.x && snoopy->pos.y == collectibles[i].pos.y)
        {
            snoopy->score += 100;
            collectibles[i].vivant = 0; // Mark the collectible as eaten
        }
    }

    // Check if Snoopy collided with the ball
    if (snoopy->pos.x == ball->pos.x && snoopy->pos.y == ball->pos.y)
    {
        snoopy->vie--;
        if (snoopy->vie <= 0)
        {
            GameOver = true;
            ShowGameOver(&snoopy);
            Sleep(5000);
            ball->vivant = false;
            return; // Exit the function if the game is over
        }
        else
        {
            snoopy->pos.x = LARGEUR / 2 + 1;
            snoopy->pos.y = HAUTEUR / 2 + 1;
        }
    }

    // Check if Snoopy won
    bool allCollected = true;
    for (int i = 0; i < NUM_COLLECTIBLES; i++)
    {
        if (collectibles[i].vivant)
        {
            allCollected = false;
            break;
        }
    }

    if (allCollected)
    {
        ShowYouWon(snoopy);
        Sleep(5000);
        ball->vivant = false;
    }
}



void MainMenu()
{
    system("cls"); // Clear the console in Windows

    printf("=============================================\n");
    printf("      WELCOME TO LA REVANCHE DE SNOOPY          \n");
    printf("=============================================\n\n");
    printf("           Press any key to start...         \n");

    _getch(); // Wait for any key press
}

int main()
{
    Balle balle;
    Snoopy snoopy;

    MainMenu();
    Setup(&balle, &snoopy);

    while (balle.vivant && !GameOver)
    {
        // Move the ball and draw
        MoveSnoopy(&snoopy);
        MoveBall(&balle);
        Draw(&balle, &snoopy);
        Logic(&balle, &snoopy);
        Sleep(100); // Sleep for 100 milliseconds
    }

    if (!GameOver)
    {
        ShowYouWon(&snoopy);
        exit(0);
    }
    else
    {
        ShowGameOver(&snoopy);
    }

    return 0;
}