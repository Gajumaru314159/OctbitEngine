//***********************************************************
//! @file
//! @brief		エントリー
//! @author		Gajumaru
//***********************************************************
int main(int argc, char** argv)
{
    ob::Logger logger;

    return CommandLineTestRunner::RunAllTests(argc, argv);
}