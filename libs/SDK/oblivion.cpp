#include "oblivion.h"

//////////////////////////////////////////////////////
///                 UNREAL ENGINE                  ///
//////////////////////////////////////////////////////

namespace UnrealEngine
{
    float FVector2D::Distance(const FVector2D& other) const
    {
        FVector2D dif = other - *this;
        return sqrt(dif.X * dif.X + dif.Y * dif.Y);
    }

    bool FVector::IsValid() { return X == 0 && Y == 0; }

    float FVector::Dot(FVector& other) { return X * other.X + Y * other.Y + Z * other.Z; }

    float FVector::Distance(FVector& other)
    {
        auto dist = *this - other;
        return sqrt(dist.X * dist.X + dist.Y * dist.Y + dist.Z * dist.Z) / 100.f;
    }

    FVector FRotator::ToVector()
    {
        static float UROTTORAD = M_PI / 180.f;

        FRotator rotator = *this;

        float pitch = rotator.Pitch * UROTTORAD;
        float yaw = rotator.Yaw * UROTTORAD;
        float roll = rotator.Roll * UROTTORAD;

        float cosPitch = cos(pitch);
        float sinPitch = sin(pitch);
        float cosYaw = cos(yaw);
        float sinYaw = sin(yaw);

        return FVector{ cosPitch * cosYaw, cosPitch * sinYaw, sinPitch };
    }

    void FRotator::GetAxes(FVector* Xaxis, FVector* Yaxis, FVector* Zaxis)
    {
        FRotator rotator = *this;

        FVector x = rotator.ToVector(); //  forward vector
        rotator.Yaw += 90.f;

        FRotator rotator2 = rotator;
        rotator2.Pitch = 0.f;
        FVector y = rotator2.ToVector();    //  right direction
        y.Z = 0.f;

        rotator.Yaw -= 90.f;
        rotator.Pitch += 90.f;
        FVector z = rotator.ToVector(); //  up direction

        *Xaxis = x;
        *Yaxis = y;
        *Zaxis = z;
    }

    D3DMATRIX FTransform::to_matrix_with_scale()
    {
        D3DMATRIX m{};
        m._41 = Translation.X;
        m._42 = Translation.Y;
        m._43 = Translation.Z;

        float x2 = Rotation.X + Rotation.X;
        float y2 = Rotation.Y + Rotation.Y;
        float z2 = Rotation.Z + Rotation.Z;
        float xx2 = Rotation.X * x2;
        float yy2 = Rotation.Y * y2;
        float zz2 = Rotation.Z * z2;

        m._11 = (1.0f - (yy2 + zz2)) * Scale3D.X;
        m._22 = (1.0f - (xx2 + zz2)) * Scale3D.Y;
        m._33 = (1.0f - (xx2 + yy2)) * Scale3D.Z;
        float yz2 = Rotation.Y * z2;
        float wx2 = Rotation.W * x2;

        m._32 = (yz2 - wx2) * Scale3D.Z;
        m._23 = (yz2 + wx2) * Scale3D.Y;
        float xy2 = Rotation.X * y2;
        float wz2 = Rotation.W * z2;

        m._21 = (xy2 - wz2) * Scale3D.Y;
        m._12 = (xy2 + wz2) * Scale3D.X;
        float xz2 = Rotation.X * z2;
        float wy2 = Rotation.W * y2;

        m._31 = (xz2 + wy2) * Scale3D.Z;
        m._13 = (xz2 - wy2) * Scale3D.X;
        m._14 = 0.0f;
        m._24 = 0.0f;
        m._34 = 0.0f;
        m._44 = 1.0f;

        return m;
    }

