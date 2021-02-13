// SmallStringOptimizationTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <cstring>
#include <map>
#include <iterator>
#include <algorithm>
#include <array>
#include <type_traits>

namespace NSSO
{

std::size_t txtLength(const char* aTxt)
{
    return std::strlen(aTxt);
}

std::size_t txtLength(const wchar_t* aTxt)
{
    return std::wcslen(aTxt);
}


template<typename T>
struct CSmallStringOptStd
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
struct CSmallStringOptFast
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


template<typename TChar>
class CSmallStringOptIterator : public std::iterator<
                                    std::random_access_iterator_tag,   // iterator_category
                                    TChar,                      // value_type
                                    long,                      // difference_type
                                    TChar*,               // pointer
                                    TChar&>
{
private:
    TChar* mPtr;
public:
    explicit CSmallStringOptIterator(TChar* aPtr)
    : mPtr(aPtr) {}
    

    CSmallStringOptIterator<TChar>& operator++()
    {
        ++mPtr;
        return *this;
    }
    
    CSmallStringOptIterator<TChar> operator++(int)
    {
        CSmallStringOptIterator<TChar> retval{mPtr};
        mPtr++;
        return retval;
    }
    
    CSmallStringOptIterator<TChar>& operator--()
    {
        --mPtr;
        return *this;
    }
    
    CSmallStringOptIterator<TChar> operator--(int)
    {
        CSmallStringOptIterator<TChar> retval{mPtr};
        mPtr--;
        return retval;
    }
    
    bool operator==(const CSmallStringOptIterator<TChar>& aIter) const
    {
        return mPtr == aIter.mPtr;
    }
    
    bool operator!=(const CSmallStringOptIterator<TChar>& aIter) const
    {
        return !(*this == aIter);
    }
    
    TChar& operator*()
    {
        return *mPtr;
    }
    
    const TChar& operator*() const
    {
        return *mPtr;
    }
    
    bool operator<(const CSmallStringOptIterator<TChar>& aIter) const
    {
        return mPtr < aIter.mPtr;
    }
    
    bool operator>(const CSmallStringOptIterator<TChar>& aIter) const
    {
        return mPtr > aIter.mPtr;
    }
    
    bool operator<=(const CSmallStringOptIterator<TChar>& aIter) const
    {
        return mPtr <= aIter.mPtr;
    }
    
    bool operator>=(const CSmallStringOptIterator<TChar>& aIter) const
    {
        return mPtr >= aIter.mPtr;
    }
    
    TChar& operator[](int aIndex)
    {
        return mPtr[aIndex];
    }
    
    const TChar& operator[](int aIndex) const
    {
        return mPtr[aIndex];
    }
    
    CSmallStringOptIterator<TChar> operator+(int aDiff) const
    {
        return CSmallStringOptIterator<TChar>(mPtr + aDiff);
    }
    
    CSmallStringOptIterator<TChar> operator-(int aDiff) const
    {
        return CSmallStringOptIterator<TChar>(mPtr - aDiff);
    }

    long operator-(const CSmallStringOptIterator<TChar>& aIter) const
    {
        return mPtr - aIter.mPtr;
    }
};

template<std::size_t TSmallStringOptLength, typename TChar, typename TAllocator>
class CSmallStringOpt;

template<std::size_t TSmallStringOptLength, typename TChar = char, typename TAllocator = std::allocator<TChar>>
class CSmallStringOpt
{
public:
    using value_type = TChar;
    using size_type = std::size_t;

    using allocator_type = TAllocator;
    using pointer = TChar*;
    
    using reference = TChar&;

    using iterator = CSmallStringOptIterator<TChar>;
    using const_iterator = CSmallStringOptIterator<const TChar>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr size_type sSmallStringOptLength = TSmallStringOptLength;

private:
    using this_type = CSmallStringOpt<TSmallStringOptLength, TChar, TAllocator>;
    using this_pointer = CSmallStringOpt<TSmallStringOptLength, TChar, TAllocator>*;
    using this_const_pointer = const CSmallStringOpt<TSmallStringOptLength, TChar, TAllocator>* ;
    
private:

    bool mSmallStringOptEnable;
    size_type mLength; 
    std::array<value_type, sSmallStringOptLength> mArray;
    value_type* mDynamicArray;
    size_type mAllocatedLength;
    allocator_type mAllocator;


    template<std::size_t _TSmallStringOptLength, typename _TChar, typename _TAllocator>
    friend class CSmallStringOpt;

