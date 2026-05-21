// Copyright (c) 2024 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <util/syserror.h>

#include <cerrno>
#include <string>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(util_syserror_tests)

BOOST_AUTO_TEST_CASE(syserrorstring_test)
{
    // Test a successful operation (errno = 0)
    std::string err_str_0 = SysErrorString(0);
    BOOST_CHECK(err_str_0.find("(0)") != std::string::npos);

    // Test standard POSIX errors (e.g., ENOENT)
    std::string err_str_enoent = SysErrorString(ENOENT);
    BOOST_CHECK(err_str_enoent.find("No such file or directory") != std::string::npos ||
                err_str_enoent.find("system") != std::string::npos); // fallback/variation
    BOOST_CHECK(err_str_enoent.find("(" + std::to_string(ENOENT) + ")") != std::string::npos);

    // Test a non-existent/invalid error code
    // The exact behavior depends on the OS, but it should at least include the code
    std::string err_str_invalid = SysErrorString(-1);
    BOOST_CHECK(err_str_invalid.find("(-1)") != std::string::npos);
}

#if defined(WIN32)
BOOST_AUTO_TEST_CASE(win32errorstring_test)
{
    // Test a standard Windows error code, e.g., ERROR_FILE_NOT_FOUND (2)
    std::string err_str_2 = Win32ErrorString(2);
    BOOST_CHECK(err_str_2.find("(2)") != std::string::npos);

    // Test an unknown/invalid error code
    std::string err_str_invalid = Win32ErrorString(-1);
    BOOST_CHECK(err_str_invalid.find("Unknown error (-1)") != std::string::npos);
}
#endif

BOOST_AUTO_TEST_SUITE_END()
