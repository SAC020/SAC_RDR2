
#include "script.h"
#include "keyboard.h"
#include <vector>

namespace std
{
#include <cstdlib>
};


void sac_replenish_cores()
{
	Player player = PLAYER::PLAYER_ID();

	//on-off-switch for the feature
	int ini_coredepletion = 1;

	//amount of core depletion
	int ini_coredepletionamounthealth = 6;
	int ini_coredepletionamountstamina = 6;
	int ini_coredepletionamountdeadeye = 6;

	//time interval for core depletion (in milliseconds)
	int ini_coredepletiontime = 3000;

	//timer for core depletion
	int coredepletiontimer = MISC::GET_GAME_TIMER() + ini_coredepletiontime;


	//core depletion code
	if (ini_coredepletion == 1)
	{
		int corehealth = ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 0);
		int corestamina = ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 1);
		int coredeadeye = ATTRIBUTE::_GET_ATTRIBUTE_CORE_VALUE(player, 2);
		if (MISC::GET_GAME_TIMER() > coredepletiontimer)
		{
			ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(player, 0, corehealth + ini_coredepletionamounthealth);
			ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(player, 1, corestamina + ini_coredepletionamountstamina);
			ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(player, 2, coredeadeye + ini_coredepletionamountdeadeye);
			coredepletiontimer = MISC::GET_GAME_TIMER() + ini_coredepletiontime;
		}
	}


	//player Ped being set again and again (in case it changes)
	player = PLAYER::PLAYER_PED_ID();


}



bool sac_is_ped_hogtied(Ped ped) // works
{
	if (TASK::GET_IS_TASK_ACTIVE(ped, 400) || TASK::GET_IS_TASK_ACTIVE(ped, 399))
	{
		return true;
	}
	else
	{
		return false;
	}

}


void sac_ragdoll_ped(Ped ped)
{

//	PED::CLEAR_RAGDOLL_BLOCKING_FLAGS(ped, 1);

//	if (PED::IS_PED_RAGDOLL(ped)) PED::RESET_PED_RAGDOLL_TIMER(ped);
//	else
//	{
	//	TASK::CLEAR_PED_TASKS_IMMEDIATELY(ped, false, false);
	//	PED::_RESET_PED_RAGDOLL_BLOCKING_FLAGS(ped, 1);
	//	TASK::TASK_STAY_IN_COVER(ped);
	//	if (PED::IS_PED_RAGDOLL(ped)) PED::RESET_PED_RAGDOLL_TIMER(ped);
	//	else
	//	{
	//		TASK::CLEAR_PED_TASKS_IMMEDIATELY(ped, false, false);
	//		TASK::CLEAR_PED_SECONDARY_TASK(ped);
			for (int i_flag = 0; i_flag < 20; i_flag++) PED::CLEAR_RAGDOLL_BLOCKING_FLAGS(ped, i_flag);
			TASK::TASK_STAY_IN_COVER(ped);
	//		PED::SET_PED_TO_RAGDOLL(ped, 3000, 3000, 3, false, false, false); // paralysis
			PED::SET_PED_TO_RAGDOLL(ped, 3000, 3000, 0, false, false, false); // dying state 1
			PED::SET_PED_RAGDOLL_FORCE_FALL(ped);
	//	}
//		PED::SET_PED_RAGDOLL_FORCE_FALL(ped);

		HUD::_DISPLAY_TEXT("RAGDOLL", 0, 0);

//	}

		DECORATOR::DECOR_SET_INT(ped, "SAC_ragdoll", 1);

//	Blip pedblip = MAP::BLIP_ADD_FOR_ENTITY(0x19365607, ped);
//	MAP::_SET_BLIP_NAME_FROM_PLAYER_STRING(pedblip, "PDO bleeder");

}





Ped spawnPed(Hash pedModel, float coordX, float coordY, float coordZ)
{
	STREAMING::REQUEST_MODEL(pedModel, true);
	while (!STREAMING::HAS_MODEL_LOADED(pedModel)) WAIT(0);
	Ped pedSpawn = PED::CREATE_PED(pedModel, coordX, coordY, coordZ, 0, 1, 1, 1, 1);
		// PED::_SET_RANDOM_OUTFIT_VARIATION(pedSpawn, 1);
					
		PED::_SET_PED_OUTFIT_PRESET(pedSpawn, (std::rand() % (PED::GET_NUM_META_PED_OUTFITS(pedSpawn) - 1)), false);
		
		ENTITY::PLACE_ENTITY_ON_GROUND_PROPERLY(pedSpawn, 1);
		ENTITY::FREEZE_ENTITY_POSITION(pedSpawn, 0);
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(pedModel);
	return pedSpawn;
}


