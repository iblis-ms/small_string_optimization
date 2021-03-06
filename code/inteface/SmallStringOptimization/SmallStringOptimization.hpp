
/*
 * Author: Marcin Serwach
 * https://github.com/iblis-ms
 * License: MIT
 */

/*
 * Simple implementation of small string optimization structure.
 */

#ifndef SMALL_STRING_OPTIMIZATION_HPP_
#define SMALL_STRING_OPTIMIZATION_HPP_

#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <array>
#include <type_traits>
#include "internal/InternalHelper.hpp"

namespace NSSO
{


/**
 * @brief Iterator class for SmallStringOpt implementation.
 * 
 * @tparam TChar Character type.
 */
template<typename TChar>
class CSmallStringOptIterator : public std::iterator<
                                    std::random_access_iterator_tag,   // iterator_category
                                    TChar,                      // value_type
                                    long,                      // difference_type
                                    TChar*,               // pointer
                                    TChar&>
{
private:

    /**
     * @brief Pointer to character in internal buffer of SmallStringOpt.
     */
    TChar* mPtr;

public:

    /**
     * @brief Construct a new iterator.
     * 
     * @param aPtr Pointer to character in an internal buffer of SmallStringOpt.
     */
    explicit CSmallStringOptIterator(TChar* aPtr)
    : mPtr(aPtr) {}
    
    /**
     * @brief Move iterator to a next character and return it.
     * @return Moved iterator.
     */
    CSmallStringOptIterator<TChar>& operator++()
    {
        ++mPtr;
        return *this;
    }
    
    /**
     * @brief Move iterator to a next character but return an iterator instance before moving.
     * @return Iterator instance before moving.
     */
    CSmallStringOptIterator<TChar> operator++(int)
    {
        CSmallStringOptIterator<TChar> retval{mPtr};
        mPtr++;
        return retval;
    }
    
    /**
     * @brief Move iterator to a previous character and return it.
     * @return Moved iterator.
     */
    CSmallStringOptIterator<TChar>& operator--()
    {
        --mPtr;
        return *this;
    }
    
    /**
     * @brief Move iterator to a previous character but return an iterator instance before moving.
     * @return Iterator instance before moving.
     */
    CSmallStringOptIterator<TChar> operator--(int)
    {
        CSmallStringOptIterator<TChar> retval{mPtr};
        mPtr--;
        return retval;
    }
    
    /**
     * @brief Check if iterators point to the same character in the same SmallStringOpt instance.
     * 
     * @param aIter Iterator to compare.
     * @return true if there are point to the same memory location.
     */
    bool operator==(const CSmallStringOptIterator<TChar>& aIter) const
    {
        return mPtr == aIter.mPtr;
    }
    
    /**
     * @brief Check if iterators point not to the same character in the same SmallStringOpt instance.
     * 
     * @param aIter Iterator to compare.
     * @return true if there are point not to the same memory location.
     */
    bool operator!=(const CSmallStringOptIterator<TChar>& aIter) const
    {
        return !(*this == aIter);
    }
    
    /**
     * @brief Retrieve pointed character.
     * 
     * @return pointed character.
     */
    TChar& operator*()
    {
        return *mPtr;
    }
    
    /**
     * @brief Retrieve pointed character.
     * 
     * @return pointed character.
     */
    const TChar& operator*() const
    {
        return *mPtr;
    }
    
    /**
     * @brief Check if this iterator points to charecter closer to the beginning of SmallStringOpt instance than the given one.
     * 
     * @param aIter Iterator to compare.
     * @return true iterator points to charecter closer to the beginning of SmallStringOpt instance than the given one.
     */
    bool operator<(const CSmallStringOptIterator<TChar>& aIter) const
    {
        return mPtr < aIter.mPtr;
    }
    
    /**
     * @brief Check if this iterator points to charecter closer to the end of SmallStringOpt instance than the given one.
     * 
     * @param aIter Iterator to compare.
     * @return true iterator points to charecter closer to the end of SmallStringOpt instance than the given one.
     */
    bool operator>(const CSmallStringOptIterator<TChar>& aIter) const
    {
        return mPtr > aIter.mPtr;
    }
    
    /**
     * @brief Check if this iterator points to charecter closer/equal to te beginning of SmallStringOpt instance than the given one.
     * 
     * @param aIter Iterator to compare.
     * @return true iterator points to charecter closer/equal to te beginning of SmallStringOpt instance than the given one.
     */
    bool operator<=(const CSmallStringOptIterator<TChar>& aIter) const
    {
        return mPtr <= aIter.mPtr;
    }
    
    /**
     * @brief Check if this iterator points to charecter closer/equal to te end of SmallStringOpt instance than the given one.
     * 
     * @param aIter Iterator to compare.
     * @return true iterator points to charecter closer/equal to te end of SmallStringOpt instance than the given one.
     */
    bool operator>=(const CSmallStringOptIterator<TChar>& aIter) const
    {
        return mPtr >= aIter.mPtr;
    }
    
    /**
     * @brief Return a character at the given offset.
     * 
     * @param aIndex Offset.
     * @return Character at the given offset.
     */
    TChar& operator[](int aIndex)
    {
        return mPtr[aIndex];
    }
    
    /**
     * @brief Return a character at the given offset.
     * 
     * @param aIndex Offset.
     * @return Character at the given offset.
     */
    const TChar& operator[](int aIndex) const
    {
        return mPtr[aIndex];
    }
    
    /**
     * @brief Return an iterator moved by the given offset from this itertor.
     * 
     * @param aDiff Offset.
     * @return Iterator moved by the given offset from this itertor.
     */
    CSmallStringOptIterator<TChar> operator+(int aDiff) const
    {
        return CSmallStringOptIterator<TChar>(mPtr + aDiff);
    }
    
    /**
     * @brief Return an iterator moved by the given offset from this itertor.
     * 
     * @param aDiff Offset.
     * @return Iterator moved by the given offset from this itertor.
     */
    CSmallStringOptIterator<TChar> operator-(int aDiff) const
    {
        return CSmallStringOptIterator<TChar>(mPtr - aDiff);
    }

    /**
     * @brief Return a difference between this iterator and the given one.
     * 
     * @param aIter Iterator to compare.
     * @return long The difference.
     */
    long operator-(const CSmallStringOptIterator<TChar>& aIter) const
    {
        return mPtr - aIter.mPtr;
    }
};

template<std::size_t TSmallStringOptLength, typename TChar, typename TAllocator>
class CSmallStringOpt;

/**
 * @brief Implementation of small string optimization algorithm. Most of functions/typedefs are analoque to std::string.
 * 
 * @tparam TSmallStringOptLength The size of characters array, that holds small strings.
 * @tparam TChar Type of character.
 * @tparam TAllocator Allocator.
 */
template<std::size_t TSmallStringOptLength, typename TChar = char, typename TAllocator = std::allocator<TChar>>
class CSmallStringOpt
{
public:
    /**
     * @brief Character type.
     */
    using value_type = TChar;

