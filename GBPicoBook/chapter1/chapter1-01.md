# STEP1　ゲームボーイROM吸い取り器の作成
## Arduino Mega 2560の逆襲
いきなり最大の山場を迎えましたが、最初に取り組むべき課題はArduino Mega 2560を利用して、ROMの吸い取り器を作製することです。実は、有志が製作し公開しているゲームボーイROMを利用するのであれば、ROM吸い取り器をわざわざ作成する必要はありません。しかしながら、市販のROMを利用する場合は、ROM吸い取り器が必要となります。これは、専用の機器が販売されているそうですが、自作することをお勧めします。筆者の場合は、何故かArduino Mega 2560の残骸が自宅に転がっていたため、勿体ないので再利用することにしました。これにはポジティブな側面も確かにあり、ROM吸い取り器の作成により必然的にゲームボーイのカートリッジ内部の構造やアーキテクチャの一部について理解を深めることができます。そのため、可能な限り実際に吸い取り器の作製をすることを推奨します。

!!! tip 本章での目標：GBカートリッジからROMデータを吸い取る

    - ROMの「bank」という仕組みを理解する
    - Arduino IDEの使い方に慣れる
    - エミュレータ上で動作させたいゲーム（MBC1又はno MBC）のROMデータ取得

## ゲームボーイカートリッジの仕組み
ゲームボーイはカートリッジ内部にROMが存在し、本体がこれを読み込むことで各種ゲームが動作します。また、カートリッジ内部のROMはある特定のアドレスが対応しており、ゲームボーイ本体のCPUは、ROMに相当するアドレスから読み込みを行うことで動作します。以下は、ゲームボーイのメモリマップです。

|開始|終了|用途|その他|
| --- | --- | --- | --- |
|0000|3FFF|16 KiB ROM bank 00|00からFFは本体内部のbootstrapであるが、起動後は割り込み処理に利用|
|4000|7FFF|16 KiB ROM Bank 01~NN|カートリッジ内部のROMである。bankを切り替えて利用する場合がある|
|8000|9FFF|8 KiB Video RAM (VRAM)|グラフィック表示用に使用されるRAM領域|
|A000|BFFF|8 KiB External RAM|カートリッジ内部のRAM|
|C000|CFFF|	4 KiB Work RAM (WRAM)|汎用のRAM領域|
|D000|DFFF|	4 KiB Work RAM (WRAM)|汎用のRAM領域|
|E000|FDFF|Mirror of C000~DDFF (ECHO RAM)|使用禁止領域|
|FE00|FE9F|Sprite attribute table (OAM)|spriteデータの保存領域|
|FEA0|FEFF|Not Usable|使用禁止領域|
|FF00|FF7F|I/O Registers|各種レジスタ領域|
|FF80|FFFE|High RAM (HRAM)|スタック等に用いられる（ここ以外が利用されることもある|
|FFFF|FFFF|Interrupt Enable register (IE)|割り込み処理の制御に利用される|

今回重要なのはbankの概念です。この表の```0000 ～ 3FFF```及び```4000 ～ 7FFF```が関係しています。前者のアドレスにはbank 00が対応しており、起動時のbootstrap実行終了後は、単純に当該アドレスへの読み込みによりデータを得ることができます。一方で、後者についてはカートリッジに応じて複数のbankが同一のアドレスに対応しており、bankの切り替えによりカートリッジ内部で読み込むデータを切り替えることがます。

!!! note メモリマップについて
    今後、エミュレータを製作するにあたってメモリマップの表はうんざりするくらい眺めることになります。そこで、自分なりに仕様を作製し、メモ等を追加することをお勧めします。

### MBCについて
ゲームボーイのカートリッジには、複数の種類が存在しています。分かりやすく分類すれば、単純にROMの容量で分類できそうですが、本質的にはbankを切り替える仕組み（搭載されているmemory bank controller:MBC）で区別されます。今回製作するエミュレータでは、最も基本的なMBC1というものと、bankの切り替え機能がないROMカートリッジに対応させます。他のMBCについても本質的にはMBC1と同様の機能のため、余裕のある方は機能を追加してみてください。

