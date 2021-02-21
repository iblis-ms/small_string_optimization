#include <gtest/gtest.h>
#include <SmallStringOptimization/SimpleString.hpp>

using namespace NSSO;
namespace std
{
using namespace NSSO;
}

TEST(SimpleStringTests, defaultValues)
{
    CSimpleString<> s1;
    ASSERT_EQ(s1.size(), 0u);
    ASSERT_TRUE(s1.empty());
    ASSERT_STREQ(s1.data(), "");

    ASSERT_EQ(s1.begin(), s1.end());
    ASSERT_EQ(s1.rbegin(), s1.rend());
    ASSERT_EQ(s1.cbegin(), s1.cend());
    ASSERT_EQ(s1.crbegin(), s1.crend());
}


TEST(SimpleStringTests, defaultValuesConst)
{
    const CSimpleString<> s1;
    ASSERT_EQ(s1.size(), 0u);
    ASSERT_TRUE(s1.empty());
    ASSERT_STREQ(s1.data(), "");

    ASSERT_EQ(s1.begin(), s1.end());
    ASSERT_EQ(s1.rbegin(), s1.rend());
    ASSERT_EQ(s1.cbegin(), s1.cend());
    ASSERT_EQ(s1.crbegin(), s1.crend());
}



TEST(SimpleStringTests, copyDefaultValues)
{
    const CSimpleString<> s1;
    CSimpleString<> s2(s1);
    ASSERT_EQ(s2.size(), 0u);
    ASSERT_TRUE(s2.empty());
    ASSERT_STREQ(s1.data(), "");

    ASSERT_EQ(s2.begin(), s2.end());
    ASSERT_EQ(s2.rbegin(), s2.rend());
    ASSERT_EQ(s2.cbegin(), s2.cend());
    ASSERT_EQ(s2.crbegin(), s2.crend());
}

TEST(SimpleStringTests, copyDefaultValuesConst)
{
    const CSimpleString<> s1;
    const CSimpleString<> s2{s1};
    ASSERT_EQ(s2.size(), 0u);
    ASSERT_TRUE(s2.empty());
    ASSERT_STREQ(s1.data(), "");

    ASSERT_EQ(s2.begin(), s2.end());
    ASSERT_EQ(s2.rbegin(), s2.rend());
    ASSERT_EQ(s2.cbegin(), s2.cend());
    ASSERT_EQ(s2.crbegin(), s2.crend());
}

TEST(SimpleStringTests, copyDefaultValuesSmaller)
{
    CSimpleString<> s1;
    CSimpleString<> s2{s1};
    ASSERT_EQ(s2.size(), 0u);
    ASSERT_TRUE(s2.empty());
    ASSERT_STREQ(s1.data(), "");

    ASSERT_EQ(s2.begin(), s2.end());
    ASSERT_EQ(s2.rbegin(), s2.rend());
    ASSERT_EQ(s2.cbegin(), s2.cend());
    ASSERT_EQ(s2.crbegin(), s2.crend());
}

TEST(SimpleStringTests, copyDefaultValuesConstSmaller)
{
    const CSimpleString<> s1;
    const CSimpleString<> s2{s1};
    ASSERT_EQ(s2.size(), 0u);
    ASSERT_TRUE(s2.empty());
    ASSERT_STREQ(s1.data(), "");

    ASSERT_EQ(s2.begin(), s2.end());
    ASSERT_EQ(s2.rbegin(), s2.rend());
    ASSERT_EQ(s2.cbegin(), s2.cend());
    ASSERT_EQ(s2.crbegin(), s2.crend());
}


TEST(SimpleStringTests, copyDefaultValuesBigger)
{
    CSimpleString<> s1;
    CSimpleString<> s2{s1};
    ASSERT_EQ(s2.size(), 0u);
    ASSERT_TRUE(s2.empty());
    ASSERT_STREQ(s1.data(), "");

    ASSERT_EQ(s2.begin(), s2.end());
    ASSERT_EQ(s2.rbegin(), s2.rend());
    ASSERT_EQ(s2.cbegin(), s2.cend());
    ASSERT_EQ(s2.crbegin(), s2.crend());
}

