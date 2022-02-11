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
	}
	
	
	
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
	const string ATTACHMENT_SLOT_BAGGER				= "Bagger";
	const string ATTACHMENT_SLOT_BUDS				= "CP_Cannabus_Buds";
	const string ATTACHMENT_SLOT_BAGS 				= "CP_Cannabus_Bags";
	const string ATTACHMENT_SLOT_BAGSWRAPPER        = "CP_CannabusWrapper_Bags";
	const string ATTACHMENT_SLOT_BRICKS 			= "CP_Cannabus_Bricks";
<<<<<<< HEAD
<<<<<<< HEAD
	
=======
>>>>>>> parent of b9f99c4 (Full working dynamic Brick wrapping system - compliments to daemonforge)
=======
>>>>>>> parent of b9f99c4 (Full working dynamic Brick wrapping system - compliments to daemonforge)
	// timer to get bagger working
	protected ref Timer m_BaggerWorkingTimer;
	// timer to get wrapper working
	protected ref Timer m_WrapperWorkingTimer;	
	// indicates whether bagger is currently working

	
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
	// A on Work Function to Make sure its powered or has Power if not Shut off.
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	override void OnWork( float consumed_energy  )
	{
		
		if ( GetGame() && GetGame().IsServer()  )
		{
			if(GetCompEM().GetEnergy() <= 1)
			{
				OnWorkStop()
			}
		}		
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
		return CP_CannabisBags.Cast(FindAttachmentBySlotName( ATTACHMENT_SLOT_BAGS) );
	}
	
	CP_CannabisBags GetCannibusWrapperBags()
	{
		return CP_CannabisBags.Cast(FindAttachmentBySlotName( ATTACHMENT_SLOT_BAGSWRAPPER) );
	}
	
	CP_CannabisBrickBase GetCannibusBricks()
	{
		return CP_CannabisBrickBase.Cast(FindAttachmentBySlotName( ATTACHMENT_SLOT_BRICKS) );
	}
	

	
	string GetBagTendancyText()
	{
		if(!GetCannibusBud())
			return "";
		
		string CPBagName = GetCannibusBud().GetDisplayName();

		
        return "Fill " + CPBagName + " Bag";

    };
	
	string GetBrickTendancyText()
	{
		if(!GetCannibusWrapperBags())
			return "";
		
		string Brickname = GetCannibusWrapperBags().GetDisplayName();

		
        return "Wrap " + Brickname;

    };

	
	
	void CreateBags()
	{

		
		ItemBase CannabisBud = GetCannibusBud();
		
		
		if(!GetCannibusBud())
		{
			return;
		}
		
		string CPBagName = GetCannibusBud().GetcpBag(); 
		
		if(!GetCannibusBags())
		{

			
			GetInventory().CreateAttachment(CPBagName);
		}
		else if (GetCannibusBags() && GetCannibusBud().GetType() == CPBagName)
		{

			GetCannibusBags().AddQuantity(1); 
		}
		else
		{
			return;
		}
        CannabisBud.AddQuantity(-2); 
	}
	
	void StepWrapperTendancy()
	{
      m_UseCPWorkbench++;
      if (m_UseCPWorkbench > 0)
	  {
         m_UseCPWorkbench = 0;
      }
      SetSynchDirty();
    };
	
	
	int CPWorkbenchTendancy()
	{
		
       return m_UseCPWorkbench;
	   
    };
	
	
	void StepWrapperTendancy()
	{
      m_UseCPWorkbench++;
      if (m_UseCPWorkbench > 0)
	  {
         m_UseCPWorkbench = 0;
      }
      SetSynchDirty();
    };
	
	
	int CPWorkbenchTendancy()
	{
		
       return m_UseCPWorkbench;
	   
    };
	
	
	void CreateBricks()
	{
		ItemBase Cannabisbags = GetCannibusWrapperBags();
		
		Print("Create bricks Function Triggered")
		
		if(!GetCannibusWrapperBags())
		{
			Print("Failed to find Bags")
			return;
		}
		
		string Brick = GetCannibusWrapperBags().GetcpBrick(); 
		
		if(GetCannibusBricks())
		{
<<<<<<< HEAD
<<<<<<< HEAD
			Print("Create Bag if empty slot")
			
			GetInventory().CreateAttachment(Brick);
			Print(Brick)
		}
		else if (GetCannibusBricks() && GetCannibusWrapperBags().GetType() == Brick)
		{
			Print("Create bag if slot is full")
			
			GetCannibusBricks().AddQuantity(1); 
		}
		else
		{
			Print("Return Nothing")
			return;
=======
=======

			GetCannibusBricks().AddQuantity(1);
		}
		else
		{
			GetInventory().CreateAttachment(Brickname);
		}
        CannabisBud.AddQuantity(-1); 
	};
	
/*
	void CreateBricks()//string CreationType
	{
		Print ("trigger");
>>>>>>> parent of b9f99c4 (Full working dynamic Brick wrapping system - compliments to daemonforge)

			GetCannibusBricks().AddQuantity(1);
		}
		else
		{
			GetInventory().CreateAttachment(Brickname);
>>>>>>> parent of b9f99c4 (Full working dynamic Brick wrapping system - compliments to daemonforge)
		}
        Cannabisbags.AddQuantity(-16); 
	};
	

	
	bool IsPowered()
	{		
		if(HasEnergyManager() && GetCompEM().IsWorking())
		{
			return true;
		}
		return false;
	};
	
	
	bool Wrapper_Attached()
	{
		if(FindAttachmentBySlotName( ATTACHMENT_SLOT_WRAPPER))
		{
			return true;
		}
		return false;
	};
	
	bool BaggerAttached()
	{
		if(FindAttachmentBySlotName( ATTACHMENT_SLOT_BAGGER))
		{
			return true;
		}
		return false;
	}
	
	override bool CanDisplayAttachmentCategory(string category_name) 
	{	
		if ( category_name == "CP_Machines")
		{
			return true;
		}
		else if ( category_name == "CP_Wrapper" && Wrapper_Attached() )
		{
			return true;
		}
		else if ( category_name == "CP_Bagger" && BaggerAttached() )
		{
			return true;
		}
		else
		{
			return false;
		}
		return false;
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
		if ( !super.CanPutIntoHands( parent ) )
			return false;
		
		// check if any item is in the attachment-slots OR if any item is in cargo space
		if(IsAnyItemAttached() || !IsCargoEmpty())
		{
			return false;
		}
		else 
		{
			return true;
		}
	};
	
	override bool CanPutInCargo( EntityAI parent )
    {
		if ( !super.CanPutIntoHands( parent ) )
			return false;
		
		// check if any item is in the attachment-slots OR if any item is in cargo space
		if(IsAnyItemAttached() || !IsCargoEmpty())
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
	override bool CanReceiveAttachment(EntityAI attachment, int slotId) 
	{
		Print("CAN RECEIVE ATTACHEMENT")

		return true;		
	}
	

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~		
	// checks if has not additional items in it
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool IsAnyItemAttached() 
	{		
		return !( GetInventory().GetCargo().GetItemCount() == 0 && GetInventory().AttachmentCount() == 0 );
	}
	
	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// checks if it has no items in cargo
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool IsCargoEmpty()	
	{		
		return ( GetInventory().GetCargo().GetItemCount() == 0 );
	}
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// determines the actions that the player can perform at the workbench
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionPlugIn);
		AddAction(ActionTurnOnWhileOnGround);
		AddAction(ActionTurnOffWhileOnGround);
		AddAction(ActionUsePlasticWrapper);
		AddAction(ActionUseBagger);
	}

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