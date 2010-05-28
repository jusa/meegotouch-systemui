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
#include "unlocknotificationsink.h"
#include "notifications/genericnotificationparameterfactory.h"
#include "notifications/notificationwidgetparameterfactory.h"

// Used event-types:
#define EVENT_EMAIL "email.arrived"
#define EVENT_MSG   "x-nokia.message.arrived"
#define EVENT_CALL  "x-nokia.call"
#define EVENT_IM    "im.received"

#define DEBUG
#include "debug.h"

UnlockNotificationSink::UnlockNotificationSink () :
    m_enabled (false),
    m_emails (0),
    m_messages (0),
    m_calls (0),
    m_im (0)
{
    SYS_DEBUG ("");
}

void
UnlockNotificationSink::setLockedState (bool islocked)
{
    if (islocked == m_enabled)
        return;

    // Set the internal state
    m_enabled = islocked;

    // Clear previous missed events
    m_emails = 0;
    m_messages = 0;
    m_calls = 0;
    m_im = 0;

    // Emit the notification-count update signal,
    // in this way ui can hide the previous ones...
    emit updateNotificationsCount (m_emails, m_messages, m_calls, m_im);
}

bool
UnlockNotificationSink::canAddNotification (const Notification &notification)
{
    // Not locked state... skip
    if (m_enabled == false)
        return false;

    bool retval = false;

    QString event_type =
        notification.parameters ().value (
            GenericNotificationParameterFactory::eventTypeKey ()).toString ();

    if ((event_type == EVENT_EMAIL) ||
        (event_type == EVENT_MSG)   ||
        (event_type == EVENT_CALL)  ||
        (event_type == EVENT_IM))
        retval = true;

    return retval;
}

void
UnlockNotificationSink::addNotification (const Notification &notification)
{
#if 0
    // Debug (verbose)

    SYS_DEBUG ("group-id = %d", notification.groupId ());
    SYS_DEBUG ("user-id = %d", notification.userId ());

    SYS_DEBUG ("event-type = \"%s\"", SYS_STR(notification.parameters ().value 
        (GenericNotificationParameterFactory::eventTypeKey ()).toString ()));
    SYS_DEBUG ("class = \"%s\"", SYS_STR(notification.parameters ().value 
        (GenericNotificationParameterFactory::classKey ()).toString ()));

    if (! notification.parameters ().value 
           (NotificationWidgetParameterFactory::actionKey ()).isNull ())
        SYS_DEBUG ("action = \"%s\"", SYS_STR(notification.parameters ().value 
           (NotificationWidgetParameterFactory::actionKey ()).toString ()));
    else
        SYS_DEBUG ("action = {empty} [[[no action set]]]");

    if (! notification.parameters ().value 
           (NotificationWidgetParameterFactory::bodyKey ()).isNull ())
        SYS_DEBUG ("body = \"%s\"", SYS_STR(notification.parameters ().value 
           (NotificationWidgetParameterFactory::bodyKey ()).toString ()));
    else
        SYS_DEBUG ("body = {empty} [[[no action set]]]");
#endif
    QString event_type =
        notification.parameters ().value (
            GenericNotificationParameterFactory::eventTypeKey ()).toString ();

    if (event_type == EVENT_EMAIL)
        m_emails++;
    else if (event_type == EVENT_MSG)
        m_messages++;
    else if (event_type == EVENT_CALL)
        m_calls++;
    else if (event_type == EVENT_IM)
        m_im++;
    else
    {
        SYS_WARNING ("What about event-type: \"%s\" ?", SYS_STR (event_type));
        return;
    }

    emit updateNotificationsCount (m_emails, m_messages, m_calls, m_im);
}

void
UnlockNotificationSink::removeNotification (uint notificationId)
{
    Q_UNUSED (notificationId);

    // No operation here...
    // FIXME: do i need to do anything here?
}
