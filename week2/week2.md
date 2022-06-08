# STEP2022 week2

## homework1
> 行列積を求めるプログラムを書いて、行列のサイズNと実行時間の関係を調べてみよう
  
ほぼharakenさんのコードをコピペしました。
試行回数10回の平均実行時間は以下の通りです。
|N      |10    |100   |1000  |
|-------|------|------|------|
|実行時間(秒)|0.000013|0.009773|13.17358|

計算量は全て O(N^3) だが、実行時間は N^3 に比例しています。

## homework2
> 木構造を使えばO(log N)、ハッシュテーブルを使えばほぼO(1)で検索・追加・削除を実現することができて、これだけ見ればハッシュテーブルのほうがはるかに優れているように見える。ところが、現実の大規模なデータベースでは、ハッシュテーブルではなく木構造が使われることが多い。その理由を考えよ。

- 大規模なデータベースではハッシュテーブルでは衝突がたくさん発生し、多段ハッシュテーブルにするか、再ハッシュすることになり、構造が複雑になる。
- 上手くハッシュが衝突しないようにするハッシュ関数を作るのは難しく、衝突がたくさん発生する一方、空の枠もあるため、メモリを有効に活用できない。

## homework3
> 問題：次の操作をほぼO(1)で実現するデータ構造を考えよう
> - 与えられた<URL, Webページ>があるかないかを検索する
> - もしない場合、キャッシュ内で一番古い<URL, Webページ>を捨てて、かわりに与えられた<URL, Webページ>を追加する

ハッシュテーブルに保存された<URL, Webページ>の組が、新しいものから古いもの順に双方向リストで繋げる。リストの最初（最新）と最後（最古）を持っておく。検索はハッシュテーブルをみる、一番古いものを捨てるのは tail を移動させてハッシュテーブルから消す、追加はリストの先頭に追加してハッシュテーブルに追加する、とほぼ O(1) でキャッシュの操作を実現できると考えた。

## homework4
> 宿題3のキャッシュを実装してみよう

少し問題と違いますが、サイズが26のハッシュテーブルで動くプログラムを作ることができました。ハッシュ値が衝突した場合は上手く動かず、もともとある値が残ります。

ハッシュテーブルと、新しいものから古いものの順に並んでいるリストを作りました。
新しい組を追加するときにやることは、
➀ハッシュテーブルに追加する
➁双方向連結リストに追加する
➂追加した組をリストの先頭に変更する
➃リストの一番古いものをリストとハッシュテーブル両方から削除する

イメージ図です。

参考文献：https://www.programiz.com/dsa/hash-table

以下は実行例です。次のように`<url,web>`を与えると、その下のようにデータ構造に入ります。
```
    HashTable h(4);
    h.insert("a.com", "AAA");
    h.insert("b.com", "BBB");
    h.insert("c.com", "CCC");
    h.displayHash();
    
    h.insert("a.com", "AAA");
    h.displayHash();

    h.insert("e.com", "EEE");
    h.displayHash();

    h.insert("s.com", "SSS");
    h.displayHash();
    
    h.insert("s.com", "SSS");
    h.displayHash();

    h.insert("a.com", "AAA");
    h.displayHash();
```

```
HashTable:
table[0]a.com
table[1]b.com
table[2]c.com
table[3]
table[4]
table[5]
table[6]
table[7]
table[8]
table[9]
table[10]
table[11]
table[12]
table[13]
table[14]
table[15]
table[16]
table[17]
table[18]
table[19]
table[20]
table[21]
table[22]
table[23]
table[24]
table[25]
head -> tail:
[c.com, b.com, a.com]
tail -> head:
[a.com, b.com, c.com]

HashTable:
table[0]a.com
table[1]b.com
table[2]c.com
table[3]
(・・・略・・・)
head -> tail:
[a.com, c.com, b.com]
tail -> head:
[b.com, c.com, a.com]

HashTable:
table[0]a.com
table[1]b.com
table[2]c.com
table[3]
table[4]e.com
table[5]
(・・・略・・・)
head -> tail:
[e.com, a.com, c.com, b.com]
tail -> head:
[b.com, c.com, a.com, e.com]

HashTable:
table[0]a.com
table[1]
table[2]c.com
table[3]
table[4]e.com
table[5]
(・・・略・・・)
table[17]
table[18]s.com
table[19]
(・・・略・・・)
head -> tail:
[s.com, e.com, a.com, c.com]
tail -> head:
[c.com, a.com, e.com, s.com]

HashTable:
table[0]a.com
table[1]
table[2]c.com
table[3]
table[4]e.com
table[5]
(・・・略・・・)
table[17]
table[18]s.com
table[19]
(・・・略・・・)
head -> tail:
[s.com, e.com, a.com, c.com]
tail -> head:
[c.com, a.com, e.com, s.com]

HashTable:
table[0]a.com
table[1]
table[2]c.com
table[3]
table[4]e.com
table[5]
(・・・略・・・)
table[17]
table[18]s.com
table[19]
(・・・略・・・)
head -> tail:
[a.com, s.com, e.com, c.com]
tail -> head:
[c.com, e.com, s.com, a.com]
```
