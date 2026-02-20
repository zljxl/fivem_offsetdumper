#pragma once

#include "includes.h"

#include <xmmintrin.h>

struct Vector3Fix
{
public:
	Vector3Fix() = default;

	Vector3Fix(float x, float y, float z) :
		x(x), y(y), z(z)
	{}
public:
	float x{};
private:
	char m_padding1[0x04]{};
public:
	float y{};
private:
	char m_padding2[0x04]{};
public:
	float z{};
private:
	char m_padding3[0x04]{};
};

struct Vec3
{
	float x, y, z;
	void Normalize() {
		while (y < -180) {
			y = 360;
		}
		while (y > 180) {
			y = -360;
		}

		while (x > 89) {
			x = -89;
		}
		while (x < -89) {
			x = 89;
		}
	}

	Vec3 operator - (Vec3 i) {
		return {
		  x - i.x,
		  y - i.y,
		  z - i.z
		};
	}

	Vec3 operator + (Vec3 i) {
		return {
		  x + i.x,
		  y + i.y,
		  z + i.z
		};
	}

	Vec3 operator / (float i) {
		return {
		  x / i,
		  y / i,
		  z / i
		};
	}

	Vec3 operator * (float i) {
		return {
		  x * i,
		  y * i,
		  z * i
		};
	}

	static Vec3 FromM128(__m128 in)
	{
		return Vec3
		{
			in.m128_f32[0],
			in.m128_f32[1],
			in.m128_f32[2]
		};
	}
};

struct Color
{
public:
	alignas(8) int r;
	alignas(8) int g;
	alignas(8) int b;

public:
	Color()
		: r(0)
		, g(0)
		, b(0)
	{ }

	Color(int r, int g, int b)
		: r(r)
		, g(g)
		, b(b)
	{ }

	Color(int arr[3])
		: r(arr[0])
		, g(arr[1])
		, b(arr[2])
	{ }
};

class Vector3 final
{
public:
	float x, y, z;

	Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
	Vector3 operator + (const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
	Vector3 operator - (const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
	Vector3 operator * (const float& rhs) const { return Vector3(x * rhs, y * rhs, z * rhs); }
	Vector3 operator / (const float& rhs) const { return Vector3(x / rhs, y / rhs, z / rhs); }
	bool operator == (const Vector3& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
	Vector3& operator += (const Vector3& rhs) { return *this = *this + rhs; }
	Vector3& operator -= (const Vector3& rhs) { return *this = *this - rhs; }
	Vector3& operator *= (const float& rhs) { return *this = *this * rhs; }
	Vector3& operator /= (const float& rhs) { return *this = *this / rhs; }
	float Length() const { return sqrt(x * x + y * y + z * z); }
	Vector3 Normalize() const { return *this * (1 / Length()); }
	Vector3 Multiple(const Vector3 caca) const { return Vector3(x * caca.x, y * caca.y, z * caca.z); }

	float Distance(const Vector3& rhs) const { return (*this - rhs).Length(); }
	void Invert() { *this *= -1; }
	static Vector3 FromM128(__m128 in) { return Vector3(in.m128_f32[0], in.m128_f32[1], in.m128_f32[2]); }
};

class Vector4
{
public:
	float x, y, z, w;
};

namespace Rogerio
{
	using GetBonesPosition_t = Vector3 * (*)(uintptr_t pedptr, __m128* tempVec4, int32_t wMask);
	inline GetBonesPosition_t GetBonePosition;

	inline uint64_t W2S;

	ImVec2 WorldToScreen(Vector3 pos)
	{
		auto& io = ImGui::GetIO();
		ImVec2 tempVec2;
		reinterpret_cast<bool(__fastcall*)(Vector3*, float*, float*)>(W2S)(&pos, &tempVec2.x, &tempVec2.y);
		tempVec2.x *= io.DisplaySize.x;
		tempVec2.y *= io.DisplaySize.y;
		return tempVec2;
	}
}

enum PedTypes
{
	PLAYER_0, // michael
	PLAYER_1, // franklin
	NETWORK_PLAYER, // mp character
	PLAYER_2, // trevor
	CIVMALE,
	CIVFEMALE,
	COP,
	GANG_ALBANIAN,
	GANG_BIKER_1,
	GANG_BIKER_2,
	GANG_ITALIAN,
	GANG_RUSSIAN,
	GANG_RUSSIAN_2,
	GANG_IRISH,
	GANG_JAMAICAN,
	GANG_AFRICAN_AMERICAN,
	GANG_KOREAN,
	GANG_CHINESE_JAPANESE,
	GANG_PUERTO_RICAN,
	DEALER,
	MEDIC,
	FIREMAN,
	CRIMINAL,
	BUM,
	PROSTITUTE,
	SPECIAL,
	MISSION,
	SWAT,
	ANIMAL,
	ARMY
};

enum PedBones
{
	HEAD,
	L_FOOT,
	R_FOOT,
	L_ANKLE,
	R_ANKLE,
	L_HAND,
	R_HAND,
	NECK,
	ABDOMEN
};

class cModelInfo {
public:
	uint32_t GetModelHash() {
		if (!this) return 0;
		return *(uint32_t*)(this + 0x18);
	}
	uint32_t SetModelHash(uint32_t hash) {
		if (!this) return 0;
		return *(uint32_t*)(this + 0x18) = hash;
	}
	Vector3 GetMin()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x30);
	}
	Vector3 GetMax()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x40);
	}
	const char* GetVehModelName()
	{
		if (!this) return 0;
		return (const char*)(char*)(this + 0x298);
	}
};

