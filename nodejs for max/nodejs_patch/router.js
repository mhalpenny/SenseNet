function route(handle, pathname, response, postData) {
  console.log("About to route a quest for " + pathname);
  if (typeof handle[pathname] === 'function') {
    handle[pathname](response, postData);
  } else {
    console.log("Handler not found for " + pathname);
  }
}

exports.route = route;