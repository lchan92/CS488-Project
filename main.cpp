#include <QApplication>
#include <QDesktopWidget>
#include <string>
#include <iostream>
#include "AppWindow.hpp"
#include "scene_lua.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    std::string mapFilename = "map.lua";

    if (argc >= 2) {
        mapFilename = argv[1];
    }
    
    SceneNode* mapRoot = import_lua(mapFilename);
    if (!mapRoot) {
        std::cerr << "Could not open " << mapFilename << std::endl;
        return 1;
    }
 


    AppWindow* window = new AppWindow(mapRoot);

    window->resize(QSize(800,600));
    int desktopArea = QApplication::desktop()->width() * 
                      QApplication::desktop()->height();

    int widgetArea = window->width() * window->height();

    if (((float)widgetArea / (float) desktopArea) < 0.75f) {
        window->show();
    } else {
        window->showMaximized();
    }

    return app.exec();
}

