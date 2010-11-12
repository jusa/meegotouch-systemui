/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#include "shutdownbusinesslogic.h"
#include "shutdownui.h"
#include "sysuid.h"
#include <signal.h>

#include <MApplication>
#include <MFeedback>
#include <MSceneManager>
#include <MNotification>
#include <MLocale>

extern sighandler_t originalSigIntHandler;

ShutdownBusinessLogic::ShutdownBusinessLogic(QObject *parent) :
    QObject(parent),
    shutdownUi(NULL)
{
#ifdef HAVE_QMSYSTEM
    m_State = new MeeGo::QmSystemState(this);
    connect(m_State, SIGNAL(systemStateChanged(MeeGo::QmSystemState::StateIndication)),
        this, SLOT(systemStateChanged(MeeGo::QmSystemState::StateIndication)));
#endif
}

ShutdownBusinessLogic::~ShutdownBusinessLogic ()
{
    if (shutdownUi) {
        shutdownUi->deleteLater();
        shutdownUi = 0;
    }
}

void  ShutdownBusinessLogic::showUI (QString  text1, QString  text2, int timeout)
{

    if (NULL == shutdownUi) {
        shutdownUi = new ShutdownUI();
        shutdownUi->showWindow(text1, text2, timeout);
    }
}

#ifdef HAVE_QMSYSTEM

void ShutdownBusinessLogic::systemStateChanged(MeeGo::QmSystemState::StateIndication what)
{
    switch (what) {
        case MeeGo::QmSystemState::Shutdown:
            // To avoid early quitting on shutdown...
            signal(SIGINT, originalSigIntHandler);
            showUI();
            break;

        case MeeGo::QmSystemState::ThermalStateFatal:
            thermalShutdown();
            break;

        case MeeGo::QmSystemState::ShutdownDeniedUSB:
            shutdownDeniedUSB();
            break;

        case MeeGo::QmSystemState::BatteryStateEmpty:
            batteryShutdown();
            break;

        default:
            break;
    }
}
#endif

void ShutdownBusinessLogic::createAndPublishNotification(const QString &type, const QString &summary, const QString &body)
{
    MNotification notification(type, summary, body);
    notification.publish ();
}

void ShutdownBusinessLogic::thermalShutdown ()
{
    //% "Temperature too high. Device shutting down."
    QString body(qtTrId ("qtn_shut_high_temp"));
    createAndPublishNotification("x-nokia.battery.temperature","", body);
}

void ShutdownBusinessLogic::batteryShutdown ()
{
    //% "Battery empty. Device shutting down."
    QString body(qtTrId ("qtn_shut_batt_empty"));
    createAndPublishNotification("x-nokia.battery.shutdown", "", body);
}

void ShutdownBusinessLogic::shutdownDeniedUSB ()
{
    //% "USB cable plugged in. Unplug the USB cable to shutdown."
    QString body(qtTrId ("qtn_shut_unplug_usb"));
    createAndPublishNotification(MNotification::DeviceAddedEvent, "", body);

    MFeedback feedback("IDF_INFORMATION_SOUND");
    feedback.play();
}

ShutdownBusinessLogicAdaptor::ShutdownBusinessLogicAdaptor (QObject *parent, ShutdownBusinessLogic *logic) :
    QDBusAbstractAdaptor(parent),
    m_logic(logic)
{
}

void ShutdownBusinessLogicAdaptor::showScreen(QString text1, QString text2, int timeout)
{
    m_logic->showUI (text1, text2, timeout);
}
