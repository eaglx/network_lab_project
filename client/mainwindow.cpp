#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectdatamanage.h"

void MainWindow::sendMessage(int internalFlag)
{
    emit messageSent(internalFlag);
}

void MainWindow::internalMessage(int internalFlag)
{
    qDebug() << "internalMessage: " << internalFlag << "\n";
    // QTextCursor  cursor = ui->textEdit->textCursor(); // Get cursor position
    disconnect(ui->textEdit, 0, this, 0);
    if(internalFlag == FLAG_UPDATE_FROM_SERV)
        ui->textEdit->insertPlainText(QString::fromStdString(dataFromServer));
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
}

void diffSearch(int len)
{
    for(int i = 0; i < len; i++) {
        if(dataFromServer[i] != dataFromQTextEdit[i]) {
            send_to_server(FLAG_REPLACE, i, dataFromQTextEdit[i]);
        }
    }
}

/* Code that executes on text change here */
void MainWindow::onTextChanged()
{
    dataFromQTextEdit = ui->textEdit->toPlainText().toUtf8().constData();
    //qDebug() << QString::fromStdString(dataFromQTextEdit);

    int lenServer = dataFromServer.length();
    int lenQText = dataFromQTextEdit.length();

    //change one char
    if(lenServer == lenQText) {
        diffSearch(lenServer);
    }

    //add new chars
    if(lenQText > lenServer) {
        diffSearch(lenServer);

        for(int i = lenServer; i < lenQText; i++) {
            send_to_server(FLAG_APPEND, 0, dataFromQTextEdit[i]);
        }
    }

    //delete chars
    if(lenServer > lenQText) {
        diffSearch(lenQText);
        for(int i = lenQText; i < lenServer; i++) {
            send_to_server(FLAG_RM, 0, ' ');
        }
    }

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
    connect(this, SIGNAL(messageSent(int)), this, SLOT(internalMessage(int)));
    MainWindow::sendMessage(FLAG_UPDATE_FROM_SERV);
}

MainWindow::~MainWindow()
{
    delete ui;
}
