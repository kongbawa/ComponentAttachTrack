// Copy-right is reserved by iqiyi VR-group


#include "ComponentAttachTrack/ComponentAttachSection.h"
#include "UObject/SequencerObjectVersion.h"
#include "Channels/MovieSceneChannelProxy.h"
#include "Evaluation/MovieScenePropertyTemplate.h"
#include "Evaluation/MovieSceneEvaluationTrack.h"
#include "ComponentAttachTrackCommon.h"

#if WITH_EDITOR
struct FComponentAttachSectionEditorData
{
	FComponentAttachSectionEditorData()
	{
		MetaData[0].SetIdentifiers("ComponentAttach", NSLOCTEXT("ComponentAttachSection", "AttachActorText", "ComponentAttachTo"));
		MetaData[0].SortOrder = 0;
		MetaData[0].Color = FCommonChannelData::RedChannelColor;
		MetaData[0].bCanCollapseToTrack = false;

		FText LocationGroup = NSLOCTEXT("MovieSceneTransformSection", "Location", "Location");
		FText RotationGroup = NSLOCTEXT("MovieSceneTransformSection", "Rotation", "Rotation");
		FText ScaleGroup = NSLOCTEXT("MovieSceneTransformSection", "Scale", "Scale");
		{
			TransformMetaData[0].SetIdentifiers("Location.X", FCommonChannelData::ChannelX, LocationGroup);
			//MetaData[0].bEnabled = EnumHasAllFlags(Mask, EMovieSceneTransformChannel::TranslationX);
			TransformMetaData[0].Color = FCommonChannelData::RedChannelColor;
			TransformMetaData[0].SortOrder = 0;
			TransformMetaData[0].bCanCollapseToTrack = false;

			TransformMetaData[1].SetIdentifiers("Location.Y", FCommonChannelData::ChannelY, LocationGroup);
			//MetaData[1].bEnabled = EnumHasAllFlags(Mask, EMovieSceneTransformChannel::TranslationY);
			TransformMetaData[1].Color = FCommonChannelData::GreenChannelColor;
			TransformMetaData[1].SortOrder = 1;
			TransformMetaData[1].bCanCollapseToTrack = false;

			TransformMetaData[2].SetIdentifiers("Location.Z", FCommonChannelData::ChannelZ, LocationGroup);
			//MetaData[2].bEnabled = EnumHasAllFlags(Mask, EMovieSceneTransformChannel::TranslationZ);
			TransformMetaData[2].Color = FCommonChannelData::BlueChannelColor;
			TransformMetaData[2].SortOrder = 2;
			TransformMetaData[2].bCanCollapseToTrack = false;
		}
		{
			TransformMetaData[3].SetIdentifiers("Rotation.X", NSLOCTEXT("MovieSceneTransformSection", "RotationX", "Roll"), RotationGroup);
			//MetaData[3].bEnabled = EnumHasAllFlags(Mask, EMovieSceneTransformChannel::RotationX);
			TransformMetaData[3].Color = FCommonChannelData::RedChannelColor;
			TransformMetaData[3].SortOrder = 3;
			TransformMetaData[3].bCanCollapseToTrack = false;

			TransformMetaData[4].SetIdentifiers("Rotation.Y", NSLOCTEXT("MovieSceneTransformSection", "RotationY", "Pitch"), RotationGroup);
			//MetaData[4].bEnabled = EnumHasAllFlags(Mask, EMovieSceneTransformChannel::RotationY);
			TransformMetaData[4].Color = FCommonChannelData::GreenChannelColor;
			TransformMetaData[4].SortOrder = 4;
			TransformMetaData[4].bCanCollapseToTrack = false;

			TransformMetaData[5].SetIdentifiers("Rotation.Z", NSLOCTEXT("MovieSceneTransformSection", "RotationZ", "Yaw"), RotationGroup);
			//MetaData[5].bEnabled = EnumHasAllFlags(Mask, EMovieSceneTransformChannel::RotationZ);
			TransformMetaData[5].Color = FCommonChannelData::BlueChannelColor;
			TransformMetaData[5].SortOrder = 5;
			TransformMetaData[5].bCanCollapseToTrack = false;
		}
		{
			TransformMetaData[6].SetIdentifiers("Scale.X", FCommonChannelData::ChannelX, ScaleGroup);
			//MetaData[6].bEnabled = EnumHasAllFlags(Mask, EMovieSceneTransformChannel::ScaleX);
			TransformMetaData[6].Color = FCommonChannelData::RedChannelColor;
			TransformMetaData[6].SortOrder = 6;
			TransformMetaData[6].bCanCollapseToTrack = false;

			TransformMetaData[7].SetIdentifiers("Scale.Y", FCommonChannelData::ChannelY, ScaleGroup);
			//MetaData[7].bEnabled = EnumHasAllFlags(Mask, EMovieSceneTransformChannel::ScaleY);
			TransformMetaData[7].Color = FCommonChannelData::GreenChannelColor;
			TransformMetaData[7].SortOrder = 7;
			TransformMetaData[7].bCanCollapseToTrack = false;

			TransformMetaData[8].SetIdentifiers("Scale.Z", FCommonChannelData::ChannelZ, ScaleGroup);
			//MetaData[8].bEnabled = EnumHasAllFlags(Mask, EMovieSceneTransformChannel::ScaleZ);
			TransformMetaData[8].Color = FCommonChannelData::BlueChannelColor;
			TransformMetaData[8].SortOrder = 8;
			TransformMetaData[8].bCanCollapseToTrack = false;
		}

		ExternalValues[0].OnGetExternalValue = ExtractTranslationX;
		ExternalValues[1].OnGetExternalValue = ExtractTranslationY;
		ExternalValues[2].OnGetExternalValue = ExtractTranslationZ;
		ExternalValues[3].OnGetExternalValue = ExtractRotationX;
		ExternalValues[4].OnGetExternalValue = ExtractRotationY;
		ExternalValues[5].OnGetExternalValue = ExtractRotationZ;
		ExternalValues[6].OnGetExternalValue = ExtractScaleX;
		ExternalValues[7].OnGetExternalValue = ExtractScaleY;
		ExternalValues[8].OnGetExternalValue = ExtractScaleZ;

		ExternalValues[0].OnGetCurrentValueAndWeight = GetChannelValueAndWeight;
		ExternalValues[1].OnGetCurrentValueAndWeight = GetChannelValueAndWeight;
		ExternalValues[2].OnGetCurrentValueAndWeight = GetChannelValueAndWeight;
		ExternalValues[3].OnGetCurrentValueAndWeight = GetChannelValueAndWeight;
		ExternalValues[4].OnGetCurrentValueAndWeight = GetChannelValueAndWeight;
		ExternalValues[5].OnGetCurrentValueAndWeight = GetChannelValueAndWeight;
		ExternalValues[6].OnGetCurrentValueAndWeight = GetChannelValueAndWeight;
		ExternalValues[7].OnGetCurrentValueAndWeight = GetChannelValueAndWeight;
		ExternalValues[8].OnGetCurrentValueAndWeight = GetChannelValueAndWeight;
	}

