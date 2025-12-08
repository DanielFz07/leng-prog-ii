#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
    #define KEY_UP 72
    #define KEY_DOWN 80
    #define KEY_LEFT 75
    #define KEY_RIGHT 77
    #define KEY_ENTER 13
#else
    #include <ncurses.h>
#endif

#include <locale.h>
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

int main(); //Prototipo

struct Pos 
{
    int x, y;
    bool operator==(const Pos& other) const { return x == other.x && y == other.y; }
};

struct Player 
{
    string name;
    int score = 0;
    int maxScore = 0;
    int level = 1;
};

enum Dir { UP, DOWN, LEFT, RIGHT };

std::vector<Pos> snake;  // Lista de posiciones: snake[0] es cabeza
std::vector<Pos> obstacles; // Vector para almacenar los obstáculos
std::vector<Player> players; // Vector para almacenar los nombres de los jugadores
Dir dir = RIGHT;         // Dirección inicial
Pos food;                // Posición de comida
bool gameOver = false;   // Flag de fin

// Variables globales para niveles
int selectedPlayer = 0; // Índice del jugador seleccionado
const int pointsPerLevel = 5; // Puntos necesarios para avanzar de nivel
const int maxLevel = 2; // Nivel máximo

// Variable global para controlar la velocidad de la serpiente
int snakeSpeed = 200; // Velocidad inicial en milisegundos

const int anchoTablero=30;
const int altoTablero=15;

void gotoxy(int x, int y) 
{
    #ifdef _WIN32
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    #else
        move(y, x);
    #endif
}

void HideCursor()
{
    #ifdef _WIN32
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = FALSE;
        SetConsoleCursorInfo(consoleHandle, &info);
    #else
        curs_set(0);
    #endif
}

void clrscr() 
{
    #ifdef _WIN32
        system("cls");
    #else
        clear();
    #endif
}

void await(int milliseconds) 
{
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        napms(milliseconds);
    #endif
}

void DrawConsole(int x, int y, const string& art, bool refreshScreen = false) 
{
    #ifdef _WIN32
        gotoxy(x, y);
        printf("%s", art.c_str());
        fflush(stdout);
    #else
        mvaddstr(y, x, art.c_str());
        if(refreshScreen) refresh();
    #endif
}

void DrawSnake()
{
    //Limpiar el interior del tablero cada vez que se dibuje la serpiente
    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD written;
        for(int i = 1; i <= altoTablero; i++) 
        {
            COORD coord = {1, (SHORT)i};
            FillConsoleOutputCharacter(hConsole, ' ', anchoTablero, coord, &written);
        }
    #else
        for (int colums = 1; colums < anchoTablero + 1; colums++) 
        {
            for (int rows = 1; rows < altoTablero + 1; rows++) 
            {
                DrawConsole(colums, rows, " ");
            }
        }
    #endif

    string head = "▲"; // Cabeza de la serpiente
    switch(dir)
    {
        case DOWN:
        {
            head = "▼";
            break;
        }
        case LEFT:
        {
            head = "◄";
            break;
        }
        case RIGHT:
        {
            head = "►";
            break;
        }
    }

    for (size_t i = 0; i < snake.size(); ++i) 
    {
        DrawConsole(snake[i].x, snake[i].y, (i == 0 ? head : "█"));
    }
}

void DrawTable()
{
    // Dibujar bordes una vez al inicio del juego
    clrscr();
    for (int colums = 0; colums < anchoTablero + 2; colums++) 
    {
        for (int rows = 0; rows < altoTablero + 2; rows++) 
        {
            if (colums == 0 || colums == anchoTablero + 1 || rows == 0 || rows == altoTablero + 1) 
            {
                /*if (colums == 0 && rows == 0) DrawConsole(colums, rows, "┌");
                else if (colums == anchoTablero + 1 && rows == 1) DrawConsole(colums, rows, "┐");
                else if (colums == 0 && rows == altoTablero + 1) DrawConsole(colums, rows, "└");
                else if (colums == anchoTablero + 1 && rows == altoTablero + 1) DrawConsole(colums, rows, "┘");*/
                if (rows == 0) DrawConsole(colums, rows, "▁");
                else if( rows == altoTablero + 1) DrawConsole(colums, rows, "▔");
                else DrawConsole(colums, rows, "┃");
            }
        }
    }
    // Mostrar datos del jugador
    DrawConsole(anchoTablero+5, 0, "Nivel: " + to_string(players[selectedPlayer].level));
    DrawConsole(anchoTablero+5, 1, "Jugador: " + players[selectedPlayer].name);
    DrawConsole(anchoTablero+5, 2, "Puntos: " + to_string(players[selectedPlayer].score), true);

    DrawConsole (anchoTablero+5, 4, "Controles:", true);
    DrawConsole (anchoTablero+5, 5, "W/A/S/D o", true);
    DrawConsole (anchoTablero+5, 6, "Flechas", true);

    #ifndef _WIN32
        refresh();
    #endif
}

