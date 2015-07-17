var wincredmgr = require('./wincredmgr-win-' + require('os').arch());

var writeResult = wincredmgr.WriteCredentials('username', 'secretpassword', 'targetName');
console.log('Writing credentials successful: ' + writeResult);

var credentials = wincredmgr.ReadCredentials('targetName');
console.log(credentials.username);
console.log(credentials.password);

var deleteResult = wincredmgr.DeleteCredentials('targetName');
console.log('Deleting credentials successful: ' + deleteResult);
