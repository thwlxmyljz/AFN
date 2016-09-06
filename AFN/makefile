cc=g++
#SO_REUSEADDR£ºlisten socket reuse
#RUNASCHILD: make as AFN.so
debug=-c -Wall -fpermissive -g -D_DEBUG -DSO_REUSEADDR -DRUNASCHILD
release=-c -Wall -DSO_REUSEADDR -DRUNASCHILD

SRCDIR := ./src
THIRDDIR := ./thirdparty
OBJDIR := ./obj
COMMDIR := ./common

cmp=$(debug)
APP_NAME=AFN
#APP_NAME=AFN.so
DEFS=-DHAVE_FCNTL_H 

VPATH=$(SRCDIR):$(THIRDDIR):$(COMMDIR)

INC = -I/usr/local/include \
	   -I/usr/local/include/mysql \
	  -I$(THIRDDIR) \
	  -I$(SRCDIR)\
	  -I$(COMMDIR)


LIB = -lpthread -levent -lxml2 -lmysqlclient
  
SRCS:=$(wildcard $(SRCDIR)/*.cpp $(THIRDDIR)/*.cpp $(COMMDIR)/*.cpp)
SRCS:=$(notdir $(SRCS))
OBJS:=$(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))

$(OBJDIR)/%.o: %.cpp
	echo compiling $(<F)
	$(cc) $(cmp) $(INC) $(DEFS) $< -o $@
all:$(OBJS)	
	$(cc) $(LIB) -o $(APP_NAME) $(OBJS)
clean :
	echo rm begin 
	rm -f $(OBJDIR)/*.o
	rm -f $(APP_NAME)
	echo rm ok


