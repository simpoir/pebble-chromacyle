Pebble.addEventListener('showConfiguration', function(e) {
  console.log('configuring');
  //Pebble.openURL('http://similipoulet:8000/config.html');
  //Pebble.openURL('https://cdn.rawgit.com/simpoir/pebble-chromacyle/v1.1/src/js/config.html');
  Pebble.openURL('https://rawgit.com/simpoir/pebble-chromacyle/master/src/js/config.html');
});

Pebble.addEventListener('webviewclosed', function(e) {
  console.log(e.response);
  var config = JSON.parse(decodeURIComponent(e.response));
  Pebble.sendAppMessage(config);
});