void generateFood() 
{
    do 
    {
        food.x = rand() % anchoTablero;
        food.y = rand() % altoTablero;

        if(food.x == 0) food.x = 1;
        if(food.y == 0) food.y = 1;
    } 
    while (std::find(snake.begin(), snake.end(), food) != snake.end()); // No en serpiente
}

void generateObstacles(int level) 
{
    obstacles.clear(); // Limpiar obstáculos previos
    int numObstacles = level; // Número de obstáculos basado en el nivel

    for (int i = 0; i < numObstacles; ++i) 
    {
        Pos obstacle;
        do 
        {
            obstacle.x = rand() % anchoTablero;
            obstacle.y = rand() % altoTablero;

            if (obstacle.x == 0) obstacle.x = 1;
            if (obstacle.y == 0) obstacle.y = 1;
        } 
        while (std::find(snake.begin(), snake.end(), obstacle) != snake.end() || obstacle == food || std::find(obstacles.begin(), obstacles.end(), obstacle) != obstacles.end());

        obstacles.push_back(obstacle);
    }
}

void MoveSnake() 
{
    Pos newHead = snake[0];
    if (dir == UP) newHead.y--;
    else if (dir == DOWN) newHead.y++;
    else if (dir == LEFT) newHead.x--;
    else if (dir == RIGHT) newHead.x++;

    // Verificar colisiones
    if (newHead.x < 1 || newHead.x > anchoTablero || newHead.y < 1 || newHead.y > altoTablero || std::find(snake.begin(), snake.end(), newHead) != snake.end()) 
    {
        gameOver = true;
        return;
    }

    // Verificar colisión con obstáculos
    if (std::find(obstacles.begin(), obstacles.end(), newHead) != obstacles.end()) 
    {
        gameOver = true;
        return;
    }

    snake.insert(snake.begin(), newHead); // Agregar nueva cabeza

    // Si come comida, crecer (no remover cola); sino, remover cola
    if (newHead == food || (newHead.x == food.x + 1 && newHead.y == food.y)) 
    {
        players[selectedPlayer].score++;
        DrawConsole(anchoTablero+5, 2, "Puntos: " + to_string(players[selectedPlayer].score), true);
        generateFood();
    } 
    else 
    {
        snake.pop_back();
    }
}

void HandleInput() 
{
    #ifdef _WIN32
        if (_kbhit()) 
        {
            char key = getch();
            if((key == 'w' || key == KEY_UP) && dir != DOWN) dir = UP;
            else if((key == 's' || key == KEY_DOWN) && dir != UP) dir = DOWN;
            else if((key == 'a' || key == KEY_LEFT) && dir != RIGHT) dir = LEFT;
            else if((key == 'd' || key == KEY_RIGHT) && dir != LEFT) dir = RIGHT;
        }
    #else
        nodelay(stdscr, TRUE);
        int key = getch();
        if (key != ERR) 
        {
            if((key == 'w' || key == KEY_UP) && dir != DOWN) dir = UP;
            else if((key == 's' || key == KEY_DOWN) && dir != UP) dir = DOWN;
            else if((key == 'a' || key == KEY_LEFT) && dir != RIGHT) dir = LEFT;
            else if((key == 'd' || key == KEY_RIGHT) && dir != LEFT) dir = RIGHT;
        }
    #endif
}

void DrawLetterY(int startRow, int startCol, int size) 
{
    for (int i = 0; i < size; ++i) 
    {
        for (int j = 0; j < size; ++j) 
        {
            if ((j == size / 2 && i >= size / 2) || (i == j && i < size / 2) || (i + j == size - 1 && i < size / 2)) 
            {
                DrawConsole(startCol + j, startRow + i, "█");
            }
        }
    }
}

void DrawLetterO(int startRow, int startCol, int size) 
{
    for (int i = 0; i < size; ++i) 
    {
        for (int j = 0; j < size; ++j) 
        {
            if (i == 0 || i == size - 1 || j == 0 || j == size - 1) 
            {
                DrawConsole(startCol + j, startRow + i, "█");
            }
        }
    }
}

