<img width="998" height="783" alt="image" src="https://github.com/user-attachments/assets/07f8ac7b-f53f-4b4e-8f03-97525add7b49" />

# 2D Fluid Simulation
본 프로젝트는 **JGL2 라이브러리**를 사용하여 C++로 구현한  
**2D 유체 시뮬레이션 프로그램**입니다.

유체의 **확산(Diffusion)**, **이동(Advection)**,  
**질량 보존(Mass Conservation)** 과 같은 기본적인 유체 물리 현상을  
실시간으로 시뮬레이션하며, 사용자 상호작용을 통해  
유체의 거동을 직관적으로 관찰할 수 있도록 설계되었습니다.

## Background
유체 시뮬레이션은 연속체 역학을 기반으로 하며,  
본 프로젝트에서는 **Stable Fluids** 기법을 기반으로 한  
격자(grid) 기반 유체 모델을 사용합니다.

각 시뮬레이션 스텝은 다음과 같은 단계로 구성됩니다.

1. 외력 추가 (Add Force)
2. 확산 (Diffusion)
3. 이동 (Advection)
4. 압력 계산 및 발산 제거 (Projection)

이를 통해 시간 스텝에 대해 안정적인 시뮬레이션을 구현합니다.


## Implementation Details

- Language: C++
- Graphics Library: JGL2

### Core Features
- Grid 기반 2D 유체 시뮬레이션
- Diffusion / Advection 구현
- Incompressible flow (질량 보존)
- 실시간 렌더링
- 사용자 입력 기반 상호작용


## Key Components

### Velocity Field
- 각 격자 셀에 저장된 유체의 속도 벡터
- 유체 이동(Advection)에 사용

### Density Field
- 유체의 밀도를 표현
- 시각화 시 색상 또는 밝기로 표현

### Diffusion
- 점성(viscosity)에 의한 유체 확산 효과
- Iterative solver를 사용하여 계산

### Projection
- 속도장의 발산(divergence)을 제거하여
  비압축성(incompressible) 유체 조건을 만족

## User Interaction
- 마우스 입력으로 유체에 힘 추가
- 밀도 주입을 통해 유체 흐름 생성
- 시뮬레이션 파라미터 실시간 조정 가능

## Important Parameters

| Parameter | Description |
|---------|-------------|
| dt | 시간 스텝 |
| diff | 확산 계수 |
| visc | 점성 계수 |
| N | 시뮬레이션 격자 해상도 |
