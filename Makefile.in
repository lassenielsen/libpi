#=====================================================================#
#                        Dynamic Parser Library                       #
# Makefile.in                                                         #
# This is the Makefile-template for DPL.                              #
# If the Makefile has been lost, copy this file to Makefile and type  #
# make config. This will generate a Makefile that is configured for   #
# your system. You can then use                                       #
# make [clean | config | build | install | uninstall].                #
# The standard use is:                                                 #
# make config; make build; sudo make install[; sudo make uninstall].  #
# - Lasse Nielsen.                                                    #
#=====================================================================#
name = libpi
version = 3.0.0
libname = $(name).so
libname_debug = $(name)_debug.so
#OS_LINUXlibname = $(name).so
#OS_LINUXlibname_debug = $(name)_debug.so
#OS_MAClibname = $(name).dylib
#OS_MAClibname_debug = $(name)_debug.dylib
#OS_LINUXlibversion = .$(version)
#OS_MAClibversion =
COMMENT = OS_
OS_AUTO = $(shell uname -s)

compiler = g++
ctags = ctags
opt = -O3
opt_debug = -g -DPIDEBUG
args = -fPIC $(opt) -I./include/
args_debug = -fPIC $(opt_debug) -I./include/
#OS_MAClibs = 
#OS_LINUXlibs = -lrt -lgmp
#OS_LINUXlibs_debug = -lrt -lgmp

library_objects = \
  objects/message.o \
  objects/value.o \
  objects/channel.o \
  objects/session.o \
  objects/channel_fifo.o \
  objects/session_fifo.o \
  objects/channel_mq.o \
  objects/session_mq.o \
#  objects/channel_tcp.o \
#  objects/session_tcp.o \

library_objects_debug = \
  objects_debug/message.o \
  objects_debug/value.o \
  objects_debug/channel.o \
  objects_debug/session.o \
  objects_debug/channel_fifo.o \
  objects_debug/session_fifo.o \
  objects_debug/channel_mq.o \
  objects_debug/session_mq.o \
#  objects_debug/channel_tcp.o \
#  objects_debug/session_tcp.o \

default:
	@echo "Use make config, make build, sudo make install, make clean and if you don't like it sudo make uninstall."

all: config build install clean

phony: default config build install uninstall clean tags package deb

config:
ifeq ($(OS),linux)
	sed -e "s/#$(COMMENT)LINUX//g" Makefile.in > Makefile
else
ifeq ($(OS),osx)
	sed -e "s/#$(COMMENT)MAC//g" Makefile.in > Makefile
else
ifeq ($(OS_AUTO),Linux)
	sed -e "s/#$(COMMENT)LINUX//g" Makefile.in > Makefile
else
ifeq ($(OS_AUTO),Darwin)
	sed -e "s/#$(COMMENT)MAC//g" Makefile.in > Makefile
else
	@echo "Unable to recognize os $(OS_AUTO)."
	@echo "If linux is used please run"
	@echo "OS=linux make config."
	@echo "If OS X is used please run"
	@echo "OS=osx make config."
endif
endif
endif
endif

build: $(libname)$(libversion) $(libname_debug)$(libversion)

include/$(name)/config.hpp:
	@echo "Creating config header"
	@echo "#ifndef CONFIG_DPL" > include/$(name)/config.hpp
	@echo "#define CONFIG_DPL" >> include/$(name)/config.hpp
	@echo "#include <string>" >> include/$(name)/config.hpp
#OS_MAC	@echo "#define OS_X" >> include/$(name)/config.hpp
#OS_LINUX	@echo "#define OS_LINUX" >> include/$(name)/config.hpp
	@echo "#endif" >> include/$(name)/config.hpp

install: $(libname)$(libversion) $(libname_debug)$(libversion)
	@echo "Copying library"
	cp $(libname)$(libversion) /usr/lib/
	cp $(libname_debug)$(libversion) /usr/lib/
