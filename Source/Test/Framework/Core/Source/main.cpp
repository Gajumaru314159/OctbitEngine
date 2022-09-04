//***********************************************************
//! @file
//! @brief		エントリー
//! @author		Gajumaru
//***********************************************************
int main(int argc, char** argv) {

    // 依存関係のあるシステムを初期化
    ob::Logger logger;

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}