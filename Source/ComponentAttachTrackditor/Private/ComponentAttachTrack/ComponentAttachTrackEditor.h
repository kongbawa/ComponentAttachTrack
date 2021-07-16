#pragma once

#include "MovieSceneTrackEditor.h"
#include "ComponentAttachTrack/ComponentAttachTrack.h"

class UComponentAttachTrack;
class UComponentAttachSection;

class FComponentAttachTrackEditor final	: public FMovieSceneTrackEditor
{
public:
	explicit FComponentAttachTrackEditor(TSharedRef<ISequencer> InSequencer);
	static TSharedRef<ISequencerTrackEditor> CreateTrackEditor(TSharedRef<ISequencer> OwningSequencer);

public:
	// ISequencerTrackEditor interface
	virtual void BuildObjectBindingTrackMenu(FMenuBuilder& MenuBuilder, const TArray<FGuid>& ObjectBindings, const UClass* ObjectClass) override;
	virtual TSharedRef<ISequencerSection> MakeSectionInterface(UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding) override;
	virtual bool SupportsType(TSubclassOf<UMovieSceneTrack> Type) const override;
	virtual const FSlateBrush* GetIconBrush() const override;
private:
	void AddNewObjectBindingTrack(TArray<FGuid> InObjectBindings) const;
	UComponentAttachSection* CreateNewSection(UComponentAttachTrack* Track, TSubclassOf<UComponentAttachSection> ClassType) const;
};