#OS_LINUX	ln -f -s /usr/lib/$(libname)$(libversion) /usr/lib/$(libname)
#OS_LINUX	ln -f -s /usr/lib/$(libname_debug)$(libversion) /usr/lib/$(libname_debug)
	@echo "Copying include-files"
	mkdir -p /usr/include/$(name)
	cp include/$(name)/*.hpp /usr/include/$(name)/
	chmod -R a+rx /usr/include/$(name)
#OS_LINUX	@echo "Reindexing libraries"
#OS_LINUX	ldconfig -n /usr/lib

uninstall:
	@echo "Removing library"
	rm -f /usr/lib/$(libname)*
	rm -f /usr/lib/$(libname_debug)*
	@echo "Removing include-files"
	rm -Rf /usr/include/$(name)
#OS_LINUX	@echo "Reindexing libraries"
#OS_LINUX	ldconfig -n /usr/lib

clean:
	touch clean~
	touch packages
	touch debs
	touch include/$(name)/config.hpp
	touch $(libname)$(libversion)
	touch $(libname_debug)$(libversion)
	rm *~
	rm -Rf packages
	rm -Rf debs
	rm -Rf objects
	rm -Rf objects_debug
	rm include/$(name)/config.hpp
	rm $(libname)$(libversion)
	rm $(libname_debug)$(libversion)
	cp Makefile.in Makefile

package:
	git clone . ./$(name)-$(version)
	rm -Rf ./$(name)-$(version)/.git
	mkdir -p packages/
	tar -czf packages/$(name)-$(version).tgz $(name)-$(version)
	rm -Rf $(name)-$(version)

deb: $(libname)$(libversion) $(libname_debug)$(libversion)
	echo "Making DEB package"
	mkdir -p debs/
	echo "Making data folder"
	mkdir -p debs/$(name)_$(version)_i386
	mkdir -p debs/$(name)_$(version)_i386/usr
	mkdir -p debs/$(name)_$(version)_i386/usr/lib
	cp $(libname)$(libversion) debs/$(name)_$(version)_i386/usr/lib/
	cp $(libname)$(libversion_debug) debs/$(name)_$(version)_i386/usr/lib/
	mkdir -p debs/$(name)_$(version)_i386/usr/include
	cp -R include/$(name) debs/$(name)_$(version)_i386/usr/include/$(name)
	echo "Making control"
	mkdir -p debs/$(name)_$(version)_i386/DEBIAN
	echo "Package: libdpl"                                         > debs/$(name)_$(version)_i386/DEBIAN/control
	echo "Version: $(version)"                                    >> debs/$(name)_$(version)_i386/DEBIAN/control
	echo "Architecture: i386"                                     >> debs/$(name)_$(version)_i386/DEBIAN/control
	echo "Maintainer: Lasse Nielsen <lasse.nielsen.dk@gmail.com>" >> debs/$(name)_$(version)_i386/DEBIAN/control
	echo "Installed-Size: 1024"                                   >> debs/$(name)_$(version)_i386/DEBIAN/control
	echo "Pre-Depends: dpkg (>= 1.14.12ubuntu3)"                  >> debs/$(name)_$(version)_i386/DEBIAN/control
	echo "Depends: libgmp3-dev"                                   >> debs/$(name)_$(version)_i386/DEBIAN/control
	echo "Recommends: apims"                                      >> debs/$(name)_$(version)_i386/DEBIAN/control
	echo "Suggests: apims"                                        >> debs/$(name)_$(version)_i386/DEBIAN/control
	echo "Conflicts: "                                            >> debs/$(name)_$(version)_i386/DEBIAN/control
	echo "Replaces: "                                             >> debs/$(name)_$(version)_i386/DEBIAN/control
	echo "Provides: libpi"                                        >> debs/$(name)_$(version)_i386/DEBIAN/control
	echo "Section: library"                                       >> debs/$(name)_$(version)_i386/DEBIAN/control
	echo "Priority: optional"                                     >> debs/$(name)_$(version)_i386/DEBIAN/control
	echo "Homepage: http://www.thelas.dk"                         >> debs/$(name)_$(version)_i386/DEBIAN/control
	echo "Description: Library providing pi-calculus operations." >> debs/$(name)_$(version)_i386/DEBIAN/control
	echo "#!/bin/sh"                                               > debs/$(name)_$(version)_i386/DEBIAN/postinst
	echo "ldconfig -n /usr/lib"                                   >> debs/$(name)_$(version)_i386/DEBIAN/postinst
	chmod a+x debs/$(name)_$(version)_i386/DEBIAN/postinst
	echo "#!/bin/sh"                                               > debs/$(name)_$(version)_i386/DEBIAN/postrm
	echo "ldconfig -n /usr/lib"                                   >> debs/$(name)_$(version)_i386/DEBIAN/postrm
	chmod a+x debs/$(name)_$(version)_i386/DEBIAN/postrm
	echo "Finalizing package"
	dpkg -b debs/$(name)_$(version)_i386/
	rm -Rf debs/$(name)_$(version)_i386

$(libname)$(libversion): $(library_objects)
#OS_LINUX	$(compiler) -shared -Wl,-soname,$(libname).1 -o $(libname)$(libversion) $(library_objects) $(libs)
#OS_MAC	$(compiler) -dynamiclib -o $(libname) $(library_objects) $(libs)

$(libname_debug)$(libversion): $(library_objects_debug)
#OS_LINUX	$(compiler) -shared -Wl,-soname,$(libname_debug).1 -o $(libname_debug)$(libversion) $(library_objects_debug) $(libs_debug)
#OS_MAC	$(compiler) -dynamiclib -o $(libname) $(library_objects) $(libs_debug)

objects/%.o: source/%.cpp include/$(name)/*.hpp  include/$(name)/config.hpp
	mkdir -p objects
	$(compiler) -c source/$*.cpp $(args) -o objects/$*.o

objects_debug/%.o: source/%.cpp include/$(name)/*.hpp  include/$(name)/config.hpp
	mkdir -p objects_debug
	$(compiler) -c source/$*.cpp $(args_debug) -o objects_debug/$*.o

tags: $(name)/*.hpp $(name)/*.cpp
	$(ctags) -a -o ~/.ctags $(PWD)/include/$(name)/*.hpp $(PWD)/source/*.cpp

