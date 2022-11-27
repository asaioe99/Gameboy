# STEP2　ROM headerの解析
## rom headerを読んでみよう
本章では、rom headerについて理解を深めます。実は、特定のMBCに対応したエミュレータを実装するだけであれば、ほとんど無視しても問題ないのです。しかしながら、いきなりゲームボーイの命令セットを理解してエミュレータを作製する前に、一度肩慣らしとして取り組みたいと思います。

!!! tip 本章での目標：GBカートリッジROMのheaderを理解する

    - 簡単なアセンブリについて体験する
    - Arduino IDEによる開発に慣れる

## headerの構造
最初に、header全体の構造について見てみましょう。

|アドレス|用途|備考|
|---|---|---|
|0x0100～0x0103|Entry point|bootstrap実行後に最初に読み込まれる部分|
|0x0104～0x0133|任天堂ロゴデータ|海賊版を検出するために利用|
|0x0134～0x0143|タイトル|内部に下記の別データが書き込まれる場合もある|
|0x013F～0x0142|製造コード||
|0x0143|CGB flag|ゲームボーイカラーとの互換性維持のためのもの|
|0x0144～0x0145|新ライセンスコード||
|0x0146|SGB flag|スーパーゲームボーイとの互換性維持のためのもの|
|0x0147|カートリッジタイプ||
|0x0148|ROM容量||
|0x0149|RAM容量||
|0x014A|地域コード||
|0x014B|旧ライセンスコード||
|0x014C|Mask ROM version number||
|0x014D|headerチェックサム||
|0x014E～0x014F|ROM全体のチェックサム||

ゲームボーイは、後方互換性のために、タイトルデータの領域に順次当初とは異なるデータが書き込まれるようになっています。言い換えれば、ゲームボーイに関して言えば、ここは動作そのものには影響を与えないということです。

これらのheader情報で唯一重要なのは、任天堂のロゴデータです。実は、この部分のデータは圧縮された画像ファイルとなっており、ゲームボーイ本体のROMに書き込まれたbootstrapがこのファイルを展開し、正しいデータが書き込まれているを確認します。ここで、もし正規のロゴデータが読み込むことができなかった場合、ゲームボーイは起動できないようになっています。

なぜこのような仕組みになっているかというと、不正にカートリッジを製造して販売する業者に対して法的に対処するためです。もしも不正なカートリッジにロゴデータが存在しないと、そのカートリッジは起動することができません。一方で、正規のロゴデータを含めた場合、登録商標の不正使用により販売を差し止めできるということだそうです。

それでは各項目について説明します。

### 0100h-0103h Entry point
この部分は、bootstrapでの処理が終了した後に最初に実行される部分です。機械語及びアセンブリで確認すると、以下の通りになっています。

```nasm
0x0100:NOP
0x0101:JP 0150h
```

基本的にはこの通りですが、筆者の経験上いつもそうである訳ではありません。`JP`先のアドレスが異なっていることが多いです。

では、実際に覗いてみます。

```c
// Dump_Entry_Point.ino
void loop() {
  Serial.println("--DUMP START--");

  PORTL = B00000111;
  delayMicroseconds(10);

  char buf[16];
  uint16_t addr;
  Serial.println("--Entry Point--");
  for (int i = 0; i < 4; i++) {
    addr = 0x100 + i;
    sprintf(buf, "%04X : %02X ", addr, read_byte(addr));
    Serial.print(buf);
    Serial.println("");
  }
  Serial.println("--END--");
  while (true);
}
```

実行結果は以下の通りです。

```
--DUMP START--
- Entry Point -
0100 : 00 // NOP
0101 : C3 // JP
0102 : 50 // 0x50 下位8bit
0103 : 01 // 0x01 上位8bit
-- END --
```

今後作成するエミュレータは、上記例の通りに、PCが指す特定のアドレスに存在する命令を読み込み、これをデコードし、命令の動作仕様に沿ってレジスタ等を変化させていけば良いのです。

### 0104h-0133h 任天堂ロゴ
ブートプロセスの際に参照する、任天堂純正のROMであるかチェックするためのデータ領域です。もしも下記のバイト列でなかった場合、ブートプロセス中に無限ループに入り、動作がその時点で終了してしまします。ちなみに、ゲームボーイカラーの場合は先頭の18hバイトを検証するだけだそうですが、理由は分かりません。

