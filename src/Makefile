#
# OMNeT++/OMNEST Makefile for cn
#
# This file was generated with the command:
#  opp_makemake -f --deep
#

# Name of target to be created (-o option)
TARGET = cn$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(QTENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)
#USERIF_LIBS = $(QTENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I. \
    -Iinet \
    -Iinet/common \
    -Iinet/common/figures \
    -Iinet/common/geometry \
    -Iinet/common/geometry/common \
    -Iinet/common/queue \
    -Iinet/mobility \
    -Iinet/mobility/base \
    -Iinet/mobility/contract \
    -Iinet/mobility/single \
    -Iinet/mobility/static \
    -Iinet/node \
    -Iinet/visualizer \
    -Iinet/visualizer/base \
    -Iinet/visualizer/contract \
    -Iinet/visualizer/mobility \
    -Iinet/visualizer/networknode

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS =

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc, .msg and .sm files
OBJS = \
    $O/inet/common/Compat.o \
    $O/inet/common/InitStages.o \
    $O/inet/common/ModuleAccess.o \
    $O/inet/common/PatternMatcher.o \
    $O/inet/common/figures/TrailFigure.o \
    $O/inet/common/geometry/common/CanvasProjection.o \
    $O/inet/common/geometry/common/Coord.o \
    $O/inet/common/geometry/common/CoordinateSystem.o \
    $O/inet/common/geometry/common/EulerAngles.o \
    $O/inet/common/geometry/common/Rotation.o \
    $O/inet/common/queue/Sink.o \
    $O/inet/mobility/base/MobilityBase.o \
    $O/inet/mobility/base/MovingMobilityBase.o \
    $O/inet/mobility/contract/IMobility.o \
    $O/inet/mobility/single/ThreeWayRandomMobility.o \
    $O/inet/mobility/static/StationaryMobility.o \
    $O/inet/visualizer/base/MobilityVisualizerBase.o \
    $O/inet/visualizer/base/NetworkNodeVisualizerBase.o \
    $O/inet/visualizer/base/VisualizerBase.o \
    $O/inet/visualizer/mobility/MobilityCanvasVisualizer.o \
    $O/inet/visualizer/networknode/NetworkNodeCanvasVisualization.o \
    $O/inet/visualizer/networknode/NetworkNodeCanvasVisualizer.o

# Message files
MSGFILES =

# SM files
SMFILES =

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)
SMCOPTS =

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders smheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