    void memcpy(TChar* aDest, const TChar* aSrc, size_type aLength)
    {
        //std::memcpy(aDest, aSrc, aLength);
        CSmallStringOptFast<TChar>::memcpy(aDest, aSrc, aLength);
    }
public:

    CSmallStringOpt() noexcept
        : mSmallStringOptEnable{ true }
        , mLength{0u}
        , mArray{}
        , mDynamicArray{nullptr}
        , mAllocatedLength{ 0 }
        , mAllocator{} 
    {
    }

    template<size_type TObjNoChar>
    explicit CSmallStringOpt(const CSmallStringOpt<TObjNoChar, TChar, TAllocator>& aObj)  noexcept(noexcept(mAllocator.allocate(0u)))
        : mSmallStringOptEnable(aObj.mSmallStringOptEnable)
        , mLength(aObj.mLength)
        , mAllocator{}
    {
        const size_type lengthToCopy = mLength + 1u;
        if (true == aObj.mSmallStringOptEnable)
        {
            memcpy(
                mArray.data(), 
                aObj.mArray.data(), 
                lengthToCopy);
        }
        else
        {
            mAllocatedLength = mLength + 1;
            mDynamicArray = mAllocator.allocate(mAllocatedLength);
            memcpy(
                mDynamicArray, 
                aObj.mDynamicArray, 
                lengthToCopy);
        }
    }


    explicit CSmallStringOpt(const CSmallStringOpt<TSmallStringOptLength, TChar, TAllocator>& aObj) noexcept(noexcept(mAllocator.allocate(0u)))
        : mSmallStringOptEnable(aObj.mSmallStringOptEnable)
        , mLength(aObj.mLength)
        , mArray{aObj.mArray}
        , mDynamicArray{ nullptr }
        , mAllocatedLength{ 0u }
        , mAllocator{std::move(aObj.mAllocator)}
    {
        const size_type lengthToCopy = mLength + 1u;
        if (true == aObj.mSmallStringOptEnable)
        {
            memcpy(
                mArray.data(), 
                aObj.mArray.data(), 
                lengthToCopy);
        }
        else
        {
            mAllocatedLength = mLength + 1;
            mDynamicArray = mAllocator.allocate(mAllocatedLength);
            memcpy(
                mDynamicArray, 
                aObj.mDynamicArray, 
                lengthToCopy);
        }
    }

    template<size_type TObjNoChar>
    CSmallStringOpt(CSmallStringOpt<TObjNoChar>&& aObj) noexcept(noexcept(mAllocator.allocate(0u)))
        : mSmallStringOptEnable(true)
        , mLength(aObj.mLength)
        , mArray{}
        , mDynamicArray{ nullptr }
        , mAllocatedLength{ 0u }
        , mAllocator{std::move(aObj.mAllocator)}
    {
        const auto lengthToCopy = mLength + 1u;
        if (lengthToCopy > sSmallStringOptLength)
        {
            mAllocatedLength = lengthToCopy;
            mDynamicArray = mAllocator.allocate(mAllocatedLength);
            mSmallStringOptEnable = false;
        }
        memcpy(beginPtr(), aObj.beginPtr(), lengthToCopy);
    }

    CSmallStringOpt(CSmallStringOpt<TSmallStringOptLength>&& aObj) noexcept(noexcept(mAllocator.allocate(0u)))
        : mSmallStringOptEnable(aObj.mSmallStringOptEnable)
        , mLength(aObj.mLength)
        , mArray{std::move(aObj.mArray)}
        , mDynamicArray{ aObj.mDynamicArray }
        , mAllocatedLength{ aObj.mAllocatedLength }
        , mAllocator{std::move(aObj.mAllocator)}
    {
        aObj.mSmallStringOptEnable = true;
        aObj.mLength = 0u;
        aObj.mDynamicArray = nullptr;
        aObj.mAllocatedLength = 0u;
    }

    explicit CSmallStringOpt(const TChar* aTxt) noexcept(noexcept(CSmallStringOpt(nullptr, 0u)))
        : CSmallStringOpt(aTxt, txtLength(aTxt))
    {}


    CSmallStringOpt(const TChar* aTxt, size_type aLength) noexcept(noexcept(mAllocator.allocate(0u)))
        : mSmallStringOptEnable(true)
        , mLength(aLength)
        , mArray{}
        , mDynamicArray{ nullptr }
        , mAllocatedLength{ 0 }
        , mAllocator{}
    {
        const size_type lengthToCopy = mLength + 1u;

        if (lengthToCopy <= sSmallStringOptLength)
        {
            memcpy(
                mArray.data(),
                aTxt,
                lengthToCopy);
        }
        else
        {
            mSmallStringOptEnable = false;
            mAllocatedLength = lengthToCopy;
            mDynamicArray = mAllocator.allocate(mAllocatedLength);
            memcpy(
                mDynamicArray,
                aTxt,
                lengthToCopy);
        }
    }

