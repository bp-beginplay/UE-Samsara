# UE-Samsara

1대1 턴제 전략 게임





개발일지

20261121

1. 나이아가라 이펙트를 사용해서 공격 사거리내의 작들을 표시
2. Spherecomponent의 자식 클래스인 rangecomponent클래스를 만들어서 사거리 기능 구현

20261122

1. 이동경로를 spline으로 표시하지 않고 effect로 표시하기 위해 MoveComponent를 만들었다.

20261125

1\.	RangeComponent에서 범위내에 모든 캐릭터를 이어주는 이펙트 생성

20261126

1. MoveComponent에서 이동 지점 설정과 이동 구현
   - 커서 위치까지 내비게이션 경로(PathPoints)를 찾아 경로 구간마다 이펙트로 표시하고, 경로 끝에 고스트를 놓아 이동 지점을 미리 보여 준다.
   - 좌클릭으로 이동 지점을 확정(SetTarget)하고, 이동을 시작하면 PathPoints를 하나씩 따라 이동한다.
2. 이동 경로(PathPoints)가 캐릭터 사이, 캐릭터와 벽 사이의 좁은 틈을 지나가지 않도록 내비게이션 설정 (아래 "기술 노트" 참고)



---

# 기술 노트

## 이동 경로가 캐릭터 사이 좁은 틈을 지나가지 않게 하기

### 목표

`UMoveComponent`가 `UNavigationSystemV1::FindPathToLocationSynchronously`로 만드는 `PathPoints`가

- 캐릭터와 캐릭터 사이
- 캐릭터와 벽(장애물) 사이

의 틈이 **캐릭터 한 명의 폭보다 좁으면** 그 틈을 지나가지 않게 한다.

### 원리

경로는 내비메시(바닥에 깔린 초록색 영역) 위에서만 만들어진다. 그래서 "지나가면 안 되는 곳"을 내비메시에서 지우면 경로도 그곳을 피해 간다.

- **벽 쪽**: 내비메시는 벽에서 **Agent Radius**만큼 떨어진 곳까지만 깔린다. 벽 사이 틈이 `Agent Radius × 2`보다 좁으면 내비메시가 끊겨 경로가 생기지 않는다.
- **캐릭터 쪽**: 기본적으로 움직이는 캐릭터는 내비메시에 영향을 주지 않는다. 그래서 캐릭터 캡슐을 **다이내믹 장애물**로 등록해 캐릭터 자리에 내비메시 구멍을 뚫는다. 이 구멍은 엔진이 자동으로 **Agent Radius만큼 넓혀 준다** (`RecastNavMeshGenerator.cpp`의 `MarkDynamicArea`). 그래서 구멍 반지름 = 캡슐 반지름 + Agent Radius가 되고, 두 캐릭터 몸 사이가 캐릭터 한 명의 폭보다 좁으면 구멍끼리 이어져 경로가 막힌다.

### 1. 프로젝트 설정

**Project Settings → Engine → Navigation Mesh → Runtime → Runtime Generation = `Dynamic Modifiers Only`**

- 실행 중에 캐릭터 위치(장애물)를 내비메시에 반영하기 위해 필요하다.
- `Dynamic`보다 가볍다. 장애물 표시만 실시간으로 다시 계산한다.
- `Config/DefaultEngine.ini`에 이렇게 저장된다.

```ini
[/Script/NavigationSystem.RecastNavMesh]
RuntimeGeneration=DynamicModifiersOnly
```

- 바꾼 뒤에는 **Build → Build Paths**로 내비메시를 다시 만들고 맵을 저장한다. 실행 중 장애물을 덧입히는 데 필요한 데이터가 빌드할 때 함께 만들어진다.

### 2. Agent Radius = 캐릭터 캡슐 반지름

레벨의 **RecastNavMesh-Default** 액터 → **생성 → 에이전트 반경(Agent Radius)**

- 캐릭터 캡슐 반지름과 같게 맞춘다. 현재 값: 캡슐 반지름 **34**, Agent Radius **35**.
- 캡슐 크기는 `BP_ArhatAurora` 등의 **클래스 디폴트 → CapsuleComponent → 캡슐 반경 / 캡슐 절반 높이**에서 확인한다 (현재 34 / 88).
- 값을 바꾸면 **Build Paths**를 다시 실행한다.

### 3. 캐릭터 캡슐을 장애물로 등록 (`BaseCharacter.cpp` 생성자)

```cpp
#include "Components/CapsuleComponent.h"
#include "NavAreas/NavArea_Null.h"

// ACharacter는 시작할 때 캡슐의 CanEverAffectNavigation을 이 값으로 덮어쓴다. (기본 false)
// 이 값을 켜야 아래 캡슐 장애물 설정이 실제로 내비메시에 반영된다.
bCanAffectNavigationGeneration = true;
GetCapsuleComponent()->SetCanEverAffectNavigation(true);
GetCapsuleComponent()->bDynamicObstacle = true;   // 충돌 형태 대신 "장애물 영역"으로 내비메시에 반영
GetCapsuleComponent()->SetAreaClassOverride(UNavArea_Null::StaticClass());
```

