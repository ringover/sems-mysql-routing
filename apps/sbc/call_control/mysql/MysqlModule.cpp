#include "MysqlModule.h"

ConnectionPool_T pool;

using namespace std;

static map<string,data_sip*> map_data;

#define GET_VALUES(cfgparam, dst)                            \
    if(values.hasMember(cfgparam)){                          \
          dst = (string)string(values[cfgparam].asCStr());   \
    }

static char* timestamp2char(unsigned int connect_ts_sec, const char *format){
    char *date = (char*)malloc(sizeof(char)*80);
    memset(date,0,sizeof(char)*80);
    struct tm *info;
    const time_t time = (const time_t)connect_ts_sec;
    info = localtime(&time);
    strftime(date, sizeof(char)*80, format, info);
    return date;
}

/*static unsigned int char2timestamp(char *date, const char *format){
    unsigned int timestamp = 0;
    struct tm info;
    if(strptime(date,format,&info) != NULL){
        timestamp = (unsigned int)mktime(&info);
    }
    return timestamp;
}*/

class MysqlModuleFactory : public AmDynInvokeFactory{
public:
    MysqlModuleFactory(const string& name)
	: AmDynInvokeFactory(name) {}

    AmDynInvoke* getInstance(){
	return MysqlModule::instance();
    }

    int onLoad(){
      if(MysqlModule::instance()->onLoad())
      	return -1;

      DBG("Call control MySQL loaded.\n");
      return 0;
    }
};

EXPORT_PLUGIN_CLASS_FACTORY(MysqlModuleFactory, MOD_NAME);

MysqlModule* MysqlModule::_instance=0;

MysqlModule* MysqlModule::instance()
{
    if(!_instance)
	_instance = new MysqlModule();
    return _instance;
}

MysqlModule::MysqlModule(){
}

MysqlModule::~MysqlModule(){
  ConnectionPool_free(&pool);
}

int MysqlModule::onLoad(){
  AmConfigReader cfg;
  if(cfg.loadFile(AmConfig::ModConfigPath + string(MOD_NAME".conf"))){
    INFO(MOD_NAME "configuration file not found");
    return -1;
  }

  if(cfg.hasParameter("mysql_routing")){
    tblRoutes = cfg.getParameter("mysql_routing");
  }else{
    tblRoutes = "true";
  }

  if(cfg.hasParameter("mysql_current_calls")){
    tblCurrentCalls = cfg.getParameter("mysql_current_calls");
  }else{
    tblCurrentCalls = "true";
  }

  if(cfg.hasParameter("mysql_logs")){
    tblLogs = cfg.getParameter("mysql_logs");
  }else{
    tblLogs = "true";
  }
  
  if(cfg.hasParameter("mysql_host") && cfg.hasParameter("mysql_dbname") && cfg.hasParameter("mysql_username") && cfg.hasParameter("mysql_password")){
    string db_host = cfg.getParameter("mysql_host");
    string db_name = cfg.getParameter("mysql_dbname");
    string db_user = cfg.getParameter("mysql_username");
    string db_pass = cfg.getParameter("mysql_password");
    char url_str[200] = "";
    sprintf(url_str,"mysql://%s:%s@%s/%s",db_user.c_str(),db_pass.c_str(),db_host.c_str(),db_name.c_str());
    INFO("Connection to Mysql DB : %s", url_str);

    URL_T url = URL_new(url_str);
    pool = ConnectionPool_new(url);
    ConnectionPool_start(pool);
    ConnectionPool_setMaxConnections(pool,10);

    Connection_T con = ConnectionPool_getConnection(pool);
    Connection_executeQuery(con,table_route);
    Connection_executeQuery(con,table_current_calls);
    Connection_executeQuery(con,table_logs_sms);
    Connection_executeQuery(con,table_logs_calls);
    Connection_close(con);
    
    return 0; 
  }else{
    ERROR("ERROR connection Mysql!");
  }
  return -1; 
}

