net = require('net');
console.log("Hello world!");

server = net.createServer(function(c) {
	console.log('client connected');
	c.on('end', function() {
		console.log('Client disconnected');
	});
	
});
//server.listen