class cCollisions3
{
public:
	bool IsPedCollision()
	{
		if (!this) return false;
		if (*(float*)(this + 0x2C) != 0.25)
			return true;
		else
			return false;
	}
	bool SetPedNoCollision(bool b)
	{
		if (!this) return false;
		if (b == true)
			*(float*)(this + 0x2C) = -1.f;

		return true;
	}
};
class cCollisions2
{
public:
	cCollisions3* Collisions() {
		if (!this) return 0;
		return (cCollisions3*)(*(uint64_t*)(this + 0x0));
	}
};
class cCollisions1
{
public:
	cCollisions2* Collisions() {
		if (!this) return 0;
		return (cCollisions2*)(*(uint64_t*)(this + 0x70));
	}
};
class cCollisions
{
public:
	cCollisions1* Collisions() {
		if (!this) return 0;
		return (cCollisions1*)(*(uint64_t*)(this + 0x20));
	}

	bool IsWaterCollision()
	{
		if (!this) return false;
		if (*(float*)(this + 0x54) != 1.f)
			return true;
		else
			return false;
	}
	bool SetWaterCollision(bool b)
	{
		if (!this) return false;
		if (b == true && *(float*)(this + 0x54) != 0.f)
			*(float*)(this + 0x54) = 0.f;
		else if (b == false && *(float*)(this + 0x54) != 1.f)
			*(float*)(this + 0x54) = 1.f;

		return true;
	}
};

class cNavigation
{
public:
	cCollisions* Collisions() {
		if (!this) return 0;
		return (cCollisions*)(*(uint64_t*)(this + 0x10));
	}

	float GetEntityHeading1()
	{
		if (!this) return 0.f;
		return *(float*)(this + 0x20);
	}
	float SetEntityHeading1(float h)
	{
		if (!this) return 0.f;
		return *(float*)(this + 0x20) = h;
	}

	float GetEntityHeading2()
	{
		if (!this) return 0.f;
		return *(float*)(this + 0x24);
	}
	float SetEntityHeading2(float h)
	{
		if (!this) return 0.f;
		return *(float*)(this + 0x24) = h;
	}

	Vector4 SetRotation(Vector4 Coords)
	{
		if (!this) return Vector4{ 0,0,0,0 };
		return *(Vector4*)(this + 0x30) = Coords;
	}

	Vector3 GetEntityCoords()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x50);
	}
	Vector3 SetEntityCoords(Vector3 Cords)
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x50) = Cords;
	}
};

