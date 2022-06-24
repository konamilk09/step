# Real malloc challenge!

## My description
`dinamic/malloc.c`についてです。

### やろうとしたこと
やろうとしていたことは、ルールから外れてしまっているかもしれないです。
#### 動機
小さいサイズのデータをばかりをmallocするとき、`my_metadata_t`のサイズの割合が多くなってしまって使用効率が良くならなかったので、`my_metadata_t`のサイズを減らせないか考えました。
#### 説明
`my_metadata_t`はfree slotまたはobjectの大きさと、次のfree slotへのポインタから成り立っています。objectととしてメモリを使用している時は次のfree slotへのポインタは必要ないので、objectとして使用している時はmetadataはobjectのサイズだけ保持するようにしようとしました。

<img width="431" alt="image" src="https://user-images.githubusercontent.com/83947560/175550693-29a5e7bb-f3de-4b82-9436-bd53161a9abe.png">


#### 起きるエラー
```
malloc_challenge.bin: main.c:233: run_challenge: Assertion `0' failed.
make: *** [Makefile:16: run] Aborted
```
と出てきます。


## Instruction

Your task is implement a better malloc logic in [malloc.c](malloc.c) to improve the speed and memory usage.

## How to build & run a benchmark

```
# build
make
# run a benchmark
make run
```

If the commands above don't work, you can build and run the challenge directly by running:

```
gcc -Wall -O3 -lm -o malloc_challenge.bin main.c malloc.c simple_malloc.c
./malloc_challenge.bin
```

## Acknowledgement

This work is based on [xharaken's malloc_challenge.c](https://github.com/xharaken/step2/blob/master/malloc_challenge.c). Thank you haraken-san!
