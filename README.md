# Introduction

This is my final project for course "Visual Computing And Learning" at Peking University.

It is a simple SVG Renderer whose GUI is based on Qt, but all the rendering is done by my own code —— My functions render the SVG image pixel by pixel.

Till now, it only supports some of the basic elements and traits. My future plan is to implement a full-featured SVG Renderer, and one day I may refactor it into Rust for learning.

# Build

This project is based on Qt 6.8.1 . You can build it with Qt Creator or CMake.

1. Clone this repository.
2. Open it with Qt Creator, and build it.
3. Or, you can configure the tool chain in any IDE and build this project with it.

# Usage

1. Run the program.
2. Choose the scale ratio you want.
3. Open an SVG file.
4. Now you can see the SVG image rendered in the window.
5. If you want to export the image, you can click on the button below the slider. Only .png files are supported now.