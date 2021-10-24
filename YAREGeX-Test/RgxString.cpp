#include "../YAREGeX/Rgx2Postfix.hpp"
#include "../YAREGeX/yaregex_common.h"
#include <gtest/gtest.h>

namespace YAReGexTest
{
namespace RgxString
{

// The fixture for testing class Foo.
class PostFixTest : public ::testing::Test
{
  public:
    std::unique_ptr<lambda::RgxString> m_RString;
    std::array<char, 4> m_Operators{{'.', '|', '+', '*'}};

  protected:
    PostFixTest()
    {
    }
    ~PostFixTest() override
    {
    }
};

TEST_F(PostFixTest, PostFixTest_Size)
{
    m_RString = std::make_unique<lambda::RgxString>("a.b");
    auto dis = std::distance(m_RString->begin(), m_RString->end());
    EXPECT_EQ(dis, 3);
}

TEST_F(PostFixTest, PostFixTest_SizeWithParanthesis)
{
    m_RString = std::make_unique<lambda::RgxString>("(a.b)");
    auto dis = std::distance(m_RString->begin(), m_RString->end());
    EXPECT_EQ(dis, 3);
}

TEST_F(PostFixTest, PostFixTest_OperatorPosition)
{
    m_RString = std::make_unique<lambda::RgxString>("(a.b)");
    char op = '.';
    EXPECT_EQ(*std::prev(m_RString->end()), op);
}

} // namespace RgxString
} // namespace YAReGexTest
