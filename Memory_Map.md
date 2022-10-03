# General Memory Map

|Start | End | Description | Notes |
|------|-----|-------------|-------|
|0000	|3FFF|16KB ROM bank 00	|From cartridge, usually a fixed bank|
|4000	|7FFF	|16KB ROM Bank 01~NN|	From cartridge, switchable bank via MBC (if any)|
|8000	|9FFF	|8KB Video RAM (VRAM)|	Only bank 0 in Non-CGB mode <br> Switchable bank 1~7 in CGB mode|
|A000	|BFFF	|8KB External RAM	In cartridge, switchable bank if any|
|C000	|CFFF	|4KB Work RAM (WRAM) bank 0	|
|D000	|DFFF	|4KB Work RAM (WRAM) bank 1~N	Only bank 1 in Non-CGB mode <br> Switchable bank 1~7 in CGB mode|
|E000|	FDFF|	Mirror of C000~DDFF (ECHO RAM)	Typically not used|
|FE00|	FE9F|	Sprite attribute table (OAM)	|
|FEA0|	FEFF|	Not Usable	|
|FF00|	FF7F|	I/O Registers	|
|FF80|	FFFE|	High RAM (HRAM)	|
|FFFF|	FFFF|	Interrupts Enable Register (IE)	|

# Jump Vectors in first ROM bank
ジャンプベクターとして使用することを想定しているアドレスは以下の通りです。

- RST commands: 0000,0008,0010,0018,0020,0028,0030,0038
- Interrupts: 0040,0048,0050,0058,0060
- 
ただし、RSTコマンドや割込みを使用しないプログラムの場合は、他の用途に使用してもかまいません。RSTコマンドは1バイトのオペコードで、宛先アドレスが固定されている以外はCALLオペコードと同じような動作をします。1バイトの大きさなので、処理速度も若干速くなります。

# Cartridge Header in first ROM bank
0100-014Fのメモリには、カートリッジ・ヘッダが含まれています。このエリアには、プログラムに関する情報、そのエントリ・ポイント、チェックサム、使用するMBCチップに関する情報、ROMとRAMのサイズなどが含まれます。このエリアのほとんどのバイトは、正しく指定する必要があります。詳細については、「カートリッジヘッダ」の章をお読みください。

# External Memory and Hardware
0000-7FFFおよびA000-BFFFのエリアは、外部ハードウェアの接続に使用することができます。メモリバンクコントローラ（MBC）を搭載したカートリッジでは、さらにこのエリアを使用してMBCチップにデータを出力します（書き込みのみ）。2つ目の領域は、外部RAMやその他の外部ハードウェア（リアルタイムクロックなど）のアドレスとして使用されることが多い。外部メモリはバッテリーバッファリングされていることが多く、ゲームボーイの電源を切っても、カートリッジを外しても、保存されたゲームポジションやハイスコアテーブルなどを保持することができる。詳しくは「メモリーバンクコントローラー」の章をお読みください。

# Echo RAM
E000-FDFFは、WRAMのミラー（またはエコー）であり、読み出しと書き込みの両方が可能です。例えば、$E123に書き込むと、$C123と$E123の両方が変更されます。このメモリ範囲を使用することは、とにかく避けることをお勧めします。このメモリ範囲の動作は、CGBやGBAだけでなく、すべてのグレイGBで確認されています。一部のエミュレータ（VisualBoyAdvance <1.8など）では、Echo RAMがエミュレートされないことがあります。WRAMに書き込み（00とFFを避ける）、その値がEcho RAMに反映されるかどうかで、Echo RAMが正しくエミュレートされているかどうか確認することが可能です。

# FEA0-FEFF range
このレンジは、ドキュメントが非常に乏しいです。名前すらありません! 私の経験では、DMGでは00のままで、CGBでは00と一見ランダムな値の間で交互に変化します。
