var server = require("./server");
var router = require("./router");
var requestHandlers = require("./requestHandlers");

var handle = {}
handle["/nodecomputation"] = requestHandlers.nodecomputation;

server.init(router.route, handle);