Pebble.addEventListener('showConfiguration', function(e) {
  console.log('configuring');
  Pebble.openURL('http://similipoulet:8000/config.html');
});

Pebble.addEventListener('webviewclosed', function(e) {
  console.log('closed config');
  console.log(e.response);
  var config = JSON.parse(decodeURIComponent(e.response));
  /*
  Pebble.sendAppMessage({
    KEY_DATE: config.date;
  });
  */
});
