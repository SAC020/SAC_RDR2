#pragma once
#include "script.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <ctime>
#include <map>
#include <fstream>

using namespace std;

struct ScriptedSpeechParams
{
	const char* speechName;
	const Any* voiceName;
	//const char* voiceName;
	alignas(8) int v3;
	alignas(8) Hash speechParamHash;
	alignas(8) Entity entity;
	alignas(8) BOOL v6;
	alignas(8) int v7;
	alignas(8) int v8;
};

bool DisarmPed(Ped ped, Vector3 weaponhandpos)
{
	Hash pedweaphash;
	bool ispedarmed = WEAPON::GET_CURRENT_PED_WEAPON(ped, &pedweaphash, false, 0, false);
	WEAPON::SET_PED_DROPS_INVENTORY_WEAPON(ped, pedweaphash, weaponhandpos.x, weaponhandpos.y, weaponhandpos.z, 30);
	return ispedarmed;
}

bool isPedStoryChar(Ped ped)
{
	//container for ped model names
	std::map<int, char*> pedmodels;
	pedmodels[1] = "CS_johnmarston";
	pedmodels[2] = "CS_MicahBell";
	pedmodels[3] = "CS_dutch";
	pedmodels[4] = "CS_lenny";
	pedmodels[5] = "CS_hoseamatthews";
	pedmodels[6] = "CS_leostrauss";
	pedmodels[7] = "CS_rainsfall";
	pedmodels[8] = "CS_javierescuella";
	pedmodels[9] = "CS_billwilliamson";
	pedmodels[10] = "CS_uncle";
	pedmodels[11] = "CS_sean";
	pedmodels[12] = "CS_charlessmith";
	pedmodels[13] = "CS_EagleFlies";
	pedmodels[14] = "CS_PAYTAH";
	pedmodels[15] = "CS_hercule";
	pedmodels[16] = "CS_leon";
	pedmodels[17] = "CS_mrsadler";
	pedmodels[18] = "CS_kieran";
	pedmodels[19] = "CS_mrpearson";
	pedmodels[20] = "CS_JAMIE";
	pedmodels[21] = "CS_Jules";
	pedmodels[22] = "CS_bronte";
	pedmodels[23] = "CS_revswanson";
	pedmodels[24] = "CS_MARSHALL_THURWELL";
	pedmodels[25] = "CS_LARAMIE";
	pedmodels[26] = "CS_ArchieDown";
	pedmodels[27] = "CS_thomasdown";
	pedmodels[28] = "CS_EdithDown";
	pedmodels[29] = "U_M_M_BHT_BENEDICTALLBRIGHT";
	pedmodels[30] = "CS_creolecaptain";
	pedmodels[31] = "CS_josiahtrelawny";

	std::map<int, char*>::iterator pedmodels_it = pedmodels.begin();
	
	//Spines of America NPCs
	if (PED::IS_PED_MODEL(ped, 1045059103) || PED::IS_PED_MODEL(ped, 1765636093) || PED::IS_PED_MODEL(ped, 847448713)) return true;

	//NPCs listed above
	while (pedmodels_it != pedmodels.end())
	{
		char* pedmodel = pedmodels_it->second;
		if (PED::IS_PED_MODEL(ped, MISC::GET_HASH_KEY(pedmodel))) return true;
		pedmodels_it++;
	}

	return false;
}

