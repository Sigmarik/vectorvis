PYTHON = python3
CC = g++
PROFILER = valgrind

CPP_BASE_FLAGS = -I./ -I./lib/ -ggdb3 -std=c++2a -O3 -pie -fPIC -pthread			\
-Wall -Wextra -Weffc++				 	 											\
-Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations				\
-Wcast-align -Wchar-subscripts -Wconditionally-supported							\
-Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-nonliteral					\
-Wformat-security -Wformat-signedness -Wformat=2 -Wlogical-op						\
-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith		\
-Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo				\
-Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn				\
-Wsuggest-override -Wswitch-default													\
-Wsync-nand -Wundef -Wunreachable-code -Wuseless-cast								\
-Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers				\
-Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector					\
-Wstack-usage=8192 -Wno-unused-parameter -Wno-deprecated-declarations				\
-Wno-unused-variable -Wno-unused-function -Wno-conditionally-supported

CPP_SANITIZER_FLAGS = -fcheck-new 													\
-fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging		\
-fno-omit-frame-pointer -fPIE -fsanitize=address,bool,${strip 						\
}bounds,enum,float-cast-overflow,float-divide-by-zero,${strip 						\
}integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,${strip 		\
}returns-nonnull-attribute,shift,signed-integer-overflow,undefined,${strip 			\
}unreachable,vla-bound,vptr

BOLD = \\033[1m
STYLE_RESET = \\033[0m

RED 	= \\033[31m
GREEN 	= \\033[32m
YELLOW 	= \\033[33m
BLUE 	= \\033[34m
PINK 	= \\033[35m
CYAN 	= \\033[36m
GREY 	= \\033[37m

CPP_DEBUG_FLAGS = -D _DEBUG

CPPFLAGS = $(CPP_BASE_FLAGS)

BLD_FOLDER = build
ASSET_FOLDER = assets
LOGS_FOLDER = logs
PROJ_DIR = .

BUILD_LOG_NAME = $(LOGS_FOLDER)/build.log

BLD_VERSION = 0.1
BLD_PLATFORM = linux
BLD_TYPE = dev
BLD_FORMAT = .out
BLD_SUFFIX = _v$(BLD_VERSION)_$(BLD_TYPE)_$(BLD_PLATFORM)$(BLD_FORMAT)

BUILD_ERRLOG_FNAME = latest_build_err.log

SFML_ARGS = -lsfml-graphics -lsfml-window -lsfml-system -lglfw -lGL -lX11	\
	-lpthread -lXrandr -lXi -ldl -lGLEW

LIB_OBJECTS = $(shell cat lib.flist)

MAIN_NAME = main
MAIN_BLD_FULL_NAME = $(MAIN_NAME)$(BLD_SUFFIX)

MAIN_MAIN = src/main.cpp

MAIN_OBJECTS = $(LIB_OBJECTS) $(shell cat src.flist)

MAIN_DEPS = $(addprefix $(PROJ_DIR)/, $(MAIN_OBJECTS))

$(BLD_FOLDER)/$(MAIN_BLD_FULL_NAME): asset $(MAIN_MAIN) $(MAIN_DEPS)
	@mkdir -p $(BLD_FOLDER)
	@echo $(YELLOW)$(BOLD)Assembling $@$(STYLE_RESET)
	@$(CC) $(MAIN_MAIN) $(MAIN_OBJECTS) $(CPPFLAGS) $(SFML_ARGS) -o $(BLD_FOLDER)/$(MAIN_BLD_FULL_NAME)

PLUGINS = $(shell cat plugins.flist)

plugin: $(addprefix assets/plugins/, $(PLUGINS))

assets/plugins/%: plugins/%.so
	@rm -f $@.so
	@cp $^ $@.so

%.so: %.o $(LIB_OBJECTS)
	@echo $(YELLOW)$(BOLD)Assembling plugin $@$(STYLE_RESET)
	@$(CC) $(LIB_OBJECTS) $(CPPFLAGS) -shared $(firstword $^) -o $@

TEST_MAIN = ./gtest/gtest.o
LIBGTEST_MAIN = /usr/lib/libgtest_main.a
LIBGTEST = /usr/lib/libgtest.a

test: $(TEST_MAIN) $(MAIN_DEPS)
	@mkdir -p $(BLD_FOLDER)
	@mkdir -p $(BLD_FOLDER)/$(ASSET_FOLDER)
	@cp -r gtest/assets/. $(BLD_FOLDER)/$(ASSET_FOLDER)
	@$(CC)  $(TEST_MAIN) $(MAIN_DEPS) $(LIBGTEST_MAIN) $(LIBGTEST) $(SFML_ARGS) $(CPPFLAGS) -o $(BLD_FOLDER)/test_$(MAIN_BLD_FULL_NAME)
	@cd $(BLD_FOLDER) && exec ./test_$(MAIN_BLD_FULL_NAME)

run: asset $(BLD_FOLDER)/$(MAIN_BLD_FULL_NAME)
	@echo $(PINK)$(BOLD)Running the program$(STYLE_RESET)
	@cd $(BLD_FOLDER) && exec ./$(MAIN_BLD_FULL_NAME) $(ARGS)

debug: asset $(BLD_FOLDER)/$(MAIN_BLD_FULL_NAME)
	@cd $(BLD_FOLDER) && gdb ./$(MAIN_BLD_FULL_NAME) $(ARGS)

install-gtest:
	sudo apt-get install libgtest-dev
	sudo apt-get install cmake
	cd /usr/src/gtest && sudo cmake CMakeLists.txt
	cd /usr/src/gtest && sudo make
	cd /usr/src/gtest/lib && sudo cp *.a /usr/lib

asset: plugin
	@mkdir -p $(BLD_FOLDER)
	@mkdir -p $(ASSET_FOLDER)
	@cp -r $(ASSET_FOLDER)/. $(BLD_FOLDER)/$(ASSET_FOLDER)

%.o: %.cpp
	@echo $(YELLOW)Building file $^$(STYLE_RESET)
	@mkdir -p $(LOGS_FOLDER)
	@$(CC) $(CPPFLAGS) -c $^ -o $@ > $(BUILD_LOG_NAME)

LST_NAME = asm_listing.log
%.o: %.s
	@echo $(YELLOW)Building assembly file $^$(STYLE_RESET)
	@mkdir -p $(LOGS_FOLDER)
	@nasm -f elf64 -l $(LST_NAME) $^ -o $@ > $(BUILD_LOG_NAME)

clean:
	@echo $(RED)Deleting object files$(STYLE_RESET)
	@find . -type f -name "*.o" -delete
	@rm -rf ./$(LOGS_FOLDER)/$(BUILD_LOG_NAME)

rmbld:
	@echo $(RED)Removing buid folders$(STYLE_RESET)
	@rm -rf $(addsuffix .so, $(addprefix assets/plugins/, $(PLUGINS)))
	@rm -rf $(BLD_FOLDER)
	@rm -rf $(TEST_FOLDER)

rm: clean rmbld
	@echo $(RED)$(BOLD)Project successfully cleared$(STYLE_RESET)