TEST(SimpleStringTests, copyDefaultValuesConstBigger)
{
    const CSimpleString<> s1;
    const CSimpleString<> s2{s1};
    ASSERT_EQ(s2.size(), 0u);
    ASSERT_TRUE(s2.empty());
    ASSERT_STREQ(s1.data(), "");

    ASSERT_EQ(s2.begin(), s2.end());
    ASSERT_EQ(s2.rbegin(), s2.rend());
    ASSERT_EQ(s2.cbegin(), s2.cend());
    ASSERT_EQ(s2.crbegin(), s2.crend());
}









TEST(SimpleStringTests, copyDefaultValuesSmallString)
{
    
    const std::string initTxt{"abc"};
    const unsigned int lenInitTxt = initTxt.size();

    CSimpleString<> s1{initTxt};
    CSimpleString<> s2{s1};
    ASSERT_EQ(s2.size(), lenInitTxt);
    ASSERT_FALSE(s2.empty());
    ASSERT_STREQ(s1.data(), initTxt.data());

    const auto equal1 = std::equal(s2.begin(), s2.end(), initTxt.begin());
    ASSERT_TRUE(equal1);
    const auto equal2 = std::equal(s2.rbegin(), s2.rend(), initTxt.rbegin());
    ASSERT_TRUE(equal2);
    const auto equal3 = std::equal(s2.cbegin(), s2.cend(), initTxt.cbegin());
    ASSERT_TRUE(equal3);
    const auto equal4 = std::equal(s2.crbegin(), s2.crend(), initTxt.crbegin());
    ASSERT_TRUE(equal4);
}

TEST(SimpleStringTests, copyDefaultValuesBigString)
{
    
    const std::string initTxt{"abcdefghijklmn"};
    const unsigned int lenInitTxt = initTxt.size();

    CSimpleString<> s1{initTxt};
    CSimpleString<> s2{s1};
    ASSERT_EQ(s2.size(), lenInitTxt);
    ASSERT_FALSE(s2.empty());
    ASSERT_STREQ(s1.data(), initTxt.data());

    const auto equal1 = std::equal(s2.begin(), s2.end(), initTxt.begin());
    ASSERT_TRUE(equal1);
    const auto equal2 = std::equal(s2.rbegin(), s2.rend(), initTxt.rbegin());
    ASSERT_TRUE(equal2);
    const auto equal3 = std::equal(s2.cbegin(), s2.cend(), initTxt.cbegin());
    ASSERT_TRUE(equal3);
    const auto equal4 = std::equal(s2.crbegin(), s2.crend(), initTxt.crbegin());
    ASSERT_TRUE(equal4);
}



TEST(SimpleStringTests, copyDefaultValuesSmallStringAssign)
{
    const std::string initTxt{"abc"};
    const unsigned int lenInitTxt = initTxt.size();

    CSimpleString<> s1{initTxt};
    CSimpleString<> s2;
    s2 = s1;
    ASSERT_EQ(s2.size(), lenInitTxt);
    ASSERT_FALSE(s2.empty());
    ASSERT_STREQ(s1.data(), initTxt.data());

    const auto equal1 = std::equal(s2.begin(), s2.end(), initTxt.begin());
    ASSERT_TRUE(equal1);
    const auto equal2 = std::equal(s2.rbegin(), s2.rend(), initTxt.rbegin());
    ASSERT_TRUE(equal2);
    const auto equal3 = std::equal(s2.cbegin(), s2.cend(), initTxt.cbegin());
    ASSERT_TRUE(equal3);
    const auto equal4 = std::equal(s2.crbegin(), s2.crend(), initTxt.crbegin());
    ASSERT_TRUE(equal4);
}

