// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "Factory/RDDataAssetFactory.h"
#include "RobotDescription/RDDataAsset.h"
#include "SDFParser.h"
#include "URDFParser.h"
#include "Editor.h" // FEditorDelegates

// Constructor
URDDataAssetFactory::URDDataAssetFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// UE_LOG(LogTemp, Error, TEXT("[%s]"), TEXT(__FUNCTION__));
	// Which asset type can the factory import
	SupportedClass = URDDataAsset::StaticClass();

	// List of formats supported by the factory. Each entry is of the form "ext;Description" where ext is the file extension
	Formats.Add(TEXT("sdf;SDF robot description format"));
	Formats.Add(TEXT("urdf;URDF robot description format"));

	//// Factory can create a new object from scratch
	//bCreateNew = false;
	//// Factory imports objects from text
	//bText = true;
	// Factory imports objects from files
	bEditorImport = true;
	// Associated editor should be opened after creating a new object
	//bEditAfterNew = true;

	// Required to allow other re importers do their CanReimport checks first
	ImportPriority = DefaultImportPriority - 1;
}

/* Begin UFactory overrides */
// Whether the specified file can be imported by this factory
bool URDDataAssetFactory::FactoryCanImport(const FString& Filename)
{
	return Filename.EndsWith(".sdf", ESearchCase::IgnoreCase) || Filename.EndsWith(".urdf", ESearchCase::IgnoreCase);
}

// Create a new object by importing it from a file name
UObject* URDDataAssetFactory::FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString & Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{

	UE_LOG(LogTemp, Log, TEXT("InName %s, InParentName %s"), *InName.ToString(), *InParent->GetName());

	// UE_LOG(LogTemp, Error, TEXT("[%s] Filename=%s"), TEXT(__FUNCTION__), *Filename);
	Flags |= RF_Transactional;

	// Called when new assets are being (re-)imported
	GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPreImport(this, InClass, InParent, InName, Parms);
	//FEditorDelegates::OnAssetPreImport.Broadcast(this, InClass, InParent, InName, Parms);

	URDDataAsset* NewDataAsset;
	if (Filename.EndsWith(".sdf", ESearchCase::IgnoreCase)) {
		FSDFParser Parser(Filename);
		NewDataAsset = Parser.ParseToNewDataAsset(InParent, InName, Flags);
	}	else {
		FURDFParser Parser(Filename);
		NewDataAsset = Parser.ParseToNewDataAsset(InParent, InName, Flags);
	}

	// Called when new assets have been (re-)imported
	GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPostImport(this, NewDataAsset);
	//FEditorDelegates::OnAssetPostImport.Broadcast(this, NewDataAsset);
	return NewDataAsset;
}
/* End UFactory overrides */
