#!/bin/bash

build_and_run() {
    if [ ! -d "build" ]; then
        mkdir build
    fi
    cd build || exit

    if cmake ..; then
        if make -j"$(nproc)"; then
                cd Reshape || exit
                ./Reshape
            echo "Reshape Build successful"
        else
            echo "Reshape Make failed."
            exit 1
        fi
    else
        echo "Reshape CMake failed."
        exit 1
    fi
}

notify-send -a "Reshape" "Begin build."

build_and_run "$1"
