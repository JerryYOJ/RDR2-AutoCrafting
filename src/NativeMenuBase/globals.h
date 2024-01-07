#pragma once
#include "script.h"

#define _G(ptr) *getGlobalPtr(##ptr)

Ped g_pedPlayer = 35;

struct satchelData {
	BOOL _bVisable = BASE + 1;
	Hash CurrItem = BASE + 9440;
private:
	static const int BASE = 1935689;
}g_satchelData;

struct playerData
{
	BOOL bPlayerDead = BASE + 12;
	BOOL _inRespawnSequence = BASE + 4; // True until player moves after respawning
	BOOL _isWanted = BASE + 23;
	BOOL _inCombat = BASE + 24;
	Ped _currentMountedHorse = BASE + 40;
	Hash _currentWeapon = BASE + 44;
	Hash _lastWeaponFired = BASE + 46;
	static const int BASE = 1935630;
}g_playerData;

struct savedGlobals
{
	BOOL bGameInitialized = (BASE); // +0
	BOOL bInGameflow = (BASE + 1);
	int vLastCampPosition = (BASE + 2);
	float fLastCampHeading = (BASE + 5);
	int vCurrentCaravanBedPosition = (BASE + 6);
	int minigameUnlocked = (BASE + 7854);
	int jobUnlocked = (BASE + 7855);
	int procmissionUnlocked = (BASE + 7856);
	int wornOutfit = (BASE + 7729);
	Hash playerCharacterModel = (BASE + 39);
	int lawBountyAchievement = (BASE + 449);
	int eRCMIndexTracked = (BASE + 1093);
	int iRCMsActive = (BASE + 1094);
	int deadeyeLevel = (BASE + 7755);
	BOOL bAttackedBurial = (BASE + 11953);
	int iMickeyMemory = (BASE + 11954);
	int iStudiedAnimalsNoBinoculars = (BASE + 11955);
	int iLastPayoffTutorialMessage = (BASE + 11956);
	int iSavedBounty = (BASE + 11957);
	BOOL bHatKnockedOff = (BASE + 7730);
	int iHerbArrLocation = (BASE + 9273);
	int iSPMinigameAchievementTracker = (BASE + 11958);
	int iSPChecksum = (BASE + 11999);
	BOOL bEndlessSummerInitialized = (BASE + 7860);
	int iMoneyBeforeGuarma = (BASE + 7861);
	//int _specialWildHorse = (BASE + 11945);
	BOOL bPhotoModeToastDelivered = (BASE + 12017);
	BOOL bFlacoCabinCigBoxLooted = (BASE + 12018);
	int eTutorialsRun = (BASE + 7832);
	BOOL bUsedFirstPerson = (BASE + 9421);

	//===========================================================//

	struct saveGameData // BASE + 9
	{
		int saveGameLocation = (BASE + 9); // +0
		int iSaveGamePropset = (BASE + 9 + 1);
		int vSaveGamePropsetPosition = (BASE + 9 + 2);
		float fSaveGamePropsetHeading = (BASE + 9 + 5);
		Hash iSaveGameScenarioHash = (BASE + 9 + 6);
		int vResetPosition = (BASE + 9 + 7);
		int iResetRespawnNode = (BASE + 9 + 10);
		int iResetAnimIndex = (BASE + 9 + 11);
		BOOL bForceAltAnim = (BASE + 9 + 11);
		int fResetHeading = (BASE + 9 + 11);
		int eSavedTOD = (BASE + 9 + 11);
		int respawnRegion = (BASE + 9 + 11);
		int iResetBounty = (BASE + 9 + 11);

		struct iRespawnPosHistory // BASE + 9 + 16
		{
			// Note: Size is 4
		}iRespawnPosHistory;

		struct missionReplayData // BASE + 9 + 21
		{
			int iBitStorage = (BASE + 9 + 21); // +0
			int iGeneralStorage = (BASE + 9 + 21 + 1); // Note: Size is 5
			float fGeneralStorage = (BASE + 9 + 21 + 6); // Note: Size is 3
		}missionReplayData;

	}saveGameData;

	//===========================================================//

	struct sFlow // BASE + 40
	{
		BOOL isGameflowActive = (BASE + 40); // +0
		BOOL flowCompleted = (BASE + 40 + 1);
	}sFlow;

	//===========================================================//

	struct playerHorse // BASE + 1095
	{
		int eCurrentStableSlot = (BASE + 1095); // +0


		struct sHorseSlotInfo // BASE + 1095 + 1
		{
			int txtHorseName = (BASE + 1095 + 1); // +0
			Hash eHorseBreed = (BASE + 1095 + 1 + 8);
			Hash eHorseModel = (BASE + 1095 + 1 + 9);
			int eGender = (BASE + 1095 + 1 + 10);
			int eLossType = (BASE + 1095 + 1 + 11);
			int iTimeOfLoss = (BASE + 1095 + 1 + 12);
			int todTimeOfLoss = (BASE + 1095 + 1 + 13);
			int eSlotState = (BASE + 1095 + 1 + 14);
			int vLastCoord = (BASE + 1095 + 1 + 427);
			float fLastHeading = (BASE + 1095 + 1 + 430);
			int iTimeStampDismounted = (BASE + 1095 + 1 + 431);
			int iTimeStampAcquired = (BASE + 1095 + 1 + 432);
			BOOL bHasDefaultSaddle = (BASE + 1095 + 1 + 433);
			BOOL bPlayerPurchasedHorse = (BASE + 1095 + 1 + 434);
			BOOL bPlayerBrokeHorse = (BASE + 1095 + 1 + 435);