    explicit CSmallStringOpt(const std::basic_string<TChar>& aStr) noexcept(noexcept(CSmallStringOpt(nullptr, 0u)))
        : CSmallStringOpt(aStr.c_str(), aStr.size())
    {}


    CSmallStringOpt& operator=(const CSmallStringOpt<TSmallStringOptLength, TChar, TAllocator>& aObj) 
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
        assign(aObj);
        return *this;
	}


    template<size_type TObjNoChar>
    CSmallStringOpt& operator=(const CSmallStringOpt<TObjNoChar, TChar, TAllocator>& aObj) 
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
        assign(aObj);
        return *this;
    }

    template<size_type TObjNoChar>
    CSmallStringOpt& operator=(CSmallStringOpt<TObjNoChar>&& aObj) 
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
        const auto requiredLength = aObj.mLength + 1u;
        if (aObj.mSmallStringOptEnable)
        {
            if (requiredLength <= sSmallStringOptLength)
            {
                memcpy(mArray.data(), aObj.mArray.data(), requiredLength);
                mSmallStringOptEnable = true;
            }
            else
            {
                if (requiredLength <= mAllocatedLength)
                {
                    memcpy(mDynamicArray, aObj.mArray.data(), requiredLength);
                }
                else
                {
                    mAllocator.deallocate(mDynamicArray, mAllocatedLength);
                    mDynamicArray = mAllocator.allocate(requiredLength);
                    mAllocatedLength = requiredLength;
                    memcpy(mDynamicArray, aObj.mArray.data(), requiredLength);
                }
                mSmallStringOptEnable = false;
            }
        }
        else
        {
            std::swap(mDynamicArray, aObj.mDynamicArray);
            std::swap(mAllocatedLength, aObj.mAllocatedLength);
            aObj.mSmallStringOptEnable = mSmallStringOptEnable;
            mSmallStringOptEnable = false;
        }
        
        mLength = aObj.mLength;
        
        return *this;
    }
    
    CSmallStringOpt& operator=(const TChar* aTxt) noexcept(this->assign(nullptr, 0))
    {
        const size_type len = txtLength(aTxt);
        assign(aTxt, len);
        return *this;
    }

    CSmallStringOpt& operator=(const std::basic_string<TChar>& aTxt) noexcept(this->assign(nullptr, 0))
    {
        assign(aTxt.c_str(), aTxt.size());
        return *this;
    }

    ~CSmallStringOpt() 
    {
        mAllocator.deallocate(mDynamicArray, mAllocatedLength);
    }

    bool empty() const noexcept
    {
        return mLength == 0u;
    }

    void shrink_to_fit()
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
        if (mDynamicArray != nullptr)
        {
            if (true == mSmallStringOptEnable)
            {
                mAllocator.deallocate(mDynamicArray, mAllocatedLength);
                mDynamicArray = nullptr;
            }
            else
            {

                const size_type lengthToCopy = mLength + 1u;
                pointer ptr = mAllocator.allocate(lengthToCopy);
                memcpy(
                    ptr,
                    mDynamicArray,
                    lengthToCopy);
                mDynamicArray = ptr;
                *(beginPtr() + mLength) = '\0';
            }
        }
    }

    size_type size() const noexcept
    {
        return mLength;
    }

    const TChar* data() const noexcept
    {
        if (empty())
        {
            return nullptr;
        }
        if (true == mSmallStringOptEnable)
        {
            return mArray.data();
        }
        return mDynamicArray;
    }

    const TChar& operator[](size_type aIndex) const noexcept
    {
        if (true == mSmallStringOptEnable)
        {
            return mArray[aIndex];
        }
        return mDynamicArray[aIndex];
    }

    TChar& operator[](size_type aIndex) noexcept
    {
        if (true == mSmallStringOptEnable)
        {
            return mArray[aIndex];
        }
        return mDynamicArray[aIndex];
    }

    void resever(size_type aSize) noexcept(noexcept(mAllocator.allocate(0u)))
    {
        if (aSize > sSmallStringOptLength && aSize > mAllocatedLength)
        {
            value_type* ptr = mAllocator.allocate(aSize);
            memcpy(ptr, mDynamicArray, mLength + 1u);
            mAllocator.deallocate(mDynamicArray, mAllocatedLength);
            mAllocatedLength = aSize;
            mDynamicArray = ptr;
            *(beginPtr() + mLength) = '\0';
        }
    }

    size_type capacity() const noexcept
    {
        return std::max(sSmallStringOptLength, mAllocatedLength);
    }

    template<size_type TObjNoChar>
    void assign(const CSmallStringOpt<TObjNoChar, TChar, TAllocator>& aObj) 
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
		if (this == &aObj)
		{
			return ;
		}
        const size_type lengthToCopy = aObj.mLength + 1u;
        if (true == mSmallStringOptEnable && true == aObj.mSmallStringOptEnable)
        {
            mLength = aObj.mLength;
            memcpy(
                mArray.data(),
                aObj.mArray.data(),
                lengthToCopy - 1u);
        }
        if (false == mSmallStringOptEnable && false == aObj.mSmallStringOptEnable)
        {
            if (mAllocatedLength <= aObj.mAllocatedLength)
            {
                // enought dynamic allocated memory to copy chars.
                mLength = aObj.mLength;
                memcpy(
                    mDynamicArray,
                    aObj.mDynamicArray,
                    lengthToCopy - 1u);
            }
            else
            {
                // not enougth dynamic allocated memory to copy chars.
                mAllocator.deallocate(mDynamicArray, mAllocatedLength);
                mDynamicArray = nullptr;
                mAllocatedLength = 0u;
                mLength = aObj.mLength;
                if (lengthToCopy > sSmallStringOptLength) // there can be different size of SSO
                {
                    // source chars will not fit into small string array
                    mAllocatedLength = lengthToCopy;
                    mDynamicArray = mAllocator.allocate(mAllocatedLength);

                    memcpy(
                        mDynamicArray,
                        aObj.mDynamicArray,
                        lengthToCopy - 1u);
                }
                else
                {
                    mSmallStringOptEnable = true;
                    memcpy(
                        mArray.data(),
                        aObj.mDynamicArray,
                        lengthToCopy - 1u);
                }
            }
        }
        if (true == mSmallStringOptEnable && false == aObj.mSmallStringOptEnable)
        {
            mLength = aObj.mLength;
            if (lengthToCopy > sSmallStringOptLength)
            {
                mSmallStringOptEnable = false;
                mAllocatedLength = lengthToCopy;
                mDynamicArray = mAllocator.allocate(mAllocatedLength);

                memcpy(
                    mDynamicArray,
                    aObj.mDynamicArray,
                    lengthToCopy - 1u);
            }
            else
            {
                // small string array is enough to hold data
                memcpy(
                    mArray.data(),
                    aObj.mDynamicArray,
                    lengthToCopy - 1u);
            }
        }
        if (false == mSmallStringOptEnable && true == aObj.mSmallStringOptEnable)
        {
            mSmallStringOptEnable = true;
            mLength = aObj.mLength;
            memcpy(
                mArray.data(),
                aObj.mArray.data(),
                lengthToCopy - 1u);

        }
        *(beginPtr() + mLength) = '\0';
    }
	
    void assign(const TChar* aTxt, size_type aLength)
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
        if (aLength + 1u <= sSmallStringOptLength)
        {
            mLength = aLength;
            if (false == mSmallStringOptEnable)
            {
                mAllocator.deallocate(mDynamicArray, mAllocatedLength);
            }
            mSmallStringOptEnable = true;
            memcpy(
                mArray.data(),
                aTxt,
                mLength);
        }
        else
        {
            mLength = aLength;
            if (true == mSmallStringOptEnable)
            {
                mAllocatedLength = mLength + 1u;
                mDynamicArray = mAllocator.allocate(mAllocatedLength);
            }
            else
            {
                if (mAllocatedLength < aLength + 1u)
                {
                    mAllocator.deallocate(mDynamicArray, mAllocatedLength);
                    mAllocatedLength = mLength + 1u;
                    mDynamicArray = mAllocator.allocate(mAllocatedLength);
                }
            }
            mSmallStringOptEnable = false;
            memcpy(
                mDynamicArray,
                aTxt,
                mLength);
        }
        *(beginPtr() + mLength) = '\0';

    }

    void assign(const TChar* aTxt) noexcept(noexcept(assign(nullptr, 0u)))
    {
        const size_type len = txtLength(aTxt);
        assign(aTxt, len);
    }

    void assign(const std::basic_string<TChar>& aTxt) noexcept(noexcept(assign(nullptr, 0u)))
    {
        assign(aTxt.c_str(), aTxt.size());
    }

    
    // append
    
    template<size_type TObjNoChar>
    void append(const CSmallStringOpt<TObjNoChar, TChar, TAllocator>& aObj)
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
        
        const auto fullLength = mLength + aObj.mLength + 1u;
        if (fullLength <= sSmallStringOptLength)
        {
            memcpy(beginPtr() + mLength, aObj.beginPtr(), aObj.mLength);
            mLength += aObj.mLength;
            return;
        }
        else
        {
            TChar* currentSource = mArray.data();
            if (false == mSmallStringOptEnable)
            {
                currentSource = mDynamicArray;
            }
        
            if (mAllocatedLength < fullLength)
            {
                pointer tmp = mAllocator.allocate(fullLength);
                memcpy(tmp, currentSource, mLength);
                memcpy(tmp + mLength, aObj.beginPtr(), aObj.mLength);
                mAllocator.deallocate(mDynamicArray, mAllocatedLength);
                mAllocatedLength = fullLength;
                mDynamicArray = tmp;
            }
            else
            {
                memcpy(mDynamicArray, currentSource, mLength);
                memcpy(mDynamicArray + mLength, aObj.beginPtr(), aObj.mLength);
            }
            
            mSmallStringOptEnable = false;
            mLength += aObj.mLength;
        }
        *(beginPtr() + mLength) = '\0';
    }
    
    void append(const TChar* aTxt)
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
        const auto len = txtLength(aTxt);
        append(aTxt, len);
    }
    
    void append(const TChar* aTxt, size_type aLen)
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
        
        const auto fullLength = mLength + aLen + 1u;
        if (fullLength <= sSmallStringOptLength)
        {
            memcpy(beginPtr() + mLength, aTxt, aLen);
            mLength += aLen;
            mSmallStringOptEnable = true;
        }
        else
        {
            TChar* currentSource = mArray.data();
            if (false == mSmallStringOptEnable)
            {
                currentSource = mDynamicArray;
            }
        
            if (mAllocatedLength < fullLength)
            {
                pointer tmp = mAllocator.allocate(fullLength);
                memcpy(tmp, currentSource, mLength);
                memcpy(tmp + mLength, aTxt, aLen);
                mAllocator.deallocate(mDynamicArray, mAllocatedLength);
                mAllocatedLength = fullLength;
                mDynamicArray = tmp;
            }
            else
            {
                memcpy(mDynamicArray, currentSource, mLength);
                memcpy(mDynamicArray + mLength, aTxt, aLen);
            }
            
            mSmallStringOptEnable = false;
            mLength += aLen;
        }
        *(beginPtr() + mLength) = '\0';
    }
    
    void append(const std::basic_string<TChar>& aTxt)
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
        append(aTxt.data(), aTxt.size());
    }
    
    template<size_type TObjNoChar>
    CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& operator+=(const CSmallStringOpt<TObjNoChar, TChar, TAllocator>& aObj)
    {
        append(aObj);
        return *this;
    }
    
    CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& operator+=(const TChar* aTxt)
    {
        append(aTxt);
        return *this;
    }
    
    CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& operator+=(const std::basic_string<TChar>& aTxt)
    {
        append(aTxt);
        return *this;
    }
    
    
    
    template<size_type TObjNoChar>
    CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator> operator+(const CSmallStringOpt<TObjNoChar, TChar, TAllocator>& aObj)
    {
        CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator> output;
        output.resever(mLength + aObj.mLength + 1u);
        output.append(*this);
        output.append(aObj);
        return output;
    }
    
    CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& operator+(const TChar* aTxt)
    {
        CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator> output;
        const auto len = txtLength(aTxt);
        output.resever(mLength + len + 1u);
        output.append(*this);
        output.append(aTxt, len);
        return output;
    }
    
    CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator> operator+(const std::basic_string<TChar>& aTxt)
    {
        CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator> output;
        output.resever(mLength + aTxt.size() + 1u);
        output.append(*this);
        output.append(aTxt);
        return output;
    }
    
    TChar& front() noexcept
    {
        // effective C++
        return const_cast<TChar&>((static_cast<this_const_pointer>(this))->front());
    }

    const TChar& front() const noexcept
    {
        if (true == mSmallStringOptEnable)
        {
            return mArray[0];
        }
        return mDynamicArray[0];
    }

    TChar& back() noexcept
    {
        // effective C++
        return const_cast<TChar&>((static_cast<this_const_pointer>(this))->back());
    }

    const TChar& back() const noexcept
    {
        if (true == mSmallStringOptEnable)
        {
            return mArray[mLength - 1];
        }
        return mDynamicArray[mLength - 1];
    }
    
    const_iterator begin() const noexcept
    {
        return const_iterator{beginPtr()};
    }

    iterator begin() noexcept
    {
        return iterator{beginPtr()};
    }

    const_iterator cbegin() const noexcept
    {
        return begin();
    }

    const_iterator end() const noexcept
    {
        return const_iterator{endPtr()};
    }

    iterator end() noexcept
    {
        return iterator{endPtr()};
    }

    const_iterator cend() const noexcept
    {
        return end();
    }

    const_reverse_iterator rbegin() const noexcept
    {
       return std::make_reverse_iterator(endPtr());
    }

    reverse_iterator rbegin() noexcept
    {
       return std::make_reverse_iterator(endPtr());
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return rbegin();
    }

    const_reverse_iterator rend() const noexcept
    {
       return std::make_reverse_iterator(beginPtr());
    }

    reverse_iterator rend() noexcept
    {
       return std::make_reverse_iterator(beginPtr());
    }

    const_reverse_iterator crend() const noexcept
    {
        return rend();
    }
   