int PedFear(Ped ped, int mode, int lastfearaudio, int ds2chance) //returns last used speech line (key)
{	
	//container for speech lines
	std::map<int, char*> speech;
		
	//setting up the speech options
	if (mode == 1) //dying stage 1
	{
		speech[1] = "PANIC_HELP";
		speech[2] = "GENERIC_SHOCKED_HIGH";
		speech[3] = "SCARED_HELP";
		speech[4] = "GENERIC_CURSE_HIGH";
		//- speech[5] = "INTIMIDATED_GEN";
		speech[5] = "GENERIC_FRIGHTENED_HIGH";
		//-speech[7] = "GENERIC_AFRAID_REACTION";
		speech[6] = "SCREAM_SHOCKED";
		speech[7] = "SCREAM_TERROR";
		//speech[10] = "PLEAD";
		//speech[6] = "GUN_RUN";
		//speech[8] = "INTIMIDATED_AGAIN_GEN";
		//speech[9] = "GUN";
	}
	else if (mode == 2) //dying stage 2
	{
		speech[1] = "PANIC_HELP";
		speech[2] = "SCARED_HELP";
		speech[3] = "GENERIC_FRIGHTENED_HIGH";
		speech[4] = "SCREAM_SHOCKED";
		speech[5] = "SCREAM_TERROR";
		//speech[3] = "PLEAD";
	}
	else if (mode == 3)//only pleading, not used
	{
		speech[1] = "PLEAD";
	}
	else if (mode == 4) //screaming
	{
		speech[1] = "SCREAM_SHOCKED";
		speech[2] = "SCREAM_TERROR";
		speech[3] = "GENERIC_FRIGHTENED_HIGH";
		speech[4] = "GENERIC_SHOCKED_HIGH";
	}
	else if (mode == 5) //fire screaming
	{
		speech[1] = "SCREAM_SHOCKED";
		speech[2] = "SCREAM_TERROR";
	}

	std::map<int, char*>::iterator speechl_it = speech.begin();

	int linerand = 1 + (std::rand() % (speech.size() - 1 + 1));
	int linecounter = 1;

	while (linerand == lastfearaudio) linerand = 1 + (std::rand() % (speech.size() - 1 + 1));

	while (speechl_it != speech.end())
	{
		if (linerand == linecounter)
		{
			string speechline = speechl_it -> second;
			ScriptedSpeechParams params{ speechline.c_str(), 0, 1, 0x67F3AB43, 0, true, 1, 1 };
			if(!AUDIO::IS_ANY_SPEECH_PLAYING(ped)) 
			{
				int yrand = 0 + (std::rand() % (100 - 0 + 1));
				if (yrand > ds2chance && mode == 2) return lastfearaudio;
			
				if (AUDIO::PLAY_PED_AMBIENT_SPEECH_NATIVE(ped, (Any*)&params)) return linerand;
				else return lastfearaudio;			
			}
			else return lastfearaudio;
		}
		linecounter++;
		speechl_it++;
	}
}

void DrawText(float x, float y, char* str)
{
	HUD::_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", str), x, y);
}

