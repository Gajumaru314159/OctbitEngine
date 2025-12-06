//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Asset/Asset.h>
#include <Framework/Asset/AssetHandle.h>
#include <Framework/Asset/AssetHandle.h>

using namespace ob;
using namespace ob::asset;


class TestAsset : public Asset {
public:
    s32 getValue() const {
        return m_value;
    }
private:
    s32 m_value = 123;
};
using TestAssetHandle = AssetHandle<TestAsset>;
/*
TEST(Asset, AssetHandle)
{
    AssetManager manager;

    AssetId assetId;
    TestAssetHandle hTexture;

    EXPECT_FALSE(hTexture.isReady());

    hTexture.load(
        assetId,
        [&]() {
            EXPECT_TRUE(hTexture.isReady());
            // EXPECT_EQ(hTexture.getPath(),"");
            EXPECT_EQ(hTexture->getId(),assetId);
            EXPECT_EQ(hTexture->getValue(),123);
        }
    );

}
*/