    /**
     * @brief Size type.
     */
    using size_type = std::size_t;

    /**
     * @brief Allocator type.
     */
    using allocator_type = TAllocator;

    /**
     * @brief Pointer to character type.
     */
    using pointer = TChar*;
    
    /**
     * @brief Type of reference to character type.
     */
    using reference = TChar&;

    /**
     * @brief Iterator type.
     */
    using iterator = CSmallStringOptIterator<TChar>;

    /**
     * @brief Constant iterator type.
     */
    using const_iterator = CSmallStringOptIterator<const TChar>;

    /**
     * @brief Reverse iterator type.
     */
    using reverse_iterator = std::reverse_iterator<iterator>;

    /**
     * @brief Constant reverse iterator type.
     */
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    /**
     * @brief Length of characters used for small string optimization.
     */
    static constexpr size_type sSmallStringOptLength = TSmallStringOptLength;

private:

    /**
     * @brief Type use for casts.
     */
    using this_const_pointer = const CSmallStringOpt<TSmallStringOptLength, TChar, TAllocator>* ;
    
private:

    /**
     * @brief True if characters are stored in compilation time array. False if characters are in a dynamic array.
     */
    bool mSmallStringOptEnable;

    /**
     * @brief Number of characters in string.
     */
    size_type mLength; 

    /**
     * @brief Array used for small string optimizaton.
     */
    std::array<value_type, sSmallStringOptLength> mArray;

    /**
     * @brief Dynamic array to store longer strings.
     */
    value_type* mDynamicArray;

    /**
     * @brief Length of a dynamic allocated memory.
     */
    size_type mAllocatedLength;

    /**
     * @brief Allocator.
     */
    allocator_type mAllocator;


    template<std::size_t _TSmallStringOptLength, typename _TChar, typename _TAllocator>
    friend class CSmallStringOpt;

