//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/File/Xml.h>
#include <filesystem>

using namespace ob;

TEST(XmlNode, Constructor) {
	// デフォルトコンストラクタ
	XmlNode node1;
	EXPECT_TRUE(node1.name.empty());
	EXPECT_TRUE(node1.attributes.empty());
	EXPECT_TRUE(node1.children.empty());

	// 要素名を指定
	XmlNode node2("div");
	EXPECT_EQ(node2.name, "div");
	EXPECT_TRUE(node2.attributes.empty());

	// 要素名と属性を指定
	XmlNode node3("div", { {"class", "container"}, {"id", "main"} });
	EXPECT_EQ(node3.name, "div");
	EXPECT_EQ(node3.attributes["class"], "container");
	EXPECT_EQ(node3.attributes["id"], "main");
}

TEST(XmlNode, ChildAccess) {
	XmlNode root("root");
	root.add(XmlNode("child1"));
	root.add(XmlNode("child2"));
	root.add(XmlNode("child1"));  // 同名の2つ目

	// has
	EXPECT_TRUE(root.has("child1"));
	EXPECT_TRUE(root.has("child2"));
	EXPECT_FALSE(root.has("child3"));

	// count
	EXPECT_EQ(root.count("child1"), 2u);
	EXPECT_EQ(root.count("child2"), 1u);
	EXPECT_EQ(root.count("child3"), 0u);

	// at(name)
	EXPECT_EQ(root.at("child1").name, "child1");
	EXPECT_EQ(root.at("child2").name, "child2");

	// at(name, index) - 1始まり
	EXPECT_EQ(root.at("child1", 1).name, "child1");
	EXPECT_EQ(root.at("child1", 2).name, "child1");

	// 存在しない要素へのアクセスで例外
	EXPECT_THROW(root.at("nonexistent"), XmlException);
	EXPECT_THROW(root.at("child1", 3), XmlException);
	EXPECT_THROW(root.at("child1", 0), XmlException);  // 0は無効
}

TEST(XmlNode, ChildRemove) {
	XmlNode root("root");
	root.add(XmlNode("child1"));
	root.add(XmlNode("child2"));
	root.add(XmlNode("child1"));

	// remove(name) - 最初のものを削除
	EXPECT_TRUE(root.remove("child1"));
	EXPECT_EQ(root.count("child1"), 1u);

	// remove(name, index)
	root.add(XmlNode("child1"));
	EXPECT_EQ(root.count("child1"), 2u);
	EXPECT_TRUE(root.remove("child1", 2));
	EXPECT_EQ(root.count("child1"), 1u);

	// 存在しない要素の削除
	EXPECT_FALSE(root.remove("nonexistent"));
	EXPECT_FALSE(root.remove("child1", 5));
	EXPECT_FALSE(root.remove("child1", 0));
}

TEST(XmlNode, AttributeAccess) {
	XmlNode node("div");
	node.attributes["class"] = "container";
	node.attributes["id"] = "main";

	// has_attr
	EXPECT_TRUE(node.has_attr("class"));
	EXPECT_TRUE(node.has_attr("id"));
	EXPECT_FALSE(node.has_attr("style"));

	// attr() const
	const XmlNode& constNode = node;
	EXPECT_EQ(constNode.attr("class"), "container");
	EXPECT_EQ(constNode.attr("nonexistent"), "");

	// attr() 非const（作成）
	node.attr("style") = "color: red";
	EXPECT_EQ(node.attr("style"), "color: red");

	// remove_attr
	EXPECT_TRUE(node.remove_attr("style"));
	EXPECT_FALSE(node.has_attr("style"));
	EXPECT_FALSE(node.remove_attr("nonexistent"));
}

