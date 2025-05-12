#pragma once
#include <cmath>
#include <vector>
#include <Memory/exMemory.hpp>
#include <Config/config.h>

#include <d3d9.h>   //  
#include <d3dx9.h>  //  D3DMatrix


#define sincos(radian, s, c) s = sin(radian); c = cos(radian)

//	Initialize Memory Class
inline auto g_memory = exMemory("OblivionRemastered-Win64-Shipping.exe");

namespace UnrealEngine
{
    /// 
    ///     FLAGS
    ///  

    enum class EObjectFlags : __int32
    {
        NoFlags = 0x00000000,

        Public = 0x00000001,
        Standalone = 0x00000002,
        MarkAsNative = 0x00000004,
        Transactional = 0x00000008,
        ClassDefaultObject = 0x00000010,
        ArchetypeObject = 0x00000020,
        Transient = 0x00000040,

        MarkAsRootSet = 0x00000080,
        TagGarbageTemp = 0x00000100,

        NeedInitialization = 0x00000200,
        NeedLoad = 0x00000400,
        KeepForCooker = 0x00000800,
        NeedPostLoad = 0x00001000,
        NeedPostLoadSubobjects = 0x00002000,
        NewerVersionExists = 0x00004000,
        BeginDestroyed = 0x00008000,
        FinishDestroyed = 0x00010000,

        BeingRegenerated = 0x00020000,
        DefaultSubObject = 0x00040000,
        WasLoaded = 0x00080000,
        TextExportTransient = 0x00100000,
        LoadCompleted = 0x00200000,
        InheritableComponentTemplate = 0x00400000,
        DuplicateTransient = 0x00800000,
        StrongRefOnFrame = 0x01000000,
        NonPIEDuplicateTransient = 0x02000000,
        Dynamic = 0x04000000,
        WillBeLoaded = 0x08000000,
    };


    /// 
    ///     STRUCTS
    ///  

    struct FName
    {
        int ComparisonIndex;	//0x0000
        int Number;	//0x0004
    };

    template <class T>
    struct TArray
    {
        T data;	    //0x0000
        int count;	//0x0008
        int max;	//0x000C
    };	//Size: 0x0010

    struct FVector2D
    {
        float X, Y;


        FVector2D()
        {
            X = 0;
            Y = 0;
        }

        FVector2D(float x, float y)
        {
            X = x;
            Y = y;
        }

        FVector2D operator+(const FVector2D& other) const { return FVector2D{ X + other.X, Y + other.Y }; }

        FVector2D operator-(const FVector2D& other) const { return FVector2D{ X - other.X, Y - other.Y }; }

        FVector2D operator*(float scalar) const { return FVector2D{ X * scalar, Y * scalar }; }

        FVector2D operator/(float scalar) const { return FVector2D{ X / scalar, Y / scalar }; }

        FVector2D operator=(float* other) const { return FVector2D{ other[0], other[1] }; }

        //  FVector2D operator=(const Vector2 other) const { return FVector2D{ other.x, other.y }; }

        //  FVector2D operator=(const ImVec2 other) const { return FVector2D{ other.x, other.y }; }

        FVector2D& operator*=(const FVector2D& other)
        {
            X += other.X;
            Y += other.Y;
            return *this;
        }


        bool IsValid() { return X == 0 && Y == 0; }

        float Distance(const FVector2D& other) const;
    };

    struct FVector
    {
        double X;	//0x0000
        double Y;	//0x0008
        double Z;	//0x0010


        FVector()
        {
            X = 0;
            Y = 0;
            Z = 0;
        }

        FVector(double x, double y, double z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        FVector operator+(const FVector& other) const { return FVector{ X + other.X, Y + other.Y, Z + other.Z }; }

        FVector operator-(const FVector& other) const { return FVector{ X - other.X, Y - other.Y, Z - other.Z }; }

        FVector operator*(float scalar) const { return FVector{ X * scalar, Y * scalar, Z * scalar }; }

        FVector operator/(float scalar) const { return FVector{ X / scalar, Y / scalar, Z / scalar }; }

        FVector operator=(float* other) const { return FVector{ other[0], other[1], other[2] }; }

        FVector& operator+=(const FVector& other)
        {
            X += other.X;
            Y += other.Y;
            Z += other.Z;
            return *this;
        }

        FVector& operator*=(const FVector& other)
        {
            X *= other.X;
            Y *= other.Y;
            Z *= other.Z;
            return *this;
        }

        FVector& operator*=(const float scalar) {
            X *= scalar;
            Y *= scalar;
            Z *= scalar;
            return *this;
        }

        bool IsValid();
        float Dot(FVector& other);
        float Distance(FVector& other);
    };	//Size: 0x0018

