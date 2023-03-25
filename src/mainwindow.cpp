#include <QMessageBox>
#include <QCloseEvent>

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
    static quint32 iii=1;
    Allonge* pAllonge = new Allonge();
    allonges_[++iii] = pAllonge;

    pAllonge->show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Application",
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
        return;
    }
    for (const auto& [key, value] : allonges_) {
        value->close();
        delete value;
    }
    allonges_.clear();

    event->accept();
}
