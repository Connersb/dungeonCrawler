#include <iostream>
#include <fstream>
#include <string>
#include "logic.h"

using std::cout, std::endl, std::ifstream, std::string;

/**
 * TODO: Student implement this function
 * Load representation of the dungeon level from file into the 2D map.
 * Calls createMap to allocate the 2D array.
 * @param   fileName    File name of dungeon level.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference to set starting position.
 * @return  pointer to 2D dynamic array representation of dungeon map with player's location., or nullptr if loading fails for any reason
 * @updates  maxRow, maxCol, player
 */
char** loadLevel(const string& fileName, int& maxRow, int& maxCol, Player& player) {
    char** map = nullptr;
    int idk = 0;
    try
    {
        ifstream inFS;
        inFS.open(fileName);
        if(!inFS.is_open())
        {
            throw std::runtime_error("Failed to open " + fileName);
        }
        string heightHolder;
        string widthHolder;
        int height;
        int width;
        inFS >> heightHolder;
        inFS >> widthHolder;
        height = std::stoi(heightHolder);
        width = std::stoi(widthHolder);
        idk = height;
        if(width*height < INT16_MAX)
        {
            if(height < 999999 && width < 999999 && height > 0 && width > 0)
            {
                maxCol = width;
                maxRow = height;
                map = createMap(height, width);
                string xholder;
                string yholder;
                int playerX;
                int playerY;
                inFS >> yholder;
                inFS >> xholder;
                playerX = std::stoi(xholder);
                playerY = std::stoi(yholder);
                bool moveOn = false;
                bool roomToMove1 = false;
                bool roomToMove2 = false;
                if(playerX < width && playerY < height && playerX >= 0 && playerY >= 0)
                {
                    player.col = playerX;
                    player.row = playerY;
                    for(int i=0;i<height;i++)
                    {
                        for(int j=0;j<width;j++)
                        {
                            char spot = 'f';
                            inFS >> spot;
                            if(spot != 'f')
                            {
                                if(spot == TILE_AMULET || spot == TILE_DOOR || spot == TILE_EXIT || spot == TILE_MONSTER || spot == TILE_OPEN || spot == TILE_PILLAR || spot == TILE_TREASURE)
                                {
                                    map[i][j] = spot;
                                    if(spot == TILE_EXIT || spot == TILE_DOOR)
                                    {
                                        moveOn = true;
                                    }
                                    if(spot == TILE_OPEN)
                                    {
                                        roomToMove1 = true;
                                    }
                                }
                                else
                                {
                                    throw std::runtime_error("please");
                                }
                            }
                            else
                            {
                                throw std::runtime_error("kill me");
                            }
                        }
                    }
                    if(playerX == 0 && playerY == 0)
                    {
                        if(map[playerY+1][playerX] == TILE_OPEN || map[playerY][playerX+1] == TILE_OPEN)
                        {
                            roomToMove2 = true;
                        }
                    }
                    else if(playerX == 0)
                    {
                        if(map[playerY+1][playerX] == TILE_OPEN || map[playerY-1][playerX] == TILE_OPEN || map[playerY][playerX+1] == TILE_OPEN)
                        {
                            roomToMove2 = true;
                        }
                    }
                    else if(playerY == 0)
                    {
                        if(map[playerY+1][playerX] == TILE_OPEN || map[playerY][playerX+1] == TILE_OPEN || map[playerY][playerX-1] == TILE_OPEN)
                        {
                            roomToMove2 = true;
                        }
                    }
                    else
                    {
                        if(map[playerY+1][playerX] == TILE_OPEN || map[playerY-1][playerX] == TILE_OPEN || map[playerY][playerX+1] == TILE_OPEN || map[playerY][playerX-1] == TILE_OPEN)
                        {
                            roomToMove2 = true;
                        }
                    }
                    if(moveOn && roomToMove1 && roomToMove2)
                    {
                        char overflow = 'q';
                        inFS >> overflow;
                        if(overflow == 'q')
                        {
                            if(map[playerY][playerX] == TILE_OPEN)
                            {
                                map[playerY][playerX] = TILE_PLAYER;
                                return map;
                            }
                            else
                            {
                                throw std::runtime_error("person is blocked");
                            }
                        }
                        else
                        {
                            throw std::runtime_error("bad");
                        }
                    }
                    else
                    {
                        throw std::runtime_error("no exit");
                    }
                }
                else
                {
                    throw std::runtime_error("Out of bounds");
                }
            }
            else
            {
                throw std::runtime_error("Too big");
            }
        }
        else
        {
            throw std::runtime_error("Wowzers");
        }
    }
    catch(...)
    {
        deleteMap(map, idk);
        return map;
    }
}