void DrawLetterU(int startRow, int startCol, int size) 
{
    for (int i = 0; i < size; ++i) 
    {
        for (int j = 0; j < size; ++j) 
        {
            if (j == 0 || j == size - 1 || i == size - 1) 
            {
                DrawConsole(startCol + j, startRow + i, "█");
            }
        }
    }
}

void DrawLetterL(int startRow, int startCol, int size) 
{
    for (int i = 0; i < size; ++i) 
    {
        for (int j = 0; j < size; ++j) 
        {
            if (j == 0 || (i == size - 1))
            {
                DrawConsole(startCol + j, startRow + i, "█");
            }
        }
    }
}

void DrawLetterS(int startRow, int startCol, int size) 
{
    for (int i = 0; i < size; ++i) 
    {
        for (int j = 0; j < size; ++j) 
        {
            if (i == 0 || i == size - 1 || (i == size / 2) || (j == 0 && i < size / 2) || (j == size - 1 && i > size / 2)) 
            {
                DrawConsole(startCol + j, startRow + i, "█");
            }
        }
    }
}

void DrawLetterN(int startRow, int startCol, int size) 
{
    for (int i = 0; i < size; ++i) 
    {
        for (int j = 0; j < size; ++j) 
        {
            if (j == 0 || j == size - 1 || i == j) 
            {
                DrawConsole(startCol + j, startRow + i, "█");
            }
        }
    }
}

void DrawLetterI(int startRow, int startCol, int size) 
{
    for (int i = 0; i < size; ++i) 
    {
        for (int j = 0; j < size; ++j) 
        {
            if (i == 0 || i == size - 1 || j == size / 2) 
            {
                DrawConsole(startCol + j, startRow + i, "█");
            }
        }
    }
}

void DrawLetterV(int startRow, int startCol, int size) 
{
    for (int i = 0; i < size; ++i) 
    {
        for (int j = 0; j < size; ++j) 
        {
            if ((j == 0 && i < size - 1) || (j == size - 1 && i < size - 1) || (i == size - 1 && j > 0 && j < size - 1)) 
            {
                DrawConsole(startCol + j, startRow + i, "█");
            }
        }
    }
}

void DrawLetterE(int startRow, int startCol, int size) 
{
    for (int i = 0; i < size; ++i) 
    {
        for (int j = 0; j < size; ++j) 
        {
            if (j == 0 || i == 0 || i == size - 1 || i == size / 2) 
            {
                DrawConsole(startCol + j, startRow + i, "█");
            }
        }
    }
}

void DrawExclamation(int startRow, int startCol, int size) 
{
    for (int i = 0; i < size; ++i) 
    {
        if (i < size - 2 || i == size - 1) 
        {
            DrawConsole(startCol, startRow + i, "█");
        } 
        else 
        {
            DrawConsole(startCol, startRow + i, " ");
        }
    }
}

void DrawLetterW(int startRow, int startCol, int size) 
{
    for (int i = 0; i < size; ++i) 
    {
        for (int j = 0; j < size; ++j) 
        {
            //Letra W
            if (j == 0 || j == size - 1 || (i >= size / 2 && (j == i - size / 2 || j == size - 1 - (i - size / 2)))) 
            {
                DrawConsole(startCol + j, startRow + i, "█");
            }
        }
    }
}

