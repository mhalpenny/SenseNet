// Run this like:
// node livegraph.js
//
// Make sure you're in the directory containing livegraph.js and the public directory that contains the

var deviceID = "3c0025000d47363433353735";
var accessToken = "7e12b97d44098c05725e3ea3169bb89b237dba3b";

var eventSource = new EventSource("https://api.spark.io/v1/devices/" + deviceID + "/events/?access_token=" + accessToken);

//var url = require('url');
//var fs = require('fs');
//var path = require('path');
var net = require('net');

// dataPort is the TCP port we listen on from the Photon. This value is encoded in the Photon code
var dataPort = 7123;

var rcvdTotal = 0;
var rcvdPeriod = 0;

// Start a TCP Server. This is what receives data from the Particle Photon
// https://gist.github.com/creationix/707146
net.createServer(function (socket) {
	console.log('data connection started from ' + socket.remoteAddress);

	// The server sends a 8-bit byte value for each sample. Javascript doesn't really like
	// binary values, so we use setEncoding to read each byte of a data as 2 hex digits instead.
	socket.setEncoding('hex');

	socket.on('data', function (data) {
		var count = data.length / 2;
		rcvdTotal += count;
		rcvdPeriod += count;
		// console.log("got " + count + " bytes rcvdPeriod=" + rcvdPeriod + " " + data.substr(0, 36));
	});
	socket.on('end', function () {
		console.log('data connection ended');
	});
}).listen(dataPort);


setInterval(function () {
	console.log("received " + rcvdPeriod + " bytes in the last 10 seconds " + rcvdTotal + " total");
	rcvdPeriod = 0;

       // return eventSource
       eventSource.addEventListener('Uptime', function(e) {
         var parsedData = JSON.parse(e.data);
         console.log('Received data', parsedData.data);
       }

}, 1000);

console.log('listening on port ' + dataPort + ' for data');
