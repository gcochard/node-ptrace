var ffi = require('ffi');

exports = module.exports = ffi.Library('./build/libchildtrace', {
  'add': [ 'void',[ 'int', 'int', 'pointer'] ],
  'detach': [ 'void',[ 'int', 'int', 'pointer'] ],
  'getsignal': [ 'void',[ 'int', 'pointer'] ],
  'sendsignal': [ 'int',[ 'int', 'int'] ],
});


var getcbptr = function (cb) {
    return  ffi.Callback('void', ['string', 'string', 'pointer'], cb);
};

exports.getcbptr = getcbptr;






