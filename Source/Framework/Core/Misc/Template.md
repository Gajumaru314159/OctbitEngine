Template
============

## 言語標準のSTLを使用する
```Template/STL.h```で対象のヘッダをインクルードしてください。  
STLのクラスオブジェクトを使用する場合は今後の実装の変更を考慮してusingで別名をつけるかラッパークラスや独自実装のクラスを作成してください。
```
namespace ob{
    template<size_t N>
    using BitSet = std::bitset<N>;
}
```
単に関数を利用する場合は```std::move(obj);```のようにそのまま使用してください。
関数はクラスオブジェクトと違い単純な名前かつ一般的な名称が多いので、ob名前空間内で宣言しなおさないよう注意していください。

## 言語標準のSTLを拡張する
```Template/```フォルダ以下で実装してください。
拡張for文への対応やコードの互換性の観点から関数の命名は言語標準に準拠してください。
[cppference](https://en.cppreference.com/w/cpp/named_req)