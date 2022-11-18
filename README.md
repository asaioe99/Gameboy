# RaspberryPi Picoでゲームボーイを作る方法

## これまでのまとめ（確定版）
1. ゲームボーイROM吸い取り器の作成
2. ROMヘッダのパーサ作成
3. bootstarapの理解
4. bootstarap実行に必要な命令のみ実装
5. SPI仕様の理解
6. PICO 1.3 LCDの仕様理解
7. PICO 1.3 LCD用デバイスドライバの作成
8. PPUのBGのみ実装（ここまでで、電源ON後のロゴスクロールが成功する）
9. デバッグ機能の実装
10. test ROMを利用して残りの命令を実装
11. DMAの実装
12. タイマー及び関連の割り込み
13. PPU及び関連の割り込み
14. キー及び関連の割り込み（←今ココ）
15. 再度各種test ROMによる動作確認
16. 最適化

まずは全体像を頭に入れるためにも以下の説明を通読してください。

## 必要スキル
本手順の過程で習得すれば良いスキルも多々ありますが、それでも事前に必要なものもあります。

- c/c++（hello worldしたことある、昔授業で習った、等では全く足りません。実装の方法にも依りますが、数千行のソースコードを書きます。また、コンパイラの先にある景色を想像できる程度の力も必要です。これは、pico上でエミュを動かすにはそれなりの最適化が必要となるからです）
- アセンブリ言語の経験（アセンブリで小規模のプログラムを作成した経験が最低でも必要。デバッグ時に必要となります。）
- CPUの仕組みを理解（関数呼び出しの際、スタックで何が起きているか説明できますか？あるいは割り込みの仕組みは分かっていますか？実用的なCPUの自作経験があれば大丈夫でしょう）
- 英語（ドキュメントはほぼ英語です）

※実際には紆余曲折あって、そもそもArduinoで実カートリッジを読み込んで動作する互換機作成を目標にしていました。そのため、Arduinoでは不足するメモリを補うためにも、SRAMを外付けする等、それなりに大変なこともしていました。結局、bootstrapの起動までは確認できたのですが、Arduinoではあまりにも非力なため、picoでの開発に急遽切り替えました。picoはArduinoに比べると大変に高性能なのですが、残念ながら開発環境の構築に難があり、それなりに面倒な部分があります。Arduinoからpicoにソースコードを移植するのにも、それなりに手間はかかりました。その代わりに、多くのことを学びました。この文書で書かれている通りにすることは、私自身が手探りで行ったことの中から、無駄な部分を排したものです。効率は良いかも知れませんが、ひょっとしたら、自己流の効率の悪い方法で挑戦した方が経験値はより多く得られるかも知れません。

## 全体の注意点
筆者自らの製作過程で得た教訓です。

- 最適化は後回しにして、明示的なコードを書く
- GBのドキュメントは必ず複数を読み込み比較する
- 型変換は慎重に行う
- PPUの実装は、BG、Window、spriteを順に追加しつつ実装するが、最後に全体をまとめて作り直すと良い
- 最低でもシリアルprintデバッグは実装する
- 命令表には必ず誤りがある
- Z-80や8080等の仕様書も参考にする
- SR7789の仕様はadafruiteのライブラリから理解する方が楽
- SPIのpin assign問題はとりあえずソフトウェア実装で乗り切る方が楽
- その他追記

## 1. ゲームボーイROM吸い取り器の作成
以下を理解し、実機を作成します。
- https://github.com/drhelius/arduinogameboy

回路の説明も読む必要があります。
- https://gbdev.gg8.se/wiki/articles/DMG_Schematics

重要なのはpin配置です。カートリッジ端子のどれが何に使われているか把握する必要があります。読み込みと書き込みについては、MBCの仕組みを理解するべきです。

色々なドキュメントがありますが、以下のものが最も分かりやすいでしょう。重要なのは、本来書き込めないはずのROMのアドレス空間に書き込みを行うことで、メモリのbankを切り替えているということです。これが分かれば何ら難しくはありません。
- http://gameboy.mongenel.com/dmg/asmmemmap.html