class cVehicleVisual
{
public:
	Color GetVehPrimaryColorRGB()
	{
		if (!this) return 0;
		return Color(*(int*)(this + 0xA6), *(int*)(this + 0xA5), *(int*)(this + 0xA4));
	}
	Color SetVehPrimaryColorRGB(Color rgb)
	{
		if (!this) return 0;
		*(int*)(this + 0xA6) = rgb.r;
		*(int*)(this + 0xA5) = rgb.g;
		*(int*)(this + 0xA4) = rgb.b;
		return this->GetVehPrimaryColorRGB();
	}

	Color GetVehSecondaryColorRGB()
	{
		if (!this) return 0;
		return Color(*(int*)(this + 0xAA), *(int*)(this + 0xA9), *(int*)(this + 0xA8));
	}
	Color SetVehSecondaryColorRGB(Color rgb)
	{
		if (!this) return 0;
		*(int*)(this + 0xAA) = rgb.r;
		*(int*)(this + 0xA9) = rgb.g;
		*(int*)(this + 0xA8) = rgb.b;
		return this->GetVehPrimaryColorRGB();
	}
};

class cVehicleDrawHandler
{
public:
	cVehicleVisual* VehVisual() {
		if (!this) return 0;
		return (cVehicleVisual*)(*(uint64_t*)(this + 0x20));
	}

	Color GetNeonRGB()
	{
		if (!this) return 0;
		return Color(*(int*)(this + 0x3A2), *(int*)(this + 0x3A1), *(int*)(this + 0x3A0));
	}
	Color SetNeonRGB(Color rgb)
	{
		if (!this) return 0;
		*(int*)(this + 0x3A2) = rgb.r;
		*(int*)(this + 0x3A1) = rgb.g;
		*(int*)(this + 0x3A0) = rgb.b;
		return this->GetNeonRGB();
	}

	bool EnableNeon(bool b)
	{
		if (!this) return 0;
		if (b == true)
		{
			*(int*)(this + 0x402) = 1; // Left
			*(int*)(this + 0x403) = 1; // Right
			*(int*)(this + 0x404) = 1; // Front
			*(int*)(this + 0x405) = 1; // Back
		}
		return true;
	}

	uint8_t GetEMSLevel()
	{
		if (!this) return 0;
		return *(uint8_t*)(this + 0x3D6);
	}
	uint8_t SetEMSLevel(uint8_t bt)
	{
		if (!this) return 0;
		return *(uint8_t*)(this + 0x3D6) = bt;
	}

	uint8_t GetBrakesLevel()
	{
		if (!this) return 0;
		return *(uint8_t*)(this + 0x3D7);
	}
	uint8_t SetBrakesLevel(uint8_t bt)
	{
		if (!this) return 0;
		return *(uint8_t*)(this + 0x3D7) = bt;
	}

	uint8_t GetTransmissionLevel()
	{
		if (!this) return 0;
		return *(uint8_t*)(this + 0x3D8);
	}
	uint8_t SetTransmissionLevel(uint8_t bt)
	{
		if (!this) return 0;
		return *(uint8_t*)(this + 0x3D8) = bt;
	}

	uint8_t GetHornNumber()
	{
		if (!this) return 0;
		return *(uint8_t*)(this + 0x3D9);
	}
	uint8_t SetHornNumber(uint8_t bt)
	{
		if (!this) return 0;
		return *(uint8_t*)(this + 0x3D9) = bt;
	}

	uint8_t GetSuspensionLevel()
	{
		if (!this) return 0;
		return *(uint8_t*)(this + 0x3DA);
	}
	uint8_t SetSuspensionLevel(uint8_t bt)
	{
		if (!this) return 0;
		return *(uint8_t*)(this + 0x3DA) = bt;
	}

	uint8_t GetArmorLevel()
	{
		if (!this) return 0;
		return *(uint8_t*)(this + 0x3DB);
	}
	uint8_t SetArmorLevel(uint8_t bt)
	{
		if (!this) return 0;
		return *(uint8_t*)(this + 0x3DB) = bt;
	}

