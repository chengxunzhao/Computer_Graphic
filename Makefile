TARGET     =  graphic.out

LOCAL_PATH = $(shell pwd)

TAG_PATH   = $(LOCAL_PATH)/tgaimage
GEO_PATH  = $(LOCAL_PATH)/geometry
SRC_PATH   = $(LOCAL_PATH)/src

INCLUDES  := -I$(TAG_PATH) \
			 -I$(SRC_PATH) \
			 -I$(GEO_PATH)

GEO_CS   := $(wildcard $(GEO_PATH)/*.c)
GEO_CCS  := $(wildcard $(GEO_PATH)/*.cc)
GEO_CXXS := $(wildcard $(GEO_PATH)/*.cpp)

TAG_CS    := $(wildcard $(TAG_PATH)/*.c)
TAG_CCS   := $(wildcard $(TAG_PATH)/*.cc)
TAG_CXXS  := $(wildcard $(TAG_PATH)/*.cpp)

SRC_CS    := $(wildcard $(SRC_PATH)/*.c)
SRC_CCS   := $(wildcard $(SRC_PATH)/*.cc)
SRC_CXXS  := $(wildcard $(SRC_PATH)/*.cpp)

OBJECT_PATH := $(LOCAL_PATH)/.obj

OBJC      := $(patsubst $(TAG_PATH)/%.c,  $(OBJECT_PATH)/%.o, $(TAG_CS))
OBJC      += $(patsubst $(SRC_PATH)/%.c,  $(OBJECT_PATH)/%.o, $(SRC_CS))
OBJC      += $(patsubst $(GEO_PATH)/%.c, $(OBJECT_PATH)/%.o, $(GEO_CS))

OBJCXX    := $(patsubst $(TAG_PATH)/%.cc,  $(OBJECT_PATH)/%.o, $(TAG_CCS))
OBJCXX    += $(patsubst $(SRC_PATH)/%.cc,  $(OBJECT_PATH)/%.o, $(SRC_CCS))
OBJCXX    += $(patsubst $(GEO_PATH)/%.cc, $(OBJECT_PATH)/%.o, $(GEO_CCS))

OBJCXX    += $(patsubst $(TAG_PATH)/%.cpp,  $(OBJECT_PATH)/%.o, $(TAG_CXXS))
OBJCXX    += $(patsubst $(SRC_PATH)/%.cpp,  $(OBJECT_PATH)/%.o, $(SRC_CXXS))
OBJCXX    += $(patsubst $(GEO_PATH)/%.cpp, $(OBJECT_PATH)/%.o, $(GEO_CXXS))

XLIBS     :=

CXX       := g++
CXXFLAGS := -std=c++17 -O2 -Wall

all:  dir_check $(TARGET)

$(TARGET): $(OBJC) $(OBJCXX)
	$(CXX) -o $@ $^ $(XLIBS)

dir_check:
	@test -d $(OBJECT_PATH) || mkdir -p $(OBJECT_PATH)

# C
$(OBJECT_PATH)/%.o: $(TAG_PATH)/%.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJECT_PATH)/%.o: $(SRC_PATH)/%.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJECT_PATH)/%.o: $(GEO_PATH)/%.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

#CC
$(OBJECT_PATH)/%.o: $(TAG_PATH)/%.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJECT_PATH)/%.o: $(SRC_PATH)/%.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJECT_PATH)/%.o: $(GEO_PATH)/%.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

#CPP
$(OBJECT_PATH)/%.o: $(TAG_PATH)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJECT_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJECT_PATH)/%.o: $(GEO_PATH)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJECT_PATH) $(TARGET)