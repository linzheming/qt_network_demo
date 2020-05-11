/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QElapsedTimer>
#include <QTcpServer>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QLabel;
class QProgressBar;
class QPushButton;
QT_END_NAMESPACE

/*
这个例子里面，客户端主动连接，向服务端发送数据。 服务端接收完数据就关闭连接，所以会导致产生TIME_WAIT现象
修改了原来的loopback源码，增加如下功能：
1、 暂停和继续发送
2、 发送速率的计算
3、 把服务器端口修改为8888 固定端口，方便用netstat命令查看
windows下
netstat -p tcp -n  -q | findstr 8888
linux 下
netstat -antp
*/

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    void startServer(int port = 8888);
    enum CLIENT_STATE {
        INIT,
        PLAY,
        PAUSE
    };
    CLIENT_STATE state = INIT;

public slots:
    void start();
    void acceptConnection();
    void startTransfer();
    void updateServerProgress();
    void updateClientProgress(qint64 numBytes);
    void displayError(QAbstractSocket::SocketError socketError);
    void resumeTransfer();

private:

    QProgressBar *clientProgressBar = nullptr;
    QProgressBar *serverProgressBar = nullptr;
    QLabel *clientStatusLabel = nullptr;
    QLabel *serverStatusLabel = nullptr;

    QPushButton *startButton = nullptr;
    QPushButton *quitButton = nullptr;
    QPushButton *pauseOrResumeButton = nullptr;
    QDialogButtonBox *buttonBox = nullptr;

    QTcpServer tcpServer;
    QTcpSocket tcpClient;
    QTcpSocket *tcpServerConnection = nullptr;
    int bytesToWrite = 0;
    int bytesWritten = 0;
    int bytesReceived = 0;

    int bytesPerSecond = 0;
    QElapsedTimer timer;
};

#endif
