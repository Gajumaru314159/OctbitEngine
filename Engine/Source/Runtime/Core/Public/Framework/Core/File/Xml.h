//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Exception/Exception.h>

namespace ob::core {

	//! @brief XMLパースエラーに対する例外クラス
	class OB_API XmlException : public Exception {
	public:
		//! @brief デフォルトコンストラクタ
		XmlException() : Exception(), m_line(-1), m_column(-1) {}

		//! @brief メッセージを指定して生成
		XmlException(StringView message) : Exception(message), m_line(-1), m_column(-1) {}

		//! @brief メッセージと位置情報を指定して生成
		XmlException(StringView message, s32 line, s32 column)
			: Exception(message), m_line(line), m_column(column) {}

		//! @brief エラー発生行番号（パースエラー時）
		s32 line() const { return m_line; }

		//! @brief エラー発生列番号（パースエラー時）
		s32 column() const { return m_column; }

	private:
		s32 m_line;
		s32 m_column;
	};

	//! @brief XMLの要素を表現するクラス
	//! @details
	//! 属性、子ノード、テキストコンテンツを保持します。
	//!
	//! @par 特殊ノード
	//! - `#comment` - XMLコメント（属性"content"にコメント内容を保持）
	//! - `#text` - テキストノード（属性"content"にテキスト内容を保持）
	class XmlNode {
	public:
		String name;							//!< 要素名
		Map<String, String> attributes;			//!< 属性のキーと値のペア
		Vector<XmlNode> children;				//!< 子ノードのリスト

	public:
		//! @brief デフォルトコンストラクタ
		XmlNode() = default;

		//! @brief 要素名を指定して生成
		//! @param name 要素名
		explicit XmlNode(StringView name);

		//! @brief 要素名と属性を指定して生成
		//! @param name 要素名
		//! @param attributes 属性マップ
		XmlNode(StringView name, Map<String, String> attributes);

		//! @brief 名前を指定して子ノードにアクセス
		//! @param name 要素名
		//! @return 最初に見つかった子ノードへの参照
		//! @throw XmlException 該当する子ノードが存在しない場合
		XmlNode& at(StringView name);
		const XmlNode& at(StringView name) const;

		//! @brief 名前とインデックスを指定して子ノードにアクセス
		//! @param name 要素名
		//! @param index 同名要素の中でのインデックス（1始まり）
		//! @return 該当する子ノードへの参照
		//! @throw XmlException 該当する子ノードが存在しない場合
		XmlNode& at(StringView name, size_t index);
		const XmlNode& at(StringView name, size_t index) const;

		//! @brief 子ノードが存在するか確認
		//! @param name 要素名
		//! @return 存在する場合true
		bool has(StringView name) const;

		//! @brief 指定名の子ノード数を取得
		//! @param name 要素名
		//! @return 該当する子ノードの数
		size_t count(StringView name) const;

		//! @brief 子ノードを追加
		//! @param child 追加するノード
		//! @return 追加したノードへの参照
		XmlNode& add(XmlNode child);

		//! @brief 子ノードを削除
		//! @param name 要素名
		//! @return 削除に成功した場合true
		bool remove(StringView name);

		//! @brief 指定名・インデックスの子ノードを削除
		//! @param name 要素名
		//! @param index 同名要素の中でのインデックス（1始まり）
		//! @return 削除に成功した場合true
		bool remove(StringView name, size_t index);

		//! @brief 属性が存在するか確認
		//! @param key 属性名
		//! @return 存在する場合true
		bool has_attr(StringView key) const;

		//! @brief 属性値への参照を取得（存在しない場合は作成）
		//! @param key 属性名
		//! @return 属性値への参照
		String& attr(StringView key);

		//! @brief 属性値を取得（存在しない場合は空文字列）
		//! @param key 属性名
		//! @return 属性値
		String attr(StringView key) const;

		//! @brief 属性値を整数として取得
		//! @param key 属性名
		//! @param fallback デフォルト値
		//! @return 属性値を整数に変換した値、変換失敗時はfallback
		s32 attr_int(StringView key, s32 fallback) const;

		//! @brief 属性値を整数として取得（Optional版）
		//! @param key 属性名
		//! @return 属性値を整数に変換した値、存在しないか変換失敗時はnullopt
		Optional<s32> attr_int(StringView key) const;

		//! @brief 属性値を浮動小数点数として取得
		//! @param key 属性名
		//! @param fallback デフォルト値
		//! @return 属性値を浮動小数点数に変換した値、変換失敗時はfallback
		f32 attr_float(StringView key, f32 fallback) const;

