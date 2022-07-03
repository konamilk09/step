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

## 参考にしたもの
- [Anna Satoさん](https://github.com/llannasatoll/step2022/tree/main/week5)
- [week5 homework review](https://docs.google.com/presentation/d/1CkgQ8HfPm6YZouknVQ38zu_y5Xz9xW6RffBfZI6ekqI/edit?resourcekey=0-dYmET4F4PzKbWTp7tIfNng#slide=id.g897153bb92_0_535)

## 実行方法
```
g++ two_opt.cpp
./a.out [0-6]
```
`[0-6]` は入力の番号を指定する。
0から6の入力ファイルに対してすべての出力ファイルが存在している状態で、以下のように実行すると経路の長さが表示される。
```
python3 output_vertifier.py
```
