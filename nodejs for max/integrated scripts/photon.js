//Require and fetch libraries
const maxApi = require("max-api");
var Particle = require('particle-api-js');

//Create new Particle object
var particle_A = new Particle();
var particle_B = new Particle();
//Photon credentials
var deviceID_A = "3c0025000d47363433353735";
var token_A = "7e12b97d44098c05725e3ea3169bb89b237dba3b";
//Photon credentials
// var deviceID_B = "3a004f000f51373331333230";
// var token_B = "fda54915211d644dab587f37de978a61771e8322";


//
// var devicesPr = particle_A.listDevices({ auth: token_A });
//
// devicesPr.then(
//   function(devices){
//     console.log('Devices: ', devices);
//   },
//   function(err) {
//     console.log('List devices call failed: ', err);
//   }
// );

// Max.post("hello world!");
//      //Get your devices events
particle_A.getEventStream({ deviceId: deviceID_A, auth: token_A }).then(function(stream) {
  stream.on('UptimeA', function(data) {
    // var parsedData = JSON.parse(data.data);
    console.log("A: ", data.data);
    // Max.outlet(data.data);

    // Use the 'addHandler' function to register a function for a particular message
    maxApi.outlet(data.data);
    maxApi.post(data.data);


  });
});



//Get your devices events
// particle_B.getEventStream({ deviceId: deviceID_B, auth: token_B }).then(function(stream) {
// stream.on('Uptime', function(data) {
// // var parsedData = JSON.parse(data.data);
// // console.log('Received data', parsedData.data);
// console.log("B: ", data.data);
// Max.outlet(data.data);
//
// });
// });
