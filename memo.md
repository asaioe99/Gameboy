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
  - https://www.waveshare.com/wiki/Pico-LCD-1.3 (上記wiki)

CSpinがあればSPI_MODE0で動くかも知れない。要実験

(1.54インチの製品はCSあり)

ひょっとしたら、CSがないため動作させられない可能性。他のSPIデバイスにアクセスしている間にどういう動作をするか不明

### SRAM
- https://ameblo.jp/pi-poh/entry-12667969903.html　（プルアップについて）

SPI＿MODE0で動作。液晶はMODE2なのでダメか