ちなみに、このバイト列は任天堂のロゴになっているのですが、それは海賊版がこのバイト列を利用すると商標権の侵害になるということで、海賊版取り締まりの法的根拠に利用されているそうです。

```
CE ED 66 66 CC 0D 00 0B 03 73 00 83 00 0C 00 0D
00 08 11 1F 88 89 00 0E DC CC 6E E6 DD DD D9 99
BB BB 67 63 6E 0E EC CC DD DC 99 9F BB B9 33 3E
```

こちらも検証してみましょう。

```c
//Dump_Logo.ino
void loop() {
  Serial.println("--DUMP START--");

  PORTL = B00000111;
  delayMicroseconds(10);

  char buf[16];
  Serial.println("--logo--");
  for (int j = 0; j < 3; j++) {
    Serial.print("    :");
    for (int i = 0; i < 16; i++) {
      sprintf(buf, "%02X ", read_byte(0x104 + i + 16 * j));
      Serial.print(buf);
    }
    Serial.println("");
  }
  Serial.println("--END--");
  while (1);
}
```

以下は実行結果です。

```
-DUMP START--
--logo--
    :CE ED 66 66 CC 0D 00 0B 03 73 00 83 00 0C 00 0D 
    :00 08 11 1F 88 89 00 0E DC CC 6E E6 DD DD D9 99 
    :BB BB 67 63 6E 0E EC CC DD DC 99 9F BB B9 33 3E 
--END--
```

このままでは理解不能ですが、別の章でbootstrapについて解説するので心配ありません。

### 0134h-0143h タイトル
ゲームのタイトルが大文字ASCII（16Byteになるように0でパディング）で格納されています。ゲームボーイカラーのソフトでは、発売時期に応じて15Byteや11Byteのみの利用になっているそうです。

早速検証してみます。

```c
// Dump_Title.ino
void loop() {
  Serial.println("--DUMP START--");

  PORTL = B00000111;
  delayMicroseconds(10);

  char buf[16];
  Serial.println("--title--");
  Serial.print("    :");
  for (int i = 0; i < 16; i++) {
    sprintf(buf, "%02X ", read_byte(0x134 + i));
    Serial.print(buf);
  }
  Serial.println("");
  Serial.println("--END--");
  while (1);
}
```

結果は以下の通りです。

```
--DUMP START--
--title--
    :50 4F 4B 45 4D 4F 4E 20 47 52 45 45 4E 00 00 00 
  // P  O  K  E  M  O  N     G  R  E  E  N   
--END--
```

### 013Fh-0142h 製造元コード
古いカートリッジでは、このエリアはタイトルの一部（上記参照）であったそうですが、新しいカートリッジでは、このエリアには4文字の大文字の製造者コードが含まれているそうです。

今回は、「古いカートリッジ」ですのでタイトルとして利用されています。

### 0143h - CGB Flag
古いカートリッジでは、このバイトはタイトルの一部でした（上記参照）。CGBカートリッジでは、上位ビットはCGB機能を有効にするために使用されます。このビットは必須で、さもなければCGBはそれ自体を非CGB-Modeに切り替えます。典型的な値は以下の通りです。

ゲームボーイではこの部分もタイトルの一部でしたが、ゲームボーイカラー発売の際にソフトがゲームボーイに対応しているかを判別するためのフラグとして利用されることになったそうです。

```
80h - ゲームボーイカラー/ゲームボーイ対応
C0h - ゲームボーイカラーのみ対応
```

7bit及び2bitまたは3bitの位置に値をセットすると、パレットが初期化されていない特殊な非ゲームボーイカラーのモードに切り替わるそうです。用途は不明ですが、ROMに固定パレットデータを持つモノクロゲームのカラー化に使用されているそうです。

今回は、「古いカートリッジ」ですので、これもタイトルとして利用されています。

### 0144-0145 ライセンスコード（SGB以降）
ゲームの会社を示す、2文字のASCIIライセンスコードを指定しています。スーパーゲームボーイが販売された以降にリリースされたゲームにのみ表示があり、それ以前の古いゲームでは、代わりに014Bのヘッダーエントリーが使用されます。

ライセンスコードの例 : 

