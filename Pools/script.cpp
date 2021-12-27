
#include "script.h"
#include "keyboard.h"
#include "..\..\inc\HelperMethods.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <ctime>
#include <map>
#include <fstream>


namespace std
{
#include <cstdlib>
};









void SAC_set_female_body_proportions(Ped ped)
{
	float proportions_multiplyier = 0.2f - (rand() % 4 + 1)/10;

//	ped = PLAYER::PLAYER_ID();

	PED::_SET_PED_FACE_FEATURE(ped, 8420, -1.0f + proportions_multiplyier);   //forearm
	PED::_SET_PED_FACE_FEATURE(ped, 46032, -0.6f + proportions_multiplyier);   //upper arm

	PED::_SET_PED_FACE_FEATURE(ped, 41478, -0.4f + proportions_multiplyier);   //chest width
	PED::_SET_PED_FACE_FEATURE(ped, 27779, -0.5f + proportions_multiplyier);   //chest depth

	PED::_SET_PED_FACE_FEATURE(ped, 50460, -0.6f + proportions_multiplyier);   //belly width
	PED::_SET_PED_FACE_FEATURE(ped, 49787, -0.5f + proportions_multiplyier);   //belly depth

	PED::_SET_PED_FACE_FEATURE(ped, 64834, -0.8f + proportions_multiplyier);   //upper legs
	PED::_SET_PED_FACE_FEATURE(ped, 42067, -1.0f + proportions_multiplyier);   //lower legs

	PED::_SET_PED_FACE_FEATURE(ped, 15833, -0.7f + proportions_multiplyier);  //shoulders height
	PED::_SET_PED_FACE_FEATURE(ped, 50039, -0.8f + proportions_multiplyier);   //shoulders width
	PED::_SET_PED_FACE_FEATURE(ped, 7010, -0.2f + proportions_multiplyier);   //shoulders depth

	PED::_SET_PED_FACE_FEATURE(ped, 36277, 0.2f + proportions_multiplyier);   //neck width
	PED::_SET_PED_FACE_FEATURE(ped, 60890, 0.0f + proportions_multiplyier);   //neck depth ?

	PED::_SET_PED_FACE_FEATURE(ped, 33485, -0.4f + proportions_multiplyier);   //traps
	PED::_SET_PED_FACE_FEATURE(ped, 18046, -0.4f + proportions_multiplyier);   //lats

	PED::_SET_PED_SCALE(ped, 1 - (rand() % 10 + 1)/10);

	PED::_UPDATE_PED_VARIATION(ped, true, true, true, true, true);

	DECORATOR::DECOR_SET_INT(ped, "SAC_morphed_female", 1);

}


void sac_ragdoll_ped(Ped ped, bool do_fall)
{
	if (ped != PED::_GET_FIRST_ENTITY_PED_IS_CARRYING(PLAYER::PLAYER_ID()) && ped != PED::_GET_FIRST_ENTITY_PED_IS_CARRYING(PLAYER::PLAYER_PED_ID())) // ragdolls carried peds?
	
	{
			int npchealth = ENTITY::GET_ENTITY_HEALTH(ped);
			int npcmaxhealth = ENTITY::GET_ENTITY_MAX_HEALTH(ped, false);
			int randommax = (npcmaxhealth * 1.5 - npchealth) * 10;


			for (int i_flag = 0; i_flag < 20; i_flag++) PED::CLEAR_RAGDOLL_BLOCKING_FLAGS(ped, i_flag);
	// add parameter		TASK::TASK_STAY_IN_COVER(ped);
	//		PED::SET_PED_TO_RAGDOLL(ped, 3000, 3000, 3, false, false, false); // paralysis

			int health_rand = std::rand() % (randommax);

	//		v1 = rand() % 100;         // v1 in the range 0 to 99
	//		v2 = rand() % 100 + 1;     // v2 in the range 1 to 100
	//		v3 = rand() % 30 + 1985;   // v3 in the range 1985-2014 

			if (do_fall) PED::SET_PED_RAGDOLL_FORCE_FALL(ped);  // force fall
	
			if (npchealth > 100) PED::SET_PED_TO_RAGDOLL(ped, (30000), (30000), 0, true, true, false); // 0
			if (npchealth > 50 && npchealth <= 100) PED::SET_PED_TO_RAGDOLL(ped, (30000), (30000), 3, true, true, false); // 3
			if (npchealth <= 50) PED::SET_PED_TO_RAGDOLL(ped, (30000), (30000), 1, true, true, false); // 1
	//		if (npchealth <= 40) PED::SET_PED_TO_RAGDOLL(ped, (5000 + health_rand), (5000 + health_rand), 2, false, false, false); // 2

	//		std::rand() % (2999 - 0 + 1)

	//		PED::SET_PED_TO_RAGDOLL(ped, 3000, 3000, 1, false, false, false); // very paralyzed
	//		PED::SET_PED_TO_RAGDOLL(ped, 3000, 3000, 2, false, false, false); 
	
			


	//	}

			std::string ragdoll_string = "RAGDOLL";
			char const* ragdoll_constchar = ragdoll_string.c_str();  //use char const* as target type
			HUD::_DISPLAY_TEXT(ragdoll_constchar, 0, 0);

			WAIT(100);
//	}
	}

	

}






