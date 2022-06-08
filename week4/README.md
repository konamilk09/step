# STEP2022 week4

## homework1
> 必須：”Google”から”渋谷”をたどる経路を、DFSとBFSで探す

### 実行方法
```
g++ path.cpp && ./a.out
```
- テスト環境
  - WSL2
  - Ubuntu 20.04
  - gcc version 9.4.0

- 実行結果(例)
```
Use small data?(y/n)> n
start> Google
goal> キットカット
DFS or BFS?(d/b)> b
Searching...
Path has written in result.txt
```

### プログラムの内容
1. データを読み込む
    - `pages`は`pages[番号]=タイトル`
    - `pages_r`は`pages[タイトル]=番号`
        - 入力した文字列を検索するときに全探索せずにその番号にアクセスできる。
    - `links`は`links[番号]={繋がっているタイトルの番号のset}`
2. DFSまたはBFS
   - DFSは`container`(これから見るもの)がstack
   - BFSは`container`(これから見るもの)がqueue
   - 後で経路を見つけられるように、`container`に追加するときに、その親を記録する
   - 親を辿って経路を記録する
3. 経路をファイルに書き出す
   - `result.txt` に始点、終点、始点から終点までの経路、が出力される


## homework2
> その他なんでも！

「ポケモン」という単語が含まれるタイトルのうち、孤立点を探した。
結果：`pokeiso.txt`意外と多くて面白くなかった。

### 実行方法
```
g++ poke.cpp && ./a.out
```
### プログラムの内容
1. データを読み込む
    - 「ポケモン」という単語が含まれるもののみ`pages`に読み込む
    - 始点または行先が`pages`に含まれるもののみ`links`に読み込む
2. 孤立点を探す
    - `pages`を`isolated`に`std::move`
    - `isolated`に含まれるもののうち、`links`に含まれているものを削除
3. 孤立点をファイルに書き出す
    - `pokeiso.txt`に結果が出力される

## 質問等
- 実行時間を減らせたらと思い、以下のようなことをしましたが、`read_pages`等のファイル読み込み関数は読み込みだけをした方がいいでしょうか。
  - `path.cpp`の方では`pages`と`pages_r`の二つを作り、id と title 両方からアクセスできるようにしました
  - `poke.cpp`の方ではファイル読み込み時に必要な情報だけを読み込みました
- `poke.cpp`の75行目で`std::move`の使い方は合っていますか
  - 今回はそれ以降`pages`を使わないため、`std::move`できますが、もっと大きなプログラムを考えると、やはり`pages`の情報は残した方がいいでしょうか

- 前回`std::variant<double, operator_type>`をclassにするアイデアをいただいて、なるほど！と思いました。
