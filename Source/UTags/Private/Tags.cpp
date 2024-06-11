// Copyright 2017-2020, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#include "Tags.h"

///////////////////////////////////////////////////////////////////////////
// Return the index where the tag type was found in the array
int32 FTags::GetTagTypeIndex(const TArray<FName>& InTags, const FString& TagType)
{
	// Iterate all the tags, check for keyword TagType
	for (int32 i = 0; i < InTags.Num(); ++i)
	{
		// Check if tag is of given type
		if (InTags[i].ToString().StartsWith(TagType + ";"))
		{
			return i;
		}
	}
	// return INDEX_NONE if type was not found 
	return INDEX_NONE;

	//// Init tag index
	//int32 TagIndex = INDEX_NONE;

	//// Iterate all the tags, check for keyword TagType
	//for (const auto& TagItr : InTags)
	//{
	//	// Increment the position of the tag in the array
	//	++TagIndex;

	//	// Check if tag is of given type
	//	if (TagItr.ToString().StartsWith(TagType + ";"))
	//	{
	//		return TagIndex;
	//	}
	//}
	//// return INDEX_NONE if type was not found 
	//return INDEX_NONE;
}

// Return the index where the tag type was found in the actor's array
int32 FTags::GetTagTypeIndex(AActor* Actor, const FString& TagType)
{
	return GetTagTypeIndex(Actor->Tags, TagType);
}

// Return the index where the tag type was found in the component's array
int32 FTags::GetTagTypeIndex(UActorComponent* Component, const FString& TagType)
{
	return GetTagTypeIndex(Component->ComponentTags, TagType);
}

// Return the index where the tag type was found in the object
int32 FTags::GetTagTypeIndex(UObject* Object, const FString& TagType)
{
	if (UActorComponent* ActComp = Cast<UActorComponent>(Object))
	{
		return GetTagTypeIndex(ActComp->ComponentTags, TagType);
	}
	else if (AActor* Act = Cast<AActor>(Object))
	{
		return GetTagTypeIndex(Act->Tags, TagType);
	}
	else
	{
		return INDEX_NONE;
	}	
}


///////////////////////////////////////////////////////////////////////////
// Check if type exists in tag
bool FTags::HasType(const FName& InTag, const FString& TagType)
{
	return InTag.ToString().StartsWith(TagType + ";");
}

// Check if type exists in tag array
bool FTags::HasType(const TArray<FName>& InTags, const FString& TagType)
{
	return FTags::GetTagTypeIndex(InTags, TagType) != INDEX_NONE;
}

// Check if type exists from actor
bool FTags::HasType(AActor* Actor, const FString& TagType)
{
	if (Actor == nullptr)
	{
		return false;
	}
	return FTags::HasType(Actor->Tags, TagType);
}

// Check if type exists from component
bool FTags::HasType(UActorComponent* Component, const FString& TagType)
{
	if (Component == nullptr)
	{
		return false;
	}
	return FTags::HasType(Component->ComponentTags, TagType);
}

// Check if type exists from object
bool FTags::HasType(UObject* Object, const FString& TagType)
{
	if (Object == nullptr)
	{
		return false;
	}
	if (UActorComponent* ActComp = Cast<UActorComponent>(Object))
	{
		return FTags::HasType(ActComp, TagType);
	}
	else if (AActor* Act = Cast<AActor>(Object))
	{
		return FTags::HasType(Act, TagType);
	}
	else
	{
		return false;
	}
}



///////////////////////////////////////////////////////////////////////////
// Check if key exists in tag
bool FTags::HasKey(const FName& InTag, const FString& TagKey)
{
	return InTag.ToString().Find(";" + TagKey + ",") != INDEX_NONE;
}

// Check if key exists tag array
bool FTags::HasKey(const TArray<FName>& InTags, const FString& TagType, const FString& TagKey)
{
	// Check if type exists and return index of its location in the array
	int32 TagIndex = FTags::GetTagTypeIndex(InTags, TagType);
	if (TagIndex != INDEX_NONE)
	{
		return FTags::HasKey(InTags[TagIndex], TagKey);
	}
	// Type was not found, return false
	return false;
}

// Check if key exists from actor
bool FTags::HasKey(AActor* Actor, const FString& TagType, const FString& TagKey)
{
	if (Actor == nullptr)
	{
		return false;
	}
	return FTags::HasKey(Actor->Tags, TagType, TagKey);
}

// Check if key exists from component
bool FTags::HasKey(UActorComponent* Component, const FString& TagType, const FString& TagKey)
{
	if (Component == nullptr)
	{
		return false;
	}
	return FTags::HasKey(Component->ComponentTags, TagType, TagKey);
}

// Check if key exists from object
bool FTags::HasKey(UObject* Object, const FString& TagType, const FString& TagKey)
{
	if (Object == nullptr)
	{
		return false;
	}
	if (UActorComponent* ActComp = Cast<UActorComponent>(Object))
	{
		return FTags::HasKey(ActComp, TagType, TagKey);
	}
	else if (AActor* Act = Cast<AActor>(Object))
	{
		return FTags::HasKey(Act, TagType, TagKey);
	}
	else
	{
		return false;
	}
}