    /**
     * @brief Internal memory copy function
     * 
     * @param aDest Destination address.
     * @param aSrc  Source address.
     * @param aLength Number of characters to copy
     */
    void internalMemcpy(TChar* aDest, const TChar* aSrc, size_type aLength)
    {
        CFast<TChar>::memcpy(aDest, aSrc,  aLength * sizeof(TChar));
    }

public:

    /**
     * @brief Construct empy string.
     */
    CSmallStringOpt() noexcept
        : mSmallStringOptEnable{ true }
        , mLength{0u}
        , mArray{}
        , mDynamicArray{nullptr}
        , mAllocatedLength{ 0 }
        , mAllocator{} 
    {
    }

    /**
     * @brief Construct a string.
     * 
     * @tparam TObjNoChar Number of characters used for small string optimization in the given object.
     * @param aObj String to copy.
     */
    template<size_type TObjNoChar>
    explicit CSmallStringOpt(const CSmallStringOpt<TObjNoChar, TChar, TAllocator>& aObj)  noexcept(noexcept(mAllocator.allocate(0u)))
        : mSmallStringOptEnable{ true }
        , mLength{0u}
        , mArray{}
        , mDynamicArray{nullptr}
        , mAllocatedLength{ 0 }
        , mAllocator{} 
    {
        const size_type lengthToCopy = mLength + 1u;
        TChar* ptr = nullptr;
        if (sSmallStringOptLength < lengthToCopy)
        {
            mAllocatedLength = lengthToCopy;
            mDynamicArray = mAllocator.allocate(lengthToCopy);
            mSmallStringOptEnable = false;
        }
        internalMemcpy(
            beginPtr(),
            aObj.beginPtr(), 
            lengthToCopy);

    }

    /**
     * @brief Construct string. Copy constructor required even there is template based constructor. Copy constructor needs to be declared.
     * 
     * @param aObj string to copy.
     */
    explicit CSmallStringOpt(const CSmallStringOpt<TSmallStringOptLength, TChar, TAllocator>& aObj) noexcept(noexcept(mAllocator.allocate(0u)))
        : mSmallStringOptEnable{ aObj.mSmallStringOptEnable }
        , mLength{aObj.mLength}
        , mArray{}
        , mDynamicArray{nullptr}
        , mAllocatedLength{ 0 }
        , mAllocator{} 
    {
        const size_type lengthToCopy = mLength + 1u;
        if (lengthToCopy > sSmallStringOptLength)
        {
            mAllocatedLength = lengthToCopy;
            mDynamicArray = mAllocator.allocate(lengthToCopy);
            mSmallStringOptEnable = false;
        }
        internalMemcpy(
            beginPtr(),
            aObj.beginPtr(), 
            lengthToCopy);
    }

    /**
     * @brief Construct string
     * 
     * @tparam TObjNoChar Number of characters used for small string optimization in the given object.
     * @param aObj String to move.
     */
    template<size_type TObjNoChar>
    CSmallStringOpt(CSmallStringOpt<TObjNoChar>&& aObj) noexcept(noexcept(mAllocator.allocate(0u)))
        : mSmallStringOptEnable(true)
        , mLength(aObj.mLength)
        , mArray{}
        , mDynamicArray{ nullptr }
        , mAllocatedLength{ 0u }
        , mAllocator{}
    {
        const auto lengthToCopy = mLength + 1u;
        if (lengthToCopy > sSmallStringOptLength)
        {
            std::swap(mAllocatedLength, aObj.mAllocatedLength);
            std::swap(mDynamicArray, aObj.mDynamicArray);
            mSmallStringOptEnable = false;
        }
        else
        {
            internalMemcpy(mArray.data(), aObj.beginPtr(), lengthToCopy);
        }
    }

    /**
     * @brief Construct string. Move constructor required even there is template based constructor. Move constructor needs to be declared.
     * 
     * @param aObj String to move.
     */
    CSmallStringOpt(CSmallStringOpt<TSmallStringOptLength>&& aObj) noexcept(noexcept(mAllocator.allocate(0u)))
        : mSmallStringOptEnable(true)
        , mLength(aObj.mLength)
        , mArray{}
        , mDynamicArray{ nullptr }
        , mAllocatedLength{ 0u }
        , mAllocator{}
    {
        const auto lengthToCopy = mLength + 1u;
        if (lengthToCopy > sSmallStringOptLength)
        {
            std::swap(mAllocatedLength, aObj.mAllocatedLength);
            std::swap(mDynamicArray, aObj.mDynamicArray);
            mSmallStringOptEnable = false;
        }
        else
        {
            internalMemcpy(mArray.data(), aObj.beginPtr(), lengthToCopy);
        }
    }