    FMatrix4x4::FMatrix4x4()
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                m[i][j] = (i == j) ? 1.0f : 0.0f;
    }

    FMatrix4x4::FMatrix4x4(const FRotator& rot, const FVector& origin)
    {
        // Convert rotation angles from degrees to radians
        float radPitch = (rot.Pitch * M_PI / 180.0f);
        float radYaw = (rot.Yaw * M_PI / 180.0f);
        float radRoll = (rot.Roll * M_PI / 180.0f);

        // Calculate sine and cosine for each rotation
        float sp = sinf(radPitch);
        float cp = cosf(radPitch);
        float sy = sinf(radYaw);
        float cy = cosf(radYaw);
        float sr = sinf(radRoll);
        float cr = cosf(radRoll);

        // Set the rotation components of the matrix
        m[0][0] = cp * cy;
        m[0][1] = cp * sy;
        m[0][2] = sp;
        m[0][3] = 0.0f;

        m[1][0] = sr * sp * cy - cr * sy;
        m[1][1] = sr * sp * sy + cr * cy;
        m[1][2] = -sr * cp;
        m[1][3] = 0.0f;

        m[2][0] = -(cr * sp * cy + sr * sy);
        m[2][1] = cy * sr - cr * sp * sy;
        m[2][2] = cr * cp;
        m[2][3] = 0.0f;

        // Set the translation components of the matrix
        m[3][0] = origin.X;
        m[3][1] = origin.Y;
        m[3][2] = origin.Z;
        m[3][3] = 1.0f; // Homogeneous coordinate
    }

    FMatrix4x4::FMatrix4x4(const D3DMATRIX& MX)
    {
        m[0][0] = MX._11;
        m[0][1] = MX._12;
        m[0][2] = MX._13;
        m[0][3] = MX._14;

        m[1][0] = MX._21;
        m[1][1] = MX._22;
        m[1][2] = MX._23;
        m[1][3] = MX._24;

        m[2][0] = MX._31;
        m[2][1] = MX._32;
        m[2][2] = MX._33;
        m[2][3] = MX._34;

        m[3][0] = MX._41;
        m[3][1] = MX._42;
        m[3][2] = MX._43;
        m[3][3] = MX._44;
    }

    FMatrix4x4::FMatrix4x4(const FTransform& TM)
    {
        const FQuat& Rotation = TM.Rotation;
        const FVector& Translation = TM.Translation;
        const FVector& Scale3D = TM.Scale3D;

        // Set translation
        m[3][0] = Translation.X;
        m[3][1] = Translation.Y;
        m[3][2] = Translation.Z;

        // Calculate rotation components
        float x2 = Rotation.X + Rotation.X;
        float y2 = Rotation.Y + Rotation.Y;
        float z2 = Rotation.Z + Rotation.Z;
        float xx2 = Rotation.X * x2;
        float yy2 = Rotation.Y * y2;
        float zz2 = Rotation.Z * z2;

        m[0][0] = (1.0f - (yy2 + zz2)) * Scale3D.X; // m[11]
        m[1][1] = (1.0f - (xx2 + zz2)) * Scale3D.Y; // m[22]
        m[2][2] = (1.0f - (xx2 + yy2)) * Scale3D.Z; // m[33]

        float yz2 = Rotation.Y * z2;
        float wx2 = Rotation.W * x2;
        m[2][1] = (yz2 - wx2) * Scale3D.Z; // m[32]
        m[1][2] = (yz2 + wx2) * Scale3D.Y; // m[23]

        float xy2 = Rotation.X * y2;
        float wz2 = Rotation.W * z2;
        m[1][1] = (xy2 - wz2) * Scale3D.Y; // m[21]
        m[0][1] = (xy2 + wz2) * Scale3D.X; // m[12]

        float xz2 = Rotation.X * z2;
        float wy2 = Rotation.W * y2;
        m[2][0] = (xz2 + wy2) * Scale3D.Z; // m[31]
        m[0][2] = (xz2 - wy2) * Scale3D.X; // m[13]

        // Last row for homogeneous coordinates
        m[3][3] = 1.0f;
        m[0][3] = m[1][3] = m[2][3] = 0.0f;
    }

    FMatrix4x4 FMatrix4x4::operator*(const FMatrix4x4& other) const
    {
        FMatrix4x4 result;

        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                result.m[i][j] =
                    m[i][0] * other.m[0][j] +
                    m[i][1] * other.m[1][j] +
                    m[i][2] * other.m[2][j] +
                    m[i][3] * other.m[3][j];
            }
        }

        return result;
    }

    FVector FMatrix4x4::GetLocation() const { return FVector(m[3][0], m[3][1], m[3][2]); }

    bool Tools::GetObjectName(i64_t pObject, std::string* out)
    {
        std::string result;

        auto object = g_memory.Read<Classes::UObject>(pObject);
        auto& index = object.UName.ComparisonIndex;
        if (!index)
            return false;

        auto a1 = (g_memory.Read<i64_t>((g_memory.GetProcessInfo().dwModuleBase + UnrealEngine::Offsets::GNames) + 8 * ((i64_t)(index & 0x1FFFFFFF) >> 16) + 16) + 2 * (unsigned int)(unsigned __int16)index);
        auto wide = g_memory.Read<unsigned __int16>(a1) & 1;
        auto len = g_memory.Read<unsigned __int16>(a1) >> 6;
        if (len <= 0 || len > 128)
            return false;

        auto fname = a1 + 2;

        char buffer[128];
        if (!g_memory.ReadMemory(fname, &buffer, len))
            return false;

        *out = std::string(buffer, buffer + len);

        return out->size() > 0;
    }

    void Tools::SetViewMode(unsigned __int8 viewMode)
    {
        //  Get World
        auto pWorld = g_memory.Read<i64_t>(g_memory.GetProcessInfo().dwModuleBase + Offsets::GWorld);
        if (!pWorld)
            return;

        //  get game instance
        auto pGameInstance = g_memory.Read<i64_t>(pWorld + Offsets::World::OwningGameInstance);
        if (!pGameInstance)
            return;

        //  get local players
        auto pLocalPlayers = g_memory.Read<i64_t>(pGameInstance + Offsets::GameInstance::LocalPlayers);
        if (!pLocalPlayers)
            return;

        //  get local player
        auto pLocalPlayer = g_memory.Read<i64_t>(pLocalPlayers);
        if (!pLocalPlayer)
            return;

        //  get local player viewport client
        auto pViewport = g_memory.Read<i64_t>(pLocalPlayer + Offsets::UPlayer::ViewportClient);
        if (!pViewport)
            return;

        //  finally apply view mode patch
        g_memory.Write<unsigned __int8>(pViewport + Offsets::ViewportClient::ViewModeIndex, viewMode);
    }

    void Tools::SetMovementMode(unsigned __int8 movementMode)
    {
        //  Get World
        auto pWorld = g_memory.Read<i64_t>(g_memory.GetProcessInfo().dwModuleBase + Offsets::GWorld);
        if (!pWorld)
            return;

        //  get game instance
        auto pGameInstance = g_memory.Read<i64_t>(pWorld + Offsets::World::OwningGameInstance);
        if (!pGameInstance)
            return;

        //  get local players
        auto pLocalPlayers = g_memory.Read<i64_t>(pGameInstance + Offsets::GameInstance::LocalPlayers);
        if (!pLocalPlayers)
            return;

        //  get local player
        auto pLocalPlayer = g_memory.Read<i64_t>(pLocalPlayers);
        if (!pLocalPlayer)
            return;

        //  get local player controller
        auto pLocalController = g_memory.Read<i64_t>(pLocalPlayer + Offsets::UPlayer::PlayerController);
        if (pLocalController)
            return;

        //  get local player character
        auto pLocalCharacter = g_memory.Read<i64_t>(pLocalController + Offsets::Controller::Character);
        if (!pLocalCharacter)
            return;

        //  get local player movement component
        auto pMovementComponent = g_memory.Read<i64_t>(pLocalCharacter + Offsets::Character::CharacterMovement);
        if (!pMovementComponent)
            return;

        g_memory.Write<unsigned __int8>(pMovementComponent + Offsets::UCharacterMovementComponent::MovementMode, movementMode);
    }

    bool Tools::IsValidPosition(FVector pos)
    {
        bool result{ true };

        if (pos.X == 0 && pos.Y == 0.0f && pos.Z == 0.0f)
            result = false;

        return result;
    }

    FVector Tools::GetLookDir(FRotator rotation)
    {
        float cp, sp, cy, sy;
        sincos(rotation.Pitch * (M_PI / 180.0f), sp, cp);
        sincos(rotation.Yaw * (M_PI / 180.0f), sy, cy);

        FVector fwd;
        fwd.X = cp * cy;
        fwd.Y = cp * sy;
        fwd.Z = sp;

        return fwd;
    }

    FVector Tools::GetUpDir(FRotator rotation)
    {
        float cp, sp, cr, sr, cy, sy;

        sincos(rotation.Pitch * (M_PI / 180.0f), sp, cp);
        sincos(rotation.Roll * (M_PI / 180.0f), sr, cr);
        sincos(rotation.Yaw * (M_PI / 180.0f), sy, cy);

        FVector up;
        up.X = -sr * cy - cr * sp * sy;
        up.Y = sr * sy - cr * sp * cy;
        up.Z = cr * cp;

        return up;
    }

    FVector Tools::GetRightDir(FRotator rotation)
    {
        float cp, sp, cr, sr, cy, sy;

        // Calculate sine and cosine for pitch, roll, and yaw
        sincos(rotation.Pitch * (M_PI / 180.0f), sp, cp);
        sincos(rotation.Roll * (M_PI / 180.0f), sr, cr);
        sincos(rotation.Yaw * (M_PI / 180.0f), sy, cy);

        // Right direction calculation (based on Yaw and Roll)
        FVector right;
        right.X = -sp * sy + cp * cr * cy;
        right.Y = sp * cy + cp * cr * sy;
        right.Z = cp * sr;

        return right;
    }

    bool Tools::TransformWorldToScreen(FCameraCacheEntry CameraView, FVector WorldLocation, FVector2D& Screen2D, FVector2D cxSize, bool isRelative)
    {
        //  Get Camera Information
        FVector vAxisX, vAxisY, vAxisZ, vDelta, vTransformed;
        FVector CamLocation = CameraView.POV.Location;
        FRotator CamRotation = CameraView.POV.Rotation;
        CamRotation.GetAxes(&vAxisX, &vAxisY, &vAxisZ);     //  get cameras view -> look, right , up directions

        //  Calculate Transformed Position
        vDelta = WorldLocation - CamLocation;
        vTransformed.X = vDelta.Dot(vAxisY);
        vTransformed.Y = vDelta.Dot(vAxisZ);
        vTransformed.Z = vDelta.Dot(vAxisX);
        if (vTransformed.Z < 1.00f)
        {
            if (isRelative)
                return false;

            vTransformed.Z = 1.00f;
        }

        //  Get Screen & FOV
        float mFOV = CameraView.POV.Fov;
        FVector2D center = { cxSize.X * .5f, cxSize.Y * .5f };

        //  Transform to Screen Space
        Screen2D.X = center.X + vTransformed.X * (center.X / tanf(mFOV * M_PI / 360.f)) / vTransformed.Z;
        Screen2D.Y = center.Y - vTransformed.Y * (center.X / tanf(mFOV * M_PI / 360.f)) / vTransformed.Z;

        //  Within camera view
        if (isRelative && Screen2D.X <= 0.0f || Screen2D.Y <= 0.0f || Screen2D.X > cxSize.X || Screen2D.Y > cxSize.Y)
            return false;

        return true;
    }

    bool Tools::WorldToScreen(CameraView camera, FVector worldLocation, FVector2D* out, FVector2D szScreen, bool isRelative)
    {
        FCameraCacheEntry cache;
        cache.POV.Location = camera.location;
        cache.POV.Rotation = camera.rotation;
        cache.POV.Fov = camera.fov;

        return TransformWorldToScreen(cache, worldLocation, *out, szScreen, isRelative);
    }

    //////////////////////////////////////////////////////
    ///                 D3D METHODS                    ///
    //////////////////////////////////////////////////////

    D3DMATRIX Tools::to_matrix(FRotator rot, FVector origin)
    {
        float radpitch = (rot.Pitch * M_PI / 180);
        float radyaw = (rot.Yaw * M_PI / 180);
        float radroll = (rot.Roll * M_PI / 180);
        float sp = sinf(radpitch);
        float cp = cosf(radpitch);
        float sy = sinf(radyaw);
        float cy = cosf(radyaw);
        float sr = sinf(radroll);
        float cr = cosf(radroll);
        D3DMATRIX matrix{};
        matrix.m[0][0] = cp * cy;
        matrix.m[0][1] = cp * sy;
        matrix.m[0][2] = sp;
        matrix.m[0][3] = 0.f;
        matrix.m[1][0] = sr * sp * cy - cr * sy;
        matrix.m[1][1] = sr * sp * sy + cr * cy;
        matrix.m[1][2] = -sr * cp;
        matrix.m[1][3] = 0.f;
        matrix.m[2][0] = -(cr * sp * cy + sr * sy);
        matrix.m[2][1] = cy * sr - cr * sp * sy;
        matrix.m[2][2] = cr * cp;
        matrix.m[2][3] = 0.f;
        matrix.m[3][0] = origin.X;
        matrix.m[3][1] = origin.Y;
        matrix.m[3][2] = origin.Z;
        matrix.m[3][3] = 1.f;
        return matrix;
    }

    D3DMATRIX Tools::matrix_multiplication(D3DMATRIX pm1, D3DMATRIX pm2)
    {
        D3DMATRIX pout{};
        pout._11 = pm1._11 * pm2._11 + pm1._12 * pm2._21 + pm1._13 * pm2._31 + pm1._14 * pm2._41;
        pout._12 = pm1._11 * pm2._12 + pm1._12 * pm2._22 + pm1._13 * pm2._32 + pm1._14 * pm2._42;
        pout._13 = pm1._11 * pm2._13 + pm1._12 * pm2._23 + pm1._13 * pm2._33 + pm1._14 * pm2._43;
        pout._14 = pm1._11 * pm2._14 + pm1._12 * pm2._24 + pm1._13 * pm2._34 + pm1._14 * pm2._44;
        pout._21 = pm1._21 * pm2._11 + pm1._22 * pm2._21 + pm1._23 * pm2._31 + pm1._24 * pm2._41;
        pout._22 = pm1._21 * pm2._12 + pm1._22 * pm2._22 + pm1._23 * pm2._32 + pm1._24 * pm2._42;
        pout._23 = pm1._21 * pm2._13 + pm1._22 * pm2._23 + pm1._23 * pm2._33 + pm1._24 * pm2._43;
        pout._24 = pm1._21 * pm2._14 + pm1._22 * pm2._24 + pm1._23 * pm2._34 + pm1._24 * pm2._44;
        pout._31 = pm1._31 * pm2._11 + pm1._32 * pm2._21 + pm1._33 * pm2._31 + pm1._34 * pm2._41;
        pout._32 = pm1._31 * pm2._12 + pm1._32 * pm2._22 + pm1._33 * pm2._32 + pm1._34 * pm2._42;
        pout._33 = pm1._31 * pm2._13 + pm1._32 * pm2._23 + pm1._33 * pm2._33 + pm1._34 * pm2._43;
        pout._34 = pm1._31 * pm2._14 + pm1._32 * pm2._24 + pm1._33 * pm2._34 + pm1._34 * pm2._44;
        pout._41 = pm1._41 * pm2._11 + pm1._42 * pm2._21 + pm1._43 * pm2._31 + pm1._44 * pm2._41;
        pout._42 = pm1._41 * pm2._12 + pm1._42 * pm2._22 + pm1._43 * pm2._32 + pm1._44 * pm2._42;
        pout._43 = pm1._41 * pm2._13 + pm1._42 * pm2._23 + pm1._43 * pm2._33 + pm1._44 * pm2._43;
        pout._44 = pm1._41 * pm2._14 + pm1._42 * pm2._24 + pm1._43 * pm2._34 + pm1._44 * pm2._44;
        return pout;
    }
}

