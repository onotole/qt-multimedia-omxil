TEMPLATE = subdirs
CONFIG += ordered
 
SUBDIRS = src/player src/plugin src/test

plugin.depends = player
test.depends = player
