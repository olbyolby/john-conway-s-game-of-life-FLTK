#include<vector>
#include<string>
#include<fstream>
#include<sstream>

#include"life.hpp"

int GameOfLife::getNeighbors(int x, int y) {
    int neighbors = 0;
    if(y - 1 >= 0) {
        if(x - 1 >= 0) {neighbors += (*activeGameField)[((y-1)*sizeX) + (x-1)];}
        neighbors += (*activeGameField)[((y-1)*sizeX) + (x)];
        if(x + 1 < sizeY) neighbors += (*activeGameField)[((y-1)*sizeX) + (x+1)];
    }
    neighbors += (*activeGameField)[y*sizeX + (x-1)];
    neighbors += (*activeGameField)[y*sizeX + (x+1)];
    
    if(y+1 < sizeY) {
        if(x - 1 >= 0) {neighbors += (*activeGameField)[((y+1)*sizeX) + (x-1)];}
        neighbors += (*activeGameField)[((y+1)*sizeX) + (x)];
        if(x + 1 < sizeY) neighbors += (*activeGameField)[((y+1)*sizeX) + (x+1)];
    }

    return neighbors;
    
}
bool GameOfLife::testCell(bool state, int neighbors) {
        if (state) {
        // Any live cell with two or three live neighbours survives
        return (neighbors == 2 || neighbors == 3);
    } else {
        // Any dead cell with three live neighbours becomes a live cell
        return (neighbors == 3);
    }
}
#include<iostream>
void GameOfLife::nextFrame() {
    //std::cout << "next frame\n";
    for(int x = 0; x < sizeX; x++) {
        for(int y = 0; y < sizeY; y++) {
            (*hiddenGameField)[y*sizeX + x] = testCell((*activeGameField)[y*sizeX + x],getNeighbors(x, y));
        }
    }
    //swap the buffers;
    auto temp = activeGameField;
    activeGameField = hiddenGameField;
    hiddenGameField = temp;
}
void GameOfLife::runFrames(int frames) {
    while(frames != 0) {
        frames--;
        nextFrame();
    }
}

void GameOfLife::loadFromArray(std::vector<bool> array, int sizeX, int sizeY) {
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    hiddenGameField->resize(array.size(), false);
    activeGameField->resize(array.size(), false);
    for (int i = 0; i < array.size(); i++) {
      (*activeGameField)[i] = array[i];
    }
};

//#include<iostream>
void GameOfLife::loadFromFile(std::string path) {
    std::ifstream file(path);
    std::string line;

    if(!file) {
        throw std::runtime_error("File not found");
    }

    std::getline(file, line); sizeX = std::stoi(line);
    std::getline(file, line); sizeY = std::stoi(line);

    bufferA.resize(sizeX * sizeY);
    bufferB.resize(sizeX * sizeY);

    char temp;
    for (int i = 0; i < activeGameField->size(); i++) {
        file >> temp;
        if (temp != '1' && temp!= '0') {
            i--; continue;
        }
        //std::cout << temp << std::endl;
        (*activeGameField)[i] = temp=='1'? true : false;
    }
    //std::cin.get();
}

std::string GameOfLife::printActiveField() {
    std::string output = "";
    std::cout << "field is " << std::hex << (long long)activeGameField << std::dec << '\n';
    for(int x = 0; x < sizeX; x++) {
        for(int y = 0; y < sizeY; y++) {
            output += (*activeGameField)[y*sizeX + x]? "[]" : "  ";
            //std::cout << "X: " << x << " Y: " << y << " state: " << (*activeGameField)[y*sizeX + x] << std::endl;
        }
        output += "|\n";

    }
    
    //add a bottom line to show the board end
    for (int i = 0; i < sizeX; i++) {
        output += "--";
    }
    return output;
}
GameOfLife::GameOfLife(int x, int y) {
    activeGameField = &bufferA;
    hiddenGameField = &bufferB;

    sizeX = x;
    sizeY = y;
    bufferA.resize(sizeX * sizeY);
    bufferB.resize(sizeX * sizeY);
}

GameOfLife::GameOfLife() {
    activeGameField = &bufferA;
    hiddenGameField = &bufferB;
}