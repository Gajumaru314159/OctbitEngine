//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <gtest/gtest.h>
#include <Framework/Core/File/Path.h>

using namespace ob::core;

TEST(Path, Combine)
{
    String path1 = "C:/Program Files";
    String path2 = "MyApp";
    String path3 = "bin";

    String combinedPath = Path::Combine(path1, path2, path3);

    EXPECT_EQ(combinedPath, "C:/Program Files/MyApp/bin");
}

TEST(Path, Separator)
{
    char separator = Path::Separator();

    EXPECT_EQ(separator, '/');
}

TEST(Path, Normalize)
{
    String path = "C:\\Program Files\\MyApp";

    String normalizedPath = Path::Normalize(path);

    EXPECT_EQ(normalizedPath, "C:/Program Files/MyApp");
}

TEST(Path, FileName)
{
    String path = "C:/Users/Admin/Documents/report.docx";

    String fileName = Path::FileName(path);

    EXPECT_EQ(fileName, "report.docx");
}

TEST(Path, Stem)
{
    String path = "C:/Users/Admin/Documents/report.docx";

    String stem = Path::Stem(path);

    EXPECT_EQ(stem, "report");
}

TEST(Path, Extension)
{
    String path = "C:/Users/Admin/Documents/report.docx";

    String extension = Path::Extension(path);

    EXPECT_EQ(extension, "docx");
}

TEST(Path, Parent)
{
    String path = "C:/Users/Admin/Documents/report.docx";

    String parentPath = Path::Parent(path, 1);

    EXPECT_EQ(parentPath, "C:/Users/Admin/Documents");

    parentPath = Path::Parent(path, 2);

    EXPECT_EQ(parentPath, "C:/Users/Admin");
}

TEST(Path, IsAbsolute)
{
#ifdef OS_WINDOWS
    String absolutePath = "C:/Program Files/MyApp";
    String relativePath = "Documents/report.docx";
#elif defined(OS_LINUX)
    String absolutePath = "/mnt/c/Program Files/MyApp";
    String relativePath = "Documents/report.docx";
#endif
    bool isAbsolutePath = Path::IsAbsolute(absolutePath);
    bool isRelativePath = Path::IsAbsolute(relativePath);

    EXPECT_TRUE(isAbsolutePath);
    EXPECT_FALSE(isRelativePath);
}

TEST(Path, IsRelative)
{
#ifdef OS_WINDOWS
    String absolutePath = "C:/Program Files/MyApp";
    String relativePath = "Documents/report.docx";
#elif defined(OS_LINUX)
    String absolutePath = "/mnt/c/Program Files/MyApp";
    String relativePath = "Documents/report.docx";
#endif
    bool isAbsolutePath = Path::IsRelative(absolutePath);
    bool isRelativePath = Path::IsRelative(relativePath);

    EXPECT_FALSE(isAbsolutePath);
    EXPECT_TRUE(isRelativePath);
}

TEST(Path, HasExtension)
{
    String path = "C:/Users/Admin/Documents/report.docx";

    bool hasExtension = Path::HasExtension(path, ".docx");
    bool hasNoExtension = Path::HasExtension(path, ".txt");

    EXPECT_TRUE(hasExtension);
    EXPECT_FALSE(hasNoExtension);
}

TEST(Path, ReplaceExtension)
{
    String path = "C:/Users/Admin/Documents/report.docx";

    String newPath = Path::ReplaceExtension(path, ".pdf");

    EXPECT_EQ(newPath, "C:/Users/Admin/Documents/report.pdf");
}

TEST(Path, ReplaceStem)
{
    String path = "C:/Users/Admin/Documents/report.docx";

    String newPath = Path::ReplaceStem(path, "invoice");

    EXPECT_EQ(newPath, "C:/Users/Admin/Documents/invoice.docx");
}

TEST(Path, ReplaceFileName)
{
    String path = "C:/Users/Admin/Documents/report.docx";

    String newPath = Path::ReplaceFileName(path, "invoice.docx");

    EXPECT_EQ(newPath, "C:/Users/Admin/Documents/invoice.docx");
}

TEST(Path, RemoveExtension)
{
    String path = "C:/Users/Admin/Documents/report.docx";

    String newPath = Path::RemoveExtension(path);

    EXPECT_EQ(newPath, "C:/Users/Admin/Documents/report");
}

TEST(Path, RemoveFileName)
{
    String path = "C:/Users/Admin/Documents/report.docx";

    String newPath = Path::RemoveFileName(path);

    EXPECT_EQ(newPath, "C:/Users/Admin/Documents/");
}
