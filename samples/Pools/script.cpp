/*
	THIS FILE IS A PART OF RDR 2 SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2019
*/

#include "script.h"
#include <string>
#include <vector>
#include "keyboard.h"

//Functions used to "monitor" walking, running, jogging...

/*void draw_Text(const char* text, float x, float y, int r, int g, int b, int a, bool centered = false, float sx = 0.342f, float sy = 0.342f)
{
	HUD::_SET_TEXT_COLOR(r, g, b, a);
	HUD::SET_TEXT_SCALE(sx, sy);
	HUD::SET_TEXT_CENTRE(centered);
	const char* literalString = MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", text);
	HUD::_DISPLAY_TEXT(literalString, x, y);
}

//int tog,still, wa, ru, spr;
//bool toggle;

void info()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	if (toggle == true)
		tog = 1;
	else
		tog = 0;

	if (CAM::_0xA24C1D341C6E0D53(0,0,0))//(TASK::IS_PED_STILL(playerPed))
		still = 1;
	else
		still = 0;

	if (TASK::IS_PED_WALKING(playerPed))
		wa = 1;
	else
		wa = 0;

	if (TASK::IS_PED_RUNNING(playerPed))
		ru = 1;
	else
		ru = 0;

	if (TASK::IS_PED_SPRINTING(playerPed))
		spr = 1;
	else
		spr = 0;

	char buffer[64];

	snprintf(buffer, 64, "First person: %d \n Walk: %d \n Jog: %d \n Sprint/Run: %d", still, wa, ru, spr);
	draw_Text(buffer, 0.5, 0.5, 255, 255, 255, 255, true, 0.66f, 0.66f);
}*/

bool isPlayerNearHorse()
{
	Vector3 myPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true, true);
	bool ret = false;
	Ped horse = PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(PLAYER::PLAYER_ID());
	Ped lastHorse = PED::_GET_LAST_MOUNT(PLAYER::PLAYER_PED_ID());

	Vector3 h = ENTITY::GET_ENTITY_COORDS(horse, true, true);
	float distance = MISC::GET_DISTANCE_BETWEEN_COORDS(myPos.x, myPos.y, myPos.z, h.x, h.y, h.z, true);
	if (distance <= 3.5)
		ret = true;

	h = ENTITY::GET_ENTITY_COORDS(lastHorse, true, true);
	distance = MISC::GET_DISTANCE_BETWEEN_COORDS(myPos.x, myPos.y, myPos.z, h.x, h.y, h.z, true);
	
	if (distance <= 3.5)
		ret = true;

	return ret;
}


bool leading = false;
void IsPlayerLeadingHorse()
{
	Ped horse = PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(PLAYER::PLAYER_ID());
	Ped lastHorse = PED::_GET_LAST_MOUNT(PLAYER::PLAYER_PED_ID());
	//char buffer[64];


	if ( /*(PLAYER::IS_PLAYER_TARGETTING_ENTITY(PLAYER::PLAYER_ID(), horse, 1) || PLAYER::IS_PLAYER_TARGETTING_ENTITY(PLAYER::PLAYER_ID(), lastHorse, 1))*/ PLAYER::_0x1A51BFE60708E482(PLAYER::PLAYER_ID()) && isPlayerNearHorse())
	{
		
		//snprintf(buffer, 64, "Targeting: Yes\nLeading: %d", leading);
		//draw_Text(buffer, 0.5, 0.5, 255, 255, 255, 255, true, 0.66f, 0.66f);

		if (PAD::IS_CONTROL_JUST_PRESSED(0, MISC::GET_HASH_KEY("INPUT_INTERACT_LEAD_ANIMAL")))
		{
			leading = true;
		}
	}

	if (leading)
	{
		if (PAD::IS_CONTROL_JUST_PRESSED(0, MISC::GET_HASH_KEY("INPUT_STOP_LEADING_ANIMAL")))
			leading = false;
	}

	if(PED::IS_PED_ON_MOUNT(PLAYER::PLAYER_PED_ID()))
		leading = false;

	
}




bool IsPedInTask()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	if (PED::IS_PED_RELOADING(playerPed))
		return true;

	if (PED::IS_PED_IN_COVER(playerPed, 0, 0))
		return true;

	if (TASK::IS_PED_RUNNING(playerPed))
		return true;

	if (TASK::IS_PED_SPRINTING(playerPed))
		return true;

	if (PED::IS_PED_PLANTING_BOMB(playerPed))
		return true;

	if (PED::_IS_PED_OPENING_A_DOOR(playerPed))
		return true;

	if (PED::IS_PED_GOING_INTO_COVER(playerPed))
		return true;

	if (PED::IS_PED_RUNNING_MOBILE_PHONE_TASK(playerPed))
		return true;

	if (PED::IS_PED_USING_ANY_SCENARIO(playerPed))
		return true;

	if (TASK::_IS_PED_DUELLING(playerPed))
		return true;

	if (CAM::IS_AIM_CAM_ACTIVE() || CAM::IS_FIRST_PERSON_AIM_CAM_ACTIVE())
		return true;

	if (PLAYER::IS_PLAYER_FREE_AIMING(PLAYER::PLAYER_ID()))
		return true;


	IsPlayerLeadingHorse();
	if (leading)
		return true;

	//if (isPlayerNearHorse())
	//	return true;

	return false;
}


// Returns true if the player is going Up, Down, Left or Right (WASD)
bool IsPlayerControlled()
{
	if (PAD::IS_DISABLED_CONTROL_PRESSED(0, MISC::GET_HASH_KEY("INPUT_MOVE_UP_ONLY")) || PAD::IS_DISABLED_CONTROL_PRESSED(0, MISC::GET_HASH_KEY("INPUT_MOVE_DOWN_ONLY")) || PAD::IS_DISABLED_CONTROL_PRESSED(0, MISC::GET_HASH_KEY("INPUT_MOVE_LEFT_ONLY")) || PAD::IS_DISABLED_CONTROL_PRESSED(0, MISC::GET_HASH_KEY("INPUT_MOVE_RIGHT_ONLY")))
		return true;
	else
		return false;

}



bool toggle;
void update()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	if (CAM::_0xA24C1D341C6E0D53(0, 0, 0))
	{
		if (IsPedInTask() == false && PED::IS_PED_ON_FOOT(playerPed))
		{
			if (TASK::IS_PED_STILL(playerPed) && !IsPlayerControlled())
				toggle = false;

			if (IsPlayerControlled() && toggle == false)
			{
				TASK::TASK_STAND_STILL(playerPed, 100);
				toggle = true;
			}
		}
	}
}



void main()
{		
	while (true)
	{
		update();
		WAIT(0);
	}
}

void ScriptMain()
{	
	srand(GetTickCount());
	main();
}
