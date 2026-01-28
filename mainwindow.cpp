#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qwebengineview.h>
#include <QWebEngineView>
#include <QWebChannel>
#include <QWebEnginePage>
#include <QUrl>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "backend.h"

#include <QWebChannel>
#include <QWebEngineView>

#include <QShortcut>
#include <QKeySequence>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    view = new QWebEngineView(this);
    setCentralWidget(view);

    Backend *backend = new Backend(this);

    QWebChannel *channel = new QWebChannel(view->page());
    channel->registerObject("qt", backend);
    view->page()->setWebChannel(channel);

    view->load(QUrl::fromLocalFile(
        // "/home/nord/Project_Data/Code_Hours/Code_hour/Web_UI/index.html"
        // "/home/nord/Project_Data/Code_Hours/Code_hour/Web_UI/problem_list.html"
        // "/home/nord/Project_Data/Code_Hours/Code_hour/Web_UI/solve_panel.html"
        "/home/nord/Project_Data/Code_Hours/SyntaxFlow/Web_UI/Problem_List_Panel.html"
        // "/home/nord/Project_Data/Code_Hours/Code_hour/UI_UX_OWN/test.html"
        ));



    QShortcut *zoom_in = new QShortcut(QKeySequence("ctrl++"),this);
    QShortcut *zoom_out = new QShortcut(QKeySequence("ctrl+-"),this);


    connect(zoom_in,&QShortcut::activated,this,[=](){
        zoom += 0.5;
        view->setZoomFactor(zoom);
        qDebug()<< " the value changed of zoom : "<<zoom;
    });

    connect(zoom_out,&QShortcut::activated,this,[=](){
        zoom -= 0.5;
        view->setZoomFactor(zoom);
        qDebug()<< " the value changed of zoom : "<<zoom;
    });



}

MainWindow::~MainWindow()
{
    delete ui;
}
