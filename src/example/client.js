var overlay;

function update(frag) {
    if (frag != overlay.innerHTML)
        overlay.innerHTML = frag;
}

function check() {
    setTimeout(function(){
        var req = new XMLHttpRequest();
        req.onreadystatechange = function () {
            if (req.readyState == XMLHttpRequest.DONE) {
                if (req.status == 200) {
                    update(req.responseText);
                }
            }
        };
        req.open("GET", "data", true);
        req.send();

        check();
    }, 500);
}

function init() {
    overlay = document.getElementById("overlay");
    check();
}