//////////////////////////////////////////////////////
///     THE ELDER SCROLLS : OBLIVION REMASTERED    ///
//////////////////////////////////////////////////////

namespace Oblivion
{

}

OblivionConfig::OblivionConfig(const std::string& path) { load(path); }

void OblivionConfig::defaults(const std::string& filePath)
{
    data =
    {
        { "gobjects_sig", "48 8B 05 ? ? ? ? 48 8B 0C C8 4C 8D 04 D1 EB ? 4C 8B C6 41 8B 40 ? 0F BA E0 ? 72 ? 0F 1F 40 ? 8B C8"},  //  
        { "gnames_sig", "48 8D 05 ? ? ? ? EB ? 48 8D 0D ? ? ? ? E8 ? ? ? ? C6 05 ? ? ? ? ? 0F 10 07"},  //  
        { "gworld_sig", "48 8B 3D ? ? ? ? 48 85 FF 0F 84 ? ? ? ? F7 47"},                               //  
    };

    save(filePath);
}

//////////////////////////////////////////////////////
///               INSTANCE METHODS                 ///
//////////////////////////////////////////////////////

TESOblivion::TESOblivion()
{
    
    if (!g_memory.bAttached)
    {
        printf("[!][TESOblivion] failed attached to process.\n");
        return;
    }
    printf("[+][TESOblivion] attached to process.\n");

    const auto& dwModule = g_memory.GetProcessInfo().dwModuleBase;

    i64_t gobjects;
    if (g_memory.FindPattern(
        g_config.get<std::string>("gobjects_sig").c_str(),
        &gobjects,
        0,
        true,
        EASM::ASM_MOV
    ) && gobjects > 0 && UnrealEngine::Offsets::GObjects != gobjects - dwModule)	//	GObjects
    {
        //	update offset
        UnrealEngine::Offsets::GObjects = gobjects - dwModule;
        printf("[+][TESOblivion] gobjects offset updated.\n");
    }

    i64_t gnames;
    if (g_memory.FindPattern(
        g_config.get<std::string>("gnames_sig").c_str(),
        &gnames,
        0,
        true,
        EASM::ASM_LEA
    ) && gnames > 0 && UnrealEngine::Offsets::GNames != gnames - dwModule)	//	GNames
    {
        //	update offset
        UnrealEngine::Offsets::GNames = gnames - dwModule;
        printf("[+][TESOblivion] gnames offset updated.\n");
    }

    i64_t gworld;
    if (g_memory.FindPattern(
        g_config.get<std::string>("gworld_sig").c_str(), 
        &gworld,
        0,
        true,
        EASM::ASM_MOV
    ) && gworld > 0 && UnrealEngine::Offsets::GWorld != gworld - dwModule)	//	GWorld
    {
        //	update offset
        UnrealEngine::Offsets::GWorld = gworld - dwModule;
        printf("[+][TESOblivion] gworld offset updated.\n");
    }
}

