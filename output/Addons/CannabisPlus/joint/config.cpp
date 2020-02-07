class CfgPatches
{
	class CannabisPlus_Joint
	{
		units[]=
		{
			"CP_Joint"
		};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_gear_tools"
		};
	};
};
class CfgVehicles
{
	class Edible_Base;
	class CP_Joint: Edible_Base
	{
		scope=2;
		displayName="Joint";
		descriptionShort="A joint for smoking weed...";
		model="\CannabisPlus\joint\joint.p3d";
		repairableWithKits[]={5,8};
		repairCosts[]={30,25};
		inventorySlot="Ingredient";
		lootCategory="Crafted";
		SingleUseActions[]={509,510};
		InteractActions[]={1002,1003};
		rotationFlags=16;
		weight=30;
		itemSize[]={1,2};
		absorbency=0;
		varQuantityInit=500;
		varQuantityMin=0;
		varQuantityMax=500;
		//varLiquidTypeInit=0;
		//liquidContainerType="1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512 + 1024 + 2048 + 4096 + 8192 + 16384 + 32768 + 65536  - (1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256)";
		heatIsolation=0.13;
		quantityBar=1;
		visibilityModifier=0.69999999;
		hiddenSelections[]=
		{
			"zbytek"
		};
		hiddenSelectionsTextures[]=
		{
			"CannabisPlus\joint\data\tobacco_co.paa"
		};
		/*
		class Nutrition {
			
			totalVolume=1;
			fullnessIndex=2.5;
			energy=1;
			water=-10;
			nutritionalIndex=1;
			toxicity=0;
		};
		class EnergyManager {

            switchOnAtSpawn=0;
            isPassiveDevice=1;
            energyStorageMax=60;
            energyUsagePerSecond=1;
            energyAtSpawn=60;
            powerSocketsCount=1;
            compatiblePlugTypes[]={7};
            convertEnergyToQuantity=1;
        };
		*/
	};
};