    struct FRotator
    {
        double Pitch;	//0x0000
        double Yaw; 	//0x0008
        double Roll;	//0x0010

        FVector ToVector();
        void GetAxes(FVector* Xaxis, FVector* Yaxis, FVector* Zaxis);
    };	//Size: 0x0018

    struct FQuat
    {
        double X;	//0x0000
        double Y;	//0x0008
        double Z;	//0x0010
        double W;	//0x0018

    };	//Size: 0x0020

    struct FTransform
    {
        FQuat Rotation;	//0x0000
        FVector Translation;	//0x0020
        char pad_0038[8];	//0x0038
        FVector Scale3D;	//0x0040
        char pad_0058[8];	//0x0058

        //  copy pasta
        D3DMATRIX to_matrix_with_scale();
    };	//Size: 0x0060

    struct FMatrix4x4
    {
        float m[4][4];

        //  
        FMatrix4x4();
        FMatrix4x4(const D3DMATRIX& MX);
        FMatrix4x4(const FTransform& TM);
        FMatrix4x4(const FRotator& rot, const FVector& origin = FVector(0, 0, 0));

        //  
        FMatrix4x4 operator*(const FMatrix4x4& other) const;

        //  
        FVector GetLocation() const;
    };  //Size: 0x0040

    struct FString
    {
        i64_t data;   //  const wchar_t*
        int count;
        int max;
    };

    struct FMinimalViewInfo
    {
        FVector Location;	//0x0000
        FRotator Rotation;	//0x0018
        float Fov;	//0x0030
        float DesiredFOV;	//0x0034
        float OrthoWidth;	//0x0038
        float OrthoNearClipPlane;	//0x003C
        float OrthoFarClipPlane;	//0x0040
        float PerspectiveNearClipPlane;	//0x0044
        float AspectRatio;	//0x0048
    };	//Size: 0x07C0

    struct FCameraCacheEntry
    {
        float Timestamp;	//0x0000
        char pad_0004[12];	//0x0004
        FMinimalViewInfo POV;	//0x0010
    };	//Size: 0x07D0
    
    struct FTViewTarget
    {
        char pad_0000[16];	//0x0000
        FMinimalViewInfo POV;	//0x0010
        i64_t PlayerState;	//0x0810    ;   APlayerState*
        char pad_0818[8];	//0x0818
    };	//Size: 0x0820

    struct FGameplayAttributeData
    {
        unsigned char Pad_0[0x8];           // 0x0000
        float BaseValue;                    // 0x0008
        float CurrentValue;                 // 0x000C
    };


    /// 
    ///     EXTRAS
    ///  

    struct EntityTransform
    {
        FVector pos;
        FRotator rot;
        FVector scale;
        FVector velocity;
    };

    struct CameraView
    {
        FVector location;
        FRotator rotation;
        float fov;
    };

    struct AABB
    {
        FVector m_min;
        FVector m_max;

        AABB()
        {
            m_min = FVector();
            m_max = FVector();
        }

        AABB(FVector min, FVector max)
        {
            m_min = min;
            m_max = max;
        }

        FVector Center() const { return (m_min + m_max) * 0.5f; }
        FVector Size() const { return m_max - m_min; }  //  width, height, depth
        FVector Extents() const { return Size() * 0.5f; }
    };

    namespace Classes
    {
        struct UObject
        {
            char pad_0000[8];	//0x0000    ;   VFTable
            int UFlags;	        //0x0008
            int UIndex;	        //0x000C
            i64_t UClass;	    //0x0010    ;   UClass*
            FName UName;	    //0x0018
            i64_t Outer;	    //0x0020    ;   UObject*
        };	//Size: 0x0028

        struct UField
        {
            UObject UObject;	//0x0000
            i64_t Next;	//0x0028                ;   UField*
        };	//Size: 0x0030

        struct UStruct
        {
            UField UField;	//0x0000
            char pad_0030[16];	//0x0030
            i64_t Super;	//0x0040                ;   UStruct*
            i64_t Children;	//0x0048            ;   UField*
            i64_t ChildrenProperties;	//0x0050    ;   UField*
            int Size;	//0x0058
            int MinAlignment;	//0x005C
            char pad_0060[80];	//0x0060
        };	//Size: 0x00B0

