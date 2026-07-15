# DataPersistence-JOYUSIK-21044893

**PoC2 — JSON 영속성 + CRUD 검증**

S-Semi 시료 생산주문관리 시스템의 DataPersistence PoC입니다.  
본 프로젝트(`SampleOrderSystem`)와 동일한 `Sample` / `Order` 스키마를 사용하여  
JSON 파일 기반 CRUD 및 앱 재시작 후 데이터 유지(영속성)를 검증합니다.

---

## 환경

| 항목 | 내용 |
|---|---|
| OS | Windows 11 Pro |
| IDE | Visual Studio 2025 Preview (v18) |
| 언어 | C++17 |
| JSON | nlohmann/json 3.11.3 (third_party/json.hpp) |
| 빌드 | MSBuild (vcxproj / slnx) |

---

## 프로젝트 구조

```
DataPersistence/
├── DataPersistence.slnx        ← VS 솔루션 (더블클릭으로 열기)
├── DataPersistence.vcxproj
├── main.cpp                    ← 콘솔 CRUD 메뉴 진입점
├── model/
│   ├── Sample.h / Sample.cpp   ← 시료 데이터 구조 + JSON 직렬화
│   └── Order.h  / Order.cpp    ← 주문 데이터 구조 + JSON 직렬화
├── repository/
│   ├── SampleRepository.h/.cpp ← samples.json CRUD
│   └── OrderRepository.h/.cpp  ← orders.json  CRUD
├── third_party/
│   └── json.hpp                ← nlohmann/json 3.11.3
└── data/
    ├── samples.json            ← 런타임 생성 (git 제외)
    └── orders.json             ← 런타임 생성 (git 제외)
```

---

## 빌드 및 실행

### Visual Studio (권장)

