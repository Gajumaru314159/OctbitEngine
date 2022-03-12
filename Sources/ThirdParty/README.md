# 外部ライブラリについて
## フォルダ構成
ライブラリ名が`Hoge`だとすると
```
ThirdParty/
+-- Hoge
    +-- include
        +-- *.h
        +-- *.hpp
        +-- *.inl
    +-- lib
        +-- Debug
            +-- Hoge.lib
        +-- Release
            +-- Hoge.lib
    +-- misc
        +-- Hoge.natvis
    +-- LICENCE.txt
    +-- ThirdPartyLicences.txt
```
を基本構成とします。

## 読み込み方法
プロジェクトは[CMake](https://cmake.org/)によって管理されています。

ライブラリごとの`CMakeLists.txt`で、依存しているサードパーティ・ライブラリが定義されています。ThirdPartyディレクトリは、変数`OCTBIT_THIRD_PARTY_PATH`からアクセスできます。プロジェクト構成でライブラリを切り替える必要があれば、変数`OCTBIT_THIRD_PARTY_PATH`を用いて切り替えてください。
```CMake
# 追加のインクルードディレクトリ
set(OB_PROJECT_INCLUDE_DIR 
    "${OCTBIT_THIRD_PARTY_PATH}/Hoge/include"
)
target_include_directories(${PROJECT_NAME} PUBLIC ${OB_PROJECT_INCLUDE_DIR})

# 追加の依存ライブラリ
set(OB_PROJECT_INCLUDE_LIB
    "${OCTBIT_ROOT_PATH}/Hoge/lib/${OB_CONFIGURATION}/Hoge.lib"
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${OB_PROJECT_INCLUDE_LIB})
```