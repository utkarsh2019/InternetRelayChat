QT += widgets

HEADERS     = dialog.h \
    userinfo.h \
    newaccount.h \
    login.h \
    roominfo.h \
    talk_client.h

SOURCES     = dialog.cpp \
              main.cpp \
    userinfo.cpp \
    newaccount.cpp \
    login.cpp \
    roominfo.cpp \
    talk_client.cpp



# install

target.path = $$[QT_INSTALL_EXAMPLES]/widgets/layouts/basiclayouts

INSTALLS += target

