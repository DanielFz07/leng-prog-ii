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

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main(); //Prototipo

enum Dir { UP, DOWN, LEFT, RIGHT };

std::vector<int> snakeX;
std::vector<int> snakeY;

std::vector<int> obstaclesX;
std::vector<int> obstaclesY;

std::vector<string> playerNames; //Vector que almacena el nombre de cada jugador
std::vector<int> playerScores; //Vector que almacena la puntuacion de cada jugador
std::vector<int> playerMaxScores; //Vector que almacena la puntuacion maxima de cada jugador
std::vector<int> playerLevels; //Vector que almacena el nivel de cada jugador

Dir dir = RIGHT;         // Direccion inicial
int foodX, foodY;        // Posicion de comida
bool gameOver = false;   // Flag de fin

// Variables globales para niveles
int selectedPlayer = 0; // indice del jugador seleccionado
const int scorePerLevel = 5; // Puntos necesarios para avanzar de nivel
const int maxLevel = 5; // Nivel maximo

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

    for (size_t i = 0; i < snakeX.size(); ++i) 
    {
        DrawConsole(snakeX[i], snakeY[i], (i == 0 ? head : "█"));
    }
}

bool checkCollision(int x, int y, const vector<int>& vx, const vector<int>& vy) 
{
    for(size_t i=0; i<vx.size(); ++i) 
    {
        if(vx[i] == x && vy[i] == y) return true;
    }
    return false;
}

void generateFood() 
{
    do 
    {
        foodX = rand() % anchoTablero;
        foodY = rand() % altoTablero;

        if(foodX == 0) foodX = 1;
        if(foodY == 0) foodY = 1;
    } 
    while (checkCollision(foodX, foodY, snakeX, snakeY) || checkCollision(foodX, foodY, obstaclesX, obstaclesY)); 
}

void generateObstacles(int level) 
{
    obstaclesX.clear();
    obstaclesY.clear();
    int numObstacles = level; 

    for(int i = 0; i < numObstacles; ++i) 
    {
        int obsX, obsY;
        do 
        {
            obsX = rand() % anchoTablero;
            obsY = rand() % altoTablero;

            if (obsX == 0) obsX = 1;
            if (obsY == 0) obsY = 1;
        } 
        while(checkCollision(obsX, obsY, snakeX, snakeY) || (obsX == foodX && obsY == foodY) || checkCollision(obsX, obsY, obstaclesX, obstaclesY));

        obstaclesX.push_back(obsX);
        obstaclesY.push_back(obsY);
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
                if (colums == 0 && rows == 0) DrawConsole(colums, rows, "┌");
                else if (colums == anchoTablero + 1 && rows == 0) DrawConsole(colums, rows, "┐");
                else if (colums == 0 && rows == altoTablero + 1) DrawConsole(colums, rows, "└");
                else if (colums == anchoTablero + 1 && rows == altoTablero + 1) DrawConsole(colums, rows, "┘");
                else if (rows == 0 || rows == altoTablero + 1) DrawConsole(colums, rows, "─");
                else DrawConsole(colums, rows, "│");
            }
        }
    }
    // Mostrar datos del jugador
    DrawConsole(anchoTablero+5, 0, "Nivel: " + to_string(playerLevels[selectedPlayer]));
    DrawConsole(anchoTablero+5, 1, "Jugador: " + playerNames[selectedPlayer]);
    DrawConsole(anchoTablero+5, 2, "Puntos: " + to_string(playerScores[selectedPlayer]), true);

    DrawConsole (anchoTablero+5, 4, "Controles:", true);
    DrawConsole (anchoTablero+5, 5, "W/A/S/D o", true);
    DrawConsole (anchoTablero+5, 6, "Flechas", true);

    #ifndef _WIN32
        refresh();
    #endif
}