/**
 * TODO: Student implement this function
 * Translate the character direction input by the user into row or column change.
 * That is, updates the nextRow or nextCol according to the player's movement direction.
 * @param   input       Character input by the user which translates to a direction.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @updates  nextRow, nextCol
 */
void getDirection(char input, int& nextRow, int& nextCol) {
    switch (input)
    {
    case 'w':
        nextRow--;
        break;
    case 'a':
        nextCol--;
        break;
    case 's':
        nextRow++;
        break;
    case 'd':
        nextCol++;
        break;
    default:
        break;
    }
}

/**
 * TODO: [suggested] Student implement this function
 * Allocate the 2D map array.
 * Initialize each cell to TILE_OPEN.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @return  2D map array for the dungeon level, holds char type.
 */
char** createMap(int maxRow, int maxCol) {
    char** map = new char*[maxRow];
    for(int i=0;i<maxRow;i++)
    {
        map[i] = new char[maxCol];
    }
    for(int i=0;i<maxRow;i++)
    {
        for(int j=0;j<maxCol;j++)
        {
            map[i][j] = TILE_OPEN;
        }
    }
    return map;
}

/**
 * TODO: Student implement this function
 * Deallocates the 2D map array.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @return None
 * @update map, maxRow
 */
void deleteMap(char**& map, int& maxRow) {
    if(map != nullptr)
    {
        for(int i=0;i<maxRow;i++)
        {
            delete[] map[i];
        }
        delete[] map;
        map = nullptr;
        maxRow = 0;
    }
    else
    {
        maxRow = 0;
    }
}

/**
 * TODO: Student implement this function
 * Resize the 2D map by doubling both dimensions.
 * Copy the current map contents to the right, diagonal down, and below.
 * Do not duplicate the player, and remember to avoid memory leaks!
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height), to be doubled.
 * @param   maxCol      Number of columns in the dungeon table (aka width), to be doubled.
 * @return  pointer to a dynamically-allocated 2D array (map) that has twice as many columns and rows in size.
 * @update maxRow, maxCol
 */
char** resizeMap(char** map, int& maxRow, int& maxCol) {
    if(map == nullptr || maxRow <= 0 || maxCol <= 0)
    {
        return nullptr;
    }
    else
    {
        int playerx;
        int playery;
        for(int i=0;i<maxRow;i++)
        {
            for(int j=0;j<maxCol;j++)
            {
                if(map[i][j] == TILE_PLAYER)
                {
                    playerx = j;
                    playery = i;
                    map[i][j] = TILE_OPEN;
                    i += 10000000;
                    break;
                }
            }
        }
        char** newMap = createMap(maxRow*2, maxCol*2);
        for(int i=0;i<maxRow;i++)
        {
            for(int j=0;j<maxCol;j++)
            {
                newMap[i][j] = map[i][j];
                newMap[i+maxRow][j] = map[i][j];
                newMap[i][j+maxCol] = map[i][j];
                newMap[i+maxRow][j+maxCol] = map[i][j];
            }
        }
        newMap[playery][playerx] = TILE_PLAYER;
        int placeholder = maxRow;
        deleteMap(map, placeholder);
        maxRow = maxRow*2;
        maxCol = maxCol*2;
        return newMap;
    }
}

/**
 * TODO: Student implement this function
 * Checks if the player can move in the specified direction and performs the move if so.
 * Cannot move out of bounds or onto TILE_PILLAR or TILE_MONSTER.
 * Cannot move onto TILE_EXIT without at least one treasure. 
 * If TILE_TREASURE, increment treasure by 1.
 * Remember to update the map tile that the player moves onto and return the appropriate status.
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object to by reference to see current location.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @return  Player's movement status after updating player's position.
 * @update map contents, player
 */