	static TOptional<FVector> GetTranslation(UObject& InObject, FTrackInstancePropertyBindings* Bindings)
	{
		if (USceneComponent* SceneComponent = Cast<USceneComponent>(&InObject))
		{
			FVector RelativeLocation = SceneComponent->GetRelativeLocation();
			FVector RelativeTranslation = SceneComponent->GetRelativeTransform().GetTranslation();
			DebugLog(FString::Printf(TEXT("Add Key RelativeLocation %s RelativeTranslation %s"), *RelativeLocation.ToString(), *RelativeTranslation.ToString()));
			return SceneComponent->GetRelativeLocation();
			//return SceneComponent->GetRelativeTransform().GetTranslation();
		}


		return TOptional<FVector>();
	}

	static TOptional<FRotator> GetRotator(UObject& InObject, FTrackInstancePropertyBindings* Bindings)
	{
		if (USceneComponent* SceneComponent = Cast<USceneComponent>(&InObject))
		{
			return SceneComponent->GetRelativeRotation();
		}

		return TOptional<FRotator>();
	}

	static TOptional<FVector> GetScale(UObject& InObject, FTrackInstancePropertyBindings* Bindings)
	{
		const FStructProperty* TransformProperty = Bindings ? CastField<FStructProperty>(Bindings->GetProperty(InObject)) : nullptr;

		if (USceneComponent* SceneComponent = Cast<USceneComponent>(&InObject))
		{
			return SceneComponent->GetRelativeTransform().GetScale3D();
		}

		return TOptional<FVector>();
	}

