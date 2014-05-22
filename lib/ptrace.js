var ffi = require('ffi');

exports = module.exports = ffi.Library('./build/libchildtrace', {
  'add': [ 'void',[ 'int', 'pointer', 'int', 'pointer'] ],
  'detach': [ 'void',[ 'int', 'pointer',  'int', 'pointer'] ],
  'getsignal': [ 'void',[ 'int', 'pointer', 'pointer'] ],
  'sendsignal': [ 'int',[ 'int', 'int'] ],
});


var getcbptr = function (cb) {
    return  ffi.Callback('void', ['string', 'int', 'pointer', 'pointer'], cb);
};

exports.getcbptr = getcbptr;






