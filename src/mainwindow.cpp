#include <QMessageBox>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QOAuthHttpServerReplyHandler>
#include <QNetworkReply>

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

void MainWindow::on_pushButton_2_clicked()
{
//    const QString CLIENT_ID = "765500671831-oratm01eg6smpf9hvvenkaodn8jvko8m.apps.googleusercontent.com";
//    const QString CLIENT_SECRET = "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";

    const QString CLIENT_ID = "765500671831-vdna23a7uens3deg33qrccck899g4afr.apps.googleusercontent.com";
    const QString CLIENT_SECRET = "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";

    this->google = new QOAuth2AuthorizationCodeFlow(this);
    this->google->setScope("email");

    connect(this->google, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);

    this->google->setAuthorizationUrl(QUrl("https://accounts.google.com/o/oauth2/auth"));
    this->google->setClientIdentifier(CLIENT_ID);
    this->google->setAccessTokenUrl(QUrl("https://oauth2.googleapis.com/token"));
    this->google->setClientIdentifierSharedKey(CLIENT_SECRET);

    // In my case, I have hardcoded 5476 to test
    auto replyHandler = new QOAuthHttpServerReplyHandler(5476, this);
    this->google->setReplyHandler(replyHandler);
    this->google->grant();


    connect(this->google, &QOAuth2AuthorizationCodeFlow::granted, [=](){
        qDebug() << __FUNCTION__ << __LINE__ << "Access Granted!";

        auto reply = this->google->get(QUrl("https://www.googleapis.com/plus/v1/people/me"));
        connect(reply, &QNetworkReply::finished, [reply](){
            qDebug() << "REQUEST FINISHED. Error? " << (reply->error() != QNetworkReply::NoError);
            qDebug() << reply->readAll();
        });
    });

}

