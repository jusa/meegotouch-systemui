#ifndef STATUSINDICATORMENUWINDOW_STUB
#define STATUSINDICATORMENUWINDOW_STUB

#include "statusindicatormenuwindow.h"
#include <stubbase.h>

#ifdef HAVE_QMSYSTEM
#include <qmlocks.h>
#endif

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class StatusIndicatorMenuWindowStub : public StubBase {
  public:
  virtual void StatusIndicatorMenuWindowConstructor(QWidget *parent);
  virtual void StatusIndicatorMenuWindowDestructor();
  virtual void makeVisible();
  virtual void displayActive();
  virtual void displayInActive();
  virtual bool event(QEvent *);
  virtual void resetMenuWidget();
#ifdef HAVE_QMSYSTEM
  virtual void setWindowStateAccordingToTouchScreenLockState(MeeGo::QmLocks::Lock what, MeeGo::QmLocks::State how);
#endif
};

// 2. IMPLEMENT STUB
void StatusIndicatorMenuWindowStub::StatusIndicatorMenuWindowConstructor(QWidget *parent) {
  Q_UNUSED(parent);

}
void StatusIndicatorMenuWindowStub::StatusIndicatorMenuWindowDestructor() {

}
void StatusIndicatorMenuWindowStub::makeVisible() {
  stubMethodEntered("makeVisible");
}

void StatusIndicatorMenuWindowStub::displayActive() {
  stubMethodEntered("displayActive");
}

void StatusIndicatorMenuWindowStub::displayInActive() {
  stubMethodEntered("displayInActive");
}

bool StatusIndicatorMenuWindowStub::event(QEvent *event) {
  stubMethodEntered("event");
  QList<ParameterBase*> params;
  params.append( new Parameter<QEvent *>(event));
  return stubReturnValue<bool>("event");
}

void StatusIndicatorMenuWindowStub::resetMenuWidget() {
  stubMethodEntered("resetMenuWidget");
}

#ifdef HAVE_QMSYSTEM
void StatusIndicatorMenuWindowStub::setWindowStateAccordingToTouchScreenLockState(MeeGo::QmLocks::Lock what, MeeGo::QmLocks::State how) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MeeGo::QmLocks::Lock>(what));
  params.append( new Parameter<MeeGo::QmLocks::State>(how));
  stubMethodEntered("setWindowStateAccordingToTouchScreenLockState",params);
}
#endif

// 3. CREATE A STUB INSTANCE
StatusIndicatorMenuWindowStub gDefaultStatusIndicatorMenuWindowStub;
StatusIndicatorMenuWindowStub* gStatusIndicatorMenuWindowStub = &gDefaultStatusIndicatorMenuWindowStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
StatusIndicatorMenuWindow::StatusIndicatorMenuWindow(QWidget *parent) {
  gStatusIndicatorMenuWindowStub->StatusIndicatorMenuWindowConstructor(parent);
}

StatusIndicatorMenuWindow::~StatusIndicatorMenuWindow() {
  gStatusIndicatorMenuWindowStub->StatusIndicatorMenuWindowDestructor();
}

void StatusIndicatorMenuWindow::makeVisible() {
  gStatusIndicatorMenuWindowStub->makeVisible();
}

void StatusIndicatorMenuWindow::displayActive() {
  gStatusIndicatorMenuWindowStub->displayActive();
}

void StatusIndicatorMenuWindow::displayInActive() {
  gStatusIndicatorMenuWindowStub->displayInActive();
}

bool StatusIndicatorMenuWindow::event(QEvent *event) {
  return gStatusIndicatorMenuWindowStub->event(event);
}

void StatusIndicatorMenuWindow::resetMenuWidget() {
  gStatusIndicatorMenuWindowStub->resetMenuWidget();
}

#ifdef HAVE_QMSYSTEM
void StatusIndicatorMenuWindow::setWindowStateAccordingToTouchScreenLockState(MeeGo::QmLocks::Lock what, MeeGo::QmLocks::State how) {
  gStatusIndicatorMenuWindowStub->setWindowStateAccordingToTouchScreenLockState(what, how);
}
#endif

#endif
