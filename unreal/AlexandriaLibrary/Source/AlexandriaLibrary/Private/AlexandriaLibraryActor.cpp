// AlexandriaLibraryActor.cpp
// Unreal Engine 5.8

#include "AlexandriaLibraryActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
	constexpr float M = 100.f;          // meters -> Unreal units
	constexpr float WallThickness = 0.6f; // meters
	constexpr float FloorThickness = 0.3f;
	constexpr float DoorWidth = 3.f;
}

AAlexandriaLibraryActor::AAlexandriaLibraryActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	ColumnInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Columns"));
	ColumnInstances->SetupAttachment(Root);
	ColumnInstances->SetMobility(EComponentMobility::Static);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylFinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	CubeMesh = CubeFinder.Object;
	CylinderMesh = CylFinder.Object;

	// --- Concept outline: six halls ringed around a central atrium ---
	// Halls sit on a ring just outside the atrium, at 60-degree intervals.
	const float RingRadius = 55.f; // meters from center to hall center

	const TCHAR* Names[6] = {
		TEXT("Reading Hall 1"), TEXT("Cataloging Hall 2"), TEXT("Astronomy Lab"),
		TEXT("Medicine Room"), TEXT("Science Hall"), TEXT("Reading Hall 2")
	};
	const FVector Sizes[6] = {
		{40, 30, 8}, {35, 25, 7}, {30, 25, 10},
		{30, 25, 9}, {40, 35, 8}, {40, 30, 8}
	};
	const bool HasColumns[6] = { true, false, false, false, true, true };

	for (int32 i = 0; i < 6; ++i)
	{
		const float AngleRad = FMath::DegreesToRadians(60.f * i);
		FLibraryRoomSpec Spec;
		Spec.Name = Names[i];
		Spec.Dimensions = Sizes[i];
		Spec.CenterOffset = FVector(FMath::Cos(AngleRad), FMath::Sin(AngleRad), 0.f) * RingRadius;
		Spec.bColumns = HasColumns[i];
		Rooms.Add(Spec);
	}
}

void AAlexandriaLibraryActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!CubeMesh || !CylinderMesh)
	{
		return;
	}

	ClearGenerated();

	ColumnInstances->SetStaticMesh(CylinderMesh);
	if (MarbleMaterial)
	{
		ColumnInstances->SetMaterial(0, MarbleMaterial);
	}

	BuildAtrium();
	BuildCentralHall();
	for (const FLibraryRoomSpec& Room : Rooms)
	{
		BuildRoom(Room);
	}
	if (bBuildUnderground)
	{
		BuildUndergroundComplex();
	}
}

void AAlexandriaLibraryActor::ClearGenerated()
{
	for (UStaticMeshComponent* Comp : GeneratedComponents)
	{
		if (Comp)
		{
			Comp->DestroyComponent();
		}
	}
	GeneratedComponents.Reset();
	ColumnInstances->ClearInstances();
}

UStaticMeshComponent* AAlexandriaLibraryActor::AddBox(const FVector& CenterMeters, const FVector& SizeMeters, UMaterialInterface* Material)
{
	UStaticMeshComponent* Comp = NewObject<UStaticMeshComponent>(this);
	Comp->SetupAttachment(Root);
	Comp->SetStaticMesh(CubeMesh);
	Comp->SetMobility(EComponentMobility::Static);
	// Engine basic cube is 100 uu (1 m) per side, so scale == size in meters.
	Comp->SetRelativeLocation(CenterMeters * M);
	Comp->SetRelativeScale3D(SizeMeters);
	if (Material)
	{
		Comp->SetMaterial(0, Material);
	}
	Comp->RegisterComponent();
	GeneratedComponents.Add(Comp);
	return Comp;
}

