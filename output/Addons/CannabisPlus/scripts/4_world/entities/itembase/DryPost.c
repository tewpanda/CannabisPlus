class CP_DryPost extends Container_Base
{	
	bool m_IsLocked = false;
	ref Timer m_PlantDryTime;
	bool RopeAttached = false;
	bool Plant1Attached = false;
	bool Plant2Attached = false;
	bool Plant3Attached = false;
	int NumPlants;
	int NumItems;
	string ItemName
	string NewPlantName;
	ref map<string, int> BudSpawn;
	ItemBase attachment;
	int i, j, k
	EntityAI target
	
	void CP_DryPost()
	{
		BudSpawn = new map<string, int>;
		BudSpawn.Clear();
	}
	
	void ~CP_DryPost()
	{
		
	}
	
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);
	        
		if (slot_name == "Rope")
		{    				
			SetAnimationPhase ("Rope", 0);  // Shows the rope on the model when rope is attached.+
			RopeAttached = true;
		} else if (slot_name == "HangingPlants") 
		{
			Plant1Attached = true;
		} else if (slot_name == "HangingPlants2") 
		{
			Plant2Attached = true;
		} else if (slot_name == "HangingPlants3") 
		{
			Plant3Attached = true;
		}
		if (RopeAttached && Plant1Attached && Plant2Attached && Plant3Attached ) 
		{	
			CheckStart();
		}		
	}
    
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		
		if (slot_name == "Rope")
		{    				
			SetAnimationPhase ("Rope", 1);  // Shows the rope on the model when rope is attached.+
			RopeAttached = false;
		} else if (slot_name == "HangingPlants") 
		{
			Plant1Attached = false;
		} else if (slot_name == "HangingPlants2") 
		{
			Plant2Attached = false;
		} else if (slot_name == "HangingPlants3") 
		{
			Plant3Attached = false;
		}	
	}
	
	bool IsItemTypeAttached( typename item_type )
	{
		if ( GetAttachmentByType( item_type ) )
		{
			return true;
		}
		return false;
    	}
	
	CP_RawSkunkCannabisPlant  GetCannibisBase()
    	{
      	return CP_RawSkunkCannabisPlant.Cast( GetAttachmentByType (CP_RawSkunkCannabisPlant) );
    	};
	
	void CheckStart()
	{
		if (!m_IsLocked)	
		{
			for ( k = 0; k < GetInventory().AttachmentCount(); k++ )
			{
				attachment = ItemBase.Cast( GetInventory().GetAttachmentFromIndex( k ) );
				ItemName  = attachment.GetType();
				if (ItemName.IndexOf("CP_Raw") >= 0)
				{
					NumPlants += 1;
				}	
			}	
			
			if (NumPlants==3)
			{
				Print("[CP] all items attached to post...starting to dry");
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(FinishDrying, GetCPConfig().cannabis_drytime*1000, false);
				m_IsLocked = true;
			}
		}		
	}
	
	override bool CanReleaseAttachment(EntityAI attachment)
	{
		if ( m_IsLocked )
		{
			return false;
		}
		return super.CanReleaseAttachment(attachment);
	}

	override bool CanPutIntoHands(EntityAI parent)
    	{
        	if ( m_IsLocked || RopeAttached || Plant1Attached || Plant2Attached || Plant3Attached)
        	{
            	return false;
        	}
        	return super.CanPutIntoHands(parent);
    	}
	
	void AddToMap(string item, int value)
	{
		private int CurrentValue = 0;
		private int NewValue = 0;
		
		if (value < 2)
		{
			NewValue = 2;
		} else
		{
			NewValue = value;
		}	
		
 		if (BudSpawn.Find(item,CurrentValue))
		{
			//get current value, remove and re-add with updated value
			NewValue += CurrentValue;
			BudSpawn.Remove(item);
			BudSpawn.Insert(item,NewValue);
				
		} else {		
			BudSpawn.Insert(item,NewValue);
		}	
	}

	void FinishDrying()
	{
		Print("[CP] finished drying...");
		NumItems = GetInventory().AttachmentCount();
		
		for ( j = 0; j < NumItems; j++ )
		{
			attachment = ItemBase.Cast( GetInventory().GetAttachmentFromIndex( j ) );
			ItemName  = attachment.GetType();
			//code to cast and get the amount of bud to spawn before deleting
			//plant                        Bud 
			//CP_RawSkunkCannabisPlant     CP_CannabisSkunk
			if (ItemName.IndexOf("CP_Raw") >= 0)
				switch(ItemName){
				      // cannabis skunk
					case "CP_RawSkunkCannabisPlant":
						CP_RawSkunkCannabisPlant skunkplant = CP_RawSkunkCannabisPlant.Cast(attachment);
						if (skunkplant)
						{
							AddToMap("CP_CannabisSkunk",	skunkplant.GetYield());
						}
						break;
					// cannabis blue
					case "CP_RawBlueCannabisPlant":
						CP_RawBlueCannabisPlant blueplant = CP_RawBlueCannabisPlant.Cast(attachment);
						if (blueplant)
						{
							AddToMap("CP_CannabisBlue",	blueplant.GetYield());
						}	
						break;
					// cannabis kush
					case "CP_RawKushCannabisPlant":
						CP_RawKushCannabisPlant kushplant = CP_RawKushCannabisPlant.Cast(attachment);
						if (kushplant)
						{
							AddToMap("CP_CannabisKush",	kushplant.GetYield());
						}
						break;
					// cannabis Stardawg
					case "CP_RawStardawgCannabisPlant":
						CP_RawStardawgCannabisPlant stardawgplant = CP_RawStardawgCannabisPlant.Cast(attachment);
						if (stardawgplant)
						{ 
							AddToMap("CP_CannabisStardawg",	stardawgplant.GetYield());
						}
						break;
					// cannabis Future
					case "CP_RawFutureCannabisPlant":
						CP_RawFutureCannabisPlant futureplant = CP_RawFutureCannabisPlant.Cast(attachment);
						if (futureplant)
						{ 
							AddToMap("CP_CannabisFuture",	futureplant.GetYield());
						}
						break;
					// cannabis S1
					case "CP_RawS1CannabisPlant":
						CP_RawS1CannabisPlant s1plant = CP_RawS1CannabisPlant.Cast(attachment);
						if (s1plant)
						{ 
							AddToMap("CP_CannabisS1",	s1plant.GetYield());
						}
						break;
					// cannabis Nomad
					case "CP_RawNomadCannabisPlant":
						CP_RawNomadCannabisPlant nomadplant = CP_RawNomadCannabisPlant.Cast(attachment);
						if (nomadplant)
						{ 
							AddToMap("CP_CannabisNomad",	nomadplant.GetYield());
						}	
						break;
					// cannabis BlackFrost
					case "CP_RawBlackFrostCannabisPlant":
						CP_RawBlackFrostCannabisPlant bfplant = CP_RawBlackFrostCannabisPlant.Cast(attachment);
						if (bfplant)
						{
							AddToMap("CP_CannabisBlackFrost",	bfplant.GetYield());
						}
						break;
					default:
						break;
			}			
		}			
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SpawnDried, 500, false);
	}
	
	void SpawnDried() 
	{
		if ( GetGame() && GetGame().IsServer() )
		{
			NumItems = GetInventory().AttachmentCount();
			
			for ( i = 0; i < NumItems; i++ )
			{
		            attachment = ItemBase.Cast( GetInventory().GetAttachmentFromIndex( i ) );
		            ItemName = attachment.GetType();
				if (ItemName.IndexOf("CP_Raw") >= 0)
		            {
		            	GetInventory().CreateInInventory("CP_DriedCannabisPlant");
					Print("[CP] spawning CP_DriedCannabisPlant");
		            }    
			}
	
			Print("[CP] The plant has " + BudSpawn.Count() + " items");
			
			for (j = 0; j < BudSpawn.Count(); j++)
			{
				string key = BudSpawn.GetKey(j);
				Print("[CP] plant[" + j + "] is " + key + " with quantity " + BudSpawn.Get(key));
				for (k = 0; k < BudSpawn.Get(key); k++)
				{
					Print("[CP] spawning "+ BudSpawn.GetKey(j));
					GetInventory().CreateInInventory(BudSpawn.GetKey(j));
				}
			}  	
		}	
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CleanUp, 500, false);
    }

    void CleanUp()
	{	
		if ( GetGame() && GetGame().IsServer() )
		{
			NumItems = GetInventory().AttachmentCount();
			for ( i = 0; i < NumItems; ++i )
			{
				attachment = ItemBase.Cast( GetInventory().GetAttachmentFromIndex( i ) );
				ItemName = attachment.GetType();
	            	if (ItemName.IndexOf("CP_Raw") >= 0)
		            {
					Print("[CP] deleting " + attachment);
					GetGame().ObjectDelete(attachment);	
				}	
	        	}
	
			NumPlants = 0;
			BudSpawn.Clear();
		}
		m_IsLocked = false;	
		syncronize();	
	}
	
	void syncronize()
	{
		if ( GetGame() && GetGame().IsServer() )
		{
			SetSynchDirty();
		}
	}

	override void OnPlacementStarted( Man player )
    	{
        	super.OnPlacementStarted( player );
        
            SetAnimationPhase ("Rope", 0);  // Shows the rope on the model when rope is attached.
    	}
	
	override string GetPlaceSoundset()
	{
		return "woodenlog_drop_SoundSet";
	}
    
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionPlaceObject);
	}
}
