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
#include "util.h"

#ifdef _WIN32
// It appears that rules_libsdl12 may be setting /SUBSYSTEM:WINDOWS as a
// transitive linkopt on Windows non-debug builds, which would cause the CRT
// to require WinMain rather than main. This WinMain shim delegates to main()
// so that gtest_main works correctly even when SUBSYSTEM:WINDOWS is forced.
// TODO: The proper fix is for rules_libsdl12 to expose a console-subsystem
// variant target that unit tests can link against without inheriting the
// SUBSYSTEM:WINDOWS linkopt. Until then, this shim is required.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>  // provides __argc/__argv with correct dllimport linkage
// Forward-declare main() which is provided by gtest_main.
int main(int argc, char** argv);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   PSTR lpCmdLine, int nCmdShow) {
    (void)hInstance; (void)hPrevInstance; (void)lpCmdLine; (void)nCmdShow;
    return main(__argc, __argv);
}
#endif

// Demonstrate some basic assertions.
TEST(StrReplaceTest, BasicAssertions) {
  auto got = str_replace(
      std::string("hello"),
      std::string("hi"),
      std::string("hello world"));
  auto want = "hi world";

  EXPECT_STREQ(got.c_str(), want);
}

TEST(EqualityTest, BasicAssertions) {
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}