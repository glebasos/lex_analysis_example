#include "mainwindow.h"
#include "manager.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Manager *manager = new Manager();
    ui->setupUi(this);

    ui->mainLayout->addWidget(manager->getWidgets());
}

MainWindow::~MainWindow()
{
    delete ui;
}