        struct UClass
        {
            UStruct UStruct;	//0x0000
            char pad_00B0[96];	//0x00B0
            i64_t DefaultObject;	//0x0110        ;   UObject*
            char pad_0118[232];	//0x0118
        };	//Size: 0x0200

        struct FNameEntryAllocator
        {
            i64_t frwLock;	//0x0000
            int CurrentBlock;	//0x0008
            int CurrentByteCursor;	//0x000C
            i64_t Blocks[8192];	//0x0010    ;   BlockData*
        };	//Size: 0x10010

        struct TUObject
        {
            i64_t pObject;	//0x0000
            char pad_0008[16];	//0x0008
        };	//Size: 0x0018

        struct UObjectPool
        {
            i64_t ObjectArray;	//0x0000    ;   TUObject*
            char pad_0008[12];	//0x0008
            int ObjectCount;	//0x0014
        };	//Size: 0x0018

        struct FNamePool
        {
            FNameEntryAllocator FNameEntryAllocator;	//0x0000
            int AnsiCount;	//0x10010
            int WideCount;	//0x10014
        };	//Size: 0x10018

        struct BlockData
        {
            char pad_0000[136];	//0x0000
        };	//Size: 0x0088

        struct UAttributeSet
        {
            UObject UObject;
            char pad_0028[8];   // 0x0028(0x0008)(Fixing Struct Size After Last Property [ Dumper-7 ])
        };

        struct UWorld
        {
            UObject UObject;	//0x0000
            char pad_0028[8];	//0x0028
            i64_t PersistentLevel;	//0x0030    ;   ULevel*
            char pad_0038[288];	//0x0038
            i64_t GameState;	//0x0158    ;   AGameStateBase*
            char pad_0160[88];	//0x0160
            i64_t OwningGameInstance;	//0x01B8    ;   UGameInstance*
        };	//Size: 0x01E0

        struct UActorComponent
        {
            UObject UObject;	//0x0000
            char pad_0028[120];	//0x0028
        };	//Size: 0x00A0

        struct USceneComponent
        {
            UActorComponent UActorComponent;	//0x0000
            char pad_00A0[136];	//0x00A0
            FVector RelativeLocation;	//0x0128
            FRotator RelativeRotation;	//0x0140
            FVector RelativeScale3D;	//0x0158
            FVector ComponentVelocity;	//0x0170
            char pad_0188[184];	//0x0188
            FTransform ComponentToWorld;	//0x0240
        };	//Size: 0x02A0

        struct UGameViewportClient
        {
            UObject N00000BD1;	//0x0000
            char pad_0028[80];	//0x0028
            i64_t World;	//0x0078    ;   UWorld*
            i64_t GameInstance;	//0x0080    ;   UGameInstance*
            char pad_0088[40];	//0x0088
            char ViewModeIndex;	//0x00B0
            char pad_00B1[751];	//0x00B1
        };	//Size: 0x03A0

        struct UPrimitiveComponent
        {
            USceneComponent USceneComponent;	//0x0000
            char pad_02A0[728];	//0x02A0
        };	//Size: 0x0578

        struct UMeshComponent
        {
            UPrimitiveComponent UPrimitiveComponent;	//0x0000
            char pad_0578[56];	//0x0578
        };	//Size: 0x05B0

        struct USkinnedMeshComponent
        {
            UMeshComponent UMeshComponent;	//0x0000
            char pad_05B0[96];	//0x05B0
            TArray<FTransform> BoneArray;	//0x0610
            char pad_0618[720];	//0x0618
        };	//Size: 0x08E8

        struct USkeletalMeshComponent
        {
            USkinnedMeshComponent USkinnedMeshComponent;	//0x0000
        };	//Size: 0x0FD0
        
        struct UMovementComponent
        {
            UActorComponent N00000D88;	//0x0000
            char pad_00A0[104];	//0x00A0
        };	//Size: 0x0108

        struct UNavMovementComponent
        {
            UMovementComponent UMovementComponent;	//0x0000
            char pad_0108[72];	//0x0108
        };	//Size: 0x0150

        struct UPawnMovementComponent
        {
            UNavMovementComponent UNavMovementComponent;	//0x0000
            i64_t PawnOwner;	//0x0150    ;   APawn*
        };	//Size: 0x0158

