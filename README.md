# 🛡️ 다목적 무인 전차 (Multipurpose Unmanned Tank)
> **IoT 기반 실시간 원격 관제, 헌터-킬러(Hunter-Killer) 연동 조준 및 4단 코일건 가속 무인 플랫폼**

![Project Status](https://img.shields.io/badge/Status-Completed-success)
![Role](https://img.shields.io/badge/Role-Hardware%20Lead-blue)
![Architecture](https://img.shields.io/badge/Architecture-Distributed%20MCU%20(Arduino%20+%20ESP8266)-orange)

중부대학교 스마트IT전공 캡스톤디자인(졸업작품)으로 개발된 다목적 원격 무인 전차 시스템입니다.  
위험 전장 및 재난 구조 현장에서 인명 피해를 방지하고 탐색·물자 수송·원격 타격을 수행할 수 있도록 설계되었습니다.  
웹 브라우저를 통한 실시간 듀얼 영상 관제, MQTT 비동기 패킷 통신, TFmini Plus LiDAR 거리 기반 고저각 산출, 전차장 독립 탐색(CPS) 및 포수 사격 연동 로직(Hunter-Killer)을 구현했습니다.

---

## 📌 핵심 요약 (Executive Summary)

- **개발 기간:** 2023.03.02 ~ 2023.06.19 (중부대학교 스마트IT프로젝트1)
- **수행 역할:** **하드웨어 총괄 (Hardware Lead)**
  - 전원 공급 분배망 설계, 메인 구동계/무장계 회로 설계 및 제작
  - 멀티 MCU(Arduino Uno × 3, WEMOS D1 R1/ESP8266 × 2) 분산 제어 및 UART 버스 배선
  - 포토트랜지스터 기반 코일건 단계별 가속 타이밍 펌웨어 구현
- **협업 구조:** 총 6인 (하드웨어 총괄 1, 하드웨어 제작 3, 소프트웨어 개발 1, 3D 디자인 1)

---

## 🛠️ 시스템 구성 및 하드웨어 스펙

### 1. 주요 부품 및 사양
| 구분 | 하드웨어 명칭 | 수량 | 역할 및 세부 규격 |
| :--- | :--- | :---: | :--- |
| **Main Compute** | **WEMOS D1 R1 (ESP8266)** | 2 | Wi-Fi 통신, MQTT 브로커 연동, 차체 기동/포탑 모듈 제어 |
| | **Arduino Uno** | 3 | 코일건 고속 점호, LiDAR 거리 파싱, 4축 서보 PWM 분산 제어 |
| **Vision & Sensor**| **ESP32-CAM** | 2 | 포수 시야(주포 동기화) & 전차장 시야(180° 회전 수색) 스트리밍 |
| | **TFmini Plus LiDAR** | 1 | 전방 타깃 거리 측정 (SoftwareSerial 통신, cm 단위 변환) |
| | **적외선 IRED & 포토트랜지스터** | 각 6 | 코일 구간별 탄자 감지 및 마이크로초(`micros()`) 탄속 측정 |
| **Actuator** | **DC 웜 기어드 모터** | 4 | 4륜 차체 구동 (강한 토크 기반 험지 주행) |
| | **L298N 모터 드라이버** | 1 | 듀얼 모터 방향 및 PWM 속도 제어 |
| | **서보모터** | 4 | 포탑 방위각, 주포 고저각, 전차장 조준경(CPS) 팬 제어 |
| | **솔레노이드 액추에이터** | 1 | 금속 서포트(탄자) 격발 챔버 인입 |
| **Power & Circuit**| **LUNAVOLT 리튬이온 배터리팩** | 2 | 12V 10Ah (3S2P) / 12V 30Ah (3S6P), 구동계 및 무장계 분리 공급 |
| | **IRF540 MOSFET 모듈** | 4 | 코일 4단 대전류 고속 스위칭 |

---

## 🧩 시스템 아키텍처 및 동작 메커니즘

### 1. 시스템 통신 데이터 흐름 (System Data Flow)
```text
[ Web Interface (HTML/JS) ]
     │  ▲
     │  │ MQTT Over WebSocket (Broker: broker.mqtt-dashboard.com:8088)
     ▼  │
[ WEMOS D1 R1 (ESP8266) ]
     │  ▲
     │  │ UART Serial Bus (115200 bps)
     ▼  │
[ Arduino Uno Cluster ] ───> [ DC Motors / 4x Servos / TFmini LiDAR / 4-Stage Coilgun ]

> 📄 **상세 결과보고서:** 저장소 내 [`2023 다목적 무인 전차 프로젝트.pdf`](./2023%20다목적%20무인%20전차%20프로젝트.pdf) 파일에서 전체 개발 보고서 전문을 확인하실 수 있습니다.
