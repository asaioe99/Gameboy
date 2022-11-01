# 参考資料一覧

## これまでのまとめ（確定版）
1. ゲームボーイROM吸い取り器の作成
2. ROMヘッダのパーサ作成
3. bootstarapの理解
4. bootstarap実行に必要な命令のみ実装
5. SPI仕様の理解
6. PICO 1.3 LCDの仕様理解
7. PICO 1.3 LCD用デバイスドライバの作成
8. PPUのBGのみ実装（ここまでで、電源ON後のロゴスクロールが成功する）
9. test ROMを利用して残りの命令を実装（←今ココ）

まずは全体像を頭に入れるためにも以下の説明を通読してください。

## 必要スキル
本手順の過程で習得すれば良いスキルも多々ありますが、それでも事前に必要なものもあります。

- c/c++（hello worldしたことある、とか、昔授業で習った、とかでは全く足りない。コンパイラの先にある景色を想像できる程度の力が必要）
- アセンブリ言語の経験（アセンブリで小規模のプログラムを作成した経験が最低でも必要）
- CPUの仕組み（関数呼び出しの際、スタックで何が起きているか説明できますか？CPU自作の経験があれば心強い）
- 英語（ドキュメントはほぼ英語です）

※実際には紆余曲折あって、そもそもArduinoで実カートリッジを読み込んで動作する互換機作成を目標にしていました。そのため、Arduinoでは不足するメモリを補うためにも、SRAMを外付けする等、それなりに大変なこともしていました。結局、bootstrapの起動までは確認できたのですが、Arduinoではあまりにも非力なため、picoでの開発に急遽切り替えました。picoはArduinoに比べると大変に高性能なのですが、残念ながら開発環境の構築に難があり、それなりに面倒な部分があります。Arduinoからpicoにソースコードを移植するのにも、それなりに手間はかかりました。その代わりに、多くのことを学びました。この文書で書かれている通りにすることは、私自身が手探りで行ったことの中から、無駄を排したものです。効率は良いかも知れませんが、ひょっとしたら、自己流の効率の悪い方法で挑戦した方が経験値はより多く得られるかも知れません。

## 1. ゲームボーイROM吸い取り器の作成
以下を理解し、実機を作成する。
- https://github.com/drhelius/arduinogameboy

回路の説明も読む必要がある。
- https://gbdev.gg8.se/wiki/articles/DMG_Schematics

重要なのはpin配置である。カートリッジ端子のどれが何に使われているか把握する必要がある。読み込みと書き込みについては、MBCの仕組みを理解するべし。

色々なドキュメントがあるが、いかのものが最も分かりやすい。重要なのは、本来書き込めないはずのROMのアドレス空間に書き込みを行うことで、メモリのbankを切り替えているということ。これが分かれば何ら難しくはない。
- http://gameboy.mongenel.com/dmg/asmmemmap.html

目標：GBカートリッジからROMデータを吸い取る

## 2. ROMヘッダのパーサ作成
手順１の内容に含まれている。ヘッダ内のデータがすべて必要になる訳ではない。

目標：ROMデータを入力し、ヘッダ情報を出力するプログラムを作成する

## 3. bootstarapの理解
これが最も分かりやすい。
- https://www.chciken.com/tlmboy/2022/05/02/gameboy-boot.html

目標：不正なROMを弾く仕組みを理解する（GBのアーキテクチャに慣れることが大事です。デバッグ時に必要になります）

## 4. bootstarap実行に必要な命令のみ実装
この時点で必要となる命令を列挙する（していない。加筆予定）。

基本的には、通常のエミュレータ作成と同じで、ROM及びRAMを定義し、これを1Byte毎に読み込むことで、対応する命令を実行するだけである。このレベルで何をすれば良いか分からないのであれば、以下の本を参考にエミュレータを書いてみよう。

- https://amzn.asia/d/dqL8x71
- https://amzn.asia/d/ji0XX5e

