var ffi = require('ffi');

exports = module.exports = ffi.Library('libchildtrace', {
  'add': [ 'void',[ 'int', 'string', 'int', 'pointer'] ],
  'detach': [ 'void',[ 'int', 'string',  'int', 'pointer'] ],
  'getsignal': [ 'void',[ 'int', 'string', 'pointer'] ],
  'sendsignal': [ 'int',[ 'int', 'int'] ],
});


var getcbptr = function (cb) {
    return  ffi.Callback('void', ['string', 'int', 'string', 'string'], cb);
};

exports.getcbptr = getcbptr;






