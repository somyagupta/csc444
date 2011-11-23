include Makefile.o.dep
include Makefile.help.dep

CC     = g++
#CFLAGS = -Wall -pedantic -s -O2 -mwindows -I../../ttmath -DTTCALC_CONVERT
CFLAGS = -Wall -pedantic -s -O2 -mwindows -I../../ttmath -DTTMATH_DONT_USE_WCHAR
name   = ttcalc.exe

# the name of the help is also set in the html help workshop project file
helpname = ttcalc.chm

# the name of the setup is also set in the Inno Setup config file
setupname = ttcalc-setup.exe



all: ttcalc


%.o: %.cpp
	$(CC) -c $(CFLAGS) $<


ttcalc: $(name)


$(name): $(o)
	$(CC) -o $(name) $(CFLAGS) $(o) -lcomctl32 -lwininet


resource.o: resource.rc
	#windres -DTTCALC_CONVERT resource.rc resource.o
	windres resource.rc resource.o


help: ../help/$(helpname)


../help/$(helpname): $(helpsrc)
	Makefile.help.sh


setup: ../setup/$(setupname)


../setup/$(setupname): $(name) ../help/$(helpname) ../setup/innosetup.iss ../setup/COPYRIGHT ../COPYRIGHT ../CHANGELOG ../README
	iscc ../setup/innosetup.iss


depend:
	makedepend -Y. -I../../ttmath -f- *.cpp | sed "s/[\\]/\//g" > Makefile.cpp.dep
	echo -n "o = resource.o " > Makefile.o.dep
	ls -1 *.cpp | xargs -I foo echo -n foo " " | sed -E "s/([^\.]*)\.cpp[ ]/\1\.o/g" >> Makefile.o.dep
	echo -n "helpsrc = " > Makefile.help.dep
	ls -1 ../help/*.html ../help/*.css ../help/*.hhp ../help/*.hhk ../help/*.hhc | xargs -I foo echo -n foo " " >> Makefile.help.dep


clean:
	rm -f *.o
	rm -f $(name)
	rm -f ttcalcp.exe
	rm -f ../help/$(helpname)
	rm -f ../setup/$(setupname)


include Makefile.cpp.dep
