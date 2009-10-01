#ifndef PHONENETWORKBUSINESSLOGIC_H
#define PHONENETWORKBUSINESSLOGIC_H

#include "notifier.h"
#include "systemuigconf.h"

#include <QObject>
#include <QStringList>

#include <NetworkRegistration>
#include <RadioAccess>

using namespace Cellular;

class PhoneNetworkBusinessLogic : public QObject
{
    Q_OBJECT

public:
    PhoneNetworkBusinessLogic(SystemUIGConf *systemUIGConf);
    virtual ~PhoneNetworkBusinessLogic();

    bool phoneNetworkEnabled();
    bool roamingEnabled();
    bool roamingUpdatesEnabled();    
    void setNetworkMode(const QString &value);
    void setNetworkSelection(const QString &value);
    void selectNetwork(const QString &value);
    void toggleNetwork(bool toggle);
    void toggleRoaming(bool toggle);
    void toggleRoamingUpdates(bool toggle);
    void queryNetworkModes();
    void queryNetworkSelectionValues();    
    QVariant GConfItemValue(SystemUIGConf::GConfKey key);

private slots:
    void availableNetworksReceived(bool success, const QList<AvailableOperator*> &operators, const QString &reason);

signals:
    void networkModeValuesAvailable(int, QStringList);
    void networkSelectionValuesAvailable(int, QStringList);
    void availableNetworksAvailable(int, QStringList);

private: //methods
    void queryAvailableNetworks();

private: //attributes
    SystemUIGConf *systemUIGConf;
    NetworkRegistration *networkRegistration;
    QHash<RadioAccess::Mode, QString> networkModes;
    QHash<NetworkRegistration::Mode, QString> networkSelectionValues;

};

#endif // PHONENETWORKBUSINESSLOGIC_H
