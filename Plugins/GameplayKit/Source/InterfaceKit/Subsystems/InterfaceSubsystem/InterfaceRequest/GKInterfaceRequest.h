#pragma once

#include "InterfaceKit/GKInterfaceComponent.h"

/** A list of FNames to represent an object path. Used for fast hashing and comparison of paths */
struct FInterfaceClassPath
{
	TArray<FName> Path;
	
	TSoftClassPtr<UGKInterfaceComponent> InterfaceClass;

	FInterfaceClassPath(const TSoftClassPtr<UGKInterfaceComponent>& InSoftClassPtr)
	{
		TArray<FString> StringPath;
		InSoftClassPtr.ToString().ParseIntoArray(StringPath, TEXT("."));
		Path.Reserve(StringPath.Num());
		for (const FString& StringPathElement : StringPath)
		{
			Path.Add(FName(*StringPathElement));
		}

		InterfaceClass = InSoftClassPtr;
	}

#if !UE_BUILD_SHIPPING
	FString ToDebugString() const
	{
		FString ReturnString;
		if (Path.Num() > 0)
		{
			ReturnString = Path[0].ToString();
			for (int32 PathIdx = 1; PathIdx < Path.Num(); ++PathIdx)
			{
				ReturnString += TEXT(".") + Path[PathIdx].ToString();
			}
		}

		return ReturnString;
	}
#endif // !UE_BUILD_SHIPPING

	bool operator==(const FInterfaceClassPath& Other) const
	{
		return Path == Other.Path;
	}

	friend FORCEINLINE uint32 GetTypeHash(const FInterfaceClassPath& Request)
	{
		uint32 ReturnHash = 0;
		for (const FName& PathElement : Request.Path)
		{
			ReturnHash ^= GetTypeHash(PathElement);
		}
		return ReturnHash;
	}
};