TEST(XmlNode, AttributeConversion) {
	XmlNode node("test");
	node.attributes["int_val"] = "42";
	node.attributes["float_val"] = "3.14";
	node.attributes["bool_true"] = "true";
	node.attributes["bool_false"] = "no";
	node.attributes["bool_one"] = "1";
	node.attributes["invalid"] = "abc";

	// attr_int
	EXPECT_EQ(node.attr_int("int_val", 0), 42);
	EXPECT_EQ(node.attr_int("invalid", 100), 100);
	EXPECT_EQ(node.attr_int("nonexistent", 200), 200);

	auto intOpt = node.attr_int("int_val");
	EXPECT_TRUE(intOpt.has_value());
	EXPECT_EQ(intOpt.value(), 42);

	EXPECT_FALSE(node.attr_int("invalid").has_value());
	EXPECT_FALSE(node.attr_int("nonexistent").has_value());

	// attr_float
	EXPECT_FLOAT_EQ(node.attr_float("float_val", 0.0f), 3.14f);
	EXPECT_FLOAT_EQ(node.attr_float("invalid", 1.0f), 1.0f);

	auto floatOpt = node.attr_float("float_val");
	EXPECT_TRUE(floatOpt.has_value());
	EXPECT_FLOAT_EQ(floatOpt.value(), 3.14f);

	// attr_bool
	EXPECT_TRUE(node.attr_bool("bool_true", false));
	EXPECT_FALSE(node.attr_bool("bool_false", true));
	EXPECT_TRUE(node.attr_bool("bool_one", false));
	EXPECT_TRUE(node.attr_bool("invalid", true));  // fallback

	EXPECT_TRUE(node.attr_bool("bool_true").value());
	EXPECT_FALSE(node.attr_bool("bool_false").value());
	EXPECT_FALSE(node.attr_bool("invalid").has_value());
}

TEST(Xml, Parse) {
	Xml xml;
	xml.parse(R"(
<html>
    <body>
        <!-- comment -->
        <div class="sample"/>
        <span/>
        <div class="sample">
            aaa
        </div>
    </body>
</html>
)");

	// Xml自体がルート要素
	EXPECT_EQ(xml.name, "html");
	EXPECT_TRUE(xml.has("body"));

	auto& body = xml.at("body");
	EXPECT_EQ(body.count("div"), 2u);
	EXPECT_EQ(body.count("span"), 1u);

	// 属性アクセス
	EXPECT_EQ(body.at("div", 1).attr("class"), "sample");
	EXPECT_EQ(body.at("div", 2).attr("class"), "sample");

	// コメントノード
	EXPECT_TRUE(body.has("#comment"));
	EXPECT_EQ(body.at("#comment").attr("content"), " comment ");

	// テキストノード
	EXPECT_TRUE(body.at("div", 2).has("#text"));
	EXPECT_EQ(body.at("div", 2).at("#text").attr("content"), "aaa");
}

TEST(Xml, Find) {
	Xml xml;
	xml.parse(R"(
<html>
    <body>
        <!-- comment -->
        <div class="sample"/>
        <span/>
        <div class="sample">
            aaa
        </div>
    </body>
</html>
)");

	// exists
	EXPECT_TRUE(xml.exists("/html"));
	EXPECT_TRUE(xml.exists("/html/body"));
	EXPECT_TRUE(xml.exists("/html/body/div"));
	EXPECT_TRUE(xml.exists("/html/body/div[1]"));
	EXPECT_TRUE(xml.exists("/html/body/div[2]"));
	EXPECT_FALSE(xml.exists("/html/body/div[3]"));
	EXPECT_FALSE(xml.exists("/html/body/nonexistent"));

	// find
	auto* htmlNode = xml.find("/html");
	ASSERT_NE(htmlNode, nullptr);
	EXPECT_EQ(htmlNode->name, "html");

	auto* div2 = xml.find("/html/body/div[2]");
	ASSERT_NE(div2, nullptr);
	EXPECT_EQ(div2->attr("class"), "sample");

	auto* comment = xml.find("/html/body/#comment");
	ASSERT_NE(comment, nullptr);
	EXPECT_EQ(comment->attr("content"), " comment ");

	auto* text = xml.find("/html/body/div[2]/#text");
	ASSERT_NE(text, nullptr);
	EXPECT_EQ(text->attr("content"), "aaa");

	// 存在しないパス
	EXPECT_EQ(xml.find("/nonexistent"), nullptr);
}

