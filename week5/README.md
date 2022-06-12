# STEP2022 week5 homework
## ファイルについて
- `data/` 入力データと出力データ
- `common.py` `output_vertifier.py` に必要
- `four.cpp` 貪欲法の後、2-optで改良
  - `greedy.cpp` に1つ関数を加えただけなのでこちらだけ見ていただければ大丈夫です。
- `greedy.cpp` 貪欲法
- `output_vertifier.py` 出力されたファイルを読み込んで経路の長さを表示

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
