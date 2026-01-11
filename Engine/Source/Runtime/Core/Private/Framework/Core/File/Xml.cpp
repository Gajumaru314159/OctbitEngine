//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/File/Xml.h>
#include <Framework/Core/File/File.h>
#include <fstream>
#include <sstream>
#include <charconv>
#include <algorithm>

namespace ob::core {

	//===============================================================
	// XmlNode
	//===============================================================

	XmlNode::XmlNode(StringView name)
		: name(name) {
	}

	XmlNode::XmlNode(StringView name, Map<String, String> attributes)
		: name(name), attributes(std::move(attributes)) {
	}

	XmlNode& XmlNode::at(StringView name) {
		for (auto& child : children) {
			if (child.name == name) {
				return child;
			}
		}
		throw XmlException(Format("Child element '{}' not found", name));
	}

	const XmlNode& XmlNode::at(StringView name) const {
		for (const auto& child : children) {
			if (child.name == name) {
				return child;
			}
		}
		throw XmlException(Format("Child element '{}' not found", name));
	}

	XmlNode& XmlNode::at(StringView name, size_t index) {
		if (index == 0) {
			throw XmlException("Index must be 1-based (XPath compatible)");
		}
		size_t count = 0;
		for (auto& child : children) {
			if (child.name == name) {
				++count;
				if (count == index) {
					return child;
				}
			}
		}
		throw XmlException(Format("Child element '{}' with index {} not found", name, index));
	}

	const XmlNode& XmlNode::at(StringView name, size_t index) const {
		if (index == 0) {
			throw XmlException("Index must be 1-based (XPath compatible)");
		}
		size_t count = 0;
		for (const auto& child : children) {
			if (child.name == name) {
				++count;
				if (count == index) {
					return child;
				}
			}
		}
		throw XmlException(Format("Child element '{}' with index {} not found", name, index));
	}

	bool XmlNode::has(StringView name) const {
		for (const auto& child : children) {
			if (child.name == name) {
				return true;
			}
		}
		return false;
	}

	size_t XmlNode::count(StringView name) const {
		size_t cnt = 0;
		for (const auto& child : children) {
			if (child.name == name) {
				++cnt;
			}
		}
		return cnt;
	}

	XmlNode& XmlNode::add(XmlNode child) {
		children.push_back(std::move(child));
		return children.back();
	}

	bool XmlNode::remove(StringView name) {
		for (auto it = children.begin(); it != children.end(); ++it) {
			if (it->name == name) {
				children.erase(it);
				return true;
			}
		}
		return false;
	}

	bool XmlNode::remove(StringView name, size_t index) {
		if (index == 0) {
			return false;
		}
		size_t count = 0;
		for (auto it = children.begin(); it != children.end(); ++it) {
			if (it->name == name) {
				++count;
				if (count == index) {
					children.erase(it);
					return true;
				}
			}
		}
		return false;
	}

	bool XmlNode::has_attr(StringView key) const {
		return attributes.find(String(key)) != attributes.end();
	}

	String& XmlNode::attr(StringView key) {
		return attributes[String(key)];
	}

	String XmlNode::attr(StringView key) const {
		auto it = attributes.find(String(key));
		if (it != attributes.end()) {
			return it->second;
		}
		return String();
	}

	s32 XmlNode::attr_int(StringView key, s32 fallback) const {
		auto opt = attr_int(key);
		return opt.has_value() ? opt.value() : fallback;
	}

	Optional<s32> XmlNode::attr_int(StringView key) const {
		auto it = attributes.find(String(key));
		if (it == attributes.end()) {
			return std::nullopt;
		}
		const auto& str = it->second;
		s32 value = 0;
		auto result = std::from_chars(str.data(), str.data() + str.size(), value);
		if (result.ec == std::errc()) {
			return value;
		}
		return std::nullopt;
	}

	f32 XmlNode::attr_float(StringView key, f32 fallback) const {
		auto opt = attr_float(key);
		return opt.has_value() ? opt.value() : fallback;
	}

	Optional<f32> XmlNode::attr_float(StringView key) const {
		auto it = attributes.find(String(key));
		if (it == attributes.end()) {
			return std::nullopt;
		}
		const auto& str = it->second;
		try {
			size_t pos;
			f32 value = std::stof(std::string(str.data(), str.size()), &pos);
			if (pos > 0) {
				return value;
			}
		}
		catch (...) {
		}
		return std::nullopt;
	}

	bool XmlNode::attr_bool(StringView key, bool fallback) const {
		auto opt = attr_bool(key);
		return opt.has_value() ? opt.value() : fallback;
	}