!!! note 各種MBCについて
    MBCについて簡単にまとめたものが以下の表となります。
    |種類|容量|特徴|
    |---|---|---|
    |No MBC|32KiB|bank切り替えがないものであり、初期のソフトウェアに一部見られる（テトリス）|
    |MBC1|最大2MByte ROM|32KiB RAMを持つものもある。ポケモン等|
    |MBC2|最大256KiB ROM|512×4bits RAM|
    |MBC3|最大2MByte ROM|32KiB RAM及びちまーを持つものもある。|
    |MBC5|||
    |MBC6|||
    |MBC7|||

### MBC1のbank切り替え手順
MBC1におけるROMのbank切り替えはとても簡単で、アドレス0x2000～0x3FFFのいずれかに切り替えたいbankの番号（最大7bit）の下位5bitを書き込み、アドレス0x4000～0x5FFFのいずれかにbankの番号の上位2bitを書き込めばよいのです。

少し不思議な気がしますが、実はとても単純な仕組みです。先ほどのメモリマップを眺めると、そもそもこのアドレスはROMであるため、文字通り読み込みしかできないはずです。なので、本来であればこのアドレスには何を書き込んでも無視されるのですが、アドレスを指定する16bitつまり16本の端子の上位３本が、MBC1のチップ内部のレジスタに繋がっており、ここに001という値をセットすることでbank番号の下位5bitが、010をセットすると上位2bitが指定できるのです。また、書き込む値は8bitのはずですが、上位の3bitまたは6bitは無視されます。

分かりやすく表現すると以下の通りになります。

```c
void switch_bank(uint8_t bank_number) {
    write_byte(0x2000, bank_number & 0b00011111);
    write_byte(0x4000, bank_number & 0b01100000);
}
```

!!! tip 理解確認：チェックリスト

    - [ ] ROMのbank切り替えの方法を理解した


## Arduino Mega 2560による作製
### gpioとpinの対応
それでは実際に作製してみます。まずは、それぞれ好きな方法によりROMカートリッジ端子（pin）とArduino Mega 2560のgpioを接続してください。カートリッジのpinは、以下の画像の通りに対応します。

![image](/chapter1/image/1.jpg)*右から0,1,2,,,31と対応*

|pin|gpio|抵抗|備考|
|---|---|---|---|
| 0:Vcc|+5V|なし|電源|
| 1:CLK|なし|なし|ROM吸い取りの場合不要|
| 2:WR|D49|220Ω/10kΩ|pinはそれぞれ220Ω、10kΩと接続し、前者はgpioへ、後者はGNDへ接続（プルダウン）する。また、負論理であることに注意|
| 3:RD|D48|220Ω/10kΩ|pinはそれぞれ220Ω、10kΩと接続し、前者はgpioへ、後者はGNDへ接続（プルダウン）また、負論理であることに注意|
| 4:CS|D47|220Ω/10kΩ|pinはそれぞれ220Ω、10kΩと接続し、前者はgpioへ、後者はGNDへ接続（プルダウン）また、負論理であることに注意|
| 5:A0|D22|220Ω|アドレス指定（最下位）|
| 6:A1|D23|220Ω|アドレス指定|
| 7:A2|D24|220Ω|アドレス指定|
| 8:A3|D25|220Ω|アドレス指定|
| 9:A4|D26|220Ω|アドレス指定|
|10:A5|D27|220Ω|アドレス指定|
|11:A6|D28|220Ω|アドレス指定|
|12:A7|D29|220Ω|アドレス指定|
|13:A8|D62|220Ω|アドレス指定|
|14:A9|D63|220Ω|アドレス指定|
|15:A10|D64|220Ω|アドレス指定|
|16:A11|D65|220Ω|アドレス指定|
|17:A12|D66|220Ω|アドレス指定|
|18:A13|D67|220Ω|アドレス指定|
|19:A14|D68|220Ω|アドレス指定|
|20:A15|D69|220Ω|アドレス指定（最上位）|
|21:D0|D54|220Ω|データ出力（最下位）|
|22:D1|D55|220Ω|データ出力|
|23:D2|D56|220Ω|データ出力|
|24:D3|D57|220Ω|データ出力|
|25:D4|D58|220Ω|データ出力|
|26:D5|D59|220Ω|データ出力|
|27:D6|D60|220Ω|データ出力|
|28:D7|D61|220Ω|データ出力（最上位）|
|29:RST|+5V|10kΩ|リセット用（負論理のため+5V）|
|30:Vin|なし|なし|ROM吸い取りの場合不要|
|31:GND|GND|なし|Arduino Mega 2560のGNDに接続|