| 코드 | 역할 |
|---|---|
| `bCanAffectNavigationGeneration = true` | **가장 중요.** 폰이 내비게이션에 영향을 줄 수 있게 한다. 아래 "주의할 점 1" 참고 |
| `SetCanEverAffectNavigation(true)` | 캡슐이 내비게이션에 영향을 줄 수 있게 한다 |
| `bDynamicObstacle = true` | 캡슐을 충돌 형태가 아니라 "장애물 영역(모디파이어)"으로 내비메시에 반영한다. 콜리전 설정과 상관없이 반영된다 |
| `SetAreaClassOverride(UNavArea_Null)` | 장애물 영역을 "지나갈 수 없는 영역"으로 만든다 |

`UNavArea_Null` 대신 `UNavArea_Obstacle`을 쓰면 완전히 막지 않고 "웬만하면 피하는" 영역이 된다 (다른 길이 없으면 지나감). 턴제에서는 캐릭터를 통과할 수 없으므로 `UNavArea_Null`을 쓴다.

캡슐의 충돌과 캐릭터 이동에는 영향이 없다. 내비메시에만 구멍이 뚫린다.

### 4. 움직이는 캐릭터 자신은 장애물에서 빼기 (`MoveComponent.cpp`)

3번을 적용하면 **이동하려는 캐릭터 자신**의 캡슐도 구멍을 뚫는다. 출발 위치가 구멍 안이 되어 경로를 못 찾으므로, 목적지를 고르는 동안과 이동하는 동안에는 자기 캡슐을 장애물에서 뺀다. 턴제라 한 번에 한 캐릭터만 움직이므로 이 방식으로 충분하다.

```cpp
void UMoveComponent::StartSetTarget()
{
	if (ABaseCharacter* Character = GetOwner<ABaseCharacter>())
	{
		Character->GetCapsuleComponent()->SetCanEverAffectNavigation(false);   // 자기 장애물 끄기
	}
	// ...
}

void UMoveComponent::FinishMove()
{
	// ...
	if (ABaseCharacter* Character = GetOwner<ABaseCharacter>())
	{
		Character->GetCapsuleComponent()->SetCanEverAffectNavigation(true);    // 이동이 끝나면 다시 켜기
	}
}
```

목적지를 고르다 중간에 그만두는 경우(빙의 해제 등)에도 다시 `true`로 돌려놓아야 한다.

### 5. 확인 방법

- **에디터 뷰포트에서 `P` 키**: 내비메시가 초록색으로 보인다. 벽에서 조금 떨어진 곳부터 깔리는지, 좁은 통로가 끊겨 있는지 본다.
- **플레이 중 콘솔(`~`)에 `show Navigation`**: 다른 캐릭터 주변에 둥근 구멍(반지름 약 69 = 캡슐 34 + Agent Radius 35)이 보여야 한다.
- 리슨 서버로 테스트할 때는 **경로를 만드는 쪽(조종하는 클라이언트) 창**에서 확인한다.

### 주의할 점 (실제로 겪은 문제)

1. **캡슐 설정만으로는 반영되지 않는다.**
   `ACharacter::UpdateNavigationRelevance()`가 게임 시작 시 `CapsuleComponent->SetCanEverAffectNavigation(bCanAffectNavigationGeneration)`을 호출해서, 생성자에서 켠 캡슐 설정을 **폰의 값(기본 false)으로 덮어쓴다.** 그래서 반드시 `bCanAffectNavigationGeneration = true`를 함께 켜야 한다.
   에디터에서는 블루프린트 **클래스 디폴트 → 폰 → 내비게이션 생성에 영향 주기 가능**에 체크되어 있는지로 확인할 수 있다.

2. **생성자 변경은 라이브 코딩으로 반영되지 않는다.**
   에디터를 닫고 빌드한 뒤 다시 열어야 블루프린트 기본값에 반영된다.

3. **블루프린트에서 값을 바꾸면 그 값이 우선한다.**
   `BP_BaseCharacter`, `BP_ArhatAurora` 등에서 캡슐의 내비게이션 항목을 직접 바꾸면 생성자 값이 무시된다. 이상하면 해당 항목 옆의 되돌리기 화살표로 기본값으로 돌린다.

4. **반영에 몇 프레임 걸린다.**
   장애물을 켜고 끄면 내비메시가 몇 프레임에 걸쳐 갱신된다. 목적지 고르기를 시작한 직후 한두 프레임은 이전 상태로 경로가 나올 수 있다.

5. **좁아 보여도 경로가 생기는 통로가 있다면** 그 통로가 실제로는 `Agent Radius × 2`(약 70)보다 넓은 것이다. 막고 싶으면 장애물 간격을 줄이거나 Agent Radius를 키운다.

6. **사거리 스피어(`RangeComponent`)는 내비메시에 영향을 주지 않는다.**
   폰 전체가 내비게이션에 영향을 줄 수 있게 되었지만, 컴포넌트는 콜리전이 켜져 있고 Pawn을 **막을(Block)** 때만 내비메시에 반영된다. 사거리 스피어는 Pawn과 겹치기(Overlap)만 하므로 구멍을 뚫지 않는다. 나중에 Pawn을 막는 컴포넌트를 캐릭터에 추가하면 그 컴포넌트도 내비메시에 구멍을 뚫으니 주의한다.