			struct sCarriedData // BASE + 1095 + 1 + 15
			{
				Hash eModel = (BASE + 1095 + 1 + 15); // +0
				Hash eItem = (BASE + 1095 + 1 + 15 + 1);
				int eCarriableType = (BASE + 1095 + 1 + 15 + 2);
				int eCarriableConfigInfo = (BASE + 1095 + 1 + 15 + 3);
				int ePedQuality = (BASE + 1095 + 1 + 15 + 4);
				int ePedSkinQuality = (BASE + 1095 + 1 + 15 + 5);
				int ePedRarity = (BASE + 1095 + 1 + 15 + 6);
				int eDamageCleanliness = (BASE + 1095 + 1 + 15 + 7);
				Hash eOutfit = (BASE + 1095 + 1 + 15 + 8);
				int iNumMetaAssets = (BASE + 1095 + 1 + 15 + 9);
				int iCarcassAge = (BASE + 1095 + 1 + 15 + 91);
				BOOL bSkinned = (BASE + 1095 + 1 + 15 + 92);
				BOOL bLegendary = (BASE + 1095 + 1 + 15 + 93);


				struct sMetaAssetData // BASE + 1095 + 1 + 15 + 10
				{
					Hash iDrawable = (BASE + 1095 + 1 + 15 + 10); // +0
					Hash iAlbedo = (BASE + 1095 + 1 + 15 + 10 + 1);
					Hash iNormal = (BASE + 1095 + 1 + 15 + 10 + 2);
					Hash iMaterial = (BASE + 1095 + 1 + 15 + 10 + 3);
					Hash iPalette = (BASE + 1095 + 1 + 15 + 10 + 4);
					int iTint0 = (BASE + 1095 + 1 + 15 + 10 + 5);
					int iTint1 = (BASE + 1095 + 1 + 15 + 10 + 6);
					int iTint2 = (BASE + 1095 + 1 + 15 + 10 + 7);
				}sMetaAssetData;

			}sCarriedData;


			struct sPeltSkinData // BASE + 1095 + 1 + 298
			{
				int iCount = (BASE + 1095 + 1 + 298); // +0
			}sPeltSkinData;


			struct sVisualPeltSkinInfo // BASE + 1095 + 1 + 356
			{
				int SatchelItem = (BASE + 1095 + 1 + 356); // +0
				Hash TextureLookupHash = (BASE + 1095 + 1 + 356 + 1);
				int TextureLookupTint = (BASE + 1095 + 1 + 356 + 2);
				int UnusedPad0 = (BASE + 1095 + 1 + 356 + 3);
				int UnusedPad1 = (BASE + 1095 + 1 + 356 + 4);
			}sVisualPeltSkinInfo;


			struct sBondingData // BASE + 1095 + 1 + 372
			{
				int iBondLevel = (BASE + 1095 + 1 + 372); // +0
				float fBondXP = (BASE + 1095 + 1 + 372 + 1);
				float fHorseEventXP = (BASE + 1095 + 1 + 372 + 2);
			}sBondingData;


			struct sHorseCoreData // BASE + 1095 + 1 + 398
			{
				int iValue = (BASE + 1095 + 1 + 398); // +0
				float fValueBuffer = (BASE + 1095 + 1 + 398 + 1);
				float fDrainBuffer = (BASE + 1095 + 1 + 398 + 2);
				int iLastGameTime = (BASE + 1095 + 1 + 398 + 3);
			}sHorseCoreData;


			struct sEfficiencyData // BASE + 1095 + 1 + 407
			{
				int iValue = (BASE + 1095 + 1 + 407); // +0
				float fValueBuffer = (BASE + 1095 + 1 + 407 + 1);
				float fDrainBuffer = (BASE + 1095 + 1 + 407 + 2);
				int iLastGameTime = (BASE + 1095 + 1 + 407 + 3);
			}sEfficiencyData;


			struct sWrithingData // BASE + 1095 + 1 + 420
			{
				BOOL bIsWrithing = (BASE + 1095 + 1 + 420); // +0
				float fWrithingDuration = (BASE + 1095 + 1 + 420 + 1);
			}sWrithingData;


			struct sStableOwnedData // BASE + 1095 + 1 + 422
			{
				BOOL bIsStableOwned = (BASE + 1095 + 1 + 422); // +0
				int eShop = (BASE + 1095 + 1 + 422 + 1);
				int todStolen = (BASE + 1095 + 1 + 422 + 2);
			}sStableOwnedData;


			struct sHorseBurdenData // BASE + 1095 + 1 + 425
			{
				BOOL bIsStableOwned = (BASE + 1095 + 1 + 425); // +0
				int todBurdenPlaced = (BASE + 1095 + 1 + 425 + 1);
			}sHorseBurdenData;

		}sHorseSlotInfo;


		struct sSaddleInfo // BASE + 1095 + 3054
		{
			int eSaddleSlot = (BASE + 1095 + 3054); // +0
			int eSaddleState = (BASE + 1095 + 3054 + 1);
			int eDeadOrDroppedTime = (BASE + 1095 + 3054 + 2);
			int vSaddleCoords = (BASE + 1095 + 3054 + 77);


			struct sPeltSkinData // BASE + 1095 + 3054 + 3
			{
				int iCount = (BASE + 1095 + 3054 + 3); // +0
			};


			struct sVisualPeltSkinInfo // BASE + 1095 + 3054 + 61
			{
				int SatchelItem = (BASE + 1095 + 3054 + 61); // +0
				Hash TextureLookupHash = (BASE + 1095 + 3054 + 61 + 1);
				int TextureLookupTint = (BASE + 1095 + 3054 + 61 + 2);
				int UnusedPad0 = (BASE + 1095 + 3054 + 61 + 3);
				int UnusedPad1 = (BASE + 1095 + 3054 + 61 + 4);
			}sVisualPeltSkinInfo;
		}sSaddleInfo;


