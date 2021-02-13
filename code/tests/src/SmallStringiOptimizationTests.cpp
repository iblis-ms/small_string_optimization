#include <gtest/gtest.h>
#include <SmallStringOptimization/SmallStringOptimization.hpp>

using namespace NSSO;

TEST(SmallStringOptimizationTests, defaultValues)
{
    const unsigned int smallStringOptLength = 10u;
    CSmallStringOpt<smallStringOptLength> s1;
    ASSERT_EQ(s1.size(), 0u);
    ASSERT_TRUE(s1.empty());
    ASSERT_EQ(s1.data(), nullptr);
    ASSERT_EQ(s1.capacity(), smallStringOptLength);

    ASSERT_EQ(s1.begin(), s1.end());
    ASSERT_EQ(s1.rbegin(), s1.rend());
    ASSERT_EQ(s1.cbegin(), s1.cend());
    ASSERT_EQ(s1.crbegin(), s1.crend());
}

TEST(SmallStringOptimizationTests, defaultValuesConst)
{
    const CSmallStringOpt<10u> s1;
    ASSERT_EQ(s1.size(), 0u);
    ASSERT_TRUE(s1.empty());
    ASSERT_EQ(s1.data(), nullptr);

    ASSERT_EQ(s1.begin(), s1.end());
    ASSERT_EQ(s1.rbegin(), s1.rend());
    ASSERT_EQ(s1.cbegin(), s1.cend());
    ASSERT_EQ(s1.crbegin(), s1.crend());
}



TEST(SmallStringOptimizationTests, copyDefaultValues)
{
    const CSmallStringOpt<10u> s1;
    CSmallStringOpt<10u> s2(s1);
    ASSERT_EQ(s2.size(), 0u);
    ASSERT_TRUE(s2.empty());
    ASSERT_EQ(s1.data(), nullptr);

    ASSERT_EQ(s2.begin(), s2.end());
    ASSERT_EQ(s2.rbegin(), s2.rend());
    ASSERT_EQ(s2.cbegin(), s2.cend());
    ASSERT_EQ(s2.crbegin(), s2.crend());
}

TEST(SmallStringOptimizationTests, copyDefaultValuesConst)
{
    const CSmallStringOpt<10u> s1;
    const CSmallStringOpt<10u> s2{s1};
    ASSERT_EQ(s2.size(), 0u);
    ASSERT_TRUE(s2.empty());
    ASSERT_EQ(s1.data(), nullptr);

    ASSERT_EQ(s2.begin(), s2.end());
    ASSERT_EQ(s2.rbegin(), s2.rend());
    ASSERT_EQ(s2.cbegin(), s2.cend());
    ASSERT_EQ(s2.crbegin(), s2.crend());
}

TEST(SmallStringOptimizationTests, copyDefaultValuesSmaller)
{
    CSmallStringOpt<10u> s1;
    CSmallStringOpt<2u> s2{s1};
    ASSERT_EQ(s2.size(), 0u);
    ASSERT_TRUE(s2.empty());
    ASSERT_EQ(s1.data(), nullptr);

    ASSERT_EQ(s2.begin(), s2.end());
    ASSERT_EQ(s2.rbegin(), s2.rend());
    ASSERT_EQ(s2.cbegin(), s2.cend());
    ASSERT_EQ(s2.crbegin(), s2.crend());
}

TEST(SmallStringOptimizationTests, copyDefaultValuesConstSmaller)
{
    const CSmallStringOpt<10u> s1;
    const CSmallStringOpt<2u> s2{s1};
    ASSERT_EQ(s2.size(), 0u);
    ASSERT_TRUE(s2.empty());
    ASSERT_EQ(s1.data(), nullptr);

    ASSERT_EQ(s2.begin(), s2.end());
    ASSERT_EQ(s2.rbegin(), s2.rend());
    ASSERT_EQ(s2.cbegin(), s2.cend());
    ASSERT_EQ(s2.crbegin(), s2.crend());
}


TEST(SmallStringOptimizationTests, copyDefaultValuesBigger)
{
    CSmallStringOpt<10u> s1;
    CSmallStringOpt<22u> s2{s1};
    ASSERT_EQ(s2.size(), 0u);
    ASSERT_TRUE(s2.empty());
    ASSERT_EQ(s1.data(), nullptr);

    ASSERT_EQ(s2.begin(), s2.end());
    ASSERT_EQ(s2.rbegin(), s2.rend());
    ASSERT_EQ(s2.cbegin(), s2.cend());
    ASSERT_EQ(s2.crbegin(), s2.crend());
}

