class CPRestApi
{    
    static protected string m_BaseUrl = "https://api.cannabisplus.tk/";
    static protected string m_AuthToken = "XQ9mOhtiFZd1TxJ23Z.ia9vC0Tb2pO;Vnx2dqvke~9UNP";
        
    static RestApi Api()
    {
        RestApi clCore = GetRestApi();
        if (!clCore)
        {
            clCore = CreateRestApi();
            clCore.SetOption(ERestOption.ERESTOPTION_READOPERATION, 15);
        }
        return clCore;
    }
    
    static void Post(string url, string jsonString = "{}", ref RestCallback UCBX = NULL)
    {
        if (!UCBX){
            UCBX = new ref CPApiSilentCallBack
        }
        RestContext ctx =  Api().GetRestContext(url);
        ctx.SetHeader("application/json");
        ctx.POST(UCBX , "", jsonString);
    }

    static string BaseUrl(){
        return m_BaseUrl;
    }
    
    static string AuthToken(){
        return m_AuthToken;
    }
    
    static void Increment(string element, float value = 1) {
        
            UCBX = new ref CPTransactionCallBack;
        
        if (auth == "" ){
            auth = AuthToken();
        }
        
        string url = BaseUrl() + "Globals/Transaction/CannabisPlus/" + auth;
        
        ref CPApiTransaction transaction = new ref CPApiTransaction(element, value);
        
        if ( element && transaction && UCBX){
            Post(url,transaction.ToJson(),UCBX);
        } else {
            Print("[CPApi] [Api] Error Updating " +  element);
        }
    }    
    
};

static ref CPRestApi g_CPRestApi;

static ref CPRestApi CPApi()
{
    if ( !g_CPRestApi )
    {
        Print("[CPApi] Init");
        g_CPRestApi = new ref CPRestApi;
    }

    return g_CPRestApi;
};