void MoveSnake() 
{
    int newHeadX = snakeX[0];
    int newHeadY = snakeY[0];

    if (dir == UP) newHeadY--;
    else if (dir == DOWN) newHeadY++;
    else if (dir == LEFT) newHeadX--;
    else if (dir == RIGHT) newHeadX++;

    // Verificar colisiones
    if (newHeadX < 1 || newHeadX > anchoTablero || newHeadY < 1 || newHeadY > altoTablero || checkCollision(newHeadX, newHeadY, snakeX, snakeY)) 
    {
        gameOver = true;
        return;
    }

    // Verificar colision con obstaculos
    if (checkCollision(newHeadX, newHeadY, obstaclesX, obstaclesY)) 
    {
        gameOver = true;
        return;
    }

    snakeX.insert(snakeX.begin(), newHeadX); // Agregar nueva cabeza
    snakeY.insert(snakeY.begin(), newHeadY);

    // Si come comida, crecer (no remover cola); sino, remover cola
    if ((newHeadX == foodX && newHeadY == foodY) || (newHeadX == foodX + 1 && newHeadY == foodY)) 
    {
        playerScores[selectedPlayer]++;
        DrawConsole(anchoTablero+5, 2, "Puntos: " + to_string(playerScores[selectedPlayer]), true);
        generateFood();
    } 
    else 
    {
        int tailX = snakeX.back();
        int tailY = snakeY.back();
        DrawConsole(tailX, tailY, " ");
        snakeX.pop_back();
        snakeY.pop_back();
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

//Daniel

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

void RegisterNewPlayer()
{
    DrawConsole(10, 5, "Nuevo jugador: ", true);

    string playerName;
    #ifdef _WIN32
        getline(cin, playerName);
    #else
        echo(); // Habilitar eco para entrada de texto
        char nameBuffer[20];
        getnstr(nameBuffer, 20);
        playerName = string(nameBuffer);
        noecho(); // Deshabilitar eco despues de la entrada
    #endif

    if(playerName.size() > 20) playerName = playerName.substr(0, 20); // Limitar longitud del nombre

    playerNames.push_back(playerName);
    playerScores.push_back(0);
    playerMaxScores.push_back(0);
    playerLevels.push_back(1);
}

bool DisplayPlayerMenu()
{
    clrscr();

    if (playerNames.empty()) 
    {
        RegisterNewPlayer();        

        return true;
    }
    bool playerSelected = false;

    while(!playerSelected) 
    {
        clrscr();
        DrawConsole(10, 5, "Selecciona un jugador:");

        for (size_t i = 0; i < playerNames.size(); ++i) 
        {
            DrawConsole(10, 7 + i, (i == selectedPlayer ? "> " : "  ") + playerNames[i] + " (Puntos: " + to_string(playerScores[i]) + ")");
        }
        DrawConsole(10, 7 + playerNames.size(), (selectedPlayer == playerNames.size() ? "> Nuevo jugador" : "  Nuevo jugador"), true);

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
            selectedPlayer = (selectedPlayer - 1 + playerNames.size() + 1) % (playerNames.size() + 1);
        } 
        else if (key == 's' || key == KEY_DOWN) 
        {
            selectedPlayer = (selectedPlayer + 1) % (playerNames.size() + 1);
        } 
        else if (key == '\n' || key == KEY_ENTER || key == 10) 
        {
            if (selectedPlayer == playerNames.size()) 
            {
                RegisterNewPlayer();
            }
            playerSelected = true;
        }
    }

    return true;
}

void SortPlayers() 
{
    for(size_t i = 0; i < playerNames.size() - 1; ++i) 
    {
        size_t maxIndex = i;
        for(size_t j = i + 1; j < playerNames.size(); ++j) 
        {
            if(playerNames[j] < playerNames[maxIndex]) 
            {
                maxIndex = j;
            } 
        }
        // Intercambiar jugadores
        if (maxIndex != i) 
        {
            swap(playerNames[i], playerNames[maxIndex]);
            swap(playerScores[i], playerScores[maxIndex]);
            swap(playerMaxScores[i], playerMaxScores[maxIndex]);
            swap(playerLevels[i], playerLevels[maxIndex]);
        }
    }
}

void DisplayTopPlayers()
{
    clrscr();

    if (playerNames.empty()) 
    {
        DrawConsole(8, 6, "No hay jugadores registrados.", true);

        DrawConsole(7, 8 + playerNames.size() + 1, "Presiona cualquier tecla para volver al menu.", true);
        #ifdef _WIN32
            _getch();
        #else
            getch();
        #endif

        return;
    } 
    
    SortPlayers(); // Ordenar jugadores
    DrawConsole(7, 6, "TOP Jugadores:", true);
    DrawConsole(7, 7, "Nombre               Puntos   Maximo", true);
    for (size_t i = 0; i < playerNames.size(); ++i) 
    {
        string line = to_string(i + 1) + ". " + playerNames[i];
        while(line.length() < 21) line += " ";
        
        string score = to_string(playerScores[i]);
        while(score.length() < 9) score += " ";
        
        string maxScore = to_string(playerMaxScores[i]);
        
        DrawConsole(7, 8 + i, line + score + maxScore, true);
    }

    DrawConsole(7, 8 + playerNames.size() + 1, "Presiona cualquier tecla para volver al menu.", true);
    #ifdef _WIN32
        _getch();
    #else
        getch();
    #endif
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
                    DisplayTopPlayers();
                    break;
                }
                case 2: exit(0); // Salir
            }
        }
    }
}

