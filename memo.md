# 参考資料一覧

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



### ST7789(pico LCD 1.3)
四の五の言わずに以下を読め
- https://github.com/adafruit/Adafruit-ST7735-Library


### エミュレータの実装例
- https://github.com/keichi/gbr

### CPU opcode一覧
- https://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html
- https://github.com/pokemium/gb-docs-ja


https://github.com/adafruit/Adafruit-ST7735-Library/blob/master/examples/graphicstest_st7789/graphicstest_st7789.ino