TESOblivion::~TESOblivion()
{
}

void TESOblivion::update()
{
    SGlobals globals;
    SGame& game = globals.game;
    SLocalPlayer& localPlayer = globals.localPlayer;
    std::vector<SImGuiActor> actors;

    //  Get World
    game.pWorld = g_memory.Read<i64_t>(g_memory.GetAddress(UnrealEngine::Offsets::GWorld));
    if (!game.pWorld)
        return;

    //  Get ULevel , UGameInstance & AGameStateBase
    game.world = g_memory.Read<UnrealEngine::Classes::UWorld>(game.pWorld);
    if (!game.world.GameState || !game.world.OwningGameInstance || !game.world.PersistentLevel)
        return;

    //  Get Local Player , Controller , Pawn & Camera
    game.actors = g_memory.Read<UnrealEngine::TArray<i64_t>>(game.world.PersistentLevel + UnrealEngine::Offsets::Level::Actors);
    game.players = g_memory.Read<UnrealEngine::TArray<i64_t>>(game.world.GameState + UnrealEngine::Offsets::GameState::PlayerArray);
    i64_t pLocalPlayer = g_memory.Read<i64_t>(game.world.OwningGameInstance + UnrealEngine::Offsets::GameInstance::LocalPlayers);    //  local player array
    if (!pLocalPlayer)
        return;

    localPlayer.pULocalPlayer = g_memory.Read<i64_t>(pLocalPlayer);    //  pointer to ULocalPlayer
    if (!localPlayer.pULocalPlayer)
        return;

    localPlayer.pPlayerController = g_memory.Read<i64_t>(localPlayer.pULocalPlayer + UnrealEngine::Offsets::UPlayer::PlayerController);
    if (!localPlayer.pPlayerController)
        return;

    //  Get Local Player Components
	const auto& pLocalController = g_memory.Read<UnrealEngine::Classes::APlayerController>(localPlayer.pPlayerController);
    localPlayer.pCameraManager = pLocalController.PlayerCameraManager;
    localPlayer.pPawn = pLocalController.AcknowledgedPawn;
    localPlayer.sController = pLocalController;
    if (!localPlayer.pPawn || !localPlayer.pCameraManager)
        return;

    //  Get Actors
	std::unique_ptr<__int64[]> actorsArray = std::make_unique<__int64[]>(game.actors.count);
    if (!g_memory.ReadMemory(game.actors.data, actorsArray.get(), game.actors.count * sizeof(__int64)))
        return;

    for (int i = 0; i < game.actors.count; i++)
    {
        auto actor = actorsArray[i];// g_memory.Read<i64_t>(game.actors.data + (i * 0x8));
        if (!actor)
            continue;

        const auto& actorObject = g_memory.Read<UnrealEngine::Classes::AActor>(actor);
		if (!actorObject.RootComponent)
			continue;

        SImGuiActor imActor;
        const auto& skeletalMesh = g_memory.Read<UnrealEngine::Classes::USkinnedMeshComponent>(actorObject.RootComponent);
        imActor.TM = {
            (skeletalMesh.UMeshComponent.UPrimitiveComponent.USceneComponent.RelativeLocation),
            (skeletalMesh.UMeshComponent.UPrimitiveComponent.USceneComponent.RelativeRotation),
            (skeletalMesh.UMeshComponent.UPrimitiveComponent.USceneComponent.RelativeScale3D),
            (skeletalMesh.UMeshComponent.UPrimitiveComponent.USceneComponent.ComponentVelocity)
        };
        imActor.CTW = (skeletalMesh.UMeshComponent.UPrimitiveComponent.USceneComponent.ComponentToWorld);
        imActor.object = actorObject.UObject;
        imActor.pEntity = actor;
        UnrealEngine::Tools::GetObjectName(actor, &imActor.name);

        if (actor == localPlayer.pPawn)
        {
            localPlayer.CTW = imActor.CTW;
            localPlayer.TM = imActor.TM;

            continue;
        }

        actors.push_back(imActor);
    }
    globals.render.actors = actors;

    //  Get Camera View
    globals.CameraView = g_memory.Read<UnrealEngine::FCameraCacheEntry>(localPlayer.pCameraManager + UnrealEngine::Offsets::APlayerCameraManager::CameraCachePrivate);

    //  set globals
    m_imCache = globals;
}