	bool IsHaveTurbo()
	{
		if (!this) return 0;
		if (*(uint8_t*)(this + 0x3DD) != 1)
			return false;
		else
			return true;
	}
	bool EnableTurbo(bool b)
	{
		if (!this) return 0;
		if (b == true && *(uint8_t*)(this + 0x3DD) != 1)
			*(uint8_t*)(this + 0x3DD) = 1;
		else if (b == false && (*(uint8_t*)(this + 0x3DD) != 0 || *(uint8_t*)(this + 0x3DD) != 255))
			*(uint8_t*)(this + 0x3DD) = 0;
		return true;
	}

	uint8_t GetXenonLightColor()
	{
		if (!this) return 0;
		return *(uint8_t*)(this + 0x3E1);
	}
	uint8_t SetXenonLightColor(uint8_t bt)
	{
		if (!this) return 0;
		return *(uint8_t*)(this + 0x3E1) = bt;
	}

	Color GetTyreSmokeRGB()
	{
		if (!this) return 0;
		return Color(*(int*)(this + 0x3FC), *(int*)(this + 0x3FD), *(int*)(this + 0x3FE));
	}
	Color SetTyreSmokeRGB(Color rgb)
	{
		if (!this) return 0;
		*(int*)(this + 0x3FC) = rgb.r;
		*(int*)(this + 0x3FD) = rgb.g;
		*(int*)(this + 0x3FE) = rgb.b;
		return this->GetNeonRGB();
	}

	uint8_t GetWindowLevel()
	{
		if (!this) return 0;
		return *(uint8_t*)(this + 0x3FF);
	}
	uint8_t SetWindowLevel(uint8_t bt)
	{
		if (!this) return 0;
		return *(uint8_t*)(this + 0x3FF) = bt;
	}
};

class cVehicleHandling
{
public:
	float SetVehMass(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x0C) = fl;
	}
	float SetInitialDragCoeff(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x0C) = fl;
	}
	float SetDownForceMultp(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x0C) = fl;
	}
	float SetPopupLightRotation(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x0C) = fl;
	}
	Vector3 SetCentreOfMass(Vector3 coords)
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x20) = coords;
	}
	Vector3 SetInertiaMultp(Vector3 coords)
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x30) = coords;
	}
	float SetBuoyancy(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x40) = fl;
	}
	float SetDriveBiasRear(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x44) = fl;
	}
	float SetDriveBiasFront(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x48) = fl;
	}
	float SetAccelerration(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x4C) = fl;
	}
	uint8_t SetInitialDriveGears(uint8_t bt)
	{
		if (!this) return 0;
		return *(uint8_t*)(this + 0x50) = bt;
	}
	float SetDriveInertia(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x54) = fl;
	}
	float SetUpShift(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x58) = fl;
	}
	float SetDownShift(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x5C) = fl;
	}
	float SetInitialDriveForce(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x60) = fl;
	}
	float SetDriveMaxFlatVelocity(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x64) = fl;
	}
	float SetInitialDriveMaxFlatVel(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x68) = fl;
	}
	float SetBrakeForce(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x6C) = fl;
	}
	float SetBrakeBiasFront(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x74) = fl;
	}
	float SetBrakeBiasRear(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x78) = fl;
	}
	float SetHandBrakeForce(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x7C) = fl;
	}
	float SetSteeringLock(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x80) = fl;
	}
	float SetSteeringLockRatio(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x84) = fl;
	}
	float SetTractionCurveMax(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x88) = fl;
	}
	float SetTractionCurveLateral(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x8C) = fl;
	}
	float SetTractionCurveMin(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x90) = fl;
	}
	float SetTractionCurveRatio(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x94) = fl;
	}
	float SetCurveLateral(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x98) = fl;
	}
	float SetCurveLateralRatio(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x9C) = fl;
	}
	float SetTractionSpringDeltaMax(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xA0) = fl;
	}
	float SetTractionSpringDeltaMaxRatio(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xA4) = fl;
	}
	float SetLowSpeedTractionLossMultp(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xA8) = fl;
	}
	float SetCamberStiffness(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xAC) = fl;
	}
	float SetTractionBiasFront(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xB0) = fl;
	}
	float SetTractionBiasRear(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xB4) = fl;
	}
	float SetTractionLossMultp(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xB8) = fl;
	}
	float SetSuspensionForce(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xBC) = fl;
	}
	float SetSuspensionCompDamp(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xC0) = fl;
	}
	float SetSuspensionReboundDamp(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xC4) = fl;
	}
	float SetSuspensionUpperLimit(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xC8) = fl;
	}
	float SetSuspensionLowerLimit(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xCC) = fl;
	}
	float SetSuspensionRaise(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xD0) = fl;
	}
	float SetSuspensionBiasFront(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xD4) = fl;
	}
	float SetSuspensionBiasRear(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xD8) = fl;
	}
	float SetAntRollBarForce(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xDC) = fl;
	}
	float SetAntRollBarBiasFront(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xE0) = fl;
	}
	float SetAntRollBarBiasRear(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xE4) = fl;
	}
	float SetRollCentreHeightFront(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xE8) = fl;
	}
	float SetRollCentreHeightRear(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xEC) = fl;
	}
	float SetWeaponDamageMultp(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xF0) = fl;
	}
	float SetCollisionDamageMultp(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xF4) = fl;
	}
	float SetDeformationMultp(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xF8) = fl;
	}
	float SetEngineDamageMultp(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0xFC) = fl;
	}
	float SetPetrolTankVolume(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x100) = fl;
	}
	float SetOilVolume(float fl)
	{
		if (!this) return 0;
		return *(float*)(this + 0x104) = fl;
	}
	Vector3 SetSeatOffsetDist(Vector3 coords)
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x10C) = coords;
	}
};


