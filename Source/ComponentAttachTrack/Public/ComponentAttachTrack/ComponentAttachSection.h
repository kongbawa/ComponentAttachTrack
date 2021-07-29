// Copy-right is reserved by iqiyi VR-group

#pragma once

#include "CoreMinimal.h"
#include "MovieSceneSection.h"
#include "MovieSceneKeyStruct.h"
#include "Channels/MovieSceneIntegerChannel.h"
#include "Sections/MovieSceneActorReferenceSection.h"
#include "Channels/MovieSceneFloatChannel.h"
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
	FString GetTitle() { return TEXT("ComponentAttach"); }

	USceneComponent* GetAttachComponent(const AActor* InParentActor, const FMovieSceneActorReferenceKey& Key) const;

	FVector GetTranslation(FFrameTime Time) const;
	FRotator GetRotation(FFrameTime Time) const;
	FVector GetScale(FFrameTime Time) const;
	FTransform GetTransform(FFrameTime Time) const;
public:
	UPROPERTY()
	FMovieSceneActorReferenceData AttachActorData;

	/** Translation curves */
	UPROPERTY()
		FMovieSceneFloatChannel Translation[3];

	/** Rotation curves */
	UPROPERTY()
		FMovieSceneFloatChannel Rotation[3];

	/** Scale curves */
	UPROPERTY()
		FMovieSceneFloatChannel Scale[3];

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attach")
		EAttachmentRule AttachmentLocationRule;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attach")
		EAttachmentRule AttachmentRotationRule;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attach")
		EAttachmentRule AttachmentScaleRule;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attach")
		EDetachmentRule DetachmentLocationRule;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attach")
		EDetachmentRule DetachmentRotationRule;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attach")
		EDetachmentRule DetachmentScaleRule;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attach")
		bool RestoreRelativeTransformWhenFinish;
};
