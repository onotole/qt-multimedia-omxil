var me;
var i = 0;
var v = [
    { "p" : null, "d" : {"x" : 0, "y" : 0,
                         "source" : "file:///usr/share/movies/big_buck_bunny_720p_surround.avi"}
    },
    { "p" : null, "d" : {"x" : 960, "y" : 0,
                         "source" : "file:///usr/share/movies/big_buck_bunny_480p_surround-fix.avi"}
    },
    { "p" : null, "d" : {"x" : 0, "y" : 540,
                         "source" : "file:///usr/share/movies/big_buck_bunny_720p_surround.avi"}
    },
    { "p" : null, "d" : {"x" : 960, "y" : 540,
                         "source" : "file:///usr/share/movies/big_buck_bunny_480p_surround-fix.avi"}
    },
    { "p" : null, "d" : {"x" : 480, "y" : 270,
                         "source" : "file:///usr/share/movies/big_buck_bunny_480p_surround-fix.avi"}
    }
];

function init_video(n) {
    v[n].p = me.createObject(root, v[n].d);
}

function load() {
    me = Qt.createComponent("Player.qml");
    init_video(i++);
}

function update() {
    var req = new XMLHttpRequest();
    req.open("GET", "http://localhost/update", true);
    req.send();
}

function click() {
    if (i < v.length) {
        init_video(i++);
    } else {
        update();
        ++i
    }
}