		struct sHorseBreakingInfo // BASE + 1095 + 3134
		{
			BOOL bMinigameSuccessful = (BASE + 1095 + 3134); // +0
		}sHorseBreakingInfo;


		struct sTutorialInfo // BASE + 1095 + 3135
		{
			int iHighestBondLevelReached = (BASE + 1095 + 3135); // +0
			int iTimesOverspurred = (BASE + 1095 + 3135 + 1);
			int iTimesRevived = (BASE + 1095 + 3135 + 2);
			int iTimesDirtyOver50Percent = (BASE + 1095 + 3135 + 3);
			BOOL bDeadHorseTutorialShouldRun = (BASE + 1095 + 3135 + 4);
		}sTutorialInfo;


		struct sHorseRaceInfo // BASE + 1095 + 3140
		{
			BOOL bSprintBondingBonusUnlocked = (BASE + 1095 + 3140); // +0
			BOOL bSpeedStatBonusUnlocked = (BASE + 1095 + 3140 + 1);
		}sHorseRaceInfo;

	}playerHorse;

	//===========================================================//

	// NOT DONE
	struct postOfficeData // BASE + 9910
	{
		// NOTE: THESE ARE WRONG. TODO -- FIX THESE

		struct trackedParcels // BASE + 9910 -- Does this exist?
		{
			// Note: Size is 6

			int eMailSendFlags = (BASE + 9910); // +0
			int eMailReplyFlags = (BASE + 9910 + 1);
			int eSentTime = (BASE + 9910 + 2);
			int eDeliveryTime = (BASE + 9910 + 3);
			int eTrackedParcel = (BASE + 9910 + 4);
			BOOL bOverrideDeliveryTime = (BASE + 9910 + 5);
		}trackedParcels;

	}postOfficeData;

	//===========================================================//

	// NOT DONE
	struct caravanCamp // BASE + 4283
	{
		int eCurrentCamp = (BASE + 4283); // +0
		int eCurrentCampRegion = (BASE + 4283 + 1);
		int eCampFlags = (BASE + 4283 + 2);
		int eCampAppearance = (BASE + 4283 + 3);
		int eCampSetupState = (BASE + 4283 + 4);
		int eCampSubstate = (BASE + 4283 + 5);
		int iCampLevel = (BASE + 4283 + 312);
		int iCampChickenCoopLevel = (BASE + 4283 + 313);
		int iCampVehicleLevel = (BASE + 4283 + 314);
		int iCampToolsLevel = (BASE + 4283 + 315);
		int iCampCraftingFireLevel = (BASE + 4283 + 316);
		int iCampShavingKitLevel = (BASE + 4283 + 317);
		int iCampPearsonLevel = (BASE + 4283 + 318);
		int iCampStraussLevel = (BASE + 4283 + 319);
		int iCampArthurLevel = (BASE + 4283 + 320);
		int iCampTentLevel = (BASE + 4283 + 321);
		int iCampHitchLevel = (BASE + 4283 + 322);
		int iNumInnocentsKilled = (BASE + 4283 + 323);
		int iCampFundsCents = (BASE + 4283 + 324);
		int iGangSavingsCents = (BASE + 4283 + 325);
		int iBankDebtCents = (BASE + 4283 + 326);
		int iCarcassesDonated = (BASE + 4283 + 327);
		int todCampSupplyDecrement = (BASE + 4283 + 328);
		int todCampFoodDecrement = (BASE + 4283 + 329);
		int todNextPlayerStewServe = (BASE + 4283 + 330);
		int todNextCompanionDonation = (BASE + 4283 + 331);
		int eLastDonationCompanions = (BASE + 4283 + 332);
		int eLastDonationCompanions0 = (BASE + 4283 + 333);
		int eLastDonationCompanions1 = (BASE + 4283 + 334);
		int eCheckinState = (BASE + 4283 + 335); // this may be a struct?
		int eLongAbsenceInstancesSeen = (BASE + 4283 + 574);
		int todLongAbsenceTOD = (BASE + 4283 + 575);
		int eMiniExchangeQueue = (BASE + 4283 + 578); // Note: Size is 81


		struct activeCampStateTags // BASE + 4283 + 6
		{
			// NOTE: Size is 5. TODO: Fix these

			int eTag = (BASE + 4283 + 6); // +0
			int eExpiry = (BASE + 4283 + 6 + 1);
			int ePersistence = (BASE + 4283 + 6 + 1);
			int iPriority = (BASE + 4283 + 6 + 1);
			int eCategory = (BASE + 4283 + 6 + 1);
		}activeCampStateTags;


		// DONE
		struct sPearsonUnlocks // BASE + 4283 + 307
		{
			int iUnlockedBitset = (BASE + 4283 + 307); // +0
			int iItemsCrafted = (BASE + 4283 + 307 + 1);
			int iAnimalsDonated = (BASE + 4283 + 307 + 2);
			int iCentsDonated = (BASE + 4283 + 307 + 3);
			int iTrinkedDonated = (BASE + 4283 + 307 + 4); // nice spelling R*
		}sPearsonUnlocks;


		struct caravanPurchaseList // BASE + 4283 + 367
		{
			// NOTE: Size is 4. TODO: Fix these

