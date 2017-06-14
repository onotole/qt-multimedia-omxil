TEMPLATE = subdirs
CONFIG += ordered
 
SUBDIRS = src/player src/plugin src/test src/example

plugin.depends = player
test.depends = player