TEST(SimpleStringTests, copyDefaultValuesBigStringAssign)
{
    const std::string initTxt{"abcdefghijklmn"};
    const unsigned int lenInitTxt = initTxt.size();

    CSimpleString<> s1{initTxt};
    CSimpleString<> s2;
    s2 = s1;
    ASSERT_EQ(s2.size(), lenInitTxt);
    ASSERT_FALSE(s2.empty());
    ASSERT_STREQ(s1.data(), initTxt.data());

    const auto equal1 = std::equal(s2.begin(), s2.end(), initTxt.begin());
    ASSERT_TRUE(equal1);
    const auto equal2 = std::equal(s2.rbegin(), s2.rend(), initTxt.rbegin());
    ASSERT_TRUE(equal2);
    const auto equal3 = std::equal(s2.cbegin(), s2.cend(), initTxt.cbegin());
    ASSERT_TRUE(equal3);
    const auto equal4 = std::equal(s2.crbegin(), s2.crend(), initTxt.crbegin());
    ASSERT_TRUE(equal4);
}






TEST(SimpleStringTests, initCharSmall)
{
    const char* initTxt = "abc";
    const std::string str1{ initTxt };
    const unsigned int lenInitTxt = str1.size();

    CSimpleString<> s1{initTxt};
    ASSERT_EQ(s1.size(), lenInitTxt);
    ASSERT_STREQ(s1.data(), initTxt);
    const auto equal1 = std::equal(s1.begin(), s1.end(), str1.begin());
    ASSERT_TRUE(equal1);
    const auto equal2 = std::equal(s1.rbegin(), s1.rend(), str1.rbegin());
    ASSERT_TRUE(equal2);
    const auto equal3 = std::equal(s1.cbegin(), s1.cend(), str1.cbegin());
    ASSERT_TRUE(equal3);
    const auto equal4 = std::equal(s1.crbegin(), s1.crend(), str1.crbegin());
    ASSERT_TRUE(equal4);
}

TEST(SimpleStringTests, initCharSmallConst)
{
    const char* initTxt = "abc";
    const std::string str1{ initTxt };
    const unsigned int lenInitTxt = str1.size();

    const CSimpleString<> s1{initTxt};
    ASSERT_EQ(s1.size(), lenInitTxt);
    ASSERT_STREQ(s1.data(), initTxt);
    const auto equal1 = std::equal(s1.begin(), s1.end(), str1.begin());
    ASSERT_TRUE(equal1);
    const auto equal2 = std::equal(s1.rbegin(), s1.rend(), str1.rbegin());
    ASSERT_TRUE(equal2);
    const auto equal3 = std::equal(s1.cbegin(), s1.cend(), str1.cbegin());
    ASSERT_TRUE(equal3);
    const auto equal4 = std::equal(s1.crbegin(), s1.crend(), str1.crbegin());
    ASSERT_TRUE(equal4);
}

TEST(SimpleStringTests, initCharNotSmall)
{
    const char* initTxt = "abcdefghijklmnopr";
    const std::string str1{ initTxt };
    const unsigned int lenInitTxt = str1.size();

    CSimpleString<> s1{initTxt};
    ASSERT_EQ(s1.size(), lenInitTxt);
    ASSERT_STREQ(s1.data(), initTxt);
    const auto equal1 = std::equal(s1.begin(), s1.end(), str1.begin());
    ASSERT_TRUE(equal1);
    const auto equal2 = std::equal(s1.rbegin(), s1.rend(), str1.rbegin());
    ASSERT_TRUE(equal2);
    const auto equal3 = std::equal(s1.cbegin(), s1.cend(), str1.cbegin());
    ASSERT_TRUE(equal3);
    const auto equal4 = std::equal(s1.crbegin(), s1.crend(), str1.crbegin());
    ASSERT_TRUE(equal4);
}

TEST(SimpleStringTests, initCharNotSmallConst)
{
    const char* initTxt = "abcdefghijklmnopr";
    const std::string str1{ initTxt };
    const unsigned int lenInitTxt = str1.size();

    const CSimpleString<> s1{initTxt};
    ASSERT_EQ(s1.size(), lenInitTxt);
    ASSERT_STREQ(s1.data(), initTxt);
    const auto equal1 = std::equal(s1.begin(), s1.end(), str1.begin());
    ASSERT_TRUE(equal1);
    const auto equal2 = std::equal(s1.rbegin(), s1.rend(), str1.rbegin());
    ASSERT_TRUE(equal2);
    const auto equal3 = std::equal(s1.cbegin(), s1.cend(), str1.cbegin());
    ASSERT_TRUE(equal3);
    const auto equal4 = std::equal(s1.crbegin(), s1.crend(), str1.crbegin());
    ASSERT_TRUE(equal4);
}














