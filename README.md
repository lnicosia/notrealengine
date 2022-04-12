# notrealengine
Cross platform OpenGL 3D Library aimed to be simple, flexible and usable in multiple 42 projects (https://42.fr/en).

## Summary

* [Introduction](#introduction)
* [Installation](#installation)
* [Features](#features)
* [Authors](#authors)

## Introduction

This library was first created to fill the [HumanGL](https://github.com/42njaber/humangl) project requirements.
The goal is to create an engine capable of printing 3D cubes of 1x1x1 in a hierarchical model, apply transformations to them to create a human character and most importantly apply animations to those members
![](doc/HumanGL.png)

It was quickly decided that many more features would be usefull, for this project but also for the potential next ones.

## Installation

Tested on Linux, Windows and WSL (needs an X server like Xming).

OpenGL 4.0 is required (hence it is not going to work on Virtual Box's Linux machines, since they can handle OpenGL 3.3 at most).

#### Linux, WSL

`clang` version compatible with C++20 is required.

Run `make`. That will produce a `libnre.a` static library file that you can link against any program you want.

#### Windows

Run `Setup.bat` from cmd or the file explorer.

Open and build the `Not Real Engine.sln`  solution file located in the `windows` folder. Debug configuration will produce a `NotRealEngined.lib` static library file in `windows/x64/Debug` and Release configuration will produce a `NotRealEngine.lib`  static library file in `windows/x64/Release`.

## Features

## Authors

Neyl Jaber (https://github.com/42njaber)

Lucas Nicosia (https://github.com/lnicosia)