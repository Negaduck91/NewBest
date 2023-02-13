#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WORKDIR = %cd%

TARGET = Scaff
#CXX = clang++
#CFLAGS = -Werror=option-ignored
CXX = g++
CFLAGS = -Wall -fexceptions -Wpedantic -Wuninitialized -Wmissing-include-dirs -g 

$(TARGET): main.o BaseScaff.o Menus.o Utilities.o
	$(CXX) -o $@ main.cpp BaseScaff.cxx Menus.cxx  Utilities.cxx

main.o: main.cpp BaseScaff.hxx Logger.hxx Utilities.hxx
	$(CXX) $(CFLAGS) -c $<

BaseScaff.o: BaseScaff.cxx BaseScaff.hxx
	$(CXX) $(CFLAGS) -c $<
	
#Logger.o: Logger.cxx Logger.hxx
#	$(CXX) $(CFLAGS) -c $<
	
Menus.o: Menus.cxx Menus.hxx
	$(CXX) $(CFLAGS) -c $<

Utilities.o: Utilities.cxx Utilities.hxx
	$(CXX) $(CFLAGS) -c $<

clean:
	-$(RM) main.o BaseScaff.o