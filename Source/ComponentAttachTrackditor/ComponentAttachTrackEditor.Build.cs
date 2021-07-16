// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class ComponentAttachTrackEditor : ModuleRules
{
    public ComponentAttachTrackEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        PublicIncludePaths.AddRange(new string[] 
        {
            "Editor/MovieSceneTools/Public",
        });
        PrivateIncludePaths.AddRange(new string[] 
        {
        });
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "Slate",
            "AnimGraphRuntime",
            "BlueprintGraph",
        });
        if (Target.bBuildEditor == true)
        {
            PrivateDependencyModuleNames.AddRange(new string[]
            {
                "UnrealEd",
                "GraphEditor",
                "ComponentAttachTrack",
                "Sequencer",
                "MovieScene",
                "MovieSceneTools",
                "MovieSceneTracks",
            });
        }
        PrivateIncludePathModuleNames.AddRange(
            new string[] {
                "Persona",
                "SkeletonEditor",
                "AdvancedPreviewScene",
            }
        );
        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "InputCore",
            "SlateCore",
            "UnrealEd",
            "GraphEditor",
            "PropertyEditor",
            "ContentBrowser",
            "AnimGraph",
			"ToolMenus",

            "LevelSequence",
            "MovieScene",
        });
        if (Target.bBuildEditor == true)
        {
            PrivateDependencyModuleNames.AddRange(new string[]
            {
                //"EditorStyle",
            });
        }
        //PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd" });
        PrivateIncludePathModuleNames.AddRange(new string[] {  });
        DynamicallyLoadedModuleNames.AddRange(new string[] {  });
    }
}