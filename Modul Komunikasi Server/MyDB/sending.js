#!/usr/bin/env node

var amqp = require('amqplib/callback_api');
var vhost = 'amqp://localhost//mobillistrik';

amqp.connect(vhost, function(err, conn) {
  conn.createChannel(function(err, ch) {
    var q = 'dataSensor';
	var routekey = 'mobillistrik.sensor';
    var myObj = {"time":"0:25:6" , "date":"1/1/1970" , "lat":"0.000000" , "lon":"0.000000" , "suhu_bat1":"26.81" , "suhu_bat2":"27.75" , "suhu_motor":"27.56" , "v_batt":"3.42", "soc":"100", "arus_dc":"-4.22" , "arus_ac":"0.02" , "rpm_motor":"0" , "torsi_motor":"0.00", "id_ind":"1"};
	var msg = JSON.stringify(myObj);

    ch.assertQueue(q, {durable: true});
    // Note: on Node 6 Buffer.from(msg) should be used
    ch.publish('amq.topic', routekey, new Buffer(msg));
    console.log(" [x] Sent " +msg);
  });
  setTimeout(function() { conn.close(); process.exit(0) }, 500);
});