|16進|企業|16進|企業|16進|企業|
|--|--|--|--|--|--|
|00|none|01|Nintendo R&D1|08|Capcom|
|13|Capcom|18|Hudson Soft|19|b-ai|
|20|kss|22|pow|24|PCM Complete|
|25|san-x|28|Kemco Japan|29|seta|
|30|Viacom|31|Nintendo|32|Bandai|
|33|Ocean/Acclaim|34|Konami|35|Hector|
|37|Taito|38|Hudson|39|Banpresto|
|41|Ubi Soft|42|Atlus|44|Malibu|
|46|angel|47|Bullet-Proof|49|irem|
|50|Absolute|51|Acclaim|52|Activision|
|53|American sammy|54|Konami|55|Hi tech entertainment|
|56|LJN	|57|Matchbox|58|Mattel|
|59|Milton Bradley|60|Titus|61|Virgin|
|64|LucasArts|67|Ocean|69|Electronic Arts|
|70|Infogrames|71|Interplay|72|Broderbund|
|73|sculptured|75|sci|78|THQ|
|79|Accolade|80|misawa|83|lozc|
|86|tokuma shoten i*|87|tsukuda ori*|91|Chunsoft|
|92|Video system|93|Ocean/Acclaim|95|Varie|
|96|Yonezawa/s'pal|97|Kaneko|99|Pack in soft|
|A4|Konami (Yu-Gi-Oh!)|||||

実際に確認してみます。

```c
// Dump_License_Code.ino
void loop() {
  Serial.println("--DUMP START--");

  PORTL = B00000111;
  delayMicroseconds(10);

  char buf[10];
  uint16_t addr;
  Serial.println("--license code--");
  for (int i = 0; i < 2; i++) {
    addr = 0x144 + i;
    sprintf(buf, "%04X : %02X", addr, read_byte(addr));
    Serial.print(buf);
    Serial.println("");
  }
  Serial.println("--END--");
  while (1);
}
```

結果は以下の通りです。

```
--DUMP START--
--license code--
0144 : 30
0145 : 31
--END--
```

※理由は分かりませんが、予期していない値が表示されました

### 0146h SGB フラグ
スーパーゲームボーイの機能をサポートしているかどうかを指定します。

```
 00h ：通常のゲームボーイ又はゲームボーイカラーのみ
 03h ：スーパーゲームボーイ対応
```

このバイトが03h以外の値に設定された場合、スーパーゲームボーイ非対応になります。

そろそろ飽きてきましたが、確認してみましょう。

```c
// Dump_SGB_Flag.ino
void loop() {
  Serial.println("--DUMP START--");

  PORTL = B00000111;
  delayMicroseconds(10);

  char buf[10];
  uint16_t addr;
  Serial.println("--sgb flag--");
  addr = 0x146;
  sprintf(buf, "%04X : %02X", addr, read_byte(addr));
  Serial.println(buf);
  
  Serial.println("--END--");
  while (1);
}
```

結果は以下の通りです。

```
--DUMP START--
--sgb flag--
0146 : 03
--END--
```

スーパーゲームボーイに対応しているそうですが、これは事実と一致します。

### 0147h カートリッジ種類
カートリッジのMBCの種別及び外部ハードウェアの存在を表しています。

|値|構成|値|構成|
|---|---|---|---|
 00h|  ROM ONLY                | 19h|  MBC5|
 01h|  MBC1                    | 1Ah|  MBC5+RAM|
 02h|  MBC1+RAM                | 1Bh|  MBC5+RAM+BATTERY|
 03h|  MBC1+RAM+BATTERY        | 1Ch|  MBC5+RUMBLE|
 05h|  MBC2                    | 1Dh|  MBC5+RUMBLE+RAM|
 06h|  MBC2+BATTERY            | 1Eh|  MBC5+RUMBLE+RAM+BATTERY|
 08h|  ROM+RAM                 | 20h|  MBC6|
 09h|  ROM+RAM+BATTERY         | 22h|  MBC7+SENSOR+RUMBLE+RAM+BATTERY|
 0Bh|  MMM01|
 0Ch|  MMM01+RAM|
 0Dh|  MMM01+RAM+BATTERY|
 0Fh|  MBC3+TIMER+BATTERY|
 10h|  MBC3+TIMER+RAM+BATTERY  | FCh|  POCKET CAMERA|
 11h|  MBC3                    | FDh|  BANDAI TAMA5|
 12h|  MBC3+RAM                | FEh|  HuC3|
 13h|  MBC3+RAM+BATTERY        | FFh|  HuC1+RAM+BATTERY|


