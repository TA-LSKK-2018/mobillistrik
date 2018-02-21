#!/usr/bin/env node

var amqp = require('amqplib/callback_api');
var vhost = 'amqp://localhost/mobilistrik';

amqp.connect(vhost, function(err, conn) {
  conn.createChannel(function(err, ch) {
    var q = 'sensor';
	var routekey = 'mobilistrik.sensor';
    var myObj = { suhu_batt1:27.8, suhu_batt2:26.1, suhu_motor:27.5, V_batt1:12.2 };
	var msg = JSON.stringify(myObj);

    ch.assertQueue(q, {durable: true});
    // Note: on Node 6 Buffer.from(msg) should be used
    ch.publish('amq.topic', routekey, new Buffer(msg));
    console.log(" [x] Sent " +msg);
  });
  setTimeout(function() { conn.close(); process.exit(0) }, 500);
});