class cVehicle
{
public:
	cModelInfo* ModelInfo() {
		if (!this) return 0;
		return (cModelInfo*)(*(uint64_t*)(this + 0x20));
	}

	cNavigation* Navigation() {
		if (!this) return 0;
		return (cNavigation*)(*(uint64_t*)(this + 0x30));
	}

	cVehicleDrawHandler* VehDrawHandler() {
		if (!this) return 0;
		return (cVehicleDrawHandler*)(*(uint64_t*)(this + 0x48));
	}

	cVehicleHandling* VehHandling() {
		if (!this) return 0;
		return (cVehicleHandling*)(*(uint64_t*)(this + 0x938));
	}

	bool IsDriver() {
		if (!this) return 0;
		if ((void*)(*(uint64_t*)(this + 0xC68)) != nullptr)
			return true;
		else
			return false;
	}

	bool IsVehInvisible()
	{
		if (!this) return 0;
		if (*(uint8_t*)(this + 0x2C) != 15)
			return true;
		else
			return false;
	}
	bool SetVehInvisible(bool b)
	{
		if (!this) return 0;
		if (b == true && *(uint8_t*)(this + 0x2C) != 14)
			*(uint8_t*)(this + 0x2C) = 14;
		else if (b == false && *(uint8_t*)(this + 0x2C) != 15)
			*(uint8_t*)(this + 0x2C) = 15;
		return this->IsVehInvisible();
	}