bool does_file_exist(const char* fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

void PedPanic(Ped ped)
{
	if (PED::IS_PED_MALE(ped))
	{
		STREAMING::REQUEST_ANIM_DICT("face_human@gen_male@base");
		while (!STREAMING::HAS_ANIM_DICT_LOADED("face_human@gen_male@base"))
		{
		}
	//	PED::_0x8B3B71C80A29A4BB(ped, "pain_1", "face_human@gen_male@base"); PDO original
		PED::SET_FACIAL_IDLE_ANIM_OVERRIDE(ped, "pain_1", "face_human@gen_male@base");
	}
	else
	{
		STREAMING::REQUEST_ANIM_DICT("face_human@gen_female@base");
		while (!STREAMING::HAS_ANIM_DICT_LOADED("face_human@gen_female@base"))
		{
		}
	//	PED::_0x8B3B71C80A29A4BB(ped, "pain_1", "face_human@gen_female@base");
		PED::SET_FACIAL_IDLE_ANIM_OVERRIDE(ped, "pain_1", "face_human@gen_female@base");
	}
	AUDIO::PLAY_PAIN(ped, 1, 1.0f, false, false);
}

void PedPain(Ped ped)
{
	//container for pain face animations
	std::map<char*, char*> faceanimmale;
	faceanimmale["pain_01"] = "face_human@gen_male@base";
	//faceanimmale["pain_2"] = "face_human@gen_male@base";
	//faceanimmale["pain_3"] = "face_human@gen_male@base";
	//faceanimmale["pain_4"] = "face_human@gen_male@base";
	//faceanimmale["pain_5"] = "face_human@gen_male@base";
	//faceanimmale["burning_1"] = "face_human@gen_male@base";
	//faceanimmale["coughing_1"] = "face_human@gen_male@base";
	//faceanimmale["die_1"] = "face_human@gen_male@base";
	//faceanimmale["die_2"] = "face_human@gen_male@base";
	//faceanimmale["electrocuted_1"] = "face_human@gen_male@base";
	//faceanimmale["mood_injured_1"] = "face_human@gen_male@base";
	//faceanimmale["pose_injured_1"] = "face_human@gen_male@base";

	//container for pain face animations
	std::map<char*, char*> faceanimfemale;
	faceanimfemale["pain_01"] = "face_human@gen_female@base";
	//faceanimfemale["pain_2"] = "face_human@gen_female@base";
	//faceanimfemale["pain_3"] = "face_human@gen_female@base";
	//faceanimfemale["pain_4"] = "face_human@gen_female@base";
	//faceanimfemale["pain_5"] = "face_human@gen_female@base";
	//faceanimfemale["burning_1"] = "face_human@gen_female@base";
	//faceanimfemale["coughing_1"] = "face_human@gen_female@base";
	//faceanimfemale["die_1"] = "face_human@gen_female@base";
	//faceanimfemale["die_2"] = "face_human@gen_female@base";
	//faceanimfemale["electrocuted_1"] = "face_human@gen_female@base";
	//faceanimfemale["mood_injured_1"] = "face_human@gen_female@base";
	//faceanimfemale["pose_injured_1"] = "face_human@gen_female@base";

	int pain = 2 + (std::rand() % (12 - 2 + 1));;
	if (PED::IS_PED_MALE(ped))
	{
		bool animplaying = false;
		std::map<char*, char*>::iterator anim_it = faceanimmale.begin();

		/*
		while (anim_it != faceanimmale.end())
		{
			char* anim = anim_it->first;
			char* dict = anim_it->second;
			if (ENTITY::IS_ENTITY_PLAYING_ANIM(ped, dict, anim, 3))
			{
				animplaying = true;
			}
			anim_it++;
		}
		*/

		if (!animplaying)
		{
			anim_it = faceanimmale.begin();
			int facerand = 1 + (std::rand() % (faceanimmale.size() - 1 + 1));
			int counter = 1;
			while (anim_it != faceanimmale.end())
			{
				if (facerand == counter)
				{
					char* anim = anim_it->first;
					char* dict = anim_it->second;
					STREAMING::REQUEST_ANIM_DICT(dict);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict))
					{
						PED::RESET_PED_RAGDOLL_TIMER(ped);
					}
				//	PED::_0x8B3B71C80A29A4BB(ped, anim, dict);
					PED::SET_FACIAL_IDLE_ANIM_OVERRIDE(ped, anim, dict);
					AUDIO::PLAY_PAIN(ped, pain, 1.0f, false, false);
				}
				counter++;
				anim_it++;
			}
		}
	}
	else
	{
		bool animplaying = false;
		std::map<char*, char*>::iterator anim_it = faceanimfemale.begin();

		/*
		while (anim_it != faceanimfemale.end())
		{
			char* anim = anim_it->first;
			char* dict = anim_it->second;
			if (ENTITY::IS_ENTITY_PLAYING_ANIM(peds[i], dict, anim, 3))
			{
				animplaying = true;
			}
			anim_it++;
		}
		*/

		if (!animplaying)
		{
			anim_it = faceanimfemale.begin();
			int facerand = 1 + (std::rand() % (faceanimfemale.size() - 1 + 1));
			int counter = 1;
			while (anim_it != faceanimfemale.end())
			{
				if (facerand == counter)
				{
					char* anim = anim_it->first;
					char* dict = anim_it->second;
					STREAMING::REQUEST_ANIM_DICT(dict);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict))
					{
						PED::RESET_PED_RAGDOLL_TIMER(ped);
					}
				//	PED::_0x8B3B71C80A29A4BB(ped, anim, dict);
					PED::SET_FACIAL_IDLE_ANIM_OVERRIDE(ped, anim, dict);
					AUDIO::PLAY_PAIN(ped, pain, 1.0f, false, false);
				}
				counter++;
				anim_it++;
			}
		}
	}
}


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

