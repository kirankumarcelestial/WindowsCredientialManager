###  WindowsCredentialManager
     =========================
### An Implementation of Store / Retrieve Passwords to/From windows Credential Store.

Windows Credential Manager allows you to store credentials, such as usernames and passwords that you use to log on to websites or other computers on a network. By storing your credentials, Windows can automatically log you on to websites or other computers. Credentials are saved in special folders on your computer called vaults. Windows and programs (such as web browsers) can securely give the credentials in the vaults to other computers and websites locally.

I could not find any npm module that could do this, so ended up writing one. though there is a WinRT version avaliable [windows.security.credentials.ui](https://www.npmjs.com/package/windows.security.credentials.ui). but cannot be used for user defined credentials in windows Credential Manager.


###Installation
   ============

$ npm install WinCredMgr


###To Rebuild the Package
 =========================

$ node-gyp build

If you have problem configuring Node-gyp with Python, I have a already built module [Build Folder] that can be directly copied and used as shown in example.js

###Usage
   =====

var wincredmgr = require('wincredmgr');

console.log('Writing credentials successful: ' + wincredmgr.WriteCredentials('username','password','targetName')

var credentials = wincredmgr.ReadCredentials('targetName');
console.log(credentials.username);
console.log(credentials.password);