        struct UCharacterMovementComponent
        {
            UPawnMovementComponent UPawnMovementComponent;	//0x0000
            char pad_0158[3496];	//0x0158
        };	//Size: 0x0F00

        struct AActor
        {
            UObject UObject;	//0x0000
            char pad_0028[376];	//0x0028
            i64_t RootComponent;	//0x01A0    ;   USceneComponent*
            char pad_01A8[240];	//0x01A8
        };	//Size: 0x0298

        struct AController
        {
            AActor AActor;	//0x0000
            char pad_0298[8];	//0x0298
            i64_t PlayerState;	//0x02A0    ;   APlayerState*
            char pad_02A8[48];	//0x02A8
            i64_t Pawn;	//0x02D8    ;   APawn*
            char pad_02E0[8];	//0x02E0
            i64_t ACharacter;	//0x02E8    ;   ACharacter* 
            i64_t TransformComponent;	//0x02F0    ;   USceneComponent*
            char pad_02F8[24];	//0x02F8
            FRotator ControlRotation;	//0x0310
            char pad_0328[8];	//0x0328
        };  //  0x0330

        struct APlayerController
        {
            AController AController;	    //0x0000
            char pad_0330[8];	//0x0330
            i64_t Player;	//0x0338    ;   UPlayer*
            i64_t AcknowledgedPawn;	//0x0340    ;   APawn*
            char pad_0348[8];	//0x0348
            i64_t PlayerCameraManager;	//0x0350    ;   APlayerCameraManager*
            char pad_0358[1304];	//0x0358
        };	//Size: 0x0870

        struct AInfo
        {
            AActor AActor;	//0x0000
            char pad_0298[8];	//0x0298
        };	//Size: 0x02A0

        struct APlayerState
        {
            AInfo AInfo;	//0x0000
            char pad_02A0[120];	//0x02A0
            i64_t PawnPrivate;	//0x0318    ;   APawn* 
            char pad_0320[120];	//0x0320
            FString PlayerNamePrivate;	//0x0398
            char pad_03A8[24];	//0x03A8
        };	//Size: 0x03C0

        struct APawn
        {
            AActor AActor;	                //0x0000
            char pad_0298[32];	            //0x0298
            i64_t PlayerState;	        //0x02B8    ;   APlayerState*
            char pad_02C0[16];	            //0x02C0
            i64_t Controller;	            //0x02D0    ;   AController*
            char pad_02D8[72];	            //0x02D8
        };	//Size: 0x0320

        struct ACharacter
        {
            APawn APawn;	                //0x0000
            i64_t Mesh;	                //0x0320    ;   USkeletalMeshComponent*
            i64_t CharacterMovement;	    //0x0328    ;   UCharacterMovementComponent*
            char pad_0338[768];	            //0x0330
        };	//Size: 0x0630
    };

    namespace Offsets
    {
        /// v1.0.0
        inline auto GObjects = 0x09145170;    //  
        inline auto GNames = 0x0909EE80;      //  
        inline auto GWorld = 0x092B3878;      // 

        namespace World
        {
            constexpr auto PersistentLevel = 0x30;      //  ULevel
            constexpr auto AuthorityGameMode = 0x0150;  //  AGameMode
            constexpr auto GameState = 0x0158;          //  AGameState
            constexpr auto OwningGameInstance = 0x01B8; //  UGameInstance
        }

        namespace Level
        {
            constexpr auto Actors = 0x98;   //  TArray<AActor*>
        }

        namespace GameState
        {
            constexpr auto PlayerArray = 0x02B8;    // TArray<APlayerState*>
        }

        namespace GameInstance
        {
            constexpr auto LocalPlayers = 0x0038;    //  TArray<ULocalPlayer*>
        }

        namespace ViewportClient
        {
            constexpr auto ViewModeIndex = 0x00B0;  //  __int8
        }

        namespace UPlayer
        {
            constexpr auto PlayerController = 0x0030;    //  APlayerController
            constexpr auto ViewportClient = 0x0078;    //  UGameViewportClient
        }

        namespace Actor
        {
            constexpr auto bActorEnableCollision = 0x5C;    // BitIndex: 7
            constexpr auto RootComponent = 0x01A0; //  USceneComponent
        }

        namespace Controller
        {
            using namespace Actor;

