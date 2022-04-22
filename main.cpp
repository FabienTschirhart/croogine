#pragma once
#include "Croogine_App.h"

//std
#include <iostream>
#include <cstdlib>
#include <stdexcept>

int main() {

    Croogine::CroogineApp application{};

    try {
        application.run();

    } catch (const std::exception& e) {

        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}