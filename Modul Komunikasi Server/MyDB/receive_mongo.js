#!/usr/bin/env node

var MongoClient = require('mongodb').MongoClient;
var url = "mongodb://localhost:27017/";
var amqp = require('amqplib/callback_api');
var vhost = 'amqp://localhost/mobilistrik';

//Connect to the amqp broker
amqp.connect(vhost, function(err, conn) {
  conn.createChannel(function(err, ch) {
    var q = 'sensor';
	var routekey = 'mobilistrik.sensor';

    ch.assertQueue(q, {durable: true});
    console.log(" [*] Waiting for messages in %s. To exit press CTRL+C", q);

    ch.consume(q, function(msg) {
      console.log("Incoming msg : "+msg.content.toString());
	  var myobj = JSON.parse(msg.content.toString());
	  
	  //Connect to the database and insert consumed data as a document
	  MongoClient.connect(url, function(err, db) {
		if (err) throw err;
		var dbo = db.db("mydb");
		dbo.collection("sensors").insertOne(myobj, function(err, result) {
			if (err) throw err;
			console.log("1 document inserted.");
			});
		});	  
    }, {noAck: true});
  });
});