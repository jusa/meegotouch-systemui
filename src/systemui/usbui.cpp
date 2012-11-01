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
#include "usbui.h"

#include <QGraphicsLinearLayout>
#include <MLayout>
#include <MNotification>
#include <MSceneWindow>
#include <MWidget>
#include <MBasicListItem>
#include <MLabel>
#include <MLocale>
#include <MGConfItem>
#include <QTimer>
#include <QDateTime>
#include <MStylableWidget>
#include <MContainer>
#include <MImageWidget>

#ifdef HAVE_QMSYSTEM
#include <qmlocks.h>
#endif

QMap<QString, QString> UsbUi::errorCodeToTranslationID;

UsbUi::UsbUi(QObject *parent) : MDialog(),
#ifdef HAVE_QMSYSTEM
    usbMode(new MeeGo::QmUSBMode(this)),
    requestedUSBMode(MeeGo::QmUSBMode::Undefined),
    locks(new MeeGo::QmLocks(this)),
#endif
    developerMode(new MGConfItem("/Meego/System/DeveloperMode", this)),
    layout(new QGraphicsLinearLayout(Qt::Vertical)),
    chargingLabel(new MLabel()),
    massStorageItem(new MBasicListItem(MBasicListItem::SingleTitle)),
#ifdef NOKIA
    oviSuiteItem(new MBasicListItem(MBasicListItem::SingleTitle)),
    sdkItem(new MBasicListItem(MBasicListItem::SingleTitle))
#endif /* NOKIA */
    MTPItem(new MBasicListItem(MBasicListItem::SingleTitle)),
    DeveloperItem(new MBasicListItem(MBasicListItem::SingleTitle))
    
{
    setParent(parent);
    setModal(false);
    setSystem(true);
    setButtonBoxVisible(false);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Spacer above the current state
    MStylableWidget *topSpacer = new MStylableWidget;
    topSpacer->setStyleName("CommonSpacer");
    layout->addItem(topSpacer);

    // Current state: charging frame
    MImageWidget *chargingIcon = new MImageWidget("icon-m-common-usb");
    chargingIcon->setStyleName("CommonSmallMainIcon");

    chargingLabel->setStyleName("CommonSingleTitleInverted");
    chargingLabel->setWordWrap(true);

    MStylableWidget *chargingFrame = new MStylableWidget;
    chargingFrame->setStyleName("CommonTextFrameInverted");
    QGraphicsLinearLayout *chargingLayout = new QGraphicsLinearLayout(Qt::Horizontal, chargingFrame);
    chargingLayout->setContentsMargins(0, 0, 0, 0);
    chargingLayout->setSpacing(0);
    chargingLayout->addItem(chargingIcon);
    chargingLayout->setAlignment(chargingIcon, Qt::AlignCenter);
    chargingLayout->addItem(chargingLabel);
    layout->addItem(chargingFrame);

    // Spacer below the current state
    MStylableWidget *bottomSpacer = new MStylableWidget;
    bottomSpacer->setStyleName("CommonSpacer");
    layout->addItem(bottomSpacer);

    // The mode items
    massStorageItem->setStyleName("CommonSmallPanelInverted");
    connect(massStorageItem, SIGNAL(clicked()), this, SLOT(setMassStorageMode()));
    layout->addItem(massStorageItem);

#ifdef NOKIA
    oviSuiteItem->setStyleName("CommonSmallPanelInverted");
    connect(oviSuiteItem, SIGNAL(clicked()), this, SLOT(setOviSuiteMode()));
    layout->addItem(oviSuiteItem);

    sdkItem->setStyleName("CommonSmallPanelInverted");
    connect(sdkItem, SIGNAL(clicked()), this, SLOT(setSDKMode()));
    connect(developerMode, SIGNAL(valueChanged()), this, SLOT(updateSDKItemVisibility()));
    updateSDKItemVisibility();
#endif /* NOKIA */

    MTPItem->setStyleName("CommonSmallPanelInverted");
    connect(MTPItem, SIGNAL(clicked()), this, SLOT(setMTPMode()));
    layout->addItem(MTPItem);

    DeveloperItem->setStyleName("CommonSmallPanelInverted");
    connect(DeveloperItem, SIGNAL(clicked()), this, SLOT(setDeveloperMode()));
    layout->addItem(DeveloperItem);

    MWidget *centralWidget = new MWidget;
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    connect(qApp, SIGNAL(localeSettingsChanged()), this, SLOT(retranslateUi()));
    retranslateUi();

    if (errorCodeToTranslationID.isEmpty()) {
        errorCodeToTranslationID.insert("qtn_usb_filessystem_inuse", "qtn_usb_filessystem_inuse");
        errorCodeToTranslationID.insert("mount_failed", "qtn_usb_mount_failed");
    }

#ifdef HAVE_QMSYSTEM
    connect(usbMode, SIGNAL(modeChanged(MeeGo::QmUSBMode::Mode)), this, SLOT(applyUSBMode(MeeGo::QmUSBMode::Mode)));
    connect(usbMode, SIGNAL(error(const QString &)), this, SLOT(showError(const QString &)));

    // Lazy initialize to improve startup time
    QTimer::singleShot(500, this, SLOT(applyCurrentUSBMode()));
#endif
}

