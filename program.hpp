#pragma once
#ifndef PROGRAM_HPP
#define PROGRAM_HPP


#include<FL/Fl_Window.H>
#include<FL/Fl_Menu.H>
#include<FL/Fl_Menu_Bar.H>
#include<FL/Fl_Menu_Button.H>
#include<FL/Fl_Button.H>
#include<FL/Fl_Box.H>
#include<FL/Fl_File_Chooser.H>
#include<FL/Fl.H>
#include<string>
#include<FL/Fl_Value_Input.H>
#include"life.hpp"

class Program {
    Fl_Window* mainWindow;
    Fl_Box* resizingBox;
    Fl_Menu_Bar* toolBar;
    Fl_Menu_Button* toolBar_file;
    Fl_Menu_Button* toolBar_edit;
    Fl_Menu_Button* toolBar_sim;
    Fl_Button* toolBar_dev;

    GameOfLife* life;
    Fl_Button** cells;
    int cellSize;

    Fl_File_Chooser *chooser = nullptr;
    std::string fileName = "";

    bool paused = true;
    int speed = 1.0 / 60;

    struct gridInputData {
        Fl_Window* window;
        Fl_Input* xgrid;
        Fl_Input* ygrid;
        Fl_Button* button;
        Program* program;
    } *gridData = nullptr;
    struct setSpeedData {
        Fl_Window* window;
        Fl_Input* speed;
        Fl_Button* button;
        Program* program;
    } *speedData = nullptr;

    static void unImplamented(Fl_Widget* widget, void* userData) {
        fl_alert("WHAT THE HELL!","If this runs something has gone horribly wrong, report to developer");
        
    }
    static void loadFile(Fl_File_Chooser *browser, void *userData);
    static void cellCallback(Fl_Widget *button, void *self);
    static void nextFrame(void *userData);
    static void renderFrame(void *userData);
    static void resizeButtons(Fl_Widget *, void *userData); 
    static void newGrid(Fl_Widget *, void* userData);
    static void setSpeed(Fl_Widget *, void* userData);
    static void saveFile(Fl_File_Chooser *browser, void *userData);
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
        toolBar_dev = new Fl_Button(300, 0, width/10, 30, "dev");

        //add menu opations to the tool bar
        toolBar->add("file", 0, toolBar_file->callback(),this, 0);
        toolBar->add("sim", 1, toolBar_sim->callback(),this, 1);
        toolBar->add("edit", 2, toolBar_edit->callback(),this, 1);

        //add options for the file sub menu
        toolBar_file->add(
            "load file", 0,
            [](Fl_Widget *widget, void *userData) {
                try {
                    reinterpret_cast<Program *>(userData)->life->loadFromFile(reinterpret_cast<Program *>(userData)->fileName);
                } catch(GameOfLife::readError &e) {
                    //alart the user
                    fl_alert(("Error loading" + e.filename + " at location (" + std::to_string(e.positionX) + "," + std::to_string(e.positionY) + ")").c_str());
                    return;
                } catch(GameOfLife::fileNotFound &e) {
                    fl_alert(("File " + e.filename + " not found").c_str());
                    return;
                }
                reinterpret_cast<Program *>(userData)->setSize();
            },
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
        toolBar_file->add("save file", FL_CTRL + 's', 
            [](Fl_Widget*, void* self) {
                try {
                    reinterpret_cast<Program *>(self)->life->writeFile(reinterpret_cast<Program *>(self)->fileName);
                } catch (GameOfLife::fileNotFound &e) {
                    fl_alert(("File " + e.filename + " not found").c_str());
                }
            }
        , this, 0);
        toolBar_file->add("save file as", 0, 
            [](Fl_Widget *widget, void *userData){
                auto self = reinterpret_cast<Program *>(userData);
                if(self->chooser != nullptr) {
                    delete self->chooser;
                }
                self->chooser = new Fl_File_Chooser("C:/", nullptr, 0, "pick file to load");
                self->chooser->callback(saveFile, userData);
                //self->chooser->type()
                self->chooser->show();
            }, this, 0);

        //add simulation options
        toolBar_sim->add("step", FL_F+3, 
            [](Fl_Widget *widget, void *self){ reinterpret_cast<Program *>(self)->life->nextFrame(); },
        this, 0);
        toolBar_sim->add("pause", FL_F+4, 
            [](Fl_Widget *widget, void *self){ reinterpret_cast<Program *>(self)->paused = !reinterpret_cast<Program *>(self)->paused; },
        this, 0);
        toolBar_sim->add("set speed", 0, setSpeed, this, 0);

        //development tools
        toolBar_dev->callback([](Fl_Widget*, void*) {
            
        });

        //add edit options
        toolBar_edit->add("new grid", 0, newGrid, this, 0);
        toolBar_edit->add("clear", 0, 
            [](Fl_Widget*, void* userData) {
                auto self = reinterpret_cast<Program *>(userData);
                auto life = self->life;
                for(int i = 0; i < life->getSizeX() * life->getSizeY(); i++) {
                    (*life->getCurrentField())[i] = false;
                }
         }, this, 0);

        life = new GameOfLife(10, 10);
        cells = new Fl_Button*[10*10];
        for (int i = 0; i < 10*10; i++) {
            cells[i] = new Fl_Button((i / 10) * 30, (i % 10) * 30 + 30, 30, 30, nullptr);
            cells[i]->color(FL_BLACK);
            cells[i]->callback(cellCallback, this);
            //cells[i]->box(FL_BORDER_BOX);
        }
        cellSize = 10 * 10;

        Fl::add_timeout(speed, nextFrame, this);
        Fl::add_timeout(1.0 / 60, renderFrame, this);

        mainWindow->end();
        mainWindow->show();
        
    }
    ~Program() {
        mainWindow->hide();
        delete[] cells;
        delete life;
        if(gridData != nullptr) {
            gridData->window->hide();
            delete gridData;
        }
        if(speedData != nullptr) {
            speedData->window->hide();
            delete speedData;
        }
    }
};
#endif