TEST(SimpleStringTests, initStringSmall)
{
    const std::string initTxt{"abc"};
    const unsigned int lenInitTxt = initTxt.size();

    CSimpleString<> s1{initTxt};
    ASSERT_EQ(s1.size(), lenInitTxt);
    ASSERT_STREQ(s1.data(), initTxt.data());
    const auto equal1 = std::equal(s1.begin(), s1.end(), initTxt.begin());
    ASSERT_TRUE(equal1);
    const auto equal2 = std::equal(s1.rbegin(), s1.rend(), initTxt.rbegin());
    ASSERT_TRUE(equal2);
    const auto equal3 = std::equal(s1.cbegin(), s1.cend(), initTxt.cbegin());
    ASSERT_TRUE(equal3);
    const auto equal4 = std::equal(s1.crbegin(), s1.crend(), initTxt.crbegin());
    ASSERT_TRUE(equal4);
}

TEST(SimpleStringTests, initStringSmallConst)
{
    const std::string initTxt{"abc"};
    const unsigned int lenInitTxt = initTxt.size();

    const CSimpleString<> s1{initTxt};
    ASSERT_EQ(s1.size(), lenInitTxt);
    ASSERT_STREQ(s1.data(), initTxt.c_str());
    const auto equal1 = std::equal(s1.begin(), s1.end(), initTxt.begin());
    ASSERT_TRUE(equal1);
    const auto equal2 = std::equal(s1.rbegin(), s1.rend(), initTxt.rbegin());
    ASSERT_TRUE(equal2);
    const auto equal3 = std::equal(s1.cbegin(), s1.cend(), initTxt.cbegin());
    ASSERT_TRUE(equal3);
    const auto equal4 = std::equal(s1.crbegin(), s1.crend(), initTxt.crbegin());
    ASSERT_TRUE(equal4);
}

TEST(SimpleStringTests, initStringNotSmall)
{
    const std::string initTxt{"abcdefghijklmnopr"};
    const unsigned int lenInitTxt = initTxt.size();

    CSimpleString<> s1{initTxt};
    ASSERT_EQ(s1.size(), lenInitTxt);
    ASSERT_STREQ(s1.data(), initTxt.data());
    const auto equal1 = std::equal(s1.begin(), s1.end(), initTxt.begin());
    ASSERT_TRUE(equal1);
    const auto equal2 = std::equal(s1.rbegin(), s1.rend(), initTxt.rbegin());
    ASSERT_TRUE(equal2);
    const auto equal3 = std::equal(s1.cbegin(), s1.cend(), initTxt.cbegin());
    ASSERT_TRUE(equal3);
    const auto equal4 = std::equal(s1.crbegin(), s1.crend(), initTxt.crbegin());
    ASSERT_TRUE(equal4);
}

TEST(SimpleStringTests, initStringNotSmallConst)
{
    const std::string initTxt{"abcdefghijklmnopr"};
    const unsigned int lenInitTxt = initTxt.size();

    const CSimpleString<> s1{initTxt};
    ASSERT_EQ(s1.size(), lenInitTxt);
    ASSERT_STREQ(s1.data(), initTxt.data());
    const auto equal1 = std::equal(s1.begin(), s1.end(), initTxt.begin());
    ASSERT_TRUE(equal1);
    const auto equal2 = std::equal(s1.rbegin(), s1.rend(), initTxt.rbegin());
    ASSERT_TRUE(equal2);
    const auto equal3 = std::equal(s1.cbegin(), s1.cend(), initTxt.cbegin());
    ASSERT_TRUE(equal3);
    const auto equal4 = std::equal(s1.crbegin(), s1.crend(), initTxt.crbegin());
    ASSERT_TRUE(equal4);
}