本書ではあまり重要ではないですが、これは複数のMBCに対応したエミュレータを製作する上では重要な情報で、この値に応じて内部的に動作を切り替える必要があります。

それでは確認してみましょう。

```c
// Dump_Cartridge_Type.ino
void loop() {
  Serial.println("--DUMP START--");

  PORTL = B00000111;
  delayMicroseconds(10);

  char buf[10];
  uint16_t addr;
  Serial.println("--artridge type--");
  addr = 0x147;
  sprintf(buf, "%04X : %02X", addr, read_byte(addr));
  Serial.println(buf);
  
  Serial.println("--END--");
  while (1);
}
```

結果は以下の通りです。

```
--DUMP START--
--cartridge type--
0147 : 03
--END--
```

```03```であるということは、```MBC1+RAM+BATTERY```ということになります。RAMとバッテリーがあるので、データをセーブできるということになります。

### 0148h ROMサイズ
カートリッジの ROM サイズを指定します。通常、```32KB shl N```として計算されます。

|値|サイズ|bank数|備考|
|---|---|---|---|---|
| 00h |  32KByte |no ROM banking||
| 01h |  64KByte |4 banks||
| 02h | 128KByte |8 banks||
| 03h | 256KByte |16 banks||
| 04h | 512KByte |32 banks||
| 05h |   1MByte |64 banks|  63バンクのみがMBC1として利用可能|
| 06h |   2MByte |128 banks| 125バンクのみがMBC1として利用可能|
| 07h |   4MByte |256 banks||
| 08h |   8MByte |512 banks||
| 52h | 1.1MByte |72 banks||
| 53h | 1.2MByte |80 banks||
| 54h | 1.5MByte |96 banks||

この情報も重要で、カセット内のデータをエミュレータ側で保持する場合は、ここで示された領域を確保する必要があります。

```c
// Dump_ROM_Size.ino
void loop() {
  Serial.println("== DUMP START ==");

  PORTL = B00000111;
  delayMicroseconds(10);

  char buf[10];
  uint16_t addr;
  Serial.println("= rom size =");
  addr = 0x148;
  sprintf(buf, "%04X : %02X", addr, read_byte(addr));
  Serial.println(buf);

  Serial.println("==  END  ==");
  while (1);
}
```

結果は以下の通りです。

```
--DUMP START--
--rom size--
0148 : 04
--END--
```

```04h - 512KByte (32 banks)```となります。

### 0149h RAMサイズ
カートリッジ内の外部RAMのサイズを指定します。

|値|サイズ|備考|
| --- | --- | --- |
|00h | None||
|01h | 2 KBytes||
|02h | 8 Kbytes||
|03h | 32 KBytes |各8KByteの合計4バンク|
|04h | 128 KBytes |各8KByteの合計16バンク|
|05h | 64 KBytes| 各8KByteの合計8バンク|

 
MBC2 には 512×4 ビットの RAM が内蔵されていますが、MBC2 を使用する場合は、このエントリに 00h を指定する必要があります。
 
この情報は重要で、カセット内のデータをエミュレータのソースコードに埋め込むことで保持する場合は、ここで示された領域を確保する必要があります。

```c
// Dump_RAM_Size.ino
void loop() {
  Serial.println("== DUMP START ==");

  PORTL = B00000111;
  delayMicroseconds(10);

  char buf[10];
  uint16_t addr;
  Serial.println("= rom size =");
  addr = 0x149;
  sprintf(buf, "%04X : %02X", addr, read_byte(addr));
  Serial.println(buf);

  Serial.println("==  END  ==");
  while (1);
}
```

結果は以下の通りでした。

```
--DUMP START--
--ram size--
0149 : 03
--END--
```

`03h - 32 KBytes (各8KByteの合計4バンク)`が利用可能となります。
 
### 014Ah 対象国コード
ROMの対応地域を表しています。

```
00h：日本
01h：海外
```

ソースコードと結果だけ示します。

