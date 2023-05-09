#pragma once
#include<vector>
#include<string>
#include<exception>
#include<stdexcept>

class GameOfLife {
    std::vector<bool> bufferA, bufferB;

    int getNeighbors(int x, int y);
    bool testCell(bool state, int neighbors);
protected:
    std::vector<bool>* activeGameField, *hiddenGameField;
    int sizeX, sizeY;
public:
    struct readError : public std::runtime_error {
        std::string filename;
        int positionX;
        int positionY;
        readError(const char* wat, std::string fileName, int positionX, int positionY) : 
            std::runtime_error(wat), 
            positionX(positionX), 
            positionY(positionY), 
            filename(fileName) 
        {};
    };
    struct fileNotFound : public std::runtime_error {
        std::string filename;
        fileNotFound(std::string fileName) :  filename(fileName), std::runtime_error("file not found") {};
    };
    GameOfLife();
    GameOfLife(int x, int y);
    void nextFrame();
    void runFrames(int frames);
    void loadFromFile(std::string filePath);
    void loadFromArray(std::vector<bool> array, int sizeX, int sizeY);
    void writeFile(std::string path);
    void setSize(int x, int y);
    std::string printActiveField();

    std::vector<bool>* getCurrentField() { 
        //std::cout << "field " << std::hex << activeGameField << std::dec << '\n';
        return activeGameField;
        
    }
    int getSizeX() { return sizeX; }
    int getSizeY() { return sizeY; }
};