上の一冊だけで十分です。読む範囲は、最初のあたりのＣ言語でエミュレータを実装する部分だけで、FPGAの部分は必要ありません（とても面白いので、時間があれば読み込むべきです）。

それはそうと、bootstrap本体は著作権の関係上ここには載せませんので、適当にどこからか探してきてください。ソースコードにバイナリで直書きするのが良いと思います。

目標：エミュレータの作り方を学ぶ

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

目標：任天堂のロゴを確認する

## 9. test ROMを利用して残りの命令を実装（←今ココ）
これまでに実装した命令は、その方法にも依存しますが半分にも満たないはずです。bootstrapが使用する命令数はとても少ないのです。そこで、ここからは残りの命令を実装します。

まずは、500個の命令をすべて実装してください。なるべくひとまとめに実装する方が誤りが少なく、修正も簡単です。最適化はもっと後にするので、正確に動作することのみを考えてください。

次に、test romにより動作を確認します。
- https://github.com/retrio/gb-test-roms

cpu_instrs.gbとinstr_timing.gbの両方が通れば、市販のROMが動き始めます。

目標：test romのクリア

## 以下は個人的メモ

### ドキュメント作成について
- https://monoworks.co.jp/post/2020-05-26-output-pdf-docx-html-from-markdown-with-vscode/
- https://qiita.com/pooshikin/items/b6fa4b9341b50cacddaf
- https://qiita.com/sta/items/c88093b1b9da9c77b577


