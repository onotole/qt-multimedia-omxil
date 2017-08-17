var os = require('os');
var http = require('http');
var fs = require('fs');
var path = require('path');

function getIpAddress() {
    var ifaces = os.networkInterfaces();
    var ip = undefined;
    Object.keys(ifaces).every(function(ifname) {
        ifaces[ifname].every(function(iface) {
            if (!iface.internal && 'IPv4' === iface.family) {
                ip = iface.address;
                return false;
            }
            return true;
        });
        return ip === undefined;
    });
    return ip;
}

var ROOT = path.dirname(process.argv[1]);
var templates = [
                    'ui0.tmpl',
                    'ui1.tmpl',
                    'ui2.tmpl',
                    'ui3.tmpl',
                    'ui4.tmpl',
                    'ui5.tmpl'
                ];
var tmpl = 0;

function handleRequest(request, response) {
    function mime(ext) {
        switch (ext) {
            case '.htm':
            case '.html':
                return 'text/html';
            case '.js':
                return 'text/javascript';
            case '.css':
                return 'text/css';
            case '.jpg':
            case '.jpeg':
                return 'image/jpeg';
            case '.png':
                return 'image/png';
        }
        return 'text/plain';
    }
    function error(status, message) {
        response.writeHead(status, {'Content-Type': 'text/html'});
        response.end('<html><head></head><body>' + status + ': ' + message + '</body></html>');
    }
    function reply(mime, data) {
        response.writeHead(200, {'Content-Type': mime});
        response.end(data);
    }
    function file(file) {
        file = ROOT + '/' + file;
        fs.exists(file, function(exists) {
            if (exists) {
                fs.readFile(file, function(err, content) {
                    if (err) {
                        error(500, 'Interval server error: ' + err.message);
                    } else {
                        reply(mime(path.extname(file)), content);
                    }
                });
            } else {
                error(404, 'Not found');
            }
        });
    }
    function update() {
        ++tmpl;
        reply('text/plain', tmpl.toString());
    }
    switch(request.url) {
        case '/':
            file('client.html');
            break;
        case '/update':
            update();
            break;
        case '/data':
            file(templates[tmpl % templates.length]);
            break;
        default:
            file(request.url);
    }
}

var PORT = 80;
var server = http.createServer(handleRequest);

server.listen(PORT, function(){
    console.log('Server listening on: http://%s:%s', getIpAddress(), PORT);
});
