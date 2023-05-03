#include<vector>
#include<string>

#include<iostream>
class GameOfLife {
    std::vector<bool> bufferA, bufferB;

    int getNeighbors(int x, int y);
    bool testCell(bool state, int neighbors);
public:
    std::vector<bool>* activeGameField, *hiddenGameField;
    int sizeX, sizeY;
public:
    GameOfLife();
    GameOfLife(int x, int y);
    void nextFrame();
    void runFrames(int frames);
    void loadFromFile(std::string filePath);
    void loadFromArray(std::vector<bool> array, int sizeX, int sizeY);
    std::string printActiveField();

    std::vector<bool>* getCurrentField() { 
        //std::cout << "field " << std::hex << activeGameField << std::dec << '\n';
        return activeGameField;
        
    }
    int getSizeX() { return sizeX; }
    int getSizeY() { return sizeY; }
};