TEST(SimpleStringTests, equal)
{
    const char* initTxt = "abc";
    const std::string str{initTxt};

    const CSimpleString<> s1{initTxt};
    const CSimpleString<> s2{initTxt};
    const CSimpleString<> s3{initTxt};
    const CSimpleString<> s4{initTxt};

    ASSERT_TRUE(s1 == s2);
    ASSERT_TRUE(s1 == s3);
    ASSERT_TRUE(s1 == s4);
    ASSERT_TRUE(s1 == initTxt);
    ASSERT_TRUE(s1 == str);
    ASSERT_TRUE(initTxt == s1);
    ASSERT_TRUE(str == s1);
    
    ASSERT_FALSE(s1 != s2);
    ASSERT_FALSE(s1 != s3);
    ASSERT_FALSE(s1 != s4);
    ASSERT_FALSE(s1 != initTxt);
    ASSERT_FALSE(s1 != str);
    ASSERT_FALSE(initTxt != s1);
    ASSERT_FALSE(str != s1);
}



TEST(SimpleStringTests, notEqual)
{
    const char* initTxt1 = "aaa";
    const char* initTxt2 = "bbb";
    const char* initTxt3 = "ccccc";
    const std::string str1{initTxt1};

    const CSimpleString<> s1{initTxt1};
    const CSimpleString<> s2{initTxt2};
    const CSimpleString<> s3{initTxt3};

    ASSERT_TRUE(s1 != s2);
    ASSERT_TRUE(s1 != s3);
    
    ASSERT_TRUE(s2 != str1);
    ASSERT_TRUE(str1 != s2);
    ASSERT_TRUE(s2 != initTxt1);
    ASSERT_TRUE(initTxt1 != s2);
    
    ASSERT_FALSE(s1 == s2);
    ASSERT_FALSE(s1 == s3);
    
    ASSERT_FALSE(s2 == str1);
    ASSERT_FALSE(str1 == s2);
    ASSERT_FALSE(s2 == initTxt1);
    ASSERT_FALSE(initTxt1 == s2);
}

TEST(SimpleStringTests, lessGreater)
{
    const char* initTxt1 = "aaa";
    const char* initTxt2 = "bbbb";
    const char* initTxt3 = "ccccc";
    const std::string str2{initTxt2};

    const CSimpleString<> s1{initTxt1};
    const CSimpleString<> s2{initTxt2};
    const CSimpleString<> s3{initTxt3};

    const CSimpleString<> s4{initTxt2};

    ASSERT_TRUE(s4 <= s2);
    ASSERT_TRUE(s2 <= s4);
    ASSERT_TRUE(initTxt2 <= s4);
    ASSERT_TRUE(s4 <= initTxt2);
    ASSERT_TRUE(str2 <= s4);
    ASSERT_TRUE(s4 <= str2);


    ASSERT_TRUE(s1 < s2);
    ASSERT_TRUE(s2 < s3);
    
    ASSERT_TRUE(s1 < str2);
    ASSERT_TRUE(str2 < s3);

    ASSERT_TRUE(s1 < initTxt2);
    ASSERT_TRUE(initTxt2 < s3);

    ASSERT_TRUE(s2 > s1);
    ASSERT_TRUE(s3 > s2);



    ASSERT_FALSE(s1 > s2);
    ASSERT_FALSE(s2 > s3);
    
    ASSERT_FALSE(s1 > str2);
    ASSERT_FALSE(str2 > s3);

    ASSERT_FALSE(s1 > initTxt2);
    ASSERT_FALSE(initTxt2 > s3);

    ASSERT_FALSE(s2 < s1);
    ASSERT_FALSE(s3 < s2);

    
    ASSERT_TRUE(s1 <= s2);
    ASSERT_TRUE(s2 <= s3);
    
    ASSERT_TRUE(s1 <= str2);
    ASSERT_TRUE(str2 <= s3);

    ASSERT_TRUE(s1 <= initTxt2);
    ASSERT_TRUE(initTxt2 <= s3);

    ASSERT_TRUE(s2 >= s1);
    ASSERT_TRUE(s3 >= s2);



    ASSERT_FALSE(s1 >= s2);
    ASSERT_FALSE(s2 >= s3);
    
    ASSERT_FALSE(s1 >= str2);
    ASSERT_FALSE(str2 >= s3);

    ASSERT_FALSE(s1 >= initTxt2);
    ASSERT_FALSE(initTxt2 >= s3);

    ASSERT_FALSE(s2 <= s1);
    ASSERT_FALSE(s3 <= s2);
}




