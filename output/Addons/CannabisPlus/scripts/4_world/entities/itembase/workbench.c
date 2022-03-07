//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 'workbench_kit' represents the item from which the finished workbench can be made.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class CP_Workbench_Kit extends ItemBase 
{
	ref protected EffectSound	m_DeployLoopSound;
	protected Object			Workbench_Kit1;
	
	

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// constructor for class CP_Workbench_Kit to initialize the necessary
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void CP_Workbench_Kit()	
	{
		RegisterNetSyncVariableBool("m_IsSoundSynchRemote");
	}
	
	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Called upon object creation.
	// Can be removed at the moment, so no changes are made to the method in the modded class.
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override void EEInit() 
	{
		super.EEInit();
	}
	
	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Event handler that is fired if location of workbench kit is changed. 
	// Can be removed at the moment, so no changes are made to the method in the modded class.
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override void OnItemLocationChanged( EntityAI old_owner, EntityAI new_owner ) 
	{
		super.OnItemLocationChanged( old_owner, new_owner );
	}	
	
	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Syncronize deployment sound for workbench kit. 
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override void OnVariablesSynchronized()	
	{
		super.OnVariablesSynchronized();
		
		if ( IsDeploySound() ) {
			PlayDeploySound();
		}
				
		if ( CanPlayDeployLoopSound() )	{
			PlayDeployLoopSound();
		}
					
		if ( m_DeployLoopSound && !CanPlayDeployLoopSound() ) {
			StopDeployLoopSound();
		}
	};
	
	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// set actions to place the workbench kit
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override void SetActions() 
	{		
		super.SetActions();		
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionPlaceObject);
	}		
	
	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Event handler that is fired if placement of workbenchkit is complete.
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
	override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" ) 
	{		
		super.OnPlacementComplete( player );		
		if ( GetGame().IsServer() )	{
			Workbench_Kit1 = GetGame().CreateObjectEx( "CP_Workbench", GetPosition(), ECE_PLACE_ON_SURFACE );
			Workbench_Kit1.SetPosition( position );
			Workbench_Kit1.SetOrientation( orientation );
		}	
		HideAllSelections();
		SetIsDeploySound( true );
		// Should be removed so that the workbench does not disappear?
		//SetLifetime(3888000);
	}
	
	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Indicates whether the workbench kit can be deployed.
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override bool IsDeployable() 
	{		
		return true;
	}	
	
	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Sets the appropriate sound effects when object is placed.
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override string GetDeploySoundset() 
	{		
		return "putDown_FenceKit_SoundSet";
	}
	
	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// returns soundset for deploying the the workbench kit
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override string GetLoopDeploySoundset() 
	{		
		return "BarbedWire_Deploy_loop_SoundSet";
	}
	
	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// starts playing the sound for deploying the workbench_kit
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void PlayDeployLoopSound() 
	{	
		// check if its currently a multiplayer-game and the code is executed on the client?	
		if ( GetGame().IsMultiplayer() && GetGame().IsClient() || !GetGame().IsMultiplayer() ) {		
			m_DeployLoopSound = SEffectManager.PlaySound( GetLoopDeploySoundset(), GetPosition() );
		}
	}
	
	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// stops playing the sound for deploying the workbench_kit
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void StopDeployLoopSound() 
	{
		// check if its currently a multiplayer-game and the code is executed on the client?
		if ( GetGame().IsMultiplayer() && GetGame().IsClient() || !GetGame().IsMultiplayer() ) {
			// deplyment sound is stoping
			m_DeployLoopSound.SoundStop();
			// Destroy object instance of deployment sound
			delete m_DeployLoopSound;
		}
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
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Entity entry Intilize
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override void EEInit()
	{
      super.EEInit();
	 // RegisterNetSyncVariableInt("m_UseCPWorkbench");
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
		if(!GetCannibusBricks())
			return "";
		
		string Brickname = GetCannibusBricks().GetDisplayName();

		
        return "Wrap " + Brickname;

    };

	void start_processing()
	{		
		if(m_CP_Processing && m_CP_Processing.IsRunning())
			return;
			Print(CP_TimerisRunning);
		if(!m_CP_Processing)
		{
		   	m_CP_Processing = new Timer;
			m_CP_Processing.Run(2000,this,"Do_processing",NULL,true);
			CP_TimerisRunning = true;
			CP_TimerIsPaused = false;
			
			Print(CP_TimerisRunning);
			Print("Void start_processing " + CP_TimerIsPaused);
		}

		
		
		SetSynchDirty();	
	}

		
	void PauseOrResume()
	{
		if(CP_TimerIsPaused == false)
		{
			m_CP_Processing.Pause();
			CP_TimerIsPaused = true;
			Print("Void PauseorResume " + CP_TimerIsPaused);
		}
		else if(CP_TimerIsPaused == true)
		{
			m_CP_Processing.Continue();
			CP_TimerIsPaused = false;
			Print("Void PauseorResume " + CP_TimerIsPaused);
		}
	
	}
	
	bool RunningOrNot()
	{
		if(CP_TimerisRunning == true)
		{
		  Print("Timer is Paused " + CP_TimerIsPaused);
		  return true;
		  Print("Void RunningOrNot " + RunningOrNot());
		}
		else
		Print("Timer is Paused " + CP_TimerIsPaused);
		return false;
		Print("Void RunningOrNot " + RunningOrNot());		
	}

	void Do_processing()
	{
	
		ItemBase CannabisBud = GetCannibusBud();
	
		ItemBase EmptyBags = GetEmptyBags();
		
		ItemBase Batteries = GetBattieries();
	
		Print("Working ");
		
		
		if(GetBattieries().GetCompEM().GetEnergy() > 10)
		{
			Print("Battery Check");
			if(GetCannibusBud() && GetCannibusBud().GetQuantity() > 2 || GetEmptyBags() && GetEmptyBags().GetQuantity() > 1)
			{
				Print("Running Running");
				CP_TimerisRunning = true;
				CreateBags();
			};
		}
		else
		{
			m_CP_Processing.Stop();
			CP_TimerisRunning = false;
			Print(CP_TimerisRunning);
			Print("Stoped Processing");
		};
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
        CannabisBud.AddQuantity(-2); 
		Batteries.GetCompEM().AddEnergy( -10 );
	};
	
	void CreateBricks()
	{
		ItemBase CannabisBag = GetCannibusBags();
		ItemBase PlasticWrap = GetPlasticRoll();
		ItemBase Batteries = GetBattieries();
		
		
		if(!GetCannibusBags())
			return;
		
		string Brickname = GetCannibusBags().GetcpBrick(); 
		
		
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
        CannabisBag.AddQuantity(-16);
		PlasticWrap.AddQuantity(-25);
		Batteries.GetCompEM().AddEnergy( -10 );
	};


	
	bool IsPowered()
	{		
		if(HasEnergyManager() && GetCompEM().IsWorking())
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
			return false;
		}
		return true;
	}
	
	override bool IsElectricAppliance()
	{
		return true;
	}
		 
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
	// checks if the player can get the workbench into his hands
	// - if any item is in attachment-slots or in cargo, the player can´t get the workbench into his hands
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override bool CanPutIntoHands(EntityAI parent) 
	{				
		// check if any item is in the attachment-slots OR if any item is in cargo space
		if(IsAnyItemAttached())
		{
			return false;
		}
		else if(!IsCargoEmpty())
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/*
	override bool CanReceiveAttachment(EntityAI attachment, int slotId) 
	{
		Print("CAN RECEIVE ATTACHEMENT");
		
		//if ( !super.CanReceiveAttachment(attachment, slotId) )
		//	return false;
		//
		//ItemBase item = ItemBase.Cast( attachment );
		//
		//if((this.m_IsBaggerActive == true) || (this.m_IsWrapperActive == true)){
		//	return false;
		//} else {
		//	return true;
		//}
		//
		return true;		
	}
	*/
	

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		
	// checks if has not additional items in it
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool IsAnyItemAttached() 
	{		
		return (GetInventory().AttachmentCount() >= 0 );
	}
	
	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// checks if it has no items in cargo
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool IsCargoEmpty()	
	{		
		return ( GetInventory().GetCargo().GetItemCount() == 0 );
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
			Print(CP_TimerisRunning);
			Print("Stoped Processing do to removal of empty bags");
			Print("Removed bags " + "buds still attached = " + CannabisBud.GetQuantity());
		}
		else if( GetCannibusBud().GetQuantity() < 2 )
		{

			m_CP_Processing.Stop();
			CP_TimerisRunning = false;
			Print(CP_TimerisRunning);
			Print("Stoped Processing do to removal of buds");
			Print("Removed buds " + "Bags still attached = "  + EmptyBags.GetQuantity());
		}

	};
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// determines the actions that the player can perform at the workbench
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionPlugIn);
		AddAction(ActionTurnOnWhileOnGround);
		AddAction(ActionTurnOffWhileOnGround);
		AddAction(ActionCPResumeAndPause);
		AddAction(ActionCPUsePlasticWrapper);
		AddAction(ActionCPUseBagger);
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