class CfgPatches
{
    class CP_Workbench
    {
        units[] =
        {
            ""
        };
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = 
        {
            "DZ_Data",
			"DZ_Gear_Medical",
			"DZ_Gear_Consumables"
        };
    };
};

class CfgSlots
{ 
      class Slot_Wrapper
      {
            name="CP_PlasticWrapper";
            displayName="Wrapper";
            ghostIcon="missing";
      };
      class Slot_Bagger
      {
            name="CP_Bagger";
            displayName="Bagger";
            ghostIcon="missing";
      };	  
      class Slot_Tape
      {
            name="DuctTapes";
            displayName="Klebeband";
            ghostIcon="missing";
            stackMax=5;
      };
      class Slot_Bags
      {
            name="CP_CannabisBags";
            displayName="Bags";
            ghostIcon="missing";
            stackMax=25;
      };
      class Slot_Bricks
      {
            name="Bricks";
            displayName="Bricks";
            ghostIcon="missing";
            stackMax=5;
      };	  
};   

class CfgVehicles
{
	class Inventory_Base;
	class WoodenCrate;
	
	class DuctTape: Inventory_Base 
	{
		inventorySlot[] = {"DuctTapes"};
	};
	
	class CP_Workbench_Kit: WoodenCrate
	{
		scope = 2;
		displayName = "Workbench Kit";
		descriptionShort = "A workbench kit for packaging of illicit drugs.";
		model="\CannabisPlus\boxes\box_medium.p3d";
		itemSize[] = {5,3};
		carveNavmesh = 1;
		canBeDigged = 0;
		simulation = "inventoryItem";
		physLayer = "item_small";
		rotationFlags = 2;
		InteractActions[] = {};
		weight = 300;
		itemBehaviour = 2;
		class Cargo{};
            hiddenSelections[]=
		{
			"zbytek"
		};
		hiddenSelectionsTextures[]=
		{
			"CannabisPlus\boxes\data\Caja_Color.paa"
		};
      };
	  
      class CP_Workbench : Inventory_Base
      {
		scope = 2;
		displayName = "Workbench";
		descriptionShort = "A workbench for packaging of illicit drugs.";
		model="\CannabisPlus\workbench\workbench.p3d";
		weight=50000;
		itemSize[]={6,6};
		varQuantityDestroyOnMin=1;
		itemBehaviour=0;
		canBeSplit=1;
		varQuantityInit=1;
		varQuantityMin=0;
		varQuantityMax=1;
		varStackMax=1;		
		physLayer="item_heavy";
		itemsCargoSize[] = {10,4};
		hiddenSelections[]=
		{
			"zbytek"
		};
		hiddenSelectionsTextures[]=
		{
			"CannabisPlus\workbench\data\tisch1.paa"
		};		
		attachments[]=
		{
			  "CP_PlasticWrapper",
			  "CP_Bagger",
			  "DuctTapes",
			  "CarBattery",			  
			  "CP_CannabisBags",
			  "Bricks"
		};
		
		class GUIInventoryAttachmentsProps
		{
			class Inputs
			{
				name = "Inputs";
				description = "";
				attachmentSlots[] = {"CP_PlasticWrapper","CP_Bagger","DuctTapes", "CarBattery"};
				//icon="knife";
			};
			class Bags
			{
				name = "Bag Output";
				description = "";
				attachmentSlots[] = {"CP_CannabisBags"};
				//icon="knife";
			};
			class Bricks
			{
				name = "Brick Output";
				description = "";
				attachmentSlots[] = {"Bricks"};
				//icon="knife";
			};
		};
	};
	
    class CP_Workbench_Holo: CP_Workbench
	{
		scope = 0;
	};	
};