	Optional<bool> XmlNode::attr_bool(StringView key) const {
		auto it = attributes.find(String(key));
		if (it == attributes.end()) {
			return std::nullopt;
		}
		String str = it->second;
		// 小文字に変換
		std::transform(str.begin(), str.end(), str.begin(), [](char c) {
			return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
			});
		if (str == "true" || str == "1" || str == "yes") {
			return true;
		}
		if (str == "false" || str == "0" || str == "no") {
			return false;
		}
		return std::nullopt;
	}

	bool XmlNode::remove_attr(StringView key) {
		return attributes.erase(String(key)) > 0;
	}

	// エンティティのエスケープ
	static String escapeXml(StringView str) {
		String result;
		result.reserve(str.size());
		for (char c : str) {
			switch (c) {
			case '&':  result += "&amp;"; break;
			case '<':  result += "&lt;"; break;
			case '>':  result += "&gt;"; break;
			case '"':  result += "&quot;"; break;
			case '\'': result += "&apos;"; break;
			default:   result += c; break;
			}
		}
		return result;
	}

	String XmlNode::string(s32 indent) const {
		std::stringstream ss;
		String indentStr(indent * 2, ' ');

		// 特殊ノード: #comment
		if (name == "#comment") {
			ss << indentStr << "<!--" << attr("content") << "-->\n";
			return String(ss.str());
		}

		// 特殊ノード: #text
		if (name == "#text") {
			String content = attr("content");
			if (!content.empty()) {
				ss << indentStr << escapeXml(content) << "\n";
			}
			return String(ss.str());
		}

		// 通常の要素
		if (name.empty()) {
			// ルートノード（名前なし）の場合は子だけ出力
			for (const auto& child : children) {
				ss << child.string(indent);
			}
			return String(ss.str());
		}

		ss << indentStr << "<" << name;

		// 属性
		for (const auto& [key, value] : attributes) {
			ss << " " << key << "=\"" << escapeXml(value) << "\"";
		}

		if (children.empty()) {
			ss << "/>\n";
		}
		else {
			ss << ">\n";
			for (const auto& child : children) {
				ss << child.string(indent + 1);
			}
			ss << indentStr << "</" << name << ">\n";
		}

		return String(ss.str());
	}

	//===============================================================
	// Xml
	//===============================================================

	Xml::Xml() = default;

	Xml::Xml(StringView path) {
		load(path);
	}

	bool Xml::load(StringView path) {
		m_path = path;

		std::basic_ifstream<Char> ifs(m_path.c_str());
		if (!ifs) {
			return false;
		}

		std::basic_stringstream<Char> ss;
		ss << ifs.rdbuf();

		return parse(ss.str());
	}

	bool Xml::save(StringView path) {
		String savePath = path.empty() ? m_path : String(path);
		if (savePath.empty()) {
			return false;
		}

		File fs(savePath, FileOpenMode::Write);
		if (!fs) {
			return false;
		}

		auto str = string();
		return fs.write(str.data(), str.size());
	}

	// エンティティ参照のアンエスケープ
	static String unescapeXml(StringView str) {
		String result;
		result.reserve(str.size());

		for (size_t i = 0; i < str.size(); ++i) {
			if (str[i] == '&') {
				// エンティティ参照を検索
				size_t endPos = str.find(';', i);
				if (endPos != String::npos) {
					StringView entity = str.substr(i + 1, endPos - i - 1);
					if (entity == "lt") {
						result += '<';
						i = endPos;
						continue;
					}
					else if (entity == "gt") {
						result += '>';
						i = endPos;
						continue;
					}
					else if (entity == "amp") {
						result += '&';
						i = endPos;
						continue;
					}
					else if (entity == "quot") {
						result += '"';
						i = endPos;
						continue;
					}
					else if (entity == "apos") {
						result += '\'';
						i = endPos;
						continue;
					}
					else if (!entity.empty() && entity[0] == '#') {
						// 文字参照
						int codePoint = 0;
						if (entity.size() > 1 && entity[1] == 'x') {
							// 16進数
							auto res = std::from_chars(entity.data() + 2, entity.data() + entity.size(), codePoint, 16);
							if (res.ec == std::errc()) {
								result += static_cast<char>(codePoint);
								i = endPos;
								continue;
							}
						}
						else {
							// 10進数
							auto res = std::from_chars(entity.data() + 1, entity.data() + entity.size(), codePoint);
							if (res.ec == std::errc()) {
								result += static_cast<char>(codePoint);
								i = endPos;
								continue;
							}
						}
					}
				}
			}
			result += str[i];
		}
		return result;
	}

	// 空白文字かどうか
	static bool isWhitespace(char c) {
		return c == ' ' || c == '\t' || c == '\n' || c == '\r';
	}

	// 空白をスキップ
	static size_t skipWhitespace(StringView text, size_t pos) {
		while (pos < text.size() && isWhitespace(text[pos])) {
			++pos;
		}
		return pos;
	}