    /**
     * @brief Construct string by copying string from given array.
     * 
     * @param aTxt String array.
     */
    explicit CSmallStringOpt(const TChar* aTxt) noexcept(noexcept(CSmallStringOpt(nullptr, 0u)))
        : CSmallStringOpt(aTxt, txtLength(aTxt))
    {}

    /**
     * @brief Construct string by copying string from given array.
     * 
     * @param aTxt String array.
     * @param aLength Length of string. Don't need to be null terminated.
     */
    CSmallStringOpt(const TChar* aTxt, size_type aLength) noexcept(noexcept(mAllocator.allocate(0u)))
        : mSmallStringOptEnable(true)
        , mLength(aLength)
        , mArray{}
        , mDynamicArray{ nullptr }
        , mAllocatedLength{ 0 }
        , mAllocator{}
    {
        const size_type lengthToCopy = aLength + 1u;
        TChar* ptr = mArray.data();
        if (lengthToCopy > sSmallStringOptLength)
        {
            mSmallStringOptEnable = false;
            mAllocatedLength = lengthToCopy;
            mDynamicArray = mAllocator.allocate(mAllocatedLength);
            ptr = mDynamicArray;
        }
        internalMemcpy(
            ptr,
            aTxt,
            aLength);
        ptr[aLength] = '\0';
    }

    /**
     * @brief Construct string from std::string.
     * 
     * @param aStr std::string object.
     */
    explicit CSmallStringOpt(const std::basic_string<TChar>& aStr) noexcept(noexcept(CSmallStringOpt(nullptr, 0u)))
        : CSmallStringOpt(aStr.c_str(), aStr.size())
    {}

    /**
     * @brief Assignment operator. 
     * 
     * @param aObj Object to copy its character to this string.
     * @return CSmallStringOpt& This object.
     */
    CSmallStringOpt& operator=(const CSmallStringOpt<TSmallStringOptLength, TChar, TAllocator>& aObj) 
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
        assign(aObj);
        return *this;
	}

