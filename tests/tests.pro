TEMPLATE = subdirs
SUBDIRS = \
    ut_unlockslider \
    ut_batterybusinesslogic \
    ut_lowbatterynotifier \
    ut_networkbusinesslogic \
    ut_networktechnology \
    ut_pincodequerybusinesslogic \

QMAKE_STRIP = echo
#include(shell.pri)
#include(runtests.pri)

QMAKE_CLEAN += **/*.log.xml ./coverage.log.xml

tests_xml.path = /usr/share/system-ui-tests
tests_xml.files = tests.xml

INSTALLS += tests_xml
