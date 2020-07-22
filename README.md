# World Famous Drawings Gallery

## Introduction

This is our data structure project. Our teacher ask us to solve an algorithm problem. Here's the description of the problem:

> The world-famous drawings gallery has m*n rooms. To protect the drawings, each room needs to be spied by a robot guard. Each robot can spy 4 rooms on its north, south, west and east. Now write a program to find one solution that can spy all rooms with minimum robots.

![program image](/src/doc/images/program-image.png)

## Algorithm

The algorithm of this program refer to [this article](https://blog.csdn.net/m0_37809890/article/details/80823205)

## GUI

We use [Q3DBars](https://doc.qt.io/qt-5/q3dbars.html) to show the result of the algorithm. The program is based on [this example](https://doc.qt.io/qt-5/qtdatavisualization-bars-example.html).

## Build Process

Here's the structrue of the project:

```
.
├── release // the release version of the program
|   ├── world-famous-drawings-gallery.exe
├── src // source code of the project
```

Use Qt Creator to open the src folder.

If you want to build an independent exe with all the dependence like the release folder, please follow [this article](https://blog.csdn.net/sinat_36264666/article/details/73305712)
