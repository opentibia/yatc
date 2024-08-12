//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Minimal test to verify the test system itself works.
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#include <string>
#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(StrReplaceTest, BasicAssertions) {
  auto haystack = std::string("hello world");
  auto needle = std::string("hello");
  auto replace = std::string("hi");

  auto got =
      haystack.replace(haystack.find(needle), needle.size(), replace);
  auto want = "hi world";

  EXPECT_STREQ(got.c_str(), want);
  // Expect equality.
  //EXPECT_EQ(7 * 6, 42);
}