TEST(Xml, EntityReference) {
	Xml xml;
	xml.parse(R"(
<root>
    <item value="&lt;test&gt;"/>
    <item value="&amp;&quot;&apos;"/>
    <item value="&#65;&#x42;"/>
</root>
)");

	auto* root = xml.find("/root");
	ASSERT_NE(root, nullptr);

	EXPECT_EQ(root->at("item", 1).attr("value"), "<test>");
	EXPECT_EQ(root->at("item", 2).attr("value"), "&\"'");
	EXPECT_EQ(root->at("item", 3).attr("value"), "AB");
}

TEST(Xml, XmlDeclarationAndDoctype) {
	Xml xml;
	xml.parse(R"(<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE html>
<html>
    <body/>
</html>
)");

	EXPECT_TRUE(xml.exists("/html"));
	EXPECT_TRUE(xml.exists("/html/body"));
}

TEST(Xml, CDATA) {
	Xml xml;
	xml.parse(R"(
<root>
    <script><![CDATA[
        if (a < b && b > c) {
            console.log("test");
        }
    ]]></script>
</root>
)");

	auto* script = xml.find("/root/script");
	ASSERT_NE(script, nullptr);
	EXPECT_TRUE(script->has("#text"));
}

TEST(Xml, NamespacePrefix) {
	Xml xml;
	xml.parse(R"(
<svg:svg xmlns:svg="http://www.w3.org/2000/svg">
    <svg:rect width="100" height="100"/>
</svg:svg>
)");

	EXPECT_TRUE(xml.exists("/svg:svg"));
	EXPECT_TRUE(xml.exists("/svg:svg/svg:rect"));

	auto* rect = xml.find("/svg:svg/svg:rect");
	ASSERT_NE(rect, nullptr);
	EXPECT_EQ(rect->attr("width"), "100");
}

TEST(Xml, String) {
	XmlNode root("root");
	XmlNode& child = root.add(XmlNode("item", { {"id", "1"}, {"name", "first"} }));
	child.add(XmlNode("value")).attr("content") = "100";

	String output = root.string();
	EXPECT_FALSE(output.empty());
	EXPECT_NE(output.find("<root>"), String::npos);
	EXPECT_NE(output.find("<item"), String::npos);
	EXPECT_NE(output.find("</root>"), String::npos);
}

TEST(Xml, FileIO) {
	auto filename = "test_xml.xml";
	std::filesystem::remove(filename);

	{
		Xml xml;
		xml.name = "config";
		xml.add(XmlNode("setting", { {"key", "value1"} }));
		xml.add(XmlNode("setting", { {"key", "value2"} }));
		EXPECT_TRUE(xml.save(filename));
	}

	{
		Xml xml;
		EXPECT_TRUE(xml.load(filename));
		EXPECT_EQ(xml.name, "config");
		EXPECT_TRUE(xml.exists("/config"));
		EXPECT_EQ(xml.count("setting"), 2u);
		EXPECT_EQ(xml.find("/config/setting[1]")->attr("key"), "value1");
		EXPECT_EQ(xml.find("/config/setting[2]")->attr("key"), "value2");
	}

	std::filesystem::remove(filename);
}

TEST(Xml, LoadNonexistentFile) {
	Xml xml;
	EXPECT_FALSE(xml.load("nonexistent_file.xml"));
}

TEST(Xml, SaveWithoutPath) {
	Xml xml;
	xml.name = "root";

	// パスが設定されていない場合はfalse
	EXPECT_FALSE(xml.save());
}

TEST(Xml, EmptyElement) {
	Xml xml;
	xml.parse(R"(<root><empty/><empty></empty></root>)");

	EXPECT_EQ(xml.name, "root");
	EXPECT_EQ(xml.count("empty"), 2u);
}

TEST(Xml, BuildXml) {
	Xml xml;
	xml.name = "root";
	XmlNode& child = xml.add(XmlNode("item", { {"id", "1"}, {"name", "first"} }));
	child.add(XmlNode("value")).attr("content") = "100";

	// 確認
	EXPECT_EQ(xml.name, "root");
	EXPECT_TRUE(xml.exists("/root"));
	EXPECT_TRUE(xml.exists("/root/item"));
	EXPECT_EQ(xml.find("/root/item")->attr("id"), "1");
	EXPECT_EQ(xml.find("/root/item")->attr("name"), "first");
}