private:
    
    
    TChar* beginPtr() noexcept
    {
        return const_cast<TChar*>((static_cast<this_const_pointer>(this))->beginPtr());
    }

    const TChar* beginPtr() const noexcept
    {
        if (true == mSmallStringOptEnable)
        {
            return mArray.data();
        }
        return mDynamicArray;
    }
    
    TChar* endPtr() noexcept
    {
        return const_cast<TChar*>((static_cast<this_const_pointer>(this))->endPtr());
    }
    
    
    const TChar* endPtr() const noexcept
    {
        if (true == mSmallStringOptEnable)
        {
            return &mArray[mLength];
        }
        return &mDynamicArray[mLength];
    }
};
// /////////////////////////////////////////

template<
    std::size_t sSmallStringOptLength1, 
    std::size_t sSmallStringOptLength2, 
    typename TChar, 
    typename TAllocator1,
    typename TAllocator2>
bool operator==(
    const CSmallStringOpt<sSmallStringOptLength1, TChar, TAllocator1>& aObj1, 
    const CSmallStringOpt<sSmallStringOptLength2, TChar, TAllocator2>& aObj2) noexcept
{
    if (reinterpret_cast<const void*>(&aObj1) == reinterpret_cast<const void*>(&aObj2))
    {
        return true;
    }
    if (aObj1.size() != aObj2.size())
    {
        return false;
    }
    return CSmallStringOptFast<TChar>::equal(&(*aObj1.begin()), &(*aObj1.end()), &(*aObj2.begin()));
   // return std::equal(aObj1.begin(), aObj1.end(), aObj2.begin());
}

