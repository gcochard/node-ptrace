var async = require('async');
var ptrace = require('../lib/ptrace');


if (process.argv.length < 4) {
  console.log('Arguments: ' + process.argv[0] + ' ' + process.argv[1] + ' <process pid>' + ' <number of retries>')
  process.exit();
}

console.log('arguments are ', parseInt(process.argv[2]), parseInt(process.argv[3]));


var cb= function(err, pid, cookie, res) {
    if (err) console.log(err);
    if (res) console.log(res);
}

var cbPtr = ptrace.getcbptr(cb);

var cookie = new Buffer("something");

var sigCb = function(err, pid, cookie, res) {
 	if (err) { 
		console.log (err);
	} else if (res) {
		// Got signal, detach from it
		console.log("Got signal ", res);
		async.series([
			console.log("Detach from the process"),
			ptrace.detach(parseInt(process.argv[2],10), cookie, parseInt(process.argv[3],10), cbPtr),
			console.log("Signalled the process."),
			ptrace.sendsignal(parseInt(process.argv[2],10), 1),
                ]);
	}
};
var sigCbPtr = ptrace.getcbptr(sigCb);

console.log("Starting to attach to the process and monitor it");
async.series([ptrace.add(parseInt(process.argv[2],10), cookie , parseInt(process.argv[3],10), cbPtr), 
               ptrace.getsignal(parseInt(process.argv[2],10), cookie, sigCbPtr)]);


