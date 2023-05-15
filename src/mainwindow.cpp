#include <QMessageBox>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QOAuthHttpServerReplyHandler>
#include <QNetworkReply>
#include <QUuid>
#include <QAbstractOAuth>
#include <QVariantMap>

#include "../../gallonge_secret/googleid.h"

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
    this->google = new QOAuth2AuthorizationCodeFlow(this);
    this->google->setScope("openid profile");

    connect(this->google, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);

//    this->google->setAuthorizationUrl(QUrl("https://accounts.google.com/o/oauth2/auth"));
    this->google->setAuthorizationUrl(QUrl("https://accounts.google.com/o/oauth2/v2/auth"));
    this->google->setClientIdentifier(CLIENT_ID);
//    this->google->setAccessTokenUrl(QUrl("https://oauth2.googleapis.com/token"));
    this->google->setAccessTokenUrl(QUrl("https://www.googleapis.com/oauth2/v4/token"));
    this->google->setClientIdentifierSharedKey(CLIENT_SECRET);
    auto code_verifier = (QUuid::createUuid().toString(QUuid::WithoutBraces) +
                          QUuid::createUuid().toString(QUuid::WithoutBraces)).toLatin1(); // 43 <= length <= 128
    auto code_challenge = QCryptographicHash::hash(code_verifier, QCryptographicHash::Sha256).toBase64(
        QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals);

    this->google->setModifyParametersFunction([=](QAbstractOAuth::Stage stage, QMultiMap<QString, QVariant>* params)
                                         {
                                             switch (stage)
                                             {
                                             case QAbstractOAuth::Stage::RequestingAuthorization:
                                                 params->insert("code_challenge", code_challenge);
                                                 params->insert("code_challenge_method", "S256");
                                                 break;
                                             case QAbstractOAuth::Stage::RequestingAccessToken:
                                                 params->insert("code_verifier", code_verifier);
                                                 break;
                                             }
                                         });

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

