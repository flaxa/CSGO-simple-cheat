#include "Hack.h"

#include <iostream>



Hack::Hack()
{
	mem = Memory();
	DWORD proc = mem.getProcess();
	dwClientBase = mem.getModule(proc, L"client.dll");
	dwEngineBase = mem.getModule(proc, L"engine.dll");
	localPlayer = mem.read<DWORD>(dwClientBase + dwLocalPlayer);

}

void Hack::updateLocalPlayer()
{
	localPlayer = mem.read<DWORD>(dwClientBase + dwLocalPlayer);
}

int Hack::getCrosshairID()
{
	
	return mem.read<int>(localPlayer + m_iCrosshairId);
}


int Hack::getMyTeam()
{
	return mem.read<int>(localPlayer + m_iTeamNum);
}
void Hack::triggerBot()
{
	POINT pos_cursor;
	HWND  hwnd_outra_win;
	while (true)
	{
		updateLocalPlayer();
		while (localPlayer == 0)
		{
			updateLocalPlayer();
			Sleep(1000); 
		}
			
			
			if (GetAsyncKeyState(0x56) != 0)
			{
				if (getCrosshairID() != 0)
				{
					GetCursorPos(&pos_cursor);
					hwnd_outra_win = WindowFromPoint(pos_cursor);
					SendMessage(hwnd_outra_win, WM_LBUTTONDOWN, MK_LBUTTON,
						MAKELPARAM(pos_cursor.x, pos_cursor.y));
					SendMessage(hwnd_outra_win, WM_LBUTTONUP, 0, MAKELPARAM(pos_cursor.x,
						pos_cursor.y));
					Sleep(300);
				}
			}
			Sleep(1);
		
		
	}
}

void Hack::esp()
{
	while (true)
	{
		while (localPlayer != 0)
		{

			int myTeam = getMyTeam();
			for (int i = 0; i <= getGreatestEntityIndex(); i++)
			{
				showEntity(i, myTeam);
			}
			Sleep(1);
		}
		Sleep(500);
		
		
			
	}
	
}





DWORD Hack::getEntity(int i)
{
	return mem.read<DWORD>(dwClientBase + dwEntityList + (i * 0x10));
}


int Hack::getGreatestEntityIndex()
{
	DWORD entity;
	int biggest = 0;
	for (int i = 1; i < 64; i++) {
		entity = getEntity(i);
		if (entity == NULL)
		{


			
		}
		else
		{
			biggest = i;
		}
	}
	return biggest;
	
}

DWORD Hack::getGlowObjectManager()
{
	return mem.read<DWORD>(dwClientBase + dwGlowObjectManager);
}

int Hack::getEntityTeam(int i)
{
	return mem.read<int>((getEntity(i) + m_iTeamNum));
}

int Hack::getGlowIndex(int i)
{
	return mem.read<int>(getEntity(i) + m_iGlowIndex);
}

bool Hack::getIsDefusing(int i)
{
	return mem.read<int>(getEntity(i) + m_bIsDefusing);;
}

void Hack::showEntity(int i, int myTeam)
{
	if (myTeam == getEntityTeam(i))
	{
		showTeam(i);

	}
	else
	{
		showEnemy(i);
	}
}

int Hack::getEntityHealth(int i)
{
	return mem.read<int>(getEntity(i) + m_iHealth);
}

void Hack::showTeam(int i)
{
	glowMemory teamMate = mem.read<glowMemory>(getGlowObjectManager() + (getGlowIndex(i) * 0x38));
	teamMate.blue = 1;
	teamMate.alpha = 1.7;
	teamMate.renderWhenOccluded = true;
	teamMate.renderWhenUnoccluded = false;
	mem.write<glowMemory>(getGlowObjectManager() + (getGlowIndex(i) * 0x38), teamMate);

}

void Hack::showEnemy(int i)
{
	glowMemory enemy = mem.read<glowMemory>(getGlowObjectManager() + (getGlowIndex(i) * 0x38));
	if (getIsDefusing(i))
	{
		enemy.blue = 1;
		enemy.red = 1;
		enemy.green = 1;
		enemy.alpha = 1.7;
		enemy.renderWhenOccluded = true;
	}
	else
	{
		int enemyHealth = getEntityHealth(i);

		enemy.green = 0.01 * enemyHealth;
		enemy.red = 1 - enemy.green;
		enemy.alpha = 1.7;
		enemy.renderWhenOccluded = true;

	}
	enemy.renderWhenUnoccluded = false;
	mem.write<glowMemory>(getGlowObjectManager() + (getGlowIndex(i) * 0x38), enemy);


}

Hack::Vector3 Hack::getMyPosition()
{
	return mem.read<Vector3>(localPlayer + m_vecOrigin);
	//return Vector3();
	
}

DWORD Hack::getBoneMatrix(int i)
{
	return mem.read<DWORD>(getEntity(i) + m_dwBoneMatrix);
}

Hack::Vector3 Hack::getBonePosition(int i, int boneID)
{
	//8 is head
	Vector3 bonePos;
	DWORD boneMatrix = getBoneMatrix(i);
	bonePos.x = mem.read<float>(boneMatrix + 0x30 * boneID + 0x0C);
	bonePos.y = mem.read<float>(boneMatrix + 0x30 * boneID + 0x1C);
	bonePos.z = mem.read<float>(boneMatrix + 0x30 * boneID + 0x2C);
	return  bonePos;
}




