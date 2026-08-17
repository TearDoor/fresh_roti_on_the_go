# The binary this exercise builds
NAME := game

RAYLIB_DIR := third_party/raylib/src
RAYLIB_LIB := $(RAYLIB_DIR)/libraylib.a

# Compiler and the gauntlet's mandatory flags
CXX      := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++17 -I$(RAYLIB_DIR)
LDFLAGS  := -L$(RAYLIB_DIR) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# List every .cpp file in the exercise here
SRC_DIR := srcs
OBJ_DIR := objs
SRCS := main.cpp Game.cpp Gameplay.cpp
OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

# `make` / `make all`: build the binary
all: $(NAME)

$(NAME): $(OBJS) $(RAYLIB_LIB)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

$(RAYLIB_LIB):
	$(MAKE) -C $(RAYLIB_DIR) PLATFORM=PLATFORM_DESKTOP

# Compile each .cpp into a .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir $@

# `make clean`: remove object files
clean:
	rm -f $(OBJS)

# `make fclean`: remove objects AND the binary
fclean: clean
	rm -f $(NAME)

fclean-raylib: fclean
	$(MAKE) clean -C $(RAYLIB_DIR)

# `make re`: rebuild everything from scratch
re: fclean all

.PHONY: all clean fclean re
