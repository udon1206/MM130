#!/bin/bash

# コンパイル
g++ -std=c++11 -O2 -Wall -Wextra GraphLabeling.cpp
#[st, en] のseed のファイルを処理する．procsはプロセス数
st=1
en=100
procs=8
# インタラクティブ処理の関数
f(){
  java -jar tester.jar -exec "./a.out" -seed $1 >> score.txt
}

# xargs で関数使うための処理
export -f f

#以前の処理を削除
rm -f score.txt

# option

usage(){
  cat <<EOM
使い方：
  -s : 開始 seed
  -e : 終了 seed
  -P : プロセス数
EOM

  exit 2
}

while getopts "s:e:P:" optKey; do
  case "$optKey" in
    s)
      st=${OPTARG}
      ;;
    e)
      en=${OPTARG}
      ;;
    P)
      procs=${OPTARG}
      ;;
  esac
done

# 並列処理
seq -f '%04g' $st $en | xargs -t -n1 -P$procs -I{} bash -c "f {}"
# score.txt に書き込まれたスコアの計算
python3 evaluate.py