### 追加の関数等（動作未検証）
```c
  case 0x76:
    halt();
    break;
  case 0x07:
    rlca();
    break;
  case 0x27:
    daa();
    break;
  case 0x37:
    scf();
    break;
  case 0x08:
    ld_pd16_sp();
    break;
  case 0xE8:
    add_sp_d8();
    break;
  case 0xE9:
    jp_hl();
    break;
  case 0xF9:
    ld_sp_hl();
    break;
  case 0x0F:
    rrca();
    break;
  case 0xF8:
    ld_hl_sp_d8();
    break;

void halt() {
  cc += 4;
  cc_dec = 4;
  pc++;
}

void rlca() {
  if (AR & 0b10000000 > 0) {
    FR = 0b00010000;
  } else {
    FR = 0b00000000;
  }
  AR = (AR << 1) | (AR >> 7);

  cc += 4;
  cc_dec = 4;
  pc++;
}
void daa() {
  if (!(FR & 0b01000000)) {
    if ((FR & 0b00010000) || AR > 0x99) {
      AR += 0x60;
      FR |= 0b00010000;
    }
    if ((FR & 0b00100000) || ((AR & 0x0F) > 0x09)) AR -= 0x06;
  } else {
    if (FR & 0b00010000) AR -= 0x60;
    if (FR & 0b00100000) AR -= 0x06;
  }
  if (AR == 0) {
    FR |= 0b10000000;
  } else {
    FR &= 0b01110000;
  }
  FR &= 0b11010000;
  cc += 4;
  cc_dec = 4;
  pc++;
} 

void scf() {
  FR &= 0b10000000;
  FR |= 0b00010000;
  cc += 4;
  cc_dec = 4;
  pc++;
}

void ld_pd16_sp() {
  put_byte(((uint16_t)get_byte(pc + 2) << 8) + get_byte(pc + 1), (uint8_t)(sp & 0x00FF));
  put_byte(((uint16_t)get_byte(pc + 2) << 8) + get_byte(pc + 1) + 1, (uint8_t)(sp >> 8));
  cc += 20;
  cc_dec = 20;
  pc += 3;
}

void add_sp_d8() {
  uint8_t val = get_byte(++pc);

  if (sp & 0x0F + val & 0x0F > 0x0F) {
    FR = 0b00100000;
  } else {
    FR = 0b00000000;
  }
  if (sp & 0xFF + (uint16_t)val & 0xFF > 0xFF) FR |= 0b00010000;

  sp += val;
  cc += 16;
  cc_dec = 16;
  pc++;
}

void jp_hl() {
  pc = HL(HR, LR);
  cc += 4;
  cc_dec = 4;
}

void ld_sp_hl() {
  sp = HL(HR, LR);
  cc += 8;
  cc_dec = 8;
  pc++;
}

void rrca() {
  if (AR & 0b00000001 > 0) {
    FR = 0b00010000;
  } else {
    FR = 0b00000000;
  }
  AR = (AR >> 1) | (AR << 7);

  cc += 4;
  cc_dec = 4;
  pc++;
}

void ld_hl_sp_d8() {
  uint16_t val = get_byte(++pc);

  if (sp & 0x0F + val & 0x0F > 0x0F) {
    FR = 0b00100000;
  } else {
    FR = 0b00000000;
  }
  if (sp & 0xFF + (uint16_t)val & 0xFF > 0xFF) FR |= 0b00010000;

  val += sp;
  HR = (uint8_t)(val >> 8);
  LR = (uint8_t)(val & 0x00FF);
  cc += 12;
  cc_dec = 12;
  pc++;
}

  case 0x08:
  case 0x09:
  case 0x0A:
  case 0x0B:
  case 0x0C:
  case 0x0D:
  case 0x0F:
    rrc_r8();
    break;
  case 0xC6:
  case 0xD6:
  case 0xE6:
  case 0xF6:
  case 0xCE:
  case 0xDE:
  case 0xEE:
  case 0xFE:
    set_phl();
    break;
  case 0x86:
  case 0x96:
  case 0xA6:
  case 0xB6:
  case 0x8E:
  case 0x9E:
  case 0xAE:
  case 0xBE:
    res_phl();
    break;
  case 0x46:
  case 0x56:
  case 0x66:
  case 0x76:
  case 0x4E:
  case 0x5E:
  case 0x6E:
  case 0x7E:
    bit_phl();
    break;

void rrc_r8() {
  uint8_t val;
  switch (code) {
    case 0x08:
      val = BR ;
      break;
    case 0x09:
      val = CR;
      break;
    case 0x0A:
      val = DR;
      break;
    case 0x0B:
      val = ER;
      break;
    case 0x0C:
      val = HR;
      break;
    case 0x0D:
      val = LR;
      break;
    case 0x0F:
      val = AR;
      break;
  }
  if (val & 0b00000001 > 0) {
    FR = 0b00010000;
  } else {
    FR = 0b00000000;
  }
  AR = (val >> 1) + ((val & 0b00000001) << 7);
  if (AR == 0) FR |= 0b10000000;
  cc += 8;
  cc_dec += 8;
  pc++;
}

void set_phl() {
  uint8_t b = (code & 0b00111000) >> 3;
  uint8_t mask = 0b00000001 << b;
  uint8_t r = get_byte(HL(HR, LR));
  r |= mask;
  put_byte(HL(HR, LR), r);
  cc += 16;
  cc_dec = 16;
  pc++;
}

void res_phl() {
  uint8_t b = (code & 0b00111000) >> 3;
  uint8_t mask = 0b00000001 << b;
  uint8_t r = get_byte(HL(HR, LR));
  r &= (~mask);
  put_byte(HL(HR, LR), r);
  cc += 16;
  cc_dec = 16;
  pc++;
}

void bit_phl() {
  uint8_t b = (code & 0b00111000) >> 3;
  uint8_t mask = 0b00000001 << b;
  uint8_t r = get_byte(HL(HR, LR));

  r = r & mask;
  if (r) {
    FR &= 0b01111111;
  } else {
    FR |= 0b10000000;
  }
  FR |= 0b00100000;
  FR &= 0b10110000;
  cc += 16;
  cc_dec = 16;
  pc++;
}
```