#ifdef NOKIA
UsbUi::~UsbUi()
{
    if (sdkItem->parentLayoutItem() == NULL) {
        delete sdkItem;
    }
}
#else
UsbUi::~UsbUi()
{
}
#endif /* NOKIA */

#ifdef HAVE_QMSYSTEM
void UsbUi::applyCurrentUSBMode()
{
    applyUSBMode(usbMode->getMode());
}
#endif

void UsbUi::showDialog()
{
    // System dialogs always create a new top level window and a scene manager so no need to worry about registering to a specific scene manager here
    appear(MSceneWindow::KeepWhenDone);

    // Tell interested parties that the dialog is now being shown
    emit dialogShown();
}

void UsbUi::hideDialog(bool acceptDialog)
{
    if (isVisible()) {
        if (acceptDialog) {
            accept();
        } else {
            reject();
        }
        disappear();
    }
}

#ifdef NOKIA
void UsbUi::setOviSuiteMode()
{
    hideDialog(true);

#ifdef HAVE_QMSYSTEM
    // Set the USB mode after a small delay to allow the dialog to close smoothly
    requestedUSBMode = MeeGo::QmUSBMode::OviSuite;
    QTimer::singleShot(100, this, SLOT(setRequestedUSBMode()));
#endif
}

void UsbUi::setSDKMode()
{
    hideDialog(true);

#ifdef HAVE_QMSYSTEM
    // Set the USB mode after a small delay to allow the dialog to close smoothly
    requestedUSBMode = MeeGo::QmUSBMode::SDK;
    QTimer::singleShot(100, this, SLOT(setRequestedUSBMode()));
#endif
}
#endif /* NOKIA */

void UsbUi::setMassStorageMode()
{
    hideDialog(true);

#ifdef HAVE_QMSYSTEM
    // Set the USB mode after a small delay to allow the dialog to close smoothly
    requestedUSBMode = MeeGo::QmUSBMode::MassStorage;
    QTimer::singleShot(100, this, SLOT(setRequestedUSBMode()));
#endif
}

void UsbUi::setMTPMode()
{ 
   hideDialog(true);

#ifdef HAVE_QMSYSTEM
    // Set the USB mode after a small delay to allow the dialog to close smoothly
    requestedUSBMode = MeeGo::QmUSBMode::MTP;
    QTimer::singleShot(100, this, SLOT(setRequestedUSBMode()));
#endif
}

void UsbUi::setDeveloperMode()
{ 
   hideDialog(true);

#ifdef HAVE_QMSYSTEM
    // Set the USB mode after a small delay to allow the dialog to close smoothly
    requestedUSBMode = MeeGo::QmUSBMode::Developer;
    QTimer::singleShot(100, this, SLOT(setRequestedUSBMode()));
#endif
}
#ifdef HAVE_QMSYSTEM
void UsbUi::setRequestedUSBMode()
{
    if (requestedUSBMode != MeeGo::QmUSBMode::Undefined) {
        usbMode->setMode(requestedUSBMode);
        requestedUSBMode = MeeGo::QmUSBMode::Undefined;
    }
}