    /**
     * @brief Assignment operator. 
     * 
     * @tparam TObjNoChar Number of characters used for small string optimization in the given object.
     * @param aObj Object to copy its character to this string.
     * @return CSmallStringOpt& This object.
     */
    template<size_type TObjNoChar>
    CSmallStringOpt& operator=(const CSmallStringOpt<TObjNoChar, TChar, TAllocator>& aObj) 
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
        assign(aObj);
        return *this;
    }

    /**
     * @brief Move assignment operator. 
     * 
     * @tparam TObjNoChar Number of characters used for small string optimization in the given object.
     * @param aObj Object to copy/move its character to this string.
     * @return CSmallStringOpt& This object.
     */
    template<size_type TObjNoChar>
    CSmallStringOpt& operator=(CSmallStringOpt<TObjNoChar>&& aObj) 
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
        const auto requiredLength = aObj.mLength + 1u;
        if (aObj.mSmallStringOptEnable)
        {
            if (requiredLength <= sSmallStringOptLength)
            {
                internalMemcpy(mArray.data(), aObj.mArray.data(), requiredLength);
                mSmallStringOptEnable = true;
            }
            else
            {
                if (requiredLength <= mAllocatedLength)
                {
                    internalMemcpy(mDynamicArray, aObj.mArray.data(), requiredLength);
                }
                else
                {
                    mAllocator.deallocate(mDynamicArray, mAllocatedLength);
                    mAllocatedLength = requiredLength;
                    mDynamicArray = mAllocator.allocate(requiredLength);
                    internalMemcpy(mDynamicArray, aObj.mArray.data(), requiredLength);
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
    
    /**
     * @brief Assignment operator.
     * 
     * @param aTxt String array to copy.
     * @return CSmallStringOpt& This object.
     */
    CSmallStringOpt& operator=(const TChar* aTxt) noexcept(this->assign(nullptr, 0))
    {
        assign(aTxt);
        return *this;
    }
    
    /**
     * @brief Assignment operator.
     * 
     * @param aTxt std::string to copy.
     * @return CSmallStringOpt& This object.
     */
    CSmallStringOpt& operator=(const std::basic_string<TChar>& aTxt) noexcept(this->assign(nullptr, 0))
    {
        assign(aTxt);
        return *this;
    }

    /**
     * @brief Destroy the instance. Deallocates memory if required.
     * 
     */
    ~CSmallStringOpt() 
    {
        mAllocator.deallocate(mDynamicArray, mAllocatedLength);
    }

    /**
     * @brief Indicate if string is empty.
     * 
     * @return true if empty.
     */
    bool empty() const noexcept
    {
        return mLength == 0u;
    }

    /**
     * @brief Deallocate or shrink dynamic memory if not required.
     */
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
            else if (mAllocatedLength > mLength + 1u)
            {
                const size_type lengthToCopy = mLength + 1u;
                pointer ptr = mAllocator.allocate(lengthToCopy);
                internalMemcpy(
                    ptr,
                    mDynamicArray,
                    lengthToCopy);
                mAllocator.deallocate(mDynamicArray, mAllocatedLength);
                mDynamicArray = ptr;
                mAllocatedLength = lengthToCopy;
            }
        }
    }

    /**
     * @brief Length of string.
     */
    size_type size() const noexcept
    {
        return mLength;
    }

    /**
     * @brief Return pointer to string array.
     * 
     * @return Pointer to string array.
     */
    const TChar* data() const noexcept
    {
        if (true == mSmallStringOptEnable)
        {
            return mArray.data();
        }
        return mDynamicArray;
    }

    /**
     * @brief Return character on the given position.
     * 
     * @param aIndex Index
     * @return Character on the given position.
     */
    const TChar& operator[](size_type aIndex) const noexcept
    {
        if (true == mSmallStringOptEnable)
        {
            return mArray[aIndex];
        }
        return mDynamicArray[aIndex];
    }

    /**
     * @brief Return character on the given position.
     * 
     * @param aIndex Index
     * @return Character on the given position.
     */
    TChar& operator[](size_type aIndex) noexcept
    {
        if (true == mSmallStringOptEnable)
        {
            return mArray[aIndex];
        }
        return mDynamicArray[aIndex];
    }

    /**
     * @brief Reserve dynamic memory to store the given number of characters without memory reallocaton.
     * 
     * @param aSize Size of reserved memory in a number of characters.
     */
    void reserve(size_type aSize) noexcept(noexcept(mAllocator.allocate(0u)))
    {
        if (aSize > sSmallStringOptLength && aSize > mAllocatedLength)
        {
            value_type* ptr = mAllocator.allocate(aSize);
            internalMemcpy(ptr, mDynamicArray, mLength + 1u);
            mAllocator.deallocate(mDynamicArray, mAllocatedLength);
            mAllocatedLength = aSize;
            mDynamicArray = ptr;
        }
    }

    /**
     * @brief Return size of the buffer dynamic or compilation time array - the bigger one.
     * 
     * @return Size of the buffer dynamic or compilation time array  
     */
    size_type capacity() const noexcept
    {
        return std::max(sSmallStringOptLength, mAllocatedLength);
    }

    /**
     * @brief Assign string to this object.
     * 
     * @tparam TObjNoChar 
     * @param aObj Object to assign.
     */
    template<size_type TObjNoChar>
    void assign(const CSmallStringOpt<TObjNoChar, TChar, TAllocator>& aObj) 
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
		if (this == &aObj)
		{
			return ;
		}
        assign(aObj.beginPtr(), aObj.mLength);
    }
	
    /**
     * @brief Assign the given string array.
     * 
     * @param aTxt String array.
     * @param aLength Length.
     */
    void assign(const TChar* aTxt, size_type aLength)
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
        const size_type lengthToCopy = aLength + 1u;
        TChar* ptr = nullptr;
        if (lengthToCopy < sSmallStringOptLength)
        {
            ptr = mArray.data();
            mSmallStringOptEnable = true;
        }
        else
        {
            if (mAllocatedLength < lengthToCopy)
            {
                mAllocator.deallocate(mDynamicArray, mAllocatedLength);
                mAllocatedLength = lengthToCopy;
                mDynamicArray = mAllocator.allocate(mAllocatedLength);
            }
            ptr = mDynamicArray;
            mSmallStringOptEnable = false;
        }

        internalMemcpy(
            ptr,
            aTxt,
            aLength);
        ptr[aLength] = '\0';
        mLength = aLength;
    }

    /**
     * @brief Assign the given string to this object
     * 
     * @param aTxt String.
     */
    void assign(const TChar* aTxt) noexcept(noexcept(assign(nullptr, 0u)))
    {
        const size_type len = NSSO::txtLength(aTxt);
        assign(aTxt, len);
    }

    /**
     * @brief Assign the given string to this object
     * 
     * @param aTxt String.
     */
    void assign(const std::basic_string<TChar>& aTxt) noexcept(noexcept(assign(nullptr, 0u)))
    {
        assign(aTxt.c_str(), aTxt.size());
    }

    
    /**
     * @brief Append the given string object.
     * 
     * @tparam TObjNoChar 
     * @param aObj String object to append.
     */
    template<size_type TObjNoChar>
    void append(const CSmallStringOpt<TObjNoChar, TChar, TAllocator>& aObj)
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
        append(aObj.beginPtr(), aObj.mLength);
        
    }
    
    /**
     * @brief Append the given string to this object.
     * 
     * @param aTxt String.
     */
    void append(const TChar* aTxt)
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
        const auto len = NSSO::txtLength(aTxt);
        append(aTxt, len);
    }
    
    /**
     * @brief Append the given string to this object.
     * 
     * @param aTxt String to append.
     * @param aLen Lenght of string to append. 
     */
    void append(const TChar* aTxt, size_type aLen)
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
        const auto fullLength = mLength + aLen + 1u;
        TChar* ptr = nullptr;
        if (fullLength <= sSmallStringOptLength)
        {
            ptr = &mArray[mLength];
        }
        else
        {
            if (mAllocatedLength < fullLength)
            {
                auto buf = mAllocator.allocate(fullLength);
                internalMemcpy(
                    buf,
                    beginPtr(),
                    mLength);
                mAllocator.deallocate(mDynamicArray, mAllocatedLength);
                mAllocatedLength = fullLength;
                mDynamicArray = buf;
            }
            ptr = mDynamicArray + mLength;
            mSmallStringOptEnable = false;
        }

        mLength += aLen;
        internalMemcpy(
            ptr,
            aTxt,
            aLen);
        ptr[aLen] = '\0';
    }
    
    /**
     * @brief Append the given string to this object.
     * 
     * @param aTxt String.
     */
    void append(const std::basic_string<TChar>& aTxt)
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
        append(aTxt.data(), aTxt.size());
    }
    
    /**
     * @brief Append the given string to this object.
     * 
     * @tparam TObjNoChar 
     * @param aObj Stirng to append.
     * @return This object with a concatenated string.
     */
    template<size_type TObjNoChar>
    CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& operator+=(const CSmallStringOpt<TObjNoChar, TChar, TAllocator>& aObj)
    {
        append(aObj);
        return *this;
    }
    
    /**
     * @brief Append the given string to this object.
     * 
     * @param aTxt String to append.
     * @return This object with a concatenated string.
     */
    CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& operator+=(const TChar* aTxt)
    {
        append(aTxt);
        return *this;
    }

    /**
     * @brief Append the given string to this object.
     * 
     * @param aTxt String to append.
     * @return This object with a concatenated string.
     */
    CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& operator+=(const std::basic_string<TChar>& aTxt)
    {
        append(aTxt);
        return *this;
    }
    
    
    /**
     * @brief Add the given string.
     * 
     * @tparam TObjNoChar 
     * @param aObj String to add.
     * @return This object with a concatenated string.
     */
    template<size_type TObjNoChar>
    CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator> operator+(const CSmallStringOpt<TObjNoChar, TChar, TAllocator>& aObj)
    {
        CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator> output;
        output.resever(mLength + aObj.mLength + 1u);
        output.append(*this);
        output.append(aObj);
        return output;
    }
    
    /**
     * @brief Add the given string.
     * 
     * @param aTxt String to add.
     * @return This object with a concatenated string.
     */
    CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& operator+(const TChar* aTxt)
    {
        CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator> output;
        const auto len = NSSO::txtLength(aTxt);
        output.resever(mLength + len + 1u);
        output.append(*this);
        output.append(aTxt, len);
        return output;
    }
    
    /**
     * @brief Add string.
     * 
     * @param aTxt String to add.
     * @return This object with a concatenated string.
     */
    CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator> operator+(const std::basic_string<TChar>& aTxt)
    {
        CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator> output;
        output.resever(mLength + aTxt.size() + 1u);
        output.append(*this);
        output.append(aTxt);
        return output;
    }
    
    /**
     * @brief Return the first character in string.
     * 
     * @return The first character in string.
     */
    TChar& front() noexcept
    {
        // effective C++
        return const_cast<TChar&>((static_cast<this_const_pointer>(this))->front());
    }
    
    /**
     * @brief Return the first character in string.
     * 
     * @return The first character in string.
     */
    const TChar& front() const noexcept
    {
        if (true == mSmallStringOptEnable)
        {
            return mArray[0];
        }
        return mDynamicArray[0];
    }
    
    /**
     * @brief Return the last character in string.
     * 
     * @return The last character in string.
     */
    TChar& back() noexcept
    {
        // effective C++
        return const_cast<TChar&>((static_cast<this_const_pointer>(this))->back());
    }

    /**
     * @brief Return the last character in string.
     * 
     * @return The last character in string.
     */
    const TChar& back() const noexcept
    {
        if (true == mSmallStringOptEnable)
        {
            return mArray[mLength - 1];
        }
        return mDynamicArray[mLength - 1];
    }
    
    /**
     * @brief Return an iterator to the beginning of the string.
     * 
     * @return Iterator to the beginning of the string.
     */
    const_iterator begin() const noexcept
    {
        return const_iterator{beginPtr()};
    }

    /**
     * @brief Return an iterator to the beginning of the string.
     * 
     * @return Iterator to the beginning of the string.
     */
    iterator begin() noexcept
    {
        return iterator{beginPtr()};
    }

    /**
     * @brief Return an iterator to the beginning of the string.
     * 
     * @return Iterator to the beginning of the string.
     */
    const_iterator cbegin() const noexcept
    {
        return begin();
    }

    /**
     * @brief Return an iterator to the beginning of the string.
     * 
     * @return Iterator to the beginning of the string.
     */
    const_iterator end() const noexcept
    {
        return const_iterator{endPtr()};
    }

    /**
     * @brief Return an iterator to the character after the last one.
     * 
     * @return Iterator to the character after the last one.
     */
    iterator end() noexcept
    {
        return iterator{endPtr()};
    }

    /**
     * @brief Return an iterator to the character after the last one.
     * 
     * @return Iterator to the character after the last one.
     */
    const_iterator cend() const noexcept
    {
        return end();
    }

    /**
     * @brief Return a reverse iterator to iterate from the last character to the first one.
     * 
     * @return Teverse iterator to iterate from the last character to the first one.
     */
    const_reverse_iterator rbegin() const noexcept
    {
       return std::make_reverse_iterator(endPtr());
    }

    /**
     * @brief Return a reverse iterator to iterate from the last character to the first one.
     * 
     * @return Teverse iterator to iterate from the last character to the first one.
     */
    reverse_iterator rbegin() noexcept
    {
       return std::make_reverse_iterator(endPtr());
    }

    /**
     * @brief Return a reverse iterator to iterate from the last character to the first one.
     * 
     * @return Teverse iterator to iterate from the last character to the first one.
     */
    const_reverse_iterator crbegin() const noexcept
    {
        return rbegin();
    }

    /**
     * @brief Ruturn a reverse iterator that points before the first character.
     * 
     * @return Iterator that points before the first character. 
     */
    const_reverse_iterator rend() const noexcept
    {
       return std::make_reverse_iterator(beginPtr());
    }

    /**
     * @brief Ruturn a reverse iterator that points before the first character.
     * 
     * @return Iterator that points before the first character. 
     */
    reverse_iterator rend() noexcept
    {
       return std::make_reverse_iterator(beginPtr());
    }

    /**
     * @brief Ruturn a reverse iterator that points before the first character.
     * 
     * @return Iterator that points before the first character. 
     */
    const_reverse_iterator crend() const noexcept
    {
        return rend();
    }
   
