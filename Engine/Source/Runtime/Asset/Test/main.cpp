//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************

int OctbitMain(int argc, char** argv) {
    GTEST_FLAG_SET(death_test_style, "threadsafe");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}