String
====================
## エンコーディング
OSごとにネイティブ文字列の型が異なります。
| OS | 型 | エンコーディング |
|-|-|-|
| Windows | wchar_t | UTF-16 |
| MacOS<br>iOS | UniChar | UTF-16 |
| Linux<br>Android | char | UTF-8 |
| Unix | char | UTF-8 または ISO-8859-1 |
| FreeBSD<br>OpenBSD<br>NetBSD | char | UTF-8 または ISO-8859-1 |