private:
    
    /**
     * @brief Return pointer to the first character in string.
     * 
     * @return Pointer to the first character in string.
     */
    TChar* beginPtr() noexcept
    {
        return const_cast<TChar*>((static_cast<this_const_pointer>(this))->beginPtr());
    }

    /**
     * @brief Return pointer to the first character in string.
     * 
     * @return Pointer to the first character in string.
     */
    const TChar* beginPtr() const noexcept
    {
        if (true == mSmallStringOptEnable)
        {
            return mArray.data();
        }
        return mDynamicArray;
    }
    
    /**
     * @brief Return pointer to the character after the last.
     * 
     * @return Pointer to the character after the last.
     */
    TChar* endPtr() noexcept
    {
        return const_cast<TChar*>((static_cast<this_const_pointer>(this))->endPtr());
    }
    
    /**
     * @brief Return pointer to the character after the last.
     * 
     * @return Pointer to the character after the last.
     */
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


/**
 * @brief Compare strings.
 * 
 * @tparam sSmallStringOptLength1 Length of small string optimization array of the 1st string.
 * @tparam sSmallStringOptLength2 Length of small string optimization array of the 2nd string.
 * @tparam TChar Character type.
 * @tparam TAllocator1 Type of allocator of the first string.
 * @tparam TAllocator2 Type of allocator of the second string.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the given strings contain the same characters.
 */
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
    return std::equal(&(*aObj1.begin()), &(*aObj1.end()), &(*aObj2.begin()));
}