void UsbUi::applyUSBMode(MeeGo::QmUSBMode::Mode mode)
{
    switch (mode) {
    case MeeGo::QmUSBMode::Connected:
        if (locks->getState(MeeGo::QmLocks::Device) == MeeGo::QmLocks::Locked) {
            // When the device lock is on and USB is connected, always pretend that the USB mode selection dialog is shown to unlock the touch screen lock
            emit dialogShown();
        }
        break;
    case MeeGo::QmUSBMode::Ask:
    case MeeGo::QmUSBMode::ModeRequest:
        showDialog();
        break;
    case MeeGo::QmUSBMode::Disconnected:
    case MeeGo::QmUSBMode::OviSuite:
    case MeeGo::QmUSBMode::MassStorage:
    case MeeGo::QmUSBMode::SDK:
    case MeeGo::QmUSBMode::MTP:
    case MeeGo::QmUSBMode::Developer:
        // Hide the mode selection dialog and show a mode notification
        hideDialog(false);
        showNotification(mode);
        break;
    case MeeGo::QmUSBMode::ChargingOnly:
        // no-op
        break;
    default:
        // Hide the mode selection dialog
        hideDialog(false);
        break;
    }
}

void UsbUi::showNotification(MeeGo::QmUSBMode::Mode mode)
{
    QString eventType = MNotification::DeviceAddedEvent;
    QString body;
    switch (mode) {
    case MeeGo::QmUSBMode::OviSuite:
        //% "Sync and connect in use"
        body = qtTrId("qtn_usb_sync_active");
        break;
    case MeeGo::QmUSBMode::MassStorage:
        //% "Mass storage in use"
        body = qtTrId("qtn_usb_storage_active");
        break;
    case MeeGo::QmUSBMode::SDK:
    case MeeGo::QmUSBMode::Developer:
        //% "SDK mode in use"
        body = qtTrId("qtn_usb_sdk_active");
        break;
    case MeeGo::QmUSBMode::Disconnected:
        eventType = MNotification::DeviceRemovedEvent;
        //% "USB cable disconnected"
        body = qtTrId("qtn_usb_disconnected");
        break;
    default:
        return;
    }

    MNotification notification(eventType, "", body);
    notification.publish();
}
#endif

void UsbUi::showError(const QString &errorCode)
{
    if (errorCodeToTranslationID.contains(errorCode)) {
        //% "USB connection error occurred"
        MNotification notification(MNotification::DeviceErrorEvent, "", qtTrId(errorCodeToTranslationID.value(errorCode).toUtf8().constData()));
        notification.publish();
    }
}

void UsbUi::retranslateUi()
{
    //% "Connected to USB device"
    setTitle(qtTrId("qtn_usb_connected"));

    //% "Current state: Charging only"
    chargingLabel->setText(qtTrId("qtn_usb_charging"));
    //% "Mass Storage mode"
    massStorageItem->setTitle(qtTrId("qtn_usb_mass_storage"));
#ifdef NOKIA
    //% "Ovi Suite mode"
    oviSuiteItem->setTitle(qtTrId("qtn_usb_ovi_suite"));
    //% "SDK mode"
    sdkItem->setTitle(qtTrId("qtn_usb_sdk_mode"));
#endif /* NOKIA */
    //% "Developer mode"
    DeveloperItem->setTitle(qtTrId("qtn_usb_developer_mode"));
    //% "MTP mode"
    MTPItem->setTitle(qtTrId("qtn_usb_mtp_mode"));
}

#ifdef NOKIA
void UsbUi::updateSDKItemVisibility()
{
    if (developerMode->value().toBool()) {
        if (sdkItem->parentLayoutItem() == NULL) {
            layout->addItem(sdkItem);
        }
    } else {
        if (sdkItem->parentLayoutItem() != NULL) {
            layout->removeItem(sdkItem);
            if (sdkItem->scene() != NULL) {
                sdkItem->scene()->removeItem(sdkItem);
            }
        }
    }
}
#endif /* NOKIA */
