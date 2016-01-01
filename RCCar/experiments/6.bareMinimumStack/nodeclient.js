var net = require('net');
var child_process = require('child_process');
var counter = 0;
child_process.exec('rm -f ./socket', function (error, stdout, stderr) {
    console.log('stdout: ' + stdout);
    console.log('stderr: ' + stderr);
    if (error !== null) {
      console.log('exec error: ' + error);
    }
    // This server listens on a Unix socket at /var/run/mysocket
    try {
            var unixServer = net.createServer(function(client) {
            
            // Do something with the client connection
            client.on('data', function (data) {console.log("Received something:"); console.log(data)});
            setInterval(function() {
                console.log('writing');
                client.write('a\n');
            }, 5000);
        });
    } catch (e) {console.log("failed creating server");}
    try {
        unixServer.listen('./socket');
    } catch (e) {
        console.log("Error when trying to listen!");
    }
});

    