	static TOptional<float> ExtractTranslationX(UObject& InObject, FTrackInstancePropertyBindings* Bindings)
	{
		TOptional<FVector> Translation = GetTranslation(InObject, Bindings);
		return Translation.IsSet() ? Translation->X : TOptional<float>();
	}
	static TOptional<float> ExtractTranslationY(UObject& InObject, FTrackInstancePropertyBindings* Bindings)
	{
		TOptional<FVector> Translation = GetTranslation(InObject, Bindings);
		return Translation.IsSet() ? Translation->Y : TOptional<float>();
	}
	static TOptional<float> ExtractTranslationZ(UObject& InObject, FTrackInstancePropertyBindings* Bindings)
	{
		TOptional<FVector> Translation = GetTranslation(InObject, Bindings);
		return Translation.IsSet() ? Translation->Z : TOptional<float>();
	}

	static TOptional<float> ExtractRotationX(UObject& InObject, FTrackInstancePropertyBindings* Bindings)
	{
		TOptional<FRotator> Rotator = GetRotator(InObject, Bindings);
		return Rotator.IsSet() ? Rotator->Roll : TOptional<float>();
	}
	static TOptional<float> ExtractRotationY(UObject& InObject, FTrackInstancePropertyBindings* Bindings)
	{
		TOptional<FRotator> Rotator = GetRotator(InObject, Bindings);
		return Rotator.IsSet() ? Rotator->Pitch : TOptional<float>();
	}
	static TOptional<float> ExtractRotationZ(UObject& InObject, FTrackInstancePropertyBindings* Bindings)
	{
		TOptional<FRotator> Rotator = GetRotator(InObject, Bindings);
		return Rotator.IsSet() ? Rotator->Yaw : TOptional<float>();
	}

	static TOptional<float> ExtractScaleX(UObject& InObject, FTrackInstancePropertyBindings* Bindings)
	{
		TOptional<FVector> Scale = GetScale(InObject, Bindings);
		return Scale.IsSet() ? Scale->X : TOptional<float>();
	}
	static TOptional<float> ExtractScaleY(UObject& InObject, FTrackInstancePropertyBindings* Bindings)
	{
		TOptional<FVector> Scale = GetScale(InObject, Bindings);
		return Scale.IsSet() ? Scale->Y : TOptional<float>();
	}
	static TOptional<float> ExtractScaleZ(UObject& InObject, FTrackInstancePropertyBindings* Bindings)
	{
		TOptional<FVector> Scale = GetScale(InObject, Bindings);
		return Scale.IsSet() ? Scale->Z : TOptional<float>();
	}

	static void GetChannelValueAndWeight(UObject* Object, UMovieSceneSection* SectionToKey, FFrameNumber KeyTime, FFrameRate TickResolution, FMovieSceneRootEvaluationTemplateInstance& RootTemplate,
		float& OutValue, float OutWeight)
	{
		OutValue = 0;
		OutWeight = 1.0f;
	}

	FMovieSceneChannelMetaData      MetaData[1];
	FMovieSceneChannelMetaData      TransformMetaData[9];

	TMovieSceneExternalValue<float> ExternalValues[9];
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