            constexpr auto PlayerState = 0x02A0;            //  APlayerState
            constexpr auto Pawn = 0x02D8;                   //  APawn
            constexpr auto Character = 0x02E8;              //  ACharacter
            constexpr auto TransformComponent = 0x02F0;     //  USceneComponent
            constexpr auto Player = 0x0338;                 //  UPlayer
            constexpr auto AcknowledgedPawn = 0x0340;       //  APawn
            constexpr auto PlayerCameraManager = 0x0350;    //  APlayerCameraManager
        }

        namespace Pawn
        {
            using namespace Actor;

            constexpr auto PlayerState = 0x02B8;    //  APlayerState
            constexpr auto Controller = 0x02D0;     //  AController
        }

        namespace PlayerState
        {
            constexpr auto PawnPrivate = 0x0318;    //  APawn
            constexpr auto PlayerNamePrivate = 0x0398;    //  FString
        }

        namespace Character
        {
            using namespace Pawn;

            constexpr auto Mesh = 0x0320;    //  USkeletalMeshComponent
            constexpr auto CharacterMovement = 0x0328;    //  UCharacterMovementComponent
        }

        namespace APlayerCameraManager
        {
            constexpr auto PCOwner = 0x0298;    //  APlayerController
            constexpr auto TransformComponent = 0x02A0;    //  USceneComponent
            constexpr auto DefaultFOV = 0x02B0;    //  float
            constexpr auto DefaultOrthoWidth = 0x02B8;    //  float
            constexpr auto DefaultAspectRatio = 0x02C0;    //  float
            constexpr auto CameraCachePrivate = 0x1340;    //  FCameraCacheEntry
            constexpr auto LastFrameCameraCachePrivate = 0x2BD0;    //  FCameraCacheEntry
        }

        namespace USceneComponent
        {
            constexpr auto RelativeLocation = 0x0128;    //  FVector
            constexpr auto RelativeRotation = 0x0140;    //  FRotator
            constexpr auto RelativeScale3D = 0x0158;    //  FVector
            constexpr auto ComponentVelocity = 0x0170;    //  FVector
        }

        namespace UCharacterMovementComponent
        {
            constexpr auto GravityScale = 0x0170;                                      // float
            constexpr auto MaxStepHeight = 0x0174;                                     // float
            constexpr auto JumpZVelocity = 0x0178;                                     // float
            constexpr auto JumpOffJumpZFactor = 0x017C;                                // float
            constexpr auto WalkableFloorAngle = 0x019C;                                // float
            constexpr auto WalkableFloorZ = 0x01A0;                                    // float
            constexpr auto MovementMode = 0x01A4;                                      // EMovementMode ; unsigned __int8
        }

        namespace USkeletalMeshComponent
        {
            constexpr auto ComponentToWorld = 0x0240;    //  FTransform
            constexpr auto BoneArray = 0x0610;    //  TArray<FTransform>
        }
    }

    namespace Tools
    {
        //  
        bool GetObjectName(const Classes::UObject& object, std::string* outName);
        bool GetObjectName(const i64_t& pObject, std::string* outName);
        void SetViewMode(const unsigned __int8& viewIndex);
        void SetMovementMode(const unsigned __int8& viewIndex);

        //  
        bool IsValidPosition(const FVector& pos);
        FVector GetLookDir(const FRotator& rotation);
        FVector GetUpDir(const FRotator& rotation);
        FVector GetRightDir(const FRotator& rotation);
        bool TransformWorldToScreen(const FCameraCacheEntry& CameraView, const FVector& WorldLocation, const FVector2D& cxSize, FVector2D* Screen2D, const bool& isRelative);
        bool WorldToScreen(const CameraView& camera, const FVector& worldLocation, const FVector2D& szScreen, FVector2D* out, const bool& isRelative = false);

        //  copy pasta
        D3DMATRIX to_matrix(FRotator rot, FVector origin = FVector(0, 0, 0));
        D3DMATRIX matrix_multiplication(D3DMATRIX pm1, D3DMATRIX pm2);
    }
}

namespace Oblivion
{

}

class OblivionConfig : public Config
{
public:
    OblivionConfig() = default;
    explicit OblivionConfig(const std::string& filePath);

protected:
    void defaults(const std::string& filePath) override;
};
inline OblivionConfig g_config("oblivion.json");  //  default construction

class TESOblivion
{
public:
    explicit TESOblivion();
    ~TESOblivion() noexcept;

public:
	struct SImGuiActor
	{
        bool bAlive{ false };                                   //  
        std::string name;								        //	name
        UnrealEngine::FTransform CTW;					        //	component to world
        UnrealEngine::EntityTransform TM;				        //	transforms
        std::vector<UnrealEngine::FTransform> bones;	        //	bones

