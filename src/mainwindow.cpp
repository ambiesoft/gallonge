
#include "mainwindow.h"
#include "allonge.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    Allonge* pAllonge = new Allonge();
    allonges_["AAA"] = pAllonge;

    pAllonge->show();
}