目標：GBカートリッジからROMデータを吸い取る

- ROM吸い取り器が手に入る
- ROMの「バンク」という仕組みを理解できる
- ArduinoIDEの使い方に慣れる

## 2. ROMヘッダのパーサ作成
手順１の内容に含まれています。ヘッダ内のデータがすべて必要になる訳ではありません。

目標：ROMデータを入力し、ヘッダ情報を出力するプログラムを作成する

- gbファイルフォーマットを理解する

## 3. bootstarapの理解
これが最も分かりやすいでしょう。
- https://www.chciken.com/tlmboy/2022/05/02/gameboy-boot.html

目標：不正なROMを弾く仕組みを理解する（GBのアーキテクチャに慣れることが大事です。デバッグ時に必要になります）

- gbの機械語に慣れる

## 4. bootstarap実行に必要な命令のみ実装
この時点で必要となる命令を列挙します（していない。加筆予定）。

基本的には、通常のエミュレータ作成と同じで、ROM及びRAMを定義し、これを1Byte毎に読み込むことで、対応する命令を実行するだけです。このレベルで何をすれば良いか分からないのであれば、以下の本を参考に一度エミュレータを書いてみましょう。

- https://amzn.asia/d/dqL8x71
- https://amzn.asia/d/ji0XX5e

上の一冊だけで十分です。読む範囲は、最初のあたりのＣ言語でエミュレータを実装する部分だけで、FPGAの部分は必要ありません（とても面白いので、時間があれば読み込むべきです）。

それはそうと、bootstrap本体は著作権の関係上ここには載せませんので、適当にどこからか探してきてください。ソースコードにバイナリで直書きするのが良いと思います。

目標：プログラムカウンタが0x100になった時の各レジスタ値をシリアル出力し、以下の通りになることを確認する

```
AF = 0x01B0
BC = 0x0013
DE = 0x00D8
HL = 0x014D
SP = 0xFFFE
```

- エミュレータの基本構造を理解する

### 補足資料
- https://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html
- https://github.com/pokemium/gb-docs-ja

## 5. SPI仕様の理解
ArduinoIDEでpico＋LCDを使用する際、「ある問題」によりSPIライブラリを利用できません。そこで、SPIをソフトウェア上で実装する必要があります。そのためには、SPIとは何か、大まかに理解する必要があります。

- https://www.analog.com/jp/analog-dialogue/articles/introduction-to-spi-interface.html

どれでも好きなものを読んで理解すれば良いでしょう。難しい部分は、pinの名称がドキュメントによって異なることです。対応表みたいなのを作ると良いですね。あとは、負論理という言葉にピンとこない方（手順１で必要なはずですが）は、一度調べてみよう。

目標：SPIの仕組みを理解する

### 補足資料
- http://www.musashinodenpa.com/arduino/ref/index.php?f=1&pos=539 （公式リファレンス）

## 6. PICO 1.3 LCDの仕様理解
以下のLCDを使います。

- https://www.waveshare.com/wiki/Pico-LCD-1.3

これをArduinoIDE上で作成したコードで制御するのが目標です。問題は、このLCDのドライバであるST7789というチップです。以下の仕様書とライブラリを読んで、LCD上にbitmapを表示させることが目標です。

- chrome-extension://efaidnbmnnnibpcajpcglclefindmkaj/https://www.rhydolabz.com/documents/33/ST7789.pdf
- https://github.com/adafruit/Adafruit-ST7735-Library

目標：LCDドライバの仕組みを理解する

## 7. PICO 1.3 LCD用デバイスドライバの作成
ここまでの理解をもとに、実際にLCD上に任意の画像を表示させるようになります。しかしながら、ある程度まとまった機能として管理しないと、ソースコードが複雑になってしまします。そのため、ST7789内部のレジスタへの値書き込み等を機能としてまとめて、簡易なデバイスドライバとAPIを作成することになります。

目標：任意のbitmapデータと座標を入力として、LCDに表示可能なデバイスドライバとAPIを作成する

