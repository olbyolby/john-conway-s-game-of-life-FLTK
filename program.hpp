#define WIN32
#include<FL/Fl_Window.H>
#include<FL/Fl_Menu.H>
#include<FL/Fl_Menu_Bar.H>
#include<FL/Fl_Menu_Button.H>
#include<FL/Fl_Button.H>
#include<FL/Fl_Box.H>
#include<FL/Fl_File_Chooser.H>
#include<FL/Fl.H>
#include<iostream>
#include<string>
#include"life.hpp"

class Program {
    Fl_Window* mainWindow;
    Fl_Box* resizingBox;
    Fl_Menu_Bar* toolBar;
    Fl_Menu_Button* toolBar_file;
    Fl_Menu_Button* toolBar_edit;
    Fl_Menu_Button* toolBar_sim;
    Fl_Menu_Button* toolBar_dev;

    GameOfLife* life;
    Fl_Button** cells;
    int cellSize;

    Fl_File_Chooser *chooser = nullptr;
    std::string fileName = "";

    bool paused = true;
    int speed = 1.0 / 60;

    static void unImplamented(Fl_Widget* widget, void* userData) {
        std::cout << "WARNING: unimplamented ui object ran" << std::endl;
        
    }
    static void loadFile(Fl_File_Chooser *browser, void *userData);
    static void cellCallback(Fl_Widget *button, void *self);
    static void updateFrame(void *userData);
    void setSize();
public:
    int start() {
        return Fl::run();
    }
    Program(int width, int hight) {
        resizingBox = new Fl_Box(0, 30, width, hight - 30, "program::reszingBox");
        mainWindow = new Fl_Window(0, 0, width, hight, "program::mainWindow");
        mainWindow->resizable(resizingBox);
        toolBar = new Fl_Menu_Bar(0, 0, width, 30, "program::toolBar");
        toolBar_file = new Fl_Menu_Button(0, 0, width/10, 30, "file");
        toolBar_edit = new Fl_Menu_Button(200, 0, width/10, 30, "edit");
        toolBar_sim = new Fl_Menu_Button(100, 0, width/10, 30, "sim");
        toolBar_dev = new Fl_Menu_Button(300, 0, width/10, 30, "dev");

        //add menu opations to the tool bar
        toolBar->add("file", 0, toolBar_file->callback(),this, 0);
        toolBar->add("sim", 1, toolBar_sim->callback(),this, 1);
        toolBar->add("edit", 2, toolBar_edit->callback(),this, 1);

        //add options for the file sub menu
        toolBar_file->add(
            "load file", 0,
            [](Fl_Widget *widget, void *userData) { reinterpret_cast<Program *>(userData)->life->loadFromFile(reinterpret_cast<Program *>(userData)->fileName);},
            this, 0);
        toolBar_file->add(
            "load file from", 0,
            [](Fl_Widget *widget, void *userData){
                auto self = reinterpret_cast<Program *>(userData);
                if(self->chooser != nullptr) {
                    delete self->chooser;
                }
                self->chooser = new Fl_File_Chooser("C:/", nullptr, 0, "pick file to load");
                self->chooser->callback(loadFile, userData);
                //self->chooser->type()
                self->chooser->show();
            }, this, 0);
        toolBar_file->add("save file", 0, unImplamented, this, 0);
        toolBar_file->add("save file as", 0, unImplamented, this, 0);

        //add simulation options
        toolBar_sim->add("reset", 0, unImplamented, this, 0);
        toolBar_sim->add("start", 0, unImplamented, this, 0);
        toolBar_sim->add("step", FL_F+3, 
        [](Fl_Widget *widget, void *self){ reinterpret_cast<Program *>(self)->life->nextFrame(); },
        this, 0);
        toolBar_sim->add("pause", FL_F+4, 
        [](Fl_Widget *widget, void *self){ reinterpret_cast<Program *>(self)->paused = !reinterpret_cast<Program *>(self)->paused; },
        this, 0);
        toolBar_sim->add("set speed", 0, unImplamented, this, 0);

        //development tools
        toolBar_dev->add("debug print", 0, [](Fl_Widget *widget, void *self) { std::cout << reinterpret_cast<Program *>(self)->life->printActiveField() << '\n'; }, 
        this, 0);

        //add edit options
        toolBar_edit->add("new grid", 0, unImplamented, this, 0);
        toolBar_edit->add("clear", 0, unImplamented, this, 0);

        life = new GameOfLife(10, 10);
        cells = new Fl_Button*[10*10];
        for (int i = 0; i < 10*10; i++) {
            cells[i] = new Fl_Button((i / 10) * 30, (i % 10) * 30 + 30, 30, 30, nullptr);
            cells[i]->color(FL_BLACK);
            cells[i]->callback(cellCallback, this);
        }
        cellSize = 10 * 10;

        Fl::add_timeout(speed, updateFrame, this);

        mainWindow->end();
        mainWindow->show();
        
    }
    ~Program() {
        mainWindow->hide();
        for (int i = 0; i < life->getSizeX() * life->getSizeY(); i++) {
            delete cells[i];
        }
        delete[] cells;
        delete life;
    }
};
