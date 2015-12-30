var net = require('net');
require('child_process').exec('rm -f ./socket', function (error, stdout, stderr) {
  if(error != null) {
    console.log('Error when removing .socket.');
  }
  try {
    var unixServer = net.createServer(function (client) {
      client.on('data', function (data) {
        console.log('received data on unix socket');
      }); 
    });
  } catch (e) {
      console.log('failed creating unix socket server.');
  }
  try {
    unixServer.listen('./socket');
  } catch (e) {
    console.log('Error when trying to open unix socket!');
  }
});