			int eGlobalItemIdx = (BASE + 4283 + 367); // +0
			int iPurchaseTimer = (BASE + 4283 + 367 + 1);
			int iPurchaseCount = (BASE + 4283 + 367 + 2);
			int eShopPurchasedFrom = (BASE + 4283 + 367 + 3);
		}caravanPurchaseList;

		// DONE
		struct sCaravanChoreData // BASE + 4283 + 408
		{
			int iChoresCompletedBitMask = (BASE + 4283 + 408); // +0
			int todLastChoreCompleted = (BASE + 4283 + 408 + 1);
			float fMilkAmount = (BASE + 4283 + 408 + 2);
			float fEggAmount = (BASE + 4283 + 408 + 3);
			int iNumberOfChoresCompletedTotal = (BASE + 4283 + 408 + 4);
			int iTimesCowWasMilkedToday = (BASE + 4283 + 408 + 5);
			BOOL bStallsCleaned = (BASE + 4283 + 408 + 6);
		}sCaravanChoreData;

		// DONE
		struct sSupplyGroups // BASE + 4283 + 415
		{
			int iGroupItemsRemovedBitset = (BASE + 4283 + 415); // +0   Note: Size is 5
			int todCompanionNextRemoval = (BASE + 4283 + 415 + 5);
			int iUnlockHashes = (BASE + 4283 + 415 + 6); // Note: Size is 18
			int iUnlockHashCount = (BASE + 4283 + 415 + 24);
		}sSupplyGroups;

		//TODO
		struct sContData // BASE + 4283 + 440
		{
			// Note: Size is 53

			// struct sPg ?

			int iCarriedOverCents; // ???
			int sCont; // ???
			int iContributionCount; // ???
		}sContData;

		// DONE
		struct sCampHitchedHorseSavedData // BASE + 4283 + 567  (Another name R* uses is sCampHorseData)
		{
			BOOL bCampHorseHitched = (BASE + 4283 + 567); // +0
			int eCampIndex = (BASE + 4283 + 567 + 1);
			int vHitchedAtCoords = (BASE + 4283 + 567 + 2); // Vector3
			float fHitchedAtHeading = (BASE + 4283 + 567 + 5);
			int iHorseGender = (BASE + 4283 + 567 + 6);
		}sCampHorseData;


		// DONE
		struct sCampAmbientStreams // BASE + 4283 + 576
		{
			int iCounterStreamBeenPlayed = (BASE + 4283 + 576); // +0
			int todLastStreamPlayed = (BASE + 4283 + 576 + 1);
		}sCampAmbientStreams;

	}caravanCamp;

	//===========================================================//

	struct loansharkingData // BASE + 9052
	{
		int iLSActiveDebtors = (BASE + 9052); // +0
		int todLSVictimRespawn = (BASE + 9052 + 1); // Note: Size is 10
		int eLSSavedMissionFlags = (BASE + 9052 + 11); // Note: Size is 10
		int eLSSavedSystemFlags = (BASE + 9052 + 21); // Note: Size is 10
	}loansharkingData;

	//===========================================================//

	struct bountyhuntingData // BASE + 9074
	{
		int iBountiesCompleted = (BASE + 9074); // +0
		int eFlags = (BASE + 9074 + 1);
		int eRCMStage = (BASE + 9074 + 2);
		int iRewardAlive = (BASE + 9074 + 3);
		int iRewardDead = (BASE + 9074 + 4);
	}bountyhuntingData;

	//===========================================================//

	struct bankRobberyData // BASE + 9079
	{
		int eFlags = (BASE + 9079); // +0  Note: Size is 5
		int eStatus = (BASE + 9079 + 5); // Note: Size is 5
		int todLastRobTime = (BASE + 9079 + 10); // Note: Size is 5
		int iCompletedRobberies = (BASE + 9079 + 15);
		BOOL bIsBraveClientActive = (BASE + 9079 + 16);
	}bankRobberyData;

	//===========================================================//

	struct coachRobbery // BASE + 9146
	{
		int iCompletedRobberies = (BASE + 9146); // +0
		int eCurrentRobberyRho = (BASE + 9146 + 1);
		int eCurrentRobberyStr = (BASE + 9146 + 2);
		int eActiveRobbery = (BASE + 9146 + 3);
		int eStatus = (BASE + 9146 + 4); // Note: Size is 15
		int eCurrentVariant = (BASE + 9146 + 19); // Note: Size is 15
		int eCurrentDifficulty = (BASE + 9146 + 34); // Note: Size is 15
		int todCoachRobExp = (BASE + 9146 + 79); // Note: Size is 15
		int todCoachRobAct = (BASE + 9146 + 64); // Note: Size is 15
		int iTimesOffered = (BASE + 9146 + 49); // Note: Size is 15
		BOOL bNoteHasBeenRead = (BASE + 9146 + 94); // Note: Size is 15
		int iRobberiesSinceLastPoliceTrap = (BASE + 9146 + 111); // Note: Size is 15
		int eCoachBitflags = (BASE + 9146 + 126);
		int todCoachRobberyCompleted = (BASE + 9146 + 109);
		int todTipGivenByFreightWorker = (BASE + 9146 + 110);
	}coachRobbery;

	//===========================================================//

	// NOT DONE
	struct tutorialSaved // BASE + 7756
	{
		// TODO

		int sTimedData;
		int iTimeLastRun;
		int iTimesRun;
		int eTutorial;
	}tutorialSaved;

	//===========================================================//

	// NOT DONE
	struct lawStateData // BASE + 358
	{
		// TODO

		int iStateBounty;
		Vector3 vLastKnownLocation;
		float fLastKnownRadius;
		int eStateFlags;
		int eLastOuftitSeenByLaw;
		int eSideburnsStyle;
		int eChinStyle;
		int eMustacheStyle;
		int eTimeOutfitLastSeen;
		int eAppearanceLawMemoryFlags;

