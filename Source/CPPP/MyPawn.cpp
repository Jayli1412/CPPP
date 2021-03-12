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

	Super::Tick(DeltaTime);
	//Grow �׼ǿ� ���� Ű��� ���̴� �� ó��
	{
		float CurrentScale = OurVisibleComponent->GetComponentScale().X;
		if (bGrowing)
		{
			//1�ʿ� ���� �ι�� Ű��
			CurrentScale += DeltaTime;
		}

		else
		{
			//Ű�� �ӵ���� �������� ����
			CurrentScale -= (DeltaTime * 0.5f);
		}

		//ó�� ũ�� 2�躸�� Ŀ���ų� ó�� ũ�⺸�� �۾����� �� ����
		CurrentScale = FMath::Clamp(CurrentScale, 1.0f, 2.0f);
		OurVisibleComponent->SetWorldScale3D(FVector(CurrentScale));

	}

	//MoveX MoveY �࿡ ���� �̵� ó��
	{
		if (!CurrentVelocity.IsZero())
		{
			FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
			SetActorLocation(NewLocation);
		}
	}
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Grow Ű�� �����ų� �� �� ����
	InputComponent->BindAction("Grow", IE_Pressed, this, &AMyPawn::StartGrowing);
	InputComponent->BindAction("Grow", IE_Released, this, &AMyPawn::StopGrowing);

	//MoveX�� MoveY �� �̵� ���� ���� �� ������ ����
	InputComponent->BindAxis("MoveX", this, &AMyPawn::Move_XAxis);
	InputComponent->BindAxis("MoveY", this, &AMyPawn::Move_YAxis);

}

void AMyPawn::Move_XAxis(float AxisValue)
{

	//�ʴ� 100������ �� �Ǵ� �ڷ� ������
	CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;

}

void AMyPawn::Move_YAxis(float AxisValue)
{

	//�ʴ� 100������ �� �Ǵ� ��� ������
	CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;

}

void AMyPawn::StartGrowing()
{

	bGrowing = true;

}

void AMyPawn::StopGrowing()
{
	bGrowing = false;
}