TEST(SimpleStringTests, assign)
{
    const char* txt1 = "abcd";
    const std::string str1{txt1};

    CSimpleString<> s1;
    s1.assign(txt1);
    ASSERT_STREQ(s1.data(), txt1);

    s1.assign(txt1, 2);
    ASSERT_STREQ(s1.data(), "ab");

    s1.assign(str1);
    ASSERT_STREQ(s1.data(), txt1);
}

TEST(SimpleStringTests, backFront)
{
    const char* txt1 = "abcd";
    const std::string str1{txt1};

    CSimpleString<> s1{str1};
    ASSERT_EQ(s1.front(), 'a');
    ASSERT_EQ(s1.back(), 'd');
}

TEST(SimpleStringTests, backFrontConst)
{
    const char* txt1 = "def";
    const std::string str1{txt1};

    const CSimpleString<> s1{str1};
    ASSERT_EQ(s1.front(), 'd');
    ASSERT_EQ(s1.back(), 'f');
}


TEST(SimpleStringTests, arrayOperator)
{
    const char* txt1 = "abcd";
    const std::string str1{txt1};

    CSimpleString<> s1{str1};
    ASSERT_EQ(s1[0], 'a');
    ASSERT_EQ(s1[1], 'b');
    ASSERT_EQ(s1[2], 'c');
    ASSERT_EQ(s1[3], 'd');
    ASSERT_EQ(s1[4], '\0');
}


TEST(SimpleStringTests, arrayOperatorConst)
{
    const char* txt1 = "abc";
    const std::string str1{txt1};

    const CSimpleString<> s1{str1};
    ASSERT_EQ(s1[0], 'a');
    ASSERT_EQ(s1[1], 'b');
    ASSERT_EQ(s1[2], 'c');
    ASSERT_EQ(s1[3], '\0');
}


TEST(SimpleStringTests, appendSmallStringOpt)
{
    
    CSimpleString<> str1{"abc"};
    CSimpleString<> str2{"defghij"};
    CSimpleString<> str3{"123"};
    CSimpleString<> str4{"123456789012345"};
    

    CSimpleString<> s1;
    s1.append(str1);
    ASSERT_STREQ(s1.data(), "abc");

    s1.append(str2);
    ASSERT_STREQ(s1.data(), "abcdefghij");
    
    s1.append(str3);
    ASSERT_STREQ(s1.data(), "abcdefghij123");
    
    s1.append(str4);
    ASSERT_STREQ(s1.data(), "abcdefghij123123456789012345");
}

TEST(SimpleStringTests, appendChar)
{
    const char* txt1 = "abcd";

    CSimpleString<> s1;
    s1.append(txt1);
    ASSERT_STREQ(s1.data(), "abcd");

    s1.append(txt1, 2);
    ASSERT_STREQ(s1.data(), "abcdab");

    s1.append(txt1);
    ASSERT_STREQ(s1.data(), "abcdababcd");
}


TEST(SimpleStringTests, appendString)
{
    const std::string txt1{"abcd"};

    CSimpleString<> s1;
    s1.append(txt1);
    ASSERT_STREQ(s1.data(), "abcd");

    s1.append(txt1);
    ASSERT_STREQ(s1.data(), "abcdabcd");

    s1.append(txt1);
    ASSERT_STREQ(s1.data(), "abcdabcdabcd");
}


