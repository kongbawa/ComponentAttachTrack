// Copy-right is reserved by iqiyi VR-group

#pragma once

#include "CoreMinimal.h"
#include "MovieSceneSection.h"
#include "MovieSceneKeyStruct.h"
#include "Channels/MovieSceneIntegerChannel.h"
#include "Sections/MovieSceneActorReferenceSection.h"
#include "ComponentAttachSection.generated.h"

/**
 * 
 */
UCLASS()
class COMPONENTATTACHTRACK_API UComponentAttachSection final : public UMovieSceneSection
{
	GENERATED_BODY()

public:
	UComponentAttachSection();

public:
	static FString GetTitle() { return TEXT("ComponentAttach"); }

	static USceneComponent* GetAttachComponent(const AActor* InParentActor, const FMovieSceneActorReferenceKey& Key);
public:
	UPROPERTY()
	FMovieSceneActorReferenceData AttachActorData;
};