void DrawNumber(int number, int startRow, int startCol, int size) 
{
    for (int i = 0; i < size; ++i) 
    {
        for (int j = 0; j < size; ++j) 
        {
            if (number == 0 && (i == 0 || i == size - 1 || j == 0 || j == size - 1)) 
            {
                DrawConsole(startCol + j, startRow + i, "█");
            }
            else if (number == 1 && j == size / 2) 
            {
                DrawConsole(startCol + j, startRow + i, "█");
            }
            else if (number == 2 && (i == 0 || i == size - 1 || i == size / 2 || (j == size - 1 && i < size / 2) || (j == 0 && i > size / 2))) 
            {
                DrawConsole(startCol + j, startRow + i, "█");
            }
            else if (number == 3 && (i == 0 || i == size - 1 || i == size / 2 || j == size - 1)) 
            {
                DrawConsole(startCol + j, startRow + i, "█");
            }
            else if (number == 4 && (j == size - 1 || j == 0 && i < size / 2 || i == size / 2)) 
            {
                DrawConsole(startCol + j, startRow + i, "█");
            }
            else if (number == 5 && (i == 0 || i == size - 1 || i == size / 2 || (j == 0 && i < size / 2) || (j == size - 1 && i > size / 2))) 
            {
                DrawConsole(startCol + j, startRow + i, "█");
            }
            else if (number == 6 && (i == 0 || i == size - 1 || i == size / 2 || j == 0 || (j == size - 1 && i > size / 2))) 
            {
                DrawConsole(startCol + j, startRow + i, "█");
            }
            else if (number == 7 && (i == 0 || j == size - 1)) 
            {
                DrawConsole(startCol + j, startRow + i, "█");
            }
            else if (number == 8 && (i == 0 || i == size - 1 || i == size / 2 || j == 0 || j == size - 1)) 
            {
                DrawConsole(startCol + j, startRow + i, "█");
            }
            else if (number == 9 && (i == 0 || i == size - 1 || i == size / 2 || j == size - 1 || (j == 0 && i < size / 2))) 
            {
                DrawConsole(startCol + j, startRow + i, "█");
            }
        }
    }
}

void SetConsoleColorRed() 
{
    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    #else
        attron(COLOR_PAIR(1));
    #endif
}

void SetConsoleColorGreen() 
{
    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    #else
        attron(COLOR_PAIR(2));
    #endif
}

void SetConsoleColorYellow() 
{
    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    #else
        attron(COLOR_PAIR(3));
    #endif
}

void ResetConsoleColor() 
{
    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    #else
        attroff(COLOR_PAIR(1));
    #endif
}

bool DisplayPlayerMenu()
{
    clrscr();

    if (players.empty()) 
    {
        DrawConsole(10, 5, "Nuevo jugador: ", true);

        string playerName;
        #ifdef _WIN32
            getline(cin, playerName);
        #else
            echo(); // Habilitar eco para entrada de texto
            char nameBuffer[50];
            getnstr(nameBuffer, 50);
            playerName = string(nameBuffer);
            noecho(); // Deshabilitar eco despues de la entrada
        #endif

        players.push_back({playerName, 0}); // Agregar jugador a la lista
    } 
    else 
    {
        bool playerSelected = false;

        while (!playerSelected) 
        {
            clrscr();
            DrawConsole(10, 5, "Selecciona un jugador:");

            for (size_t i = 0; i < players.size(); ++i) 
            {
                DrawConsole(10, 7 + i, (i == selectedPlayer ? "> " : "  ") + players[i].name + " (Puntos: " + to_string(players[i].score) + ")");
            }
            DrawConsole(10, 7 + players.size(), (selectedPlayer == players.size() ? "> Nuevo jugador" : "  Nuevo jugador"));

            int key = 0;
            #ifdef _WIN32
                key = _getch();
                if (key == 0 || key == 224) key = _getch();
            #else
                nodelay(stdscr, FALSE);
                key = getch();
            #endif

            if (key == 'w' || key == KEY_UP) 
            {
                selectedPlayer = (selectedPlayer - 1 + players.size() + 1) % (players.size() + 1);
            } 
            else if (key == 's' || key == KEY_DOWN) 
            {
                selectedPlayer = (selectedPlayer + 1) % (players.size() + 1);
            } 
            else if (key == '\n' || key == KEY_ENTER || key == 10) 
            {
                if (selectedPlayer == players.size()) 
                {
                    DrawConsole(10, 7 + players.size() + 2, "Nuevo jugador: ", true);

                    string playerName;
                    #ifdef _WIN32
                        getline(cin, playerName);
                    #else
                        echo(); // Habilitar eco para entrada de texto
                        char nameBuffer[50];
                        getnstr(nameBuffer, 50);
                        playerName = string(nameBuffer);
                        noecho(); // Deshabilitar eco despues de la entrada
                    #endif

                    players.push_back({playerName, 0}); // Agregar jugador a la lista
                }
                playerSelected = true;
            }
        }
    }

    return true;
}

void SortPlayers() 
{
    for (size_t i = 0; i < players.size() - 1; ++i) 
    {
        size_t maxIndex = i;
        for (size_t j = i + 1; j < players.size(); ++j) 
        {
            // Comparar puntaje
            if (players[j].score > players[maxIndex].score) 
            {
                maxIndex = j;
            } 
            else if (players[j].score == players[maxIndex].score) 
            {
                // Comparar nombres alfabeticamente
                if (players[j].name < players[maxIndex].name) 
                {
                    maxIndex = j;
                } 
                else if (players[j].name == players[maxIndex].name) 
                {
                    // Comparar longitud del nombre
                    if (players[j].name.size() < players[maxIndex].name.size()) 
                    {
                        maxIndex = j;
                    }
                }
            }
        }
        // Intercambiar jugadores
        if (maxIndex != i) 
        {
            Player temp = players[i];
            players[i] = players[maxIndex];
            players[maxIndex] = temp;
        }
    }
}

