// Copyright 2017-2020, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#pragma once

#include "EngineMinimal.h"
#include "Misc/Guid.h"
#include "Misc/Base64.h"
#include "Ids.generated.h"

/**
* Helper functions to generate UUIDs in base64 using FGuid
* and convert them back from base64
*/
USTRUCT(BlueprintType, Category = "UIds")
struct UIDS_API FIds
{
	GENERATED_BODY()

	//////////////////////////////////////////////////////////////////////////
	// UUID Functions

	// TODO not tested
	// Encodes GUID to hex
	static FString GuidToHex(FGuid InGuid)
	{
		return FString::Printf(TEXT("%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X"),
			InGuid.A, InGuid.B >> 16, InGuid.B & 0xFFFF,
			InGuid.C >> 24, (InGuid.C >> 16) & 0xFF, (InGuid.C >> 8) & 0xFF, InGuid.C & 0XFF,
			InGuid.D >> 24, (InGuid.D >> 16) & 0XFF, (InGuid.D >> 8) & 0XFF, InGuid.D & 0XFF);
	}

	// Creates a new GUID and encodes it to hex
	static FString NewGuidInHex()
	{
		FGuid NewGuid = FGuid::NewGuid();
		return GuidToHex(NewGuid);
	}

	// TODO check size first
	// Creates GUID from hex string
	static FGuid HexToGuid(const FString& InHex)
	{
		return FGuid(
			FParse::HexNumber(*InHex.Mid(0, 8)),
			FParse::HexNumber(*InHex.Mid(8, 8)),
			FParse::HexNumber(*InHex.Mid(16, 8)),
			FParse::HexNumber(*InHex.Mid(24, 8))
		);
	}

	// Encodes GUID to Base64
	static FString GuidToBase64(FGuid InGuid)
	{		
		FBufferArchive GuidBufferArchive;							// FBufferArchive inherits from TArray<uint8>
		GuidBufferArchive << InGuid;
		FString GuidInBase64 = FBase64::Encode(GuidBufferArchive);	// Needs binary as TArray<uint8>
		GuidInBase64.RemoveFromEnd(TEXT("=="));						// Remove last unnecessary two equal characters from end
		return GuidInBase64;
	}

	// Creates a new GUID and encodes it to Base64
	static FString NewGuidInBase64()
	{
		FGuid NewGuid = FGuid::NewGuid();
		return GuidToBase64(NewGuid);
	}

	// Encodes GUID to Base64
	static FString GuidToBase64Url(FGuid InGuid)
	{
		FBufferArchive GuidBufferArchive;							// FBufferArchive inherits from TArray<uint8>
		GuidBufferArchive << InGuid;
		FString GuidInBase64 = FBase64::Encode(GuidBufferArchive);	// Needs binary as TArray<uint8>
		GuidInBase64.RemoveFromEnd(TEXT("=="));						// Remove last unnecessary two equal characters from end
		return Base64ToBase64Url(GuidInBase64);
	}

	// Creates a new GUID and encodes it to Base64Url
	static FString NewGuidInBase64Url()
	{
		FGuid NewGuid = FGuid::NewGuid();
		return GuidToBase64Url(NewGuid);
	}

	// Creates a GUID from Base64
	static FGuid Base64ToGuid(const FString& InGuidInBase64, bool bNewIfInvalid = false)
	{
		TArray<uint8> GuidBinaryArray;
		FBase64::Decode(InGuidInBase64, GuidBinaryArray);
		FMemoryReader Ar = FMemoryReader(GuidBinaryArray, true);
		Ar.Seek(0);
		FGuid LocalGuid;
		Ar << LocalGuid;
		if (bNewIfInvalid)
		{
			if (LocalGuid.IsValid())
			{
				return LocalGuid;
			}
			else
			{
				return FGuid::NewGuid();
			}
		}
		else
		{
			return LocalGuid;
		}
	}

	// Creates a GUID from Base64Url
	static FGuid Base64UrlToGuid(const FString& InGuidInBase64, bool bNewIfInvalid = false)
	{
		return Base64ToGuid(Base64UrlToBase64(InGuidInBase64), bNewIfInvalid);
	}

	// Convert Base64 to Base64Url (e.g. replace '+', '/' with '_','-')
	FORCEINLINE static FString Base64ToBase64Url(const FString& InBase64)
	{
		FString Base64Url = InBase64;
		Base64Url.ReplaceInline(TEXT("+"), TEXT("-"), ESearchCase::CaseSensitive);
		Base64Url.ReplaceInline(TEXT("/"), TEXT("_"), ESearchCase::CaseSensitive);
		return Base64Url;
	}

	// Convert Base64Url to Base64 (e.g. replace '_','-' with '+', '/')
	FORCEINLINE static FString Base64UrlToBase64(const FString& InBase64Url)
	{
		FString Base64 = InBase64Url;
		Base64.ReplaceInline(TEXT("-"), TEXT("+"), ESearchCase::CaseSensitive);
		Base64.ReplaceInline(TEXT("_"), TEXT("/"), ESearchCase::CaseSensitive);
		return Base64;
	}

	//////////////////////////////////////////////////////////////////////////
	// Pairing functions

	// Encode to 64 bit pair
	// https://stackoverflow.com/questions/26222273/is-there-a-better-implementation-for-keeping-a-count-for-unique-integer-pairs
	static uint64 PairEncodeShift(uint32 X, uint32 Y)
	{
		return 0;
		// TODO TEST
		//uint64 A = X + Y;
		//uint64 B = abs((int32)(X - Y));
		//return (uint64)(A << 32) | (B);
	}

	// Decode from 64 bit pair
	static void PairDecodeShift(uint64 InP, uint32& OutX, uint32& OutY)
	{
		// TODO TEST
		//OutX = InP >> 32;
		//OutY = InP & 0xFFFFFFFF;
	}


	// Encode to cantor pair; !! f(a,b) != f(b,a); !!
	// https://en.wikipedia.org/wiki/Pairing_function
	static uint64 PairEncodeCantor(uint32 X, uint32 Y)
	{
		return (uint64)(0.5*(X + Y)*(X + Y + 1) + Y);
	}

	// Decode to cantor pair
	// (if order is ignored the small number will alway be X)
	static void PairDecodeCantor(uint64 InP, uint32& OutX, uint32& OutY)
	{
		uint32 W = floor(((sqrt((InP * 8) + 1)) - 1) / 2);
		uint32 T = (W*(W + 1)) / 2;
		OutY = InP - T;
		OutX = W - OutY;
	}

	// Encode to Szudzik pair (if order is ignored the small number will alway be X)
	// http://szudzik.com/ElegantPairing.pdf
	static uint64 PairEncodeSzudzik(uint32 X, uint32 Y)
	{
		return 0;
		// TODO TEST
		//return X < Y ? (uint64)(Y*Y+X) : (uint64)(X*X+X+Y);
	}

	// Encode to Szudzik pair
	static void PairDecodeSzudzik(uint64 InP, uint32& OutX, uint32& OutY)
	{
		// TODO TEST
		//uint32 Q = floor(sqrt(InP));
		//uint32 L = InP - Q ^ 2;
		//if (Q < L)
		//{
		//	OutX = Q;
		//	OutY = L;
		//}
		//else
		//{
		//	OutX = Q;
		//	OutY = L - Q;
		//}
	}
};
