#include <iostream>
#include "memory.h"
#include <algorithm>


const DWORD processId = VARS::GetProcess(L"tf_win64.exe");
const DWORD baseAddress = VARS::GetModuleBaseAddress(VARS::processId, L"server.dll");
const DWORD localPlayer = VARS::memRead<uintptr_t>(VARS::baseAddress + 0xD7CF08);
const DWORD healt = 0x174;
const DWORD loopDistance = 0x20;
const DWORD X =  0x394;
const DWORD Y = 0x390;
const DWORD Z =  0x398;



#define M_PI 3.141592653
namespace Offsets{
	 uintptr_t localPlayer;
	 uintptr_t MyMouseY;
	 uintptr_t MyMouseX;
}

struct Entity {
	int health;
	int teamFlag;
	float X;
	float Y;
	float Z;


};

struct CompareTargetEnArray {
	bool operator() (const TargetList_t& lhs, const TargetList_t& rhs) const {
		return lhs.Distance < rhs.Distance;
	}
};

struct MyPlayer {
	int team;
	int Health;
	float Position[3];
	MyPlayer() {

	}

	void ReadInformation() {

	}
}MyPlayer;

struct PlayerList {
	int team;
	int Health;
	float Position[3];
	float aimbotAngle[3];
	PlayerList() {

	}
}PlayerList[4];
struct TargetList_t {
	float Distance;
	float aimbotAngle[3];

	TargetList_t() {

	}

	TargetList_t(float aimbotAngle[] , float myCoords[], float enemyCoords[]) {

		Distance = Get3dDistance(myCoords[0], myCoords[1], myCoords[2],
								enemyCoords[0], enemyCoords[1], enemyCoords[2]);
		
		this->aimbotAngle[0] = aimbotAngle[0];
		this->aimbotAngle[1] = aimbotAngle[1];
		this->aimbotAngle[2] = aimbotAngle[2];
	}

	float Get3dDistance(float X, float Y, float Z, float enemyX, float enemyY, float enemyZ) {
		float distance = std::sqrt(
			std::pow(enemyX - X, 2.0f) +
			std::pow(enemyY - Y, 2.0f) +
			std::pow(enemyZ - Z, 2.0f)
		);
		return distance;
	}
}TargetList;


void fullEntity(Entity en[],int flag) {
	int index = 0;
	for (int i = 0; i < 4; i++) {

		
		uintptr_t localPlayer = VARS::memRead<uintptr_t>((VARS::baseAddress + 0xD7CF08) + i * 0x20);
		
		int health = VARS::memRead<int>(localPlayer + 0x174);
		int teamFlag = VARS::memRead<int>(localPlayer + 0x304);
		float X = VARS::memRead<float>(localPlayer + 0x394);
		float Y = VARS::memRead<float>(localPlayer + 0x390);
		float Z = VARS::memRead<float>(localPlayer + 0x398);
		std::cout << "Health full: " << health << std::endl;
		std::cout << "enemy flag: " << teamFlag << std::endl;
		std::cout << "flag parametro: " << flag << std::endl;
		if(teamFlag == flag) {
			continue;
		}

		en[index].health = health;
		en[index].teamFlag = teamFlag;
		en[index].X = X;
		en[index].Y = Y;
		en[index].Z = Z;

		index = index + 1; // 
		std::cout << "index: " << index << std::endl;
	}
}

	

	// Função para calcular o ângulo necessário
	void CalculateAngle(float *src , float *dst, float *angle) {
		
		double delta[3] = { (src[0] - dst[0]) , (src[1] - dst[1]) , (src[2] - dst[2]) };
		double hyp = sqrt(delta[0] * delta[0] + delta[1] * delta[1]);
		angle[0] = (float)(asin(delta[1] / delta[0]) * 57.295779513082f);
		angle[1] = (float)(atanf(delta[1] / delta[0]) * 57.295779513082f);
		angle[2] = 0;

		if (delta[0] >= 0.0) {
			angle[1] += 180.0f;
		}

		
	}

	void aimbot() {

		TargetList_t *targetList = new TargetList_t[4];

		int targetLoop = 0;

		for (int i = 0; i < 4; i++) {
				
			CalculateAngle(MyPlayer.Position, PlayerList[i].Position, PlayerList[i].aimbotAngle);

			targetList[targetLoop] = TargetList_t(PlayerList[i].aimbotAngle, MyPlayer.Position, PlayerList[i].Position);
			targetLoop++;
		}

		if (targetLoop > 0) {
			std::sort(targetList, targetList + targetLoop, CompareTargetEnArray());
			if (!GetAsyncKeyState(0x2)) {
				VARS::memWrite<int>(0x7FFADAB20000 + 0x543364, targetList[0].aimbotAngle);
			}
		}
		targetLoop = 0;
		delete[] targetList;
	}

	

	





	int main()
	{


		VARS::processId = VARS::GetProcess(L"tf_win64.exe");
		VARS::baseAddress = VARS::GetModuleBaseAddress(VARS::processId, L"server.dll");
		Offsets::localPlayer = VARS::memRead<uintptr_t>(VARS::baseAddress + 0xD7CF08  );


		//uintptr_t mouse = VARS::GetModuleBaseAddress(VARS::processId, L"engine.dll");
		//Offsets::MyMouseY = VARS::memRead<uintptr_t>(mouse + 0x543364);
		// Abre o processo


		VARS::processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, VARS::processId);
		
		//const uintptr_t localPlayer = VARS::memRead<uintptr_t>(VARS::baseAddress + 0xD7CF08);

		int flag = VARS::memRead<int>(Offsets::localPlayer + 0x304);
		Entity entities[32];
		Entity enemy;
		float MyMOUSEy = VARS::memRead<float>(0x7FFADAB20000 + 0x543364);
		float MyMOUSEX = VARS::memRead<float>(0x7FFADAB20000 + 0x543364+4);

		//std::cout << "Endereço base do engine.dll: " << std::hex << mouse << std::endl;
	


		int health = VARS::memRead<int>(Offsets::localPlayer + 0x174 );
		std::cout << "healt " << health << std::endl;
		//std::cout << "Y mouse: " << MyMOUSEy << std::endl;
		//std::cout << "x mouse: " << MyMOUSEX << std::endl;
		std::cout << "flag: " << flag << std::endl;
		

		float X = VARS::memRead<float>(Offsets::localPlayer + 0x394);
		float Y = VARS::memRead<float>(Offsets::localPlayer + 0x390);
		float Z = VARS::memRead<float>(Offsets::localPlayer + 0x398);

		
		fullEntity(entities,flag);

		
		
		 
		
		float enemyX;
		float enemyY;
		float enemyZ;

		for (int i = 0; i < 32; i++) {
			int offset = i * 0x20;
			float enemyX = VARS::memRead<float>(Offsets::localPlayer + 0x394);
			float enemyY = VARS::memRead<float>(Offsets::localPlayer + 0x390);
			float enemyZ = VARS::memRead<float>(Offsets::localPlayer + 0x398);
		}

		
		for (int i = 0; i < 5; i++) {
			std::cout << "healt loop: " << entities[i].health << std::endl;
			}

		while (!GetAsyncKeyState(VK_F6)) {
			MyPlayer.readInformation;
	}
			
		

		

	}