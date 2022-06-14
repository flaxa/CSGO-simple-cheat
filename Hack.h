#pragma once
#include "Memory.h"
class Hack
{
public:

	//structures

	struct Vector2 {
		float x, y;
		Vector2()
		{
			this->x = 0;
			this->y = 0;
		}

		Vector2(float x, float y)
		{
			this->x = x;
			this->y = y;
		}

		float length() {
			return sqrt((double)this->x * (double)this->x + (double)this->y * (double)this->y );

		}

		Vector2 add(Vector2 y) {
			return Vector2(this->x + y.x, this->y + y.y);

		}

		Vector2 sub(Vector2 y) {
			return Vector2(this->x - y.x, this->y - y.y);

		}

		Vector2 div(int divBy)
		{
			return Vector2(this->x / (float)divBy, this->y / (float)divBy);
		}
		Vector2 normaleAngles()
		{
			while (this->y > 180.0f)
				this -> y -= 360.0f;

			while (this -> y < -180.0f)
				this ->y += 360.0f;

			if (this->x > 89.0f)
				this ->x = 89.0f;

			if (this->x < -89.0f)
				this->x = -89.0f;

			return Vector2(this->x, this->y );
			
		}


	};
	struct Vector3 {

		float x, y, z;

		Vector3()
		{
			this->x = 0;
			this->y = 0;
			this->z = 0;
		}

		Vector3(float x,float y,float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		Vector3 add( Vector3 y) {
			return Vector3(this->x+y.x,this->y+y.y, this->z+y.z);

		}

		Vector3 sub(Vector3 y) {
			return Vector3(this->x - y.x, this->y - y.y, this->z - y.z);

		}

		float length( ) {
			return sqrt((double)this->x * (double)this->x + (double)this->y * (double)this->y + (double)this->z * (double)this->z);

		}
	};

	struct glowMemory {
		BYTE ignoreBase[4];
		float red;
		float green;
		float blue;
		float alpha;
		BYTE buffer[16];
		bool renderWhenOccluded;
		bool renderWhenUnoccluded;
		bool fullBloom;
		BYTE buffer2[5];
		int glowType;
	};
	
	const float PI = 3.14159265358979;
	//offsets needed
	DWORD dwLocalPlayer = 0xD892CC;
	DWORD m_iCrosshairId = 0xB3E8;
	DWORD m_aimPunchAngle = 0x302C;


	DWORD m_iShotsFired = 0xA390;
	DWORD dwClientState = 0x588FEC;
	DWORD dwClientState_ViewAngles = 0x4D90;
	DWORD m_iTeamNum = 0xF4;
	DWORD dwEntityList = 0x4DA215C;
	DWORD m_bSpotted = 0x93D;

	DWORD dwGlowObjectManager = 0x52EA5D0;
	DWORD m_iGlowIndex = 0xA438;
	DWORD m_iHealth = 0x100;
	DWORD m_bIsDefusing = 0x3930;

	DWORD m_vecOrigin = 0x138;
	DWORD m_vecViewOffset = 0x108;
	DWORD m_dwBoneMatrix = 0x26A8;
	DWORD m_hActiveWeapon = 0x2EF8;
	DWORD m_iItemDefinitionIndex = 0x2FAA;


	//end
	Memory mem;
	DWORD dwClientBase;
	DWORD dwEngineBase;
	DWORD localPlayer;
	

	Hack();
	//intermediate methods
	void updateLocalPlayer();

	int getCrosshairID();
	int getMyTeam();
	DWORD getEntity(int i);
	int getGreatestEntityIndex();
	DWORD getGlowObjectManager();
	int getEntityTeam(int i);
	int getGlowIndex(int i);
	bool getIsDefusing(int i);
	void showEntity(int i, int myTeam);
	int getEntityHealth(int i);
	void showTeam(int i);
	void showEnemy(int i);
	Vector3 getMyPosition();
	Vector3 getMyHeadPosition();
	DWORD getBoneMatrix(int i);
	Vector3 getBonePosition(int i, int boneID);
	Vector3 getViewDiffToEnemyHead(int i);
	Vector2 getDiffInViewAngles(int i);
	Vector2 getCurrentViewAngles();
	int getEntityImAimingNearFOV(float FOV);
	void aimAtEntitySmoothing();
	void aimAt(Vector2 aimAt);
	Vector2 getPunchAngle();
	int getShotsFired();
	int getMyWeaponID();
	int getWeapon(int iWeaponID);
	
	

	//cheat methods
	void triggerBot();
	void esp();


};

