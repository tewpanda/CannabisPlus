class CfgPatches
{
	class CannabisPlus_Joint
	{
		units[]={};
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
	class Inventory_Base;
	class CP_JointBase : Inventory_Base
	{
		scope=2;
		displayName="Skunk Joint";
		descriptionShort="A joint made out of finest Skunk weed";
		model="\CannabisPlus\joints\joint.p3d";
		inventorySlot="Ingredient";
		rotationFlags=16;
		weight=30;
		itemSize[]={1,2};
	};

	class CP_JointBlue: CP_JointBase
	{
		displayName="Blue God Joint";
		descriptionShort="A joint made out of finest Blue God weed";
	};

	class CP_JointKush: CP_JointBase
	{
		displayName="Purple Kush Joint";
		descriptionShort="A joint made out of finest Purple Kush weed";
	};

	class CP_JointStardawg: CP_JointBase
	{
		displayName="Stardawg Joint";
		descriptionShort="A joint made out of finest Stardawg weed";
	};
};
