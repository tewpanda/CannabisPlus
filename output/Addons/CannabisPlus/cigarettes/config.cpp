class CfgPatches
{
	class CannabisPlus_Gear_Cultivation_Cigarettes
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Gear_Medical"
		};
	};
};

class CfgVehicles
{
    class Inventory_Base;
    class CigarettePack_ColorBase: Inventory_Base
    {
        scope=2;
        descriptionShort="A pack of cigarettes.";
        lootCategory="Crafted";
    };
    
    class CigarettePack_Chernamorka: CigarettePack_ColorBase
	{
		scope=2;
		displayName="Chernamorka Cigarettes";
		hiddenSelectionsTextures[]=
		{
			"DZ\gear\medical\Data\pack_of_cigs_cherno_co.paa",
			"DZ\gear\medical\Data\pack_of_cigs_cherno_co.paa"
		};
	};

	class CigarettePack_Merkur: CigarettePack_ColorBase
	{
		scope=2;
		displayName="Merkur Cigarettes";
		hiddenSelectionsTextures[]=
		{
			"DZ\gear\medical\Data\pack_of_cigs_merkur_co.paa",
			"DZ\gear\medical\Data\pack_of_cigs_merkur_co.paa"
		};
	};

	class CigarettePack_Partyzanka: CigarettePack_ColorBase
	{
		scope=2;
		displayName="Partyzanka Cigarettes";
		hiddenSelectionsTextures[]=
		{
			"DZ\gear\medical\Data\pack_of_cigs_partyzanka_co.paa",
			"DZ\gear\medical\Data\pack_of_cigs_partyzanka_co.paa"
		};
	};

	class CigarettePack_Cannabis: CigarettePack_ColorBase
	{
		displayName="Marijuana Cigarettes";
		descriptionShort="A pack of marijuana cigarettes.";
		lootCategory="Crafted";
		hiddenSelectionsTextures[]=
		{
			"CannabisPlus\cigarettes\data\pack_of_cigs_cannabis.paa",
			"CannabisPlus\cigarettes\data\pack_of_cigs_cannabis.paa",
			"CannabisPlus\cigarettes\data\pack_of_cigs_cannabis.paa"
		};
	};

	class CigarettePack_CannabisBlue: CigarettePack_ColorBase
	{
		displayName="Marijuana Cigarettes Blue God";
		descriptionShort="A pack of marijuana cigarettes.";
		lootCategory="Crafted";
		hiddenSelectionsTextures[]=
		{
			"CannabisPlus\cigarettes\data\pack_of_cigs_cannabis.paa",
			"CannabisPlus\cigarettes\data\pack_of_cigs_cannabis.paa",
			"CannabisPlus\cigarettes\data\pack_of_cigs_cannabis.paa"
		};
	};

	class CigarettePack_CannabisKush: CigarettePack_ColorBase
	{
		displayName="Marijuana Cigarettes Purple Kush";
		descriptionShort="A pack of marijuana cigarettes.";
		lootCategory="Crafted";
		hiddenSelectionsTextures[]=
		{
			"CannabisPlus\cigarettes\data\pack_of_cigs_cannabis.paa",
			"CannabisPlus\cigarettes\data\pack_of_cigs_cannabis.paa",
			"CannabisPlus\cigarettes\data\pack_of_cigs_cannabis.paa"
		};
	};
}