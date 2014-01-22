Selecting Tests
------------------------------
Running a Subset of the Tests
==============================
By default, a Google Test program runs all tests the user has defined. Sometimes, you want to run only a subset of the tests (e.g. for debugging or quickly verifying a change). If you set the GTEST_FILTER environment variable or the --gtest_filter flag to a filter string, Google Test will only run the tests whose full names (in the form of TestCaseName.TestName) match the filter.

The format of a filter is a ':'-separated list of wildcard patterns (called the positive patterns) optionally followed by a '-' and another ':'-separated pattern list (called the negative patterns). A test matches the filter if and only if it matches any of the positive patterns but does not match any of the negative patterns.

A pattern may contain '*' (matches any string) or '?' (matches any single character). For convenience, the filter '*-NegativePatterns' can be also written as '-NegativePatterns'.

For example:

./foo_test Has no flag, and thus runs all its tests.
./foo_test --gtest_filter=* Also runs everything, due to the single match-everything * value.
./foo_test --gtest_filter=FooTest.* Runs everything in test case FooTest.
./foo_test --gtest_filter=*Null*:*Constructor* Runs any test whose full name contains either "Null" or "Constructor".
./foo_test --gtest_filter=-*DeathTest.* Runs all non-death tests.
./foo_test --gtest_filter=FooTest.*-FooTest.Bar Runs everything in test case FooTest except FooTest.Bar.

-------------------------------

Temporarily Disabling Tests
================================
If you have a broken test that you cannot fix right away, you can add the DISABLED_ prefix to its name. This will exclude it from execution. This is better than commenting out the code or using #if 0, as disabled tests are still compiled (and thus won't rot).

If you need to disable all tests in a test case, you can either add DISABLED_ to the front of the name of each test, or alternatively add it to the front of the test case name.

For example, the following tests won't be run by Google Test, even though they will still be compiled:

// Tests that Foo does Abc.
TEST(FooTest, DISABLED_DoesAbc) { ... }

class DISABLED_BarTest : public ::testing::Test { ... };

// Tests that Bar does Xyz.
TEST_F(DISABLED_BarTest, DoesXyz) { ... }
Note: This feature should only be used for temporary pain-relief. You still have to fix the disabled tests at a later date. As a reminder, Google Test will print a banner warning you if a test program contains any disabled tests.

Tip: You can easily count the number of disabled tests you have using grep. This number can be used as a metric for improving your test quality.

-----------------------------------

Temporarily Enabling Disabled Tests
===================================
To include disabled tests in test execution, just invoke the test program with the --gtest_also_run_disabled_tests flag or set the GTEST_ALSO_RUN_DISABLED_TESTS environment variable to a value other than 0. You can combine this with the --gtest_filter flag to further select which disabled tests to run.

Availability: Linux, Windows, Mac; since version 1.3.0.

Repeating the Tests
Once in a while you'll run into a test whose result is hit-or-miss. Perhaps it will fail only 1% of the time, making it rather hard to reproduce the bug under a debugger. This can be a major source of frustration.

The --gtest_repeat flag allows you to repeat all (or selected) test methods in a program many times. Hopefully, a flaky test will eventually fail and give you a chance to debug. Here's how to use it:

$ foo_test --gtest_repeat=1000	 Repeat foo_test 1000 times and don't stop at failures.
$ foo_test --gtest_repeat=-1	 A negative count means repeating forever.
$ foo_test --gtest_repeat=1000 --gtest_break_on_failure	 Repeat foo_test 1000 times, stopping at the first failure. This is especially useful when running under a debugger: when the testfails, it will drop into the debugger and you can then inspect variables and stacks.
$ foo_test --gtest_repeat=1000 --gtest_filter=FooBar	 Repeat the tests whose name matches the filter 1000 times.
If your test program contains global set-up/tear-down code registered using AddGlobalTestEnvironment(), it will be repeated in each iteration as well, as the flakiness may be in it. You can also specify the repeat count by setting the GTEST_REPEAT environment variable.