!!! note 抵抗と負論理について
    上記表における抵抗値は、別に指示がなければpinとgpioの間に挿入し、電流値を制限するのに用いています。適切な値は正直なところ明確ではないので、最適値ではありません。
    　また、負論理のpinの場合、通常とは逆で0Vをに繋げると正となり、5Vだと負となるため注意が必要です。

### 1Byteの読み込み
ここまでで理解、Arduino Mega 2560とカートリッジROMの接続が完了しました。それでは、実際にgpioを制御することで、ROMのデータを読み込んでみます。

まずは、カートリッジにアドレスを指定するためのgpioについて設定します。愚直に実装すれば以下の通りになります。

```C
void setup() {
  Serial.begin(9600);

  // PORT L 
  pinMode(49, OUTPUT); // Write
  pinMode(48, OUTPUT); // Read
  pinMode(47, OUTPUT); // Chip select

  // address下位8bit PORT A
  pinMode(22, OUTPUT); // LSB
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(29, OUTPUT); // MSB

  // address上位8bit PORT K
  pinMode(62, OUTPUT); // LSB
  pinMode(63, OUTPUT);
  pinMode(64, OUTPUT);
  pinMode(65, OUTPUT);
  pinMode(66, OUTPUT);
  pinMode(67, OUTPUT);
  pinMode(68, OUTPUT);
  pinMode(69, OUTPUT); // MSB

  // DATA PORT F
  pinMode(54, INPUT); //LSB
  pinMode(55, INPUT);
  pinMode(56, INPUT);
  pinMode(57, INPUT);
  pinMode(58, INPUT);
  pinMode(59, INPUT);
  pinMode(60, INPUT);
  pinMode(61, INPUT); // MSB
}
void loop() {
  uint16_t address = 0x0000; // 読み込みたいaddress
  uint8_t data = 0x00; // 読み込んだdata

  // Address指定
  digitalWrite(22, Address & 0x0001);
  digitalWrite(23, Address & 0x0002);
  digitalWrite(24, Address & 0x0004);
  digitalWrite(25, Address & 0x0008);
  digitalWrite(26, Address & 0x0010);
  digitalWrite(27, Address & 0x0020);
  digitalWrite(28, Address & 0x0040);
  digitalWrite(29, Address & 0x0080);
  digitalWrite(62, Address & 0x0100);
  digitalWrite(63, Address & 0x0200);
  digitalWrite(64, Address & 0x0400);
  digitalWrite(65, Address & 0x0800);
  digitalWrite(66, Address & 0x1000);
  digitalWrite(67, Address & 0x2000);
  digitalWrite(68, Address & 0x4000);
  digitalWrite(69, Address & 0x8000);

  // 読み込み指定
  digitalWrite(49,HIGH); // 1
  digitalWrite(48, LOW); // 0
  digitalWrite(47,HIGH); // 1

  data |= digitalRead(54) << 0;
  data |= digitalRead(55) << 1;
  data |= digitalRead(56) << 2;
  data |= digitalRead(57) << 3;
  data |= digitalRead(58) << 4;
  data |= digitalRead(59) << 5;
  data |= digitalRead(60) << 6;
  data |= digitalRead(61) << 7;

  Serial.println(data, HEX);
}
```

このままでは酷いソースコードです。実は、Arduinoでは`digitalWrite()`等の関数でgpioを制御するととても時間が掛かります。しかも、同時に入力値を制御できないので、場合によってはタイミングが上手く合いません。そこで、以下の様に実装します。