		struct stateLawMemoryData
		{
			// TODO		 Note: Size is 6

		}stateLawMemoryData;

	}lawStateData;

	//===========================================================//

	// NOT DONE
	struct lawDistrictData // BASE + 431
	{
		// TODO		 Note: Size is 1

		int eLawDistrictFlags;
	}lawDistrictData;

	//===========================================================//

	// NOT DONE
	struct caravanEventData // BASE + 7100
	{
		// TODO		 Note: Size is 8

		int iTimesCompleted;
		int iTimesSeen;
		int iTimesLaunched;
		int iTimesSpawned;
		int todNextAvailable;
		int iGeneric1;
		int iGeneric2;
		BOOL bUnlocked;
	}caravanEventData;

	//===========================================================//

	// NOT DONE
	struct pendingRCMStage // BASE + 450
	{
		// TODO		Note: Size is 40
	}pendingRCMStage;

	//===========================================================//

	// NOT DONE
	struct rcmDataSaved // BASE + 490
	{
		float iSavedFloat1; // Note: Size is 201
		float iSavedFloat2; // Note: Size is 201
		int eRCMSavedFlags; // Note: Size is 201
	}rcmDataSaved;

	//===========================================================//

	// NOT DONE
	struct playerOnlyGreetStack // BASE + 4237
	{
		// TODO		Note: Size is 3

		int eRuleset;
		int todExpiry;
		BOOL bTempDisable;
	}playerOnlyGreetStack;

	//===========================================================//

	// NOT DONE
	struct companionData // BASE + 4942
	{
		// TODO		Note: Size is 60

		int companionFlags;
		int iTrustLevel;
		int iFollowerLevel;
		int companionOutfit;
		int lastWornOutfitOnMission;
		int wornItems;
		int modelCompanion;
		int modelCompanionHorse;
		int immediateGreetOverride;
		int eRuleset;
		int todExpiry;
		BOOL bTempDisable;
		int eTemporaryMood;
		int ePersistentMood;
		int todTemporaryMoodExpiry;
		int todLastItemInteraction;
		int todLastChore;
		int todLastActivity;
		int todRoleCooldown;
		int eTimedEventType;
		int todTimedEvent;
		int txtSchedule;
		int eDeedReservedFor;

		struct greetOverrides
		{
			// Note: Size is 3

			int eRuleset;
			int todExpiry;
			BOOL bTempDisable;
		}greetOverrides;

	}companionData;

	//===========================================================//

	// NOT DONE
	struct companion_manager // BASE + 6563
	{
		int iItemRequestCompleteBitset = (BASE + 6563 + 271);
		int eReturnedJackBook = (BASE + 6563 + 272);
		int iNumAvailableActivities = (BASE + 6563 + 273);
		int todHungoverExpiry = (BASE + 6563 + 475);

		struct itemRequests
		{
			// Note: size is 27

			int eRequest;
			int eRequestItem;
			int iNumRequested;
			int eCompanion;
			int eHandInType;
			int eResultType;
			int txtConversation;
			int txtThankDialogue;
			int txtHandOverScenePath;
			int eVignette;
			int ePickupReward;
			int eModelReward;
			int iRewardSupplyGroupHash;
			int eState;
			BOOL bHasThankDialoguePlayed;
			BOOL bRewardHelpTextShown;
		}itemRequests;


		struct availableActivities // BASE + 6563 + 274
		{
			// Note: size is 20

			int eActivity;
			int eActivityInstance;
			int eHost;
			int eParticipants;
			int eConditionFlags;
			int vInvitePos;
			int vActivityPos;
			BOOL bEnabledInFlow;
			BOOL bHasBeenLaunched;
			BOOL bMustOffer;
			BOOL bOfferUntilHasRun;
			BOOL bForceLaunch;
			int eActivityDeed;
			int eTODAvailable;
			int iWeight;
			int iNumTimesOffered;
		}availableActivities;

	}companion_manager;

	//===========================================================//

	// NOT DONE
	struct activities // BASE + 7039
	{
		// TODO		Note: Size is 2

		BOOL bHasBeenPerformed;
		BOOL bHasBeenOffered;
	}activities;

	//===========================================================//

	// NOT DONE
	struct outfits // BASE + 7157
	{
		// TODO		Note: size is 3

		int status;
		int effect;
		int tags;
	}outfits;

	//===========================================================//

	// NOT DONE
	struct playerFacialHair // BASE + 7731
	{
		int eFacialHairFlags = (BASE + 7731 + 16);

		struct facialHair // BASE + 7731 + 0 ?
		{
			// Note: Size is 5

			int facialHairLength;
			int facialHairCut;
			int facialHairStyle;
			int facialHairNextGrowthTimer;
			int iNumAcceleratedGrowthCycles;
		}facialHair;

	}playerFacialHair;

	//===========================================================//

	struct playerHeadHair // BASE + 7748
	{
		int ePomadeWearOffTimer = (BASE + 7748); // +0
		int headHairLength = (BASE + 7748 + 1);
		int eHeadHairCut = (BASE + 7748 + 2);
		int eHeadHairStyle = (BASE + 7748 + 3);
		int eHeadHairFlags = (BASE + 7748 + 4);
		int headHairNextGrowthTimer = (BASE + 7748 + 5);
		int iNumAcceleratedGrowthCycles = (BASE + 7748 + 6);
	}playerHeadHair;

	//===========================================================//

	// NOT DONE
	struct mapBlipData // BASE + 7862
	{
		// TODO		Note: Size is 4

