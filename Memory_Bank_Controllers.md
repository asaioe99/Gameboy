# メモリバンクコントローラ（MBC）

ゲームボーイの16ビットアドレスバスは、ROMとRAMのアドレス空間が限られているため、多くのゲームでは、バンクスイッチによって利用可能なアドレス空間を拡張するメモリバンクコントローラ（MBC）が使用されている。このMBCチップはゲームカートリッジの中に入っています（つまり、ゲームボーイ本体には入っていません）。

各カートリッジでは、カートリッジのヘッダーに記載されているように、ROMの0147hのバイトに、必要な（または好ましい）MBCのタイプを指定する必要があります。いくつかの異なるMBCタイプが利用可能です。

## None (32KByte ROM only)
ROMが32KBytes以下の小型ゲームでは、ROMバンキング用のMBCチップは必要ありません。ROMは0000-7FFFhのメモリに直接マッピングされます。オプションとして、A000-BFFFに8KByteまでのRAMを接続することができますが、その場合、小さなMBCのような回路は必要でも、本物のMBCチップは必要ありません。

## MBC1 (max 2MByte ROM and/or 32KByte RAM)
これは、ゲームボーイ用の最初のMBCチップです。新しいMBCチップはどれも同じように動作するので、あるMBCチップから別のMBCチップにプログラムをアップグレードするのは比較的簡単です。

0000-7FFFのメモリは、ROMからの読み込みと、MBCのコントロールレジスタへの書き込みの両方に使用されていることに注意してください。

### 0000-3FFF - ROM Bank 00 (Read Only)
このエリアには、常にカートリッジROMの最初の16KByteが含まれています。

### 4000-7FFF - ROM Bank 01-7F (Read Only)
この領域には、さらに16KByteのROMバンクのいずれかを含めることができ、最大125ROMバンク（ほぼ2MByte）のアドレス指定が可能です。後述するように、バンク番号20h、40h、60hは使用できないため、125バンクという奇数バンクになります。

### A000-BFFF - RAM Bank 00-03, if any (Read/Write)
この領域は、カートリッジに外部RAMがある場合、そのアドレスに使用されます。外部RAMはバッテリーバッファリングされていることが多く、ゲームボーイの電源が切れても、カートリッジがゲームボーイから外されても、ゲームポジションやハイスコアテーブルを保存することができます。使用可能なRAMのサイズは以下の通りです。2KByte（A000-A7FF）、8KByte（A000-BFFF）、32KByte（A000-BFFFの8Kバンク4つ）です。

### 0000-1FFF - RAM Enable (Write Only)
外部RAMを読み書きする前に、このアドレス空間に書き込んで、外部RAMを有効にする必要があります。Gameboyのパワーダウン時に外部RAMの内容が損傷しないように、アクセス後は外部RAMを無効にすることをお勧めします。通常、以下の値が使用されます。

```
00h  Disable RAM (default)
0Ah  Enable RAM
```

実際には、下位4ビットが0Ahの値の場合はRAMを有効にして、それ以外の値はRAMを無効にする、ということになる。

### 2000-3FFF - ROM Bank Number (Write Only)
このアドレス空間に書き込むと、ROMのバンク番号の下位5ビット（01～1Fhの範囲）を選択します。00hを書き込むと、MBCはそれを01hのバンクにも変換します。0000-3FFFを読めば常にROMバンク00hに直接アクセスできるので、今のところ支障はありません。しかし、（下記のレジスタで上位ROMバンクのビットを指定した場合）バンク20h、40h、60hでも同じことが起こります。これらのROMバンクをアドレス指定しようとすると、代わりにバンク21h、41h、61hが選択されます。

### 4000-5FFF - RAM Bank Number - or - Upper Bits of ROM Bank Number (Write Only)
この2ビットレジスタは、現在のROM/RAMモードに応じて、00-03hの範囲でRAMバンクを選択するか、ROMバンク番号の上位2ビット（5-6ビット）を指定するために使用されます。(下記参照)

### 6000-7FFF - ROM/RAM Mode Select (Write Only)
上記レジスタの2ビットをROMバンクの上位2ビットとして使用するか、RAMバンク番号として使用するかを選択する1ビットレジスタです。

```
00h = ROM Banking Mode (up to 8KByte RAM, 2MByte ROM) (default)
01h = RAM Banking Mode (up to 32KByte RAM, 512KByte ROM)
```

モード0ではRAMバンク00hのみ、モード1ではROMバンク00-1Fhのみ使用可能であり、プログラムは自由に両モードを切り替えることができます。

