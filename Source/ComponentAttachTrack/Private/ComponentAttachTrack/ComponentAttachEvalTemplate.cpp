// Copy-right is reserved by iqiyi VR-group


#include "ComponentAttachEvalTemplate.h"
#include "ComponentAttachTrackCommon.h"
#include "ComponentAttachTrack/ComponentAttachSection.h"

struct FCachedComponentAttachTrackData final : IPersistentEvaluationData
{
	TWeakObjectPtr<class USceneComponent> SourceComponent = nullptr;
	FTransform SourceRelativeTransform;
	FName SourceSocket;
	TWeakObjectPtr<class USceneComponent> AttachComponent = nullptr;
	FDetachmentTransformRules DetachRules = FDetachmentTransformRules::KeepWorldTransform;
	bool RestoreRelativeTransformWhenFinish = true;

	static FCachedComponentAttachTrackData Get(const UObject& InObject,const UComponentAttachSection* Section)
	{
		FCachedComponentAttachTrackData TrackData;
		UObject* Object = const_cast<UObject*>(&InObject);
		TrackData.AttachComponent = Cast<USceneComponent>(Object);
		TrackData.SourceComponent = TrackData.AttachComponent->GetAttachParent();
		TrackData.SourceRelativeTransform = TrackData.AttachComponent->GetRelativeTransform();
		TrackData.SourceSocket = TrackData.AttachComponent->GetAttachSocketName();		
		TrackData.DetachRules = FDetachmentTransformRules(Section->DetachmentLocationRule, Section->DetachmentRotationRule, Section->DetachmentScaleRule, false);
		TrackData.RestoreRelativeTransformWhenFinish = Section->RestoreRelativeTransformWhenFinish;
		DebugLog(FString::Printf(TEXT("Cache TrackData Actor %s SourceRelativeTransform %s"), *TrackData.SourceComponent->GetOwner()->GetName(), *TrackData.SourceRelativeTransform.ToString()));
		return TrackData;
	}

	void RestoreState()
	{
		if (AttachComponent.IsValid() && SourceComponent.IsValid())
		{
			DebugLog(FString::Printf(TEXT("RestoreState Relative %s SourceSocket %s actor %s"), *SourceRelativeTransform.ToString(), *SourceSocket.ToString(), *SourceComponent->GetOwner()->GetName()));
			AttachComponent->DetachFromComponent(DetachRules);

			if(RestoreRelativeTransformWhenFinish)
				AttachComponent->SetRelativeTransform(FTransform::Identity);
			AttachComponent->AttachToComponent(SourceComponent.Get(), FAttachmentTransformRules::KeepRelativeTransform, SourceSocket);
			SourceComponent = nullptr;
			SourceSocket = NAME_None;
			AttachComponent = nullptr;
		}
	}
};

/** Stop audio from playing */
struct FStopComponentAttachPreAnimatedToken final : IMovieScenePreAnimatedToken
{
	FCachedComponentAttachTrackData TrackData;

	explicit FStopComponentAttachPreAnimatedToken(FCachedComponentAttachTrackData InTrackData) : TrackData(InTrackData)
	{
		
	}
	static FMovieSceneAnimTypeID GetAnimTypeID()
	{
		return TMovieSceneAnimTypeID<FStopComponentAttachPreAnimatedToken>();
	}

	virtual void RestoreState(UObject& InObject, IMovieScenePlayer& Player) override
	{
		TrackData.RestoreState();	
	}
	
	struct FProducer final : IMovieScenePreAnimatedTokenProducer
	{
		const UComponentAttachSection* Section;

		explicit FProducer(const UComponentAttachSection* InSection)
			: Section(InSection)
		{}

		virtual IMovieScenePreAnimatedTokenPtr CacheExistingState(UObject& Object) const override
		{
			DebugLog(FString::Printf(TEXT("CacheExistingState")));
			return FStopComponentAttachPreAnimatedToken(FCachedComponentAttachTrackData::Get(Object, Section));
		}
	};
};

struct FComponentAttachExecutionToken final : IMovieSceneExecutionToken
{
	const UComponentAttachSection* Section;

	explicit FComponentAttachExecutionToken(const UComponentAttachSection* InSection)
		: Section(InSection)
	{}



