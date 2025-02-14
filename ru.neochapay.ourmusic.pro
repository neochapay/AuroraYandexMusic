TEMPLATE = subdirs

SUBDIRS = libs src

OTHER_FILES += \
    LICENSE \
    README.md \
    ru.neochapay.ourmusic.desktop_nojail \
    rpm/*.spec

CONFIG += ordered