///////////////////////////////////////////////////////////////////////////
// Check if key value pair exists in tag
bool FTags::HasKeyValuePair(const FName& InTag, const FString& TagKey, const FString& TagValue)
{
	return InTag.ToString().Find(";" + TagKey + "," + TagValue + ";") != INDEX_NONE;
}

// Check if key value pair exists in tag array
bool FTags::HasKeyValuePair(const TArray<FName>& InTags, const FString& TagType, const FString& TagKey, const FString& TagValue)
{
	// Check if type exists and return index of its location in the array
	int32 TagIndex = FTags::GetTagTypeIndex(InTags, TagType);
	if (TagIndex != INDEX_NONE)
	{
		return FTags::HasKeyValuePair(InTags[TagIndex], TagKey, TagValue);
	}
	// Type was not found, return false
	return false;
}

// Check if key value pair exists in actor tags
bool FTags::HasKeyValuePair(AActor* Actor, const FString& TagType, const FString& TagKey, const FString& TagValue)
{
	if (Actor == nullptr)
	{
		return false;
	}
	return FTags::HasKeyValuePair(Actor->Tags, TagType, TagKey, TagValue);
}

// Check if key value pair exists in component tags
bool FTags::HasKeyValuePair(UActorComponent* Component, const FString& TagType, const FString& TagKey, const FString& TagValue)
{
	if (Component == nullptr)
	{
		return false;
	}
	return FTags::HasKeyValuePair(Component->ComponentTags, TagType, TagKey, TagValue);
}

// Check if key value pair exists in object
bool FTags::HasKeyValuePair(UObject* Object, const FString& TagType, const FString& TagKey, const FString& TagValue)
{
	if (Object == nullptr)
	{
		return false;
	}
	if (UActorComponent* ActComp = Cast<UActorComponent>(Object))
	{
		return FTags::HasKeyValuePair(ActComp, TagType, TagKey, TagValue);
	}
	else if (AActor* Act = Cast<AActor>(Object))
	{
		return FTags::HasKeyValuePair(Act, TagType, TagKey, TagValue);
	}
	else
	{
		return false;
	}	
}


///////////////////////////////////////////////////////////////////////////
// Get tag key value from tag
FString FTags::GetValue(const FName& InTag, const FString& TagKey)
{
	// Copy of the current tag as FString
	FString CurrTag = InTag.ToString();

	// Check the position of the key string in the tag
	int32 KeyPos = CurrTag.Find(";" + TagKey + ",");
	if (KeyPos != INDEX_NONE)
	{
		// Remove from tag with the cut length of: 
		// pos of the key + length of the semicolon char + length of the key + length of the comma char 
		CurrTag.RemoveAt(0, KeyPos + 1 + TagKey.Len() + 1);
		// Set the tag value as the left side of the string before the semicolon
		return CurrTag.Left(CurrTag.Find(";"));
	}
	// Return empty string if key was not found
	return FString();
}

// Get tag key value from tag array
FString FTags::GetValue(const TArray<FName>& InTags, const FString& TagType, const FString& TagKey)
{
	// Check if type exists and return index of its location in the array
	int32 TagIndex = FTags::GetTagTypeIndex(InTags, TagType);
	if (TagIndex != INDEX_NONE)
	{
		return FTags::GetValue(InTags[TagIndex], TagKey);
	}

	// If type was not found return an empty string
	return FString();
}

// Get tag key value from actor
FString FTags::GetValue(AActor* Actor, const FString& TagType, const FString& TagKey)
{
	if (Actor == nullptr)
	{
		return FString();
	}
	return FTags::GetValue(Actor->Tags, TagType, TagKey);
}

// Get tag key value from component
FString FTags::GetValue(UActorComponent* Component, const FString& TagType, const FString& TagKey)
{
	if (Component == nullptr)
	{
		return FString();
	}
	return FTags::GetValue(Component->ComponentTags, TagType, TagKey);
}

// Get tag key value from object
FString FTags::GetValue(UObject* Object, const FString& TagType, const FString& TagKey)
{
	if (AActor* ObjAsAct = Cast<AActor>(Object))
	{
		return GetValue(ObjAsAct->Tags, TagType, TagKey);
	}
	else if (UActorComponent* ObjAsActComp = Cast<UActorComponent>(Object))
	{
		return GetValue(ObjAsActComp->ComponentTags, TagType, TagKey);
	}
	return FString();
}

///////////////////////////////////////////////////////////////////////////
// Add tag key value from tags, if bReplaceExisting is true, replace existing value
bool FTags::AddKeyValuePair(FName& InTag, const FString& TagKey, const FString& TagValue, bool bReplaceExisting, UObject* Owner)
{
	// Get the key value
	const FString CurrVal = FTags::GetValue(InTag, TagKey);
	if (CurrVal.IsEmpty())
	{
		if (Owner)
		{
			Owner->Modify();
		}
		// Key does not exist, add new one at the end
		InTag = FName(*InTag.ToString().Append(TagKey).Append(",").Append(TagValue).Append(";"));
		return true;
	}
	else if (bReplaceExisting)
	{
		if (Owner)
		{
			Owner->Modify();
		}
		// Key exist, replace
		const FString Old = TagKey + "," + CurrVal;
		const FString New = TagKey + "," + TagValue;
		InTag = FName(*InTag.ToString().Replace(*Old, *New));
		InTag = FName(*InTag.ToString().Replace(*CurrVal, *TagValue));
		return true;
	}
	// Cannot overwrite value, return false
	return false;
}

