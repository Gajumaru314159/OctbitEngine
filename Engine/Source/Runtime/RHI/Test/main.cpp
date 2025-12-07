//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************

int OctbitMain(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    GTEST_FLAG_SET(death_test_style, "threadsafe");
    return RUN_ALL_TESTS();
}