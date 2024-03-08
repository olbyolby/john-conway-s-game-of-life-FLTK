#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<assert.h>
#include<iostream>

#include"life.hpp"

int GameOfLife::getNeighbors(int x, int y) {
    //calculate the number of neighbors a cell and sated index has
    //we check the value of every cell touching the current cell and add all the live ones together
    int neighbors = 0;

    // Iterate over every X value, checking for bounds. (x+2 is done to account for the less than)
    for (int ix = std::max(0, x-1); ix < std::min(x+2, sizeX); ix++) {
        // Itreate over every Y value within the current column
        for (int iy = std::max(0, y-1); iy < std::min(y+2, sizeY); iy++) {
            // Skip the center cell(A cell isn't it's own neighbor) and only count living cells
            if (!(ix == x && iy == y) && (*this->activeGameField)[sizeX*iy + ix] == true) {
                neighbors += 1;
            };
        };
    };

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
void GameOfLife::loadFromFile(std::string path) {
    std::vector<bool> backup = *activeGameField;

    std::ifstream file(path);
    if(!file) {
        throw fileNotFound(path);
    }

    std::string fileData;
    std::ostringstream ss;
    ss << file.rdbuf(); // reading data
    fileData = ss.str();
    if(fileData[fileData.length() - 1] != '\n') {fileData+='\n';}

    sizeY = std::count(fileData.begin(), fileData.end(), '\n');
    sizeX = fileData.substr(1, fileData.find('\n')-1).length()+1;

    //resize buffers
    bufferA.resize(sizeX*sizeY);
    bufferB.resize(sizeX*sizeY);

    //load the buffer data
    int written = 0;
    for(char& cur : fileData) {
        if(cur != '\n' && cur != '\r') {
            (*activeGameField)[written] = cur=='0'? false : true;
            written++;

        }
    }
    if(written < sizeX * sizeY) {
        //load a copy to prevent data corruption
        *activeGameField = backup;
        hiddenGameField->resize(activeGameField->size());
        throw readError("unable to read, data uneven", path, int(written / sizeX), written % sizeY);
    }
}
void GameOfLife::writeFile(std::string path) {
    std::ofstream file(path);
    if(!file) { throw fileNotFound("file not found");}
    int ln = 1;
    for(int i = 0; i < sizeX*sizeY; i++) {  
        file << (*activeGameField)[i]? '1' : '0';
        if(ln == sizeX) { ln = 1; file << '\n';} else {ln++;}

    }
}
std::string GameOfLife::printActiveField() {
    std::string output = "";
    for(int x = 0; x < sizeX; x++) {
        for(int y = 0; y < sizeY; y++) {
            output += (*activeGameField)[y*sizeX + x]? "[]" : "  ";
        }
        output += "|\n";

    }
    
    //add a bottom line to show the board end
    for (int i = 0; i < sizeX; i++) {
        output += "--";
    }
    return output;
}

void GameOfLife::setSize(int x, int y) {
    if(x < 0 || y < 0) {
        throw std::invalid_argument("x or y value is negitive");
    }
    sizeX = x; sizeY = y;
    bufferA.resize(x*y);
    bufferB.resize(x*y);
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