1. `DataPersistence\DataPersistence.slnx` 더블클릭
2. 상단 구성: `Release | x64`
3. **Ctrl+Shift+B** → 빌드
4. **F5** → 실행 (작업 디렉터리 자동으로 `DataPersistence\` 설정)

> ⚠️ F5로 실행해야 `data/` 경로가 올바르게 잡힙니다.  
> 빌드 결과물: `DataPersistence\x64\Release\DataPersistence.exe`

### 터미널

```powershell
$msbuild = "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe"
& $msbuild DataPersistence\DataPersistence.vcxproj /p:Configuration=Release /p:Platform=x64

# 프로젝트 폴더에서 실행 (data/ 경로 기준)
cd DataPersistence
.\x64\Release\DataPersistence.exe
```

---

## 도메인 모델

### Sample (시료)

```json
{
  "id": "S-001",
  "name": "실리콘 웨이퍼-8인치",
  "avg_production_time": 0.5,
  "yield_rate": 0.92,
  "stock": 480
}
```

### Order (주문)

```json
{
  "order_id": "ORD-20260715-0001",
  "sample_id": "S-001",
  "customer": "삼성전자 파운드리",
  "quantity": 200,
  "status": "RESERVED",
  "created_at": "2026-07-15T09:32:15"
}
```

---

## Repository 인터페이스

```cpp
// SampleRepository
std::vector<Sample>      findAll()
std::optional<Sample>    findById(const std::string& id)
Sample                   save(Sample sample)      // ID 자동 생성 (S-NNN)
bool                     update(const Sample& sample)
bool                     remove(const std::string& id)

// OrderRepository
std::vector<Order>       findAll()
std::optional<Order>     findById(const std::string& orderId)
Order                    save(Order order)         // ID 자동 생성 (ORD-YYYYMMDD-NNNN)
bool                     update(const Order& order)
bool                     remove(const std::string& orderId)
```

---

## 입출력 케이스

### CASE 1 — 시료 등록 `save()`

**입력**
```
메인 메뉴 > 1   (시료 관리)
서브 메뉴 > 3   (등록)
이름      > 실리콘 웨이퍼-8인치
평균생산시간(min) > 0.5
수율 (0~1)       > 0.92
초기재고         > 480
```

**출력**
```
 [완료] 시료가 등록되었습니다. (ID: S-001)
```

**생성된 data/samples.json**
```json
[
  {
    "avg_production_time": 0.5,
    "id": "S-001",
    "name": "실리콘 웨이퍼-8인치",
    "stock": 480,
    "yield_rate": 0.92
  }
]
```

---

### CASE 2 — 전체조회 `findAll()`

**입력**
```
메인 메뉴 > 1   (시료 관리)
서브 메뉴 > 1   (전체조회)
```

**출력**
```
============================================================
 시료 목록 (총 2개)
============================================================
 ID      이름              재고  수율  생산시간(min)
------------------------------------------------------------
 S-001   실리콘 웨이퍼-8인치480     0.92    0.5
 S-002   GaN 에피 웨이퍼   120     0.85    1.2
```

---

### CASE 3a — ID 조회 성공 `findById()` ✅

**입력**
```
메인 메뉴 > 1
서브 메뉴 > 2   (ID조회)
시료 ID   > S-001
```

**출력**
```
============================================================
 시료 상세
============================================================
 ID           : S-001
 이름         : 실리콘 웨이퍼-8인치
 재고         : 480
 수율         : 0.92
 평균생산시간 : 0.5 min
```

---

### CASE 3b — ID 조회 실패 `findById()` — 없는 ID ❌

**입력**
```
메인 메뉴 > 1
서브 메뉴 > 2
시료 ID   > S-999
```

**출력**
```
 [오류] 존재하지 않는 ID입니다: S-999
```

---

### CASE 4 — 재고 수정 `update()`

**입력**
```
메인 메뉴 > 1
서브 메뉴 > 4   (수정)
수정할 ID > S-001
새 재고   > 300
```

**출력**
```
 현재 재고: 480
 [완료] 재고가 수정되었습니다.
```

**수정 후 data/samples.json**
```json
{ "id": "S-001", "stock": 300, ... }
```

---

### CASE 5 — 영속성: 앱 재시작 후 데이터 유지 ✅

앱 종료 후 재시작 → 전체조회 시 이전 수정 상태(재고 300) 그대로 조회됨.

```
 S-001   실리콘 웨이퍼-8인치300     0.92    0.5
 S-002   GaN 에피 웨이퍼   120     0.85    1.2
```

> JSON 파일에 저장되므로 프로세스 종료 후에도 데이터 유지.

---

### CASE 6 — 주문 등록 + `ORD-YYYYMMDD-NNNN` 형식 검증

**입력**
```
메인 메뉴 > 2   (주문 관리)
서브 메뉴 > 3   (등록)
시료 ID   > S-001
고객명    > 삼성전자 파운드리
수량      > 200
```

**출력**
```
 [완료] 주문이 등록되었습니다. (주문ID: ORD-20260715-0001)
```

동일 날짜에 두 번째 등록 시:
```
 [완료] 주문이 등록되었습니다. (주문ID: ORD-20260715-0002)
```

**생성된 data/orders.json**
```json
[
  {
    "created_at": "2026-07-15T09:32:15",
    "customer": "삼성전자 파운드리",
    "order_id": "ORD-20260715-0001",
    "quantity": 200,
    "sample_id": "S-001",
    "status": "RESERVED"
  }
]
```

---

### CASE 7 — 주문 상태변경 `update()` RESERVED → CONFIRMED

**입력**
```
메인 메뉴 > 2
서브 메뉴 > 4   (수정)
주문 ID   > ORD-20260715-0001
새 상태   > CONFIRMED
```

**출력**
```
 현재 상태: RESERVED
 [완료] 상태가 변경되었습니다.
```

**변경 후 orders.json**
```json
{ "order_id": "ORD-20260715-0001", "status": "CONFIRMED", ... }
```

> 지원 상태: `RESERVED` / `CONFIRMED` / `PRODUCING` / `RELEASE` / `REJECTED`

---

### CASE 8 — 시료 삭제 `remove()` 후 빈 목록 확인

**입력**
```
메인 메뉴 > 1
서브 메뉴 > 5   (삭제)
삭제할 ID > S-001
(반복 후)
서브 메뉴 > 1   (전체조회)
```

**출력**
```
 [완료] 삭제되었습니다.
...
 시료 목록 (총 0개)
 등록된 시료가 없습니다.
```

---

## 본 프로젝트와의 연관

| 항목 | DataPersistence PoC | SampleOrderSystem |
|---|---|---|
| Sample 스키마 | 동일 (`id`, `name`, `avg_production_time`, `yield_rate`, `stock`) | 동일 |
| Order 스키마 | 동일 (`order_id`, `sample_id`, `customer`, `quantity`, `status`, `created_at`) | 동일 |
| Repository 패턴 | `findAll` / `findById` / `save` / `update` / `remove` | 동일 인터페이스 |
| JSON 파일 경로 | `data/samples.json`, `data/orders.json` | 동일 |

> DataMonitor(PoC3), DummyDataGenerator(PoC4)는 본 프로젝트의 `data/` 폴더를 직접 읽고 삽입합니다.