UStaticMeshComponent* AAlexandriaLibraryActor::AddCylinder(const FVector& CenterMeters, float DiameterMeters, float HeightMeters, UMaterialInterface* Material)
{
	UStaticMeshComponent* Comp = NewObject<UStaticMeshComponent>(this);
	Comp->SetupAttachment(Root);
	Comp->SetStaticMesh(CylinderMesh);
	Comp->SetMobility(EComponentMobility::Static);
	Comp->SetRelativeLocation(CenterMeters * M);
	// Basic cylinder is 1 m diameter, 1 m tall.
	Comp->SetRelativeScale3D(FVector(DiameterMeters, DiameterMeters, HeightMeters));
	if (Material)
	{
		Comp->SetMaterial(0, Material);
	}
	Comp->RegisterComponent();
	GeneratedComponents.Add(Comp);
	return Comp;
}

void AAlexandriaLibraryActor::BuildRoom(const FLibraryRoomSpec& Room)
{
	const FVector& D = Room.Dimensions;   // X length, Y width, Z height
	const FVector& C = Room.CenterOffset; // room center at floor level

	// Floor (stone)
	AddBox(C + FVector(0, 0, -FloorThickness * 0.5f), FVector(D.X, D.Y, FloorThickness), StoneMaterial);

	const float HalfZ = D.Z * 0.5f;

	// Long walls (+Y / -Y), marble
	AddBox(C + FVector(0,  D.Y * 0.5f, HalfZ), FVector(D.X, WallThickness, D.Z), MarbleMaterial);
	AddBox(C + FVector(0, -D.Y * 0.5f, HalfZ), FVector(D.X, WallThickness, D.Z), MarbleMaterial);

	// End wall (+X), solid
	AddBox(C + FVector(D.X * 0.5f, 0, HalfZ), FVector(WallThickness, D.Y, D.Z), MarbleMaterial);

	// End wall (-X) split around a centered door gap, facing the atrium
	const float SegY = (D.Y - DoorWidth) * 0.5f;
	if (SegY > 0.f)
	{
		AddBox(C + FVector(-D.X * 0.5f,  (DoorWidth + SegY) * 0.5f, HalfZ), FVector(WallThickness, SegY, D.Z), MarbleMaterial);
		AddBox(C + FVector(-D.X * 0.5f, -(DoorWidth + SegY) * 0.5f, HalfZ), FVector(WallThickness, SegY, D.Z), MarbleMaterial);
	}

	if (Room.bColumns)
	{
		// Two interior rows of columns along the room length
		const int32 PerRow = FMath::Max(2, FMath::FloorToInt32(D.X / 8.f));
		for (int32 i = 0; i < PerRow; ++i)
		{
			const float X = -D.X * 0.5f + D.X * (i + 0.5f) / PerRow;
			for (const float Y : { -D.Y * 0.25f, D.Y * 0.25f })
			{
				FTransform T(FRotator::ZeroRotator,
					(C + FVector(X, Y, D.Z * 0.5f)) * M,
					FVector(0.8f, 0.8f, D.Z)); // basic cylinder is 1 m tall, 1 m diameter
				ColumnInstances->AddInstance(T);
			}
		}
	}
}

void AAlexandriaLibraryActor::BuildAtrium()
{
	const float R = AtriumDiameter * 0.5f;

	// Circular marble floor approximated with a scaled flat cylinder
	AddCylinder(FVector(0, 0, -FloorThickness * 0.5f), AtriumDiameter, FloorThickness, MarbleMaterial);

	// Colonnade ring + central stone fountain basin
	AddColumnRing(FVector::ZeroVector, R - 2.f, AtriumColumnCount, 10.f);
	AddCylinder(FVector(0, 0, 0.5f), 8.f, 1.f, StoneMaterial);
}

