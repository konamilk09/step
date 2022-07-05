# TSP challenge
## ファイルについて
- `ga.cpp` :これだけ見ていただければ大丈夫です。
  - 遺伝子アルゴリズム
- `tsp.cpp` :前回のTSP Challengeと共通で使える関数、クラス
  - 点のクラス、時刻を返す、ファイルの読み込み
- `data/` :入力データと出力データ
- `common.py` :`output_vertifier.py` に必要
- `output_vertifier.py` 出力されたファイルを読み込んで経路の長さを表示

## アルゴリズムについて
### 概要
- 貪欲法とtwo_optで最初の世代を作成する
- 選択、交差、two_optを行う
- 1世代50個体、2000世代繰り返す
- 最後の世代でもっともよいスコアの経路を返す
### 詳細
#### 初期化
- スタート地点を変えて貪欲法、それぞれtwo_optを行う
- 50個体作成する
- 経路(`Tour`)の配列に入れる
#### 1回の世代交代
- 選択
  - 1世代の中で上位30％は複製し、2倍の個体数にする
  - 残りの枠は上位30％以外の個体をランダムに選択する
- 交差
  - 親二人(`parent1`と`parent2`)を世代からランダムに選択する
  - ランダムに開始点(`begin`)と終了点(`end`)を取得する
  - 開始点から終了点までは`parent1`を引き継ぐ
  - `parent2`を前から見ていき、まだ通っていない点を子供の経路に追加する
- two_opt で改良
- 以上を50個体の子供分行う
- 親が重複する可能性はある
### 実行時間
- init_dist: $N^2$
- greedy: 
  - init_set: $NlogN$
  - min_city: $N^2$
- two_opt: $N^2$
- init_generation: 
  $$
  50*greedy*twoopt 
  = NlogN * N^2 * 10 
  $$
- next_generation: 50 *
  - selection: $50log50 + 50$
  - cross_over: $N^2$
  - two_opt: $N^2$
- best_score: $50*N$
- genetic_algorithm: 
  $$
  2000 * nextgeneration
  = 10^5 * N*N * 50log50 
  $$
- write_tour: $N$
- reat_input: $N$

より、全体の計算量は $10^5 * N^2 * 50log50=10^7*N^2$ <br>
N=2048、2秒で$10^8$を処理できるとすると、$10^{14} = 10^6 * 2[s] = 約555[h]$ となってしまう。
challenge5、challenge6 と challenge7 については は20世代行った。

## 参考にしたもの
- [Anna Satoさん](https://github.com/llannasatoll/step2022/tree/main/week5)
- [week5 homework review](https://docs.google.com/presentation/d/1CkgQ8HfPm6YZouknVQ38zu_y5Xz9xW6RffBfZI6ekqI/edit?resourcekey=0-dYmET4F4PzKbWTp7tIfNng#slide=id.g897153bb92_0_535)

## 実行方法
```
g++ two_opt.cpp
./a.out [0-7]
```
`[0-7]` は入力の番号を指定する。
0から6の入力ファイルに対してすべての出力ファイルが存在している状態で、以下のように実行すると経路の長さが表示される。
```
python3 output_vertifier.py
```

## 結果
|generations|challenge0|challenge1|challenge2|challenge3|challenge4|challenge5|challenge6|challenge7|
|-|-|-|-|-|-|-|-|-|
|2000|3291.62| 3832.29| 4931.44| 8258.64| 10953.18|
|200|3418.10|  3832.29|  4494.42|  8350.17| 10919.05|21304.74|
|20|3418.10|  3778.72|  4931.44|  8147.03| 10718.20| 21130.78|42412.74|84835.23
|two-opt|3418.10|3832.29|4994.89|8970.05|11489.79|21363.6|42712.37|84190.72| 

世代数を増やしてもchallengeによって経路が改良されたりされなかったりした。親から子を作るときに、優れた親が必ずしも子に遺伝されないためである。

## 今後できそうなこと
- 領域を分割し、各領域で並列に最適経路を求めたあと、経路をつなぐことで高速化する
- two-optでどのくらい経路が更新されているかを見る
  - 少なかったら全探索ではなく別の方法を考える
- 遺伝子アルゴリズムの世代数や1世代の個体数を変えてみる
  
