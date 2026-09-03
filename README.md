# Into the Dead 3D Clone

> DirectX 11 기반의 자체 게임 프레임워크로 제작한 3D 횡스크롤 좀비 생존 액션 게임입니다.

[![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![DirectX](https://img.shields.io/badge/DirectX-11-107C10?logo=microsoft&logoColor=white)](https://learn.microsoft.com/windows/win32/direct3d11/)
[![HLSL](https://img.shields.io/badge/Shader-HLSL-7A5AF8)](https://learn.microsoft.com/windows/win32/direct3dhlsl/dx-graphics-hlsl)
[![Platform](https://img.shields.io/badge/Platform-Windows-0078D4?logo=windows&logoColor=white)](https://www.microsoft.com/windows)

**[▶ 플레이 영상 보기](https://www.youtube.com/watch?v=_vJ03esy1Zw)**

## 프로젝트 소개

| 항목 | 내용 |
| --- | --- |
| 개발 기간 | 2026.04.03 ~ 2026.06.24 (약 12주) |
| 개발 인원 | 1명 (개인 프로젝트) |
| 장르 | 3D 횡스크롤 좀비 생존 액션 |
| 플랫폼 | Windows PC |
| 개발 환경 | Visual Studio 2022, C++17, DirectX 11, HLSL |
| 담당 범위 | 프레임워크, 렌더링, 게임플레이, 에디터, UI, 데이터 관리 |

상용 엔진을 사용하지 않고 DirectX 11의 렌더링 파이프라인부터 게임 오브젝트 구조, 애니메이션, 충돌, 에디터와 스테이지 흐름까지 직접 구현하는 것을 목표로 제작했습니다.

플레이어는 폐허가 된 지역을 탐색하며 좀비와 전투하거나 은신 처치를 수행하고, 아이템을 수집해 생존해야 합니다. Stage 1과 Stage 2를 거쳐 대피소로 이동하는 게임 흐름과 로딩, 맵 전환, 사운드, 엔딩을 구성했습니다.

## 핵심 구현

### 1. DirectX 11 게임 프레임워크

- 게임 오브젝트와 컴포넌트를 조합하는 객체 구조 설계
- Prototype을 복제해 오브젝트를 생성하고 Level·Layer 단위로 관리
- Render Group을 이용해 우선순위, 비광원, 불투명, 블렌드, UI 렌더링 순서 분리
- 입력, 사운드, 조명, 충돌, 아이템, 리소스와 레벨을 Manager 단위로 모듈화

### 2. Deferred Rendering과 조명

- Diffuse, Normal, Depth 정보를 저장하는 MRT 기반 Deferred Rendering 파이프라인 구현
- Directional Light, Point Light, Spot Light 지원
- Bright Pass와 가로·세로 Blur 결과를 합성하는 Bloom 후처리 구성
- HLSL로 Fog, Emissive, Blur, Bloom, Dissolve, Fade In/Out 효과 구현

### 3. 캐릭터 상태 머신

제네릭 State Machine으로 행동별 로직과 애니메이션 전환을 분리했습니다.

| 대상 | 상태 |
| --- | --- |
| Player (11) | Idle, Walk, Run, Attack, Damaged, Execute, StealthAttack, Door, OpenDoor, CloseDoor, Stair |
| Zombie (9) | Idle, Walk, Run, Attack, Damaged, Fatal, Dead, StealthDeath, Stair |

- 플레이어 이동, 공격, 피격, 처형, 잠입 공격과 문·계단 상호작용 연동
- 좀비 탐지, 추적, 공격, 피격과 사망 흐름 구성
- 상태 진입·갱신·종료를 분리해 조건문 집중을 줄이고 행동 확장이 가능하도록 설계

### 4. 충돌과 Picking

- AABB·OBB 기반 충돌 판정 구현
- 플레이어, 좀비, 문, 계단과 상호작용 오브젝트 충돌 처리
- 화면 좌표를 World Ray로 변환해 에디터 오브젝트 선택에 활용

### 5. 맵 에디터와 데이터 파이프라인

- ImGui·ImGuizmo 기반의 인게임 맵 에디터 제작
- 모델, 충돌체, 문, 계단, Blocker와 Directional·Point·Spot Light 배치 및 수정
- 게임 오브젝트와 충돌 데이터를 JSON으로 저장·불러오기
- Assimp로 읽은 Model·Mesh·Bone·Animation 데이터를 자체 Binary 형식으로 직렬화해 로딩 시 재사용

### 6. 멀티스레드 업데이트

- 하드웨어 스레드 수에 맞춰 Thread Pool 생성
- 스테이지의 좀비 목록을 작업 구간으로 분할해 상태 갱신을 병렬 처리
- 작업 완료를 동기화한 뒤 다음 프레임 로직을 진행하도록 구성

### 7. 인벤토리와 게임 흐름

- 탐색 오브젝트에서 랜덤 아이템 획득 및 수량 관리
- 마우스 기반 아이템 선택, 회복 아이템 사용과 장비 선택 UI 구현
- Logo → Loading → Stage 1 → Stage 2 → Shelter → Ending 흐름 구성
- Fade 연출과 FMOD 기반 배경음·효과음 연동

## 구조

```text
Framework.sln
├─ Engine/                    # DirectX 11 프레임워크 DLL
│  ├─ Object / Component / Prototype / Layer
│  ├─ Renderer / RenderTarget / Light / Shader
│  ├─ Model / Mesh / Bone / Animation
│  └─ Input / Collision / Sound / ThreadPool / Managers
├─ Client/                    # 게임 로직과 콘텐츠
│  ├─ Player / Zombie / State Machine
│  ├─ Stage / Shelter / Loading / Ending
│  ├─ Inventory / Search / Map UI
│  └─ ImGui Map Editor
├─ Resources/Data/            # 오브젝트·셰이더 목록 등 데이터
