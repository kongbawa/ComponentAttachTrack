#include "ComponentAttachTrackEditor.h"
#include "MovieScene.h"
#include "Slate/SlateTextures.h"
#include "Rendering/DrawElements.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "SequencerUtilities.h"
#include "ISequencerSection.h"
#include "ScopedTransaction.h"
#include "SequencerSectionPainter.h"
#include "AssetRegistryModule.h"
#include "Async/Async.h"
#include "ComponentAttachTrack/ComponentAttachSection.h"
#include "GameFramework/Actor.h"
#include "ActorEditorUtils.h"
#include "SceneOutlinerPublicTypes.h"
#include "SceneOutlinerModule.h"
#include "MovieSceneObjectBindingIDPicker.h"
#include "MovieSceneToolHelpers.h"
#include "GameFramework/WorldSettings.h"

#define LOCTEXT_NAMESPACE "ComponentAttachTrackEditor"

class FComponentAttachSection final	: public ISequencerSection
{
public:
	FComponentAttachSection(UMovieSceneSection& InSection, TWeakPtr<ISequencer> InSequencer)
		: Section(Cast<UComponentAttachSection>(&InSection))
        , Sequencer(InSequencer)
	{
    }
    virtual ~FComponentAttachSection() override
    {
    }
	virtual UMovieSceneSection* GetSectionObject() override { return Section; }
	virtual FText GetSectionTitle() const override
	{
        if (Section != nullptr)
        {
			return FText::FromString(Section->GetTitle());
        }
		return FText::GetEmpty();
	}
    virtual void BuildSectionContextMenu(FMenuBuilder& MenuBuilder, const FGuid& ObjectBinding) override 
    {
	}
	virtual int32 OnPaintSection(FSequencerSectionPainter& InPainter) const override
	{
		const int32 LayerId = InPainter.PaintSectionBackground();
		return LayerId;
	}

private:
	UComponentAttachSection* Section;
	TWeakPtr<ISequencer> Sequencer;
};


FComponentAttachTrackEditor::FComponentAttachTrackEditor(TSharedRef<ISequencer> InSequencer)
	: FMovieSceneTrackEditor(InSequencer)
{
}

TSharedRef<ISequencerTrackEditor> FComponentAttachTrackEditor::CreateTrackEditor(TSharedRef<ISequencer> InSequencer)
{
	return MakeShareable(new FComponentAttachTrackEditor(InSequencer));
}

bool FComponentAttachTrackEditor::SupportsType(TSubclassOf<UMovieSceneTrack> Type) const
{
    return Type == UComponentAttachTrack::StaticClass();
}

TSharedRef<ISequencerSection> FComponentAttachTrackEditor::MakeSectionInterface(UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding)
{
	return MakeShareable(new FComponentAttachSection(SectionObject, GetSequencer()));
}

const FSlateBrush* FComponentAttachTrackEditor::GetIconBrush() const
{
	return nullptr;
}

void FComponentAttachTrackEditor::CreateNewSection(UComponentAttachTrack* Track) const
{
	TSharedPtr<ISequencer> SequencerPin = GetSequencer();
	UClass* Class = UComponentAttachSection::StaticClass();

	if (Class && SequencerPin)
	{
		FScopedTransaction Transaction(FText::Format(LOCTEXT("AddCustomSection_Transaction", "Add New Section From Class %s"), FText::FromName(Class->GetFName())));
		//UMovieScene3DTransformSection* NewSection = NewObject<UMovieScene3DTransformSection>(Track, UMovieScene3DTransformSection::StaticClass(), NAME_None, RF_Transactional);
		UComponentAttachSection* NewAttachSection = NewObject<UComponentAttachSection>(Track, Class, NAME_None, RF_Transactional);
		
		const FQualifiedFrameTime CurrentTime = SequencerPin->GetLocalTime();

		const FFrameNumber Duration = (5.f * CurrentTime.Rate).FrameNumber;
		NewAttachSection->SetRange(TRange<FFrameNumber>(CurrentTime.Time.FrameNumber, CurrentTime.Time.FrameNumber + Duration));
		NewAttachSection->InitialPlacement(Track->GetAllSections(), CurrentTime.Time.FrameNumber, Duration.Value, Track->SupportsMultipleRows());

		Track->AddSection(*NewAttachSection);
		//Track->AddSection(*NewSection);

		SequencerPin->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
	}
}

void FComponentAttachTrackEditor::AddNewObjectBindingTrack(TArray<FGuid> InObjectBindings) const
{
	UMovieScene* MovieScene = GetFocusedMovieScene();
	if (MovieScene == nullptr || MovieScene->IsReadOnly())
	{
		return;
	}

	UClass* ClassToAdd = UComponentAttachTrack::StaticClass();//LoadClassFromAssetData(AssetData);


	const FScopedTransaction Transaction(FText::Format(LOCTEXT("AddCustomObjectTrack_Transaction", "Add Object Track %s"), FText::FromName(ClassToAdd->GetFName())));

	MovieScene->Modify();

	for (const FGuid& ObjectBindingID : InObjectBindings)
	{
		UComponentAttachTrack* CustomTrack = CastChecked<UComponentAttachTrack>(MovieScene->AddTrack(ClassToAdd, ObjectBindingID));
		CreateNewSection(CustomTrack);

		if (GetSequencer().IsValid())
		{
			GetSequencer()->OnAddTrack(CustomTrack, FGuid());
		}
	}
}

void FComponentAttachTrackEditor::BuildObjectBindingTrackMenu(FMenuBuilder& MenuBuilder, const TArray<FGuid>& ObjectBindings, const UClass* ObjectClass)
{
	if (ObjectClass->IsChildOf(USceneComponent::StaticClass()))
	{
		MenuBuilder.AddMenuEntry(
			LOCTEXT("AddComponentAttachTrackTrack", "ComponentAttach"),
			LOCTEXT("AddComponentAttachTrackTooltip", "Adds ComponentAttach Track."),
			FSlateIcon("ComponentAttach", "EventIcon"),
			FUIAction(
				FExecuteAction::CreateSP(this, &FComponentAttachTrackEditor::AddNewObjectBindingTrack, ObjectBindings)
			)
		);

	}
}

#undef LOCTEXT_NAMESPACE
