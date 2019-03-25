
//Require and fetch libraries
var Particle = require('particle-api-js');
const maxApi = require("max-api");
//Create new Particle object
var particle = new Particle();
var deviceID = "3c0025000d47363433353735";
//auth token for Photon
var token;

//Login to IDE cloud
//Nest all functions inside for synchrony
particle.login({username: 'matt.halpenny@gmail.com', password: 'Or4nge9!'}).then(
  function(data) {
    //save token info
    token = data.body.access_token;
    console.log(token);

        //HTML
        // document.getElementById("uptime").innerHTML = "Waiting for data...";
        // var deviceID = "<<hex device id here>>";
        // var accessToken = "<<hex access token here>>";

        //Connect to Particle Server
        var eventSource = new EventSource("https://api.spark.io/v1/devices/" + deviceID + "/events/?access_token=" + token);

        //Add eventlistener for connection
        eventSource.addEventListener('open', function(e) {
            console.log("Opened!"); },false);

        //Error adding
        eventSource.addEventListener('error', function(e) {
            console.log("Errored!"); },false);

          //Add eventlistener for 9-DOF data stream
        eventSource.addEventListener('Uptime', function(e) {
          //parse JSON object
            var parsedData = JSON.parse(e.data);
            console.log(parsedData.coreid + " uptime: " + parsedData.data + " (h:m:s)");

        }, false);


  //
  // //  Retrieve variables from cloud
  //   particle.getVariable({ deviceId: '3c0025000d47363433353735', name: 'led1', auth: token }).then(function(data) {
  //     console.log('Device variable retrieved successfully:', data);
  //
  //     x = data.toString();
  //       maxApi.outlet(x);
  //       maxApi.post(x);
  //
  //     maxApi.outlet("0");
  //     Create an outlet for the variable in Max
  //     maxApi.addHandler("bang", (x) => {
  //         x = JSON.parse(data);
  //         maxApi.outlet(x.result);
  //         maxApi.outlet('5');
  //         maxApi.post(data.result + '0');
  //   });
  //
  //   }, function(err) {
  //     console.log('An error occurred while getting attrs:', err);
  //     maxApi.post("failed to retrieve variable");
  //   });
  },
  function (err) {
    console.log('Could not log in.', err);
  }
);
