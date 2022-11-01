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
この時点で必要となる命令を如何に列挙する（していない。加筆予定）。

基本的には、通常のエミュレータ作成と同じで、ROM及びRAMを定義し、これを1Byte毎に読み込むことで、対応する命令を実行するだけである。このレベルで何をすれば良いか分からないのであれば、以下の本を参考にエミュレータを書いてみよう。

- https://amzn.asia/d/dqL8x71
- https://amzn.asia/d/ji0XX5e

上の一冊だけで十分です。読む範囲は、最初のあたりのＣ言語でエミュレータを実装する部分だけで、FPGAの部分は必要ありません（とても面白いので、時間があれば読み込むべきです）。

目標：エミュレータの作り方を学ぶ

## 5. SPI仕様の理解
ArduinoIDEでpico＋LCDを使用する際、「ある問題」によりSPIライブラリを利用できません。そこで、SPIをソフトウェア上で実装する必要があります。そのためには、SPIとは何か、大まかに理解する必要があります。

- https://www.analog.com/jp/analog-dialogue/articles/introduction-to-spi-interface.html

どれでも好きなものを読んで理解すれば良いでしょう。難しい部分は、pinの名称がドキュメントによって異なることです。対応表みたいなのを作ると良いですね。あとは、負論理という言葉にピンとこない方（手順１で必要なはずですが）は、一度調べてみよう。

目標：SPIの仕組みを理解する

## 6. PICO 1.3 LCDの仕様理解
以下のLCDを使います。

- https://www.waveshare.com/wiki/Pico-LCD-1.3

これをArduinoIDE上で作成したコードで制御するのが目標です。問題は、このLCDのドライバであるST7789というチップです。以下の仕様書とライブラリを読んで、LCD上にbitmapを表示させることが目標です。

- chrome-extension://efaidnbmnnnibpcajpcglclefindmkaj/https://www.rhydolabz.com/documents/33/ST7789.pdf
- https://github.com/adafruit/Adafruit-ST7735-Library

目標：LCDドライバの仕組みを理解する

## 7. PICO 1.3 LCD用デバイスドライバの作成

## 8. PPUのBGのみ実装（ここまでで、電源ON後のロゴスクロールが成功する）

## 9. test ROMを利用して残りの命令を実装（←今ココ）


### ドキュメント作成について
- https://monoworks.co.jp/post/2020-05-26-output-pdf-docx-html-from-markdown-with-vscode/
- https://qiita.com/pooshikin/items/b6fa4b9341b50cacddaf
- https://qiita.com/sta/items/c88093b1b9da9c77b577

### Arduino SPIについて
- http://www.musashinodenpa.com/arduino/ref/index.php?f=1&pos=539 （公式リファレンス）

Arduino Mega: 50(MISO)、51(MOSI)、52(SCK)、53(SS)

### 液晶（ST7789 TFT）
- ananevilya/Arduino-ST7789-Library
- https://shikarunochi.matrix.jp/?p=2834
- https://simple-circuit.com/arduino-st7789-ips-tft-display-example/
- http://kako.com/blog/?p=47810 (CSピンを取り出す方法についての考察)
- https://www.switch-science.com/products/7327?_pos=17&_sid=2ec415bed&_ss=r (転用可能？)
  - https://www.waveshare.com/wiki/Pico-LCD-1.3 (上記wiki ST7789でCS有りなのでたぶん行ける！)

CSpinがあればSPI_MODE0で動くかも知れない。要実験

液晶について知っておくべきことは、SPI対応のものでも、コストカット品にはCSピンがない製品もあり、それ故に複数スレーブに利用できない（かも知れない）可能性があることである。CSがない場合、SPI_MODEが2でないと動作しないらしく（理由不明）、このためにSRAM（SPI_MODE0）との共用が不可能になる。

解決方法としては、同じST7789チップを搭載した1.54インチのLCDが販売されており、これはCSピンがあり、恐らくSPI_MODE0で動作するとのことなので、これを利用することが挙げられる。

また、より合理的な解決方法は、同じくST7789制御の1.3インチLCDを搭載した、ラズパイpico用のジョイスティック＋ボタン×4（GameBoyエミュとしては申し分ない）のLCDがあり、これはCSがきちんと省略されずに使用可能である！

未確認の情報であるが、CS省略品でも、バラして基板上のパターンをカットしてCSを取り出せば（どうやらGNDに繋がっているらしい）、SPI_MODE0かつ複数スレーブで利用できる可能性がある。

(1.54インチの製品はCSあり)

ひょっとしたら、CSがないため動作させられない可能性。他のSPIデバイスにアクセスしている間にどういう動作をするか不明

- http://try3dcg.world.coocan.jp/note/spi/st7789.html

### SRAM
- https://ameblo.jp/pi-poh/entry-12667969903.html　（プルアップについて）

SPI＿MODE0で動作。液晶はMODE2なので共用不可


### MBCについて
以下を読む
- http://gameboy.mongenel.com/dmg/asmmemmap.html

### ST7789(pico LCD 1.3)
四の五の言わずに以下を読め
- https://github.com/adafruit/Adafruit-ST7735-Library


### エミュレータの実装例
- https://github.com/keichi/gbr

### CPU opcode一覧
- https://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html
- https://github.com/pokemium/gb-docs-ja


https://github.com/adafruit/Adafruit-ST7735-Library/blob/master/examples/graphicstest_st7789/graphicstest_st7789.ino

### picoに移行
SPIのpinは公式ArduinoIDEからでは割り当て変更不可なので、ソフトウェアSPIを実装すべき。


### 追加の関数等
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