```c
void setup() {
  Serial.begin(9600);

  DDRA = B11111111;
  DDRK = B11111111;
  DDRF = B00000000;
  DDRL = B00000111;
  PORTL = B00000000;
  PORTA = B00000000;
  PORTK = B00000000;
}
void loop() {
  uint16_t address = 0x0000; // 読み込みたいaddress
  uint8_t data = 0x00; // 読み込んだdata

  PORTA = address & 0xFF;
  PORTK = (address >> 8) & 0xFF;

  write_address(address); // 実装は上記例の通り
  PORTL = B00000101;
  delayMicroseconds(20);
  data = PINF;
  PORTL = B00000111;
  delayMicroseconds(20);

  Serial.println(data, HEX);
}
```

こうすることにより、一括してgpioの入出力を実行できます。

!!! tip 理解確認：チェックリスト
    - [ ] 指定したアドレスからデータを読み込む方法を理解したか
    - [ ] 実際にアドレス0x0100からデータを読み込み、結果は0x00になったか
    - [ ] 改良後のソースコードを理解できたか

## MBC1のbank単位での読み込み
ここまでの作業により、Arduino Mega 2560によりROM内部の任意のアドレスのデータを取得できることになりました。ここからは、プログラムの規模を拡大させて、指定したbankのデータを取得する様にしましょう。

### bank00の読み込み
bank00はアドレス0x0000～0x3FFFに存在します。MBC1では、特にbankを切り替える必要のないのでそのまま読み込むことができます。

今回は、作業の簡略のためにSDカードを使用してROMデータを保存します。そこで、SPIにより通信を行うSDモジュールを使用し、データを保存しましょう。

以下は、SDカードスロットを追加した場合の実装例です。

```C
#include <SD.h>

void setup() {
    SD.begin(53);
}

void loop() {
    if (!SD.exists(romFileName)){
      FILE dumpFile = SD.open("bank00.gb", FILE_WRITE);
    }
    for (uint16_t address = 0; address < 0x4000; address++){
        uint8_t data = read_byte(address);
        dumpFile.write(data);
    }

    dumpFile.flush();
    dumpFile.close();
}
```

SDカードモジュールを正しくArduino Mega 2560に組み込めば、上記の様な実装でbank00内のデータを取得できます。また、`read_byte()`はこれまでの例から自作可能ですので、自力で実装してください。

!!! tip 理解確認：チェックリスト
    - [ ] SDカードモジュールを実装できたか
    - [ ] 取得したbank00のデータをASCIIコードでデコードし、ゲームタイトルを確認できたか

### bank01～の読み込み
次はbank01～読み込みに挑戦します。これらのbankはアドレス0x4000～0x7FFFに配置されており、更にbankの切り替えを要します。ここまでの説明により実装は可能なはずですので、挑戦してみてください。

!!! tip 理解確認：チェックリスト
    - [ ] 任意のbankのデータを取得できたか
    - [ ] ROM内の全てのデータを取得できたか

## ROMデータの利用方法
本章の最後として、取得したROMデータの組み込み方法について説明します。Picoでは、ROMデータをファイルシステムとして扱うことも一応可能だそうですが、実装が複雑になるため、今回は直接ソースコード内に配列として組み込みます。

```c
const uint8_t bank_00[] {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11,,,,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
```

このように実装するためには、先ほどSDカードに書き込んだバイナリデータを配列の表記に変換する必要があります。簡単な方法としては、マクロ機能付きのエディタ等を利用することが考えられます。特に方法は指定しませんので、何らかの方法で変換できるようにしてください。

今後、test romを利用したエミュレータを作製する際に気を付けるべきことですが、それらのtest romは`.gb`という拡張子になっていると思います。これは、bank00からbank01,bank02,,,と生のバイナリ列が連続しているだけです。つまり、先頭から0x4000Byte毎に区切ることで、各bankのバイナリデータが取得できます。そして、取得した各bankデータをc言語の配列としてソースコードに含めることになります。

!!! tip 理解確認：チェックリスト
    - [ ] 取得したROMのバイナリデータをcの配列形式に変換できたか