void PedPlayRandomScenario(Ped ped)
{

	std::map<int, char*> pedscenarios;

	pedscenarios[1] = "GENERIC_SIT_GROUND_SCENARIO";
	pedscenarios[2] = "GENERIC_STANDING_SCENARIO";
	pedscenarios[3] = "MP_LOBBY_WORLD_HUMAN_CROUCH_INSPECT";
	pedscenarios[4] = "MP_LOBBY_WORLD_HUMAN_STARE_STOIC";
	pedscenarios[5] = "MP_LOBBY_WORLD_HUMAN_STERNGUY_IDLES";
	pedscenarios[6] = "PROP_HUMAN_CATFISH_COWER_BED";
	pedscenarios[7] = "PROP_HUMAN_SEAT_BENCH_FIDDLE";
	pedscenarios[8] = "PROP_HUMAN_SEAT_CHAIR_CLEAN_SADDLE";
	pedscenarios[9] = "WORLD_CAMP_FIRE_SIT_GROUND";
	pedscenarios[10] = "WORLD_CAMP_JACK_ES_READ_GROUND";
	pedscenarios[11] = "WORLD_HUMAN_BROOM";
	pedscenarios[12] = "WORLD_HUMAN_BROOM_RESTING";
	pedscenarios[13] = "WORLD_HUMAN_BROOM_WORKING";
	pedscenarios[14] = "WORLD_HUMAN_BUCKET_FILL";
	pedscenarios[15] = "WORLD_HUMAN_BUCKET_POUR_LOW";
	pedscenarios[16] = "WORLD_HUMAN_CROUCH_INSPECT";
	pedscenarios[17] = "WORLD_HUMAN_DRINKING";
	pedscenarios[18] = "WORLD_HUMAN_DRINKING_INTERACTION";
	pedscenarios[19] = "WORLD_HUMAN_FEED_CHICKEN";
	pedscenarios[20] = "WORLD_HUMAN_FIRE_STAND";
	pedscenarios[21] = "WORLD_HUMAN_GRAVE_MOURNING";
	pedscenarios[22] = "WORLD_HUMAN_GRAVE_MOURNING_KNEEL";
	pedscenarios[23] = "WORLD_HUMAN_LEAN_BACK_WALL_DRINKING";
	pedscenarios[24] = "WORLD_HUMAN_LEAN_WALL_DRINKING";
	pedscenarios[25] = "WORLD_HUMAN_SIT_BACK_EXHAUSTED";
	pedscenarios[26] = "WORLD_HUMAN_SIT_DRINK";
	pedscenarios[27] = "WORLD_HUMAN_SIT_FALL_ASLEEP";
	pedscenarios[28] = "WORLD_HUMAN_SIT_GROUND";
	pedscenarios[29] = "WORLD_HUMAN_SIT_GROUND_COFFEE_DRINK";
	pedscenarios[30] = "WORLD_HUMAN_SIT_GROUND_DRINKING_DRUNK_PASSED_OUT";
	pedscenarios[31] = "WORLD_HUMAN_SIT_GROUND_READING";
	pedscenarios[32] = "WORLD_HUMAN_SIT_GROUND_READING_BOOK";
	pedscenarios[33] = "WORLD_HUMAN_SIT_GROUND_SKETCHING";
	pedscenarios[34] = "WORLD_HUMAN_SLEEP_GROUND_ARM";
	pedscenarios[35] = "WORLD_HUMAN_SLEEP_GROUND_PILLOW";
	pedscenarios[36] = "WORLD_HUMAN_SLEEP_GROUND_PILLOW_NO_PILLOW";
	pedscenarios[37] = "WORLD_HUMAN_STRAW_BROOM_RESTING";
	pedscenarios[38] = "WORLD_HUMAN_STRAW_BROOM_WORKING";
	pedscenarios[39] = "WORLD_HUMAN_WASH_FACE_BUCKET_GROUND";
	pedscenarios[40] = "WORLD_HUMAN_WRITE_NOTEBOOK";

	//		v1 = rand() % 100;         // v1 in the range 0 to 99
	//		v2 = rand() % 100 + 1;     // v2 in the range 1 to 100
	//		v3 = rand() % 30 + 1985;   // v3 in the range 1985-2014 

	int randomscenario = rand() % (40 + 1);

	TASK::TASK_START_SCENARIO_IN_PLACE_HASH(ped, MISC::GET_HASH_KEY(pedscenarios[randomscenario]), 60000, true, true, 1.0, false);

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

Ped sac_spawn_female()
{
	Vector3 vfront = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0f, 5.0f, 0.0f);

	Ped myPed = spawnPed((MISC::GET_HASH_KEY("sac_female")), vfront.x, vfront.y, vfront.z);

	TASK::TASK_TURN_PED_TO_FACE_ENTITY(myPed, PLAYER::PLAYER_PED_ID(), 4000, 0, 0, 0);

	Vector3 vpedloc = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(myPed, 0.0f, 0.0f, 0.0f);
	TASK::_TASK_USE_NEAREST_SCENARIO_TO_COORD(myPed, vpedloc.x, vpedloc.y, vpedloc.z, 100, -1, true, true, true, true);

	return myPed;

	//	HUD::_DISPLAY_TEXT("PLAY SCENARIO", 0, 0);
}

