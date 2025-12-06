コンテナ {#Container}
============
コンテナには要素が追加時の順序を保つ**シケーンスコンテナ**(Vector/List/Deque)と、追加時の順序を保たない**連想コンテナ**(Set/Map)があります。

コンテナに求められている機能は[cppference](https://en.cppreference.com/w/cpp/named_req)を確認してください。(Container/ReversibleContainerなど)

-------------------------------------
## 基本コンテナ

### Array (固定長配列)
* [std::array](https://ja.cppreference.com/w/cpp/container/array)のラッパーです
* 要素数がコンパイル時に決定します
* 各要素はメモリ上に連続して格納されます
* ネイティブ配列と比べてサイズの取得などの標準コンテナの機能が使用できます
* [Container](https://ja.cppreference.com/w/cpp/named_req/Container)と[ReversibleContainer](https://ja.cppreference.com/w/cpp/named_req/ReversibleContainer)の全ての要件をすべて満たします

### Vector
* [std::vector](https://ja.cppreference.com/w/cpp/container/vector)のラッパーです
* 要素数は必要に応じて拡縮されます
* 各要素はメモリ上に連続して格納されます

### FixedVector
* 最大要素数がコンパイル時に決定します
* 各要素はメモリ上に連続して格納されます

### Deque (二重終端キュー)
* [std::deque](https://ja.cppreference.com/w/cpp/container/deque)のラッパーです
* 要素は線形シーケンスに従って並べられます。
* 各要素にインデックスでアクセスできます。
* 先頭への効率的な挿入・削除ができます。
* ポインタ演算を介しての安全なアクセスができません。

### ForwardList (単方向リスト)
* [std::forward_list](https://ja.cppreference.com/w/cpp/container/forward_list)のラッパーです
* 単方向リンクリストとして実装されています
* メモリ効率が高く、挿入・削除が高速です
* ランダムアクセスはサポートされていません

### List (双方向リスト)
* [std::list](https://ja.cppreference.com/w/cpp/container/list)のラッパーです
* 双方向リンクリストとして実装されています
* 任意の位置での挿入・削除が高速です
* ランダムアクセスはサポートされていません

### Map (連想配列)
* [std::map](https://ja.cppreference.com/w/cpp/container/map)のラッパーです
* キーと値のペアを保持する連想コンテナです
* キーは自動的にソートされます
* 検索、挿入、削除が対数時間で行われます

### HashMap (非順序連想配列)
* [std::unordered_map](https://ja.cppreference.com/w/cpp/container/unordered_map)のラッパーです
* キーと値のペアを保持する非順序連想コンテナです
* ハッシュテーブルを使用して実装されています
* 検索、挿入、削除が平均定数時間で行われます

### Set (集合)
* [std::set](https://ja.cppreference.com/w/cpp/container/set)のラッパーです
* 一意の要素を保持する集合コンテナです
* 要素は自動的にソートされます
* 検索、挿入、削除が対数時間で行われます

### HashSet (非順序集合)
* [std::unordered_set](https://ja.cppreference.com/w/cpp/container/unordered_set)のラッパーです
* 一意の要素を保持する非順序集合コンテナです
* ハッシュテーブルを使用して実装されています
* 検索、挿入、削除が平均定数時間で行われます

コンテナアダプタ
===============
コンテナを内部的に使って実装されるクラスです。
## Stack (LIFOコンテナ)
* [std::stack](https://ja.cppreference.com/w/cpp/container/stack)のラッパーです
* LIFO (Last In, First Out) の動作を提供します
* 内部的には任意のシーケンスコンテナを使用して実装されます (デフォルトは`std::deque`)

## Queue (FIFOコンテナ)
* [std::queue](https://ja.cppreference.com/w/cpp/container/queue)のラッパーです
* FIFO (First In, First Out) の動作を提供します
* 内部的には任意のシーケンスコンテナを使用して実装されます (デフォルトは`std::deque`)

## PriorityQueue (優先度付きキュー)
* [std::priority_queue](https://ja.cppreference.com/w/cpp/container/priority_queue)のラッパーです
* 優先度の高い要素が先に処理されます
* 内部的にはヒープを使用して実装されます

その他のコンテナ
===============
## BitSet
* [std::bitset](https://ja.cppreference.com/w/cpp/utility/bitset)のラッパーです
* 固定サイズのビット集合を管理します
* ビット単位の操作が可能です

## BitFlags
* enumのビットフラグを管理するためのクラスです
* フラグの設定、解除、チェックが簡単に行えます

## HandleList
* ハンドルを使用して要素を管理するリストです
* ハンドルが解放されるとコンテナからも要素が削除されます

## HandleForwardList
* ハンドルを使用して要素を管理する単方向リストです
* ハンドルが解放されるとコンテナからも要素が削除されます