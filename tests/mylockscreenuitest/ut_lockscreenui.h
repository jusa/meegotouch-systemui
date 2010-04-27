/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
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
#ifndef Ut_LockScreenUIWIDGET_H
#define Ut_LockScreenUIWIDGET_H

#include <QtTest/QtTest>
#include <QObject>

class MApplication;
class MApplicationWindow;
class LockScreenUI;

class Ut_LockScreenUI : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testLockScreenUI ();

private:
    void showLockScreenUI ();
    void hideLockScreenUI ();
    
    MApplicationWindow  *m_MainWindow;
    LockScreenUI          *m_LockScreenUI;
    MApplication        *m_App;
};

#endif