/**
 * @brief Compares strings.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if there are equal.
 */
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

/**
 * @brief Compares string.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aArray String array to compare.
 * @return true if there are equal.
 */
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

/**
 * @brief Compares string.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if there are equal.
 */
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

/**
 * @brief Compares string.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aArray First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if there are equal.
 */
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


/**
 * @brief Compare strings.
 * 
 * @tparam sSmallStringOptLength1 Length of small string optimization array of the 1st string.
 * @tparam sSmallStringOptLength2 Length of small string optimization array of the 2nd string.
 * @tparam TChar Character type.
 * @tparam TAllocator1 Type of allocator of the first string.
 * @tparam TAllocator2 Type of allocator of the second string.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the given strings don't contain the same characters.
 */
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

/**
 * @brief Compares strings.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if there are equal.
 */
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

/**
 * @brief Compares string.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aArray String array to compare.
 * @return true if there are equal.
 */
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


/**
 * @brief Compares string.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if there are not equal.
 */
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

/**
 * @brief Compares string.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aArray First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if there are not equal.
 */
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

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam sSmallStringOptLength1 Length of small string optimization array of the 1st string.
 * @tparam sSmallStringOptLength2 Length of small string optimization array of the 2nd string.
 * @tparam TChar Character type.
 * @tparam TAllocator1 Type of allocator of the first string.
 * @tparam TAllocator2 Type of allocator of the second string.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly before the second string.
 */
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

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly before the second string.
 */
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

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aArray String array to compare.
 * @return true if the first string is a lexicographicaly before the second string.
 */
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

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly before the second string.
 */
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

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aArray First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly before the second string.
 */
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