int doPlayerMove(char** map, int maxRow, int maxCol, Player& player, int nextRow, int nextCol) {
    if(nextCol < maxCol && nextRow < maxRow && nextCol >= 0 && nextRow >= 0)
    {
        char moveSpot = map[nextRow][nextCol];
        if(moveSpot != TILE_MONSTER && moveSpot != TILE_PILLAR)
        {
            if(moveSpot == TILE_TREASURE)
            {
                player.treasure++;
                map[nextRow][nextCol] = TILE_PLAYER;
                map[player.row][player.col] = TILE_OPEN;
                player.col = nextCol;
                player.row = nextRow;
                return STATUS_TREASURE;
            }
            else if(moveSpot == TILE_OPEN)
            {
                map[nextRow][nextCol] = TILE_PLAYER;
                map[player.row][player.col] = TILE_OPEN;
                player.col = nextCol;
                player.row = nextRow;
                return STATUS_MOVE;
            }
            else if(moveSpot == TILE_AMULET)
            {
                map[nextRow][nextCol] = TILE_PLAYER;
                map[player.row][player.col] = TILE_OPEN;
                player.col = nextCol;
                player.row = nextRow;
                return STATUS_AMULET;
            }
            else if(moveSpot == TILE_DOOR)
            {
                map[nextRow][nextCol] = TILE_PLAYER;
                map[player.row][player.col] = TILE_OPEN;
                player.col = nextCol;
                player.row = nextRow;
                return STATUS_LEAVE;
            }
            else if(moveSpot == TILE_EXIT)
            {
                if(player.treasure > 0)
                {
                    map[nextRow][nextCol] = TILE_PLAYER;
                    map[player.row][player.col] = TILE_OPEN;
                    player.col = nextCol;
                    player.row = nextRow;
                    return STATUS_ESCAPE;
                }
                else
                {
                    return STATUS_STAY;
                }
            }
            else
            {
                return STATUS_STAY;
            }
        }
        else
        {
            return STATUS_STAY;
        }
    }
    else
    {
        return STATUS_STAY;
    }
}

/**
 * TODO: Student implement this function
 * Update monster locations:
 * We check up, down, left, right from the current player position.
 * If we see an obstacle, there is no line of sight in that direction, and the monster does not move.
 * If we see a monster before an obstacle, the monster moves one tile toward the player.
 * We should update the map as the monster moves.
 * At the end, we check if a monster has moved onto the player's tile.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference for current location.
 * @return  Boolean value indicating player status: true if monster reaches the player, false if not.
 * @update map contents
 */
bool doMonsterAttack(char** map, int maxRow, int maxCol, const Player& player) {
    bool monster = false;
    for(int i=player.row-1;i>=0;i--)
    {
        if(map[i][player.col] == TILE_MONSTER)
        {
            if(i+1 == player.row)
            {
                monster = true;
            }
            map[i+1][player.col] = TILE_MONSTER;
            map[i][player.col] = TILE_OPEN;
        }
        if(map[i][player.col] == TILE_PILLAR)
        {
            break;
        }
    }
    for(int i=player.row+1;i<maxRow;i++)
    {
        if(map[i][player.col] == TILE_MONSTER)
        {
            if(i-1 == player.row)
            {
                monster = true;
            }
            map[i-1][player.col] = TILE_MONSTER;
            map[i][player.col] = TILE_OPEN;
        }
        if(map[i][player.col] == TILE_PILLAR)
        {
            break;
        }
    }
    for(int i=player.col-1;i>=0;i--)
    {
        if(map[player.row][i] == TILE_MONSTER)
        {
            if(i+1 == player.col)
            {
                monster = true;
            }
            map[player.row][i+1] = TILE_MONSTER;
            map[player.row][i] = TILE_OPEN;
        }
        if(map[player.row][i] == TILE_PILLAR)
        {
            break;
        }
    }
    for(int i=player.col+1;i<maxCol;i++)
    {
        if(map[player.row][i] == TILE_MONSTER)
        {
            if(i-1 == player.col)
            {
                monster = true;
            }
            map[player.row][i-1] = TILE_MONSTER;
            map[player.row][i] = TILE_OPEN;
        }
        if(map[player.row][i] == TILE_PILLAR)
        {
            break;
        }
    }
    return monster;
}
