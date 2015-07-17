#  WindowsCredentialManager

## An implementation of Store / Retrieve / Delete username and passwords to/from the Windows Credential Manager.

Windows Credential Manager allows you to store credentials, such as usernames and passwords that you use to log on to websites or other computers on a network. By storing your credentials, Windows can automatically log you on to websites or other computers. Credentials are saved in special folders on your computer called vaults. Windows and programs (such as web browsers) can securely give the credentials in the vaults to other computers and websites locally.

I could not find any npm module that could do this, so ended up writing one. though there is a WinRT version avaliable [windows.security.credentials.ui](https://www.npmjs.com/package/windows.security.credentials.ui). but cannot be used for user defined credentials in windows Credential Manager.


##Installation

    $ npm install wincredmgr


##To build the Package

To build, you must have nodejs, npm, and node-gyp installed. Furthermore, node-gyp requires a C++ compiler and Python 2.7.3 or higher installed.

    $ npm install node-gyp
    $ node-gyp rebuild

If you have problem configuring Node-gyp with Python, I have a already built module (wincredmgr.node) that can be directly copied (wincredmgr.node and wincredmgr-x86.node) and used as shown in example.js


##Usage

    var wincredmgr = require('wincredmgr');

    var writeResult = wincredmgr.WriteCredentials('username', 'secretpassword', 'targetName');
    console.log('Writing credentials successful: ' + writeResult);

    var credentials = wincredmgr.ReadCredentials('targetName');
    console.log(credentials.username);
    console.log(credentials.password);

    var deleteResult = wincredmgr.DeleteCredentials('targetName');
    console.log('Deleting credentials successful: ' + deleteResult);