/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam sSmallStringOptLength1 Length of small string optimization array of the 1st string.
 * @tparam sSmallStringOptLength2 Length of small string optimization array of the 2nd string.
 * @tparam TChar Character type.
 * @tparam TAllocator1 Type of allocator of the first string.
 * @tparam TAllocator2 Type of allocator of the second string.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly after the second string.
 */
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

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly after the second string.
 */
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

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aArray String array to compare.
 * @return true if the first string is a lexicographicaly after the second string.
 */
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

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly after the second string.
 */
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

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aArray First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly after the second string.
 */
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

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam sSmallStringOptLength1 Length of small string optimization array of the 1st string.
 * @tparam sSmallStringOptLength2 Length of small string optimization array of the 2nd string.
 * @tparam TChar Character type.
 * @tparam TAllocator1 Type of allocator of the first string.
 * @tparam TAllocator2 Type of allocator of the second string.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly after or equal the second string.
 */
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

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly after or equal the second string.
 */
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

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aArray String array to compare.
 * @return true if the first string is a lexicographicaly after or equal the second string.
 */
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

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly after or equal the second string.
 */
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

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aArray First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly after or equal the second string.
 */
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

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam sSmallStringOptLength1 Length of small string optimization array of the 1st string.
 * @tparam sSmallStringOptLength2 Length of small string optimization array of the 2nd string.
 * @tparam TChar Character type.
 * @tparam TAllocator1 Type of allocator of the first string.
 * @tparam TAllocator2 Type of allocator of the second string.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly before or equal the second string.
 */
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

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly before or equal the second string.
 */
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

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aArray String array to compare.
 * @return true if the first string is a lexicographicaly before or equal the second string.
 */
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
/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly before or equal the second string.
 */
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

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam sSmallStringOptLength Length of small string optimization array of the 1st string.
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aArray First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly before or equal the second string.
 */
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
/**
 * @brief Push characters to the stream.
 * 
 * @tparam TCharStream Type of characters in the stream.
 * @tparam sSmallStringOptLength Length of small string optimization array of the string.
 * @tparam TChar type of characters in the string.
 * @tparam TAllocator Allocator type.
 * @param aStream Stream
 * @param aString String
 * 
 * @return Stream.
 */
template<
    typename TCharStream, 
    std::size_t sSmallStringOptLength, 
    typename TChar,
    typename TAllocator>
std::basic_ostream<TCharStream, std::char_traits<TCharStream>>&
    operator<<(
        std::basic_ostream<TCharStream, std::char_traits<TCharStream>>& aStream, 
        const CSmallStringOpt<sSmallStringOptLength, TChar, TAllocator>& aString)
{
    aStream << aString.data();
    return aStream;
}

} // namespace NSSO

#endif // SMALL_STRING_OPTIMIZATION_HPP_
