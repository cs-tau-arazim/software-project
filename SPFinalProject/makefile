CC = gcc
CPP = g++
#put your object files here
OBJS = main.o SPImageProc.o SPPoint.o 
#The executabel filename
EXEC = SPCBIR
INCLUDEPATH=/usr/local/lib/opencv-3.1.0/include/
LIBPATH=/usr/local/lib/opencv-3.1.0/lib/
LIBS=-lopencv_xfeatures2d -lopencv_features2d \
-lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_core


CPP_COMP_FLAG = -std=c++11 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

C_COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(C++) $(OBJS) -L$(LIBPATH) $(LIBS) -o $@
main.o: main.cpp main_aux.h SPImageProc.h
	$(C++) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
main_aux.o: main_aux.c main_aux.h KDTreeNode.h SPConfig.h
	$(C++) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
SPImageProc.o: SPImageProc.cpp SPImageProc.h SPConfig.h SPPoint.h SPLogger.h
	$(C++) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
SPConfig.o: SPConfig.c SPConfig.h SPLogger.h
	$(C++) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
SPLogger.o: SPLogger.c SPLogger.h
	$(C++) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
KDTreeNode.o: KDTreeNode.c KDTreeNode.h SPBPriorityQueue.h KDArray.h
	$(C++) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
KDArray.o: KDArray.c KDArray.h SPPoint.h
	$(C++) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
SPBPriorityQueue.o: SPBPriorityQueue.c SPBPriorityQueue.h SPList.h
	$(C++) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
SPList.o: SPList.c SPList.h SPListElement.h
	$(C++) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
SPListElement.o: SPListElement.c SPListElement.h
	$(C++) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
SPPoint.o: SPPoint.c SPPoint.h 
	$(CC) $(CPP_COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
