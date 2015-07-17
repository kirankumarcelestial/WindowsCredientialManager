var wincredmgr = require('./credentialModule');

console.log('Writing credentials successful: ' + wincredmgr.WriteCredentials('username','secretpassword','targetName'));

var credentials = wincredmgr.ReadCredentials('targetName');
console.log(credentials.username);
console.log(credentials.password);
