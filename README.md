<a href="https://club-project-one.vercel.app/" target="_blank">
<img src="https://github.com/user-attachments/assets/2368e03a-a5c1-4267-a486-616542e8f06e" alt="배너" width="100%"/>
</a>

<br/>
<br/>

# 1. Project Overview (프로젝트 개요)
- 프로젝트 이름: 딸깍
- 프로젝트 설명: MQTT 통신을 기반으로 한 스마트 홈 시스템입니다.
- 어플리케이션 리포지토리: [딸깍-앱](https://github.com/pknu-wap/2025_1_Embedded1_TTALKKAG)

<br/>
<br/>

# 2. Team Members (팀원 및 팀 소개)
| 김동건 | 강유경 | 장혜림 | 홍우민 | 고근호 | 김예람 |
|:------:|:------:|:------:|:------:|:------:|:------:|
| <img src="https://avatars.githubusercontent.com/u/125544913?v=4" alt="김동건" width="150"> | <img src="https://avatars.githubusercontent.com/u/202641007?v=4" alt="강유경" width="150"> | <img src="https://avatars.githubusercontent.com/u/202640692?v=4" alt="장혜림" width="150"> | <img src="https://avatars.githubusercontent.com/u/155062635?v=4" alt="홍우민" width="150"> | <img src="https://avatars.githubusercontent.com/u/95072015?v=4" alt="고근호" width="150"> | <img src="https://avatars.githubusercontent.com/u/202640579?v=4" alt="김예람" width="150"> |
| Arduino, Raspberry Pi | Arduino, Raspberry Pi | RN | RN | Spring | Design |
| [GitHub](https://github.com/Danny-Caesar) | [GitHub](https://github.com/dbrud919) | [GitHub](https://github.com/hyerim819) | [GitHub](https://github.com/hongwoomin02) | [GitHub](https://github.com/kokeunho) | [GitHub](https://github.com/rlaexram) |

<br/>
<br/>

# 3. Key Features (주요 기능)
- **MQTT**:
  - MQTT(Message Queuing Telemetry Transport)는 ISO 표준 발행-구독 기반의 메시징 프로토콜입니다.
  - IoT 환경에서 오픈소스 및 라이브러리 없이 MQTT 네트워크를 구현하는 것을 목표하고 있습니다.
  - 앱 BE에서 자바 스프링을 사용해 허브와 MQTT 통신을 수행합니다.

- **클라이언트**:
  - 조작에 따라 버튼을 누르는 버튼 클리커
  - 단계별로 다이얼을 회전하는 다이얼 액추에이터
  - 문 열림을 감지하는 개폐감지센서

- **클라이언트 관리**:
  - 한 화면에서 관리 중인 클라이언트를 확인할 수 있습니다.
  - 실시간으로 브로커에 연결된 클라이언트가 추가/해제 됩니다.

- **클라이언트 조작 및 설정**:
  - 각 클라이언트의 개별 페이지는 클라이언트의 정보를 표시하거나 변경할 수 있습니다.
  - 클라이언트를 조작하고 설정할 수 있습니다.

<br/>
<br/>

# 4. Tasks & Responsibilities (작업 및 역할 분담)
|  |  |  |
|-----------------|-----------------|-----------------|
| 김동건   |  <img src="https://avatars.githubusercontent.com/u/125544913?v=4" alt="" width="100">| <ul><li>프로젝트 계획 및 관리</li><li>MQTT 브로커 개발</li><li>다이얼 액추에이터 개발</li></ul> |
| 강유경   |  <img src="https://avatars.githubusercontent.com/u/202641007?v=4" alt="" width="100">| <ul><li>버튼 클리커 개발</li><li>개폐 감지 센서 개발</li></ul> |
| 장혜림   |  <img src="https://avatars.githubusercontent.com/u/202640692?v=4" alt="" width="100">| <ul><li>디바이스 목록 페이지 개발</li></ul> |
| 홍우민   |  <img src="https://avatars.githubusercontent.com/u/155062635?v=4" alt="" width="100">| <ul><li>트리거 목록 페이지 개발</li><li>디바이스 상세 페이지 개발</li></ul> |
| 고근호   |  <img src="https://avatars.githubusercontent.com/u/95072015?v=4" alt="" width="100">| <ul><li>회원가입 페이지 개발</li><li>마이 프로필 페이지 개발</li></ul> |
| 김예람   |  <img src="https://avatars.githubusercontent.com/u/202640579?v=4" alt="" width="100">| <ul><li>앱 페이지 전반 디자인</li><li>로고 및 배너 디자인</li></ul> |

<br/>
<br/>

# 5. Technology Stack (기술 스택)
## 5.1 Development
|  |  |
|-----------------|-----------------|
| Arduino    |  <img src="" alt="Arduino" width="100"> | 
| Raspberry Pi    |  <img src="" alt="RPi" width="100"> | 
| React Native    |<img src="" alt="RN" width="100">| 
| Spring    |   <img src="" alt="Spring" width="100">|

<br/>

## 5.2 Cooperation
|  |  |
|-----------------|-----------------|
| Git    |  <img src="https://github.com/user-attachments/assets/483abc38-ed4d-487c-b43a-3963b33430e6" alt="git" width="100">    |
| Notion    |  <img src="https://github.com/user-attachments/assets/34141eb9-deca-416a-a83f-ff9543cc2f9a" alt="Notion" width="100">    |
| Figma    |  <img src="" alt="Figma" width="100">    |

<br/>

# 6. Project Structure (프로젝트 구조)
```plaintext
project/
├── broker/
├── client/
|   ├── subscriber/
│   └── publisher/
├── .gitignore               # Git 무시 파일 목록
└── README.md                # 프로젝트 개요 및 사용법
```

<!--
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

<!--
<br/>
<br/>

# 7. Development Workflow (개발 워크플로우)
## 브랜치 전략 (Branch Strategy)
우리의 브랜치 전략은 Git Flow를 기반으로 하며, 다음과 같은 브랜치를 사용합니다.

- Main Branch
  - 배포 가능한 상태의 코드를 유지합니다.
  - 모든 배포는 이 브랜치에서 이루어집니다.
  
- {name} Branch
  - 팀원 각자의 개발 브랜치입니다.
  - 모든 기능 개발은 이 브랜치에서 이루어집니다.

<br/>
<br/>

# 8. Coding Convention
## 문장 종료
```
// 세미콜론(;)
console.log("Hello World!");
```

<br/>


## 명명 규칙
* 상수 : 영문 대문자 + 스네이크 케이스
```
const NAME_ROLE;
```
* 변수 & 함수 : 카멜케이스
```
// state
const [isLoading, setIsLoading] = useState(false);
const [isLoggedIn, setIsLoggedIn] = useState(false);
const [errorMessage, setErrorMessage] = useState('');
const [currentUser, setCurrentUser] = useState(null);

// 배열 - 복수형 이름 사용
const datas = [];

// 정규표현식: 'r'로 시작
const = rName = /.*/;

// 이벤트 핸들러: 'on'으로 시작
const onClick = () => {};
const onChange = () => {};

// 반환 값이 불린인 경우: 'is'로 시작
const isLoading = false;

// Fetch함수: method(get, post, put, del)로 시작
const getEnginList = () => {...}
```

<br/>

## 블록 구문
```
// 한 줄짜리 블록일 경우라도 {}를 생략하지 않고, 명확히 줄 바꿈 하여 사용한다
// good
if(true){
  return 'hello'
}

// bad
if(true) return 'hello'
```

<br/>

## 함수
```
함수는 함수 표현식을 사용하며, 화살표 함수를 사용한다.
// Good
const fnName = () => {};

// Bad
function fnName() {};
```

<br/>

## 태그 네이밍
Styled-component태그 생성 시 아래 네이밍 규칙을 준수하여 의미 전달을 명확하게 한다.<br/>
태그명이 길어지더라도 의미 전달의 명확성에 목적을 두어 작성한다.<br/>
전체 영역: Container<br/>
영역의 묶음: {Name}Area<br/>
의미없는 태그: <><br/>
```
<Container>
  <ContentsArea>
    <Contents>...</Contents>
    <Contents>...</Contents>
  </ContentsArea>
</Container>
```

<br/>

## 폴더 네이밍
카멜 케이스를 기본으로 하며, 컴포넌트 폴더일 경우에만 파스칼 케이스로 사용한다.
```
// 카멜 케이스
camelCase
// 파스칼 케이스
PascalCase
```

<br/>

## 파일 네이밍
```
컴포넌트일 경우만 .jsx 확장자를 사용한다. (그 외에는 .js)
customHook을 사용하는 경우 : use + 함수명
```

<br/>
<br/>

# 9. 커밋 컨벤션
## 기본 구조
```
type : subject

body 
```

<br/>

## type 종류
```
feat : 새로운 기능 추가
fix : 버그 수정
docs : 문서 수정
style : 코드 포맷팅, 세미콜론 누락, 코드 변경이 없는 경우
refactor : 코드 리펙토링
test : 테스트 코드, 리펙토링 테스트 코드 추가
chore : 빌드 업무 수정, 패키지 매니저 수정
```

<br/>

## 커밋 이모지
```
== 코드 관련
📝	코드 작성
🔥	코드 제거
🔨	코드 리팩토링
💄	UI / style 변경

== 문서&파일
📰	새 파일 생성
🔥	파일 제거
📚	문서 작성

== 버그
🐛	버그 리포트
🚑	버그를 고칠 때

== 기타
🐎	성능 향상
✨	새로운 기능 구현
💡	새로운 아이디어
🚀	배포
```

<br/>

## 커밋 예시
```
== ex1
✨Feat: "회원 가입 기능 구현"

SMS, 이메일 중복확인 API 개발

== ex2
📚chore: styled-components 라이브러리 설치

UI개발을 위한 라이브러리 styled-components 설치
```

<br/>
<br/>

# 10. 컨벤션 수행 결과
<img width="100%" alt="코드 컨벤션" src="https://github.com/user-attachments/assets/0dc218c0-369f-45d2-8c6d-cdedc81169b4">
<img width="100%" alt="깃플로우" src="https://github.com/user-attachments/assets/2a4d1332-acc2-4292-9815-d122f5aea77c">
