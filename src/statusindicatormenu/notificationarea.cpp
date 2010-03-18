/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of system ui.
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

#include "sysuid.h"
#include "notificationarea.h"
#include "notificationareasink.h"
#include "duicompositornotificationsink.h"
#include <DuiInfoBanner>
#include <QTimer>

NotificationArea::NotificationArea(DuiWidget *parent) :
    DuiWidgetController(new NotificationAreaModel, parent),
    notificationAreaSink(new NotificationAreaSink)
{
    // Connect notification signals
    NotificationManager *notificationManager = &Sysuid::sysuid()->notificationManager();

    connect(&Sysuid::sysuid()->compositorNotificationSink(), SIGNAL(notificationAdded(const Notification &)), notificationAreaSink, SLOT(addNotification(const Notification &)));
    connect(notificationManager, SIGNAL(groupUpdated(uint, const NotificationParameters &)), notificationAreaSink, SLOT(addGroup(uint, const NotificationParameters &)));
    connect(notificationManager, SIGNAL(groupRemoved(uint)), notificationAreaSink, SLOT(removeGroup(uint)));
    connect(notificationManager, SIGNAL(notificationRemoved(uint)), notificationAreaSink, SLOT(removeNotification(uint)));
    connect(notificationAreaSink, SIGNAL(addNotification(DuiInfoBanner &)), this, SLOT(addNotification(DuiInfoBanner &)));
    connect(notificationAreaSink, SIGNAL(removeNotification(DuiInfoBanner &)), this, SLOT(removeNotification(DuiInfoBanner &)));
    connect(notificationAreaSink, SIGNAL(notificationRemovalRequested(uint)), notificationManager, SLOT(removeNotification(uint)));
    connect(notificationAreaSink, SIGNAL(notificationGroupClearingRequested(uint)), notificationManager, SLOT(removeNotificationsInGroup(uint)));
    connect(notificationAreaSink, SIGNAL(notificationUpdated(DuiInfoBanner &)), this, SLOT(moveNotificationToTop(DuiInfoBanner &)));
}

NotificationArea::~NotificationArea()
{
    delete notificationAreaSink;
}

void NotificationArea::moveNotificationToTop(DuiInfoBanner &notification)
{
    QList<DuiInfoBanner *> banners(model()->banners());
    if(banners.count() != 0) {
        banners.move(banners.indexOf(&notification), 0);
        model()->setBanners(banners);
    }
}

void NotificationArea::addNotification(DuiInfoBanner &notification)
{
    // Put the notification into the model of the notification area
    notification.setParentItem(this);
    QList<DuiInfoBanner *> banners(model()->banners());
    banners.push_front(&notification);
    model()->setBanners(banners);

    emit notificationCountChanged(model()->banners().count());
}

void NotificationArea::removeNotification(DuiInfoBanner &notification)
{
    // Remove the notification from the model of the notification area
    QList<DuiInfoBanner *> banners(model()->banners());
    banners.removeOne(&notification);
    model()->setBanners(banners);
    notification.setParentItem(NULL);

    emit notificationCountChanged(model()->banners().count());
}