		int blipRegion;
		int blipType;
		Vector3 blipPackedPosition;
		int blipTODFlags;
	}mapBlipData;

	//===========================================================//

	struct discoverableData // BASE + 8863
	{
		int eFlags = (BASE + 8863); // +0	Note: Size is 144
		int DiscoEasel_timesViewed = (BASE + 8863 + 145);
		int todDiscoEasel_timeStamp = (BASE + 8863 + 144);
		int DiscoEasel_currentlyViewed = (BASE + 8863 + 147);
		int DiscoPhonograph_visits = (BASE + 8863 + 146);
		int DiscoDreamcatchers_bitField = (BASE + 8863 + 148);
		int DiscoCarriable_iBitField = (BASE + 8863 + 149);
		int DiscoCarriableMore_iBitField = (BASE + 8863 + 150);
		int DiscoCarriableArrows_iBitField = (BASE + 8863 + 151);
		int DiscoLocationVersion_iBitField = (BASE + 8863 + 152);
		int DiscoLocationMoreVersion_iBitField = (BASE + 8863 + 153);
		int DiscoLocationOtherVersion_iBitField = (BASE + 8863 + 154);
		int DiscoCorpse_iBitField = (BASE + 8863 + 155);
		int DiscoDisable = (BASE + 8863 + 156);
	}discoverableData;

	//===========================================================//

	struct showManagerData // BASE + 9028
	{
		int eCurrentShow = (BASE + 9028); // +0	Note: Size is 6
		int TODLastTimeViewed = (BASE + 9028 + 6); // +0	Note: Size is 6
		int iShowBits = (BASE + 9028 + 12);
		int iTotalShowViews = (BASE + 9028 + 13);
		int iTotalMCViews = (BASE + 9028 + 14);
		int iTotalMagicLanternViews = (BASE + 9028 + 15);
		int iTotalMovieViews = (BASE + 9028 + 16);
	}showManagerData;

	//===========================================================//

	struct townSecretData // BASE + 9020
	{
		int eFlags = (BASE + 9020); // +0	Note: Size is 5
		int TownSecret_VanHorn_bitfield = (BASE + 9020 + 5);
		int iTownSecret_Saint_Denis_bitfield = (BASE + 9020 + 6);
		int eStrawberryLastDayUFOSeen = (BASE + 9020 + 7);
	}townSecretData;

	//===========================================================//

	// NOT DONE
	struct eventArea // BASE + 9096
	{
		// TODO		Note: Size is 12

		int iStage;
		int iForceTransitionStage;
		int todNextStage;
		int todCounter;
		int eFlags;
		int eStateFlags;
		int iEventAreaCustomData;
		int iEventAreaCustomCounter;
		int iEventAreaMassacreCount;
		int iAggroStage;
		int todAggroed;
		int todMassacred;
	}eventArea;

	//===========================================================//

	struct duelingData // BASE + 9145
	{
		int eFlags = (BASE + 9145); // +0
	}duelingData;

	//===========================================================//

	struct herbs // BASE + 9274
	{
		int eHerbTypeFlags = (BASE + 9274); // +0	Note: Size is 45
	}herbs;

	//===========================================================//

	// NOT DONE
	struct huntingZoneData // BASE + 9319
	{
		// TODO

		struct sHuntingZoneTypes // BASE + 9319 + 0 ?
		{
			// Note: Size is 4

			BOOL bDiscovered;
			BOOL bCompleted;
			int eLaunchTime;
			BOOL bLegendaryHasBeenSkinned;
		}sHuntingZoneTypes;

	}huntingZoneData;

	//===========================================================//

	// NOT DONE
	struct propertyData // BASE + 9384
	{
		// TODO		Note: Size is 2

		int eFlags;
		int eTimeOfDay;
	}propertyData;

	//===========================================================//

	// NOT DONE
	struct homeRobberyData // BASE + 9422
	{
		// Note: Size is 7

		int todNextAvailable = (BASE + 9422); // +0
		int todLastVisited = (BASE + 9422 + 1);
		int eFlags = (BASE + 9422 + 4);
		int eFlagsPlus = (BASE + 9422 + 5);
		int eFlagsSet3 = (BASE + 9422 + 6);
		int iTimesRobbed = (BASE + 9422 + 2);
		int iPlayerCentsLostAtHome = (BASE + 9422 + 3);
	}homeRobberyData;

	//===========================================================//

	// NOT DONE
	struct newspapers // BASE + 9479
	{
		// TODO		Note: Size is 4

		int eStatus;
		int eDynamicStory1;
		int eDynamicStory2;
		int eFishingStory;
	}newspapers;

	//===========================================================//

	// NOT DONE
	struct dynamicArticles // BASE + 9536
	{

	}dynamicArticles;

	//===========================================================//

	struct itemData // BASE + 11943
	{
		int todSurvivalistActive = (BASE + 11943); // +0
		int todChewingGumActive = (BASE + 11943 + 1);
	}itemData;

	//===========================================================//

	// NOT DONE
	struct gangs // BASE + 9571
	{
		// TODO		Note: Size is 10

		int gangSavedFlags;
		int gangStatus;
		int iKillsSinceStatusChange;
		int todLastKill;
		int iGenericInt1;
		int iNumCampfiresCleared;
		int iNumCampfiresSpawned;
		int iNumCampfiresSeen;
		int iNumCampfiresPersisted;
		int iGangEncounterStage;
	}gangs;

	//===========================================================//