// Add tag key value from tags, if bReplaceExisting is true, replace existing value
bool FTags::AddKeyValuePair(TArray<FName>& InTags, const FString& TagType, const FString& TagKey, const FString& TagValue, bool bReplaceExisting, UObject* Owner)
{
	// Check if type exists and return index of its location in the array
	int32 TagIndex = FTags::GetTagTypeIndex(InTags, TagType);
	if (TagIndex != INDEX_NONE)
	{
		return FTags::AddKeyValuePair(InTags[TagIndex], TagKey, TagValue, bReplaceExisting, Owner);
	}
	else // Type was not found, create a new one
	{
		if (Owner)
		{
			Owner->Modify();
		}
		FString NewTag;
		InTags.Add(FName(*NewTag.Append(TagType).Append(";").Append(TagKey).Append(",").Append(TagValue).Append(";")));
		return true;
	}
	return false;
}

// Add tag key value to actor, if bReplaceExisting is true, replace existing value
bool FTags::AddKeyValuePair(AActor* Actor, const FString& TagType, const FString& TagKey, const FString& TagValue, bool bReplaceExisting)
{
	return FTags::AddKeyValuePair(Actor->Tags, TagType, TagKey, TagValue, bReplaceExisting, Actor);
}

// Add tag key value to component, if bReplaceExisting is true, replace existing value
bool FTags::AddKeyValuePair(UActorComponent* Component, const FString& TagType, const FString& TagKey, const FString& TagValue, bool bReplaceExisting)
{
	return FTags::AddKeyValuePair(Component->ComponentTags, TagType, TagKey, TagValue, bReplaceExisting, Component);
}

// Add tag key value to object, if bReplaceExisting is true, replace existing value
bool FTags::AddKeyValuePair(UObject* Object, const FString& TagType, const FString& TagKey, const FString& TagValue, bool bReplaceExisting)
{
	if (AActor* ObjAsAct = Cast<AActor>(Object))
	{
		return FTags::AddKeyValuePair(ObjAsAct->Tags, TagType, TagKey, TagValue, bReplaceExisting);
	}
	else if (UActorComponent* ObjAsActComp = Cast<UActorComponent>(Object))
	{
		return FTags::AddKeyValuePair(ObjAsActComp->ComponentTags, TagType, TagKey, TagValue, bReplaceExisting);
	}
	return false;
}


///////////////////////////////////////////////////////////////////////////
//Add array of tag key values to tag, if bReplaceExisting is true, replace existing value
bool FTags::AddKeyValuePairs(FName& InTag, const TArray<TPair<FString, FString>>& InKeyValuePairs, bool bReplaceExisting, UObject* Owner)
{
	bool bAllKeyValuePairsAdded = true;
	for (const auto& KV : InKeyValuePairs)
	{
		// Get the key value
		const FString CurrVal = FTags::GetValue(InTag, KV.Key);
		if (CurrVal.IsEmpty())
		{
			if (Owner)
			{
				Owner->Modify();
			}
			// Key does not exist, add new one at the end
			InTag = FName(*InTag.ToString().Append(KV.Key).Append(",").Append(KV.Value).Append(";"));
		}
		else if (bReplaceExisting)
		{
			if (Owner)
			{
				Owner->Modify();
			}
			// Key exist, replace
			const FString Old = KV.Key + "," + CurrVal;
			const FString New = KV.Key + "," + KV.Value;
			InTag = FName(*InTag.ToString().Replace(*Old, *New));
		}
		else
		{
			// Could not overwrite value
			bAllKeyValuePairsAdded = false;
		}
	}
	return bAllKeyValuePairsAdded;
}

// Add array of tag key values to tags, if bReplaceExisting is true, replace existing value
bool FTags::AddKeyValuePairs(TArray<FName>& InTags, const FString& TagType, const TArray<TPair<FString, FString>>& InKeyValuePairs, bool bReplaceExisting, UObject* Owner)
{
	// Check if type exists and return index of its location in the array
	int32 TagIndex = FTags::GetTagTypeIndex(InTags, TagType);
	if (TagIndex != INDEX_NONE)
	{
		return FTags::AddKeyValuePairs(InTags[TagIndex], InKeyValuePairs, bReplaceExisting);
	}
	else // Type was not found, create a new one
	{
		if (Owner)
		{
			Owner->Modify();
		}
		FString NewTag;
		NewTag.Append(TagType).Append(";");
		for (const auto& TagKeyVal : InKeyValuePairs)
		{
			NewTag.Append(TagKeyVal.Key).Append(",").Append(TagKeyVal.Value).Append(";");
		}
		InTags.Add(FName(*NewTag));
		return true;
	}
	return false;
}

