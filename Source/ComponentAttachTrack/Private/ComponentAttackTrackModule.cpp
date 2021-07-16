// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ComponentAttachTrackModule.h"
#include "ComponentAttachTrackCommon.h"

#define LOCTEXT_NAMESPACE "FSeqExModule"

void FComponentAttachTrackModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FComponentAttachTrackModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FComponentAttachTrackModule, ComponentAttachTrack)



DECLARE_LOG_CATEGORY_EXTERN(LogComponentAttach, Verbose, All);
DEFINE_LOG_CATEGORY(LogComponentAttach);

static FString AppendLogPrefix(const UObject* WorldContextObject, const FString& msg)
{
	if (WorldContextObject != nullptr)
	{
		const auto World = WorldContextObject->GetWorld();
		FString Prefix;
		if (World && World->WorldType == EWorldType::PIE)
		{
			switch (World->GetNetMode())
			{
			case NM_Client:
				Prefix = FString::Printf(TEXT("Client %d: "), GPlayInEditorID - 1);
				break;
			case NM_DedicatedServer:
			case NM_ListenServer:
				Prefix = FString::Printf(TEXT("Server: "));
				break;
			case NM_Standalone:
				break;
			default:
				break;
			}
		}

		return FString::Printf(TEXT("[%s] %s: %s"), *GetNameSafe(WorldContextObject),
			*Prefix, *msg);
	}
	return msg;
}
extern void DebugLog(const FString& Msg)
{
	DebugLog(nullptr, Msg);
}
extern void DebugLog(const UObject* WorldContextObject, const FString& Msg)
{
	const auto MsgFinal = AppendLogPrefix(WorldContextObject, Msg);

	UE_LOG(LogComponentAttach, Verbose, TEXT("%s"), *MsgFinal);
}


extern FString ToString(const FMovieSceneObjectBindingID& ID, IMovieScenePlayer& Player)
{
	for (TWeakObjectPtr<> WeakEventContext : Player.FindBoundObjects(ID.GetGuid(), ID.GetSequenceID()))
	{
		if (UObject* EventContext = WeakEventContext.Get())
		{
			return EventContext->GetFullName();
		}
	}
	return TEXT("None");
}