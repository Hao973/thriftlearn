//client.cpp
#include "StudentServ.h"
#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>

#include <sstream>
#include "inifile.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using boost::shared_ptr;

void sendRecv();
bool InitConfInfo(const std::string& file, string& str_ip, int& port);

int main()
{
	sendRecv();
	return 0;
}

void sendRecv()
{
	string conffile = "conf/conf.ini";
	std::string str_ip = "192.168.37.131";
	int port = 9090;
	if(!InitConfInfo(conffile, str_ip, port)){
		return ;
	}
	printf("serverip: %s, port: %d\n", str_ip.c_str(), port);
    boost::shared_ptr<TSocket> socket(new TSocket(str_ip.c_str(), port));
    boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    
    socket->setConnTimeout(1000);
    socket->setRecvTimeout(1000*10000);

    transport->open();
   	StudentServClient* pClient = new StudentServClient(protocol);
	sturesult retinfo;
	student stu;
	
	
	std::vector<subjectscore> vec_subscore;
	for(int i=0; i<3; ++i){
		subjectscore subscore;
		if(0 == i){
			subscore.__set_str_subject("Match");
			subscore.__set_i_score(88);
		}else if(1 == i){
			subscore.__set_str_subject("Chinese");
			subscore.__set_i_score(85);
		}else{
			subscore.__set_str_subject("English");
			subscore.__set_i_score(82);
		}
		vec_subscore.push_back(subscore);
	}
	stu.__set_list_subscores(vec_subscore);
	stu.__set_i_uid(1001);
	stu.__set_str_name("xiaoming");
	stu.__set_str_sex("man");
	
	ostringstream  out;
	out.clear();
	stu.printTo(out);
	printf("student: %s\n", out.str().c_str());
	pClient->work_score(retinfo, stu);
	
	transport->close();
}

// 初始化配置信息
bool InitConfInfo(const std::string& file, string& str_ip, int& port) {
	CIniFile ini;
	std::string str_val;
	if (!ini.Init(file)) {
		printf("ini init file failed: %s\n", file.c_str());
		return false;
	}

	// load config
	ini.Read_Profile_String("client", "host", str_val);
	if (!str_val.empty())
		str_ip = str_val;
	else 
		str_ip = "192.168.37.131";
	
	ini.Read_Profile_String("client", "port", str_val);
	if (!str_val.empty())
		port = atoi(str_val.c_str());
	else
		port = 8828;

	return true;
}