TEST(SmallStringOptimizationTests, copyDefaultValuesConstBigger)
{
    const CSmallStringOpt<10u> s1;
    const CSmallStringOpt<22u> s2{s1};
    ASSERT_EQ(s2.size(), 0u);
    ASSERT_TRUE(s2.empty());
    ASSERT_EQ(s1.data(), nullptr);

    ASSERT_EQ(s2.begin(), s2.end());
    ASSERT_EQ(s2.rbegin(), s2.rend());
    ASSERT_EQ(s2.cbegin(), s2.cend());
    ASSERT_EQ(s2.crbegin(), s2.crend());
}










TEST(SmallStringOptimizationTests, copyDefaultValuesSmallString)
{
    
    const std::string initTxt{"abc"};
    const unsigned int lenInitTxt = initTxt.size();

    CSmallStringOpt<10u> s1{initTxt};
    CSmallStringOpt<10u> s2{s1};
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

TEST(SmallStringOptimizationTests, copyDefaultValuesBigString)
{
    
    const std::string initTxt{"abcdefghijklmn"};
    const unsigned int lenInitTxt = initTxt.size();

    CSmallStringOpt<10u> s1{initTxt};
    CSmallStringOpt<10u> s2{s1};
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



TEST(SmallStringOptimizationTests, copyDefaultValuesSmallStringAssign)
{
    const std::string initTxt{"abc"};
    const unsigned int lenInitTxt = initTxt.size();

    CSmallStringOpt<10u> s1{initTxt};
    CSmallStringOpt<10u> s2;
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

TEST(SmallStringOptimizationTests, copyDefaultValuesBigStringAssign)
{
    const std::string initTxt{"abcdefghijklmn"};
    const unsigned int lenInitTxt = initTxt.size();

    CSmallStringOpt<10u> s1{initTxt};
    CSmallStringOpt<10u> s2;
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






TEST(SmallStringOptimizationTests, initCharSmall)
{
    const char* initTxt = "abc";
    const std::string str1{ initTxt };
    const unsigned int lenInitTxt = str1.size();

    CSmallStringOpt<10u> s1{initTxt};
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

TEST(SmallStringOptimizationTests, initCharSmallConst)
{
    const char* initTxt = "abc";
    const std::string str1{ initTxt };
    const unsigned int lenInitTxt = str1.size();

    const CSmallStringOpt<10u> s1{initTxt};
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

TEST(SmallStringOptimizationTests, initCharNotSmall)
{
    const char* initTxt = "abcdefghijklmnopr";
    const std::string str1{ initTxt };
    const unsigned int lenInitTxt = str1.size();

    CSmallStringOpt<10u> s1{initTxt};
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

TEST(SmallStringOptimizationTests, initCharNotSmallConst)
{
    const char* initTxt = "abcdefghijklmnopr";
    const std::string str1{ initTxt };
    const unsigned int lenInitTxt = str1.size();

    const CSmallStringOpt<10u> s1{initTxt};
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













TEST(SmallStringOptimizationTests, initStringSmall)
{
    const std::string initTxt{"abc"};
    const unsigned int lenInitTxt = initTxt.size();

    CSmallStringOpt<10u> s1{initTxt};
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

TEST(SmallStringOptimizationTests, initStringSmallConst)
{
    const std::string initTxt{"abc"};
    const unsigned int lenInitTxt = initTxt.size();

    const CSmallStringOpt<10u> s1{initTxt};
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

TEST(SmallStringOptimizationTests, initStringNotSmall)
{
    const std::string initTxt{"abcdefghijklmnopr"};
    const unsigned int lenInitTxt = initTxt.size();

    CSmallStringOpt<10u> s1{initTxt};
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

TEST(SmallStringOptimizationTests, initStringNotSmallConst)
{
    const std::string initTxt{"abcdefghijklmnopr"};
    const unsigned int lenInitTxt = initTxt.size();

    const CSmallStringOpt<10u> s1{initTxt};
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














TEST(SmallStringOptimizationTests, equal)
{
    const char* initTxt = "abc";
    const std::string str{initTxt};

    const CSmallStringOpt<10u> s1{initTxt};
    const CSmallStringOpt<10u> s2{initTxt};
    const CSmallStringOpt<15u> s3{initTxt};
    const CSmallStringOpt<5u> s4{initTxt};

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



TEST(SmallStringOptimizationTests, notEqual)
{
    const char* initTxt1 = "aaa";
    const char* initTxt2 = "bbb";
    const char* initTxt3 = "ccccc";
    const std::string str1{initTxt1};

    const CSmallStringOpt<10u> s1{initTxt1};
    const CSmallStringOpt<10u> s2{initTxt2};
    const CSmallStringOpt<15u> s3{initTxt3};

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

TEST(SmallStringOptimizationTests, lessGreater)
{
    const char* initTxt1 = "aaa";
    const char* initTxt2 = "bbbb";
    const char* initTxt3 = "ccccc";
    const std::string str2{initTxt2};

    const CSmallStringOpt<10u> s1{initTxt1};
    const CSmallStringOpt<10u> s2{initTxt2};
    const CSmallStringOpt<15u> s3{initTxt3};

    const CSmallStringOpt<15u> s4{initTxt2};

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




TEST(SmallStringOptimizationTests, assign)
{
    const char* txt1 = "abcd";
    const std::string str1{txt1};

    CSmallStringOpt<10u> s1;
    s1.assign(txt1);
    ASSERT_EQ(s1, txt1);

    s1.assign(txt1, 2);
    ASSERT_EQ(s1, "ab");

    s1.assign(str1);
    ASSERT_EQ(s1, txt1);
}

TEST(SmallStringOptimizationTests, backFront)
{
    const char* txt1 = "abcd";
    const std::string str1{txt1};

    CSmallStringOpt<10u> s1{str1};
    ASSERT_EQ(s1.front(), 'a');
    ASSERT_EQ(s1.back(), 'd');
}

TEST(SmallStringOptimizationTests, backFrontConst)
{
    const char* txt1 = "def";
    const std::string str1{txt1};

    const CSmallStringOpt<10u> s1{str1};
    ASSERT_EQ(s1.front(), 'd');
    ASSERT_EQ(s1.back(), 'f');
}


TEST(SmallStringOptimizationTests, arrayOperator)
{
    const char* txt1 = "abcd";
    const std::string str1{txt1};

    CSmallStringOpt<10u> s1{str1};
    ASSERT_EQ(s1[0], 'a');
    ASSERT_EQ(s1[1], 'b');
    ASSERT_EQ(s1[2], 'c');
    ASSERT_EQ(s1[3], 'd');
    ASSERT_EQ(s1[4], '\0');
}


TEST(SmallStringOptimizationTests, arrayOperatorConst)
{
    const char* txt1 = "abc";
    const std::string str1{txt1};

    const CSmallStringOpt<10u> s1{str1};
    ASSERT_EQ(s1[0], 'a');
    ASSERT_EQ(s1[1], 'b');
    ASSERT_EQ(s1[2], 'c');
    ASSERT_EQ(s1[3], '\0');
}


TEST(SmallStringOptimizationTests, appendSmallStringOpt)
{
    
    CSmallStringOpt<10u> str1{"abc"};
    CSmallStringOpt<4u> str2{"defghij"};
    CSmallStringOpt<10u> str3{"123"};
    CSmallStringOpt<10u> str4{"123456789012345"};
    

    CSmallStringOpt<10u> s1;
    s1.append(str1);
    ASSERT_STREQ(s1.data(), "abc");

    s1.append(str2);
    ASSERT_STREQ(s1.data(), "abcdefghij");
    
    s1.append(str3);
    ASSERT_STREQ(s1.data(), "abcdefghij123");
    
    s1.append(str4);
    ASSERT_STREQ(s1.data(), "abcdefghij123123456789012345");
}

TEST(SmallStringOptimizationTests, appendChar)
{
    const char* txt1 = "abcd";

    CSmallStringOpt<10u> s1;
    s1.append(txt1);
    ASSERT_STREQ(s1.data(), "abcd");

    s1.append(txt1, 2);
    ASSERT_STREQ(s1.data(), "abcdab");

    s1.append(txt1);
    ASSERT_STREQ(s1.data(), "abcdababcd");
}


TEST(SmallStringOptimizationTests, appendString)
{
    const std::string txt1{"abcd"};

    CSmallStringOpt<10u> s1;
    s1.append(txt1);
    ASSERT_STREQ(s1.data(), "abcd");

    s1.append(txt1);
    ASSERT_STREQ(s1.data(), "abcdabcd");

    s1.append(txt1);
    ASSERT_STREQ(s1.data(), "abcdabcdabcd");
}


TEST(SmallStringOptimizationTests, plusEqualSmallStringOpt)
{
    
    CSmallStringOpt<10u> str1{"abc"};
    CSmallStringOpt<4u> str2{"defghij"};
    CSmallStringOpt<10u> str3{"123"};
    CSmallStringOpt<10u> str4{"123456789012345"};
    

    CSmallStringOpt<10u> s1;
    s1 += str1;
    ASSERT_STREQ(s1.data(), "abc");
    
    s1 += str2;
    ASSERT_STREQ(s1.data(), "abcdefghij");
    
    s1 += str3;
    ASSERT_STREQ(s1.data(), "abcdefghij123");
    
    s1 += str4;
    ASSERT_STREQ(s1.data(), "abcdefghij123123456789012345");
}

TEST(SmallStringOptimizationTests, plusEqualChar)
{
    const char* txt1 = "abcd";
    const char* txt2 = "ab";

    CSmallStringOpt<10u> s1;
    s1 += txt1;
    ASSERT_STREQ(s1.data(), "abcd");

    s1 += txt2;
    ASSERT_STREQ(s1.data(), "abcdab");
    
    s1 += txt1;
    ASSERT_STREQ(s1.data(), "abcdababcd");
}


TEST(SmallStringOptimizationTests, plusEqualString)
{
    const std::string txt1{"abcd"};

    CSmallStringOpt<10u> s1;
    s1 += txt1;
    ASSERT_STREQ(s1.data(), "abcd");
    
    s1 += txt1;
    ASSERT_STREQ(s1.data(), "abcdabcd");
    
    s1 += txt1;
    ASSERT_STREQ(s1.data(), "abcdabcdabcd");
}


TEST(SmallStringOptimizationTests, moveConstr)
{
    {
        CSmallStringOpt<10u> s1{"abc"};
        CSmallStringOpt<10u> s2{std::move(s1)};
        ASSERT_STREQ(s2.data(), "abc");
    }
    {
        CSmallStringOpt<10u> s1{"abcdefghijklmnop"};
        CSmallStringOpt<10u> s2{std::move(s1)};
        ASSERT_STREQ(s2.data(), "abcdefghijklmnop");
    }
    {
        CSmallStringOpt<9u> s1{"abc"};
        CSmallStringOpt<10u> s2{std::move(s1)};
        ASSERT_STREQ(s2.data(), "abc");
    }
    {
        CSmallStringOpt<9u> s1{"abcdefghijklmnop"};
        CSmallStringOpt<10u> s2{std::move(s1)};
        ASSERT_STREQ(s2.data(), "abcdefghijklmnop");
    }
    {
        CSmallStringOpt<11u> s1{"abc"};
        CSmallStringOpt<10u> s2{std::move(s1)};
        ASSERT_STREQ(s2.data(), "abc");
    }
    {
        CSmallStringOpt<9u> s1{"abcdefghijklmnop"};
        CSmallStringOpt<10u> s2{std::move(s1)};
        ASSERT_STREQ(s2.data(), "abcdefghijklmnop");
    }
}


TEST(SmallStringOptimizationTests, moveAssign)
{
    {
        CSmallStringOpt<10u> s1{"abc"};
        CSmallStringOpt<10u> s2{"123"};
        s2 = std::move(s1);
        ASSERT_STREQ(s2.data(), "abc");
    }
    {
        CSmallStringOpt<10u> s1{"abcdefghijklmnop"};
        CSmallStringOpt<10u> s2{"123"};
        s2 = std::move(s1);
        ASSERT_STREQ(s2.data(), "abcdefghijklmnop");
    }
    {
        CSmallStringOpt<9u> s1{"abc"};
        CSmallStringOpt<10u> s2{"123"};
        s2 = std::move(s1);
        ASSERT_STREQ(s2.data(), "abc");
    }
    {
        CSmallStringOpt<9u> s1{"abcdefghijklmnop"};
        CSmallStringOpt<10u> s2{"123"};
        s2 = std::move(s1);
        ASSERT_STREQ(s2.data(), "abcdefghijklmnop");
    }
    {
        CSmallStringOpt<11u> s1{"abc"};
        CSmallStringOpt<10u> s2{"123"};
        s2 = std::move(s1);
        ASSERT_STREQ(s2.data(), "abc");
    }
    {
        CSmallStringOpt<9u> s1{"abcdefghijklmnop"};
        CSmallStringOpt<10u> s2{"123"};
        s2 = std::move(s1);
        ASSERT_STREQ(s2.data(), "abcdefghijklmnop");
    }
}