template<
    std::size_t sSmallStringOptLength,
    typename TChar,
    typename TAllocator>
    bool operator==(
        const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj1,
        const std::basic_string<TChar>& aObj2) noexcept
{
    if (aObj1.size() != aObj2.size())
    {
        return false;
    }
    return std::equal(aObj1.begin(), aObj1.end(), aObj2.begin());
}

template<
    std::size_t sSmallStringOptLength,
    typename TChar,
    typename TAllocator>
    bool operator==(
        const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj1,
        const TChar* aArray) noexcept
{
    const auto len = txtLength(aArray);
    if (aObj1.size() != len)
    {
        return false;
    }
    return std::equal(aObj1.begin(), aObj1.end(), aArray);
}

///
template<
    std::size_t sSmallStringOptLength, 
    typename TChar, 
    typename TAllocator>
bool operator==(
    const std::basic_string<TChar>& aObj1,
    const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj2) noexcept
{
    if (aObj1.size() != aObj2.size())
    {
        return false;
    }
    return std::equal(aObj1.begin(), aObj1.end(), aObj2.begin());
}

template<
    std::size_t sSmallStringOptLength, 
    typename TChar, 
    typename TAllocator>
bool operator==(
    const TChar* aArray,
    const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj) noexcept
{
    return aObj == aArray;
}