void SAC_mess_up_IK(Ped ped)
{

	// SET_IK_TARGET(ped, int ikIndex, Entity entityLookAt, int boneLookAt, float offsetX, float offsetY, float offsetZ, Any p7, int blendInDuration, int blendOutDuration);

	//PED::SET_IK_TARGET(ped, 39, ped, 65245, 0.0, 0.0, -10.0, 0, -1, -1); // ik l foot
	//PED::SET_IK_TARGET(ped, 11, ped, 65245, 0.0, 0.0, -10.0, 0, -1, -1); // ik r foot

	PED::SET_IK_TARGET(ped, 1, PLAYER::PLAYER_PED_ID(), 21030, 0.0, 0.0, -10.0, 0, -1, 100000); // skel_head
//	PED::SET_IK_TARGET(ped, 1, PLAYER::PLAYER_PED_ID(), 21030, 0.0, 0.0, -10.0, 0, -1, 100000); // skel_head

	//PED::SET_IK_TARGET(l_F0, 1, PLAYER::PLAYER_PED_ID(), 31086, 0.0, 0.0, 0.0, 0, -1, -1);
	//PED::SET_IK_TARGET(PLAYER::PLAYER_PED_ID(), 1, l_F0, 31086, 0.0, 0.0, 0.0, 0, -1, -1);
}




