//server.cpp
#include "StudentServ.h"
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>

#include <sstream>
#include "inifile.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using boost::shared_ptr;
using namespace std;


// 初始化配置信息
bool InitConfInfo(const std::string& file, int& port) {
	CIniFile ini;
	std::string str_val;
	if (!ini.Init(file)) {
		printf("ini init file failed: %s\n", file.c_str());
		return false;
	}

	// load config
	ini.Read_Profile_String("server", "port", str_val);
	if (!str_val.empty())
		port = atoi(str_val.c_str());
	else
		port = 8828;

	return true;
}


class ServerHandler : virtual public StudentServIf {
 public:
  ServerHandler() {
    // Your initialization goes here
  }

  void work_score(sturesult& _return, const student& stu) {
  	ostringstream  myout;
	myout.clear();
    stu.printTo(myout);
	printf("student: %s\n", myout.str().c_str());
    _return.__set_i_uid(stu.i_uid);
	_return.__set_str_name(stu.str_name);
	_return.__set_str_sex(stu.str_sex);
	_return.__set_i_subscore(200);
	_return.__set_i_subscore(85);
	_return.__set_str_maxsub("Match");
	_return.__set_str_mixsub("English");
	myout.str("");//清空数据 
	_return.printTo(myout);
	printf("return info:%s\n", myout.str().c_str());
    printf("ServerHandler work_score\n");
  }

};

int main(int argc, char **argv) {
  string conffile = "conf/conf.ini";
  int port = 9090;
  if(!InitConfInfo(conffile, port)){
	return -1;
  }
  printf("Server Port:%d\n", port);
  boost::shared_ptr<ServerHandler> handler(new ServerHandler());
  boost::shared_ptr<TProcessor> processor(new StudentServProcessor(handler));
  boost::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  boost::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