//////////////////////////////////// !=
template<
    std::size_t sSmallStringOptLength1, 
    std::size_t sSmallStringOptLength2, 
    typename TChar, 
    typename TAllocator1,
    typename TAllocator2>
bool operator!=(
    const CSmallStringOpt<sSmallStringOptLength1, TChar, TAllocator1>& aObj1, 
    const CSmallStringOpt<sSmallStringOptLength2, TChar, TAllocator2>& aObj2) noexcept
{
    return !(aObj1 == aObj2);
}

template<
    std::size_t sSmallStringOptLength, 
    typename TChar, 
    typename TAllocator>
bool operator!=(
    const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj1, 
    const std::basic_string<TChar>& aObj2) noexcept
{
    return !(aObj1 == aObj2);
}

template<
    std::size_t sSmallStringOptLength,
    typename TChar,
    typename TAllocator>
bool operator!=(
    const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj1,
    const TChar* aArray) noexcept
{
    return !(aObj1 == aArray);
}


//////


template<
    std::size_t sSmallStringOptLength, 
    typename TChar, 
    typename TAllocator>
bool operator!=(
    const std::basic_string<TChar>& aObj1,
    const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj2) noexcept
{
    return !(aObj1 == aObj2);
}

template<
    std::size_t sSmallStringOptLength, 
    typename TChar, 
    typename TAllocator>