// Add array of tag key values to the actor,, if bReplaceExisting is true, replace existing value
bool FTags::AddKeyValuePairs(AActor* Actor, const FString& TagType, const TArray<TPair<FString, FString>>& InKeyValuePairs, bool bReplaceExisting)
{
	return FTags::AddKeyValuePairs(Actor->Tags, TagType, InKeyValuePairs, bReplaceExisting, Actor);
}

// Add array of tag key values to component, if bReplaceExisting is true, replace existing value
bool FTags::AddKeyValuePairs(UActorComponent* Component, const FString& TagType, const TArray<TPair<FString, FString>>& InKeyValuePairs, bool bReplaceExisting)
{
	return FTags::AddKeyValuePairs(Component->ComponentTags, TagType, InKeyValuePairs, bReplaceExisting, Component);
}

// Add array of tag key values to object, if bReplaceExisting is true, replace existing value
bool FTags::AddKeyValuePairs(UObject* Object, const FString& TagType, const TArray<TPair<FString, FString>>& InKeyValuePairs, bool bReplaceExisting)
{
	if (AActor* ObjAsAct = Cast<AActor>(Object))
	{
		return FTags::AddKeyValuePairs(ObjAsAct->Tags, TagType, InKeyValuePairs, bReplaceExisting);
	}
	else if (UActorComponent* ObjAsActComp = Cast<UActorComponent>(Object))
	{
		return FTags::AddKeyValuePairs(ObjAsActComp->ComponentTags, TagType, InKeyValuePairs, bReplaceExisting);
	}
	return false;
}


///////////////////////////////////////////////////////////////////////////
//Add map of tag key values to tag, if bReplaceExisting is true, replace existing value
bool FTags::AddKeyValuePairs(FName& InTag, const TMap<FString, FString>& InKeyValuePairs, bool bReplaceExisting, UObject* Owner)
{
	bool bAllKeyValuePairsAdded = true;
	for (const auto& KV : InKeyValuePairs)
	{
		// Get the key value
		const FString CurrVal = FTags::GetValue(InTag, KV.Key);
		if (CurrVal.IsEmpty())
		{
			if (Owner)
			{
				Owner->Modify();
			}
			// Key does not exist, add new one at the end
			InTag = FName(*InTag.ToString().Append(KV.Key).Append(",").Append(KV.Value).Append(";"));
		}
		else if (bReplaceExisting)
		{
			if (Owner)
			{
				Owner->Modify();
			}
			// Key exist, replace
			const FString Old = KV.Key + "," + CurrVal;
			const FString New = KV.Key + "," + KV.Value;
			InTag = FName(*InTag.ToString().Replace(*Old, *New));
		}
		else
		{
			// Could not overwrite value
			bAllKeyValuePairsAdded = false;
		}
	}
	return bAllKeyValuePairsAdded;
}

// Add map of tag key values to tags, if bReplaceExisting is true, replace existing value
bool FTags::AddKeyValuePairs(TArray<FName>& InTags, const FString& TagType, const TMap<FString, FString>& InKeyValuePairs, bool bReplaceExisting, UObject* Owner)
{
	// Check if type exists and return index of its location in the array
	int32 TagIndex = FTags::GetTagTypeIndex(InTags, TagType);
	if (TagIndex != INDEX_NONE)
	{
		return FTags::AddKeyValuePairs(InTags[TagIndex], InKeyValuePairs, bReplaceExisting);
	}
	else // Type was not found, create a new one
	{
		if (Owner)
		{
			Owner->Modify();
		}
		FString NewTag;
		NewTag.Append(TagType).Append(";");
		for (const auto& TagKeyVal : InKeyValuePairs)
		{
			NewTag.Append(TagKeyVal.Key).Append(",").Append(TagKeyVal.Value).Append(";");
		}
		InTags.Add(FName(*NewTag));
		return true;
	}
	return false;
}

// Add map of tag key values to the actor,, if bReplaceExisting is true, replace existing value
bool FTags::AddKeyValuePairs(AActor* Actor, const FString& TagType, const TMap<FString, FString>& InKeyValuePairs, bool bReplaceExisting)
{
	return FTags::AddKeyValuePairs(Actor->Tags, TagType, InKeyValuePairs, bReplaceExisting, Actor);
}

// Add map of tag key values to component, if bReplaceExisting is true, replace existing value
bool FTags::AddKeyValuePairs(UActorComponent* Component, const FString& TagType, const TMap<FString, FString>& InKeyValuePairs, bool bReplaceExisting)
{
	return FTags::AddKeyValuePairs(Component->ComponentTags, TagType, InKeyValuePairs, bReplaceExisting, Component);
}

// Add map of tag key values to object, if bReplaceExisting is true, replace existing value
bool FTags::AddKeyValuePairs(UObject* Object, const FString& TagType, const TMap<FString, FString>& InKeyValuePairs, bool bReplaceExisting)
{
	if (AActor* ObjAsAct = Cast<AActor>(Object))
	{
		return FTags::AddKeyValuePairs(ObjAsAct->Tags, TagType, InKeyValuePairs, bReplaceExisting);
	}
	else if (UActorComponent* ObjAsActComp = Cast<UActorComponent>(Object))
	{
		return FTags::AddKeyValuePairs(ObjAsActComp->ComponentTags, TagType, InKeyValuePairs, bReplaceExisting);
	}
	return false;
}


