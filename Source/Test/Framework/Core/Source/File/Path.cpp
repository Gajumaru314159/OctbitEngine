//***********************************************************
//! @file
//! @brief		Path のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/File/Path.h>

using namespace ob;

TEST(PathTest, Constructors)
{
    Path emptyPath;
    EXPECT_TRUE(emptyPath.empty());

    Path pathFromStdString("C:/Program Files/MyApp");
    EXPECT_EQ(pathFromStdString.string(), "C:/Program Files/MyApp");

    Path pathFromCString("C:\\Users\\Admin\\Documents");
    EXPECT_EQ(pathFromCString.string(), "C:/Users/Admin/Documents");

    Path pathFromPath(pathFromCString);
    EXPECT_EQ(pathFromPath.string(), pathFromCString.string());
}

TEST(PathTest, Assignment)
{
    Path emptyPath;
    Path path("C:/Windows");

    emptyPath = path;
    EXPECT_EQ(emptyPath.string(), "C:/Windows");

    emptyPath = "C:/Program Files";
    EXPECT_EQ(emptyPath.string(), "C:/Program Files");
}

TEST(PathTest, Concatenation)
{
    Path path("C:/Program Files");
    path /= "MyApp";

    EXPECT_EQ(path.string(), "C:/Program Files/MyApp");

    Path newPath = path / "bin";
    EXPECT_EQ(newPath.string(), "C:/Program Files/MyApp/bin");
}

TEST(PathTest, Extension)
{
    Path path("C:/Users/Public/image.png");
    EXPECT_EQ(path.extension(), "png");
    EXPECT_EQ(path.extension(WithDot::Yes), ".png");

    path.removeExtension();
    EXPECT_EQ(path.string(), "C:/Users/Public/image");

    path.replaceExtension(".jpg");
    EXPECT_EQ(path.string(), "C:/Users/Public/image.jpg");

    EXPECT_TRUE(path.hasExtension(".jpg"));
    EXPECT_FALSE(path.hasExtension(".png"));
}

TEST(PathTest, Accessor)
{
    Path path("C:/Users/Public/image.png");
    EXPECT_EQ(path.fileName(), "image.png");
    EXPECT_EQ(path.stem(), "image");

    path.replaceFileName("logo.jpg");
    EXPECT_EQ(path.fileName(), "logo.jpg");

    path.replaceStem("icon");
    EXPECT_EQ(path.fileName(), "icon.jpg");
}

TEST(PathTest, Directory)
{
    Path path("C:/Users/Public/image.png");
    EXPECT_FALSE(path.isDirectory());

    Path directoryPath = path.parent();
    EXPECT_TRUE(directoryPath.isDirectory());
    EXPECT_EQ(directoryPath.string(), "C:/Users/Public/");

    directoryPath /= "pictures";
    EXPECT_FALSE(directoryPath.isDirectory());
    EXPECT_EQ(directoryPath.string(), "C:/Users/Public/pictures");
}

TEST(PathTest, Root)
{
    Path path("C:/Program Files");
    EXPECT_FALSE(path.isRoot());

    Path rootPath = path.parent();
    EXPECT_TRUE(rootPath.isRoot());
    EXPECT_EQ(rootPath.string(), "C:/");
}