void TESOblivion::shutdown()
{

    //  DISABLE PATCHES
    if (bFlyMode)
        NoClip(false);

    if (bFullbright)
        Fullbright(false);

    g_memory.Detach();
}

//////////////////////////////////////////////////////
///                 STATIC METHODS                 ///
//////////////////////////////////////////////////////

i64_t TESOblivion::GetWorld()
{
    return g_memory.Read<i64_t>(g_memory.GetProcessInfo().dwModuleBase + UnrealEngine::Offsets::GWorld);
}

i64_t TESOblivion::GetLocalUPlayer(i64_t gWorld)
{
    auto pGameInstance = g_memory.Read<i64_t>(gWorld + UnrealEngine::Offsets::World::OwningGameInstance);
    if (!pGameInstance)
        return 0;

    auto pLocalPlayers = g_memory.Read<i64_t>(pGameInstance + UnrealEngine::Offsets::GameInstance::LocalPlayers);
    if (!pLocalPlayers)
        return 0;

    return g_memory.Read<i64_t>(pLocalPlayers);
}

i64_t TESOblivion::GetLocalPlayerController(i64_t uPlayer)
{
    return g_memory.Read<i64_t>(uPlayer + UnrealEngine::Offsets::UPlayer::PlayerController);
}

i64_t TESOblivion::GetLocalPlayerState(i64_t pController)
{
    return g_memory.Read<i64_t>(pController + UnrealEngine::Offsets::Controller::PlayerState);
}