Hack::Vector3 Hack::getViewDiffToEnemyHead(int i)
{
	Vector3 theirHeadPos = getBonePosition(i,8);


	return theirHeadPos.sub(getMyHeadPosition());
}



Hack::Vector3 Hack::getMyHeadPosition()
{
	Vector3 headOffset = mem.read<Vector3>(localPlayer + m_vecViewOffset);
	return headOffset.add(getMyPosition());
}


Hack::Vector2 Hack::getDiffInViewAngles(int i)
{
	Vector3 myDeltaEnemy = getViewDiffToEnemyHead(i);
	float myDeltaEnemyLength = myDeltaEnemy.length();
	float pitch = -asin(myDeltaEnemy.z / myDeltaEnemyLength) * (180 / PI);
	float yaw = atan2(myDeltaEnemy.y, myDeltaEnemy.x) * (180 / PI);
	Vector2 anglesToEnemy = Vector2(pitch, yaw);
	return getCurrentViewAngles().sub(anglesToEnemy);
}

Hack::Vector2 Hack::getCurrentViewAngles()
{
	DWORD clientState = mem.read<DWORD>(dwEngineBase + dwClientState);
	return mem.read<Vector2>(clientState + dwClientState_ViewAngles);
}

void Hack::aimAtEntitySmoothing()
{
	float FOV=0;
	int smoothing=1;
	while (true)
	{
		while (localPlayer != 0)
		{

			int weapon = getMyWeaponID();
			if (weapon == 1)
			{
				FOV = 3;
				smoothing = 3;
			}
			else if (weapon == 2)
			{
				FOV = 3;
				smoothing = 7;
			}
			else if (weapon == 3)
			{
				FOV = 3;
				smoothing = 5;
			}
			else if (weapon == 4)
			{
				FOV = 2;
				smoothing = 1;
			}
			else
			{
				FOV = 3;
				smoothing = 3;
			}
			int entity = getEntityImAimingNearFOV(FOV);
			Vector2 diffToEnemy;
			Vector2 newViewAngles;
			if (entity != -1)
			{
				while (GetAsyncKeyState(MK_LBUTTON) != 0 && getEntityHealth(entity) > 0)
				{
					diffToEnemy = getDiffInViewAngles(entity);
					if (getShotsFired() >= 2)
					{
						diffToEnemy = diffToEnemy.add(getPunchAngle());


						//newViewAngles = newViewAngles.sub(getPunchAngle());

					}
					diffToEnemy = diffToEnemy.div(smoothing);
					newViewAngles = getCurrentViewAngles().sub(diffToEnemy.normaleAngles());
					aimAt(newViewAngles);
					Sleep(1);

				}
				
			}
			Sleep(1);
		}
		Sleep(500);
		
	}
}

Hack::Vector2 Hack::getPunchAngle()
{
	Vector2 punch = mem.read<Vector2>(localPlayer + m_aimPunchAngle);
	punch.x = punch.x * 2.0f;
	punch.y = punch.y * 2.0f;
	return punch ;
}

int Hack::getShotsFired()
{
	return mem.read<int>(localPlayer + m_iShotsFired);
}

int Hack::getMyWeaponID()
{
	DWORD myWeapon = mem.read<DWORD>(localPlayer + m_hActiveWeapon);
	DWORD weaponEntity = mem.read<DWORD>(dwClientBase + dwEntityList + ((myWeapon & 0xFFF) - 1) * 0x10);
	int weaponID = mem.read<INT16>(weaponEntity + m_iItemDefinitionIndex);
	return getWeapon(  weaponID);
}



void Hack::aimAt(Hack::Vector2 aimAt)
{
	DWORD clientState = mem.read<DWORD>(dwEngineBase + dwClientState);
	mem.write<Vector2>(clientState + dwClientState_ViewAngles, aimAt.normaleAngles());
}



int Hack::getEntityImAimingNearFOV(float FOV)
{
	int myTeam = getMyTeam();
	int bestIndex = -1;
	float currentBest = 10000;
	float temp;
	for (int i = 0; i <= getGreatestEntityIndex(); i++)
	{
		if (myTeam != getEntityTeam(i) && getEntityHealth(i) > 0)
		{
			temp = abs(getDiffInViewAngles(i).length());
			if (temp < currentBest && temp <= FOV)
			{
				currentBest = temp;
				bestIndex = i;
			}
		}
	}
	return bestIndex;
}

int Hack::getWeapon(int iWeaponID)
{

	if (iWeaponID == 1 || iWeaponID == 2 || iWeaponID == 3 || iWeaponID == 4
		|| iWeaponID == 36 || iWeaponID == 61 || iWeaponID == 30 || iWeaponID == 32 || iWeaponID == 64 || iWeaponID == 63) {
		return 1;//pistol
	}
	else if (iWeaponID == 7 || iWeaponID == 8 || iWeaponID == 10 || iWeaponID == 13 || iWeaponID == 16 || iWeaponID == 39 || iWeaponID == 60)
	{

		return 2;//rifle
	}
	else if (iWeaponID == 17 || iWeaponID == 19 || iWeaponID == 23 || iWeaponID == 24 || iWeaponID == 26 || iWeaponID == 33 || iWeaponID == 34)
	{
		return 3;//sub
	}
	else if (iWeaponID == 11 || iWeaponID == 9 || iWeaponID == 38 || iWeaponID == 40)
	{
		return 4;//sniper
	}
	//make aimbot not work for frags and equipment and knife




}







