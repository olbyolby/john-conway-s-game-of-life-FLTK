#include<FL/Fl.H>
#include<FL/Fl_Window.H>
#include "program.hpp"

void Program::setSize() {
    int sizeX = life->getSizeX();
    int sizeY = life->getSizeY();
    for (int i = 0; i < cellSize; i++) {
        //delete cells[i];
        //Fl::delete_widget(cells[i]);
        cells[i]->hide();
    }
    delete[] cells;
    cells = new Fl_Button*[sizeX*sizeY];
    for (int i = 0; i < sizeX*sizeY; i++) {
        cells[i] = new Fl_Button((i / sizeX) * 30, (i % sizeY) * 30 + 30, 30, 30, nullptr);
        cells[i]->color(FL_BLACK);
        cells[i]->callback(cellCallback, this);
        cells[i]->show();
        mainWindow->add(cells[i]);
    }
    cellSize = sizeX * sizeY;
}
void Program::updateFrame(void *userData) {
    auto self = reinterpret_cast<Program *>(userData);
    auto life = self->life;
    auto gameState = life->getCurrentField();
    auto displayState = self->cells;

    for (int i = 0; i < life->getSizeX() * life->getSizeY(); i++) {
        (*gameState)[i] ? displayState[i]->color(FL_YELLOW) : displayState[i]->color(FL_BLACK);
        displayState[i]->redraw();
    }
    if(self->paused) {
        Fl::add_timeout(self->speed, Program::updateFrame, self);
        return;
    }
    life->nextFrame();
    Fl::add_timeout(self->speed, Program::updateFrame, self);
}
void Program::cellCallback(Fl_Widget *button, void *userData) {
    auto self = reinterpret_cast<Program *>(userData);
    auto life = self->life;
    auto field = life->getCurrentField();

    //std::cout << "field is " << std::hex << (long long)field << std::dec << '\n' << "raw field is " << std::hex << life->activeGameField << std::dec << '\n';
    for (int i = 0; i < self->life->getSizeX() * self->life->getSizeY(); i++) {
        
        if(self->cells[i] == static_cast<Fl_Button *>(button)) {
            
            //std::cout << "checking pos (" << i / self->life->getSizeY() << ',' << i % self->life->getSizeX() << ")\n";
            //std::cout << "it's value is " << ((*field)[i] ? "true" : "false") << '\n';
            (*field)[i] = !(*field)[i];
            //std::cout << "it's now " << ((*field)[i] ? "true" : "false") << '\n';
            //std::cout << "pause is " << (self->paused ? "true" : "false") << '\n';
        }
    }
}
void Program::loadFile(Fl_File_Chooser *browser, void *userData) {
    auto self = reinterpret_cast<Program *>(userData);

    //std::cout << self->chooser->value() << '\n';
    if (self->chooser->shown() == false) {
        self->life->loadFromFile(std::string(browser->value()));
        self->fileName = browser->value();
        self->setSize();
    }
    
}