bool operator!=(
    const TChar* aArray,
    const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj) noexcept
{
    return !(aArray == aObj);
}




//////////////////////////////////// <


template<
    std::size_t sSmallStringOptLength1,
    std::size_t sSmallStringOptLength2,
    typename TChar,
    typename TAllocator1,
    typename TAllocator2>
bool operator<(
    const CSmallStringOpt<sSmallStringOptLength1, TChar, TAllocator1>& aObj1, 
    const CSmallStringOpt<sSmallStringOptLength2, TChar, TAllocator2>& aObj2) noexcept
{
    if (reinterpret_cast<const void*>(&aObj1) == reinterpret_cast<const void*>(&aObj2))
    {
        return false;
    }

    return std::lexicographical_compare(aObj1.begin(), aObj1.end(),
        aObj2.begin(), aObj2.end());
}

template<
    std::size_t sSmallStringOptLength, 
    typename TChar, 
    typename TAllocator>
bool operator<(
    const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj1, 
    const std::basic_string<TChar>& aObj2) noexcept
{
    return std::lexicographical_compare(aObj1.begin(), aObj1.end(),
        aObj2.begin(), aObj2.end());
}

template<
    std::size_t sSmallStringOptLength,
    typename TChar,
    typename TAllocator>
    bool operator<(
        const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj1,
        const TChar* aArray) noexcept
{
    const auto len = txtLength(aArray);
    return std::lexicographical_compare(aObj1.begin(), aObj1.end(),
        aArray, aArray + len);
}

//////
template<
    std::size_t sSmallStringOptLength, 
    typename TChar, 
    typename TAllocator>
bool operator<(
    const std::basic_string<TChar>& aObj1,
    const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj2) noexcept
{
    return std::lexicographical_compare(aObj1.begin(), aObj1.end(),
        aObj2.begin(), aObj2.end());
}

template<
    std::size_t sSmallStringOptLength,
    typename TChar,
    typename TAllocator>
bool operator<(
    const TChar* aArray,
    const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj) noexcept
{
    const auto len = txtLength(aArray);

    return std::lexicographical_compare(aArray, aArray + len,
        aObj.begin(), aObj.end());
}


//////////////////////////////////// >



template<
    std::size_t sSmallStringOptLength1,
    std::size_t sSmallStringOptLength2,
    typename TChar,
    typename TAllocator1,
    typename TAllocator2>
bool operator>(
    const CSmallStringOpt<sSmallStringOptLength1, TChar, TAllocator1>& aObj1, 
    const CSmallStringOpt<sSmallStringOptLength2, TChar, TAllocator2>& aObj2) noexcept
{
    return aObj2 < aObj1;
}

template<
    std::size_t sSmallStringOptLength, 
    typename TChar, 
    typename TAllocator>
bool operator>(
    const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj1, 
    const std::basic_string<TChar>& aObj2) noexcept
{
    return aObj2 < aObj1;
}

template<
    std::size_t sSmallStringOptLength,
    typename TChar,
    typename TAllocator>
    bool operator>(
        const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj1,
        const TChar* aArray) noexcept
{
    return aArray < aObj1;
}



/////
template<
    std::size_t sSmallStringOptLength, 
    typename TChar, 
    typename TAllocator>
bool operator>(
    const std::basic_string<TChar>& aObj1,
    const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj2) noexcept
{
    return aObj2 < aObj1;
}

template<
    std::size_t sSmallStringOptLength,
    typename TChar,
    typename TAllocator>
bool operator>(
    const TChar* aArray,
    const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj2) noexcept
{
    return aObj2 < aArray;
}



//////////////////////////////////// >=

template<
    std::size_t sSmallStringOptLength1,
    std::size_t sSmallStringOptLength2,
    typename TChar,
    typename TAllocator1,
    typename TAllocator2>
