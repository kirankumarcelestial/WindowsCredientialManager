#include <node.h>
#include <Windows.h>
#include <WinCred.h>
#include <string>

using namespace v8;

void WriteCredentials(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    // Check that there are enough arguments. If we access an index that doesn't
    // exist, it'll be Undefined().
    if (args.Length() < 3) {
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Incorrect arguments: requires username, password, and targetName")));
        return;
    }

    if(!args[0]->IsString() || !args[1]->IsString() || !args[2]->IsString()) {
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "All arguments must be strings")));
        return;
    }

    String::Utf8Value username(args[0]->ToString());
    String::Utf8Value password(args[1]->ToString());
    String::Utf8Value targetName(args[2]->ToString());

    CREDENTIALA credsToAdd = {};
    credsToAdd.Flags = 0;
    credsToAdd.UserName = const_cast<LPSTR>(*username);
    credsToAdd.Type = CRED_TYPE_GENERIC;
    credsToAdd.TargetName = *targetName;
    credsToAdd.CredentialBlob = (LPBYTE)*password;
    credsToAdd.CredentialBlobSize = strlen(*password); // in bytes
    credsToAdd.Persist = CRED_PERSIST_LOCAL_MACHINE;
    BOOL bRet = CredWrite(&credsToAdd, 0);

    Local<Boolean> result = Boolean::New(isolate, bRet);
    args.GetReturnValue().Set(result);
}

void ReadCredentials(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    // Check that there are enough arguments. If we access an index that doesn't
    // exist, it'll be Undefined().
    if (args.Length() < 1) {
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Incorrect arguments: requires targetName")));
        return;
    }

    if(!args[0]->IsString()) {
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "All arguments must be strings")));
        return;
    }

    v8::String::Utf8Value strTarget(args[0]->ToString());

    PCREDENTIALA creds;
    BOOL bRet = CredRead(*strTarget, 1, 0, &creds);

    if (!bRet) {
        const char* errorString;

        if(GetLastError() == ERROR_NOT_FOUND) {
            errorString = "Provided targetName not found";
        }
        else {
            errorString = "Error occurred when reading credentials";
        }

        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, errorString)));
        return;
    }

    Local<Object> credentials = Object::New(isolate);
    credentials->Set(String::NewFromUtf8(isolate, "username"),
                     String::NewFromUtf8(isolate, (char const * const)creds->UserName));
    credentials->Set(String::NewFromUtf8(isolate, "password"),
                     String::NewFromUtf8(isolate, (char const * const)creds->CredentialBlob));

    args.GetReturnValue().Set(credentials);

    CredFree(creds);
}

void DeleteCredentials(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    // Check that there are enough arguments. If we access an index that doesn't
    // exist, it'll be Undefined().
    if (args.Length() < 1) {
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Incorrect arguments: requires targetName")));
        return;
    }

    if(!args[0]->IsString()) {
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "All arguments must be strings")));
        return;
    }

    v8::String::Utf8Value strTarget(args[0]->ToString());

    BOOL bRet = CredDelete(*strTarget, 1, 0);

    Local<Boolean> result = Boolean::New(isolate, bRet);
    args.GetReturnValue().Set(result);
}

void RegisterModule(Handle<Object> exports) {
    NODE_SET_METHOD(exports, "WriteCredentials", WriteCredentials);
    NODE_SET_METHOD(exports, "ReadCredentials", ReadCredentials);
    NODE_SET_METHOD(exports, "DeleteCredentials", DeleteCredentials);
}

NODE_MODULE(credentialModule, RegisterModule);