void MysqlModule::invoke(const string& method, const AmArg& args, AmArg& ret){
  if(method == "start"){
    SBCCallProfile* call_profile = dynamic_cast<SBCCallProfile*>(args[CC_API_PARAMS_CALL_PROFILE].asObject());
    start(args[CC_API_PARAMS_CC_NAMESPACE].asCStr(),
          args[CC_API_PARAMS_LTAG].asCStr(),
          call_profile,
          args[CC_API_PARAMS_TIMESTAMPS][CC_API_TS_START_SEC].asInt(),
          args[CC_API_PARAMS_TIMESTAMPS][CC_API_TS_START_USEC].asInt(),
          args[CC_API_PARAMS_CFGVALUES],
          args[CC_API_PARAMS_SIP_METHODE].asCStr(),
          args[CC_API_PARAMS_TIMERID].asInt(), ret);

  } else if(method == "connect"){
    SBCCallProfile* call_profile = dynamic_cast<SBCCallProfile*>(args[CC_API_PARAMS_CALL_PROFILE].asObject());
    connect(args[CC_API_PARAMS_CC_NAMESPACE].asCStr(),
            args[CC_API_PARAMS_LTAG].asCStr(),
            call_profile,
            args[CC_API_PARAMS_OTHERID].asCStr(),
            args[CC_API_PARAMS_TIMESTAMPS][CC_API_TS_CONNECT_SEC].asInt(),
            args[CC_API_PARAMS_TIMESTAMPS][CC_API_TS_CONNECT_USEC].asInt());

  } else if(method == "end"){
    SBCCallProfile* call_profile = dynamic_cast<SBCCallProfile*>(args[CC_API_PARAMS_CALL_PROFILE].asObject());
    end(args[CC_API_PARAMS_CC_NAMESPACE].asCStr(),
        args[CC_API_PARAMS_LTAG].asCStr(),
        call_profile,
        args[CC_API_PARAMS_TIMESTAMPS][CC_API_TS_END_SEC].asInt(),
        args[CC_API_PARAMS_TIMESTAMPS][CC_API_TS_END_USEC].asInt()
        );

  } else if(method == "_list"){
    ret.push("start");
    ret.push("connect");
    ret.push("end");
  }
  else
    throw AmDynInvoke::NotImplemented(method);
}

void MysqlModule::start(const string& cc_name, const string& ltag,
		       SBCCallProfile* call_profile,
		       int start_ts_sec, int start_ts_usec,
		       const AmArg& values, const string& methode,
           int timer_id, AmArg& res) {
  res.push(AmArg());
  AmArg& res_cmd = res[0];

  data_sip *data = new data_sip;
  GET_VALUES("FromUser",data->fromUser);
  GET_VALUES("FromDomain",data->fromDomain);
  GET_VALUES("ToUser",data->toUser);
  GET_VALUES("ToDomain",data->toDomain);
  GET_VALUES("RURI",data->ruri);
  data->methode = string(methode);
  data->uuid = string(ltag);
  map_data.insert(pair<string,data_sip*>(string(ltag),data));
  
  if(tblRoutes == "true" || tblRoutes == "1"){
    Connection_T con = ConnectionPool_getConnection(pool);
  
    char query[200];
    sprintf(query,"SELECT dest FROM routes WHERE number = '%s' LIMIT 1",data->toUser.c_str());
    ResultSet_T result = Connection_executeQuery(con,query);
  
    if(ResultSet_next(result)){
       call_profile->ruri = ResultSet_getString(result, 1);
       call_profile->to   = ResultSet_getString(result, 1);
    }else{
       ERROR("Query failed [%s]", query);
       res_cmd[SBC_CC_ACTION] = SBC_CC_REFUSE_ACTION;
       res_cmd[SBC_CC_REFUSE_CODE] = 404;
       res_cmd[SBC_CC_REFUSE_REASON] = "Not Found";
       free(data);
       map_data.erase(ltag);
    }

    Connection_close(con);
  }
  return;
}