void DisplayGameOverArt() 
{
    int size = 5;
    int startRow = (altoTablero) / 2;
    int startCol = (anchoTablero) / 2;
    
    while (size <= 9) 
    {
        clrscr();

        if (size == 7) 
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

    int startRow = (altoTablero) / 2;
    int startCol = (anchoTablero) / 2;

    for (int i = 0; i < 3; ++i) // Mostrar por 3 segundos
    {
        clrscr();

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
    
    int startRow = (altoTablero) / 2;
    int startCol = (anchoTablero) / 2;
    
    for (int i = 0; i < 3; ++i) // Mostrar por 3 segundos
    {
        clrscr();

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

int main(void)
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

    snakeX.push_back(anchoTablero/2);
    snakeY.push_back(altoTablero/2);

    generateFood();

    DrawTable(); // Dibuja tablero

    while (true) 
    {
        HandleInput(); // Leer direccion
        MoveSnake();  // Mover y verificar
        DrawSnake(); // Dibujar serpiente

        // Dibujar comida
        DrawConsole(foodX, foodY, "🐁");

        // Dibujar obstaculos
        for (size_t i = 0; i < obstaclesX.size(); ++i) 
        {
            DrawConsole(obstaclesX[i], obstaclesY[i], "🦡");
        }

        // Verificar progreso de nivel
        if(playerScores[selectedPlayer] >= playerLevels[selectedPlayer] * scorePerLevel) 
        {
            playerLevels[selectedPlayer]++;
            
            DrawConsole(anchoTablero+5, 0, "Nivel: " + to_string(playerLevels[selectedPlayer]), true);
            
            if(playerLevels[selectedPlayer] == maxLevel) 
            {
                snakeX.clear();
                snakeY.clear();
                obstaclesX.clear();
                obstaclesY.clear();

                playerMaxScores[selectedPlayer] = playerScores[selectedPlayer];

                DisplayWinArt();

                playerLevels[selectedPlayer] ++; //Empezar modo infinito
                
                break;
            }
            else DisplayLevelUpMessage(playerLevels[selectedPlayer]);

            // Aumentar la velocidad al pasar de nivel
            if (snakeSpeed > 50) // Limite minimo de velocidad
            {
                snakeSpeed -= 10; // Reducir el tiempo de espera para aumentar la velocidad
            }

            generateObstacles(playerLevels[selectedPlayer]); // Generar obstaculos para el nuevo nivel

            DrawTable(); // Dibuja tablero
        }

        if(gameOver) 
        {
            DisplayGameOverArt();

            if(playerScores[selectedPlayer] > playerMaxScores[selectedPlayer]) 
            {
                playerMaxScores[selectedPlayer] = playerScores[selectedPlayer];
            }

            playerScores[selectedPlayer] = 0;
            playerLevels[selectedPlayer] = 1;
            snakeSpeed = 200; // Reiniciar la velocidad

            gameOver = false;
            snakeX.clear();
            snakeY.clear();
            obstaclesX.clear();
            obstaclesY.clear();
            selectedPlayer = 0;
            main(); //Reiniciamos el juego sin salir del programa
            break;
        }

        await(snakeSpeed); // Controlar la velocidad de la serpiente
    }

    return 1;
}