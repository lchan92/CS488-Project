#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <vector>
#include "Viewer.hpp"
#include "scene.hpp"

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppWindow(SceneNode* mapRoot);
	static Viewer* m_viewer;

private:
    void createApplicationActions();
    void createMenu();

    // Each menu itself
    QMenu *m_menu_app;

    std::vector<QAction*> m_menu_actions;
};

#endif