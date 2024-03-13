#pragma once

#include <functional>

template<typename OriginalType, typename Lambda>
auto MapArray(const TArray<OriginalType>& OriginalArray, Lambda Func) -> TArray<decltype(Func(*OriginalArray.GetData()))>
{
	TArray<decltype(Func(*OriginalArray.GetData()))> MappedArray;
    MappedArray.Reserve(OriginalArray.Num());

    for (const OriginalType& Item : OriginalArray)
    {
        MappedArray.Add(Func(Item));
    }

    return MappedArray;
}