	Vector3 GetVehCamCoords()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x90);
	}
	Vector3 SetVehCamCoords(Vector3 Cords)
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x90) = Cords;
	}

	bool IsVehGod()
	{
		if (!this) return 0;
		if (*(uint8_t*)(this + 0x189) != 0)
			return true;
		else
			return false;
	}
	bool SetVehGod(bool b)
	{
		if (!this) return 0;
		if (b == true && *(uint8_t*)(this + 0x189) != 2)
			*(uint8_t*)(this + 0x189) = 2;
		else if (b == false && *(uint8_t*)(this + 0x189) != 0)
			*(uint8_t*)(this + 0x189) = 0;

		return true;
	}

	bool IsVehBurnt()
	{
		if (!this) return 0;
		if (*(uint8_t*)(this + 0x18A) != 64)
			return true;
		else
			return false;
	}
	bool SetVehBurnt(bool b)
	{
		if (!this) return 0;
		if (b == true && *(uint8_t*)(this + 0x18A) != 74)
			*(uint8_t*)(this + 0x18A) = 74;
		else if (b == false && *(uint8_t*)(this + 0x18A) != 64)
			*(uint8_t*)(this + 0x18A) = 64;

		return true;
	}

	Vector3 GetVehVelocity()
	{
		if (!this) return Vector3(0, 0, 0);
		return *(Vector3*)(this + 0x320);
	}
	Vector3 SetVehVelocity()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x320) = Vector3(0, 0, 0);
	}

	float GetVehBodyHealth()
	{
		if (!this) return 0;
		return *(float*)(this + 0x840);
	}
	float SetVehBodyHealth(float f)
	{
		if (!this) return 0;
		return *(float*)(this + 0x840) = f;
	}
	float GetVehPetrolTankHealth()
	{
		if (!this) return 0;
		return *(float*)(this + 0x844);
	}
	float SetVehPetrolTankHealth(float f)
	{
		if (!this) return 0;
		return *(float*)(this + 0x844) = f;
	}
	float GetVehEngineHealth()
	{
		if (!this) return 0;
		return *(float*)(this + 0x908);
	}
	float SetVehEngineHealth(float f)
	{
		if (!this) return 0;
		return *(float*)(this + 0x908) = f;
	}
	float SetVehicleHealth(float f)
	{
		if (!this) return 0;
		return *(float*)(this + 0x840) = f;
	}
	float SetVehicleHealth1(float f)
	{
		if (!this) return 0;
		return *(float*)(this + 0x844) = f;
	}
	float GetVehicleHealth2()
	{
		if (!this) return 0;
		return *(float*)(this + 0x280);
	}
	float SetVehicleHealth2(float f)
	{
		if (!this) return 0;
		return *(float*)(this + 0x280) = f;
	}

	bool TyresCanBurst(bool b)
	{
		if (!this) return 0;
		if (b == true && *(uint8_t*)(this + 0x943) != 32)
			*(uint8_t*)(this + 0x943) = 32;
		else if (b == false && *(uint8_t*)(this + 0x943) != 1)
			*(uint8_t*)(this + 0x943) = 1;
		return true;
	}

	bool DeformGod(bool b)
	{
		if (!this) return 0;
		if (b == true && *(uint8_t*)(this + 0x944) != 140)
			*(uint8_t*)(this + 0x944) = 140;
		else if (b == false && *(uint8_t*)(this + 0x944) != 156)
			*(uint8_t*)(this + 0x944) = 156;
		return true;
	}

	float SetVehDirtLevel(float f)
	{
		if (!this) return 0;
		return *(float*)(this + 0x9F8) = f;
	}

	bool LowGripTyres(bool b)
	{
		if (!this) return 0;
		if (b == true && *(uint8_t*)(this + 0xAC2) != 1)
			*(uint8_t*)(this + 0xAC2) = 1;
		else if (b == false && *(uint8_t*)(this + 0xAC2) != 0)
			*(uint8_t*)(this + 0xAC2) = 0;
		return true;
	}

	float SetVehGravity(float f)
	{
		if (!this) return 0;
		return *(float*)(this + 0xC5C) = f;
	}

	bool IsLocked()
	{
		if (!this) return 0;
		if (*(uint8_t*)(this + 0x1390) != 1)
			return true;
		else
			return false;
	}
	uint8_t SetDoorsStatus(uint8_t b)
	{
		if (!this) return 0;
		return *(uint8_t*)(this + 0x1390) = b;
	}
};

class cOxygenInfo
{
public:
	float RestorePlayerOxygen()
	{
		if (!this) return 0;
		return *(float*)(this + 0x278) = 0.f;
	}
};

class cPlayerInfo
{
public:
	float GetSwimSpeedMultp()
	{
		if (!this) return 0;
		return *(float*)(this + 0x170);
	}
	float SetSwimSpeedMultp(float f)
	{
		if (!this) return 0;
		return *(float*)(this + 0x170) = f;
	}
	float GetSprintSpeedMultp()
	{
		if (!this) return 0;
		return *(float*)(this + 0xCF0);
	}
	float SetSprintSpeedMultp(float f)
	{
		if (!this) return 0;
		return *(float*)(this + 0xCF0) = f;
	}
	float SetStamina(float f)
	{
		if (!this) return 0;
		return *(float*)(this + 0xCF4) = f;
	}
	bool SetPunchForceMultp(bool b)
	{
		if (!this) return 0;
		if (b == true && *(float*)(this + 0xD1C) != 999999.f)
			*(float*)(this + 0xD1C) = 999999.f;
		else if (b == true && *(float*)(this + 0xD1C) != 1.f)
			*(float*)(this + 0xD1C) = 1.f;
		return true;
	}
	uint32_t UpdateFrameFlag(uint32_t bt)
	{
		if (!this) return 0;
		return *(uint32_t*)(this + 0x219) = bt;
	}
};