void DisplayMenu() 
{
    int option = 0;
    bool exitMenu = false;

    while (!exitMenu) 
    {
        clrscr();
        DrawConsole(10, 5, "=== MENU ===");
        DrawConsole(10, 7, (option == 0 ? "> Jugar" : "  Jugar"));
        DrawConsole(10, 8, (option == 1 ? "> TOP Jugadores" : "  TOP Jugadores"));
        DrawConsole(10, 9, (option == 2 ? "> Salir" : "  Salir"));
        int key = 0;
        #ifdef _WIN32
            key = _getch();
            if (key == 0 || key == 224) key = _getch();
        #else
            nodelay(stdscr, FALSE);
            key = getch();
        #endif

        if (key == 'w' || key == KEY_UP) 
        {
            option = (option - 1 + 3) % 3;
        } 
        else if (key == 's' || key == KEY_DOWN) 
        {
            option = (option + 1) % 3;
        } 
        else if (key == '\n' || key == KEY_ENTER || key == 10) 
        {
            switch (option) 
            {
                case 0: //Jugar
                {
                    exitMenu = DisplayPlayerMenu();
                    break;
                }
                case 1: // TOP Jugadores
                {
                    clrscr();
                    
                    if (players.empty()) 
                    {
                        DrawConsole(8, 6, "No hay jugadores registrados.", true);
                    } 
                    else 
                    {
                        SortPlayers(); // Ordenar jugadores
                        DrawConsole(7, 6, "TOP Jugadores:", true);
                        DrawConsole(7, 7, "Nombre               Puntos   Maximo", true);
                        for (size_t i = 0; i < players.size(); ++i) 
                        {
                            string line = to_string(i + 1) + ". " + players[i].name;
                            while(line.length() < 21) line += " ";
                            
                            string score = to_string(players[i].score);
                            while(score.length() < 9) score += " ";
                            
                            string maxScore = to_string(players[i].maxScore);
                            
                            DrawConsole(7, 8 + i, line + score + maxScore, true);
                        }
                    }
                    DrawConsole(7, 8 + players.size() + 1, "Presiona cualquier tecla para volver al menu.", true);
                    #ifdef _WIN32
                        _getch();
                    #else
                        getch();
                    #endif
                    break;
                }
                case 2: exit(0); // Salir
            }
        }
    }
}

void DisplayGameOverArt() 
{
    int size = 4;
    while (size <= 8) 
    {
        clrscr();

        int startRow = (altoTablero) / 2;
        int startCol = (anchoTablero) / 2;

        if (size == 6) 
        {
            SetConsoleColorRed();
        } 

        DrawLetterY(startRow, startCol - 2, size);
        DrawLetterO(startRow, startCol + size - 1, size);
        DrawLetterU(startRow, startCol + 2 * (size), size);
        DrawLetterL(startRow, startCol + 3 * (size + 1), size);
        DrawLetterO(startRow, startCol + 4 * (size + 1), size);
        DrawLetterO(startRow, startCol + 5 * (size + 1), size);
        DrawLetterS(startRow, startCol + 6 * (size + 1), size);
        DrawLetterE(startRow, startCol + 7 * (size + 1), size);
        DrawExclamation(startRow, startCol + 8 * (size + 1), size);


        #ifndef _WIN32
            refresh();
        #endif

        await(2000); // 2 segundos

        size += 2;

        ResetConsoleColor();
    }
}

void DisplayWinArt() 
{
    int size = 5;
    for (int i = 0; i < 3; ++i) // Mostrar por 3 segundos
    {
        clrscr();

        int startRow = (altoTablero) / 2;
        int startCol = (anchoTablero) / 2;

        if (i % 2 == 0) 
        {
            SetConsoleColorYellow();
        } 
        else 
        {
            ResetConsoleColor(); // Blanco
        }

        DrawLetterY(startRow, startCol - 2, size);
        DrawLetterO(startRow, startCol + size - 1, size);
        DrawLetterU(startRow, startCol + 2 * (size), size);
        DrawLetterW(startRow, startCol + 3 * (size + 1), size);
        DrawLetterI(startRow, startCol + 4 * (size + 1), size);
        DrawLetterN(startRow, startCol + 5 * (size + 1), size);
        DrawExclamation(startRow, startCol + 6 * (size + 1), size);

        #ifndef _WIN32
            refresh();
        #endif

        await(1000); // 1 segundo
    }
    ResetConsoleColor();

    main(); //Reiniciamos el juego sin salir del programa
}

