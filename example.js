var cm = require('./build/Release/credentialModule');

console.warn(cm.WriteToWindowsCredMgr('Kiran','kumar','test'));
console.warn(cm.ReadUserNameFromWindowsCredMgr('test'));
console.warn(cm.ReadPasswordNameFromWindowsCredMgr('test'));