///////////////////////////////////////////////////////////////////////////
// Add tag type
bool FTags::AddTagType(TArray<FName>& InTags, const FString& TagType, UObject* Owner)
{
	// Check if type exists and return index of its location in the array
	int32 TagIndex = FTags::GetTagTypeIndex(InTags, TagType);
	if (TagIndex == INDEX_NONE)
	{
		if (Owner)
		{
			Owner->Modify();
		}
		FString NewTag;
		InTags.Add(FName(*NewTag.Append(TagType).Append(";")));
		return true;
	}
	// Tag already exist
	return false;
}

// Add tag type
bool FTags::AddTagType(AActor* Actor, const FString& TagType)
{
	return FTags::AddTagType(Actor->Tags, TagType, Actor);
}

// Add tag type
bool FTags::AddTagType(UActorComponent* Component, const FString& TagType)
{
	return FTags::AddTagType(Component->ComponentTags, TagType, Component);
}

// Add tag type
bool FTags::AddTagType(UObject* Object, const FString& TagType)
{
	if (AActor* ObjAsAct = Cast<AActor>(Object))
	{
		return FTags::AddTagType(ObjAsAct->Tags, TagType);
	}
	else if (UActorComponent* ObjAsActComp = Cast<UActorComponent>(Object))
	{
		return FTags::AddTagType(ObjAsActComp->ComponentTags, TagType);
	}
	return false;
}


///////////////////////////////////////////////////////////////////////////
// Remove tag key value from tag
bool FTags::RemoveKeyValuePair(FName& InTag, const FString& TagKey, UObject* Owner)
{
	// Copy of the current tag as FString
	FString CurrTag = InTag.ToString();
	const FString ToRemove = TagKey + TEXT(",") + GetValue(InTag, TagKey) + TEXT(";");
	int32 FindPos = CurrTag.Find(ToRemove, ESearchCase::CaseSensitive);
	if (FindPos != INDEX_NONE)
	{
		if (Owner)
		{
			Owner->Modify();
		}
		CurrTag.RemoveAt(FindPos, ToRemove.Len());
		InTag = FName(*CurrTag);
	}
	// "TagKey,TagValue;" combo could not be found
	return false;
}

// Remove tag key value from tags
bool FTags::RemoveKeyValuePair(TArray<FName>& InTags, const FString& TagType, const FString& TagKey, UObject* Owner)
{
	// Check if type exists and return index of its location in the array
	int32 TagIndex = FTags::GetTagTypeIndex(InTags, TagType);
	if (TagIndex != INDEX_NONE)
	{
		return FTags::RemoveKeyValuePair(InTags[TagIndex], TagKey, Owner);
	}
	// Tag type not found, nothing to remove
	return false;
}

// Remove tag key value from actor
bool FTags::RemoveKeyValuePair(AActor* Actor, const FString& TagType, const FString& TagKey)
{
	return FTags::RemoveKeyValuePair(Actor->Tags, TagType, TagKey, Actor);
}

// Remove tag key value from component
bool FTags::RemoveKeyValuePair(UActorComponent* Component, const FString& TagType, const FString& TagKey)
{
	return FTags::RemoveKeyValuePair(Component->ComponentTags, TagType, TagKey, Component);
}

// Remove all tag key values from world
bool FTags::RemoveAllKeyValuePairs(UWorld* World, const FString& TagType, const FString& TagKey)
{
	// Iterate actors from world
	//for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	for (const auto& ActorItr : World->PersistentLevel->Actors)
	{
		FTags::RemoveKeyValuePair(ActorItr, TagType, TagKey);

		// Iterate components of the actor
		for (const auto& CompItr : ActorItr->GetComponents())
		{
			FTags::RemoveKeyValuePair(CompItr, TagType, TagKey);
		}
	}
	return true;
}


///////////////////////////////////////////////////////////////////////////
// Get tag key value pairs from tag array
TMap<FString, FString> FTags::GetKeyValuePairs(const TArray<FName>& InTags, const FString& TagType)
{
	// Map of the tag properties
	TMap<FString, FString> TagProperties;

	// Iterate all the tags, check for keyword TagType
	for (const auto& TagItr : InTags)
	{
		// Copy of the current tag as FString
		FString CurrTag = TagItr.ToString();

		// Check if tag is related to the TagType
		if (CurrTag.RemoveFromStart(TagType))
		{
			// Split on semicolon
			FString CurrPair;
			while (CurrTag.Split(TEXT(";"), &CurrPair, &CurrTag))
			{
				// Split on comma
				FString CurrKey, CurrValue;
				if (CurrPair.Split(TEXT(","), &CurrKey, &CurrValue))
				{
					if (!CurrKey.IsEmpty() && !CurrValue.IsEmpty())
					{
						TagProperties.Emplace(CurrKey, CurrValue);
					}
				}
			}
		}
	}
	return TagProperties;
}

// Get tag key value pairs from actor
TMap<FString, FString> FTags::GetKeyValuePairs(AActor* Actor, const FString& TagType)
{
	return FTags::GetKeyValuePairs(Actor->Tags, TagType);
}

