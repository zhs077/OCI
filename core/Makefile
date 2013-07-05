objects=main.o OCIConnection.o OCICursor.o OCIExecption.o
ocitest:$(objects)
	g++ -o ocitest $(objects) -L $ORACLE_HOME/lib -lclntsh
OCIConnection.o:OCIConnection.cpp ocidef.h
	g++ -c OCIConnection.cpp
OCICursor.o: OCICursor.cpp ocidef.h
	g++ -c OCICursor.cpp
OCIExecption.o:OCIExecption.cpp ocidef.h
	g++ -c OCIExecption.cpp
main.o:main.cpp ocidef.h
	g++ -c main.cpp
clean:
	rm -f *.o


