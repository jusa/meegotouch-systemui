#include "slidercontainer.h"
#include "batterytranslation.h"

#include <DuiButton>
#include <DuiGridLayoutPolicy>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiSlider>

SliderContainer::SliderContainer(DuiWidget *parent) :
        DuiContainer(parent),
        PSMAutoButton(NULL),
        PSMSlider(NULL)
{
    setLayout();
}

SliderContainer::~SliderContainer()
{
}

void SliderContainer::setLayout()
{
    PSMAutoButton = new DuiButton();
    PSMAutoButton->setCheckable(true);
    PSMAutoButton->setObjectName("PSMAutoButton");    
    PSMSlider = new DuiSlider(this, "continuous");

    connect(PSMAutoButton, SIGNAL(toggled()), this, SLOT(PSMAutoButtonToggled()));
    connect(PSMSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanegd(int)));

    DuiLayout *layout = new DuiLayout();
    layoutPolicy = new DuiGridLayoutPolicy(layout);
    layoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::PSMAutoActivateText), 0, 0);
    layoutPolicy->addItemAtPosition(PSMAutoButton, 0, 1);
    layoutPolicy->setRowMaximumHeight(1, 75);

    centralWidget()->setLayout(layout);
}

void SliderContainer::initSlider(const QStringList &values)
{
    sliderValues = QStringList(values);
    PSMSlider->setRange(0,sliderValues.size()-1);
    PSMSlider->setOrientation(Qt::Horizontal);
}

void SliderContainer::updateSlider(const QString &value)
{    
    PSMSlider->setValue(sliderValues.indexOf(value)); //in case this is the first call, we need to set the value
    PSMSlider->setThumbLabel(QString("%1%").arg(value));
}

void SliderContainer::sliderValueChanged(int value)
{        
    updateSlider(sliderValues.at(value));
    emit PSMThresholdValueChanged(sliderValues.at(value));
}

void SliderContainer::toggleSliderExistence()
{
    if(PSMAutoButton->isChecked()) {
        if(layoutPolicy->itemAt(1, 0) != PSMSlider)
            layoutPolicy->addItemAtPosition(PSMSlider, 1, 0, 1, 2);
    }
    else {
        if(layoutPolicy->itemAt(1, 0) == PSMSlider)
            layoutPolicy->removeItem(PSMSlider);
    }
}

void SliderContainer::initPSMAutoButton(bool toggle)
{
    PSMAutoButton->setChecked(toggle);
    toggleSliderExistence();
}

void SliderContainer::PSMAutoButtonToggled()
{
    toggleSliderExistence();
    emit PSMToggled(PSMAutoButton->isChecked());
}
