//Require and fetch libraries
var Particle = require('particle-api-js');
const maxApi = require("max-api");
//Create new Particle object
var particle = new Particle();
var deviceID = "3c0025000d47363433353735";
//auth token for Photon
var token = "7e12b97d44098c05725e3ea3169bb89b237dba3b";

// var eventSource = new EventSource("https://api.spark.io/v1/devices/" + deviceID + "/events/?access_token=" + accessToken);
//
// eventSource.addEventListener('open', function(e) {
//     console.log("Opened!"); },false);
//
// eventSource.addEventListener('error', function(e) {
//     console.log("Errored!"); },false);
//
//      // return eventSource
//
//      eventSource.addEventListener('Uptime', function(e) {
//        var parsedData = JSON.parse(e.data);
//        console.log('Received data', parsedData.data);
//      },false);

     //Get your devices events
particle.getEventStream({ deviceId: deviceID, auth: token }).then(function(stream) {
  stream.on('Uptime', function(data) {
    // var parsedData = JSON.parse(data.data);
    // console.log('Received data', parsedData.data);
    console.log(" ", data.data);
    maxApi.outlet(data.data);
  });
});
