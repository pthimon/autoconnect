/*
 * autoconnect.h
 *
 * Automatically connect to the network specified in argv[1]
 * with the interval argv[2] seconds.
 *
 *  Created on: 06-Jun-2010
 *      Author: Simon Butler
 */

#ifndef AUTOCONNECT_H
#define AUTOCONNECT_H

#include <QObject>
#include <QNetworkConfigurationManager>
#include <QNetworkSession>
#include <QTimer>

QTM_USE_NAMESPACE

class AutoConnect : public QObject
{
    Q_OBJECT
public:
    explicit AutoConnect();
    bool init();

signals:

public slots:
    void onlineStateChanged(bool isOnline);
    void onError(QNetworkSession::SessionError error);
    void onUpdateCompleted();
    void startScan();
    void onConnected();

protected:
    void connectNetworkSession();

private:
    QNetworkConfigurationManager mManager;
    QNetworkConfiguration mConnectConfig;
    QNetworkSession *mSession;
    QTimer *mTimer;
    QString mSSID;
    int mTimerInterval;
};

#endif // AUTOCONNECT_H
