String
====================

StringクラスはChar型の可変長配列として定義されています。
内部表現はUTF-8を使用していますが、将来的にエンコーディング方法を変更できるように文字列リテラルを記述する場合は```TC()```マクロを使用して記述します。
UTF-8環境下では```TC("Text")```は```u8"Text"```へ変換されます。

文字列を扱う場合はString型を使用することを推奨します。ただし外部ライブラリと連携できるように以下の文字列型も定義してあります。

| クラス名 | 文字型 | エンコーディング |
|-|-|-|
|WString|wchar_t|プラットフォーム依存|
|U8String|char|UTF-8|
|U16String|char16_t|UTF-16|
|U32String|char32_t|UTF-32|
|NativeString|下記表参照|下記表参照|

NativeStringはプラットフォームごとに異なります。

| OS | 型 | エンコーディング |
|-|-|-|
| Windows | wchar_t | UTF-16 |
| MacOS<br>iOS | UniChar | UTF-16 |
| Linux<br>Android | char | UTF-8 |
| Unix | char | UTF-8 または ISO-8859-1 |
| FreeBSD<br>OpenBSD<br>NetBSD | char | UTF-8 または ISO-8859-1 |

各文字列はStringEncoderを使用することで相互変換が可能です。
```c++
String string = TC("Sample Text");
WString wstring;
StringEncoder::Encode(string,wstring);
```
## 文字リテラルのエンコーディング
charのエンコーディングはUTF-8を使用します。

## フォーマット
文字列をフォーマットする場合は```Format()```を使用します。内部的にはfmtlibを使用しています。
```c++
auto text = Format(TC("{}+{}={}",1,2,3);
OB_ASSERT(text,TC("1+2=3"));
```
Vec3やColorなど基本的な型はformatterに対応しています。
```c++
auto text = Format(TC("{}"),Color::Red);
```
書式文字列の詳細は[std::format](https://cpprefjp.github.io/reference/format/format.html)を参考にしてください。
### formatter対応
独自追加した型をFormatに対応する場合はfmt::formatterを直接特殊化してください。以下はRange型のformatterの例です。
```cpp
template <> struct fmt::formatter<ob::core::Range, ob::core::Char> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
		return ctx.end();
	}

	template<typename FormatContext>
	auto format(ob::core::Range value, FormatContext& ctx) -> decltype(ctx.out()) {
		return format_to(ctx.out(), TC("({},{})"), value.min,value.max);
	}
};
```