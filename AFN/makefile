cc=g++
#SO_REUSEADDR£ºlisten socket reuse
#RUNASCHILD: make as MGLOG.so

debug=-c -Wall -g -D_DEBUG -DSO_REUSEADDR -DRUNASCHILD
release=-c -Wall -DSO_REUSEADDR -DRUNASCHILD -DLOGLOG

SRCDIR := ./src
BASEDIR := ./base
YQCOMMONDIR := ../../common/comm
SOCKETDIR := ./socket
OBJDIR := ./obj
H248API := ../../common/YQH248Api
YIQIAO := /home/yiqiao/lib

#cmp=$(debug)
cmp=$(release)
#APP_NAME=LogSvr
APP_NAME=MGLOG.so
DEFS=-DHAVE_FCNTL_H 

SYBASE=/opt/sybase-12.5
SYBASE_OCS=OCS-12_5

VPATH=$(SRCDIR):$(BASEDIR):$(YQCOMMONDIR):$(SOCKETDIR)

INC = -I/usr/include/mysql -I$(SYBASE)/$(SYBASE_OCS)/include \
	  -I$(YQCOMMONDIR) \
	  -I$(BASEDIR) \
	  -I$(SOCKETDIR) \
	  -I$(SRCDIR) \
	  -I$(H248API)


LIB = -L/usr/lib/mysql -lmysqlclient -lz -lcrypt -lnsl -lm -L/usr/lib -lssl -lcrypto \
  -L$(SYBASE)/$(SYBASE_OCS)/lib -lct -lcs -lsybtcl -lcomn -lintl -rdynamic -ldl -lnsl -lm -lpthread \
  -L$(YIQIAO) $(YIQIAO)/YQH248MgApi.so $(YIQIAO)/YQH248.so
  
#LIBDIR   = $(SYBASE)/$(SYBASE_OCS)/devlib     # for debug library versions
#LIBDIR    = $(SYBASE)/$(SYBASE_OCS)/lib       # Sybase library directory
SRCS:=$(wildcard *.cpp $(SRCDIR)/*.cpp $(BASEDIR)/*.cpp $(YQCOMMONDIR)/*.cpp $(SOCKETDIR)/*.cpp)
SRCS:=$(notdir $(SRCS))
OBJS:=$(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))

$(OBJDIR)/%.o: %.cpp
	echo compiling $(<F)
	$(cc) $(cmp) $(LIB) $(INC) $(DEFS) $< -o $@
all:$(OBJS)	
#	$(cc) -g -DDEBUG -Dlinux=l $(LIB) $(INC) $(DEFS) -o $(APP_NAME) $(OBJS)
	$(cc) -g -DDEBUG -Dlinux=l $(LIB) $(INC) $(DEFS) -fPIC -shared -o $(APP_NAME) $(OBJS)
clean :
	echo rm begin 
	rm -f $(OBJDIR)/*.o
	rm -f $(APP_NAME)
	echo rm ok


