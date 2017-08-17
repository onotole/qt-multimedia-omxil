TEMPLATE = aux

base.path = /usr/share/examples
base.files = $$files(*.qml) $$files(*.js) $$files(*.html) $$files(*.tmpl) $$files(*.jpg)

INSTALLS += base
