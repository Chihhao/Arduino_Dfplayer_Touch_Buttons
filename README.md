# Arduino_Dfplayer_Touch_Buttons (觸碰按鈕MP3播放器)  
## 使用 Dfplayer MP3播放模組  
![image](https://github.com/Chihhao/Arduino_Dfplayer_Touch_Buttons/blob/main/image/4.jpg)  
## 成品  
![image](https://github.com/Chihhao/Arduino_Dfplayer_Touch_Buttons/blob/main/image/1.jpg)  
## 材料  
(1) Arduino Nano x 1  
(2) Dfplayer MP3播放模組 x 1  
(3) 觸碰介面(這裡用螺絲) x 8  
(4) A331J排阻 x 1  
(5) LED x 8  
(6) 1M歐姆 x 8  
(7) 74HC595 IC x 1  
(8) 1K電阻 x 2  
(9) 鋰電池充電板(附帶升壓功能) x 1  
(10) 船型開關 x 1  
(11) 1000uF電解電容 x 1  
(12) micro usb 轉接板 x 1  
(13) 18650 鋰電池 x 1  
(14) 2歐姆喇叭 x 1  
(15) 外盒  
## 函式庫 (已內含)  
(1) https://github.com/DFRobot/DFRobotDFPlayerMini  
(2) https://github.com/PaulStoffregen/CapacitiveSensor  
## 說明  
(1)  前6顆按鈕，按下#1播放第1首，按下#2播放第2首，按下#1+#2播放第3首，按下#3播放第4首，以此類推  
(2) 最多到第64首。  
(3) #7按鈕: 小聲  
(4) #8按鈕: 大聲  
(5) mp3檔案放在SD卡中，必須按以下格式命名: 0001_歌名.mp3, 0002_歌名.mp3, ...  
## Wiring  
![image](https://github.com/Chihhao/Arduino_Dfplayer_Touch_Buttons/blob/main/image/wiring.png)  
