#!/usr/bin/env node

var MongoClient = require('mongodb').MongoClient;
var url = "mongodb://localhost:27017/";
var amqp = require('amqplib/callback_api');
var vhost = 'amqp://localhost//mobillistrik';
//'amqp://mobillistrik:mobillistrik@167.205.66.196:5672/%2fmobillistrik?heartbeat=10'

//Connect to the amqp broker
amqp.connect(vhost, function(err, conn) {
  conn.createChannel(function(err, ch) {
    var q = 'dataSensor';
	var routekey = 'mobillistrik.sensor';

    ch.assertQueue(q, {durable: true});
    console.log(" [*] Waiting for messages in %s. To exit press CTRL+C", q);

    ch.consume(q, function(msg) {
      console.log("Incoming msg : "+msg.content.toString());
	  
	  var myobj = JSON.parse(msg.content.toString());
	  
	  var buffer = {};
	  var id_ref = 1;
	  var id_in = parseInt(myobj.id_ind.toString(), 10);
	  
	  for (var dataLength = 0; dataLength < 4; dataLength++) {
		  if (id_ref == id_in) {
			buffer += myobj + " ";
		  }
	  }
	  
	  id_ref += 1;
	  
	  for (var i = 0; i < myobj.length; i++) {
			console.log(buffer[i]);
	  }
	  	
		//Connect to the database and insert consumed data as a document	
		MongoClient.connect(url, function(err, db) {
		var dbo = db.db("EVOI");
		
		if (err) throw err;
		
			dbo.collection("sensors").insertOne(myobj, function(err, result) {
				if (err) throw err;
				console.log("1 document inserted.");
			});
			
		/*	//If existing document has the same id_ind as the received message, then push the message into the document
			var query = { id_ind: { $eq: myobj.id_ind } };
			var newvalues = { $push: myobj };
			
			dbo.collection("sensors").updateOne( query, newvalues, function(err, res) {
				if (err) throw err;
				console.log("1 document updated.");
			});
		*/
	    });
    }, {noAck: true});
  });
});
