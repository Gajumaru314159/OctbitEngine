//***********************************************************
//! @file
//! @brief		エントリー
//! @author		Gajumaru
//***********************************************************

int main(int argc, char** argv)
{
    ob::Application app();

    if (!app.Run()) {
        return -1;
    }
    return 0;
}