// Get tag key value pairs from component
TMap<FString, FString> FTags::GetKeyValuePairs(UActorComponent* Component, const FString& TagType)
{
	return FTags::GetKeyValuePairs(Component->ComponentTags, TagType);
}

// Get tag key value pairs from object
TMap<FString, FString> FTags::GetKeyValuePairs(UObject* Object, const FString& TagType)
{
	if (AActor* ObjAsAct = Cast<AActor>(Object))
	{
		return FTags::GetKeyValuePairs(ObjAsAct->Tags, TagType);
	}
	else if (UActorComponent* ObjAsActComp = Cast<UActorComponent>(Object))
	{
		return FTags::GetKeyValuePairs(ObjAsActComp->ComponentTags, TagType);
	}
	return TMap<FString, FString>();
}

///////////////////////////////////////////////////////////////////////////
// Get all objects (actor and actor components) to tag key value pairs from world
TMap<UObject*, TMap<FString, FString>> FTags::GetObjectKeyValuePairsMap(UWorld* World, const FString& TagType)
{
	// Map of actors to their tag properties
	TMap<UObject*, TMap<FString, FString>> ObjectToTagProperties;
	// Iterate all actors
	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		const TMap<FString, FString> ActorTagProperties =
			FTags::GetKeyValuePairs(ActorItr->Tags, TagType);
		// If actor has tag type and at least one property
		if (ActorTagProperties.Num() > 0)
		{
			ObjectToTagProperties.Emplace(*ActorItr, ActorTagProperties);
		}

		// Iterate components of the actor
		for (const auto& CompItr : ActorItr->GetComponents())
		{
			const TMap<FString, FString> CompTagProperties =
				FTags::GetKeyValuePairs(CompItr->ComponentTags, TagType);
			// If tag type has at least one property
			if (CompTagProperties.Num() > 0)
			{
				ObjectToTagProperties.Emplace(CompItr, CompTagProperties);
			}
		}
	}
	return ObjectToTagProperties;
}

// Get all actors to tag key value pairs from world
TMap<AActor*, TMap<FString, FString>> FTags::GetActorsToKeyValuePairs(UWorld* World, const FString& TagType)
{
	// Map of actors to their tag properties
	TMap<AActor*, TMap<FString, FString>> ActorToTagProperties;
	// Iterate all actors
	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		const TMap<FString, FString> TagProperties = FTags::GetKeyValuePairs(ActorItr->Tags, TagType);
		// If actor has tag type and at least one property
		if (TagProperties.Num() > 0)
		{
			ActorToTagProperties.Emplace(*ActorItr, TagProperties);
		}
	}
	return ActorToTagProperties;
}

// Get all components to tag key value pairs from world
TMap<UActorComponent*, TMap<FString, FString>> FTags::GetComponentsToKeyValuePairs(UWorld* World, const FString& TagType)
{
	// Map of actors to their tag properties
	TMap<UActorComponent*, TMap<FString, FString>> ComponentToTagProperties;
	// Iterate all actors
	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		// Iterate components of the actor
		for (const auto& CompItr : ActorItr->GetComponents())
		{
			const TMap<FString, FString> TagProperties = FTags::GetKeyValuePairs(CompItr->ComponentTags, TagType);
			// If tag type has at least one property
			if (TagProperties.Num() > 0)
			{
				ComponentToTagProperties.Emplace(CompItr, TagProperties);
			}
		}
	}
	return ComponentToTagProperties;
}


/////////////////////////////////////////////////////////////////////////
// Get all objects unique id (actor and actor components) to tag key value
TMap<uint32, FString> FTags::GetObjectsIdToKeyValue(UWorld* World, const FString& TagType, const FString& TagKey)
{
	// Map of actors to their tag properties
	TMap<uint32, FString> ObjectsIdToKeyValue;
	// Iterate all actors
	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		// Add to map if key is found in the actor
		FString ActValue = FTags::GetValue(*ActorItr, TagType, TagKey);
		if (!ActValue.IsEmpty())
		{
			ObjectsIdToKeyValue.Emplace(ActorItr->GetUniqueID(), ActValue);
		}

		// Iterate components of the actor
		for (const auto& CompItr : ActorItr->GetComponents())
		{
			// Add to map if key is found in the actor
			FString CompValue = FTags::GetValue(CompItr, TagType, TagKey);
			if (!CompValue.IsEmpty())
			{
				ObjectsIdToKeyValue.Emplace(CompItr->GetUniqueID(), CompValue);
			}
		}
	}
	return ObjectsIdToKeyValue;
}

// Get all objects (actor and actor components) to tag key value
TMap<UObject*, FString> FTags::GetObjectsToKeyValue(UWorld* World, const FString& TagType, const FString& TagKey)
{
	// Map of actors to their tag properties
	TMap<UObject*, FString> ObjectsToKeyValue;
	// Iterate all actors
	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		// Add to map if key is found in the actor
		FString ActValue = FTags::GetValue(*ActorItr, TagType, TagKey);
		if (!ActValue.IsEmpty())
		{
			ObjectsToKeyValue.Emplace(*ActorItr, ActValue);
		}

		// Iterate components of the actor
		for (const auto& CompItr : ActorItr->GetComponents())
		{
			// Add to map if key is found in the actor
			FString CompValue = FTags::GetValue(CompItr, TagType, TagKey);
			if (!CompValue.IsEmpty())
			{
				ObjectsToKeyValue.Emplace(CompItr, CompValue);
			}
		}
	}
	return ObjectsToKeyValue;
}