i64_t TESOblivion::GetLocalPlayerPawn(i64_t pController)
{
    return g_memory.Read<i64_t>(pController + UnrealEngine::Offsets::Controller::AcknowledgedPawn);
}

i64_t TESOblivion::GetLocalPlayerCamera(i64_t pController)
{
    return g_memory.Read<i64_t>(pController + UnrealEngine::Offsets::Controller::PlayerCameraManager);
}

bool TESOblivion::GetLocalCameraView(i64_t pCamera, UnrealEngine::FCameraCacheEntry* view)
{
    if (!pCamera)
        return false;

    *view = g_memory.Read<UnrealEngine::FCameraCacheEntry>(pCamera + UnrealEngine::Offsets::APlayerCameraManager::CameraCachePrivate);

    return true;
}

bool TESOblivion::GetPlayerPosition(i64_t pPawn, UnrealEngine::FVector* out)
{
    if (!pPawn)
        return false;

    auto pRoot = g_memory.Read<i64_t>(pPawn + UnrealEngine::Offsets::Pawn::RootComponent);
    if (!pRoot)
        return false;

    *out = g_memory.Read<UnrealEngine::FVector>(pRoot + UnrealEngine::Offsets::USceneComponent::RelativeLocation);

    return true;
}

bool TESOblivion::GetPlayerRotation(i64_t pPawn, UnrealEngine::FRotator* out)
{
    auto pRoot = g_memory.Read<i64_t>(pPawn + UnrealEngine::Offsets::Pawn::RootComponent);
    if (!pRoot)
        return false;

    *out = g_memory.Read<UnrealEngine::FRotator>(pRoot + UnrealEngine::Offsets::USceneComponent::RelativeRotation);

    return true;
}