		//! @brief 属性値を浮動小数点数として取得（Optional版）
		//! @param key 属性名
		//! @return 属性値を浮動小数点数に変換した値、存在しないか変換失敗時はnullopt
		Optional<f32> attr_float(StringView key) const;

		//! @brief 属性値を真偽値として取得
		//! @param key 属性名
		//! @param fallback デフォルト値
		//! @return 属性値を真偽値に変換した値、変換失敗時はfallback
		//! @details "true", "1", "yes" をtrue、"false", "0", "no" をfalseとして解釈
		bool attr_bool(StringView key, bool fallback) const;

		//! @brief 属性値を真偽値として取得（Optional版）
		//! @param key 属性名
		//! @return 属性値を真偽値に変換した値、存在しないか変換失敗時はnullopt
		Optional<bool> attr_bool(StringView key) const;

		//! @brief 属性を削除
		//! @param key 属性名
		//! @return 削除に成功した場合true
		bool remove_attr(StringView key);

		//! @brief XMLノードを整形済み文字列に変換
		//! @param indent インデント深度（デフォルト: 0）
		//! @return XML形式の文字列
		String string(s32 indent = 0) const;
	};

	//! @brief XMLドキュメント全体を表現するクラス
	//! @details
	//! XmlNodeを継承し、ファイルI/OとXPathサブセット検索機能を追加します。
	//! parse()でXMLを読み込むと、ルート要素がXml自体に展開されます。
	//!
	//! @par 使用例
	//! @code
	//! Xml xml;
	//! xml.parse("<html><body><div/></body></html>");
	//! // xml.name == "html"
	//! // xml.at("body").at("div") でアクセス
	//! // xml.find("/html/body/div") でも検索可能
	//! @endcode
	//!
	//! @par サポートする構文
	//! | 構文                    | サポート | 備考                                      |
	//! |-------------------------|----------|-------------------------------------------|
	//! | 要素 `<tag>`            | ○        |                                           |
	//! | 属性 `attr="value"`     | ○        |                                           |
	//! | 空要素 `<tag/>`         | ○        |                                           |
	//! | コメント `<!-- -->`     | ○        | #comment ノードとして保持                 |
	//! | テキストノード           | ○        | #text ノードとして保持                    |
	//! | XML宣言 `<?xml ?>`      | ○        | パース時にスキップ                        |
	//! | DOCTYPE                 | ○        | パース時にスキップ                        |
	//! | CDATA `<![CDATA[ ]]>`   | ○        | テキストとして展開                        |
	//! | 名前空間                 | △       | プレフィックス付きで保持のみ              |
	//! | エンティティ参照         | ○        | `&lt;` `&gt;` `&amp;` `&quot;` `&apos;`   |
	//! | 文字参照                | ○        | `&#123;` `&#x7B;`                         |
	//!
	//! @par XPathサブセット (find/exists)
	//! **サポートする構文:**
	//! - `/` で階層を区切る（絶対パス）
	//! - `[n]` で n 番目の同名要素を指定（1始まり）
	//! - `#comment` でコメントノードを指定
	//! - `#text` でテキストノードを指定
	//! - 名前空間プレフィックス（例: `svg:rect`, `xlink:href`）
	//!
	//! **サポートしない構文:**
	//! - 軸指定（`parent::`, `ancestor::` など）
	//! - 述語内の条件式（`[@attr='value']` など）
	//! - 関数（`text()`, `contains()` など）
	//! - `//` による子孫検索
	//! - ワイルドカード（`*`）
	class Xml : public XmlNode {
	public:
		//! @brief デフォルトコンストラクタ
		Xml();

		//! @brief ファイルからロードして生成
		//! @param path ファイルパス
		explicit Xml(StringView path);

		//! @brief ファイルから読み込み
		//! @param path ファイルパス
		//! @return 成功した場合true
		bool load(StringView path);

		//! @brief ファイルに保存
		//! @param path ファイルパス（省略時は読み込み時のパス）
		//! @return 成功した場合true
		bool save(StringView path = "");

		//! @brief 文字列からXMLをパース
		//! @param text XML文字列
		//! @return 成功した場合true
		bool parse(StringView text);

		//! @brief XPathサブセットでノードが存在するか確認
		//! @param path パス式（例: "/html/body/div[2]", "/svg:svg/svg:rect"）
		//! @return 存在する場合true
		bool exists(StringView path) const;

		//! @brief XPathサブセットでノードを検索
		//! @param path パス式
		//! @return 見つかったノードへのポインタ、見つからない場合nullptr
		XmlNode* find(StringView path);
		const XmlNode* find(StringView path) const;

	private:
		String m_path;
	};

}
