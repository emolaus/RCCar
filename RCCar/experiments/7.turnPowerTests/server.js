var net = require('net');
var child_process = require('child_process');
var express = require('express');

var unixSocketClient;

// Remove old socket and open new
child_process.exec('rm -f ./socket', function (error, stdout, stderr) {
    if (error !== null) {
      console.log('exec error: ' + error);
    }
    try {
      var unixServer = net.createServer(function(client) {
        unixSocketClient = client;
        unixSocketClient.setEncoding('ascii');
      // Do something with the client connection
      //client.on('data', function (data) {console.log("Received something:"); console.log(data)});
      //setInterval(function() {
      //    console.log('writing');
      //    client.write('a\n');
      //}, 5000);
      });
    } catch (e) {console.log("failed creating server");}
    try {
        unixServer.listen('./socket');
    } catch (e) {
        console.log("Error when trying to listen!");
    }
    console.log('Unix socket server listening on .socket');
});

var app = express();

app.get('/command/:command', function (req,res) {
  var intCommand = parseInt(req.params.command);
  var bufArray = [0,0,0,0,0];
  bufArray[3] = intCommand % 10;
  bufArray[2] = (intCommand - intCommand % 10)/10 % 10;
  bufArray[1] = (intCommand - intCommand % 100)/100 % 10;
  bufArray[0] = (intCommand - intCommand % 1000)/1000 % 10;
  bufArray[4] = '\0';
  var buffer = new Buffer(bufArray);
  if (unixSocketClient) unixSocketClient.write(buffer);
  res.send(req.params.command);
});

app.use(express.static(__dirname + '/webpage'));

var httpServer = app.listen(80, function () {
  var host = httpServer.address().address;
  var port = httpServer.address().port;
  console.log('HTTP server listening at http://%s:%s', host, port);
});

    
 
