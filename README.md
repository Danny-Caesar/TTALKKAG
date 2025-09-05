# 딸깍 TTALKKAG
![readme_mockup2](https://github.com/user-attachments/assets/e16f3c66-62b0-4fd4-a7bd-81e4dcfde3ab)

- 어플리케이션 리포지토리: https://github.com/pknu-wap/2025_1_Embedded1_TTALKKAG

<br/>
<br/>

## 프로젝트 소개
- '딸깍'은 일상 속의 편리함을 높이고 싶은 사람들을 위한 스마트 홈 시스템 입니다.
- ISO 표준 통신 프로토콜인 MQTT와 시스템 전반을 조작·관리하는 앱을 활용해 맞춤형 사용자 경험을 제공합니다.

<br/>
<br/>

## 프로젝트 특징
- **MQTT**:
  - MQTT(Message Queuing Telemetry Transport)는 ISO 표준 발행-구독 기반의 메시징 프로토콜입니다.
  - MQTT 오픈소스 및 라이브러리 없이 MQTT 브로커를 구현하였습니다.

- **클라이언트**:

  ![Image](https://github.com/user-attachments/assets/0cdc400e-97e5-4343-93ea-e5434ffc734e)
  ![Image](https://github.com/user-attachments/assets/3d702c27-f9b5-47f8-9746-85bfd0f460a3)

  - MCU(마이크로 컨트롤러 유닛)을 사용해 간단한 통신과 동작을 사용하는 클라이언트를 구성했습니다.
  - 클라이언트는 트리거/디바이스로 두 종류가 있으며 서로 브로커를 통해 통신합니다.

- **어플리케이션**:

  ![Image](https://github.com/user-attachments/assets/16c25fab-ee06-4821-88f7-5831c9f065fd)
  ![Image](https://github.com/user-attachments/assets/4e68b147-dfbc-43f2-972f-52b7ab449ac3)
  
  
  ![Image](https://github.com/user-attachments/assets/7f2eb74d-e740-48d5-b59f-980559053431)
  ![Image](https://github.com/user-attachments/assets/09fe8428-8210-4782-976e-d9f12c661e14)

  - 스마트 홈에 연결된 디바이스를 사용자가 직접 조작할 수 있습니다.
  - 여러 종류의 트리거를 관리하고, 연결된 디바이스의 동작을 구성할 수 있습니다.

<br/>
<br/>

## 팀 구성
| **김동건** | **강유경** | **장혜림** | **홍우민** | **고근호** | **김예람** |
|:------:|:------:|:------:|:------:|:------:|:------:|
| <img src="https://avatars.githubusercontent.com/u/125544913?v=4" alt="김동건" width="125"> | <img src="https://avatars.githubusercontent.com/u/202641007?v=4" alt="강유경" width="125"> | <img src="https://avatars.githubusercontent.com/u/202640692?v=4" alt="장혜림" width="125"> | <img src="https://avatars.githubusercontent.com/u/155062635?v=4" alt="홍우민" width="125"> | <img src="https://avatars.githubusercontent.com/u/95072015?v=4" alt="고근호" width="125"> | <img src="https://avatars.githubusercontent.com/u/202640579?v=4" alt="김예람" width="125"> |
| [@Danny-Caesar](https://github.com/Danny-Caesar) | [@dbrud919](https://github.com/dbrud919) | [@hyerim819](https://github.com/hyerim819) | [@hongwoomin02](https://github.com/hongwoomin02) | [@kokeunho](https://github.com/kokeunho) | [@rlaexram](https://github.com/rlaexram) |

<br/>
<br/>

## 개발 환경
|  |  |
|-----------------|-----------------|
| Frontend    | ![React Native](https://img.shields.io/badge/react_native-%2320232a.svg?style=for-the-badge&logo=react&logoColor=%2361DAFB) ![JavaScript](https://img.shields.io/badge/javascript-%23323330.svg?style=for-the-badge&logo=javascript&logoColor=%23F7DF1E) | 
| Backend    |  ![Spring](https://img.shields.io/badge/spring-%236DB33F.svg?style=for-the-badge&logo=spring&logoColor=white) ![Java](https://img.shields.io/badge/java-%23ED8B00.svg?style=for-the-badge&logo=openjdk&logoColor=white)| 
| Embedded System    | ![Raspberry Pi](https://img.shields.io/badge/-Raspberry_Pi-C51A4A?style=for-the-badge&logo=Raspberry-Pi) ![Debian](https://img.shields.io/badge/Debian-D70A53?style=for-the-badge&logo=debian&logoColor=white) ![Espressif](https://img.shields.io/badge/espressif-E7352C.svg?style=for-the-badge&logo=espressif&logoColor=white) ![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)| 
| Hosting    | ![AWS](https://img.shields.io/badge/AWS-%23FF9900.svg?style=for-the-badge&logo=amazon-aws&logoColor=white) |

<br/>
<br/>

## 브로커 구조
broker/  
├── broker.cpp  
├── logo_display.cpp  
│  
├── packet/                # mqtt 컨트롤 패킷, 패킷 파서, 패킷 핸들러  
│  
├── session/               # 세션, 세션 매니저  
│  
├── socket/                # 소켓  
│  
└── subscription/          # 구독, 구독 매니저  

<br/>
<br/>

<p align="center">
  <img src="https://github.com/user-attachments/assets/7619f462-04b2-4ec2-8bdf-cbf1409a222c" alt="배너" width="80%" horizontal_align="center"/>
</p>
