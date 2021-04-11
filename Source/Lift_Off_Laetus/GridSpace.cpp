// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSpace.h"

// Sets default values
AGridSpace::AGridSpace(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridMesh"));
	collision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>CubeMeshAsset(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'"));
	mesh->SetStaticMesh(CubeMeshAsset.Object);
	mesh->SetRelativeScale3D(FVector(2.f, 2.f, 0.1f));

	collision->SetRelativeScale3D(FVector(1.55f, 1.55f, 5.f));
	collision->SetHiddenInGame(false);
	collision->AttachToComponent(mesh, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AGridSpace::BeginPlay(){
	Super::BeginPlay();
}

// Called every frame
void AGridSpace::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

