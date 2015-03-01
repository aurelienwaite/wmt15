
SHELL = /bin/sh

# If ROOT_DIR is not defined try to guess it here
ifndef ROOT_DIR
   export ROOT_DIR := $(shell pwd | sed 's/\/source\/.*//')
endif

export ROOT_SOURCE := $(ROOT_DIR)/source
include $(ROOT_SOURCE)/Make_aux

#------------------------------------------------------------------------------------
# This part of the makefile only is used when executed from the target directory
# Make_aux remakes it from there (and set/unset THIS_IS_A_SRC_DIR)
#------------------------------------------------------------------------------------

ifndef THIS_IS_A_SRC_DIR


#-----------------------------------------------------------------
# TO DO: Complete the program name,
LINK1 := $(BIN_DIR)/mt-tokenise

#-----------------------------------------------------------------
all: $(LINK1)
#-----------------------------------------------------------------

$(LINK1):
	ln -sf $(SOURCE_DIR)/bin/mt-tokenise.bash $@
	#chmod ug+rx $(SOURCE_DIR)/bin/mt-tokenise.bash


#-----------------------------------------------------------------
clean:
	-rm -f $(SOURCE_DIR)/*~
	-rm -f $(SOURCE_DIR)/tokenitzador/*~
	-rm -f $(SOURCE_DIR)/engtok-1.0/*~

#-----------------------------------------------------------------
clean_all:
	-rm -f $(SOURCE_DIR)/*~
	-rm -f $(LINK1)
endif

