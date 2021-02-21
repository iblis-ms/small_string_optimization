
/*
 * Author: Marcin Serwach
 * https://github.com/iblis-ms
 * License: MIT
 */

#ifndef SIMPLE_STRING_HPP_
#define SIMPLE_STRING_HPP_

#include <iostream>
#include <string>
#include <cstring>
#include <map>
#include <iterator>
#include <algorithm>
#include <array>
#include <type_traits>
#include "internal/InternalHelper.hpp"

namespace NSSO
{



template<typename TChar>
class CSimpleStringIterator : public std::iterator<
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
    explicit CSimpleStringIterator(TChar* aPtr)
    : mPtr(aPtr) {}
    
    /**
     * @brief Move iterator to a next character and return it.
     * @return Moved iterator.
     */
    CSimpleStringIterator<TChar>& operator++()
    {
        ++mPtr;
        return *this;
    }
    
    /**
     * @brief Move iterator to a next character but return an iterator instance before moving.
     * @return Iterator instance before moving.
     */
    CSimpleStringIterator<TChar> operator++(int)
    {
        CSimpleStringIterator<TChar> retval{mPtr};
        mPtr++;
        return retval;
    }
    
    /**
     * @brief Move iterator to a previous character and return it.
     * @return Moved iterator.
     */
    CSimpleStringIterator<TChar>& operator--()
    {
        --mPtr;
        return *this;
    }
    
    /**
     * @brief Move iterator to a previous character but return an iterator instance before moving.
     * @return Iterator instance before moving.
     */
    CSimpleStringIterator<TChar> operator--(int)
    {
        CSimpleStringIterator<TChar> retval{mPtr};
        mPtr--;
        return retval;
    }
    
    /**
     * @brief Check if iterators point to the same character in the same SmallStringOpt instance.
     * 
     * @param aIter Iterator to compare.
     * @return true if there are point to the same memory location.
     */
    bool operator==(const CSimpleStringIterator<TChar>& aIter) const
    {
        return mPtr == aIter.mPtr;
    }
    
    /**
     * @brief Check if iterators point not to the same character in the same SmallStringOpt instance.
     * 
     * @param aIter Iterator to compare.
     * @return true if there are point not to the same memory location.
     */
    bool operator!=(const CSimpleStringIterator<TChar>& aIter) const
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
    bool operator<(const CSimpleStringIterator<TChar>& aIter) const
    {
        return mPtr < aIter.mPtr;
    }
    
    /**
     * @brief Check if this iterator points to charecter closer to the end of SmallStringOpt instance than the given one.
     * 
     * @param aIter Iterator to compare.
     * @return true iterator points to charecter closer to the end of SmallStringOpt instance than the given one.
     */
    bool operator>(const CSimpleStringIterator<TChar>& aIter) const
    {
        return mPtr > aIter.mPtr;
    }
    
    /**
     * @brief Check if this iterator points to charecter closer/equal to te beginning of SmallStringOpt instance than the given one.
     * 
     * @param aIter Iterator to compare.
     * @return true iterator points to charecter closer/equal to te beginning of SmallStringOpt instance than the given one.
     */
    bool operator<=(const CSimpleStringIterator<TChar>& aIter) const
    {
        return mPtr <= aIter.mPtr;
    }
    
    /**
     * @brief Check if this iterator points to charecter closer/equal to te end of SmallStringOpt instance than the given one.
     * 
     * @param aIter Iterator to compare.
     * @return true iterator points to charecter closer/equal to te end of SmallStringOpt instance than the given one.
     */
    bool operator>=(const CSimpleStringIterator<TChar>& aIter) const
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
    CSimpleStringIterator<TChar> operator+(int aDiff) const
    {
        return CSimpleStringIterator<TChar>(mPtr + aDiff);
    }
    
    /**
     * @brief Return an iterator moved by the given offset from this itertor.
     * 
     * @param aDiff Offset.
     * @return Iterator moved by the given offset from this itertor.
     */
    CSimpleStringIterator<TChar> operator-(int aDiff) const
    {
        return CSimpleStringIterator<TChar>(mPtr - aDiff);
    }

    /**
     * @brief Return a difference between this iterator and the given one.
     * 
     * @param aIter Iterator to compare.
     * @return long The difference.
     */
    long operator-(const CSimpleStringIterator<TChar>& aIter) const
    {
        return mPtr - aIter.mPtr;
    }
};