	Translation[0].SetDefault(0.f);
	Translation[1].SetDefault(0.f);
	Translation[2].SetDefault(0.f);

	Rotation[0].SetDefault(0.f);
	Rotation[1].SetDefault(0.f);
	Rotation[2].SetDefault(0.f);

	Scale[0].SetDefault(1.f);
	Scale[1].SetDefault(1.f);
	Scale[2].SetDefault(1.f);

	FMovieSceneChannelProxyData Channels;

#if WITH_EDITOR
	static FComponentAttachSectionEditorData EditorData;
	Channels.Add(AttachActorData, EditorData.MetaData[0]);

	Channels.Add(Translation[0], EditorData.TransformMetaData[0], EditorData.ExternalValues[0]);
	Channels.Add(Translation[1], EditorData.TransformMetaData[1], EditorData.ExternalValues[1]);
	Channels.Add(Translation[2], EditorData.TransformMetaData[2], EditorData.ExternalValues[2]);

	Channels.Add(Rotation[0], EditorData.TransformMetaData[3], EditorData.ExternalValues[3]);
	Channels.Add(Rotation[1], EditorData.TransformMetaData[4], EditorData.ExternalValues[4]);
	Channels.Add(Rotation[2], EditorData.TransformMetaData[5], EditorData.ExternalValues[5]);

	Channels.Add(Scale[0], EditorData.TransformMetaData[6], EditorData.ExternalValues[6]);
	Channels.Add(Scale[1], EditorData.TransformMetaData[7], EditorData.ExternalValues[7]);
	Channels.Add(Scale[2], EditorData.TransformMetaData[8], EditorData.ExternalValues[8]);
#else

	Channels.Add(AttachActorData);

#endif

	ChannelProxy = MakeShared<FMovieSceneChannelProxy>(MoveTemp(Channels));

}

USceneComponent* UComponentAttachSection::GetAttachComponent(const AActor* InParentActor, const FMovieSceneActorReferenceKey& Key) const
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
	return nullptr;
}

FVector UComponentAttachSection::GetTranslation(FFrameTime Time) const
{
	float X = 0;
	float Y = 0;
	float Z = 0;
	float ChannelValue = 0.f;
	if (Translation[0].Evaluate(Time, ChannelValue))
	{
		X = ChannelValue;
	}
	if (Translation[1].Evaluate(Time, ChannelValue))
	{
		Y = ChannelValue;
	}
	if (Translation[2].Evaluate(Time, ChannelValue))
	{
		Z = ChannelValue;
	}
	
	return FVector(X, Y, Z);
}

FRotator UComponentAttachSection::GetRotation(FFrameTime Time) const
{
	float X = 0;
	float Y = 0;
	float Z = 0;
	float ChannelValue = 0.f;
	if (Rotation[0].Evaluate(Time, ChannelValue))
	{
		X = ChannelValue;
	}
	if (Rotation[1].Evaluate(Time, ChannelValue))
	{
		Y = ChannelValue;
	}
	if (Rotation[2].Evaluate(Time, ChannelValue))
	{
		Z = ChannelValue;
	}

	return FRotator(X, Y, Z);
}

FVector UComponentAttachSection::GetScale(FFrameTime Time) const
{
	float X = 0;
	float Y = 0;
	float Z = 0;
	float ChannelValue = 0.f;
	if (Scale[0].Evaluate(Time, ChannelValue))
	{
		X = ChannelValue;
	}
	if (Scale[1].Evaluate(Time, ChannelValue))
	{
		Y = ChannelValue;
	}
	if (Scale[2].Evaluate(Time, ChannelValue))
	{
		Z = ChannelValue;
	}

	return FVector(X, Y, Z);
}

FTransform UComponentAttachSection::GetTransform(FFrameTime Time) const
{
	FVector CurrentLocation = GetTranslation(Time);
	FRotator CurrentRotation = GetRotation(Time);
	FVector CurrentScale = GetScale(Time);

	return FTransform(CurrentRotation, CurrentLocation, CurrentScale);
}