## 8. PPUのBGのみ実装（ここまでで、電源ON後のロゴスクロールが成功する）
ここまでの作業では、実際にエミュレータが動作していても、シリアルコンソール等でしか内部の状態（レジスタの値等）が分かりません。そこで、bootstrapが表示する任天堂のロゴを確認するためにも、PPUのBG機能のみ実装してみます。これには、前手順で作成したAPIが不可欠です。

PPUの全体像は以下のドキュメントが参考になります。
- https://pixelbits.16-b.it/GBEDG/ppu/

目標：任天堂のロゴを確認する

## 9. デバッグ機能の実装
既にここまでの段階で実装している方もいるかも知れませんが、今後の開発に必要なデバッグ機能を実装します。今後、ゲームボーイの各機能を実装することになりますが、そこには必ず間違いがあります。なので、効率の良い開発のためにもデバッグ機能をそろそろ実装すべきです。

最低でも、ブレークポイントの設定と、レジスタの値表示、VRAM上のtile表示くらいは必要だと思います。他にも、後の工程で必要に応じて様々な機能を追加で実装することになると思います。また、pico特有の問題ですがArduinoのシリアルモニタが利用できないことがあります。その場合には、gpio25pinに割り当てられているLEDを使ってデバッグする方法や、他のラズパイを利用する方法も考えられます。前者の場合、単純なLチカやモールス信号でも良いですが、点滅の速度を利用した光通信の実装等、1bit以上の情報を含ませると効果的です。

目標：ブレークポイントの設定、レジスタの値の表示、tileマップの表示機能を実装する

## 10. test ROMを利用した命令の実装
これまでに実装した命令は、その方法にも依存しますが半分にも満たないはずです。bootstrapが使用する命令数はとても少ないのです。そこで、ここからは残りの命令を実装します。

まずは、500個の命令をすべて実装してください。なるべくひとまとめに実装する方が誤りが少なく、修正も簡単です。最適化はもっと後にするので、正確に動作することのみを考えてください。恐らく、想像以上に実装上の誤りがあり、test romの起動すら困難だと思います。また、一部の特殊な命令（割り込み関連等）については、現時点では不明の部分もあると思いますが、とりあえずクロックサイクルのみ正しく実装し、動作は未定としてください。

次に、以下のtest romにより動作を確認します。
- https://github.com/c-sp/gameboy-test-roms

ここではこの中にあるhello world test romを使用します。

目標１：hello worldを表示させる

次に、cpu_instrs.gbとinstr_timing.gbというROMの起動を目指します。これらはある種の自己診断プログラムで、正しく各機能が実装されているかを判断することができます。必要なのはBGのみであるため、現段階でも動作可能です。しかしながら、両test romも完ぺきではないため、実装に誤りが多い場合は正しい診断結果を得ることはできませんし、そもそも起動すらしない場合があります。筆者の経験では、目標１とこの目標２の間にはかなりの差があって、しかも中間的な標識はありません。つまり、hello worldやその他の軽易なtest romが起動したのにcpu_instrs.gbとinstr_timing.gbが起動しないという状態がある程度続くことを覚悟すべきです。こういう状態に陥ってしまった場合、以下の方法が役に立ちます。

- GB各ドキュメントの通読
- 信頼できるエミュレータのコード読み込み
- 信頼できるエミュレータのデバッグ機能の活用（自身のpicoからシリアルで値を取得して比較）
- test romの読み込み（ここでアセンブリ言語の経験が必要になります）

また、そのドキュメントにも誤りがあるため、以下に筆者が発見した者の一覧を載せておきます。（そのうち載せる）

目標２：cpu_instrs.gbとinstr_timing.gbの起動（パスはしなくても良い）

要注意の命令
- ```ADD HL, r16```
- ```LD HL, SP+r8```

## 11. DMAの実装
DMAは、メモリ上の大量のデータを、別のアドレスに移動させるための機能です。CPUの命令では時間が掛かるような大量のメモリコピーを、CPUとは独立したチップで実行することで高速化するものです。ゲームボーイでは、OAMにグラフィックデータ（の定義）をロードする際に用いられることが多いです。そのため、この機能を実装しないと、PPUや各種割り込み機能を実装してもspriteが表示されません。

