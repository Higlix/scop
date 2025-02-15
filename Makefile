BUILD=build
INCLUDE_DIR_NAME=include
SOURCE_DIR_NAME=src
LIBRARY_DIR_NAME=lib
OBJECT_DIR_NAME=$(BUILD)/object

GLFW_BUILD = lib/glfw/build
GLFW_LIB = lib/glfw/build/src
GLFW_INC = lib/glfw/include/GLFW

GLAD_FILE = lib/glad/src/glad.c
GLAD_INC = lib/glad/include/glad
KHR_INC = lib/glad/include/KHR

FRAMEWORKS = -framework Cocoa \
			-framework OpenGL \
			-framework IOKit \
			-framework Carbon \
			-framework CoreVideo \
			-framework QuartzCore

SOURCE_FILES = $(shell find $(SOURCE_DIR_NAME) -name '*.cpp')
OBJECT_FILES = $(SOURCE_FILES:$(SOURCE_DIR_NAME)/%.cpp=$(OBJECT_DIR_NAME)/%.o)

CXXFLAGS = -I$(GLFW_INC) -I$(INCLUDE_DIR_NAME) -I$(GLAD_INC) -I$(KHR_INC)
LDFLAGS = -L$(GLFW_LIB) -lglfw3 $(GLAD_FILE)
OS := $(shell uname)

ifeq ($(OS),Darwin)  # macOS
    # macOS specific commands
	CC = g++ -std=c++11 -stdlib=libc++ #-g
	LDFLAGS += $(FRAMEWORKS)
else ifeq ($(OS),Linux)  # Linux
    # Linux specific commands
	CC = g++ -std=c++11 #-g

endif

NAME=scop

all: $(GLFW_BUILD) $(OBJECT_DIR_NAME) $(NAME)

$(GLFW_BUILD): 
	cmake -S lib/glfw -B $@
	make -C $@

$(OBJECT_DIR_NAME):
	mkdir -p $@

$(OBJECT_DIR_NAME)/%.o: $(SOURCE_DIR_NAME)/%.cpp
	mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CXXFLAGS)

$(NAME): $(OBJECT_FILES)
	$(CC) -o $(BUILD)/$@ $(OBJECT_FILES) $(LDFLAGS)
	
reset_lib:
	rm -rf $(GLFW_BUILD)

clean:
	rm -rf $(OBJECT_DIR_NAME)

fclean: clean
	rm -rf $(BUILD)

re: fclean all

# all:
# 	$(CC) -I$(GLFW_INC) src/main.cpp -o test -L$(GLFW_LIB) -lglfw3 $(FRAMEWORKS)