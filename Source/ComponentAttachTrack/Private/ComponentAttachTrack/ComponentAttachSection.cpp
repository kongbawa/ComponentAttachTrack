// Copy-right is reserved by iqiyi VR-group


#include "ComponentAttachTrack/ComponentAttachSection.h"
#include "UObject/SequencerObjectVersion.h"
#include "Channels/MovieSceneChannelProxy.h"
#include "Evaluation/MovieScenePropertyTemplate.h"
#include "Evaluation/MovieSceneEvaluationTrack.h"

#if WITH_EDITOR
struct FComponentAttachSectionEditorData
{
	FComponentAttachSectionEditorData()
	{
		MetaData[0].SetIdentifiers("ComponentAttach", NSLOCTEXT("ComponentAttachSection", "AttachActorText", "ComponentAttachTo"));
		MetaData[0].SortOrder = 0;
		MetaData[0].Color = FCommonChannelData::RedChannelColor;
		MetaData[0].bCanCollapseToTrack = false;
	}

	FMovieSceneChannelMetaData      MetaData[1];
	//TMovieSceneExternalValue<float> ExternalValues[1];
};
#endif



UComponentAttachSection::UComponentAttachSection()
{
	EvalOptions.EnableAndSetCompletionMode
	(GetLinkerCustomVersion(FSequencerObjectVersion::GUID) < FSequencerObjectVersion::WhenFinishedDefaultsToRestoreState ?
		EMovieSceneCompletionMode::KeepState :
		GetLinkerCustomVersion(FSequencerObjectVersion::GUID) < FSequencerObjectVersion::WhenFinishedDefaultsToProjectDefault ?
		EMovieSceneCompletionMode::RestoreState :
		EMovieSceneCompletionMode::ProjectDefault);
	BlendType = EMovieSceneBlendType::Absolute;
	bSupportsInfiniteRange = true;

	FMovieSceneChannelProxyData Channels;

#if WITH_EDITOR
	static FComponentAttachSectionEditorData EditorData;
	Channels.Add(AttachActorData, EditorData.MetaData[0]);
#else

	Channels.Add(AttachActorData);

#endif

	ChannelProxy = MakeShared<FMovieSceneChannelProxy>(MoveTemp(Channels));

}

USceneComponent* UComponentAttachSection::GetAttachComponent(const AActor* InParentActor, const FMovieSceneActorReferenceKey& Key)
{
	const FName AttachComponentName = Key.ComponentName;
	const FName AttachSocketName = Key.SocketName;

	if (AttachSocketName != NAME_None)
	{
		if (AttachComponentName != NAME_None)
		{
			TInlineComponentArray<USceneComponent*> PotentialAttachComponents(InParentActor);
			for (USceneComponent* PotentialAttachComponent : PotentialAttachComponents)
			{
				if (PotentialAttachComponent->GetFName() == AttachComponentName && PotentialAttachComponent->DoesSocketExist(AttachSocketName))
				{
					return PotentialAttachComponent;
				}
			}
		}
		else if (InParentActor->GetRootComponent()->DoesSocketExist(AttachSocketName))
		{
			return InParentActor->GetRootComponent();
		}
	}
	else if (AttachComponentName != NAME_None)
	{
		TInlineComponentArray<USceneComponent*> PotentialAttachComponents(InParentActor);
		for (USceneComponent* PotentialAttachComponent : PotentialAttachComponents)
		{
			if (PotentialAttachComponent->GetFName() == AttachComponentName)
			{
				return PotentialAttachComponent;
			}
		}
	}

	if (InParentActor->GetDefaultAttachComponent())
	{
		return InParentActor->GetDefaultAttachComponent();
	}
	else
	{
		return InParentActor->GetRootComponent();
	}
}