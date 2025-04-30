<a href="https://club-project-one.vercel.app/" target="_blank">
<img src="https://github.com/user-attachments/assets/2368e03a-a5c1-4267-a486-616542e8f06e" alt="배너" width="100%"/>
</a>

<br/>
<br/>

# 딸깍 TTALKKAG
더 똑똑하게 살고 싶은 당신을 위해

[딸깍-어플리케이션 리포지토리](https://github.com/pknu-wap/2025_1_Embedded1_TTALKKAG)

<br/>
<br/>

# Team Members (팀원)
| 김동건 | 강유경 | 장혜림 | 홍우민 | 고근호 | 김예람 |
|:------:|:------:|:------:|:------:|:------:|:------:|
| <img src="https://avatars.githubusercontent.com/u/125544913?v=4" alt="김동건" width="150"> | <img src="https://avatars.githubusercontent.com/u/202641007?v=4" alt="강유경" width="150"> | <img src="https://avatars.githubusercontent.com/u/202640692?v=4" alt="장혜림" width="150"> | <img src="https://avatars.githubusercontent.com/u/155062635?v=4" alt="홍우민" width="150"> | <img src="https://avatars.githubusercontent.com/u/95072015?v=4" alt="고근호" width="150"> | <img src="https://avatars.githubusercontent.com/u/202640579?v=4" alt="김예람" width="150"> |
| Raspberry Pi | Arduino | RN | RN | Spring | Design |
| [GitHub](https://github.com/Danny-Caesar) | [GitHub](https://github.com/dbrud919) | [GitHub](https://github.com/hyerim819) | [GitHub](https://github.com/hongwoomin02) | [GitHub](https://github.com/kokeunho) | [GitHub](https://github.com/rlaexram) |

<br/>
<br/>

# Key Features (주요 기능)
- **MQTT**:
  - MQTT(Message Queuing Telemetry Transport)는 ISO 표준 발행-구독 기반의 메시징 프로토콜입니다.
  - MQTT 오픈소스 및 라이브러리 없이 MQTT 브로커를 구현했습니다.
  - 브로커를 중심으로 디바이스(버튼 클리커, 다이얼 액추에이터)와 트리거(개폐감지센서, 스마트폰 앱) 간 통신을 수행합니다.

- **클라이언트**:
  - 조작에 따라 버튼을 누르는 버튼 클리커
  - 단계별로 다이얼을 회전하는 다이얼 액추에이터
  - 문 열림을 감지하는 개폐감지센서

- **어플리케이션**:
  - 시스템에 연결된 클라이언트를 연결하고 관리합니다.
  - 각 디바이스를 어플리케이션에서 설정하고 조작합니다.
  - 각 트리거를 기준으로 연동할 디바이스를 설정합니다.

<br/>
<br/>

# Technology Stack (기술 스택)
|  |  |
|-----------------|-----------------|
| Arduino    |  ![Arduino](https://img.shields.io/badge/-Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white) | 
| Raspberry Pi    |  ![Raspberry Pi](https://img.shields.io/badge/-Raspberry_Pi-C51A4A?style=for-the-badge&logo=Raspberry-Pi) | 
| React Native    | ![React Native](https://img.shields.io/badge/react_native-%2320232a.svg?style=for-the-badge&logo=react&logoColor=%2361DAFB) | 
| Spring    | ![Spring](https://img.shields.io/badge/spring-%236DB33F.svg?style=for-the-badge&logo=spring&logoColor=white) |

<br/>
<br/>

<!-- # 6. Project Structure (프로젝트 구조)
```plaintext
project/
├── client/
├── server/
├── .gitignore               # Git 무시 파일 목록
└── README.md                # 프로젝트 개요 및 사용법

project/
├── public/
│   ├── index.html           # HTML 템플릿 파일
│   └── favicon.ico          # 아이콘 파일
├── src/
│   ├── assets/              # 이미지, 폰트 등 정적 파일
│   ├── components/          # 재사용 가능한 UI 컴포넌트
│   ├── hooks/               # 커스텀 훅 모음
│   ├── pages/               # 각 페이지별 컴포넌트
│   ├── App.js               # 메인 애플리케이션 컴포넌트
│   ├── index.js             # 엔트리 포인트 파일
│   ├── index.css            # 전역 css 파일
│   ├── firebaseConfig.js    # firebase 인스턴스 초기화 파일
│   package-lock.json    # 정확한 종속성 버전이 기록된 파일로, 일관된 빌드를 보장
│   package.json         # 프로젝트 종속성 및 스크립트 정의
├── .gitignore               # Git 무시 파일 목록
└── README.md                # 프로젝트 개요 및 사용법
--!>
