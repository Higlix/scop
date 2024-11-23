BUILD=build
INCLUDE_DIR_NAME=include
SOURCE_DIR_NAME=src
LIBRARY_DIR_NAME=lib
OBJECT_DIR_NAME=$(BUILD)/object

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
LDFLAGS = -L$(GLFW_LIB) -lglfw3 $(GLAD_FILE) $(FRAMEWORKS) 

CC = g++ -std=c++11 -stdlib=libc++ #-g

NAME=scop

all: $(OBJECT_DIR_NAME) $(NAME)

$(OBJECT_DIR_NAME):
	mkdir -p $@

$(OBJECT_DIR_NAME)/%.o: $(SOURCE_DIR_NAME)/%.cpp
	mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CXXFLAGS)

$(NAME): $(OBJECT_FILES)
	$(CC) -o $(BUILD)/$@ $(OBJECT_FILES) $(LDFLAGS)

clean:
	rm -rf $(OBJECT_DIR_NAME)

fclean: clean
	rm -rf $(BUILD)

re: fclean all

# all:
# 	$(CC) -I$(GLFW_INC) src/main.cpp -o test -L$(GLFW_LIB) -lglfw3 $(FRAMEWORKS)