TEST(SimpleStringTests, plusEqualSmallStringOpt)
{
    
    CSimpleString<> str1{"abc"};
    CSimpleString<> str2{"defghij"};
    CSimpleString<> str3{"123"};
    CSimpleString<> str4{"123456789012345"};
    

    CSimpleString<> s1;
    s1 += str1;
    ASSERT_STREQ(s1.data(), "abc");
    
    s1 += str2;
    ASSERT_STREQ(s1.data(), "abcdefghij");
    
    s1 += str3;
    ASSERT_STREQ(s1.data(), "abcdefghij123");
    
    s1 += str4;
    ASSERT_STREQ(s1.data(), "abcdefghij123123456789012345");
}

TEST(SimpleStringTests, plusEqualChar)
{
    const char* txt1 = "abcd";
    const char* txt2 = "ab";

    CSimpleString<> s1;
    s1 += txt1;
    ASSERT_STREQ(s1.data(), "abcd");

    s1 += txt2;
    ASSERT_STREQ(s1.data(), "abcdab");
    
    s1 += txt1;
    ASSERT_STREQ(s1.data(), "abcdababcd");
}


TEST(SimpleStringTests, plusEqualString)
{
    const std::string txt1{"abcd"};

    CSimpleString<> s1;
    s1 += txt1;
    ASSERT_STREQ(s1.data(), "abcd");
    
    s1 += txt1;
    ASSERT_STREQ(s1.data(), "abcdabcd");
    
    s1 += txt1;
    ASSERT_STREQ(s1.data(), "abcdabcdabcd");
}


TEST(SimpleStringTests, moveConstr)
{
    {
        CSimpleString<> s1{"abc"};
        CSimpleString<> s2{std::move(s1)};
        ASSERT_STREQ(s2.data(), "abc");
    }
    {
        CSimpleString<> s1{"abcdefghijklmnop"};
        CSimpleString<> s2{std::move(s1)};
        ASSERT_STREQ(s2.data(), "abcdefghijklmnop");
    }
    {
        CSimpleString<> s1{"abc"};
        CSimpleString<> s2{std::move(s1)};
        ASSERT_STREQ(s2.data(), "abc");
    }
    {
        CSimpleString<> s1{"abcdefghijklmnop"};
        CSimpleString<> s2{std::move(s1)};
        ASSERT_STREQ(s2.data(), "abcdefghijklmnop");
    }
    {
        CSimpleString<> s1{"abc"};
        CSimpleString<> s2{std::move(s1)};
        ASSERT_STREQ(s2.data(), "abc");
    }
    {
        CSimpleString<> s1{"abcdefghijklmnop"};
        CSimpleString<> s2{std::move(s1)};
        ASSERT_STREQ(s2.data(), "abcdefghijklmnop");
    }
}


TEST(SimpleStringTests, moveAssign)
{
    {
        CSimpleString<> s1{"abc"};
        CSimpleString<> s2{"123"};
        s2 = std::move(s1);
        ASSERT_STREQ(s2.data(), "abc");
    }
    {
        CSimpleString<> s1{"abcdefghijklmnop"};
        CSimpleString<> s2{"123"};
        s2 = std::move(s1);
        ASSERT_STREQ(s2.data(), "abcdefghijklmnop");
    }
    {
        CSimpleString<> s1{"abc"};
        CSimpleString<> s2{"123"};
        s2 = std::move(s1);
        ASSERT_STREQ(s2.data(), "abc");
    }
    {
        CSimpleString<> s1{"abcdefghijklmnop"};
        CSimpleString<> s2{"123"};
        s2 = std::move(s1);
        ASSERT_STREQ(s2.data(), "abcdefghijklmnop");
    }
    {
        CSimpleString<> s1{"abc"};
        CSimpleString<> s2{"123"};
        s2 = std::move(s1);
        ASSERT_STREQ(s2.data(), "abc");
    }
    {
        CSimpleString<> s1{"abcdefghijklmnop"};
        CSimpleString<> s2{"123"};
        s2 = std::move(s1);
        ASSERT_STREQ(s2.data(), "abcdefghijklmnop");
    }
}



TEST(SimpleStringTests, less)
{
    using DType = NSSO::CSimpleString<>;
    
    const DType a{"abcdefghijklmnop"};
    const DType b{"123"};
    const std::less<DType> c2;
    c2(a, b);
    
}