void DisplayLevelUpMessage(int level) 
{
    int size = 5;
    for (int i = 0; i < 3; ++i) // Mostrar por 3 segundos
    {
        clrscr();

        int startRow = (altoTablero) / 2;
        int startCol = (anchoTablero) / 2;

        if (i % 2 == 0) 
        {
            SetConsoleColorGreen();
        } 
        else 
        {
            ResetConsoleColor(); // Blanco
        }

        DrawLetterN(startRow, startCol, size);
        DrawLetterI(startRow, startCol + size + 1, size);
        DrawLetterV(startRow, startCol + 2 * (size + 1), size);
        DrawLetterE(startRow, startCol + 3 * (size + 1), size);
        DrawLetterL(startRow, startCol + 4 * (size + 1), size);
        DrawNumber(level, startRow, startCol + 5 * (size + 1), size);

        #ifndef _WIN32
            refresh();
        #endif

        await(1000); // 1 segundo
    }
    ResetConsoleColor();
}

int main()
{
    srand(time(0)); // Semilla para rand

    #ifdef _WIN32
        SetConsoleOutputCP(65001); // UTF-8 para Windows
    #else
        setlocale(LC_ALL, ""); // UTF-8 para ncurses
        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK); // Par de colores para texto rojo sobre fondo negro
        init_pair(2, COLOR_GREEN, COLOR_BLACK); // Par de colores para texto verde sobre fondo negro
        init_pair(3, COLOR_YELLOW, COLOR_BLACK); // Par de colores para texto amarillo sobre fondo negro
    #endif

    HideCursor();

    DisplayMenu(); // Mostrar el menú antes de iniciar el juego

    snake.push_back({anchoTablero/2, altoTablero/2});
    generateFood();

    DrawTable(); // Dibuja tablero

    while (true) 
    {
        HandleInput(); // Leer dirección
        MoveSnake();  // Mover y verificar
        DrawSnake(); // Dibujar serpiente

        // Dibujar comida
        DrawConsole(food.x, food.y, "🐁");

        // Dibujar obstáculos
        for (const auto& obstacle : obstacles) 
        {
            DrawConsole(obstacle.x, obstacle.y, "🦡");
        }

        // Verificar progreso de nivel
        if(players[selectedPlayer].score >= players[selectedPlayer].level * pointsPerLevel) 
        {
            players[selectedPlayer].level++;
            
            DrawConsole(anchoTablero+5, 0, "Nivel: " + to_string(players[selectedPlayer].level), true);
            
            if(players[selectedPlayer].level == maxLevel) 
            {
                snake.clear();
                obstacles.clear();

                players[selectedPlayer].maxScore = players[selectedPlayer].score;


                DisplayWinArt();

                players[selectedPlayer].level ++; //Empezar modo infinito
                
                break;
            }
            else DisplayLevelUpMessage(players[selectedPlayer].level);

            // Aumentar la velocidad al pasar de nivel
            if (snakeSpeed > 50) // Límite mínimo de velocidad
            {
                snakeSpeed -= 10; // Reducir el tiempo de espera para aumentar la velocidad
            }

            generateObstacles(players[selectedPlayer].level); // Generar obstáculos para el nuevo nivel

            DrawTable(); // Dibuja tablero
        }

        if(gameOver) 
        {
            DisplayGameOverArt();

            if(players[selectedPlayer].score > players[selectedPlayer].maxScore) 
            {
                players[selectedPlayer].maxScore = players[selectedPlayer].score;
            }

            players[selectedPlayer].score = 0;
            players[selectedPlayer].level = 1;
            snakeSpeed = 200; // Reiniciar la velocidad

            gameOver = false;
            snake.clear();
            obstacles.clear();
            selectedPlayer = 0;
            main(); //Reiniciamos el juego sin salir del programa
            break;
        }

        await(snakeSpeed); // Controlar la velocidad de la serpiente
    }

    return 0;
}//Jugare yo, se ve mal porque estoy compartiendo el codigo, ahorita probamos en tu pc