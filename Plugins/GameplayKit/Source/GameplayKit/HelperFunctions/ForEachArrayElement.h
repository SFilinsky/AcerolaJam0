#pragma once

#include <functional>

template<typename OriginalType, typename Lambda>
void ForEachArrayElement(const TArray<OriginalType>& OriginalArray, Lambda Func)
{
    for (const OriginalType& Item : OriginalArray)
    {
        Func(Item);
    }
}
