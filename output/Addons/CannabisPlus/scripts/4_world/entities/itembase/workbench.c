//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 'workbench_kit' represents the item from which the finished workbench can be made.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class CP_Workbench_Kit extends ItemBase 
{
	override void EEInit()
	{
		super.EEInit();
		
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Call( AssembleKit );
	}

	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		if ( !super.CanReceiveAttachment(attachment, slotId) )
			return false;
		
		ItemBase att = ItemBase.Cast(GetInventory().FindAttachment(slotId));
		if (att)
			return false;
		
		return true;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Event handler that is fired if placement of workbenchkit is complete.
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
	override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" ) 
	{		
		super.OnPlacementComplete( player );		
		
		if ( GetGame().IsServer() )
		{
			PlayerBase player_base = PlayerBase.Cast( player );

			CP_Workbench CPWorkbench = CP_Workbench.Cast( GetGame().CreateObjectEx( "CP_Workbench", GetPosition(), ECE_PLACE_ON_SURFACE ) );
			
			CPWorkbench.SetPosition( position);
			CPWorkbench.SetOrientation( orientation );
			
			//make the kit invisible, so it can be destroyed from deploy UA when action ends
			HideAllSelections();
			
			this.Delete();
			SetIsDeploySound( true );
		}	
	}
	
	override void OnPlacementStarted( Man player )
    	{
        	super.OnPlacementStarted( player );
        
		SetAnimationPhase ("Rope", 0);  // Shows the rope on the model when rope is attached.
    	}

	override string GetPlaceSoundset()
	{
		return "seachest_drop_SoundSet";
	}
	
	override bool IsDeployable()
	{
		return true;
	}

	override bool DoPlacingHeightCheck()
	{
		return false;
	}
	override float HeightCheckOverride()
	{
		return 20;
	}
	void AssembleKit()
	{
		if (!IsHologram())
		{
			Rope rope = Rope.Cast(GetInventory().CreateAttachment("Rope"));
		}
	}
	void CreateRope(Rope rope)
	{
		if (!rope)
			return;
		
		InventoryLocation targetLoc = rope.GetTargetLocation();
		if (targetLoc && targetLoc.GetType() != InventoryLocationType.GROUND)
		{
			MiscGameplayFunctions.TransferItemProperties(this, rope);
			return;
		}
		
		EntityAI newRope = EntityAI.Cast(GetGame().CreateObjectEx(rope.GetType(), GetPosition(), ECE_PLACE_ON_SURFACE));
		
		if (newRope)
			MiscGameplayFunctions.TransferItemProperties(this, newRope);
		
		rope.Delete();
	}
	void DisassembleKit(ItemBase item)
	{
		if (!IsHologram())
		{
			ItemBase Log = ItemBase.Cast(GetGame().CreateObjectEx("WoodenLog",GetPosition(),ECE_PLACE_ON_SURFACE));
			MiscGameplayFunctions.TransferItemProperties(this, Log);
			Rope rope = Rope.Cast(item);
			CreateRope(rope);
		}
	}
	override void EEItemDetached(EntityAI item, string slot_name)
    {
		super.EEItemDetached( item, slot_name );
		
		PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
		if ( player && player.IsPlayerDisconnected() )
			return;
		
		if (item && slot_name == "Rope")
		{
			if (GetGame().IsServer())
			{
				DisassembleKit(ItemBase.Cast(item));
				Delete();
			}
		}
	}
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionDeployObject);
	}
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// The workbench class provides all methods for processing and production of the goods.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class CP_Workbench extends ItemBase 
{
	
	const string ATTACHMENT_SLOT_WRAPPER	 	    = "Wrapper";
	const string ATTACHMENT_SLOT_BAGGER	 	        = "Bagger";
	const string ATTACHMENT_SLOT_BATTERIES			= "LargeBattery";
	const string ATTACHMENT_SLOT_PLASTICWRAP		= "CP_PlasticRoll";
	const string ATTACHMENT_SLOT_EMPTYBAGS          = "CP_Empty_Bags";
	const string ATTACHMENT_SLOT_BUDS 				= "CP_Cannabus_Buds";
	const string ATTACHMENT_SLOT_BAGS				= "CP_Cannabus_Bags";
	const string ATTACHMENT_SLOT_BRICKS 			= "CP_Cannabus_Bricks";
	
	/*///////////////////////////////////////////////////////////////////
	////					Start of Config Control                 /////
	////           All Config Main Variables ill be put Below       /////
	///////////////////////////////////////////////////////////////////*/
	
	/*                   Whole # Config Options                        */
	/////////////////////////////////////////////////////////////////////
		int BatteryRequired = GetCPConfig().RequireBattery;
		int Workbench_Timer_Repeat = GetCPConfig().Workbench_Processing_Time;
		int BudsToBagsUsage = GetCPConfig().Buds_To_Bags_Required;
		int BagsToBricksUsage = GetCPConfig().Bags_To_Bricks_Required;	




	
	
	
	/*				Percentile Config Options 0% - 100%                */
	/////////////////////////////////////////////////////////////////////

	
	    float Battery_Percent = GetCPConfig().WorkBench_PowerUsed / 100 * 1500;
	
		float PlaticWrap_Percent = GetCPConfig().Plastic_Wrap_Usage;


	
	/*///////////////////////////////////////////////////////////////////
	////					End of Config Control                 ///////
	////           All Config Main Variables ill be put Above     ///////
	///////////////////////////////////////////////////////////////////*/
	
	// timer to get bagger working
	protected ref Timer m_CP_Processing;

	protected bool CP_TimerisRunning
	protected bool CP_TimerIsPaused
	
	int m_UseCPWorkbench = 0;
	//Defines a int used for action switching
			
	PlayerBase 		m_Player;
	//defines playerbase to call player
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// constructor of workbench class
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void CP_Workbench() 
	{
		RegisterNetSyncVariableBool("m_IsPlaceSound");
		//RegisterNetSyncVariableBool("m_UseCPWorkbench")
		RegisterNetSyncVariableBool("CP_TimerisRunning");
		RegisterNetSyncVariableBool("CP_TimerIsPaused");
		m_CP_Processing.Stop();
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Entity entry Intilize
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override void EEInit()
	{
      	super.EEInit();
    };


	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Start of Step based Variables to change outcome and Text shown for actions. not complicated but very handy
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	

	
		
	CP_CannabisBud GetCannibusBud()
	{
		return CP_CannabisBud.Cast( FindAttachmentBySlotName( ATTACHMENT_SLOT_BUDS ) );
	};

	CP_CannabisBags GetCannibusBags()
	{
		return CP_CannabisBags.Cast(FindAttachmentBySlotName( ATTACHMENT_SLOT_BAGS ) );
	};
	
	CP_EmptyBag GetEmptyBags()
	{
		return CP_EmptyBag.Cast(FindAttachmentBySlotName( ATTACHMENT_SLOT_EMPTYBAGS ) );
	};
	CP_PlasticRoll GetPlasticRoll()
	{
		return CP_PlasticRoll.Cast(FindAttachmentBySlotName(ATTACHMENT_SLOT_PLASTICWRAP) );
	};
	CP_CannabisBrickBase GetCannibusBricks()
	{
		return CP_CannabisBrickBase.Cast(FindAttachmentBySlotName( ATTACHMENT_SLOT_BRICKS ) );
	};
	VehicleBattery GetBattieries()
	{
		return VehicleBattery.Cast(FindAttachmentBySlotName( ATTACHMENT_SLOT_BATTERIES ) );
	};
	CP_Bagger GetBagger()
	{
		return CP_Bagger.Cast(FindAttachmentBySlotName( ATTACHMENT_SLOT_BAGGER ) );
	};
	CP_PlasticWrapper GetWrapper()
	{
		return CP_PlasticWrapper.Cast(FindAttachmentBySlotName( ATTACHMENT_SLOT_WRAPPER ) );
	};	
	
	string GetPauseOrResumeText()
	{
		if(CP_TimerIsPaused == false)
		{		
			return "Pause Production";
		}
		else if(CP_TimerIsPaused == true)
			return "Resume Production";
			
		return "";
		
	}
	
	string GetBagTendancyText()
	{
		if(!GetCannibusBud())
			return "";
		
		string BagName = GetCannibusBud().GetDisplayName();

		
        return "Fill bag with " + BagName;

    };
	
	string GetBrickTendancyText()
	{
		if(!GetCannibusBags())
			return "";
		
		string Brickname = GetCannibusBags().GetDisplayName();

		
        return "Wrap " + Brickname;

    };

	void start_processing()
	{		
		if(m_CP_Processing && m_CP_Processing.IsRunning())
			return;
		
		if(!m_CP_Processing)
		{
		   	m_CP_Processing = new Timer;
			m_CP_Processing.Run(Workbench_Timer_Repeat,this,"Do_processing",NULL,true);
			CP_TimerisRunning = true;
			CP_TimerIsPaused = false;
			LockCPWorkbenchSlots(true);

		}

		//LockCPWorkbenchSlots(false);
		
		SetSynchDirty();	
	}

		
	void PauseOrResume()
	{
		if(CP_TimerIsPaused == false)
		{
			m_CP_Processing.Pause();
			CP_TimerIsPaused = true;
			LockCPWorkbenchSlots(false);
		}
		else if(CP_TimerIsPaused == true)
		{
			m_CP_Processing.Continue();
			CP_TimerIsPaused = false;
			LockCPWorkbenchSlots(true);
		}
		SetSynchDirty();	
	}

	
	
	bool RunningOrNot()
	{
		if(CP_TimerisRunning == true)
		{

		  return true;
		}
		else
		return false;	

	}
	bool Pausedornot()
	{
		if(CP_TimerIsPaused == true)
		{

		  return true;
		}
		else
		return false;	
	}
	bool EnabledStop()
	{
		if(CP_TimerisRunning == true)
		{
			return true;
		}
		return false;
	}	
	void StopProduction()
	{
		m_CP_Processing.Stop();
		CP_TimerisRunning = false;
		CP_TimerIsPaused = false;
		LockCPWorkbenchSlots(false);
		SetSynchDirty();	
	}	

	void Do_processing()
	{

		if(BatteryRequired == 1)
		{
		
			if(GetBattieries().GetCompEM().GetEnergy() > Battery_Percent)
			{
				Print("Battery Check");
				if(GetCannibusBud() && GetCannibusBud().GetQuantity() >= BudsToBagsUsage && GetCannibusBags().GetQuantity() < 160 && GetEmptyBags() && GetEmptyBags().GetQuantity() > 0)
				{
					CP_TimerisRunning = true;
					LockCPWorkbenchSlots(true);					
					CreateBags();
				}
				else if(GetCannibusBags() && GetCannibusBags().GetQuantity() >= BagsToBricksUsage && GetCannibusBricks().GetQuantity() < 25 && GetPlasticRoll() && GetPlasticRoll().GetQuantity() > PlaticWrap_Percent)
				{
					CP_TimerisRunning = true;
					LockCPWorkbenchSlots(true);					
					CreateBricks();
				}	
					
			}
			else
			{
				m_CP_Processing.Stop();
				CP_TimerisRunning = false;
				LockCPWorkbenchSlots(false);
			}
		}
		else if (BatteryRequired == 0)
		{
			if(GetCannibusBud() && GetCannibusBud().GetQuantity() >= BudsToBagsUsage && GetCannibusBags().GetQuantity() < 160 && GetEmptyBags() && GetEmptyBags().GetQuantity() > 0)
			{
				CP_TimerisRunning = true;
				LockCPWorkbenchSlots(true);	
				CreateBags();
			}
			else if(GetCannibusBags() && GetCannibusBags().GetQuantity() >= BagsToBricksUsage  && GetCannibusBricks().GetQuantity() < 25 && GetPlasticRoll() && GetPlasticRoll().GetQuantity() > PlaticWrap_Percent)
			{

				CP_TimerisRunning = true;
				LockCPWorkbenchSlots(true);					
				CreateBricks();
				
			}	
			else
			{

				m_CP_Processing.Stop();
				CP_TimerisRunning = false;
				LockCPWorkbenchSlots(false);
			};
		}
		SetSynchDirty();
	}


	void CreateBags()
	{
		ItemBase CannabisBud = GetCannibusBud();
		ItemBase EmptyBags = GetEmptyBags();
		ItemBase Batteries = GetBattieries();
		
		
		if(!GetCannibusBud())
			return;
		
		string Bagname = GetCannibusBud().GetcpBag(); 
		
		if(BatteryRequired == 1)
		{
			if(GetBattieries().GetCompEM().GetEnergy() >= Battery_Percent)
			{	
			
				if(GetCannibusBud().GetQuantity() >= BudsToBagsUsage)
				{		
					if(!GetCannibusBags())
					{
						GetInventory().CreateAttachment(Bagname);
					}
					else if (GetCannibusBags() && GetCannibusBags().GetType() == Bagname)
					{
						GetCannibusBags().AddQuantity(1); 
					}
					else
					{
						return;
					}
					EmptyBags.AddQuantity(-1);
	        		CannabisBud.AddQuantity(-BudsToBagsUsage); 
					Batteries.GetCompEM().AddEnergy( -Battery_Percent );
				};
			};
		}
		else if(BatteryRequired == 0)
		{
			if(GetCannibusBud().GetQuantity() >= BudsToBagsUsage)
			{
				if(!GetCannibusBags())
				{
					GetInventory().CreateAttachment(Bagname);
				}
				else if (GetCannibusBags() && GetCannibusBags().GetType() == Bagname)
				{
					GetCannibusBags().AddQuantity(1); 
				}
				else
				{
					return;
				}
				EmptyBags.AddQuantity(-1);
	        	CannabisBud.AddQuantity(-BudsToBagsUsage);
			} 
		};
	};
	
	void CreateBricks()
	{
		ItemBase CannabisBag = GetCannibusBags();
		ItemBase PlasticWrap = GetPlasticRoll();
		ItemBase Batteries = GetBattieries();
		

		
		
		if(!GetCannibusBags())
			return;
		
		string Brickname = GetCannibusBags().GetcpBrick(); 
		
		if(BatteryRequired == 1)
		{
		
			if(GetCannibusBags().GetQuantity() >= BagsToBricksUsage && GetPlasticRoll().GetQuantity() >= PlaticWrap_Percent)
			{
				if(!GetCannibusBricks())
				{
					GetInventory().CreateAttachment(Brickname);
				}
				else if (GetCannibusBricks() && GetCannibusBricks().GetType() == Brickname)
				{
					GetCannibusBricks().AddQuantity(1); 
				}
				else
				{
					return;
				}
        		CannabisBag.AddQuantity(-BagsToBricksUsage);
				PlasticWrap.AddQuantity(-PlaticWrap_Percent);
				Batteries.GetCompEM().AddEnergy( -Battery_Percent );
			};
		}
	    else if(BatteryRequired == 0)
		{
			if(GetCannibusBags().GetQuantity() >= BagsToBricksUsage && GetPlasticRoll().GetQuantity() >= PlaticWrap_Percent)
			{
		
				if(!GetCannibusBricks())
				{
					GetInventory().CreateAttachment(Brickname);
				}
				else if (GetCannibusBricks() && GetCannibusBricks().GetType() == Brickname)
				{
					GetCannibusBricks().AddQuantity(1); 
				}
				else
				{
					return;
				}
        		CannabisBag.AddQuantity(-BagsToBricksUsage);
				PlasticWrap.AddQuantity(-PlaticWrap_Percent);
			}
		};
	
	};

	void LockCPWorkbenchSlots(bool do_lock)
	{
		ItemBase Buds = GetCannibusBud();
		ItemBase EmptyBags = GetEmptyBags();	
		ItemBase FilledBags = GetCannibusBags();
		ItemBase PlasticWrap = GetPlasticRoll();
		ItemBase Wrapper = GetWrapper();
		ItemBase Bagger = GetBagger();		
		
		if (do_lock)
		{
			PlasticWrap.LockToParent();
			Bagger.LockToParent();
			Wrapper.LockToParent();
		}
		else
		{
			PlasticWrap.UnlockFromParent();
			Bagger.UnlockFromParent();
			Wrapper.UnlockFromParent();
			
		};
		SetSynchDirty();
	};
	
	bool IsPowered()
	{		
		if(BatteryRequired == true && HasEnergyManager() && GetCompEM().IsWorking() )
		{
			return true;
		}
		return false;
	};
	
	int GetInPutOccupied()
	{
		ItemBase thing = ItemBase.Cast(FindAttachmentBySlotName("BudsBuds"));
		if(thing)
		{
			return 1;
		}
		return 0;
	};
	
	int GetOutPutOccupied()
	{
		ItemBase thing = ItemBase.Cast(FindAttachmentBySlotName("Bricks"));
		if(thing)
		{
			return 1;
		}
		return 0;
	};
	
	bool Wrapper_Attached()
	{
		if(FindAttachmentBySlotName( ATTACHMENT_SLOT_WRAPPER))
		{
			return true;
		}
		return false;
	}
	bool Bagger_Attached()
	{
		if(FindAttachmentBySlotName( ATTACHMENT_SLOT_BAGGER))
		{
			return true;
		}
		return false;
	}
	
	bool Wrapper_Attachments()
	{
		if(FindAttachmentBySlotName( ATTACHMENT_SLOT_WRAPPER) && FindAttachmentBySlotName(ATTACHMENT_SLOT_BAGS))
		{
			return true;
		}
		return false;
	}
	bool Bagger_Attachments()
	{
		if(FindAttachmentBySlotName( ATTACHMENT_SLOT_BAGGER) && FindAttachmentBySlotName(ATTACHMENT_SLOT_EMPTYBAGS))
		{
			return true;
		}
		return false;
	}
	
	override bool CanDisplayAttachmentCategory(string category_name) 
	{	
		if ( category_name == "CP_Machines")
			return true;
		else if ( category_name == "CP_Wrapper" && Wrapper_Attached() )
			return true;
		else if ( category_name == "CP_Bagger" && Bagger_Attached() )
			return true;
		else
			return false;
    }
	/*
	override bool CanDisplayAttachmentSlot( string slot_name )
	{
		if (!super.CanDisplayAttachmentSlot(slot_name))
			return false;
		
		if ( slot_name == "CP_Cannabus_Bags" )
		{
			if ( Bagger_Attached() )
			{
				return true;
			}
			else
			 return false;
		}
		if ( slot_name == "LargeBattery" )
		{
			if(BatteryRequired == true)
			{
				return true;
			}
			else
			 return false;
		}
		return true;
	}
		*/
	override bool IsElectricAppliance()
	{
		return true;
	}
		 
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
	// checks if the player can get the workbench into his hands
	// - if any item is in attachment-slots or in cargo, the player can´t get the workbench into his hands
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override bool CanPutIntoHands(EntityAI parent) 
	{				
		// check if any item is in the attachment-slots OR if any item is in cargo space
		if(IsAnyItemAttached() || IsCargoEmpty())
		{
			return false;
		}
		else
			return true;
	}
	override bool CanPutInCargo( EntityAI parent )
	{
		// check if any item is in the attachment-slots OR if any item is in cargo space
		if(IsAnyItemAttached() || IsCargoEmpty())
		{
			return false;
		}
		else
			return true;
	
	}
	
	

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		
	// checks if has not additional items in it
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool IsAnyItemAttached() 
	{		
		if(GetInventory().AttachmentCount() > 0)
		{
			return true;		
		}
		return false;
	}

	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// checks if it has no items in cargo
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool IsCargoEmpty()	
	{		
		if(GetInventory().GetCargo().GetItemCount() > 0 )
		{
			return true;		
		}
		return false;
	}
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached( item, slot_name );
		
		ItemBase CannabisBud = GetCannibusBud();
		ItemBase EmptyBags = GetEmptyBags();
		
		
		if(GetEmptyBags().GetQuantity() < 1)
		{

			m_CP_Processing.Stop();
			CP_TimerisRunning = false;
		}
		else if( GetCannibusBud().GetQuantity() < 2 )
		{

			m_CP_Processing.Stop();
			CP_TimerisRunning = false;

		}

	};
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// determines the actions that the player can perform at the workbench
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
	override void SetActions()
	{
		super.SetActions();
		

		AddAction(ActionCPUseBagger);
		AddAction(ActionCPResumeAndPause);	
	};

}




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// not sure if this class is necessary
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class CP_Workbench_Holo extends ItemBase {
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override bool CanPutInCargo( EntityAI parent ) {
		return false;
	}
    
	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override bool CanPutIntoHands(EntityAI parent) {
		return false;
	}
}