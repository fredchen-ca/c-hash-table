# 
# Simple Generic GNU C/C++ Makefile
#
# Author: Frederic Chen
#
# Date: Nov 14, 2015
# 
# Tested: OK
#

####################################################################
#
#  << Program Dependent Section >>
#
###################################################################

TARGET = hash
SRCDIR = 
OBJDIR = 
PREFIX_BIN =
# start with -I
INCLUDES = 
# start with -L
LIBS =

####################################################################
#
#  << Program Common Section >>
#
####################################################################

CC = gcc
CXX = g++
CFLAGS =-Wall -Werror -Wno-deprecated -O2 -g 
LINKFLAGS =

C_SRCDIR = $(SRCDIR)  
ifdef SRCDIR
  C_SOURCES = $(foreach d,$(C_SRCDIR),$(wildcard $(d)/*.c) )
else
  C_SOURCES = $(wildcard *.c)
endif

ifdef OBJDIR
  C_OBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(C_SOURCES))  
else
  C_OBJS = $(patsubst %.c, %.o, $(C_SOURCES))  
endif

CPP_SRCDIR = $(SRCDIR)  
ifdef SRCDIR
  CPP_SOURCES = $(foreach d,$(CPP_SRCDIR),$(wildcard $(d)/*.cpp) )  
else
  CPP_SOURCES = $(wildcard *.cpp)  
endif

ifdef OBJDIR
  CPP_OBJS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(CPP_SOURCES))  
else
  CPP_OBJS = $(patsubst %.cpp, %.o, $(CPP_SOURCES))  
endif

#----------------------------------------------------------------------
#
# make targets definitions
#
#----------------------------------------------------------------------

# Default to build all
default all: init compile

# Create object subdirectories
init:  
ifdef OBJDIR
# multiple subdirectories of two levels as $OBJ/$SRC
	$(foreach d,$(SRCDIR), mkdir -p $(OBJDIR)/$(d);)  
# single level not working
#	mkdir -p $(OBJDIR)
endif

# Display predefined program directories 
info:  
	@echo "C_SOURCES: $(C_SOURCES)"  
	@echo "C_OBJS: $(C_OBJS)"  
	@echo "CPP_SOURCES: $(CPP_SOURCES)"  
	@echo "CPP_OBJS: $(CPP_OBJS)"  
	@echo "PREFIX_BIN: $(PREFIX_BIN)"  

# Generate the objects and target program
# Use CXX for both C/C++ objects
compile: $(C_OBJS) $(CPP_OBJS) $(TARGET) 

ifdef OBJDIR 
  $(C_OBJS):$(OBJDIR)/%.o:%.c  
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@  

  $(CPP_OBJS):$(OBJDIR)/%.o:%.cpp  
	$(CXX) -c $(CFLAGS) $(INCLUDES) $< -o $@  
else
  $(C_OBJS):%.o:%.c  
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@  

  $(CPP_OBJS):%.o:%.cpp  
	$(CXX) -c $(CFLAGS) $(INCLUDES) $< -o $@  
endif

  $(TARGET):
	$(CXX)  $(C_OBJS) $(CPP_OBJS) -o $(TARGET)  $(LINKFLAGS) $(LIBS)  


# Cleanup all
clean:  
ifdef OBJDIR
	rm -rf $(OBJDIR) $(TARGET)
else 
	rm -f $(TARGET) $(C_OBJS) $(CPP_OBJS)
endif 

# Install program into traget binary directory 
install: $(TARGET)  
ifdef PREFIX_BIN
	cp $(TARGET) $(PREFIX_BIN)
endif

# Remove program from traget binary directory 
uninstall:  
ifdef PREFIX_BIN
	rm -f $(PREFIX_BIN)/$(TARGET)  
endif

# Cleanup everything
realclean: clean uninstall

# Force to rebuild all
rebuild: clean init compile 

# Display help menu
help:
	@echo "Makefile options:"
	@echo "init: create object subdirectories"
	@echo "compile: build program"
	@echo "all: init and compile (default)"
	@echo "rebuild: force to rebuild all"
	@echo "install: install program into target directory"
	@echo "uninstall: remove program from target directory"
	@echo "clean: remove all objects and program"
	@echo "realclean: clean and uninstall"
	@echo "info: display program directory pathes"
	@echo "help: display this help menu "
 
