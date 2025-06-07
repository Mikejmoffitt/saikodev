#
# Saikodev Megadrive / Genesis rules and sources.
#

SAI_TARGET := SAI_TARGET_CPS

SAI_SOURCES_ASM := \
	$(SAI)/boot/vectors.a68 \
	$(SAI)/cps/boot.a68 \
	$(SAI)/boot/crt0.a68 \
	$(SAI)/boot/exceptions.a68 \
	$(SAI)/boot/memcheck.a68 \
	$(SAI)/cps/io_cps2.a68 \
	$(SAI)/sai.a68 \
	$(SAI)/input.a68 \

SAI_SOURCES_C := \

SAI_SOURCES_CPP := 

SAI_OBJECTS_ASM := $(patsubst $(SAI)/%.a68,$(SAIOBJDIR)/%.o,$(SAI_SOURCES_ASM))
SAI_OBJECTS_C   := $(patsubst $(SAI)/%.c,$(SAIOBJDIR)/%.o,$(SAI_SOURCES_C))
SAI_OBJECTS_CPP := $(patsubst $(SAI)/%.cpp,$(SAIOBJDIR)/%.o,$(SAI_SOURCES_CPP))
