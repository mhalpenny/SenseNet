var http = require("http");
var url = require("url");

function init(route, handle) {
  function onRequest(request, response) {
        var postedData = "";
        var pathname = url.parse(request.url).pathname;
        console.log("Request for " + pathname + " received.");
        request.addListener("data", function(trozoPosteado) {
          postedData += trozoPosteado;
          console.log("Received POST '" + trozoPosteado + "'.");
    });

    request.addListener("end", function() {
      route(handle, pathname, response, postedData);
    });

  }

  http.createServer(onRequest).listen(8888);
  console.log("Server init");
}

exports.init = init;