```c
// Dump_Destination_Code.ino
void loop() {
  Serial.println("--DUMP START--");

  PORTL = B00000111;
  delayMicroseconds(10);

  char buf[10];
  uint16_t addr;
  Serial.println("--destination code--");
  addr = 0x14a;
  sprintf(buf, "%04X : %02X", addr, read_byte(addr));
  Serial.println(buf);

  Serial.println("--END--");
  while (1);
}
```

```
--DUMP START--
--destination code--
014A : 00
--END--
```
 
### 014Bh 旧ライセンスコード
過去にはこのアドレスにライセンスコードが指定されていたそうです。33hを指定すると、このアドレスではなく代わりに0144-0145hを参照することになります。

ソースコードと結果だけ示します。

```c
// Dump_Old_Licensee_Code.ino
void loop() {
  Serial.println("--DUMP START--");

  PORTL = B00000111;
  delayMicroseconds(10);

  char buf[10];
  uint16_t addr;
  Serial.println("--old_llicensee_code--");
  addr = 0x14b;
  sprintf(buf, "%04X : %02X", addr, read_byte(addr));
  Serial.println(buf);

  Serial.println("--END--");
  while (1);
}
```

```
--DUMP START--
--old_llicensee_code--
014B : 33
--END--
```

### 014Ch ROMバージョン
ゲームのバージョンを指定しているそうです。通常は00hとなります。
 
早速確認してみましょう。

```c
// Dump_ROM_Version.ino
void loop() {
  Serial.println("--DUMP START--");

  PORTL = B00000111;
  delayMicroseconds(10);

  char buf[10];
  uint16_t addr;
  Serial.println("--rom_version--");
  addr = 0x14c;
  sprintf(buf, "%04X : %02X", addr, read_byte(addr));
  Serial.println(buf);

  Serial.println("--END--");
  while (1);
}
```

実行結果は以下の通りです。

```
--DUMP START--
--rom version--
014C : 01
--END--
```

これは少し面白い事実です。どうやら、このROMは初期のものではなく後期版であるみたいです。
 
### 014Dh ヘッダーチャックサム
カートリッジヘッダーバイト0134-014Cの8ビットチェックサムが含まれています。チェックサムは以下のように計算されます。
 
```c
int checksum = 0:
for (uint16_t address = 0x134; address < 0x14D; address++) {
  checksum -= read_byte(address) - 1;
}
```
結果の下位8ビットは、この値と同じでなければならず、そうでない場合はゲームが動作しません。

ソースコードと結果だけ示します。

```c
// Dump_Header_Checksum.ino
void loop() {
  Serial.println("--DUMP START--");

  PORTL = B00000111;
  delayMicroseconds(10);

  char buf[10];
  uint16_t addr;
  Serial.println("--header checksum--");
  addr = 0x14d;
  sprintf(buf, "%04X : %02X", addr, read_byte(addr));
  Serial.println(buf);

  Serial.println("--END--");
  while (1);
}
```

```
--DUMP START--
--header checksum--
014D : 9B
--END--
```
  
### 014Eh-014Fh グローバルチェックサム
カートリッジROM全体の16ビット・チェックサム(上位バイトが先)が含まれています。カートリッジの全バイト（2つのチェックサムバイトを除く）を加算して作成されます。ゲームボーイ本体はこの値を参照することはありません。何のために用意されたのか不明ですが、ROM吸い取りが正しく行われたことを確認する際に有用です。

こちらもソースコードと結果だけ示します。

```c
// Dump_Global_Checksum.ino
void loop() {
  Serial.println("--DUMP START--");

  PORTL = B00000111;
  delayMicroseconds(10);

  char buf[10];
  uint16_t addr;
  Serial.println("--global checksum--");
  for (int i = 0; i < 2; i++) {
    addr = 0x14e + i;
    sprintf(buf, "%04X : %02X", addr, read_byte(addr));
    Serial.print(buf);
    Serial.println("");
  }

  Serial.println("--END--");
  while (1);
}
```

```
--DUMP START--
--global checksum--
014E : F5
014F : 47
--END--
```

!!! tip 理解確認：チェックリスト
    - [ ] ゲームボーイのheaderについて理解したか
    - [ ] 自分で吸い取ったROMのheader内の各項目を確認できたか
