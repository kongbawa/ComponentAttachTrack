
#pragma once

#include "CoreMinimal.h"
#include "Engine.h"


#include "Evaluation/MovieSceneEvalTemplate.h"

extern void DebugLog(const FString& Msg);
extern void DebugLog(const UObject* WorldContextObject, const FString& Msg);

extern FString ToString(const FMovieSceneObjectBindingID& ID, IMovieScenePlayer& Player);