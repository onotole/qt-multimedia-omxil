TEMPLATE = aux

OTHER_FILES = $$files(*.qml) $$files(*.js)

base.path = /usr/share/examples
base.files = $$files(*.qml) $$files(*.js)

INSTALLS += base