        //	ref
        i64_t pEntity{ 0 };                                       //  entity base address
        UnrealEngine::Classes::UObject object;
	};

    struct SImGuiCache
    {
        std::vector<SImGuiActor> actors;
    };

	struct SGame
	{
		bool bInGame{ false };
        i64_t pWorld{ 0 };                                    //  GWorld
        UnrealEngine::Classes::UWorld world;                  //  
        UnrealEngine::TArray<i64_t> actors;                   //  
        UnrealEngine::TArray<i64_t> players;                  //  
	};

	struct SLocalPlayer
	{
        i64_t pULocalPlayer{ 0 };                             //  UWorld->OwningGameInstance->LocalPlayers[0]
        i64_t pPlayerController{ 0 };                         //  ULocalPlayer->PlayerController
        i64_t pPawn{ 0 };                                     //  APlayerController->AcknowledgedPawn
        i64_t pCameraManager{ 0 };                            //  APlayerController->PlayerCameraManager
        UnrealEngine::FTransform CTW;                           //  transforms translation
        UnrealEngine::EntityTransform TM;                       //  world transforms { location, rotation, scale, velocity }
        std::vector<UnrealEngine::FTransform> Skeleton;         //  Skeleton Points
        UnrealEngine::Classes::APlayerController sController;   //  APlayerController Structure , used for most scatter reads. contains most all player information
	};

	struct SGlobals
	{
        SGame game;                                             //  game pointers
        SLocalPlayer localPlayer;                               //  local player pointers & transforms
        SImGuiCache render;                                     //  render cache transforms
        UnrealEngine::FCameraCacheEntry CameraView;             //  Camera View Matrix
	};


public:	//	patches
    bool bFlyMode{ false };
    bool bFullbright{ false };
    bool bInfiniteAmmo{ false };
    bool bInfiniteConsumablesQ{ false };

private:
    SGlobals m_imCache;                                                                           //  cache for imgui thread

public:
	void update();
    void shutdown();

    const SGlobals& GetCache() noexcept { return m_imCache; }

public: //  helpers

    /*attempts to obtain UWorld*/
    static i64_t GetWorld();

    /*attempts to obtain the local UPlayer
    * World -> UGameInstance -> LocalPlayers[0]
    */
    static i64_t GetLocalUPlayer(i64_t gWorld);

    /*attempts to obtain the Local Player Controller
    * ULocalPlayer -> Controller
    */
    static i64_t GetLocalPlayerController(i64_t uPlayer); 

    /*attempts to obtain the Local PlayerState
    * APlayerController -> PlayerState
    */
    static i64_t GetLocalPlayerState(i64_t pController);

    /*attempts to obtain the Local PlayerState
    * APlayerController -> Pawn
    */
    static i64_t GetLocalPlayerPawn(i64_t pController);

    /*attempts to obtain the Local Player Camera Manager
    * APlayerController -> PlayerCameraManager
    */
    static i64_t GetLocalPlayerCamera(i64_t pController);

    /*attempts to obtain the specified camera's FCameraCache*/
    static bool GetLocalCameraView(i64_t pCamera, UnrealEngine::FCameraCacheEntry* view);

    /* attempts to obtain the Actor Array 
    *  World -> PersistentLevel -> Actors
    */
    static bool GetActorArray(i64_t gWorld, std::vector<i64_t>* actors);
    
    /* attempts to obtain the Player Array 
    *  World -> GameState -> Players
    */
    static bool GetPlayerArray(i64_t gWorld, std::vector<i64_t>* actors);

    /*attempts to get an actors position
    * AActor -> RootComponent -> LocalLocation
    */
    static bool GetPlayerPosition(i64_t pPawn, UnrealEngine::FVector* out);

    /*attempts to get an actors position
    * AActor -> RootComponent -> LocalRotation
    */
    static bool GetPlayerRotation(i64_t pPawn, UnrealEngine::FRotator* out);

    /*attempts to obtain a pawns bone location at a specified index.
    * APawn -> Mesh -> BoneArray
    */
    static bool GetPlayerBonePosByIndex(i64_t pPawn, int index, UnrealEngine::FVector* bone);


public: //  patches
    /**/
    static void Fullbright(bool bEnable);

    /**/
    static void NoClip(bool bEnable);

private:

};
inline std::unique_ptr<TESOblivion> g_Oblivion;