bool TESOblivion::GetActorArray(i64_t gWorld, std::vector<i64_t>* actors)
{
    std::vector<i64_t> _result;

    auto pLevel = g_memory.Read<i64_t>(gWorld + UnrealEngine::Offsets::World::PersistentLevel);
    if (!pLevel)
        return false;

    UnrealEngine::TArray players = g_memory.Read<UnrealEngine::TArray<i64_t>>(pLevel + UnrealEngine::Offsets::Level::Actors);
    for (int i = 0; i < players.count; i++)
    {
        auto ent = g_memory.Read<i64_t>(players.data + (i * 0x8));
        if (!ent)
            continue;

        _result.push_back(ent);
    }

    if (_result.size() <= 0)
        return false;

    actors->clear();
    *actors = _result;

    return true;
}

bool TESOblivion::GetPlayerArray(i64_t gWorld, std::vector<i64_t>* actors)
{
    std::vector<i64_t> result;

    i64_t pGameState = g_memory.Read<i64_t>(gWorld + UnrealEngine::Offsets::World::GameState);
    if (!pGameState)
        return false;

    UnrealEngine::TArray players = g_memory.Read<UnrealEngine::TArray<i64_t>>(pGameState + UnrealEngine::Offsets::GameState::PlayerArray);
    for (int i = 0; i < players.count; i++)
    {
        __int32 index = i * 8;
        auto ent = g_memory.Read<i64_t>(players.data + (i * 0x8));
        if (!ent)
            continue;

        result.push_back(ent);
    }

    actors->clear();
    *actors = result;

    return result.size() > 0;
}

