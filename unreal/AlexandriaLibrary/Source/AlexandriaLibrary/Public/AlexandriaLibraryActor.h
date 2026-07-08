// AlexandriaLibraryActor.h
// Procedural greybox generator for a walkable Library of Alexandria concept.
// Unreal Engine 5.8. Drop the actor into a level; geometry builds in-editor.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AlexandriaLibraryActor.generated.h"

class UStaticMesh;
class UStaticMeshComponent;
class UInstancedStaticMeshComponent;
class UMaterialInterface;

/** One rectangular room: floor, four walls with a door gap, optional columns. */
USTRUCT(BlueprintType)
struct FLibraryRoomSpec
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	FString Name = TEXT("Room");

	/** X = length, Y = width, Z = wall height (meters, matching the outline table). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	FVector Dimensions = FVector(40.f, 30.f, 8.f);

	/** Placement relative to the actor, in meters. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	FVector CenterOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	bool bColumns = false;
};

UCLASS()
class ALEXANDRIALIBRARY_API AAlexandriaLibraryActor : public AActor
{
	GENERATED_BODY()

public:
	AAlexandriaLibraryActor();

	virtual void OnConstruction(const FTransform& Transform) override;

	/** Rooms to generate. Prefilled with the concept outline in the constructor. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Library")
	TArray<FLibraryRoomSpec> Rooms;

	/** Central atrium diameter in meters. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Library|Atrium")
	float AtriumDiameter = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Library|Atrium")
	int32 AtriumColumnCount = 16;

	/** Octagonal domed hall enclosing the atrium. Circumradius to each vertex, meters. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Library|CentralHall")
	float CentralHallRadius = 35.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Library|CentralHall")
	float CentralHallHeight = 14.f;

	/** Number of stacked rings approximating the dome. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Library|CentralHall")
	int32 DomeRings = 8;

	/** Optional stone chamber network beneath the central hall. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Library|Underground")
	bool bBuildUnderground = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Library|Materials")
	TObjectPtr<UMaterialInterface> MarbleMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Library|Materials")
	TObjectPtr<UMaterialInterface> StoneMaterial;

private:
	UPROPERTY()
	TObjectPtr<USceneComponent> Root;

	/** All columns rendered as one instanced component. */
	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> ColumnInstances;

	UPROPERTY()
	TObjectPtr<UStaticMesh> CubeMesh;      // /Engine/BasicShapes/Cube (100 uu)

	UPROPERTY()
	TObjectPtr<UStaticMesh> CylinderMesh;  // /Engine/BasicShapes/Cylinder (100 uu)

	/** Components created in OnConstruction; cleared and rebuilt each run. */
	UPROPERTY(Transient)
	TArray<TObjectPtr<UStaticMeshComponent>> GeneratedComponents;

	void ClearGenerated();
	UStaticMeshComponent* AddBox(const FVector& CenterMeters, const FVector& SizeMeters, UMaterialInterface* Material);
	UStaticMeshComponent* AddCylinder(const FVector& CenterMeters, float DiameterMeters, float HeightMeters, UMaterialInterface* Material);
	void BuildRoom(const FLibraryRoomSpec& Room);
	void BuildAtrium();
	void BuildCentralHall();
	void BuildUndergroundComplex();
	void AddColumnRing(const FVector& CenterMeters, float RadiusMeters, int32 Count, float HeightMeters);
};