	// パース用コンテキスト
	struct ParseContext {
		StringView text;
		size_t pos = 0;
		s32 line = 1;
		s32 column = 1;

		char peek() const {
			return pos < text.size() ? text[pos] : '\0';
		}

		char get() {
			if (pos >= text.size()) return '\0';
			char c = text[pos++];
			if (c == '\n') {
				++line;
				column = 1;
			}
			else {
				++column;
			}
			return c;
		}

		void skipWhitespace() {
			while (pos < text.size() && isWhitespace(text[pos])) {
				get();
			}
		}

		bool match(StringView str) {
			if (pos + str.size() > text.size()) return false;
			for (size_t i = 0; i < str.size(); ++i) {
				if (text[pos + i] != str[i]) return false;
			}
			for (size_t i = 0; i < str.size(); ++i) {
				get();
			}
			return true;
		}

		bool startsWith(StringView str) const {
			if (pos + str.size() > text.size()) return false;
			for (size_t i = 0; i < str.size(); ++i) {
				if (text[pos + i] != str[i]) return false;
			}
			return true;
		}
	};

	// 名前の解析
	static String parseName(ParseContext& ctx) {
		String name;
		// 名前の開始文字
		char c = ctx.peek();
		if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == ':')) {
			return name;
		}
		name += ctx.get();

		// 名前の継続文字
		while (true) {
			c = ctx.peek();
			if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
				(c >= '0' && c <= '9') || c == '_' || c == ':' || c == '-' || c == '.') {
				name += ctx.get();
			}
			else {
				break;
			}
		}
		return name;
	}

	// 属性値の解析
	static String parseAttributeValue(ParseContext& ctx) {
		char quote = ctx.get(); // ' or "
		if (quote != '"' && quote != '\'') {
			return String();
		}

		String value;
		while (ctx.peek() != quote && ctx.peek() != '\0') {
			value += ctx.get();
		}
		if (ctx.peek() == quote) {
			ctx.get(); // 閉じ引用符
		}
		return unescapeXml(value);
	}

	// ノードの解析（前方宣言）
	static bool parseNode(ParseContext& ctx, XmlNode& parent);

	// テキストコンテンツの解析
	static void parseTextContent(ParseContext& ctx, XmlNode& parent) {
		String text;
		while (ctx.peek() != '<' && ctx.peek() != '\0') {
			text += ctx.get();
		}

		// 空白のみのテキストは無視
		bool hasNonWhitespace = false;
		for (char c : text) {
			if (!isWhitespace(c)) {
				hasNonWhitespace = true;
				break;
			}
		}

		if (hasNonWhitespace) {
			// トリム
			size_t start = 0;
			while (start < text.size() && isWhitespace(text[start])) ++start;
			size_t end = text.size();
			while (end > start && isWhitespace(text[end - 1])) --end;

			XmlNode textNode("#text");
			textNode.attributes["content"] = unescapeXml(text.substr(start, end - start));
			parent.children.push_back(std::move(textNode));
		}
	}

	// 要素の解析
	static bool parseElement(ParseContext& ctx, XmlNode& parent) {
		ctx.get(); // '<'を消費

		// 終了タグか確認
		if (ctx.peek() == '/') {
			return false; // 終了タグは呼び出し元で処理
		}

		// コメント
		if (ctx.startsWith("!--")) {
			ctx.get(); ctx.get(); ctx.get(); // "!--"
			String comment;
			while (!ctx.startsWith("-->") && ctx.peek() != '\0') {
				comment += ctx.get();
			}
			if (ctx.startsWith("-->")) {
				ctx.get(); ctx.get(); ctx.get();
			}
			XmlNode commentNode("#comment");
			commentNode.attributes["content"] = comment;
			parent.children.push_back(std::move(commentNode));
			return true;
		}

		// DOCTYPE
		if (ctx.startsWith("!DOCTYPE") || ctx.startsWith("!doctype")) {
			while (ctx.peek() != '>' && ctx.peek() != '\0') {
				ctx.get();
			}
			if (ctx.peek() == '>') ctx.get();
			return true;
		}

		// CDATA
		if (ctx.startsWith("![CDATA[")) {
			for (int i = 0; i < 8; ++i) ctx.get(); // "![CDATA["
			String cdata;
			while (!ctx.startsWith("]]>") && ctx.peek() != '\0') {
				cdata += ctx.get();
			}
			if (ctx.startsWith("]]>")) {
				ctx.get(); ctx.get(); ctx.get();
			}
			// CDATAはテキストノードとして追加
			XmlNode textNode("#text");
			textNode.attributes["content"] = cdata;
			parent.children.push_back(std::move(textNode));
			return true;
		}

		// XML宣言
		if (ctx.startsWith("?xml") || ctx.startsWith("?XML")) {
			while (!ctx.startsWith("?>") && ctx.peek() != '\0') {
				ctx.get();
			}
			if (ctx.startsWith("?>")) {
				ctx.get(); ctx.get();
			}
			return true;
		}

		// 処理命令
		if (ctx.peek() == '?') {
			while (!ctx.startsWith("?>") && ctx.peek() != '\0') {
				ctx.get();
			}
			if (ctx.startsWith("?>")) {
				ctx.get(); ctx.get();
			}
			return true;
		}

		// 要素名
		String name = parseName(ctx);
		if (name.empty()) {
			return false;
		}

		XmlNode element(name);

		// 属性
		while (true) {
			ctx.skipWhitespace();
			char c = ctx.peek();
			if (c == '/' || c == '>' || c == '\0') break;

			String attrName = parseName(ctx);
			if (attrName.empty()) break;

			ctx.skipWhitespace();
			if (ctx.peek() == '=') {
				ctx.get();
				ctx.skipWhitespace();
				String attrValue = parseAttributeValue(ctx);
				element.attributes[attrName] = attrValue;
			}
			else {
				// 値なし属性
				element.attributes[attrName] = "";
			}
		}

		ctx.skipWhitespace();

		// 空要素タグ
		if (ctx.peek() == '/') {
			ctx.get();
			if (ctx.peek() == '>') {
				ctx.get();
			}
			parent.children.push_back(std::move(element));
			return true;
		}

		// '>'
		if (ctx.peek() == '>') {
			ctx.get();
		}

		// 子要素とテキスト
		while (ctx.peek() != '\0') {
			ctx.skipWhitespace();

			if (ctx.peek() == '<') {
				// 終了タグか確認
				if (ctx.pos + 1 < ctx.text.size() && ctx.text[ctx.pos + 1] == '/') {
					ctx.get(); ctx.get(); // "</"
					String endName = parseName(ctx);
					ctx.skipWhitespace();
					if (ctx.peek() == '>') ctx.get();
					if (endName == name) {
						break;
					}
					// 不一致の場合はエラーだが続行
					break;
				}
				parseNode(ctx, element);
			}
			else if (ctx.peek() != '\0') {
				parseTextContent(ctx, element);
			}
		}

		parent.children.push_back(std::move(element));
		return true;
	}

	static bool parseNode(ParseContext& ctx, XmlNode& parent) {
		ctx.skipWhitespace();

		if (ctx.peek() == '<') {
			return parseElement(ctx, parent);
		}
		else if (ctx.peek() != '\0') {
			parseTextContent(ctx, parent);
			return true;
		}
		return false;
	}

	bool Xml::parse(StringView text) {
		// クリア
		name.clear();
		attributes.clear();
		children.clear();

		ParseContext ctx;
		ctx.text = text;
		ctx.pos = 0;

		while (ctx.pos < ctx.text.size()) {
			ctx.skipWhitespace();
			if (ctx.peek() == '\0') break;
			if (!parseNode(ctx, *this)) {
				break;
			}
		}

		return true;
	}

	bool Xml::exists(StringView path) const {
		return find(path) != nullptr;
	}

	XmlNode* Xml::find(StringView path) {
		return const_cast<XmlNode*>(static_cast<const Xml*>(this)->find(path));
	}

	const XmlNode* Xml::find(StringView path) const {
		if (path.empty()) {
			return this;
		}

		// パスを解析
		size_t start = 0;
		if (!path.empty() && path[0] == '/') {
			++start;
		}

		const XmlNode* current = this;

		while (start < path.size()) {
			// 次のセグメントを取得
			size_t end = path.find('/', start);
			if (end == StringView::npos) {
				end = path.size();
			}

			StringView segment = path.substr(start, end - start);
			if (segment.empty()) {
				start = end + 1;
				continue;
			}

			// インデックスを解析 [n]
			String name;
			size_t index = 1;

			size_t bracketPos = segment.find('[');
			if (bracketPos != StringView::npos) {
				name = String(segment.substr(0, bracketPos));
				size_t bracketEnd = segment.find(']', bracketPos);
				if (bracketEnd != StringView::npos) {
					StringView indexStr = segment.substr(bracketPos + 1, bracketEnd - bracketPos - 1);
					int idx = 0;
					auto result = std::from_chars(indexStr.data(), indexStr.data() + indexStr.size(), idx);
					if (result.ec == std::errc() && idx > 0) {
						index = static_cast<size_t>(idx);
					}
				}
			}
			else {
				name = String(segment);
			}

			// 子ノードを検索
			const XmlNode* found = nullptr;
			size_t count = 0;
			for (const auto& child : current->children) {
				if (child.name == name) {
					++count;
					if (count == index) {
						found = &child;
						break;
					}
				}
			}

			if (!found) {
				return nullptr;
			}

			current = found;
			start = end + 1;
		}

		return current;
	}

}
