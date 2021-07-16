// Copy-right is reserved by iqiyi VR-group


#include "ComponentAttachTrack/ComponentAttachTrack.h"
#include "ComponentAttachTrack/ComponentAttachSection.h"
#include "ComponentAttachTrack/ComponentAttachEvalTemplate.h"
#include "MovieScene.h"


FMovieSceneEvalTemplatePtr UComponentAttachTrack::CreateTemplateForSection(const UMovieSceneSection& InSection) const
{
	return FComponentAttachEvalTemplate(CastChecked<const UComponentAttachSection>(&InSection));
}

UMovieSceneSection* UComponentAttachTrack::CreateNewSection()
{
	return NewObject<UMovieSceneSection>(this, UComponentAttachTrack::StaticClass(), NAME_None, RF_Transactional);
}

void UComponentAttachTrack::AddNewEvent(const FFrameNumber& Time)
{
	UComponentAttachSection* NewSection = NewObject<UComponentAttachSection>(this);
	ensure(NewSection);

	NewSection->InitialPlacement(GetAllSections(), Time, 2, SupportsMultipleRows());
	AddSection(*NewSection);
}

#if WITH_EDITORONLY_DATA
FText UComponentAttachTrack::GetDisplayName() const
{
	return NSLOCTEXT("ComponentAttachTrack", "TrackName", "ComponentAttach");
}
#endif

FName UComponentAttachTrack::GetTrackName() const
{
	static FName TrackName("ComponentAttaches");
	return TrackName;
}

bool UComponentAttachTrack::SupportsType(TSubclassOf<UMovieSceneSection> Type) const
{
	return Type == UComponentAttachSection::StaticClass();
}
