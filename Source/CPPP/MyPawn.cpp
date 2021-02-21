// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Engine/Classes/Camera/CameraComponent.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
	OurCamera->SetupAttachment(RootComponent);
	OurCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
	OurCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	OurVisibleComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	//Grow 액션에 따라 키우고 줄이는 것 처리
	{
		float CurrentScale = OurVisibleComponent->GetComponentScale().X;
		if (bGrowing)
		{
			//1초에 걸쳐 두배로 키움
			CurrentScale += DeltaTime;
		}

		else
		{
			//키운 속도대로 절반으로 줄임
			CurrentScale -= (DeltaTime * 0.5f);
		}

		//처음 크기 2배보다 커지거나 처음 크기보다 작아지는 걸 방지
		CurrentScale = FMath::Clamp(CurrentScale, 1.0f, 2.0f);
		OurVisibleComponent->SetWorldScale3D(FVector(CurrentScale));

	}

	//MoveX MoveY 축에 따라 이동 처리
	{
		if (!CurrnntVelocity.IsZero())
		{
			FVector NewLocation = GetActorLocation() + (CurrnntVelocity * DeltaTime);
			SetActorLocation(NewLocation);
		}
	}
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Grow 키를 누르거나 뗄 때 반응
	InputComponent->BindAction("Grow", IE_Pressed, this, &AMyPawn::StartGrowing);
	InputComponent->BindAction("Grow", IE_Released, this, &AMyPawn::StopGrowing);

	//MoveX와 MoveY 두 이동 축의 값에 매 프레임 반응
	InputComponent->BindAxis("MoveX", this, &AMyPawn::Move_XAxis);
	InputComponent->BindAxis("MoveY", this, &AMyPawn::Move_YAxis);

}

void AMyPawn::Move_XAxis(float AxisValue)
{

	//초당 100유닛을 앞 또는 뒤로 움직임
	CurrnntVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;

}

void AMyPawn::Move_YAxis(float AxisValue)
{

	//초당 100유닛을 좌 또는 우로 움직임
	CurrnntVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;

}

void AMyPawn::StartGrowing()
{

	bGrowing = true;

}