class  cFixedAmmoCount
{
public:
	int32_t GetAmmo()
	{
		if (!this) return 0;
		return *(int32_t*)(this + 0x18);
	}
	int32_t SetAmmo(int i)
	{
		if (!this) return 0;
		return *(int32_t*)(this + 0x18) = i;
	}
};
class cAmmoCount
{
public:
	cFixedAmmoCount* FixedAmmoCount()
	{
		if (!this) return 0;
		return (cFixedAmmoCount*)(*(uint64_t*)(this + 0x0));
	}
};
class cAmmoInfo
{
public:
	cAmmoCount* AmmoCount()
	{
		if (!this) return 0;
		return (cAmmoCount*)(*(uint64_t*)(this + 0x8));
	}
	float SetSpeedBullet(float f)
	{
		if (!this) return 0;
		return *(float*)(this + 0x58) = f;
	}
};

class cWeaponInfo
{
public:
	cAmmoInfo* AmmoInfo()
	{
		if (!this) return 0;
		return (cAmmoInfo*)(*(uint64_t*)(this + 0x60));
	}

	uint32_t GetWeaponNameHash()
	{
		if (!this) return 0;
		return *(uint32_t*)(this + 0x10);
	}

	uint32_t GetWeaponModelHash()
	{
		if (!this) return 0;
		return *(uint32_t*)(this + 0x14);
	}

	float GetWeaponDamage()
	{
		if (!this) return 0;
		return *(float*)(this + 0xB0);
	}
	float SetWeaponDamage(float f)
	{
		if (!this) return 0;
		return *(float*)(this + 0xB0) = f;
	}

	float GetForceOnPed()
	{
		if (!this) return 0;
		return *(float*)(this + 0xDC);
	}
	float SetForceOnPed(float f)
	{
		if (!this) return 0;
		return *(float*)(this + 0xDC) = f;
	}

	float GetForceOnVehicle()
	{
		if (!this) return 0;
		return *(float*)(this + 0xE0);
	}
	float SetForceOnVehicle(float f)
	{
		if (!this) return 0;
		return *(float*)(this + 0xE0) = f;
	}

	float GetForceOnHeli()
	{
		if (!this) return 0;
		return *(float*)(this + 0xE4);
	}
	float SetForceOnHeli(float f)
	{
		if (!this) return 0;
		return *(float*)(this + 0xE4) = f;
	}

	float GetWeaponPenetration()
	{
		if (!this) return 0;
		return *(float*)(this + 0x110);
	}
	float SetWeaponPenetration(float f)
	{
		if (!this) return 0;
		return *(float*)(this + 0x110) = f;
	}
};

class cWeaponManager
{
public:
	cWeaponInfo* WeaponInfo() {
		if (!this) return 0;
		return (cWeaponInfo*)(*(uint64_t*)(this + 0x20));
	}
};

class cInfiniteAmmo
{
public:
	bool InfiniteAmmo(bool b)
	{
		if (!this) return 0;
		if (b == true && *(uint8_t*)(this + 0x78) != 1)
			*(uint8_t*)(this + 0x78) = 1;
		else if (b == false && *(uint8_t*)(this + 0x78) != 0)
			*(uint8_t*)(this + 0x78) = 0;
		return true;
	}

	bool InfiniteAmmoClip(bool b)
	{
		if (!this) return 0;
		if (b == true && *(uint8_t*)(this + 0x78) != 2)
			*(uint8_t*)(this + 0x78) = 2;
		else if (b == false && *(uint8_t*)(this + 0x78) != 0)
			*(uint8_t*)(this + 0x78) = 0;

		return true;
	}
};

class cPed {
public:
	cModelInfo* ModelInfo() {
		if (!this) return 0;
		return (cModelInfo*)(*(uint64_t*)(this + 0x20));
	}