bool TESOblivion::GetPlayerBonePosByIndex(i64_t pPawn, int index, UnrealEngine::FVector* bone)
{
    auto pMesh = g_memory.Read<i64_t>(pPawn + UnrealEngine::Offsets::Character::Mesh);
    if (!pMesh)
        return false;

    auto bones = g_memory.Read<UnrealEngine::TArray<i64_t>>(pMesh + UnrealEngine::Offsets::USkeletalMeshComponent::BoneArray);
    if (!bones.data || bones.count <= 0 || bones.max <= 0)
        return false;

    auto p = (bones.data + (index * sizeof(UnrealEngine::FTransform)));
    if (!p)
        return false;

    auto bone_root = g_memory.Read<UnrealEngine::FTransform>(p);
    auto transform = g_memory.Read<UnrealEngine::FTransform>(pMesh + UnrealEngine::Offsets::USkeletalMeshComponent::ComponentToWorld);

    *bone = (bone_root.Translation + transform.Translation);

    return true;
}

//////////////////////////////////////////////////////
///                    FEATURES                    ///
//////////////////////////////////////////////////////

void TESOblivion::Fullbright(bool bEnable) { UnrealEngine::Tools::SetViewMode(bEnable ? 2 : 5); }

void TESOblivion::NoClip(bool bEnable) { UnrealEngine::Tools::SetMovementMode(bEnable ? 5 : 1); }