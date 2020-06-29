//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
modded class PlayerBase {
	
	ref Timer swayTimer;				// timer that resets the values after the effect is over
	ref Timer jointTimer;				// timer that resets the values after the effect is over
	
	bool m_HasConsumedCigarette = false;	// has the player consumed a hole cigarette	
	int m_cigaretteValue = 0;			// the quantity of the cigarette, what the player consumed
	static bool m_activateCigaretteSmokingEffect = true;
	static int m_cigaretteCyclesToActivateEffect = 8;
	static int m_smokingCigaretteEffectDuration = 60;
	
	bool m_HasConsumedJoint = false;
	int m_jointValue = 0;				// the quantity of the cigarette, what the player consumed
	static bool m_activateJointSmokingEffect =  true;
	static int m_jointCyclesToActivateEffect = 8;
	static int m_smokingJointEffectDuration = 60;
	
	static float m_jointEffectRed 		= 0.5;
	static float m_jointEffectGreen 		= 0.3;
	static float m_jointEffectBlue 		= 0.1;
	
	//getters for cig/joint smoke state
	bool HasConsumedJoint () {
		return m_HasConsumedJoint; 
	}
	bool HasConsumedCigarette () {
		return m_HasConsumedCigarette; 
	}		
	
	void PlayerBase() {
		if ( GetGame().IsServer() ){
			m_activateCigaretteSmokingEffect = CannabisPlus.getInstance().GetConfig().activateCigaretteSmokingEffect;
			m_cigaretteCyclesToActivateEffect = CannabisPlus.getInstance().GetConfig().cigaretteCyclesToActivateEffect;
			m_smokingCigaretteEffectDuration = CannabisPlus.getInstance().GetConfig().smokingCigaretteEffectDuration;	
			m_activateJointSmokingEffect =  CannabisPlus.getInstance().GetConfig().activateJointSmokingEffect;
			m_jointCyclesToActivateEffect = CannabisPlus.getInstance().GetConfig().jointCyclesToActivateEffect;
			m_smokingJointEffectDuration = CannabisPlus.getInstance().GetConfig().smokingJointEffectDuration;
			
			m_jointEffectRed = CannabisPlus.getInstance().GetConfig().jointEffectRed;
			m_jointEffectGreen = CannabisPlus.getInstance().GetConfig().jointEffectGreen;
			m_jointEffectBlue = CannabisPlus.getInstance().GetConfig().jointEffectBlue;
			Print("CP_JointBase: Setting up joint effects");
			Print(m_activateCigaretteSmokingEffect);
			Print(m_cigaretteCyclesToActivateEffect);
			Print(m_smokingCigaretteEffectDuration);
			Print(m_activateJointSmokingEffect);
			Print(m_jointCyclesToActivateEffect);
			Print(m_smokingJointEffectDuration);
			Print(m_jointEffectRed);
			Print(m_jointEffectGreen);
			Print(m_jointEffectBlue);
		}	
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// set to true if the player consumed a cigarette
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void AddValueToCigaretteValue(int value) {		
		if(m_activateCigaretteSmokingEffect) {
			m_cigaretteValue += value;
					
			if(m_cigaretteValue >= m_cigaretteCyclesToActivateEffect){
				m_HasConsumedCigarette = true;
				swayTimer = new Timer();
				swayTimer.Run(m_smokingCigaretteEffectDuration, this, "ResetCigaretteValues", null, false);				
			}
		}		
	}	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// set to true if the player consumed a joint
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void AddValueToJointValue(int value) {		
		if(m_activateJointSmokingEffect) {
			m_jointValue += value;
					
			if(m_jointValue >= m_jointCyclesToActivateEffect){				
				m_HasConsumedJoint = true;
				jointTimer = new Timer();
				jointTimer.Run(m_smokingJointEffectDuration, this, "ResetJointValues", null, false);				
			}
		}
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// called by timer if the effect is over, resets all values that the player "consume again"
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void ResetCigaretteValues() {		
		m_cigaretteValue = 0;
		m_HasConsumedCigarette = false;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// called by timer if the effect is over, resets all values that the player "consume again"
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void ResetJointValues() {
		m_HasConsumedJoint = false;
		m_jointValue = 0;
	}

	override void Init()
      {
        if ( GetGame().IsServer() || GetGame().IsMultiplayer() )
		{
            DayzPlayerItemBehaviorCfg     heavy = new DayzPlayerItemBehaviorCfg;
		DayzPlayerItemBehaviorCfg 	onehand = new DayzPlayerItemBehaviorCfg;

            heavy.SetHeavyItems();
		onehand.SetToolsOneHanded();

            GetDayZPlayerType().AddItemInHandsProfileIK("CP_JointSkunk", "dz/anims/workspaces/player/player_main/player_main_1h.asi", onehand, "dz/anims/anm/player/ik/gear/thermometer.anm");
		GetDayZPlayerType().AddItemInHandsProfileIK("CP_JointBlue", "dz/anims/workspaces/player/player_main/player_main_1h.asi", onehand, "dz/anims/anm/player/ik/gear/thermometer.anm");
		GetDayZPlayerType().AddItemInHandsProfileIK("CP_JointKush", "dz/anims/workspaces/player/player_main/player_main_1h.asi", onehand, "dz/anims/anm/player/ik/gear/thermometer.anm");
		GetDayZPlayerType().AddItemInHandsProfileIK("CP_JointStardawg", "dz/anims/workspaces/player/player_main/player_main_1h.asi", onehand, "dz/anims/anm/player/ik/gear/thermometer.anm");
		GetDayZPlayerType().AddItemInHandsProfileIK("CP_Cigarette", "dz/anims/workspaces/player/player_main/player_main_1h.asi", onehand, "dz/anims/anm/player/ik/gear/thermometer.anm");
	  }
	  
        super.Init();
    }
}