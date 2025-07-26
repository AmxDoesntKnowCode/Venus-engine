# THOU SHALL NOT CHANGE THE MAKEFILE 

CXX = g++
CXXFLAGS = -std=c++17 -Isrc/include -IC:/Cpp-Libraries/GLFWlib/include -IC:/Cpp-Libraries/GLEWlib/include -IC:\Cpp-Libraries\imgui  
LDFLAGS = -LC:/Cpp-Libraries/GLFWlib/lib-mingw-w64 -LC:/Cpp-Libraries/GLEWlib/lib/Release/x64 -Lsrc/lib 
LIBS = -lglfw3 -lglew32 -lopengl32 -lgdi32 -luser32 -lkernel32 
RES = icon.rc  
EXE_NAME = main.exe
RM = cmd /C del /Q


# convert all forward slashes in object files and executable name to backslashes for windows commands
WIN_OBJS = $(subst /,\,$(OBJS))
WIN_EXE_NAME = $(subst /,\,$(EXE_NAME))

IMGUI_SRCS = \
	src/include/imgui/imgui.cpp \
	src/include/imgui/imgui_draw.cpp \
	src/include/imgui/imgui_widgets.cpp \
	src/include/imgui/imgui_tables.cpp \
	src/include/imgui/imgui_demo.cpp \
	src/include/imgui/backends/imgui_impl_glfw.cpp \
	src/include/imgui/backends/imgui_impl_opengl3.cpp \
	src/include/imgui/imgui_spectrum.cpp 


# them source files
SRCS = \
	main.cpp \
	res/shaders/shader.cpp \
	module.cpp \
	res/Events/events.cpp \
	res/ImGui_functions/ImGui_Func.cpp 
	#res/Handle/Models/model_import.cpp
	
	

# auto generate object file names from source files
OBJS = $(patsubst %.cpp, %.o, $(SRCS))
OBJS += $(patsubst %.cpp, %.o, $(IMGUI_SRCS)) 
OBJS += icon.o 

.PHONY: all clean run

# default 
all: $(EXE_NAME)
	./$(EXE_NAME) 
# rule to build the executable from object files
$(EXE_NAME): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) $(LIBS) -Werror -o $(EXE_NAME) 
	@echo "THOU SHALL COMPLETE THE BUILD"

# generic rule for compiling .cpp files into .o files
# this shit tells make how to create any .o file from its corresponding .cpp file
%.o: %.cpp
	@echo "compiling: $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# shit for compiling the resource file
icon.o: $(RES) 
	@echo "Compiling icon $<..." 
	windres $< -o $@ 


# shit to clean up compiled files
clean:
	@echo "zum cleaning up..."
	$(RM) $(WIN_OBJS) $(WIN_EXE_NAME)
	@echo "done."
