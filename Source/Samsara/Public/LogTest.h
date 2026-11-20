#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTest, Log, All);

#define LOG_HASAUTHORITY() \
UE_LOG(LogTest, Warning, TEXT("%s"), HasAuthority() ? TEXT("Server") : TEXT("Client"));

#define LOG_FUNCNAME() \
UE_LOG(LogTest, Warning, TEXT("%hs"), __FUNCTION__);

#define LOG_THISNAME() \
UE_LOG(LogTest, Warning, TEXT("%s"), *GetName());

#define LOG_SUCCESS() \
UE_LOG(LogTest, Warning, TEXT("Success"));

#define LOG_FAIL() \
UE_LOG(LogTest, Warning, TEXT("Fail"));

#define LOG_FSTRING(Str) \
UE_LOG(LogTest, Warning, TEXT("%s"), *Str);

#define LOG_ISVALID(Obj) \
UE_LOG(LogTest, Warning, TEXT("%s"), IsValid(Obj) ? TEXT("IsValid") : TEXT("IsNotValid"));

#define LOG_BOOL(b) \
UE_LOG(LogTest, Warning, TEXT(#b ": %s"), (b) ? TEXT("true") : TEXT("false"));

#define LOG_OBJNAME(Obj) \
UE_LOG(LogTest, Warning, TEXT(#Obj ": %s"), (Obj ? *Obj->GetName() : TEXT("nullptr")));

#define LOG_INT(i) \
UE_LOG(LogTest, Warning, TEXT(#i ": %d"), i);

#define LOG_FLOAT(f) \
UE_LOG(LogTest, Warning, TEXT(#f ": %f"), f);

#define LOG_VECTOR(v) \
UE_LOG(LogTest, Warning, TEXT(#v ": %s"), *v.ToString())

#define LOG_ENUM(e) \
UE_LOG(LogTest, Warning, TEXT(#e ": %s"), *UEnum::GetValueAsString(e));

#define LOG_TEXT(text) \
UE_LOG(LogTest, Warning, TEXT("%s"), TEXT(text));

#define LOG_FNAME(Name) \
UE_LOG(LogTest, Warning, TEXT("%s"), *Name.ToString())
