#include<FL/Fl.H>
#include<FL/Fl_Window.H>
#include<FL/fl_ask.H>
#include<FL/Fl_Output.H>
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
    for(int x = 0; x < sizeX; x++) {
        for(int y = 0; y < sizeY; y++) {
            cells[y*sizeX + x] = new Fl_Button(y * 30, x * 30 + 30, 30, 30, nullptr);
            cells[y*sizeX + x]->color(FL_BLACK);
            cells[y*sizeX + x]->callback(cellCallback, this);
            cells[y*sizeX + x]->show();
            mainWindow->add(cells[y*sizeX + x]);
        }
    }

    cellSize = sizeX * sizeY;
}
void Program::nextFrame(void *userData) {
    auto self = reinterpret_cast<Program *>(userData);
    auto life = self->life;
    auto gameState = life->getCurrentField();
    auto displayState = self->cells;
    Fl::add_timeout(self->speed, Program::nextFrame, self);

    if(!self->paused) {
        life->nextFrame();
    }
}
void Program::renderFrame(void *userData) {
    auto self = reinterpret_cast<Program *>(userData);
    auto life = self->life;
    auto gameState = life->getCurrentField();
    auto displayState = self->cells;

    for (int i = 0; i < life->getSizeX() * life->getSizeY(); i++) {
        (*gameState)[i] ? displayState[i]->color(FL_YELLOW) : displayState[i]->color(FL_BLACK);
        displayState[i]->redraw();
    }
    Fl::add_timeout(1.0 / 60, Program::renderFrame, self);
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
        try {
            self->life->loadFromFile(std::string(browser->value()));
            self->fileName = browser->value();
            self->setSize();
        } catch(GameOfLife::readError &e) {
            //alart the user
            fl_alert(("Error loading" + e.filename + " at location (" + std::to_string(e.positionX) + "," + std::to_string(e.positionY) + ")").c_str());
        } catch(GameOfLife::fileNotFound &e) {
            fl_alert(("File " + e.filename + " not found").c_str());
        }
    }
    
}
void Program::saveFile(Fl_File_Chooser *browser, void *userData) {
    auto self = reinterpret_cast<Program *>(userData);

    //std::cout << self->chooser->value() << '\n';
    if (self->chooser->shown() == false) {
        try {
            self->life->writeFile(std::string(browser->value()));
            self->fileName = browser->value();
        } catch(GameOfLife::fileNotFound &e) {
            fl_alert(("File " + e.filename + " not found").c_str());
        }
    }
    
}
void Program::newGrid(Fl_Widget* widget, void *userData) {
    auto self = reinterpret_cast<Program *>(userData);

    if(self->gridData != nullptr) {
        fl_alert("new grid window already open");
        return;
    }

    auto data = new gridInputData();
    data->program = self;
    data->window = new Fl_Window(256, 128, "enter grid size");
    data->xgrid = new Fl_Input(64, 0, 256 - 64, 32, "x value");
    data->ygrid = new Fl_Input(64, 32, 256 - 64, 32, "y value");
    data->button = new Fl_Button(0, 128-32, 128, 32, "create");
    self->gridData = data;

    data->window->show();
    data->window->add(data->xgrid);
    data->window->add(data->ygrid);
    data->window->add(data->button);
    auto callBack = [](Fl_Widget* widget, void *userData) {
        auto self = reinterpret_cast<Program *>(userData);
        auto data = self->gridData;

        int x, y;
        try {
            x = std::stoi(data->xgrid->value());
            y = std::stoi(data->ygrid->value());
        } catch (std::invalid_argument &e) {
            std::cout << "Pleace enter a valid size" << std::endl;
            fl_alert("Pleace enter a valid size");
            return;
        }
        if(x < 0 || y < 0) {
            fl_alert("Pleace enter a valid size");
            return;
        }

        self->life->setSize(x, y);
        self->setSize();
        data->window->hide();
        delete data;
        self->gridData = nullptr;
    };
    auto closeCallback = [](Fl_Widget* window, void* userData) {
        auto self = reinterpret_cast<Program *>(userData);
        self->gridData = nullptr;
        window->hide();
    };
    data->window->callback(closeCallback, self);
    data->button->callback(callBack, self);
}

void Program::setSpeed(Fl_Widget* widget, void* userData) {
    auto self = reinterpret_cast<Program *>(userData);

    if(self->speedData != nullptr) {
        fl_alert("speed window already open");
        return;
    }

    auto data = new setSpeedData();
    data->window = new Fl_Window(256, 128, "enter new speed(frames per second)");
    data->speed = new Fl_Input(64, 0, 256 - 64, 32, "speed");
    data->button = new Fl_Button(0, 128 - 32, 64, 32, "set");
    data->program = self;
    self->speedData = data;

    data->window->add(data->speed);
    data->window->add(data->button);
    data->window->show();

    auto callback = [](Fl_Widget*, void* userData) {
        auto self = reinterpret_cast<Program *>(userData);
        auto data = self->speedData;

        int newSpeed = 0;
        try {
            newSpeed = std::stoi(data->speed->value());
        } catch (std::invalid_argument &e) {
            std::cout << "Pleace enter a valid speed" << std::endl;
            fl_alert("pleace enter a valid speed");
            return;
        }
        if(newSpeed < 0) {
            fl_alert("pleace enter a valid speed");
            return;
        }

        data->window->hide();
        data->program->speed = 1.0 / newSpeed;
        delete data;
        self->speedData = nullptr;

    };
    auto closeCallback = [](Fl_Widget* window, void* userData) {
        auto self = reinterpret_cast<Program *>(userData);
        self->speedData = nullptr;
        window->hide();
    };
    data->window->callback(closeCallback, self);
    data->button->callback(callback, self);
};