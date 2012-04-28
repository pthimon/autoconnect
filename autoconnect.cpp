/*
 * autoconnect.cpp
 *
 * Automatically connect to the network specified in argv[1]
 * with the interval argv[2] seconds.
 *
 *  Created on: 06-Jun-2010
 *      Author: Simon Butler
 */

#include "autoconnect.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QString>
#include <QStringList>

AutoConnect::AutoConnect()
{
    QStringList args = QCoreApplication::arguments();
    int numArgs = args.count();
    if (numArgs == 3) {
        mSSID = args.at(1);
        mTimerInterval = QString(args.at(2)).toInt()*1000;
    } else if (numArgs == 2) {
        mSSID = args.at(1);
        mTimerInterval = 300000; //default to 5 min scan interval
    } else {
        mTimerInterval = 0;
    }

    if (mSSID.isEmpty()) {
        qDebug() << "Available networks to auto-connect to:";
    } else {
        qDebug() << "Connecting to " << mSSID << " with interval " << mTimerInterval/1000 << "secs";
    }
}

bool AutoConnect::init() {
    QList<QNetworkConfiguration> configurations = mManager.allConfigurations ( QNetworkConfiguration::Defined );

    QNetworkConfiguration config;
    foreach(config, configurations) {
        qDebug() << "Network: " << config.name();
        if (config.name() == mSSID) {
            mConnectConfig = config;
        }
    }

    if (mSSID.isEmpty()) {
        qDebug() << "Relaunch with " << QCoreApplication::arguments().at(0) << " {ssid} {interval}";
        return false;
    }

    connect(&mManager, SIGNAL(onlineStateChanged(bool)), this ,SLOT(onlineStateChanged(bool)));
    connect(&mManager, SIGNAL(updateCompleted()), this, SLOT(onUpdateCompleted()));

    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(startScan()));

    if (!mManager.isOnline()) { //always returns false here :-(
        qDebug() << "(potentially) not online, starting scan and timer";
        startScan();
        mTimer->start(mTimerInterval);
    }
    return true;
}

void AutoConnect::connectNetworkSession() {
    if (!mManager.isOnline()) {
        //assume network is available
        mSession = new QNetworkSession(mConnectConfig,this);
        mSession->open();
        connect(mSession, SIGNAL(error(QNetworkSession::SessionError)), this, SLOT(onError(QNetworkSession::SessionError)));
        connect(mSession, SIGNAL(opened()), this, SLOT(onConnected()));
    } else {
        qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss") <<  "Aborting connection attempt because already online, stopping timer";
        mTimer->stop();
    }
}

void AutoConnect::onError(QNetworkSession::SessionError error) {
    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss") << "Connection failed: " << error;
    //delete old session
    mSession->deleteLater();

    if (!mTimer->isActive()) {
        //sometimes session can trigger an error event without an associated onlineStateChanged event, so restart the timer if it is not running
        qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss") << "Timer not running, restarting";
        mTimer->start(mTimerInterval);
    }
}

void AutoConnect::onConnected() {
    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss") << "Session connected, stopping timer";
    mTimer->stop();
}

void AutoConnect::startScan() {
    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss") << "Scanning...";
    mManager.updateConfigurations();
}

void AutoConnect::onlineStateChanged(bool isOnline) {
    if (!isOnline) {
        qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss") <<  "Gone offline, starting scan and timer";
        //immediately try to reconnect
        startScan();
        //and start scan timer
        mTimer->start(mTimerInterval);
    } else {
        //weird: onlineStateChanged(true) not emitted when QNetworkSession is connected (or disconnected), but is emitted when connection is initiated through maemo gui
        qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss") << "Connected, stopping timer";
        mTimer->stop();
    }
}

void AutoConnect::onUpdateCompleted() {
    QList<QNetworkConfiguration> configurations = mManager.allConfigurations ( QNetworkConfiguration::Discovered );
    QNetworkConfiguration config;
    foreach(config, configurations) {
        qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss") << "Discovered: " << config.name();
        if (config.name() == mSSID) {
            qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss") << "Connecting to " << mConnectConfig.name();
            connectNetworkSession();
        }
    }
}