void MysqlModule::connect(const string& cc_name, const string& ltag,
			 SBCCallProfile* call_profile,
			 const string& other_tag,
			 int connect_ts_sec, int connect_ts_usec){
  data_sip *data = (data_sip*)map_data.find(ltag)->second;
  data->connect_ts_sec = connect_ts_sec;
  if(data->methode != "MESSAGE" && (tblCurrentCalls == "true" || tblCurrentCalls == "1")){
    char *date_start = timestamp2char(connect_ts_sec,"%Y-%m-%d %H:%M:%S");

    char query[500];
    sprintf(query,"INSERT INTO current_calls (caller, callee, server, uuid, start_time) VALUES ('%s','%s','%s','%s','%s')",data->fromUser.c_str(),data->toUser.c_str(),data->fromDomain.c_str(),ltag.c_str(),date_start);
    free(date_start);

    Connection_T con = ConnectionPool_getConnection(pool);    
    ResultSet_T result = Connection_executeQuery(con,query);
    if(!result){
      ERROR("Query error [%s]", query);
    }
    Connection_close(con);    
  }
  return;
}

void MysqlModule::end(const string& cc_name, const string& ltag,
		     SBCCallProfile* call_profile,
		     int end_ts_sec, int end_ts_usec) {
  data_sip *data = (data_sip*)map_data.find(ltag)->second;
  if(!data){
    ERROR("Data is NULL (ltag = %s)",ltag.c_str());
    map_data.erase(ltag);
    return;
  }
  if(data->methode == "MESSAGE" && (tblLogs == "true" || tblLogs == "1")){
    char query[200];
    sprintf(query,"INSERT INTO logs_sms ( caller, callee, server) VALUES ('%s', '%s', '%s')",data->fromUser.c_str(),data->toUser.c_str(),data->fromDomain.c_str());

    Connection_T con = ConnectionPool_getConnection(pool);    
    ResultSet_T result = Connection_executeQuery(con,query);
    if(!result){
      ERROR("Query error [%s]", query);
    }
    Connection_close(con);
  }else{
    if(tblCurrentCalls == "true" || tblCurrentCalls == "1"){
      /*char query[200];
      int id = 0;
      sprintf(query,"SELECT id FROM current_calls WHERE uuid = '%s'",ltag.c_str());
      ResultSet_T result = Connection_executeQuery(con,query);
     
      if(ResultSet_next(result)){
        id = ResultSet_getString(result, 1);
      }*/
      char query[200];
      sprintf(query,"DELETE FROM current_calls WHERE uuid = '%s'",ltag.c_str());
      Connection_T con = ConnectionPool_getConnection(pool);
      ResultSet_T result = Connection_executeQuery(con,query);
      if(!result){
         ERROR("Query error [%s]", query);
      }
      Connection_close(con);
    }
    if(tblLogs == "true" || tblLogs == "1"){
      char query[200];
      char *start_time = timestamp2char((unsigned int)data->connect_ts_sec,"%Y-%m-%d %H:%M:%S");
      char *end_time   = timestamp2char((unsigned int)end_ts_sec,"%Y-%m-%d %H:%M:%S");

      int duration = end_ts_sec - data->connect_ts_sec;
      ostringstream duration_str;
      duration_str << duration;

      sprintf(query,"INSERT INTO logs_calls ( caller, callee, server, duration, start_time, end_time ) VALUES ('%s', '%s', '%s', '%s', '%s', '%s')",data->fromUser.c_str(),data->toUser.c_str(),data->fromDomain.c_str(),duration_str.str().c_str(),start_time,end_time);

      Connection_T con = ConnectionPool_getConnection(pool);
      ResultSet_T result = Connection_executeQuery(con,query);
      if(!result){
         ERROR("Query error [%s]", query);
      }
      free(start_time);
      free(end_time);
      Connection_close(con);    
    }
  }
  free(data);
  map_data.erase(ltag);
  return;
}