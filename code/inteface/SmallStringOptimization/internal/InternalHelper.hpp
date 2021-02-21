
/*
 * Author: Marcin Serwach
 * https://github.com/iblis-ms
 * License: MIT
 */
#ifndef INTERNAL_HELPER_HPP_
#define INTERNAL_HELPER_HPP_
#include <cstring>
#include <map>
#include <iterator>
#include <algorithm>

namespace NSSO
{

inline std::size_t txtLength(const char* aTxt)
{
    return std::strlen(aTxt);
}

inline std::size_t txtLength(const wchar_t* aTxt)
{
    return std::wcslen(aTxt);
}


template<typename T>
struct CStd
{
    static T* memcpy(T* aDest, const T* aSrc, std::size_t aLength)
    {
        return std::memcpy(aDest, aSrc, aLength);
    }
    
    
    static void equal(const T* aBeg1, const T* aEnd1, const T* aBeg2)
    {
    }
};

template<typename T, typename TFastType = long>
struct CFast
{
    static T* memcpy(T* aDest, const T* aSrc, std::size_t aLength)
    {
        const char* srcPtrByte = reinterpret_cast<const char*>(aSrc);
        char* destPtrByte = reinterpret_cast<char*>(aDest);
        
        const TFastType* srcPtr = reinterpret_cast<const TFastType*>(srcPtrByte);
        TFastType* destPtr = reinterpret_cast<TFastType*>(destPtrByte);
        
        const auto full = aLength / sizeof(TFastType);
        const auto notFull = aLength % sizeof(TFastType);
        
        for (std::size_t i = 0; i < full; ++i)
        {
            *destPtr++ = *srcPtr++;
        }
        
        const char* restSrcPtrByte = srcPtrByte + sizeof(TFastType) * full;
        char* restDescPtrByte = destPtrByte + sizeof(TFastType) * full;
        
        for (std::size_t i = 0; i < notFull; ++i)
        {
            *restDescPtrByte++ = *restSrcPtrByte++;
        }
        return aDest;
    }
    
    static bool equal(const T* aBeg1, const T* aEnd1, const T* aBeg2)
    {
        const char* begPtrByte1 = reinterpret_cast<const char*>(aBeg1);
        const char* begPtrByte2 = reinterpret_cast<const char*>(aBeg2);
        const char* endPtrByte1 = reinterpret_cast<const char*>(aEnd1);
        
        const auto length = endPtrByte1 - begPtrByte1;
        const TFastType* begPtr1 = reinterpret_cast<const TFastType*>(begPtrByte1);
        const TFastType* begPtr2 = reinterpret_cast<const TFastType*>(begPtrByte2);
        
        const auto full = length / sizeof(TFastType);
        const auto notFull = length % sizeof(TFastType);
        
        for (std::size_t i = 0; i < full; ++i)
        {
            if (*begPtr1++ != *begPtr2++)
            {
                return false;
            }
        }
        
        const char* restSrcPtrByte1 = begPtrByte1 + sizeof(TFastType) * full;
        const char* restSrcPtrByte2 = begPtrByte2 + sizeof(TFastType) * full;
        
        for (std::size_t i = 0; i < notFull; ++i)
        {
            if (*restSrcPtrByte1++ != *restSrcPtrByte2++)
            {
                return false;
            }
        }
        return true;
    }
};


} // namespace NSSO


#endif // INTERNAL_HELPER_HPP_
