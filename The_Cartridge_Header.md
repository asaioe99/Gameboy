https://gbdev.gg8.se/wiki/articles/The_Cartridge_Header

# 0100-0103 - エントリポイント
任天堂ロゴを表示した後、内蔵ブート手順はこのアドレス（100h）にジャンプし、カートリッジ内の実際のメインプログラムにジャンプするようになっているはずです。通常、この4バイトの領域にはNOP命令があり、その後にJP 0150h命令が続きます。しかし、いつもそうとは限りません。

# 0104-0133 - 任天堂ロゴ
これらのバイトは、ゲームボーイの電源を入れたときに表示される任天堂のロゴのビットマップを定義しています。このビットマップのヘキサダンプは次のとおりです。

```
CE ED 66 66 CC 0D 00 0B 03 73 00 83 00 0C 00 0D
 00 08 11 1F 88 89 00 0E DC CC 6E E6 DD DD D9 99
 BB BB 67 63 6E 0E EC CC DD DC 99 9F BB B9 33 3E
```

ゲームボーイのブートプロシージャは、このビットマップの内容を（表示した後で）検証し、これらのバイトが正しくない場合、それをロックします。CGBはビットマップの最初の18hバイトだけを検証しますが、他のもの（例えばポケットゲームボーイ）は30hバイト全部を検証します。

# 0134-0143 - タイトル
ゲームのタイトルを大文字のASCIIで指定します。もし16文字以下なら、残りのバイトは00で埋め尽くされます。CGBを開発したとき、任天堂はこの領域を15文字に減らしましたが、数カ月後、11文字だけに減らすというすばらしいアイデアを思いつきました。元のタイトルバイトの新しい意味を以下に説明します。

# 013F-0142 - 製造元コード
古いカートリッジでは、このエリアはタイトルの一部（上記参照）であったが、新しいカートリッジでは、このエリアには4文字の大文字の製造者コードが含まれている。目的及び深い意味は不明。

# 0143 - CGB Flag
古いカートリッジでは、このバイトはタイトルの一部でした（上記参照）。CGBカートリッジでは、上位ビットはCGB機能を有効にするために使用されます。このビットは必須で、さもなければCGBはそれ自体を非CGB-Modeに切り替えます。典型的な値は以下の通りです。

```
 80h - ゲームはCGBの機能をサポートしていますが、古いゲームボーイでも動作します。
 C0h - ゲームは CGB のみで動作します (物理的には 80h と同じです)。
```

Bit7をセットし、Bit2または3をセットすると、ゲームボーイはパレットが初期化されていない特殊な非CGBモードに切り替わります。用途は不明ですが、ROMに固定パレットデータを持つモノクロゲームのカラー化に使用されるものと思われます。

# 0144-0145 - New Licensee Code
ゲームの会社または出版社を示す、2文字のASCIIライセンシーコードを指定します。この2バイトは新しいゲームでのみ使用されます (SGBが発明された後にリリースされたゲーム)。古いゲームでは、代わりに014Bのヘッダーエントリーが使用されます。

ライセンシーコードの例 : 

| class="wikitable"
|-
| '''00''' 
| none
| '''01''' 
| Nintendo R&D1
| '''08''' 
| Capcom
|-
| '''13''' 
| Electronic Arts
| '''18''' 
| Hudson Soft
| '''19''' 
| b-ai
|-
| '''20''' 
| kss
| '''22''' 
| pow
| '''24''' 
| PCM Complete
|-
| '''25''' 
| san-x
| '''28''' 
| Kemco Japan
| '''29''' 
| seta
|-
| '''30''' 
| Viacom
| '''31''' 
| Nintendo
| '''32''' 
| Bandai
|-
| '''33''' 
| Ocean/Acclaim
| '''34''' 
| Konami
| '''35''' 
| Hector
|-
| '''37''' 
| Taito
| '''38''' 
| Hudson
| '''39''' 
| Banpresto
|-
| '''41''' 
| Ubi Soft
| '''42''' 
| Atlus
| '''44''' 
| Malibu
|-
| '''46''' 
| angel
| '''47''' 
| Bullet-Proof
| '''49''' 
| irem
|-
| '''50''' 
| Absolute
| '''51''' 
| Acclaim
| '''52''' 
| Activision
|-
| '''53''' 
| American sammy
| '''54''' 
| Konami
| '''55''' 
| Hi tech entertainment
|-
| '''56''' 
| LJN
| '''57''' 
| Matchbox
| '''58''' 
| Mattel
|-
| '''59''' 
| Milton Bradley
| '''60''' 
| Titus
| '''61''' 
| Virgin
|-
| '''64''' 
| LucasArts
| '''67''' 
| Ocean
| '''69''' 
| Electronic Arts
|-
| '''70''' 
| Infogrames
| '''71''' 
| Interplay
| '''72''' 
| Broderbund
|-
| '''73''' 
| sculptured
| '''75''' 
| sci
| '''78''' 
| THQ
|-
| '''79''' 
| Accolade
| '''80''' 
| misawa
| '''83''' 
| lozc
|-
| '''86''' 
| tokuma shoten i*
| '''87''' 
| tsukuda ori*
| '''91''' 
| Chunsoft
|-
| '''92''' 
| Video system
| '''93''' 
| Ocean/Acclaim
| '''95''' 
| Varie
|-
| '''96''' 
| Yonezawa/s'pal
| '''97''' 
| Kaneko
| '''99''' 
| Pack in soft
|-
| '''A4''' 
| Konami (Yu-Gi-Oh!)
| 
| 
| 
| 
|

# 0147 - カートリッジ種類
カートリッジでどのメモリ・バンク・コントローラ(もしあれば)が使用されているか、また、カートリッジにさらに外部ハードウェアが存在するかどうかを指定します。

```
 00h  ROM ONLY                 19h  MBC5
 01h  MBC1                     1Ah  MBC5+RAM
 02h  MBC1+RAM                 1Bh  MBC5+RAM+BATTERY
 03h  MBC1+RAM+BATTERY         1Ch  MBC5+RUMBLE
 05h  MBC2                     1Dh  MBC5+RUMBLE+RAM
 06h  MBC2+BATTERY             1Eh  MBC5+RUMBLE+RAM+BATTERY
 08h  ROM+RAM                  20h  MBC6
 09h  ROM+RAM+BATTERY          22h  MBC7+SENSOR+RUMBLE+RAM+BATTERY
 0Bh  MMM01
 0Ch  MMM01+RAM
 0Dh  MMM01+RAM+BATTERY
 0Fh  MBC3+TIMER+BATTERY
 10h  MBC3+TIMER+RAM+BATTERY   FCh  POCKET CAMERA
 11h  MBC3                     FDh  BANDAI TAMA5
 12h  MBC3+RAM                 FEh  HuC3
 13h  MBC3+RAM+BATTERY         FFh  HuC1+RAM+BATTERY
```

# 0148 - ROMサイズ
カートリッジの ROM サイズを指定します。通常、"32KB shl N "として計算されます。

```
 00h -  32KByte (no ROM banking)
 01h -  64KByte (4 banks)
 02h - 128KByte (8 banks)
 03h - 256KByte (16 banks)
 04h - 512KByte (32 banks)
 05h -   1MByte (64 banks)  - only 63 banks used by MBC1
 06h -   2MByte (128 banks) - only 125 banks used by MBC1
 07h -   4MByte (256 banks)
 08h -   8MByte (512 banks)
 52h - 1.1MByte (72 banks)
 53h - 1.2MByte (80 banks)
 54h - 1.5MByte (96 banks)
```

# 0149 - RAMサイズ
