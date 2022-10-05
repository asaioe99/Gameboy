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

CSpinがあればSPI_MODE0で動くかも知れない。要実験

(1.54インチの製品はCSあり)

ひょっとしたら、CSがないため動作させられない可能性。他のSPIデバイスにアクセスしている間にどういう動作をするか不明

### SRAM
- https://ameblo.jp/pi-poh/entry-12667969903.html　（プルアップについて）

SPI＿MODE0で動作。液晶はMODE2なのでダメか
