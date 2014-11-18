#include <QApplication>
#include <QDesktopWidget>
#include <string>
#include <iostream>
#include "AppWindow.hpp"
#include "scene_lua.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    std::string modelFilename = "puppet.lua";
    std::string mapFilename = "map.lua";

    if (argc >= 2) {
        modelFilename = argv[1];
        mapFilename = argv[2];
    }

    // This is how you might import a scene
    SceneNode* modelRoot = import_lua(modelFilename);
    if (!modelRoot) {
        std::cerr << "Could not open " << modelFilename << std::endl;
        return 1;
    }
    SceneNode* mapRoot = import_lua(mapFilename);
    if (!mapRoot) {
        std::cerr << "Could not open " << mapFilename << std::endl;
        return 1;
    }
 


    AppWindow* window = new AppWindow(modelRoot, mapRoot);

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

