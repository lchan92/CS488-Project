#include <QtWidgets>
#include <iostream>
#include "AppWindow.hpp"

Viewer* AppWindow::m_viewer = NULL;

AppWindow::AppWindow(SceneNode* mapRoot) {
    setWindowTitle("Project");

    QGLFormat glFormat;
    glFormat.setVersion(3,3);
    glFormat.setProfile(QGLFormat::CoreProfile);
    glFormat.setSampleBuffers(true);

    QVBoxLayout *layout = new QVBoxLayout;

    m_viewer = new Viewer(glFormat, this);
    m_viewer->setMapRoot(mapRoot);

    layout->addWidget(m_viewer);

    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);

    createApplicationActions();
    createMenu();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(displayVictoryPopup()));
    timer->start(20);

    QTimer *timer2 = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(closeWindow()));
    timer->start(20);
}

void AppWindow::createApplicationActions() {
    // Creates a new action for quiting and pushes it onto the menu actions vector
    QAction* quitAct    = new QAction(tr("&Quit"), this);

    m_menu_actions.push_back(quitAct);

    // We set the accelerator keys
    // Alternatively, you could use: setShortcuts(Qt::CTRL + Qt::Key_P); 
    quitAct->setShortcuts(QKeySequence::Quit);

    // Set the tip
    quitAct->setStatusTip(tr("Exits the file"));

    // Connect the action with the signal and slot designated
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));
}

void AppWindow::createMenu() {
    m_menu_app = menuBar()->addMenu(tr("&Application"));

    for (auto& action : m_menu_actions) {
        m_menu_app->addAction(action);
    }
}



void AppWindow::displayVictoryPopup() {
    if (m_viewer->isTrophyAcquired()) {    
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "V I C T O R Y", "Quit?", 
                                            QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            close();
        } else {
            m_viewer->reset();
        }
    }
}


void AppWindow::closeWindow() {
    if (m_viewer->mClose) {
        close();
    }
}