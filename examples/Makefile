#Makefile
OBJECT=./util/publicfunctions.o ./util/inifile.o ./thrift/gen-cpp/StudentServ.o \
		./thrift/gen-cpp/student_types.o ./thrift/gen-cpp/student_constants.o \
		
CPP=g++ -g -std=c++11

ITHRIFT=-I./thrift/gen-cpp -I/usr/local/include/thrift/ 
LTHRIFT=/usr/local/thrift/lib/libthrift.a

CFLAGS=-I. -I./util ${ITHRIFT} 
LFLAGS=-pthread -ldl

all: client server

$(OBJECT):%.o:%.cpp
	$(CPP) ${CFLAGS} ${LFLAGS} -c $< -o $@

client.o:%.o:%.cpp
	$(CPP) ${CFLAGS} ${LFLAGS} -c $< -o $@
	
server.o:%.o:%.cpp
	$(CPP) ${CFLAGS} ${LFLAGS} -c $< -o $@

client:$(OBJECT) client.o
	$(CPP) -o client ${OBJECT}  client.o ${CFLAGS} ${LFLAGS} ${LTHRIFT}
	
server:$(OBJECT) server.o
	$(CPP) -o server ${OBJECT}  server.o ${CFLAGS} ${LFLAGS} ${LTHRIFT}

clean:
	$(RM) *.o ./util/*.o ./thrift/gen-cpp/*.o  client server
	/usr/local/thrift/bin/thrift -o ./thrift/ --gen cpp ./thrift/student.thrift
	