// Get all actors to tag key value
TMap<AActor*, FString> FTags::GetActorsToKeyValue(UWorld* World, const FString& TagType, const FString& TagKey)
{
	// Map of actors to their tag properties
	TMap<AActor*, FString> ActorsToKeyValue;
	// Iterate all actors
	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		// Add to map if key is found in the actor
		FString Value = FTags::GetValue(*ActorItr, TagType, TagKey);
		if (!Value.IsEmpty())
		{
			ActorsToKeyValue.Emplace(*ActorItr, Value);
		}
	}
	return ActorsToKeyValue;
}

// Get all components to tag key value
TMap<UActorComponent*, FString> FTags::GetComponentsToKeyValue(UWorld* World, const FString& TagType, const FString& TagKey)
{
	// Map of actors to their tag properties
	TMap<UActorComponent*, FString> ComponentsToKeyValue;
	// Iterate all actors
	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		// Iterate components of the actor
		for (const auto& CompItr : ActorItr->GetComponents())
		{
			// Add to map if key is found in the actor
			FString Value = FTags::GetValue(CompItr, TagType, TagKey);
			if (!Value.IsEmpty())
			{
				ComponentsToKeyValue.Emplace(CompItr, Value);
			}
		}
	}
	return ComponentsToKeyValue;
}


/////////////////////////////////////////////////////////////////////////
// Get key values to objects (actor and actor components)
TMap<FString, UObject*> FTags::GetKeyValuesToObject(UWorld* World, const FString& TagType, const FString& TagKey)
{
	// Map of actors to their tag properties
	TMap<FString, UObject*> KeyValuesToObjects;
	// Iterate all actors
	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		// Add to map if key is found in the actor
		FString ActValue = FTags::GetValue(*ActorItr, TagType, TagKey);
		if (!ActValue.IsEmpty())
		{
			KeyValuesToObjects.Emplace(ActValue, *ActorItr);
		}

		// Iterate components of the actor
		for (const auto& CompItr : ActorItr->GetComponents())
		{
			// Add to map if key is found in the actor
			FString CompValue = FTags::GetValue(CompItr, TagType, TagKey);
			if (!CompValue.IsEmpty())
			{
				KeyValuesToObjects.Emplace(CompValue, CompItr);
			}
		}
	}
	return KeyValuesToObjects;
}

// Get tag key values to actors
TMap<FString, AActor*> FTags::GetKeyValuesToActor(UWorld* World, const FString& TagType, const FString& TagKey)
{
	// Map of actors to their tag properties
	TMap<FString, AActor*> KeyValuesToActor;
	// Iterate all actors
	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		// Add to map if key is found in the actor
		FString Value = FTags::GetValue(*ActorItr, TagType, TagKey);
		if (!Value.IsEmpty())
		{
			KeyValuesToActor.Emplace(Value, *ActorItr);
		}
	}
	return KeyValuesToActor;
}

// Get tag key values to components
TMap<FString, UActorComponent*> FTags::GetKeyValuesToComponents(UWorld* World, const FString& TagType, const FString& TagKey)
{
	// Map of actors to their tag properties
	TMap<FString, UActorComponent*> KeyValuesToComponents;
	// Iterate all actors
	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		// Iterate components of the actor
		for (const auto& CompItr : ActorItr->GetComponents())
		{
			// Add to map if key is found in the actor
			FString Value = FTags::GetValue(CompItr, TagType, TagKey);
			if (!Value.IsEmpty())
			{
				KeyValuesToComponents.Emplace(Value, CompItr);
			}
		}
	}
	return KeyValuesToComponents;
}


///////////////////////////////////////////////////////////////////////////
// Get all actors with the key value pair as array
TArray<AActor*> FTags::GetActorsWithKeyValuePair(UWorld* World, const FString& TagType, const FString& TagKey, const FString& TagValue)
{
	// Array of actors
	TArray<AActor*> ActorsWithKeyValuePair;
	// Iterate all actors in the world
	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		// Add actor to array if it has tag type with key and value pair
		if (FTags::HasKeyValuePair(ActorItr->Tags, TagType, TagKey, TagValue))
		{
			ActorsWithKeyValuePair.Emplace(*ActorItr);
		}
	}
	return ActorsWithKeyValuePair;
}

// Get all actors with the key value pair as set
TSet<AActor*> FTags::GetActorsWithKeyValuePairAsSet(UWorld* World, const FString& TagType, const FString& TagKey, const FString& TagValue)
{
	// Set of actors
	TSet<AActor*> ActorsWithKeyValuePair;
	// Iterate all actors in the world
	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		// Add actor to array if it has tag type with key and value pair
		if (FTags::HasKeyValuePair(ActorItr->Tags, TagType, TagKey, TagValue))
		{
			ActorsWithKeyValuePair.Emplace(*ActorItr);
		}
	}
	return ActorsWithKeyValuePair;
}