%_sm.cc %_sm.h: %.sm
	$(qecho) SMC: $<
	$(Q)$(SMC) -c++ -suffix cc $(SMCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

smheaders: $(SMFILES:.sm=_sm.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f cn cn.exe libcn.so libcn.a libcn.dll libcn.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h ./*_sm.cc ./*_sm.h
	$(Q)-rm -f inet/*_m.cc inet/*_m.h inet/*_sm.cc inet/*_sm.h
	$(Q)-rm -f inet/common/*_m.cc inet/common/*_m.h inet/common/*_sm.cc inet/common/*_sm.h
	$(Q)-rm -f inet/common/figures/*_m.cc inet/common/figures/*_m.h inet/common/figures/*_sm.cc inet/common/figures/*_sm.h
	$(Q)-rm -f inet/common/geometry/*_m.cc inet/common/geometry/*_m.h inet/common/geometry/*_sm.cc inet/common/geometry/*_sm.h
	$(Q)-rm -f inet/common/geometry/common/*_m.cc inet/common/geometry/common/*_m.h inet/common/geometry/common/*_sm.cc inet/common/geometry/common/*_sm.h
	$(Q)-rm -f inet/common/queue/*_m.cc inet/common/queue/*_m.h inet/common/queue/*_sm.cc inet/common/queue/*_sm.h
	$(Q)-rm -f inet/mobility/*_m.cc inet/mobility/*_m.h inet/mobility/*_sm.cc inet/mobility/*_sm.h
	$(Q)-rm -f inet/mobility/base/*_m.cc inet/mobility/base/*_m.h inet/mobility/base/*_sm.cc inet/mobility/base/*_sm.h
	$(Q)-rm -f inet/mobility/contract/*_m.cc inet/mobility/contract/*_m.h inet/mobility/contract/*_sm.cc inet/mobility/contract/*_sm.h
	$(Q)-rm -f inet/mobility/single/*_m.cc inet/mobility/single/*_m.h inet/mobility/single/*_sm.cc inet/mobility/single/*_sm.h
	$(Q)-rm -f inet/mobility/static/*_m.cc inet/mobility/static/*_m.h inet/mobility/static/*_sm.cc inet/mobility/static/*_sm.h
	$(Q)-rm -f inet/node/*_m.cc inet/node/*_m.h inet/node/*_sm.cc inet/node/*_sm.h
	$(Q)-rm -f inet/visualizer/*_m.cc inet/visualizer/*_m.h inet/visualizer/*_sm.cc inet/visualizer/*_sm.h
	$(Q)-rm -f inet/visualizer/base/*_m.cc inet/visualizer/base/*_m.h inet/visualizer/base/*_sm.cc inet/visualizer/base/*_sm.h
	$(Q)-rm -f inet/visualizer/contract/*_m.cc inet/visualizer/contract/*_m.h inet/visualizer/contract/*_sm.cc inet/visualizer/contract/*_sm.h
	$(Q)-rm -f inet/visualizer/mobility/*_m.cc inet/visualizer/mobility/*_m.h inet/visualizer/mobility/*_sm.cc inet/visualizer/mobility/*_sm.h
	$(Q)-rm -f inet/visualizer/networknode/*_m.cc inet/visualizer/networknode/*_m.h inet/visualizer/networknode/*_sm.cc inet/visualizer/networknode/*_sm.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES) $(SM_CC_FILES)  ./*.cc inet/*.cc inet/common/*.cc inet/common/figures/*.cc inet/common/geometry/*.cc inet/common/geometry/common/*.cc inet/common/queue/*.cc inet/mobility/*.cc inet/mobility/base/*.cc inet/mobility/contract/*.cc inet/mobility/single/*.cc inet/mobility/static/*.cc inet/node/*.cc inet/visualizer/*.cc inet/visualizer/base/*.cc inet/visualizer/contract/*.cc inet/visualizer/mobility/*.cc inet/visualizer/networknode/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/inet/common/Compat.o: inet/common/Compat.cc \
	inet/common/Compat.h
$O/inet/common/InitStages.o: inet/common/InitStages.cc \
	inet/common/Compat.h \
	inet/common/INETDefs.h \
	inet/common/InitStages.h \
	inet/features.h
$O/inet/common/ModuleAccess.o: inet/common/ModuleAccess.cc \
	inet/common/Compat.h \
	inet/common/INETDefs.h \
	inet/common/InitStages.h \
	inet/common/ModuleAccess.h \
	inet/features.h
$O/inet/common/PatternMatcher.o: inet/common/PatternMatcher.cc \
	inet/common/Compat.h \
	inet/common/INETDefs.h \
	inet/common/InitStages.h \
	inet/common/PatternMatcher.h \
	inet/features.h
$O/inet/common/figures/TrailFigure.o: inet/common/figures/TrailFigure.cc \
	inet/common/Compat.h \
	inet/common/INETDefs.h \
	inet/common/InitStages.h \
	inet/common/figures/TrailFigure.h \
	inet/features.h
$O/inet/common/geometry/common/CanvasProjection.o: inet/common/geometry/common/CanvasProjection.cc \
	inet/common/Compat.h \
	inet/common/INETDefs.h \
	inet/common/INETMath.h \
	inet/common/InitStages.h \
	inet/common/geometry/common/CanvasProjection.h \
	inet/common/geometry/common/Coord.h \
	inet/common/geometry/common/EulerAngles.h \
	inet/common/geometry/common/Rotation.h \
	inet/features.h
$O/inet/common/geometry/common/Coord.o: inet/common/geometry/common/Coord.cc \
	inet/common/Compat.h \
	inet/common/INETDefs.h \
	inet/common/INETMath.h \
	inet/common/InitStages.h \
	inet/common/geometry/common/Coord.h \
	inet/features.h
$O/inet/common/geometry/common/CoordinateSystem.o: inet/common/geometry/common/CoordinateSystem.cc \
	inet/common/Compat.h \
	inet/common/INETDefs.h \
	inet/common/INETMath.h \
	inet/common/InitStages.h \
	inet/common/geometry/common/Coord.h \
	inet/common/geometry/common/CoordinateSystem.h \
	inet/common/geometry/common/EulerAngles.h \
	inet/features.h
$O/inet/common/geometry/common/EulerAngles.o: inet/common/geometry/common/EulerAngles.cc \
	inet/common/Compat.h \
	inet/common/INETDefs.h \
	inet/common/INETMath.h \
	inet/common/InitStages.h \
	inet/common/geometry/common/EulerAngles.h \
	inet/features.h
$O/inet/common/geometry/common/Rotation.o: inet/common/geometry/common/Rotation.cc \
	inet/common/Compat.h \
	inet/common/INETDefs.h \
	inet/common/INETMath.h \
	inet/common/InitStages.h \
	inet/common/geometry/common/Coord.h \
	inet/common/geometry/common/EulerAngles.h \
	inet/common/geometry/common/Rotation.h \
	inet/features.h
$O/inet/common/queue/Sink.o: inet/common/queue/Sink.cc \
	inet/common/Compat.h \
	inet/common/INETDefs.h \
	inet/common/InitStages.h \
	inet/common/queue/Sink.h \
	inet/features.h
$O/inet/mobility/base/MobilityBase.o: inet/mobility/base/MobilityBase.cc \
	inet/common/Compat.h \
	inet/common/INETDefs.h \
	inet/common/INETMath.h \
	inet/common/InitStages.h \
	inet/common/ModuleAccess.h \
	inet/common/geometry/common/CanvasProjection.h \
	inet/common/geometry/common/Coord.h \
	inet/common/geometry/common/CoordinateSystem.h \
	inet/common/geometry/common/EulerAngles.h \
	inet/common/geometry/common/Rotation.h \
	inet/features.h \
	inet/mobility/base/MobilityBase.h \
	inet/mobility/contract/IMobility.h
$O/inet/mobility/base/MovingMobilityBase.o: inet/mobility/base/MovingMobilityBase.cc \
	inet/common/Compat.h \
	inet/common/INETDefs.h \
	inet/common/INETMath.h \
	inet/common/InitStages.h \
	inet/common/ModuleAccess.h \
	inet/common/geometry/common/CanvasProjection.h \
	inet/common/geometry/common/Coord.h \
	inet/common/geometry/common/EulerAngles.h \
	inet/common/geometry/common/Rotation.h \
	inet/features.h \
	inet/mobility/base/MobilityBase.h \
	inet/mobility/base/MovingMobilityBase.h \
	inet/mobility/contract/IMobility.h
$O/inet/mobility/contract/IMobility.o: inet/mobility/contract/IMobility.cc \
	inet/common/Compat.h \
	inet/common/INETDefs.h \
	inet/common/INETMath.h \
	inet/common/InitStages.h \
	inet/common/geometry/common/Coord.h \
	inet/common/geometry/common/EulerAngles.h \
	inet/features.h \
	inet/mobility/contract/IMobility.h
$O/inet/mobility/single/ThreeWayRandomMobility.o: inet/mobility/single/ThreeWayRandomMobility.cc \
	inet/common/Compat.h \
	inet/common/INETDefs.h \
	inet/common/INETMath.h \
	inet/common/InitStages.h \
	inet/common/ModuleAccess.h \
	inet/common/geometry/common/CanvasProjection.h \
	inet/common/geometry/common/Coord.h \
	inet/common/geometry/common/EulerAngles.h \
	inet/common/geometry/common/Rotation.h \
	inet/features.h \
	inet/mobility/base/MobilityBase.h \
	inet/mobility/base/MovingMobilityBase.h \
	inet/mobility/contract/IMobility.h \
	inet/mobility/single/ThreeWayRandomMobility.h
$O/inet/mobility/static/StationaryMobility.o: inet/mobility/static/StationaryMobility.cc \
	inet/common/Compat.h \
	inet/common/INETDefs.h \
	inet/common/INETMath.h \
	inet/common/InitStages.h \
	inet/common/ModuleAccess.h \
	inet/common/geometry/common/CanvasProjection.h \
	inet/common/geometry/common/Coord.h \
	inet/common/geometry/common/EulerAngles.h \
	inet/common/geometry/common/Rotation.h \
	inet/features.h \
	inet/mobility/base/MobilityBase.h \
	inet/mobility/contract/IMobility.h \
	inet/mobility/static/StationaryMobility.h
$O/inet/visualizer/base/MobilityVisualizerBase.o: inet/visualizer/base/MobilityVisualizerBase.cc \
	inet/common/Compat.h \
	inet/common/INETDefs.h \
	inet/common/INETMath.h \
	inet/common/InitStages.h \
	inet/common/ModuleAccess.h \
	inet/common/geometry/common/Coord.h \
	inet/common/geometry/common/EulerAngles.h \
	inet/features.h \
	inet/mobility/contract/IMobility.h \
	inet/visualizer/base/MobilityVisualizerBase.h \
	inet/visualizer/base/VisualizerBase.h
$O/inet/visualizer/base/NetworkNodeVisualizerBase.o: inet/visualizer/base/NetworkNodeVisualizerBase.cc \
	inet/common/Compat.h \
	inet/common/INETDefs.h \
	inet/common/INETMath.h \
	inet/common/InitStages.h \
	inet/common/ModuleAccess.h \
	inet/common/PatternMatcher.h \
	inet/common/geometry/common/Coord.h \
	inet/features.h \
	inet/visualizer/base/NetworkNodeVisualizerBase.h \
	inet/visualizer/base/VisualizerBase.h
$O/inet/visualizer/base/VisualizerBase.o: inet/visualizer/base/VisualizerBase.cc \
	inet/common/Compat.h \
	inet/common/INETDefs.h \
	inet/common/INETMath.h \
	inet/common/InitStages.h \
	inet/common/ModuleAccess.h \
	inet/common/geometry/common/Coord.h \
	inet/common/geometry/common/EulerAngles.h \
	inet/features.h \
	inet/mobility/contract/IMobility.h \
	inet/visualizer/base/VisualizerBase.h
$O/inet/visualizer/mobility/MobilityCanvasVisualizer.o: inet/visualizer/mobility/MobilityCanvasVisualizer.cc \
	inet/common/Compat.h \
	inet/common/INETDefs.h \
	inet/common/INETMath.h \
	inet/common/InitStages.h \
	inet/common/ModuleAccess.h \
	inet/common/PatternMatcher.h \
	inet/common/figures/TrailFigure.h \
	inet/common/geometry/common/CanvasProjection.h \
	inet/common/geometry/common/Coord.h \
	inet/common/geometry/common/EulerAngles.h \
	inet/common/geometry/common/Rotation.h \
	inet/features.h \
	inet/mobility/contract/IMobility.h \
	inet/visualizer/base/MobilityVisualizerBase.h \
	inet/visualizer/base/NetworkNodeVisualizerBase.h \
	inet/visualizer/base/VisualizerBase.h \
	inet/visualizer/mobility/MobilityCanvasVisualizer.h \
	inet/visualizer/networknode/NetworkNodeCanvasVisualization.h \
	inet/visualizer/networknode/NetworkNodeCanvasVisualizer.h
$O/inet/visualizer/networknode/NetworkNodeCanvasVisualization.o: inet/visualizer/networknode/NetworkNodeCanvasVisualization.cc \
	inet/common/Compat.h \
	inet/common/INETDefs.h \
	inet/common/InitStages.h \
	inet/features.h \
	inet/visualizer/networknode/NetworkNodeCanvasVisualization.h
$O/inet/visualizer/networknode/NetworkNodeCanvasVisualizer.o: inet/visualizer/networknode/NetworkNodeCanvasVisualizer.cc \
	inet/common/Compat.h \
	inet/common/INETDefs.h \
	inet/common/INETMath.h \
	inet/common/InitStages.h \
	inet/common/ModuleAccess.h \
	inet/common/PatternMatcher.h \
	inet/common/geometry/common/CanvasProjection.h \
	inet/common/geometry/common/Coord.h \
	inet/common/geometry/common/EulerAngles.h \
	inet/common/geometry/common/Rotation.h \
	inet/features.h \
	inet/visualizer/base/NetworkNodeVisualizerBase.h \
	inet/visualizer/base/VisualizerBase.h \
	inet/visualizer/networknode/NetworkNodeCanvasVisualization.h \
	inet/visualizer/networknode/NetworkNodeCanvasVisualizer.h

