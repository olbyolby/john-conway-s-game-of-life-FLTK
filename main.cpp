#include"program.hpp"
int main() {

    try {
        Program program(1000, 500);
        return program.start();
    } catch (std::exception &e) {
        fl_alert("A fatal error occurred!\n", e.what());
    }

}

