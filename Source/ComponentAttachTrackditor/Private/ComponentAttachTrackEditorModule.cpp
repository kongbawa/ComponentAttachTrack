#include "ComponentAttachTrackEditorModule.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"
#include "ISequencerModule.h"
#include "ComponentAttachTrack/ComponentAttachTrackEditor.h"


IMPLEMENT_GAME_MODULE(FComponentAttachTrackEditorModule, ComponentAttachTrackEditor);

// DEFINE_LOG_CATEGORY(ComponentAttachTrackEditor)

// #define LOCTEXT_NAMESPACE "ComponentAttachTrackEditor"

void FComponentAttachTrackEditorModule::StartupModule()
{
	ISequencerModule& SequencerModule = FModuleManager::LoadModuleChecked<ISequencerModule>(TEXT("Sequencer"));
	SeqHandles.Add( SequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FComponentAttachTrackEditor::CreateTrackEditor)) );
}

void FComponentAttachTrackEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded(TEXT("Sequencer")))
	{
		ISequencerModule& SequencerModule = FModuleManager::GetModuleChecked<ISequencerModule>(TEXT("Sequencer"));

		for (const auto Handle : SeqHandles)
		{
			SequencerModule.UnRegisterTrackEditor(Handle);
		}
	}
}

// #undef LOCTEXT_NAMESPACE