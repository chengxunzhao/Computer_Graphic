TARGET     =  graphic.out

LOCAL_PATH = $(shell pwd)

SRC_PATH   = $(LOCAL_PATH)/src
LOG_PATH   = $(LOCAL_PATH)/log

INCLUDES  := -I$(SRC_PATH)/tgaimage \
			 -I$(SRC_PATH)/geometry \
			 -I$(SRC_PATH)/draw \
			 -I$(SRC_PATH)/model \
			 -I$(SRC_PATH) \
			 -I$(LOG_PATH)

SRC_CS    := $(wildcard $(SRC_PATH)/*.c)
SRC_CCS   := $(wildcard $(SRC_PATH)/*.cc)
SRC_CXXS  := $(wildcard $(SRC_PATH)/*.cpp)

SRC_CS    += $(wildcard $(SRC_PATH)/tgaimage/*.c)
SRC_CCS   += $(wildcard $(SRC_PATH)/tgaimage/*.cc)
SRC_CXXS  += $(wildcard $(SRC_PATH)/tgaimage/*.cpp)

SRC_CS    += $(wildcard $(SRC_PATH)/geometry/*.c)
SRC_CCS   += $(wildcard $(SRC_PATH)/geometry/*.cc)
SRC_CXXS  += $(wildcard $(SRC_PATH)/geometry/*.cpp)

SRC_CS    += $(wildcard $(SRC_PATH)/draw/*.c)
SRC_CCS   += $(wildcard $(SRC_PATH)/draw/*.cc)
SRC_CXXS  += $(wildcard $(SRC_PATH)/draw/*.cpp)

SRC_CS    += $(wildcard $(SRC_PATH)/model/*.c)
SRC_CCS   += $(wildcard $(SRC_PATH)/model/*.cc)
SRC_CXXS  += $(wildcard $(SRC_PATH)/model/*.cpp)

LOG_CS    := $(wildcard $(LOG_PATH)/*.c)
LOG_CCS   := $(wildcard $(LOG_PATH)/*.cc)
LOG_CXXS  := $(wildcard $(LOG_PATH)/*.cpp)

OBJECT_PATH := $(LOCAL_PATH)/.obj

OBJC      := $(patsubst $(SRC_PATH)/%.c,    $(OBJECT_PATH)/%.o, $(SRC_CS))
OBJC      += $(patsubst $(LOG_PATH)/%.c,    $(OBJECT_PATH)/%.o, $(LOG_CS))      

OBJCXX    := $(patsubst $(SRC_PATH)/%.cc,   $(OBJECT_PATH)/%.o, $(SRC_CCS))
OBJCXX    += $(patsubst $(LOG_PATH)/%.cc,   $(OBJECT_PATH)/%.o, $(LOG_CCS))

OBJCXX    += $(patsubst $(SRC_PATH)/%.cpp,  $(OBJECT_PATH)/%.o, $(SRC_CXXS))
OBJCXX    += $(patsubst $(LOG_PATH)/%.cpp,  $(OBJECT_PATH)/%.o, $(LOG_CXXS))

XLIBS     := -lfmt

CXX       := g++
CXXFLAGS := -std=c++17 -O2 -Wall

all:  dir_check $(TARGET)

$(TARGET): $(OBJC) $(OBJCXX)
	$(CXX) -o $@ $^ $(XLIBS)

dir_check:
	@test -d $(OBJECT_PATH) || mkdir -p $(OBJECT_PATH)

# C
$(OBJECT_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJECT_PATH)/%.o: $(LOG_PATH)/%.c
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

#CC
$(OBJECT_PATH)/%.o: $(SRC_PATH)/%.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJECT_PATH)/%.o: $(LOG_PATH)/%.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

#CPP
$(OBJECT_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJECT_PATH)/%.o: $(LOG_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


clean:
	rm -rf $(OBJECT_PATH) $(TARGET)