void ScriptMain()
{

// ------------------------------------- Variable declaration -------------------------------------------------//


	DECORATOR::DECOR_REGISTER("SAC_ragdoll", 3);
	DECORATOR::DECOR_REGISTER("SAC_morphed_female", 3);


	std::map<Ped, bool> pedmap_ishanging;
	std::map<Ped, bool> pedmap_ismorphed;
	std::map<Ped, int> pedmap_hangingtick;


	

		int msgtime{ 0 };

//		Player player = PLAYER::PLAYER_PED_ID();

		Player player = PLAYER::PLAYER_ID();








// ------------------------------------- End variable declaration ---------------------------------------------//




	while (true)
	{
		// do stuff here.

		srand(GetTickCount());


		AUDIO::STOP_PED_SPEAKING(PLAYER::PLAYER_ID(), true);
		AUDIO::STOP_PED_SPEAKING(PLAYER::PLAYER_PED_ID(), true);

		AUDIO::SET_AMBIENT_VOICE_NAME(PLAYER::PLAYER_ID(), "shut the fuck up");
	//	AUDIO::SET_AMBIENT_VOICE_NAME(PLAYER::PLAYER_ID(), "shut the fuck up");

	//	AUDIO::DISABLE_PED_PAIN_AUDIO(peds[i], true);


		const int ARR_SIZE = 150;
		Ped peds[ARR_SIZE];
		int count = worldGetAllPeds(peds, ARR_SIZE);

		for (int i = 0; i < count; i++)
		{

			// *********************************** Blanket population management *********************************************** //

		//	if (sac_is_ped_hogtied(peds[i]))
		//	{
		//		TASK::TASK_STAND_STILL(peds[i], -1);
		//	}

			PED::SET_PED_CAN_ARM_IK(peds[i], true);
			PED::SET_PED_CAN_HEAD_IK(peds[i], true);
			PED::SET_PED_CAN_LEG_IK(peds[i], true);
			PED::SET_PED_CAN_TORSO_IK(peds[i], true);


			if (DECORATOR::DECOR_EXIST_ON(peds[i], "SAC_ragdoll") && !ENTITY::IS_ENTITY_DEAD(peds[i]) && !sac_is_ped_hogtied(peds[i]))
			{
				sac_ragdoll_ped(peds[i], true);
			}


			// sandboxing

			if (!PED::IS_PED_RAGDOLL(peds[i]) && PED::IS_PED_HUMAN(peds[i]) && !PED::IS_PED_MALE(peds[i]) && peds[i] != PLAYER::PLAYER_ID() && !pedmap_ishanging[peds[i]])
			{
				if (std::rand() % (1999 - 0 + 1) < 1) PedPlayRandomScenario(peds[i]);
				else if (std::rand() % (1999 - 0 + 1) < 1) TASK::TASK_WANDER_STANDARD(peds[i], 10.0f, 1);
				else if (std::rand() % (1999 - 0 + 1) < 1)
				{
					Vector3 vpedloc = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(peds[i], 0.0f, 0.0f, 0.0f);
					TASK::_TASK_USE_NEAREST_SCENARIO_TO_COORD(peds[i], vpedloc.x, vpedloc.y, vpedloc.z, 100, -1, true, true, true, true);
				}

			}

			// end sandboxing


			if (DECORATOR::DECOR_EXIST_ON(peds[i], "SAC_morphed_female") == false)
			{
				if (PED::IS_PED_HUMAN(peds[i]) && !PED::IS_PED_MALE(peds[i]) && !pedmap_ismorphed[peds[i]])
				{
					pedmap_ismorphed[peds[i]] = true;
					SAC_set_female_body_proportions(peds[i]);
				}
			}


			// *********************************** End blanket population management ***************************************** //

			// *********************************** Hanging management *********************************************** //


								//checking if NPC is being hanged with TieYourLasso mod

		//	pedmapishanging[peds[i]] = false;
			
			if (pedmap_ishanging[peds[i]] && !DECORATOR::DECOR_EXIST_ON(peds[i], "TYL_hanged"))
			{
				pedmap_ishanging[peds[i]] = false;
			}
			else if (!pedmap_ishanging[peds[i]] && DECORATOR::DECOR_EXIST_ON(peds[i], "TYL_hanged") && !ENTITY::IS_ENTITY_DEAD(peds[i]))
			{
				Vector3 vecfoot = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(peds[i], PED::GET_PED_BONE_INDEX(peds[i], 45454)); 
				float groundzcoordped;
				MISC::GET_GROUND_Z_FOR_3D_COORD(vecfoot.x, vecfoot.y, vecfoot.z, &groundzcoordped, true);
				if (vecfoot.z > groundzcoordped + 0.1 && peds[i] != PED::_GET_FIRST_ENTITY_PED_IS_CARRYING(player)) //is not carried by player
				{
					pedmap_ishanging[peds[i]] = true;
					
			//		int initial_hanging_ragdoll = std::rand() % 3 + 1;

			//		if (initial_hanging_ragdoll == 1) PED::SET_PED_TO_RAGDOLL(ped, (30000), (30000), 0, true, true, false);
			//		if (initial_hanging_ragdoll == 2) PED::SET_PED_TO_RAGDOLL(ped, (30000), (30000), 1, true, true, false);
			//		if (initial_hanging_ragdoll == 3) PED::SET_PED_TO_RAGDOLL(ped, (30000), (30000), 3, true, true, false);




			//		pedmap_hangingtick[peds[i]] = GetTickCount();
				}
			}


			if (pedmap_ishanging[peds[i]] && !ENTITY::IS_ENTITY_DEAD(peds[i]))
			{


		// test		PED::SET_PED_CAN_LEG_IK(peds[i], false);

		//		TASK::TASK_STAY_IN_COVER(peds[i]);
			
				if (std::rand() % (100) < 1) // && GetTickCount() > pedmap_hangingtick[peds[i]] + 5000)
				{
					sac_ragdoll_ped(peds[i], true);
				//	pedmap_hangingtick[peds[i]] = GetTickCount();
				}
				else PED::RESET_PED_RAGDOLL_TIMER(peds[i]);
								
				std::string healthstring = std::to_string(ENTITY::GET_ENTITY_HEALTH(peds[i]));
				char const* healthconstchar = healthstring.c_str();  //use char const* as target type
				
			//	UIDEBUG::_BG_SET_TEXT_SCALE(0.5, 0.5);
			//	UIDEBUG::_BG_DISPLAY_TEXT(healthconstchar, 0, 0);

				HUD::_DISPLAY_TEXT(healthconstchar, 0, 0);



				PedPanic(peds[i]);
				

				if (sac_is_ped_hogtied(peds[i]) == false)
				{


					if (std::rand() % (1) < 1) PED::SET_ENABLE_HANDCUFFS(peds[i], true, false); //  || DECORATOR::DECOR_EXIST_ON(peds[i], "TYL_hanged_by_hands")) 
						else TASK::UNCUFF_PED(peds[i]);
					if (std::rand() % (20) < 1 || DECORATOR::DECOR_EXIST_ON(peds[i], "TYL_hanged_by_feet")) PED::SET_ENABLE_BOUND_ANKLES(peds[i], true);
						else PED::SET_ENABLE_BOUND_ANKLES(peds[i], false);

				}
					
				//	APPLY_FORCE_TO_ENTITY(peds[i], int forceFlags, float x, float y, float z, float offX, float offY, float offZ, int boneIndex, BOOL isDirectionRel, BOOL ignoreUpVec, BOOL isForceRel, BOOL p12, BOOL p13)

				//	NATIVE_INLINE static void APPLY_FORCE_TO_ENTITY(Entity entity, int forceFlags, float x, float y, float z, float offX, float offY, float offZ, int boneIndex, BOOL isDirectionRel, BOOL ignoreUpVec, BOOL isForceRel, BOOL p12, BOOL p13)

				//	ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, 0, 0, -1, false, false, true, true);

			//		WAIT(100);
			//test		sac_ragdoll_ped(peds[i], false);
			}
			
				

			// *********************************** End hanging management *********************************************** //
			









		}    // end scanning peds[i]


		if (std::rand() % (49999 - 0 + 1) < 2) // automatically spawn random female
		{
		Ped autospawnfemale = sac_spawn_female();
		WAIT(1000);
		}





		if (IsKeyJustUp(VK_F3)) // F3 to spawn a female // works only with PLAYER::PLAYER_PED_ID()
		{

		//	Vector3 plCoords2 = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true, false);

			Ped f3femalespawn = sac_spawn_female();
			WAIT(1000);

		//	HUD::_DISPLAY_TEXT("Spawning NPC", 0, 0);
		}

		

		Entity playerFreeAimingTarget, playerTarget;

		PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &playerFreeAimingTarget);	// works only with PLAYER_ID()
		PLAYER::GET_PLAYER_TARGET_ENTITY(PLAYER::PLAYER_ID(), &playerTarget);						// works only with PLAYER_ID()

		if (PED::IS_PED_HUMAN(playerFreeAimingTarget)) // && activation_state) // && peds[i] != player)
		{

			if (IsKeyJustUp(VK_F5))		TASK::TASK_WANDER_STANDARD(playerFreeAimingTarget, 10.0f, 1);

			if (IsKeyJustUp(VK_F7))		SAC_set_female_body_proportions(playerFreeAimingTarget);


		//	std::string ismorphed_string = std::to_string(pedmap_ismorphed[playerFreeAimingTarget]);
		//	char const* ismorphed_char = ismorphed_string.c_str();  //use char const* as target type

		//	UIDEBUG::_BG_SET_TEXT_SCALE(0.5, 0.5);
		//	UIDEBUG::_BG_DISPLAY_TEXT(healthconstchar, 0, 0);

		//	HUD::_DISPLAY_TEXT(ismorphed_char, 0, 0);




			//	PED::SET_PED_RAGDOLL_FORCE_FALL(playerFreeAimingTarget);


			//	TASK::TASK_KNOCKED_OUT(playerFreeAimingTarget, 0, false);
			//	TASK::TASK_KNOCKED_OUT_AND_HOGTIED(playerFreeAimingTarget, 0, false);
			//	PED::SET_ENABLE_BOUND_ANKLES(playerFreeAimingTarget, true); // works
			
				//TASK::TASK_START_SCENARIO_IN_PLACE_HASH(playerFreeAimingTarget, MISC::GET_HASH_KEY("WORLD_HUMAN_BROOM_WORKING"), 20000, true, true, 1.0, false);
				// true, false, *false = 
				// false, true, *false = walk away with broom
				// false, false, *false = ?
				// true, true, *false = ?

			//	Vector3 vpedloc = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerFreeAimingTarget, 0.0f, 0.0f, 0.0f);
			//	TASK::TASK_USE_NEAREST_SCENARIO_CHAIN_TO_COORD(playerFreeAimingTarget, vpedloc.x, vpedloc.y, vpedloc.z, 100, true, true, true, true);

			//	TASK_USE_NEAREST_SCENARIO_CHAIN_TO_COORD(Ped ped, float x, float y, float z, float distance, BOOL p5, BOOL p6, BOOL p7, BOOL p8)

			//	TASK::TASK_KNOCKED_OUT_AND_HOGTIED(playerFreeAimingTarget, 0, true);



		}


		if (PED::IS_PED_HUMAN(playerTarget)) // && activation_state) // && peds[i] != player)
		{
			if (IsKeyJustUp(VK_F5))		TASK::TASK_WANDER_STANDARD(playerFreeAimingTarget, 10.0f, 1);

			if (IsKeyJustUp(VK_F7))	SAC_set_female_body_proportions(playerTarget);


		//	std::string ismorphed_string = std::to_string(pedmap_ismorphed[playerTarget]);
		//	char const* ismorphed_char = ismorphed_string.c_str();  //use char const* as target type

		//	UIDEBUG::_BG_SET_TEXT_SCALE(0.5, 0.5);
		//	UIDEBUG::_BG_DISPLAY_TEXT(healthconstchar, 0, 0);

		//	HUD::_DISPLAY_TEXT(ismorphed_char, 0, 0);


			//	TASK::TASK_KNOCKED_OUT(playerTarget, 0, false);
			//	TASK::TASK_KNOCKED_OUT_AND_HOGTIED(playerTarget, 0, false);
			//	PED::SET_ENABLE_BOUND_ANKLES(playerTarget, true); //works

				//TASK::TASK_START_SCENARIO_IN_PLACE_HASH(playerTarget, MISC::GET_HASH_KEY("WORLD_HUMAN_BROOM_WORKING"), 20000, true, true, 1.0, false);
				// true, false, *false = drop broom / walk away with broom if not interrupted / broom appears weirdly
				// false, true, *false = walk away with broom 
				// false, false, *false = broom appears weirdly, drop when scenario interrupted but I think closes gracefully when not interrupted
				// true, true (correct prop intro), *false = correct appearance of broom, walks away with broom
				// true, false, true = walk away with broom
				// true, true, true = walk away with broom
				// 0.0 = drops broom
				// 2.0 = drops broom

			//	Vector3 vpedloc = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerTarget, 0.0f, 0.0f, 0.0f);
			//	TASK::TASK_USE_NEAREST_SCENARIO_CHAIN_TO_COORD(playerTarget, vpedloc.x, vpedloc.y, vpedloc.z, 100, true, true, true, true);

			//	TASK::TASK_KNOCKED_OUT_AND_HOGTIED(playerTarget, 0, true);

		}





		
		// You MUST have a wait in while loops, or the game will freeze.

		WAIT(0);

// REFERENCE

		// ------------------------------------------------- WORKING PARALYSIS --------------------------------------------------------------------

		//TASK::TASK_STAY_IN_COVER(ped);
		//PED::SET_PED_TO_RAGDOLL(ped, 3000, 3000, 3, false, false, false); // duration 1 = actual duration / duration 2 = actual duration
		//PED::SET_PED_RAGDOLL_FORCE_FALL(ped);

		//FOLLOWED BY

		//TASK::TASK_STAY_IN_COVER(peds[i]);
		//PED::SET_PED_TO_RAGDOLL(peds[i], 3000, 3000, 3, false, false, false); // duration 1 = actual duration / duration 2 = actual duration

		// ------------------------------------------------- END WORKING PARALYSIS -----------------------------------------------------------------

	}
}