bool operator>=(
    const CSmallStringOpt<sSmallStringOptLength1, TChar, TAllocator1>& aObj1, 
    const CSmallStringOpt<sSmallStringOptLength2, TChar, TAllocator2>& aObj2) noexcept
{
    return !(aObj1 < aObj2);
}

template<
    std::size_t sSmallStringOptLength, 
    typename TChar, 
    typename TAllocator>
bool operator>=(
    const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj1, 
    const std::basic_string<TChar>& aObj2) noexcept
{
    return !(aObj1 < aObj2);
}

template<
    std::size_t sSmallStringOptLength,
    typename TChar,
    typename TAllocator>
    bool operator>=(
        const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj1,
        const TChar* aArray) noexcept
{
    return !(aObj1 < aArray);
}



///////////

template<
    std::size_t sSmallStringOptLength, 
    typename TChar, 
    typename TAllocator>
bool operator>=(
    const std::basic_string<TChar>& aObj1,
    const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj2) noexcept
{
    return !(aObj1 < aObj2);
}

template<
    std::size_t sSmallStringOptLength,
    typename TChar,
    typename TAllocator>
bool operator>=(
    const TChar* aArray,
    const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj) noexcept
{
    return !(aArray < aObj);
}


//////////////////////////////////// <=

template<
    std::size_t sSmallStringOptLength1,
    std::size_t sSmallStringOptLength2,
    typename TChar,
    typename TAllocator1,
    typename TAllocator2>
bool operator<=(
    const CSmallStringOpt<sSmallStringOptLength1, TChar, TAllocator1>& aObj1, 
    const CSmallStringOpt<sSmallStringOptLength2, TChar, TAllocator2>& aObj2) noexcept
{
    return !(aObj1 > aObj2);
}

template<
    std::size_t sSmallStringOptLength, 
    typename TChar, 
    typename TAllocator>
bool operator<=(
    const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj1, 
    const std::basic_string<TChar>& aObj2) noexcept
{
    return !(aObj1 > aObj2);
}


template<
    std::size_t sSmallStringOptLength,
    typename TChar,
    typename TAllocator>
    bool operator<=(
        const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj1,
        const TChar* aArray) noexcept
{
    return !(aObj1 > aArray);
}


/////
template<
    std::size_t sSmallStringOptLength, 
    typename TChar, 
    typename TAllocator>
bool operator<=(
    const std::basic_string<TChar>& aObj1,
    const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj2) noexcept
{
    return !(aObj1 > aObj2);
}

template<
    std::size_t sSmallStringOptLength,
    typename TChar,
    typename TAllocator>
bool operator<=(
    const TChar* aArray,
    const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj) noexcept
{
    return !(aArray > aObj);
}

////

////////////////////////////////////  +
#if 0
template<
    std::size_t sSmallStringOptLength1,
    std::size_t sSmallStringOptLength2,
    typename TChar,
    typename TAllocator1,
    typename TAllocator2>
bool operator+=(
    const CSmallStringOpt<sSmallStringOptLength1, TChar, TAllocator1>& aObj1,
    const CSmallStringOpt<sSmallStringOptLength2, TChar, TAllocator2>& aObj2) noexcept
{
    return !(aObj1 > aObj2);
}

template<
    std::size_t sSmallStringOptLength,
    typename TChar,
    typename TAllocator>
bool operator<=(
    const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj1,
    const std::basic_string<TChar>& aObj2) noexcept
{
    return !(aObj1 > aObj2);
}


template<
    std::size_t sSmallStringOptLength,
    typename TChar,
    typename TAllocator>
    bool operator<=(
        const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj1,
        const TChar* aArray) noexcept
{
    return !(aObj1 > aArray);
}


/////
template<
    std::size_t sSmallStringOptLength,
    typename TChar,
    typename TAllocator>
bool operator<=(
    const std::basic_string<TChar>& aObj1,
    const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj2) noexcept
{
    return !(aObj1 > aObj2);
}

template<
    std::size_t sSmallStringOptLength,
    typename TChar,
    typename TAllocator>
bool operator<=(
    const TChar* aArray,
    const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aObj) noexcept
{
    return !(aArray > aObj);
}
#endif


template<typename TCharStream, std::size_t TNoChar, typename TChar>
std::basic_ostream<TCharStream, std::char_traits<TCharStream>>&
    operator<<(std::basic_ostream<TCharStream, std::char_traits<TCharStream>>& aStream, const CSmallStringOpt<TNoChar, TChar>& aString)
{
    aStream << aString.data();
    return aStream;
}
} // namespace NSSO
