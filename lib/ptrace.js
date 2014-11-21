var ffi = require('ffi');
var path = require('path');

exports = module.exports = ffi.Library(path.resovle(__dirname,'../build','libchildtrace'), {
  'add': [ 'void',[ 'int', 'string', 'int', 'pointer'] ],
  'detach': [ 'void',[ 'int', 'string', 'int', 'pointer'] ],
  'getsignal': [ 'void',[ 'int', 'string', 'pointer'] ],
  'sendsignal': [ 'int',[ 'int', 'int'] ]
});


var getcbptr = function (cb) {
    return  ffi.Callback('void', ['string', 'int', 'string', 'string'], cb);
};
var getScbptr = function (cb) {
    return  ffi.Callback('void', ['string', 'int', 'string', 'string', 'int'], cb);
};

exports.getcbptr = getcbptr;
exports.getScbptr = getScbptr;
