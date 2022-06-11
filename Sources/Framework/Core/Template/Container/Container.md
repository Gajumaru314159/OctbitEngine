コンテナ
============
コンテナには要素が追加時の順序を保つ**シケーンスコンテナ**(Array/List/Deque)と、追加時の順序を保たない**連想コンテナ**(Set/Map)があります。

コンテナに求められている機能は[cppference](https://en.cppreference.com/w/cpp/named_req)を確認してください。(Container/ReversibleContainerなど)

-------------------------------------

## Array (固定長配列)
* 要素数がコンパイル時に決定します。
* 各要素はメモリ上に連続して格納されます。
* ネイティブ配列と比べてサイズの取得などの標準コンテナの機能が使用できます。
* [Container](https://en.cppreference.com/w/cpp/named_req/Container)と[ReversibleContainer](https://en.cppreference.com/w/cpp/named_req/ReversibleContainer)の全ての要件をすべて満たします。

## Deque (二重終端キュー)
* 要素は線形シーケンスに従って並べられます。
* 各要素にインデックスでアクセスできます。
* 先頭への効率的な挿入・削除ができます。
* ポインタ演算を介しての安全なアクセスができません。

## ForwardList (短方向リスト)
## List (双方向リスト)
## Map (連想配列)
## HashMap (非順序連想配列)
## Set (集合)
## HashSet (非順序集合)


コンテナアダプタ
===============
コンテナを内部的に使って実装されるクラスです。
## Stack(LIFOコンテナ)
## Queue(FIFOコンテナ)
## PriorityQueue(優先度付きキュー)

## FreeList
## LockFreeList

その他のコンテナ
===============
## BitSet
## BitFlags
## HandleList
## HandleForwardList