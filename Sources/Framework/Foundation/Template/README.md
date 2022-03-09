Template
============

### 言語標準のSTLを使用する
```Template/STL/```フォルダ以下でusing宣言をしてください。
このフォルダの中には独自実装のコードを書いてはいけません。
仮に```std::sort```のように直接STLを利用すると、パフォーマンスなどの理由から実装を変更する際にコードの修正が全体に及んでしまうためです。そのため言語標準のSTLは直接利用せずusingします。
```c++
namespace ob{
    using std::sort;
}
```

### 言語標準のSTLを拡張する
```Template/```フォルダ以下で実装してください。
拡張for文への対応やコードの互換性の観点から関数の命名は言語標準に準拠してください。
```c++
namespace ob{
    template<typename T,s32 N>
    class fixed_vector{
    public:
        size_t size()const noexcept{
            return N;
        }
    private:
        T m_data[N];
    }
}
```

## 例外
### std::unique_ptr / std::make_unique
Visual Studioのインテリセンスが効かないため例外として直接使用することを許容します。```std::shared_ptr``` ```std::make_shared```も同様です。