template<typename TChar = char, typename TAllocator = std::allocator<TChar>>
class CSimpleString
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
    using iterator = CSimpleStringIterator<TChar>;

    /**
     * @brief Constant iterator type.
     */
    using const_iterator = CSimpleStringIterator<const TChar>;

    /**
     * @brief Reverse iterator type.
     */
    using reverse_iterator = std::reverse_iterator<iterator>;

    /**
     * @brief Constant reverse iterator type.
     */
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;



private:

    /**
     * @brief Allocator.
     */
    allocator_type mAllocator;

    /**
     * @brief Number of characters in string.
     */
    size_type mLength;

    /**
     * @brief Length of a dynamic allocated memory.
     */
    size_type mAllocatedLength;

    /**
     * @brief Dynamic array to store longer strings.
     */
    value_type* mDynamicArray;



    void internalMemcpy(TChar* aDest, const TChar* aSrc, size_type aLength)
    {
        CFast<TChar>::memcpy(aDest, aSrc,  aLength * sizeof(TChar));
    }

public:

    /**
     * @brief Construct empy string.
     */
    CSimpleString() noexcept
        : mAllocator{} 
        , mLength{0u}
        , mAllocatedLength{ 0 }
        , mDynamicArray{nullptr}
    {
    }

    /**
     * @brief Construct a string.
     * 
     * @param aObj String to copy.
     */
    explicit CSimpleString(const CSimpleString<TChar, TAllocator>& aObj)  noexcept(noexcept(mAllocator.allocate(0u)))
        : CSimpleString(aObj.mDynamicArray, aObj.mLength)
    {
    }

    CSimpleString(CSimpleString<TChar, TAllocator>&& aObj) noexcept(noexcept(mAllocator.allocate(0u)))
        : mAllocator{} 
        , mLength{aObj.mLength}
        , mAllocatedLength{aObj.mAllocatedLength}
        , mDynamicArray{aObj.mDynamicArray}
    {
        aObj.mDynamicArray = nullptr;
        aObj.mAllocatedLength = 0u;
    }

    /**
     * @brief Construct string. 
     *
     * @param aObj string to copy.
     */
    explicit CSimpleString(const TChar* aTxt) noexcept(noexcept(CSimpleString(nullptr, 0u)))
        : CSimpleString(aTxt, txtLength(aTxt))
    {}

    /**
     * @brief Construct string by copying string from given array.
     * 
     * @param aTxt String array.
     * @param aLength Length of string. Don't need to be null terminated.
     */
    CSimpleString(const TChar* aTxt, size_type aLength) noexcept(noexcept(mAllocator.allocate(0u)))
        : mAllocator{} 
        , mLength{aLength}
        , mAllocatedLength{aLength + 1u}
        , mDynamicArray{mAllocator.allocate(mAllocatedLength)}
    {
        internalMemcpy(
            mDynamicArray, 
            aTxt, 
            mLength);
        mDynamicArray[mLength] = '\0';
    }

    /**
     * @brief Construct string from std::string.
     * 
     * @param aStr std::string object.
     */
    explicit CSimpleString(const std::basic_string<TChar>& aStr) noexcept(noexcept(CSimpleString(nullptr, 0u)))
        : CSimpleString(aStr.c_str(), aStr.size())
    {}

    /**
     * @brief Assignment operator. 
     * 
     * @param aObj Object to copy its character to this string.
     * @return CSmallStringOpt& This object.
     */
    CSimpleString& operator=(const CSimpleString<TChar, TAllocator>& aObj) 
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
    CSimpleString& operator=(CSimpleString<TChar, TAllocator>&& aObj) 
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
        std::swap(mDynamicArray, aObj.mDynamicArray);
        std::swap(mAllocatedLength, aObj.mAllocatedLength);
        std::swap(mLength, aObj.mLength);
        return *this;
    }
    
    /**
     * @brief Assignment operator.
     * 
     * @param aTxt String array to copy.
     * @return CSmallStringOpt& This object.
     */
    CSimpleString& operator=(const TChar* aTxt) noexcept(this->assign(nullptr, 0))
    {
        const size_type len = txtLength(aTxt);
        assign(aTxt, len);
        return *this;
    }

    /**
     * @brief Assignment operator.
     * 
     * @param aTxt std::string to copy.
     * @return CSmallStringOpt& This object.
     */
    CSimpleString& operator=(const std::basic_string<TChar>& aTxt) noexcept(this->assign(nullptr, 0))
    {
        assign(aTxt.c_str(), aTxt.size());
        return *this;
    }

    ~CSimpleString() 
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

        if (mLength < mAllocatedLength)
        {
            const auto lengthToCopy = mLength + 1u;
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
        if (empty())
        {
            return "";
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
        return mDynamicArray[aIndex];
    }

    void resever(size_type aSize) noexcept(noexcept(mAllocator.allocate(0u)))
    {
        if (aSize > mAllocatedLength)
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
        return mAllocatedLength;
    }

    void assign(const CSimpleString<TChar, TAllocator>& aObj) 
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
		if (this == &aObj)
		{
			return ;
		}
        assign(aObj.mDynamicArray, aObj.mLength);
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
        
        if (mAllocatedLength <= lengthToCopy)
        {
            mAllocator.deallocate(mDynamicArray, mAllocatedLength);
            mAllocatedLength = lengthToCopy;
            mDynamicArray = mAllocator.allocate(mAllocatedLength);
        }

        mLength = aLength;
        internalMemcpy(
            mDynamicArray,
            aTxt,
            aLength);
        mDynamicArray[aLength] = '\0';
    }

    /**
     * @brief Assign the given string to this object
     * 
     * @param aTxt String.
     */
    void assign(const TChar* aTxt) noexcept(noexcept(assign(nullptr, 0u)))
    {
        const size_type len = txtLength(aTxt);
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

    
    // append

    /**
     * @brief Append the given string object.
     * 
     * @param aObj String object to append.
     */
    void append(const CSimpleString<TChar, TAllocator>& aObj)
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
        append(aObj.mDynamicArray, aObj.mLength);
    }
    
    /**
     * @brief Append the given string to this object.
     * 
     * @param aTxt String.
     */
    void append(const TChar* aTxt)
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
        const auto len = txtLength(aTxt);
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
        if (mAllocatedLength <= fullLength)
        {
            mAllocatedLength = fullLength;
            auto ptr = mAllocator.allocate(fullLength);
            internalMemcpy(ptr, mDynamicArray, mLength);
            mAllocator.deallocate(mDynamicArray, mAllocatedLength);
            mDynamicArray = ptr;
        }
        internalMemcpy(mDynamicArray + mLength, aTxt, aLen);
        mLength += aLen;
        mDynamicArray[mLength] = '\0';
    }
    
    void append(const std::basic_string<TChar>& aTxt)
        noexcept(noexcept(mAllocator.allocate(0u)) && noexcept(mAllocator.deallocate(nullptr, 0u)))
    {
        append(aTxt.data(), aTxt.size());
    }
    
    /**
     * @brief Append the given string to this object.
     * 
     * @param aObj Stirng to append.
     * @return This object with a concatenated string.
     */
    CSimpleString<TChar, TAllocator>& operator+=(const CSimpleString<TChar, TAllocator>& aObj)
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
    CSimpleString<TChar, TAllocator>& operator+=(const TChar* aTxt)
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
    CSimpleString<TChar, TAllocator>& operator+=(const std::basic_string<TChar>& aTxt)
    {
        append(aTxt);
        return *this;
    }
    
    
    /**
     * @brief Add the given string.
     *  
     * @param aObj String to add.
     * @return This object with a concatenated string.
     */
    CSimpleString<TChar, TAllocator> operator+(const CSimpleString<TChar, TAllocator>& aObj)
    {
        CSimpleString<TChar, TAllocator> output;
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
    CSimpleString<TChar, TAllocator>& operator+(const TChar* aTxt)
    {
        CSimpleString<TChar, TAllocator> output;
        const auto len = txtLength(aTxt);
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
    CSimpleString<TChar, TAllocator> operator+(const std::basic_string<TChar>& aTxt)
    {
        CSimpleString<TChar, TAllocator> output;
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
        return mDynamicArray[0];
    }

    const TChar& front() const noexcept
    {
        return mDynamicArray[0];
    }

    /**
     * @brief Return the last character in string.
     * 
     * @return The last character in string.
     */
    TChar& back() noexcept
    {
        return mDynamicArray[mLength - 1u];
    }

    /**
     * @brief Return the last character in string.
     * 
     * @return The last character in string.
     */
    const TChar& back() const noexcept
    {
        return mDynamicArray[mLength - 1u];
    }
    
    /**
     * @brief Return an iterator to the beginning of the string.
     * 
     * @return Iterator to the beginning of the string.
     */
    const_iterator begin() const noexcept
    {
        return const_iterator{mDynamicArray};
    }

    /**
     * @brief Return an iterator to the beginning of the string.
     * 
     * @return Iterator to the beginning of the string.
     */
    iterator begin() noexcept
    {
        return iterator{mDynamicArray};
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
        return const_iterator{&mDynamicArray[mLength]};
    }

    /**
     * @brief Return an iterator to the character after the last one.
     * 
     * @return Iterator to the character after the last one.
     */
    iterator end() noexcept
    {
        return iterator{&mDynamicArray[mLength]};
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
       return std::make_reverse_iterator(&mDynamicArray[mLength]);
    }

    /**
     * @brief Return a reverse iterator to iterate from the last character to the first one.
     * 
     * @return Teverse iterator to iterate from the last character to the first one.
     */
    reverse_iterator rbegin() noexcept
    {
       return std::make_reverse_iterator(&mDynamicArray[mLength]);
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
       return std::make_reverse_iterator(&mDynamicArray[0]);
    }

    /**
     * @brief Ruturn a reverse iterator that points before the first character.
     * 
     * @return Iterator that points before the first character. 
     */
    reverse_iterator rend() noexcept
    {
       return std::make_reverse_iterator(&mDynamicArray[0]);
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
   
};


/**
 * @brief Compare strings.
 * 
 * @tparam TChar Character type.
 * @tparam TAllocator1 Type of allocator of the first string.
 * @tparam TAllocator2 Type of allocator of the second string.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the given strings contain the same characters.
 */
template<
    typename TChar,
    typename TAllocator1,
    typename TAllocator2>
bool operator==(
    const CSimpleString<TChar, TAllocator1>& aObj1,
    const CSimpleString<TChar, TAllocator2>& aObj2) noexcept
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
 * @tparam TChar Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if there are equal.
 */
template<
    typename TChar,
    typename TAllocator>
bool operator==(
    const CSimpleString<TChar, TAllocator>& aObj1,
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
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aArray String array to compare.
 * @return true if there are equal.
 */
template<
    typename TChar,
    typename TAllocator>
bool operator==(
    const CSimpleString<TChar, TAllocator>& aObj1,
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
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if there are equal.
 */
template<
    typename TChar,
    typename TAllocator>
bool operator==(
    const std::basic_string<TChar>& aObj1,
    const CSimpleString<TChar, TAllocator>& aObj2) noexcept
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
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aArray First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if there are equal.
 */
template<
    typename TChar,
    typename TAllocator>
bool operator==(
    const TChar* aArray,
    const CSimpleString<TChar, TAllocator>& aObj) noexcept
{
    return aObj == aArray;
}


//////////////////////////////////// !=


/**
 * @brief Compare strings.
 * 
 * @tparam TChar Character type.
 * @tparam TAllocator1 Type of allocator of the first string.
 * @tparam TAllocator2 Type of allocator of the second string.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the given strings don't contain the same characters.
 */
template<
    typename TChar,
    typename TAllocator1,
    typename TAllocator2>
bool operator!=(
    const CSimpleString<TChar, TAllocator1>& aObj1,
    const CSimpleString<TChar, TAllocator2>& aObj2) noexcept
{
    return !(aObj1 == aObj2);
}

/**
 * @brief Compares strings.
 * 
 * @tparam TChar Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if there are equal.
 */
template<
    typename TChar,
    typename TAllocator>
bool operator!=(
    const CSimpleString<TChar, TAllocator>& aObj1,
    const std::basic_string<TChar>& aObj2) noexcept
{
    return !(aObj1 == aObj2);
}

/**
 * @brief Compares string.
 * 
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aArray String array to compare.
 * @return true if there are equal.
 */
template<
    typename TChar,
    typename TAllocator>
bool operator!=(
    const CSimpleString<TChar, TAllocator>& aObj1,
    const TChar* aArray) noexcept
{
    return !(aObj1 == aArray);
}


//////


/**
 * @brief Compares string.
 * 
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if there are not equal.
 */
template<
    typename TChar,
    typename TAllocator>
bool operator!=(
    const std::basic_string<TChar>& aObj1,
    const CSimpleString<TChar, TAllocator>& aObj2) noexcept
{
    return !(aObj1 == aObj2);
}

/**
 * @brief Compares string.
 * 
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aArray First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if there are not equal.
 */
template<
    typename TChar,
    typename TAllocator>
bool operator!=(
    const TChar* aArray,
    const CSimpleString<TChar, TAllocator>& aObj) noexcept
{
    return !(aArray == aObj);
}




//////////////////////////////////// <

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam TChar Character type.
 * @tparam TAllocator1 Type of allocator of the first string.
 * @tparam TAllocator2 Type of allocator of the second string.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly before the second string.
 */
template<
    typename TChar,
    typename TAllocator1,
    typename TAllocator2>
bool operator<(
    const CSimpleString<TChar, TAllocator1>& aObj1,
    const CSimpleString<TChar, TAllocator2>& aObj2) noexcept
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
 * @tparam TChar Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly before the second string.
 */
template<
    typename TChar,
    typename TAllocator>
bool operator<(
    const CSimpleString<TChar, TAllocator>& aObj1,
    const std::basic_string<TChar>& aObj2) noexcept
{
    return std::lexicographical_compare(aObj1.begin(), aObj1.end(),
        aObj2.begin(), aObj2.end());
}

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aArray String array to compare.
 * @return true if the first string is a lexicographicaly before the second string.
 */
template<
    typename TChar,
    typename TAllocator>
    bool operator<(
        const CSimpleString<TChar, TAllocator>& aObj1,
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
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly before the second string.
 */
template<
    typename TChar,
    typename TAllocator>
bool operator<(
    const std::basic_string<TChar>& aObj1,
    const CSimpleString<TChar, TAllocator>& aObj2) noexcept
{
    return std::lexicographical_compare(aObj1.begin(), aObj1.end(),
        aObj2.begin(), aObj2.end());
}

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aArray First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly before the second string.
 */
template<
    typename TChar,
    typename TAllocator>
bool operator<(
    const TChar* aArray,
    const CSimpleString<TChar, TAllocator>& aObj) noexcept
{
    const auto len = txtLength(aArray);

    return std::lexicographical_compare(aArray, aArray + len,
        aObj.begin(), aObj.end());
}


//////////////////////////////////// >



/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam TChar Character type.
 * @tparam TAllocator1 Type of allocator of the first string.
 * @tparam TAllocator2 Type of allocator of the second string.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly after the second string.
 */
template<
    typename TChar,
    typename TAllocator1,
    typename TAllocator2>
bool operator>(
    const CSimpleString<TChar, TAllocator1>& aObj1,
    const CSimpleString<TChar, TAllocator2>& aObj2) noexcept
{
    return aObj2 < aObj1;
}

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam TChar Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly after the second string.
 */
template<
    typename TChar,
    typename TAllocator>
bool operator>(
    const CSimpleString<TChar, TAllocator>& aObj1,
    const std::basic_string<TChar>& aObj2) noexcept
{
    return aObj2 < aObj1;
}

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aArray String array to compare.
 * @return true if the first string is a lexicographicaly after the second string.
 */
template<
    typename TChar,
    typename TAllocator>
bool operator>(
    const CSimpleString<TChar, TAllocator>& aObj1,
    const TChar* aArray) noexcept
{
    return aArray < aObj1;
}



/////

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly after the second string.
 */
template<
    typename TChar,
    typename TAllocator>
bool operator>(
    const std::basic_string<TChar>& aObj1,
    const CSimpleString<TChar, TAllocator>& aObj2) noexcept
{
    return aObj2 < aObj1;
}

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aArray First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly after the second string.
 */
template<
    typename TChar,
    typename TAllocator>
bool operator>(
    const TChar* aArray,
    const CSimpleString<TChar, TAllocator>& aObj2) noexcept
{
    return aObj2 < aArray;
}



//////////////////////////////////// >=

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam TChar Character type.
 * @tparam TAllocator1 Type of allocator of the first string.
 * @tparam TAllocator2 Type of allocator of the second string.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly after or equal the second string.
 */
template<
    typename TChar,
    typename TAllocator1,
    typename TAllocator2>
bool operator>=(
    const CSimpleString<TChar, TAllocator1>& aObj1,
    const CSimpleString<TChar, TAllocator2>& aObj2) noexcept
{
    return !(aObj1 < aObj2);
}

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam TChar Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly after or equal the second string.
 */
template<
    typename TChar,
    typename TAllocator>
bool operator>=(
    const CSimpleString<TChar, TAllocator>& aObj1,
    const std::basic_string<TChar>& aObj2) noexcept
{
    return !(aObj1 < aObj2);
}

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aArray String array to compare.
 * @return true if the first string is a lexicographicaly after or equal the second string.
 */
template<
    typename TChar,
    typename TAllocator>
    bool operator>=(
        const CSimpleString<TChar, TAllocator>& aObj1,
        const TChar* aArray) noexcept
{
    return !(aObj1 < aArray);
}



///////////

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly after or equal the second string.
 */
template<
    typename TChar,
    typename TAllocator>
bool operator>=(
    const std::basic_string<TChar>& aObj1,
    const CSimpleString<TChar, TAllocator>& aObj2) noexcept
{
    return !(aObj1 < aObj2);
}

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aArray First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly after or equal the second string.
 */
template<
    typename TChar,
    typename TAllocator>
bool operator>=(
    const TChar* aArray,
    const CSimpleString<TChar, TAllocator>& aObj) noexcept
{
    return !(aArray < aObj);
}


//////////////////////////////////// <=

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam TChar Character type.
 * @tparam TAllocator1 Type of allocator of the first string.
 * @tparam TAllocator2 Type of allocator of the second string.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly before or equal the second string.
 */
template<
    typename TChar,
    typename TAllocator1,
    typename TAllocator2>
bool operator<=(
    const CSimpleString<TChar, TAllocator1>& aObj1,
    const CSimpleString<TChar, TAllocator2>& aObj2) noexcept
{
    return !(aObj1 > aObj2);
}

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam TChar Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly before or equal the second string.
 */
template<
    typename TChar,
    typename TAllocator>
bool operator<=(
    const CSimpleString<TChar, TAllocator>& aObj1,
    const std::basic_string<TChar>& aObj2) noexcept
{
    return !(aObj1 > aObj2);
}

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aArray String array to compare.
 * @return true if the first string is a lexicographicaly before or equal the second string.
 */
template<
    typename TChar,
    typename TAllocator>
bool operator<=(
    const CSimpleString<TChar, TAllocator>& aObj1,
    const TChar* aArray) noexcept
{
    return !(aObj1 > aArray);
}


/////
/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aObj1 First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly before or equal the second string.
 */
template<
    typename TChar,
    typename TAllocator>
bool operator<=(
    const std::basic_string<TChar>& aObj1,
    const CSimpleString<TChar, TAllocator>& aObj2) noexcept
{
    return !(aObj1 > aObj2);
}

/**
 * @brief Compare strings using std::lexicographical_compare.
 * 
 * @tparam TChar  Type of characters
 * @tparam TAllocator Type of allocator.
 * @param aArray First string to compare.
 * @param aObj2 Second string to compare.
 * @return true if the first string is a lexicographicaly before or equal the second string.
 */
template<
    typename TChar,
    typename TAllocator>
bool operator<=(
    const TChar* aArray,
    const CSimpleString<TChar, TAllocator>& aObj) noexcept
{
    return !(aArray > aObj);
}

/**
 * @brief Push characters to the stream.
 * 
 * @tparam TCharStream Type of characters in the stream.
 * @tparam TChar type of characters in the string.
 * @tparam TAllocator Allocator type.
 * @param aStream Stream
 * @param aString String
 * 
 * @return Stream.
 */
template<typename TCharStream, typename TChar>
std::basic_ostream<TCharStream, std::char_traits<TCharStream>>&
    operator<<(std::basic_ostream<TCharStream, std::char_traits<TCharStream>>& aStream, const CSimpleString<TChar>& aString)
{
    aStream << aString.data();
    return aStream;
}

} // namespace NSSO

#endif // SIMPLE_STRING_HPP_