さて、肝心のDMAの実装についてですが、これはとても簡単なので、特に具体的なテストは必要ありません。実装後は、OAM領域をダンプすることで、動作を確認してみてください。

目標：DMAを実装し、OAM領域を確認する

## 12. タイマー及び関連の割り込み（←今ココ）
ゲームボーイのタイマ機能に関連するレジスタは0xFF04~07に存在している。これらのレジスタを正しく動作させないと、PPUを実装しただけでは一部のグラフィックが正常に表示されないことがあります。

目標：未定　※タイマー用のtest romがあったらそれの動作を目標にしたい

## 13. PPU及び関連の割り込み
CPU以上に複雑なのがPPUで、今回のエミュレータ製作における最大の山場です。既にBGは実装していると思いますが、ここではWindowやspriteも実装し、加えて各種割り込みやフラグについても実装します。恐らく、過去に書いたBGのみのPPUに機能を追加しようとすると、汚いソースコードになると思います。ですので、思い切ってすべてを統合したPPUを最初から書き直すことも有力な方法だと思います。

当然ですが、PPUの動作を完璧に理解しなければ実装は不可能ですので、再度以下の文書を参考にPPUを理解してください。

- https://pixelbits.16-b.it/GBEDG/ppu/

目標：未定 ※良い感じのtest romを探す

## 14. キー及び関連の割り込み
キー入力の処理は、ゲームボーイのROM上で行われます。なので、ハード側の処理としてはとてもシンプルで、入力に応じてレジスタ0xFF00の各bitを変化すれば良いのです。

今回使用しているLCDモジュールの各ボタンが利用できるので、以下のURLをもとに実装してみましょう。

- https://www.waveshare.com/wiki/Pico-LCD-1.3

目標：キー入力テスト用のROMを動かす

## 15. 再度各種test ROMによる動作確認
cpu_instrs.gbとinstr_timing.gbの両方が通れば、市販のROMが動き始めます。しかし、それは容易ではありません。以下の点に注意することが重要です。

- 割り込みの理解と実装（これをしないとtest romが動かないことも多々あった）
- ドキュメントの誤り（命令長が異なると致命的）
- 信頼できるエミュレータとの比較（おすすめはhttps://bgb.bircd.org/）
- 複数の他のtest romでのデバッグ（https://github.com/c-sp/gameboy-test-roms）

### cpu_instrs.gbの項目別試験
以下はcpu_instrs.gbのテスト項目を分割し、個別に不具合のある命令を表示する便利なtest romです。とても便利なので使用することを推奨しますが、結果が常に正しい訳ではないので注意しましょう。

- 01-special.gb
- 02-interrupts.gb
- 03-op sp,hl.gb
- 04-op r,imm.gb
- 05-op rp.gb
- 06-ld r,r.gb
- 07-jr,jp,call,ret,rst.gb
- 08-misc instrs.gb
- 09-op r,r.gb
- 10-bit ops.gb
- 11-op a,(hl).gb 

https://github.com/retrio/gb-test-roms/blob/master/cpu_instrs/source/


## 16. 最適化
楽しくて果てのない作業です。ArduinoIDEのArduino公式ボードライブラリでは、様々な制約があるため最適化は困難です。

### LCDの最適化

### SPIの最適化

### PPUの最適化

### CPUの最適化
```switch case```でデコーダを作成すると、コンパイラにも依りますが冗長で速度に悪影響が出ます。そこで、命令の呼び出しは関数ポインタの配列により定義し、直接アドレスを参照して関数を呼び出すと多少高速になります。

```c
switch (code) {
  case 0x00:
    nop();
    break;
  case 0x01:
    stop_0():
    break;
  //以下同様
}
```

### MMUの最適化

### その他の最適化
 
- http://geo.d51498.com/nakamiya_town/ProC.html


### ドキュメント作成について
- https://monoworks.co.jp/post/2020-05-26-output-pdf-docx-html-from-markdown-with-vscode/
- https://qiita.com/pooshikin/items/b6fa4b9341b50cacddaf
- https://qiita.com/sta/items/c88093b1b9da9c77b577