	// NOT DONE
	struct ambush // BASE + 9632
	{
		int eAmbushSavedFlags = (BASE + 9632 + 191);
		int todLastAmbushTime = (BASE + 9632 + 192);
		int eLastAmbushGang = (BASE + 9632 + 193);
		int eLastAmbush = (BASE + 9632 + 194);
		int iAmbushesCompleted = (BASE + 9632 + 195);
		int iNumRecognizeAmbushesStraight = (BASE + 9632 + 196);

		struct ambushTrack // BASE + 9632 + 0 ?
		{
			// Note: Size is 4

			int iCompletedCount;
			int iSeenCount;
			int iSpawnedCount;
			int iNextAllowedSpawnMinutes;
		}ambushTrack;


		struct gangAmbushesSeen
		{
			// Note: Size is 7
		}gangAmbushesSeen;


		struct gangAmbushesStraight
		{
			// Note: Size is 7
		}gangAmbushesStraight;

	}ambush;

	//===========================================================//

	struct campfires // BASE + 10991
	{
		int eFoothillsStoriesSpoken = (BASE + 10991); // +0
		int eMountainsStoriesSpoken = (BASE + 10991 + 1);
		int ePlainsStoriesSpoken = (BASE + 10991 + 2);
		int eSwampsStoriesSpoken = (BASE + 10991 + 3);
		int vCampfireUsedPositions = (BASE + 10991 + 4); // Note: Size is 31
		int iVignetteVariations = (BASE + 10991 + 35); // Note: Size is 2
		int eCampSavedFlags = (BASE + 10991 + 37);
	}campfires;

	//===========================================================//

	struct worldStates // BASE + 283
	{
		// bitfields. See EXAMPLES.md
	}worldStates;

	//===========================================================//

	struct worldStateTimers // BASE + 297
	{
		// bitfields. Note: Size is 61
	}worldStateTimers;

	//===========================================================//

	struct featureUnlocked // BASE + 7857
	{
		// bitfields. Note: Size is 3
	}featureUnlocked;

	//===========================================================//

	// NOT DONE
	struct jailData // BASE + 9829
	{
		struct sPrisoners // BASE + 9829 + 0 ?
		{
			// Note: Size is 4

			int eJailRegion;
			int ePrisoner;
			int ePrisonerFlag;
			int ePrisonerDialogue;
		}sPrisoners;
	}jailData;

	//===========================================================//

	struct trainData // BASE + 11029
	{
		// Note: Size is 5

		int trainFlags = (BASE + 11029); // +0
		int vTrainLoc = (BASE + 11029 + 1);
		BOOL bDirection = (BASE + 11029 + 4);
	}trainData;

	//===========================================================//

	struct playerRPGData // BASE + 11095
	{
		float fPlayerEfficiency = (BASE + 11095); // +0 -- TODO: THIS IS A STRUCT THAT CONTAINS fCorePoints

		int iHonor = (BASE + 11095 + 35);
		int iHighestHonorRank = (BASE + 11095 + 36);
		int iLowestHonorRank = (BASE + 11095 + 37);
		int iHighestBountyHonorReached = (BASE + 11095 + 38);
		float fDeadeyeAmount = (BASE + 11095 + 39);
		float fPlayerWeightLowerLimit = (BASE + 11095 + 40);
		float fPlayerWeightUpperLimit = (BASE + 11095 + 41);
		float fPlayerTankSickMod = (BASE + 11095 + 44);
		float fPlayerStamWeightMod = (BASE + 11095 + 45);
		int RPG_TIME_BEFORE_DEADEYECORE_EMPTY = (BASE + 11095 + 46);
		int RPG_TIME_BEFORE_STAMINACORE_EMPTY = (BASE + 11095 + 47);
		int RPG_TIME_BEFORE_HEALTHCORE_EMPTY = (BASE + 11095 + 48);
		int iOverfedTimer = (BASE + 11095 + 42);
		BOOL bPlayerPoisoned = (BASE + 11095 + 43);
		float fGritResist = (BASE + 11095 + 49);
		float fInstinctResist = (BASE + 11095 + 50);
		float fStrengthResist = (BASE + 11095 + 51);
		float fFatResist = (BASE + 11095 + 52);
		float fHeatResist = (BASE + 11095 + 53);
		float fColdResist = (BASE + 11095 + 54);
		float fBonusHealthXP = (BASE + 11095 + 55);
		float fBonusDeadeyeXP = (BASE + 11095 + 56);
		float fBonusStaminaXP = (BASE + 11095 + 57);
		int ePlayerSickness = (BASE + 11095 + 67);
		float fEagleEyeDepleteMod = (BASE + 11095 + 58);
		float fEagleEyeRangeBonus = (BASE + 11095 + 59);
		float fWeaponDegradeResist = (BASE + 11095 + 60);
		float fDamageScaleMelee = (BASE + 11095 + 61);
		float fDamageMod = (BASE + 11095 + 62);
		float fDamageTakenOnHorseModifier = (BASE + 11095 + 63);
		float fBowStaminaModifier = (BASE + 11095 + 64);
		int iGrappledKilledBears = (BASE + 11095 + 65);
		int iTimesEatenFood = (BASE + 11095 + 66);
		float fHorseBondingXPMod = (BASE + 11095 + 68);
		float fHealthRefillMod = (BASE + 11095 + 69);
		float fFortifyMod = (BASE + 11095 + 70);

		struct sAttributeOverpoweredTime // BASE + 11095 + 4
		{
			// Note: Size is 2

			float fTankTime;
			float fCoreTime;
		}sAttributeOverpoweredTime;


		struct fAttributePoints
		{
			// Note: Size is 24
		}fAttributePoints;

	}playerRPGData;

	//===========================================================//