// Get all components with the key value pair as array
TArray<UActorComponent*> FTags::GetComponentsWithKeyValuePair(UWorld* World, const FString& TagType, const FString& TagKey, const FString& TagValue)
{
	// Array of components
	TArray<UActorComponent*> ComponentsWithKeyValuePair;
	// Iterate all actors in the world
	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		// Iterate actor's components
		for (const auto& CompItr : ActorItr->GetComponents())
		{
			// Add component to container if it has tag type with key and value pair
			if (FTags::HasKeyValuePair(CompItr->ComponentTags, TagType, TagKey, TagValue))
			{
				ComponentsWithKeyValuePair.Emplace(CompItr);
			}
		}
	}
	return ComponentsWithKeyValuePair;
}

// Get all components with the key value pair as set
TSet<UActorComponent*> FTags::GetComponentsWithKeyValuePairAsSet(UWorld* World, const FString& TagType, const FString& TagKey, const FString& TagValue)
{
	// Set of components
	TSet<UActorComponent*> ComponentsWithKeyValuePair;
	// Iterate all actors in the world
	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		// Iterate actor's components
		for (const auto& CompItr : ActorItr->GetComponents())
		{
			// Add component to container if it has tag type with key and value pair
			if (FTags::HasKeyValuePair(CompItr->ComponentTags, TagType, TagKey, TagValue))
			{
				ComponentsWithKeyValuePair.Emplace(CompItr);
			}
		}
	}
	return ComponentsWithKeyValuePair;
}


///////////////////////////////////////////////////////////
// Get all actors with the key value pair	
TSet<AActor*> FTags::GetActorSetWithKeyValuePair(UWorld* World, const FString& TagType, const FString& TagKey, const FString& TagValue)
{
	return TSet<AActor*>(GetActorsWithKeyValuePair(World, TagType, TagKey, TagValue));
}

// Gets all components with the key value pair
TSet<UActorComponent*> FTags::GetComponentSetWithKeyValuePair(UWorld* World, const FString& TagType, const FString& TagKey, const FString& TagValue)
{
	TSet<UActorComponent*> ComponentsWithTag;

	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		for (auto& Component : ActorItr->GetComponents())
		{
			if (FTags::HasKeyValuePair(Component->ComponentTags, TagType, TagKey, TagValue))
			{
				ComponentsWithTag.Add(Component);
			}
		}
	}
	return ComponentsWithTag;
}


///////////////////////////////////////////////////////////////////////////
// Get all the tags from the all the actors in the world which includes the Tags of each actor's components
TMap<TWeakObjectPtr<UObject>, TArray<FTagData>> FTags::GetWorldTagsData(UWorld * World)
{
	//Declaring our map data type that stores an object reference and all the tag related data in a defined struct
	TMap<TWeakObjectPtr<UObject>, TArray<FTagData>> WorldTagsData;
	//Iterate Actors from World
	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		TWeakObjectPtr<AActor> WeakActorPtr = *ActorItr;
		TArray<FName> ActorTags = WeakActorPtr->Tags;
		TArray<FTagData> ActorTagsData;
		if (WeakActorPtr.IsValid())
		{
			//Only add new tag data to valid weak pointer objects if they contain any tags
			if (ActorTags.GetData() != nullptr)
			{
				ActorTagsData = GetObjectTagsData(ActorTags, *ActorItr);//TODO ARRAY
				WorldTagsData.Add(WeakActorPtr, ActorTagsData);
			}
			//Iterate Components Of The Actor
			for (const auto& CompItr : ActorItr->GetComponents())
			{
				TWeakObjectPtr<UActorComponent> WeakComponentPtr = CompItr;
				TArray<FName> ComponentTags = CompItr->ComponentTags;
				TArray<FTagData> ComponentTagsData;

				if (WeakComponentPtr.IsValid())
				{
					if (ComponentTags.GetData() != nullptr) {
						ComponentTagsData = GetObjectTagsData(ComponentTags, CompItr);
						WorldTagsData.Add(WeakComponentPtr, ComponentTagsData);
					}
				}
			}
		}
	}
	return WorldTagsData;
}

// Get all the Tags Data (TagType and Key/Values) from a given Object
TArray<FTagData> FTags::GetObjectTagsData(TArray<FName>& InTags, UObject* ObjectOfActorOrComponent)
{
	TArray<FTagData> ObjectsTagsData;

	for (FName Tag : InTags)
	{
		FString TagString = Tag.ToString();
		if (!TagString.IsEmpty())
		{
			FString TagType, KeyValuePairs;
			TagString.Split(TEXT(";"), &TagType, &KeyValuePairs);

			FTagData IndividualTagData;
			IndividualTagData.TagType = TagType;
			IndividualTagData.KeyValueMap = FTags::GetKeyValuePairs(ObjectOfActorOrComponent, TagType);

			ObjectsTagsData.Add(IndividualTagData);
		}
	}
	return ObjectsTagsData;

}