void sac_spawn_female()
{
	Vector3 vfront = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 3.0f, 3.0f, 0.0f);

	Ped myPed = spawnPed((MISC::GET_HASH_KEY("sac_female")), vfront.x, vfront.y, vfront.z);

	TASK::TASK_TURN_PED_TO_FACE_ENTITY(myPed, PLAYER::PLAYER_PED_ID(), 4000, 0, 0, 0);

	Vector3 vpedloc = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(myPed, 0.0f, 0.0f, 0.0f);
	TASK::_TASK_USE_NEAREST_SCENARIO_TO_COORD(myPed, vpedloc.x, vpedloc.y, vpedloc.z, 100, -1, true, true, true, true);
//	HUD::_DISPLAY_TEXT("PLAY SCENARIO", 0, 0);
}


void ScriptMain()
{

	DECORATOR::DECOR_REGISTER("SAC_ragdoll", 3);

	srand(GetTickCount());

		int msgtime{ 0 };

//		Player player = PLAYER::PLAYER_PED_ID();

		Player player = PLAYER::PLAYER_ID();

//		bool activation_state{ false };
//		constexpr int activation_key{ VK_F7 };


	while (true)
	{
		// do stuff here.


		const int ARR_SIZE = 150;
		Ped peds[ARR_SIZE];
		int count = worldGetAllPeds(peds, ARR_SIZE);

		for (int i = 0; i < count; i++)
		{


			PED::SET_PED_CAN_ARM_IK(peds[i], true);
			PED::SET_PED_CAN_HEAD_IK(peds[i], true);
			PED::SET_PED_CAN_LEG_IK(peds[i], true);
			PED::SET_PED_CAN_TORSO_IK(peds[i], true);


			if (DECORATOR::DECOR_EXIST_ON(peds[i], "SAC_ragdoll") && !ENTITY::IS_ENTITY_DEAD(peds[i]))
		//		if (PED::IS_PED_RAGDOLL(peds[i])) PED::RESET_PED_RAGDOLL_TIMER(peds[i]);
		//		else
					{
					for (int i_flag = 0; i_flag < 20; i_flag++) PED::CLEAR_RAGDOLL_BLOCKING_FLAGS(peds[i], i_flag);
			//		TASK::CLEAR_PED_TASKS_IMMEDIATELY(peds[i], false, false); //stop whatever the NPC is doing
					TASK::TASK_STAY_IN_COVER(peds[i]);
			//		PED::SET_PED_TO_RAGDOLL(peds[i], 3000, 3000, 3, false, false, false); // paralysis
					PED::SET_PED_TO_RAGDOLL(peds[i], 3000, 3000, 0, false, false, false); // dying state 1
			//		PED::SET_PED_RAGDOLL_FORCE_FALL(peds[i]);
					}
			

		//	TASK::TASK_HOGTIEABLE(peds[i]);
			//filling map for recognized NPCs (if the NPC is not already present in the map)
		//	if (sac_is_ped_hogtied(peds[i]) && !ENTITY::IS_ENTITY_DEAD(peds[i]))
		//	{
				// TASK::TASK_TURN_PED_TO_FACE_ENTITY(peds[i], peds[i], 1000, 0, 0, 0);
		//		Vector3 plCoords = ENTITY::GET_ENTITY_COORDS(peds[i], true, false);
		//		TASK::TASK_TURN_PED_TO_FACE_COORD(peds[i], plCoords.x, plCoords.y, plCoords.z-10, 0);
		//	}

			if (!PED::IS_PED_RAGDOLL(peds[i]))
			{
				if ((PED::GET_PED_TYPE(peds[i]) == 5) && (std::rand() % (99999 - 0 + 1)) < 2)
				{
					TASK::TASK_START_SCENARIO_IN_PLACE_HASH(peds[i], MISC::GET_HASH_KEY("WORLD_HUMAN_GRAVE_MOURNING_KNEEL"), 60000, true, false, 1.0, false);
				}

				if ((PED::GET_PED_TYPE(peds[i]) == 5) && (std::rand() % (99999 - 0 + 1)) < 2)
				{
					TASK::TASK_START_SCENARIO_IN_PLACE_HASH(peds[i], MISC::GET_HASH_KEY("GENERIC_SIT_GROUND_SCENARIO"), 60000, true, false, 1.0, false);
				}

				if ((PED::GET_PED_TYPE(peds[i]) == 5) && (std::rand() % (99999 - 0 + 1)) < 2)
				{
					TASK::TASK_START_SCENARIO_IN_PLACE_HASH(peds[i], MISC::GET_HASH_KEY("WORLD_CAMP_FIRE_SIT_GROUND"), 60000, true, false, 1.0, false);
				}

				if ((PED::GET_PED_TYPE(peds[i]) == 5) && (std::rand() % (99999 - 0 + 1)) < 2)
				{
					TASK::TASK_START_SCENARIO_IN_PLACE_HASH(peds[i], MISC::GET_HASH_KEY("WORLD_HUMAN_BROOM_WORKING"), 60000, true, false, 1.0, false);
				}

				if ((PED::GET_PED_TYPE(peds[i]) == 5) && (std::rand() % (99999 - 0 + 1)) < 2)
				{
					TASK::TASK_START_SCENARIO_IN_PLACE_HASH(peds[i], MISC::GET_HASH_KEY("WORLD_HUMAN_BUCKET_POUR_LOW"), 60000, true, false, 1.0, false);
				}

				if ((PED::GET_PED_TYPE(peds[i]) == 5) && (std::rand() % (99999 - 0 + 1)) < 2)
				{
					TASK::TASK_START_SCENARIO_IN_PLACE_HASH(peds[i], MISC::GET_HASH_KEY("WORLD_HUMAN_STRAW_BROOM_WORKING"), 60000, true, false, 1.0, false);
				}

				if ((PED::GET_PED_TYPE(peds[i]) == 5) && (std::rand() % (99999 - 0 + 1)) < 2)
				{
					TASK::TASK_START_SCENARIO_IN_PLACE_HASH(peds[i], MISC::GET_HASH_KEY("WORLD_HUMAN_SIT_GROUND_DRINKING_DRUNK_PASSED_OUT"), 60000, true, false, 1.0, false);
				}

				if ((PED::GET_PED_TYPE(peds[i]) == 5) && (std::rand() % (99999 - 0 + 1)) < 2)
				{
					TASK::TASK_START_SCENARIO_IN_PLACE_HASH(peds[i], MISC::GET_HASH_KEY("WORLD_HUMAN_SLEEP_GROUND_ARM"), 60000, true, false, 1.0, false);
				}

				if ((PED::GET_PED_TYPE(peds[i]) == 5) && (std::rand() % (99999 - 0 + 1)) < 2)
				{
					TASK::TASK_START_SCENARIO_IN_PLACE_HASH(peds[i], MISC::GET_HASH_KEY("WORLD_HUMAN_SIT_GROUND"), 60000, true, false, 1.0, false);
				}

				if ((PED::GET_PED_TYPE(peds[i]) == 5) && (std::rand() % (99999 - 0 + 1)) < 2)
				{
					TASK::TASK_START_SCENARIO_IN_PLACE_HASH(peds[i], MISC::GET_HASH_KEY("WORLD_HUMAN_SIT_DRINK"), 60000, true, false, 1.0, false);
				}

				if ((PED::GET_PED_TYPE(peds[i]) == 5) && (std::rand() % (99999 - 0 + 1)) < 2)
				{
					TASK::TASK_START_SCENARIO_IN_PLACE_HASH(peds[i], MISC::GET_HASH_KEY("WORLD_HUMAN_WASH_FACE_BUCKET_GROUND"), 60000, true, false, 1.0, false);
				}

				if ((PED::GET_PED_TYPE(peds[i]) == 5) && (std::rand() % (99999 - 0 + 1)) < 2)
				{
					TASK::TASK_START_SCENARIO_IN_PLACE_HASH(peds[i], MISC::GET_HASH_KEY("WORLD_HUMAN_SIT_GROUND_SKETCHING"), 60000, true, false, 1.0, false);
				}

				if ((PED::GET_PED_TYPE(peds[i]) == 5) && (std::rand() % (99999 - 0 + 1)) < 2)
				{
					TASK::TASK_START_SCENARIO_IN_PLACE_HASH(peds[i], MISC::GET_HASH_KEY("WORLD_HUMAN_SIT_GROUND_READING"), 60000, true, false, 1.0, false);
				}

			}

			//		MP_LOBBY_WORLD_HUMAN_CROUCH_INSPECT
			//		MP_LOBBY_WORLD_HUMAN_STARE_STOIC
			//		MP_LOBBY_WORLD_HUMAN_STERNGUY_IDLES
			//		PROP_HUMAN_CATFISH_COWER_BED
			//		PROP_HUMAN_SEAT_BENCH_FIDDLE
			//		PROP_HUMAN_SEAT_CHAIR_CLEAN_SADDLE
			//		WORLD_CAMP_JACK_ES_READ_GROUND
			//		WORLD_HUMAN_BROOM
			//		WORLD_HUMAN_BROOM_RESTING
			//		WORLD_HUMAN_BUCKET_FILL
			//		WORLD_HUMAN_CROUCH_INSPECT
			//		WORLD_HUMAN_DRINKING
			//		WORLD_HUMAN_DRINKING_INTERACTION
			//		WORLD_HUMAN_FEED_CHICKEN
			//		WORLD_HUMAN_FIRE_STAND
			//		WORLD_HUMAN_GRAVE_MOURNING
			//		WORLD_HUMAN_GRAVE_MOURNING_KNEEL
			//		WORLD_HUMAN_LEAN_BACK_WALL_DRINKING
			//		WORLD_HUMAN_LEAN_WALL_DRINKING
			//		WORLD_HUMAN_SIT_BACK_EXHAUSTED
			//		WORLD_HUMAN_SIT_FALL_ASLEEP
			//		WORLD_HUMAN_SIT_GROUND_COFFEE_DRINK
			//		WORLD_HUMAN_SIT_GROUND_READING_BOOK
			//		WORLD_HUMAN_SLEEP_GROUND_PILLOW
			//		WORLD_HUMAN_SLEEP_GROUND_PILLOW_NO_PILLOW
			//		WORLD_HUMAN_STRAW_BROOM_RESTING
			//		WORLD_HUMAN_WASH_FACE_BUCKET_GROUND_NO_BUCKET
			//		WORLD_HUMAN_WRITE_NOTEBOOK






			

			// *********************************** Hanging management *********************************************** //

			if (DECORATOR::DECOR_EXIST_ON(peds[i], "TYL_hanged") && !ENTITY::IS_ENTITY_DEAD(peds[i]))
			{
				Vector3 vecfoot = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(peds[i], PED::GET_PED_BONE_INDEX(peds[i], 45454));
					float groundzcoordped;
					MISC::GET_GROUND_Z_FOR_3D_COORD(vecfoot.x, vecfoot.y, vecfoot.z, &groundzcoordped, true);
					if (vecfoot.z > groundzcoordped + 20)	// is hanging
					{
						if (sac_is_ped_hogtied(peds[i]))
						{
							//	TASK::TASK_STAY_IN_COVER(ped);
							if (PED::IS_PED_RAGDOLL(peds[i])) PED::RESET_PED_RAGDOLL_TIMER(peds[i]);
							else
							{
								PED::SET_PED_TO_RAGDOLL(peds[i], 5000, 5000, 0, false, false, false);
								PED::SET_PED_RAGDOLL_FORCE_FALL(peds[i]);
							}
						}
			//		TASK::TASK_STAY_IN_COVER(peds[i]);
			//		if (PED::IS_PED_RAGDOLL(peds[i])) PED::RESET_PED_RAGDOLL_TIMER(peds[i]);
			//		else
			//			{
			//			TASK::CLEAR_PED_TASKS_IMMEDIATELY(peds[i], false, false); //stop whatever the NPC is doing
			//			TASK::TASK_STAY_IN_COVER(peds[i]);
						//  PED::SET_PED_TO_RAGDOLL(peds[i], 5000, 5000, 0, false, false, false);
						//	PED::SET_PED_RAGDOLL_FORCE_FALL(peds[i]);

							for (int i_flag = 0; i_flag < 20; i_flag++) PED::CLEAR_RAGDOLL_BLOCKING_FLAGS(peds[i], i_flag);
							PED::SET_PED_TO_RAGDOLL(peds[i], 3000, 3000, 1, false, false, false);

						//	PED::SET_PED_TO_RAGDOLL(peds[i], 3000, 3000, 2, false, false, false);
						//	PED::SET_PED_TO_RAGDOLL(peds[i], 3000, 3000, 3, false, false, false);
			//			}						
					}
			}


			// *********************************** End hanging management *********************************************** //

		}


		if (std::rand() % (59999 - 0 + 1) < 2) // automatically spawn random female
		{
		sac_spawn_female();
		}





		if (IsKeyJustUp(VK_F3)) // F3 to spawn a female // works only with PLAYER::PLAYER_PED_ID()
		{

		//	Vector3 plCoords2 = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true, false);

			sac_spawn_female();


		//	HUD::_DISPLAY_TEXT("Spawning NPC", 0, 0);
		}

		

		Entity playerFreeAimingTarget, playerTarget;

		PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playerFreeAimingTarget);	// works only with PLAYER_ID()
		PLAYER::GET_PLAYER_TARGET_ENTITY(player, &playerTarget);						// works only with PLAYER_ID()

		if (PED::IS_PED_HUMAN(playerFreeAimingTarget)) // && activation_state) // && peds[i] != player)
		{

			if (IsKeyJustUp(VK_F7))		// F7 to perform scenario
			{
			//	TASK::TASK_KNOCKED_OUT(playerFreeAimingTarget, 0, false);
			//	TASK::TASK_KNOCKED_OUT_AND_HOGTIED(playerFreeAimingTarget, 0, false);

			}

		}


		if (PED::IS_PED_HUMAN(playerTarget)) // && activation_state) // && peds[i] != player)
		{
			if (IsKeyJustUp(VK_F7))		// F7 to perform scenario
			{
			//	TASK::TASK_KNOCKED_OUT(playerTarget, 0, false);
			//	TASK::TASK_KNOCKED_OUT_AND_HOGTIED(playerTarget, 0, false);

			}

		}



		if (PED::IS_PED_HUMAN(playerFreeAimingTarget)) // && sac_is_ped_hogtied(playerFreeAimingTarget)) // && activation_state) // && peds[i] != player)
		{
			// AI::TASK_HANDS_UP(peds[i], 20000, player, 0, false);
			// AI::_0x42AC6401ABB8C7E5(playerFreeAimingTarget, 1, true);	// TASK_KNOCKED_OUT_AND_HOGTIED 
			// AI::_0x27829AFD3E03AC1A(player, playerFreeAimingTarget);    // TASK_HOGTIE_TARGET_PED
			// AI::_0x7981037A96E7D174(playerFreeAimingTarget);			// _SET_ENABLE_HANDCUFFS_2 <= works
			// AI::TASK_CLEAR_LOOK_AT(playerFreeAimingTarget);			// not working
			// AI::TASK_STAND_STILL(playerFreeAimingTarget, -1);		// not working

			// AI::CLEAR_PED_TASKS_IMMEDIATELY(playerFreeAimingTarget, false, false); // breaks the hogtie and makes them ragdoll
			
			if (IsKeyJustUp(VK_F5))		// F5 to ragdoll ped while aiming
			{
				sac_ragdoll_ped(playerFreeAimingTarget);
			}
			
			// HUD::_DISPLAY_TEXT("Targeting a hogtied NPC", 0, 0);

		}

		if (PED::IS_PED_HUMAN(playerTarget)) // && sac_is_ped_hogtied(playerTarget)) // && activation_state) // && peds[i] != player)
		{
			// AI::TASK_HANDS_UP(peds[i], 20000, player, 0, false);
			// AI::_0x42AC6401ABB8C7E5(playerTarget, 1, true);	// TASK_KNOCKED_OUT_AND_HOGTIED 
			// AI::_0x27829AFD3E03AC1A(player, playerTarget); // TASK_HOGTIE_TARGET_PED
			// AI::_0x7981037A96E7D174(playerTarget);			// _SET_ENABLE_HANDCUFFS_2
			// AI::TASK_CLEAR_LOOK_AT(playerTarget);			// not working
			// AI::TASK_STAND_STILL(playerTarget, -1);			// not working

			// AI::CLEAR_PED_TASKS_IMMEDIATELY(playerTarget, false, false);

			if (IsKeyJustUp(VK_F5))		// F5 to ragdoll ped while aiming
			{
				sac_ragdoll_ped(playerTarget);
			}



		//	HUD::_DISPLAY_TEXT("Targeting a hogtied NPC", 0, 0);
			

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
