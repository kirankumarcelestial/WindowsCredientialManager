#include <node.h>

 
#include <SDKDDKVer.h>
#include <tchar.h>
#include <Windows.h>
#include <WinCred.h>
#include <Userenv.h>
 
#include <Lmcons.h>
#include <atlbase.h>
#include <iostream>
 
 
using namespace std;
using namespace v8;

Handle<Value> WriteToWindowsCredMgr(const Arguments& args) {
    HandleScope scope;
    BOOL bRet;
    // Check that there are enough arguments. If we access an index that doesn't
    // exist, it'll be Undefined().
    if (args.Length() < 3) {
        // No argument was passed. Throw an exception to alert the user to
        // incorrect usage. Alternatively, we could just use 0.
        return ThrowException(
            Exception::TypeError(String::New("Please Specify UserName, Password & TargetName"))
        );
    }

    // Cast a value to a specific type. See
    // http://izs.me/v8-docs/classv8_1_1Value.html for available To*() functions
    // and type checking functions. When converting to integer, make sure the
    // POD type you use is big enough!
   
	v8::String::Utf8Value param1(args[0]->ToString());
	std::string strUsrName = std::string(*param1);
	
	v8::String::Utf8Value param2(args[1]->ToString());
	std::string strPasswd = std::string(*param2);
	
	v8::String::Utf8Value param3(args[2]->ToString());
	std::string strTarget = std::string(*param3);
	

	HANDLE hToken;
    bRet = OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken);
 
    if (!bRet)
       return scope.Close(Integer::New(0));
 
 
    cout << "Opened process token." << endl;
 
    PROFILEINFO profile = {};
    profile.dwSize = sizeof(profile);
    profile.lpUserName = const_cast<LPSTR>(strUsrName.c_str());
    bRet = LoadUserProfileA (hToken, &profile);
	
	
    if (!bRet)
       return scope.Close(Integer::New(0));
 
 
	CREDENTIALA credsToAdd = {};
    credsToAdd.Flags = 0;
    credsToAdd.UserName = const_cast<LPSTR>(strUsrName.c_str());
    credsToAdd.Type = CRED_TYPE_GENERIC;
    credsToAdd.TargetName = const_cast<char*>(strTarget.c_str());
    credsToAdd.CredentialBlob = (LPBYTE)strPasswd.c_str();
    credsToAdd.CredentialBlobSize = strPasswd.length(); // in bytes
    credsToAdd.Persist = CRED_PERSIST_LOCAL_MACHINE;
    bRet = CredWrite(&credsToAdd, 0);
 
    if (!bRet)
       scope.Close(Integer::New(0));
 
    wcout << L"Wrote successfully." << endl;
 

    return scope.Close(Integer::New(1));
	
//Convert String to V8	

//Handle<Value> something_else = String::New( strUsrName.c_str() );
//scope.Close(something_else);
}



Handle<Value> ReadUserNameFromWindowsCredMgr(const Arguments& args) {
    HandleScope scope;
    BOOL bRet;
    // Check that there are enough arguments. If we access an index that doesn't
    // exist, it'll be Undefined().
    if (args.Length() < 1) {
        // No argument was passed. Throw an exception to alert the user to
        // incorrect usage. Alternatively, we could just use 0.
        return ThrowException(
            Exception::TypeError(String::New("Please Specify UserName, Password & TargetName"))
        );
    }

    // Cast a value to a specific type. See
    // http://izs.me/v8-docs/classv8_1_1Value.html for available To*() functions
    // and type checking functions. When converting to integer, make sure the
    // POD type you use is big enough!
   

	
	v8::String::Utf8Value param3(args[0]->ToString());
	std::string strTarget = std::string(*param3);
	

	HANDLE hToken;
    bRet = OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken);
 
    if (!bRet)
       scope.Close(Integer::New(0));
 
 
 
	PCREDENTIALA creds;
     bRet = CredRead(strTarget.c_str(), 1, 0, &creds);
 
    if (!bRet)
       scope.Close(Integer::New(0));
 
   
    std::string password = (char const * const)creds->CredentialBlob;
    std::string usrName = (char const * const)creds->UserName;
 
	CredFree(creds);
 	
	
return scope.Close(String::New( usrName.c_str() ));
}


Handle<Value> ReadPasswordNameFromWindowsCredMgr(const Arguments& args) {
    HandleScope scope;
    BOOL bRet;
    // Check that there are enough arguments. If we access an index that doesn't
    // exist, it'll be Undefined().
    if (args.Length() < 1) {
        // No argument was passed. Throw an exception to alert the user to
        // incorrect usage. Alternatively, we could just use 0.
        return ThrowException(
            Exception::TypeError(String::New("Please Specify UserName, Password & TargetName"))
        );
    }

    // Cast a value to a specific type. See
    // http://izs.me/v8-docs/classv8_1_1Value.html for available To*() functions
    // and type checking functions. When converting to integer, make sure the
    // POD type you use is big enough!
   

	
	v8::String::Utf8Value param3(args[0]->ToString());
	std::string strTarget = std::string(*param3);
	

	HANDLE hToken;
    bRet = OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken);
 
    if (!bRet)
     return  scope.Close(Integer::New(0));
 
 
 
	PCREDENTIALA creds;
     bRet = CredRead(strTarget.c_str(), 1, 0, &creds);
 
    if (!bRet)
      return scope.Close(Integer::New(0));
 
   
    std::string password = (char const * const)creds->CredentialBlob;
    std::string usrName = (char const * const)creds->UserName;
 
	CredFree(creds);
 	
	
return scope.Close(String::New( password.c_str() ));
}


 
void RegisterModule(Handle<Object> target) {
    target->Set(String::NewSymbol("fibonacci"),
        FunctionTemplate::New(Fibonacci)->GetFunction());

	target->Set(String::NewSymbol("WriteToWindowsCredMgr"),
        FunctionTemplate::New(WriteToWindowsCredMgr)->GetFunction());

	target->Set(String::NewSymbol("ReadUserNameFromWindowsCredMgr"),
        FunctionTemplate::New(ReadUserNameFromWindowsCredMgr)->GetFunction());
				
	target->Set(String::NewSymbol("ReadPasswordNameFromWindowsCredMgr"),
        FunctionTemplate::New(ReadPasswordNameFromWindowsCredMgr)->GetFunction());
		
		
}

NODE_MODULE(credentialModule, RegisterModule);