	cNavigation* Navigation() {
		if (!this) return 0;
		return (cNavigation*)(*(uint64_t*)(this + 0x30));
	}

	cOxygenInfo* OxygenInfo() {
		if (!this) return 0;
		return (cOxygenInfo*)(*(uint64_t*)(this + 0x10C0));
	}

	cInfiniteAmmo* InfiniteAmmo() {
		if (!this) return 0;
		return (cInfiniteAmmo*)(*(uint64_t*)(this + 0x10D0));
	}

	Vector3 GetPedCamCoords()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x90);
	}
	Vector3 SetPedCamCoords(Vector3 coords)
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x90) = coords;
	}

	Vector3 SetPedVelocity()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x320) = Vector3(0, 0, 0);
	}

	BYTE btPedGod()
	{
		if (!this) return 0;
		return *(BYTE*)(this + 0x189);
	}
	BYTE SetPedGod(BYTE bt)
	{
		if (!this) return 0;
		return *(BYTE*)(this + 0x189) = bt;
	}

	bool IsPedInvisible()
	{
		if (!this) return 0;

		if (*(uint8_t*)(this + 0xAC) >= 100)
			return false;
		else
			return true;
	}
	bool SetPedInvisible(bool inv)
	{
		if (!this) return 0;

		if (inv == true && *(uint8_t*)(this + 0x2C) != 54)
			*(uint8_t*)(this + 0x2C) = 54;
		else if (inv == false && *(uint8_t*)(this + 0x2C) != 55)
			*(uint8_t*)(this + 0x2C) = 55;

		return this->IsPedInvisible();
	}

	float GetPedHealth()
	{
		if (!this) return 0;
		return *(float*)(this + 0x280);
	}
	float SetPedHealth(float health)
	{
		if (!this) return 0;
		return *(float*)(this + 0x280) = health;
	}

	float GetPedMaxHealth()
	{
		if (!this) return 0;
		return *(float*)(this + 0x2A0);
	}
};

class cPedList
{
public:
	cPed* Ped(int index)
	{
		if (!this) return 0;
		return (cPed*)(*(uint64_t*)(this + (index * 0x10)));
	}
	uintptr_t PedUintPtr(int index)
	{
		if (!this) return 0;
		return *(uintptr_t*)(this + (index * 0x10));
	}
};

class cPedInterface
{
public:
	int32_t MaxPed()
	{
		if (!this) return 0;
		return *(int32_t*)(this + 0x108);
	}

	cPedList* PedList()
	{
		if (!this) return 0;
		return (cPedList*)(*(uint64_t*)(this + 0x100));
	}

	int32_t CurrentPeds()
	{
		if (!this) return 0;
		return *(int32_t*)(this + 0x110);
	}
};

class cVehicleList
{
public:
	cVehicle* Vehicle(int index)
	{
		if (!this) return 0;
		return (cVehicle*)(*(uint64_t*)(this + (index * 0x10)));
	}

};

class cVehicleInterface
{
public:
	int32_t MaxVeh()
	{
		if (!this) return 0;
		return *(int32_t*)(this + 0x188);
	}

	int32_t CurrentVeh()
	{
		if (!this) return 0;
		return *(int32_t*)(this + 0x190);
	}

	cVehicleList* VehList()
	{
		if (!this) return 0;
		return (cVehicleList*)(*(uint64_t*)(this + 0x180));
	}
};

class cReplayInterface
{
public:
	cVehicleInterface* VehicleInterface()
	{
		if (!this) return 0;
		return (cVehicleInterface*)(*(uint64_t*)(this + 0x10));
	}
	cPedInterface* PedInterface()
	{
		if (!this) return 0;
		return (cPedInterface*)(*(uint64_t*)(this + 0x18));
	}
};

class cWorld {
public:
	cPed* LocalPed()
	{
		if (!this) return 0;
		return (cPed*)(*(uint64_t*)(this + 0x8));
	}
	uintptr_t pedUintPtr()
	{
		if (!this) return 0;
		return *(uintptr_t*)(this + 0x8);
	}
};