	// NOT DONE
	struct moralChoices // BASE + 11166
	{
		// TODO		Note: Size is 1

		int eChoiceType;
	}moralChoices;

	//===========================================================//

	struct cacheOutfitData // BASE + 7443
	{
		// Note: Size is 286 (?)

		struct cachedOutfits
		{
			// Note: Size is 3

			int status;
			int effect;
			int tags;
		};
	}cacheOutfitData;

	//===========================================================//

	struct playerCampData // BASE + 9419
	{
		int eFlags = (BASE + 9419); // +0
		int iNumberOfCamps = (BASE + 9419 + 1);
	}playerCampData;

	//===========================================================//

	struct jailbreakData // BASE + 11182
	{
		int iViewedJailbreaks = (BASE + 11182); // +0
		int lastViewedJailbreakTime = (BASE + 11182 + 1);
	}jailbreakData;

	//===========================================================//

	struct sFishingDataSaved // BASE + 11184
	{
		Hash eCurrentBaitEquipped = (BASE + 11184); // +0	Note: Size is 5
		BOOL bHasLegendaryFishBeenCaught = (BASE + 11184 + 5); // Note: Size is 16
		int iFishingDataSavedBits = (BASE + 11184 + 21);
	}sFishingDataSaved;

	//===========================================================//

	// NOT DONE
	struct managedTowns // BASE + 11206
	{
		// eActiveTownStateTags - possible struct (26) that contains the following:

		int eTag;
		int eTagPriority;
		int eTagPersistence;
		int eTODExpiry;
	}managedTowns;

	//===========================================================//

	// NOT DONE
	struct specialPeds // BASE + 11623
	{
		// TODO		Note: Size is 8

		int ePedStage;
		int eSavedFlags;
		int todLastSeen;
		int todLastEncounter;
		int todDeathTime;
		int iTimesSeen;
		int iInteracts;
		int iDeaths;
	}specialPeds;

	//===========================================================//

	struct dataShacksSaved // BASE + 11959
	{
		int iSavedFloat1 = (BASE + 11959); // +0	Note: Size is 20
		int iSavedFloat2 = (BASE + 11959 + 20); // Note: Size is 20
	}dataShacksSaved;

	//===========================================================//

	// NOT DONE
	struct hideouts // BASE + 11864
	{
		// TODO		Note: Size is 2

		int eFlags;
		int iNumActorsRemaining;
	}hideouts;

	//===========================================================//

	struct sFilletDataSaved // BASE + 11883
	{
		int iBestLaps = (BASE + 11883); // +0	Note: Size is 13
		int iFastestLap = (BASE + 11883 + 13); // Note: Size is 13
		int iLongestTime = (BASE + 11883 + 26); // Note: Size is 13
	}sFilletDataSaved;

	//===========================================================//

	struct eBufferedJournalEntryUnlocks // BASE + 11922
	{
		// Note: Size is 21
	}eBufferedJournalEntryUnlocks;

	//===========================================================//

	struct collectiblesData // BASE + 9045
	{
		BOOL bDinoBonesStage2RCM = (BASE + 9045); // +0
		BOOL bLegendaryFishStage2RCM = (BASE + 9045 + 1);
		BOOL bRockCarvingsStage2RCM = (BASE + 9045 + 2);
		BOOL bTaxidermyStage1RCM = (BASE + 9045 + 3);
		BOOL bCarolinaParakeetExtinction = (BASE + 9045 + 4);
		BOOL bChalTownHoldups = (BASE + 9045 + 5);
		BOOL bTSAnnesburgRiddle = (BASE + 9045 + 6);
	}collectiblesData;

	//===========================================================//

	struct sCheatManagerSaved // BASE + 12000
	{
		int iUnlockedCheats = (BASE + 12000); // +0	Note: Size is 3
	}sCheatManagerSaved;

	//===========================================================//

	struct reinforcedequipmentSaved // BASE + 12003
	{
		int eEquipmentRewardFlags = (BASE + 12003); // +0
	}reinforcedequipmentSaved;

	//===========================================================//

	struct collectableUiLogSaved // BASE + 12004
	{
		int eUiCigCardsSavedFlags = (BASE + 12004); // +0
		int eUiDinoBonesSavedFlags = (BASE + 12004 + 1);
		int eUiExoticsSavedFlags = (BASE + 12004 + 2);
		int eUiLegendaryFishSavedFlags = (BASE + 12004 + 3);
		int eUiParakeetsSavedFlags = (BASE + 12004 + 4);
		int eUiRockCarvingsSavedFlags = (BASE + 12004 + 5);
		int eUiTaxidermySavedFlags = (BASE + 12004 + 6);
		int eUiTHJackHallSavedFlags = (BASE + 12004 + 7);
		int eUiTHHighStakesSavedFlags = (BASE + 12004 + 8);
		int eUiTHPoisonTrailSavedFlags = (BASE + 12004 + 9);
		int eUiTHTresorMortsSavedFlags = (BASE + 12004 + 10);
		int eUiTHElementalTrailSavedFlags = (BASE + 12004 + 11);
		int eUiTHLandmarksOfRichesSavedFlags = (BASE + 12004 + 12);
	}collectableUiLogSaved;

	//===========================================================//

	// NOT DONE
	struct shared // BASE + 12019
	{
		// TODO

		// cards
		// rockCarvings
		// legendaryFish
		// gatorEggs
		// egretFeathers
		// taxidermyAnimals
		// taxidermyMarital
		// journalData
		// exotics
		// firstRecipePamphlets
		// treasureHunter
	};

	//===========================================================//


	// TODO: Other structs
	
	
	static const int BASE = 40;
}g_savedGlobals;