void AAlexandriaLibraryActor::BuildCentralHall()
{
	const float R = CentralHallRadius;                       // circumradius (to vertex)
	const float Apothem = R * FMath::Cos(PI / 8.f);          // center to wall midpoint
	const float Side = 2.f * R * FMath::Sin(PI / 8.f);       // octagon edge length
	const float HalfZ = CentralHallHeight * 0.5f;

	// Eight walls. The -X-facing segment (i == 4) is left open as the main entrance.
	for (int32 i = 0; i < 8; ++i)
	{
		if (i == 4)
		{
			continue;
		}
		const float AngleDeg = 45.f * i;
		const float AngleRad = FMath::DegreesToRadians(AngleDeg);
		const FVector Mid(FMath::Cos(AngleRad) * Apothem, FMath::Sin(AngleRad) * Apothem, HalfZ);

		UStaticMeshComponent* Wall = AddBox(Mid, FVector(WallThickness, Side, CentralHallHeight), MarbleMaterial);
		Wall->SetRelativeRotation(FRotator(0.f, AngleDeg, 0.f));
	}

	// Stepped dome: stacked shrinking discs from the wall tops up to a small oculus.
	const float BaseDia = Apothem * 2.f;
	const int32 Rings = FMath::Max(2, DomeRings);
	const float DomeRise = CentralHallHeight * 0.6f;
	for (int32 r = 0; r < Rings; ++r)
	{
		const float t = static_cast<float>(r) / (Rings - 1); // 0..1
		// Hemispherical profile: radius shrinks as sin, height climbs as (1 - cos-ish)
		const float Dia = BaseDia * FMath::Cos(t * (PI / 2.f) * 0.95f);
		const float Z = CentralHallHeight + DomeRise * t;
		AddCylinder(FVector(0, 0, Z), Dia, 0.4f, MarbleMaterial);
	}
}

void AAlexandriaLibraryActor::BuildUndergroundComplex()
{
	// A 3x3 grid of stone chambers one level down, linked by a central spine corridor.
	const float ChamberSize = 12.f;
	const float Spacing = 20.f;
	const float Depth = -CentralHallHeight; // floor of the complex
	const float CeilH = 6.f;

	for (int32 gx = -1; gx <= 1; ++gx)
	{
		for (int32 gy = -1; gy <= 1; ++gy)
		{
			const FVector C(gx * Spacing, gy * Spacing, Depth);

			// Chamber floor and four walls (stone)
			AddBox(C + FVector(0, 0, -FloorThickness * 0.5f), FVector(ChamberSize, ChamberSize, FloorThickness), StoneMaterial);

			const float HalfZ = CeilH * 0.5f;
			AddBox(C + FVector( ChamberSize * 0.5f, 0, HalfZ), FVector(WallThickness, ChamberSize, CeilH), StoneMaterial);
			AddBox(C + FVector(-ChamberSize * 0.5f, 0, HalfZ), FVector(WallThickness, ChamberSize, CeilH), StoneMaterial);
			AddBox(C + FVector(0,  ChamberSize * 0.5f, HalfZ), FVector(ChamberSize, WallThickness, CeilH), StoneMaterial);
			AddBox(C + FVector(0, -ChamberSize * 0.5f, HalfZ), FVector(ChamberSize, WallThickness, CeilH), StoneMaterial);

			// Ceiling
			AddBox(C + FVector(0, 0, CeilH + FloorThickness * 0.5f), FVector(ChamberSize, ChamberSize, FloorThickness), StoneMaterial);
		}
	}
}

void AAlexandriaLibraryActor::AddColumnRing(const FVector& CenterMeters, float RadiusMeters, int32 Count, float HeightMeters)
{
	for (int32 i = 0; i < Count; ++i)
	{
		const float AngleRad = 2.f * PI * i / FMath::Max(1, Count);
		const FVector Pos = CenterMeters + FVector(FMath::Cos(AngleRad) * RadiusMeters, FMath::Sin(AngleRad) * RadiusMeters, HeightMeters * 0.5f);
		FTransform T(FRotator::ZeroRotator, Pos * M, FVector(1.f, 1.f, HeightMeters));
		ColumnInstances->AddInstance(T);
	}
}
