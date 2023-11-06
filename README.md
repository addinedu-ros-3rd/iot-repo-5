# Self Balancing Robot

<img src="https://img.shields.io/badge/Github-181717?style=for-the-badge&logo=Github&logoColor=white"> <img src="https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=Python&logoColor=white"> <img src="https://img.shields.io/badge/Arduino-00878F?style=for-the-badge&logo=Arduino&logoColor=white"> <img src="https://img.shields.io/badge/qt-41CD52?style=for-the-badge&logo=qt&logoColor=white"> <img src="https://img.shields.io/badge/mysql-4479A1?style=for-the-badge&logo=mysql&logoColor=white">

---

## Index

- [What is Self Balancing Robot](#what-is-self-balancing-robot)
- [Purpose of Project](#purpose-of-project)
- [Showcase](#showcase)
- [Hardware](#hardware)
- [Body Design](#body-design)
- [How To Use](#how-to-use)
    - [Installation](#installation)
    - [Pin Settings](#pin-settings)
    - [Program Settings](#program-settings)
- [If you want to PID Controll (Only PID Motor Controll) - ESP32 Nano](#if-you-want-to-pid-controll-only-pid-motor-controll---esp32-nano)
    - [Pin Settings](#pin-settings-1)
    - [Program Settings](#program-settings-1)
- [SetBack & Improvements & Wrap Up](#setback--improvements--wrap-up)
    - [SetBack](#setback)
    - [Improvements](#improvements)
    - [Wrap Up](#wrap-up)
- [Acknowledgements](#acknowledgements)
---

## What is Self Balancing Robot
> __Self Balancing Robot__ is a robot that stands on its own like __Segway__.
This Robot requires __PID motor control__ and __Gyro sensor__, and we used __L298N__ and __MPU6050__.

---

## Author
|역할|문서영|조홍기|
|---|---|---|
||DC모터 제어|PyQt 프로그램 제작|
||하드웨어 설계 및 전장|RFID 센싱|
||MPU6050 센싱|ESP32 (유/무선) 통신|
||DB 관련 작업|UNO-ESP32 시리얼통신|

---

## Purpose of Project
1. Deal with ```Acceleration, Gyroscope``` sensor like ```MPU6050``` and input these values on control process.
2. Study PID and apply it to control Robot.
3. Control the MCU wirelessly.
4. Get familiarized with other MCUs than Arduino UNO

---

## Showcase
1. Wireless Data Communication using __Wifi AP__
2. __PID control__
3. __Access MPU6050 registries__ to get Acceleration and Gyroscope values
4. Sense __RFID__ tags
5. Remote Control using __QT__
6. Log Sensor Data into __MySQL__
---

## Hardware
- MCU   :   Espressif ESP32-S3 N16R8, Arduino Uno
- Gyro Sensor   :   GY-521 (MPU 6050)
- Motors (x2)   :   NP01D-288
- Motor Driver  :   L298N
- Wheels    :   $\phi$ 66mm
- Battery   :   1.5V * 4
- RFID      :   MFRC522

---

## Body Design
![Alt text](Body_design.png)

---

## How To Use
### Installation
1. ```Arduino > Tools > Board > Boards Manager > esp32 (Espressif)```
2. ```Arduino > Tools > Library Manager > MFRC522```

### Pin Settings
- ESP32-S3
```
TX 17, RX 18

--------------------------
MPU6050

SCL 1
SDA 2
INTERRUPT   14

--------------------------
L298N

IN1 9
IN2 8
IN4 5
IN3 6
ENA 10  // Right Side Motor
ENB 4  // Left Side Motor

```
- UNO
```
RX 0
TX 1

--------------------------
MRFC522

RST_PIN     9     
SS_PIN      10
```

### Program Settings
1. Build ```uno_final/uno_final.ino``` into Arduino Uno
2. Build ```esp32_final/esp32_final.ino``` into ESP32-S3  
    2-1. Connect ESP32-S3 with Serial and Press Reset to find out WIFI IP  
    2-2. Open ```main.py``` and fix IP on line 20  
3. Run ```main.py```

---

## If you want to PID Controll (Only PID Motor Controll) - ESP32 Nano

### Pin Settings
- ESP32 Nano
![](<Balanced Robot.png>)

### Program Settings
1. Build ```Balanced_Robot/Balanced_Robot.ino``` into ESP32 Nano

---

## SetBack & Improvements & Wrap Up

### SetBack
1. esp32-s3와 esp32 nano의 호환성 문제로 코드 통합 실패
2. 납땜 잘못해서 mpu센서 태워먹고 납땜 안하다가 오히려 삽질 더 함
3. socket 통신 이론 부족으로 인한 패킷 드랍 현상
4. PID 제어 미흡
5. 
6. 시나리오 미흡
7. 하드웨어 구성이 기능 구현에 무리가 없는지 사전 확인이 부족 (센서 사용법, 전장 설계 등등)


### Improvements
1. QT에서 모니터링 그래프 구현
2. QT 키로거 DB 생성 안함
3. 코드 통합
4. 통신 제대로 구현


### Wrap Up
1. 모터를 PID tuning을 통해 제어하려면 많은 배경지식이 필요하다 절실히 느낌
2. 다음에는 물성을 수식화된 시스템으로  PID tuning을 해보고 싶음
3. 하드웨어의 특성과 한계를 폭 넓게 이해하기
4. 여러 통신 기술에 대해 얇고 넓게 알게됨
5. MCU와의 통신에서 어떤 공부를 해야할지 감이 옴.
6. ESP32-S3와 같은 생소한 MCU를 다룰 때 제품 설명서를 어떻게 읽고 사용해야할지 조금 이해함.
7. 납땜이 매우매우매우매우매우매우 중요했음. 지금은 개인이 쓸 정도는 할 수 있게 되었음.
---

## Acknowledgements
- [Espressif - ESP32](https://github.com/espressif/arduino-esp32)
- [PyQt5 Docs](https://doc.qt.io/qtforpython-5/PySide2/QtWidgets/index.html)
- [I2CDev - MPU6050](https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050)