	virtual void Execute(const FMovieSceneContext& Context, const FMovieSceneEvaluationOperand& Operand, FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) override
	{
		const int Stat = static_cast<int>(Context.GetStatus());
		//DebugLog(FString::Printf(TEXT("FComponentAttachEvalTemplate::Execute。 Stat %d"), Stat));
		FCachedComponentAttachTrackData& TrackData = PersistentData.GetOrAddTrackData<FCachedComponentAttachTrackData>();
		if ((Context.GetStatus() != EMovieScenePlayerStatus::Playing && Context.GetStatus() != EMovieScenePlayerStatus::Scrubbing) || Context.HasJumped())
		{
			TrackData.RestoreState();
			//return;
		}
		//DebugLog(FString::Printf(TEXT("FComponentAttachEvalTemplate::Execute。2222222222222 ")));
		//const FName AttachComponentName = Section->ComponentName;
		//if (AttachComponentName == NAME_None)
		//{
		//	return;
		//}
		const FMovieSceneActorReferenceData& AttachActorData = Section->AttachActorData;

		USceneComponent* AttachTargetComponent = nullptr;
		FMovieSceneActorReferenceKey AttachKey;
		AttachActorData.Evaluate(Context.GetTime(), AttachKey);
		const FMovieSceneObjectBindingID AttachBindingID = AttachKey.Object;

		if (AttachBindingID.IsValid())
		{
			FMovieSceneSequenceID SequenceID = Operand.SequenceID;
			if (AttachBindingID.GetSequenceID().IsValid())
			{
				// Ensure that this ID is resolvable from the root, based on the current local sequence ID
				const FMovieSceneObjectBindingID RootBindingID = AttachBindingID.ResolveLocalToRoot(SequenceID, Player);
				SequenceID = RootBindingID.GetSequenceID();
			}

			// If the transform is set, otherwise use the bound actor's transform
			const FMovieSceneEvaluationOperand ObjectOperand(SequenceID, AttachBindingID.GetGuid());

			for (TWeakObjectPtr<> WeakObject : Player.FindBoundObjects(ObjectOperand))
			{ 
				AActor* AttachActor = Cast<AActor>(WeakObject.Get());
				if (AttachActor)
				{
					AttachTargetComponent = Section->GetAttachComponent(AttachActor, AttachKey);
				}
				if (AttachTargetComponent)
				{
					break;
				}
			}
		}

		if (!AttachTargetComponent)
			return;

		FName AttachSocketName = AttachKey.SocketName;
		
		for (TWeakObjectPtr<> Object : Player.FindBoundObjects(Operand))
		{
			USceneComponent* Component = Cast<USceneComponent>(Object.Get());
			if (Component)
			{
				if (!TrackData.AttachComponent.IsValid())
				{
				
					TrackData.SourceComponent = Component->GetAttachParent();
					TrackData.SourceRelativeTransform = Component->GetRelativeTransform();
					TrackData.SourceSocket = Component->GetAttachSocketName();
					TrackData.AttachComponent = Component;
					TrackData.RestoreRelativeTransformWhenFinish = Section->RestoreRelativeTransformWhenFinish;
					TrackData.DetachRules = FDetachmentTransformRules(Section->DetachmentLocationRule, Section->DetachmentRotationRule, Section->DetachmentScaleRule, false);
					DebugLog(FString::Printf(TEXT("FComponentAttachEvalTemplate::Execute Socket %s Owner %s"), *AttachTargetComponent->GetName(), *Component->GetOwner()->GetName()));
					Player.SavePreAnimatedState(*Component, FStopComponentAttachPreAnimatedToken::GetAnimTypeID(), FStopComponentAttachPreAnimatedToken::FProducer(Section));

					FAttachmentTransformRules AttachmentRules(Section->AttachmentLocationRule, Section->AttachmentRotationRule, Section->AttachmentScaleRule, false);

					Component->AttachToComponent(AttachTargetComponent, AttachmentRules, AttachSocketName);
					
				}
				const FFrameTime Time = Context.GetTime();
				const FTransform Transfrom = Section->GetTransform(Time);
				Component->SetRelativeTransform(Transfrom);
				break;
				/*
				TInlineComponentArray<USceneComponent*> PotentialAttachComponents(Actor);
				for (USceneComponent* Component : PotentialAttachComponents)
				{
				}*/
			}
		}
	}
};

FComponentAttachEvalTemplate::FComponentAttachEvalTemplate()
{
	Section = nullptr;
}

FComponentAttachEvalTemplate::FComponentAttachEvalTemplate(const UComponentAttachSection* InSection)
	: Section(InSection)
{}

void FComponentAttachEvalTemplate::Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context,
	const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const
{
	ExecutionTokens.Add(FComponentAttachExecutionToken(Section));
}

void FComponentAttachEvalTemplate::Setup(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const 
{
	if (!Section)
	{
		return;
	}
	
	if (Player.AsUObject() == nullptr || Player.AsUObject()->GetWorld() == nullptr)
	{
		return;
	}

	// 触发
	DebugLog(FString::Printf(TEXT("FComponentAttachEvalTemplate::Setup。 ")));

	const_cast<FComponentAttachEvalTemplate*>(this)->PlayerObj = Player.AsUObject();
}

void FComponentAttachEvalTemplate::TearDown(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const
{
	if (!Section)
	{
		return;
	}

	if (Player.AsUObject() == nullptr || Player.AsUObject()->GetWorld() == nullptr)
	{
		return;
	}

	const_cast<FComponentAttachEvalTemplate*>(this)->PlayerObj = nullptr;

	// 结束
	DebugLog(FString::Printf(TEXT("FComponentAttachEvalTemplate::TearDown")));
}
