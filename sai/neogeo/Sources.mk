#
# Toaplan GCU-based hardware.
#

SAI_TARGET := SAI_TARGET_NEOGEO

SAI_SOURCES_ASM := \
	$(SAI)/neogeo/vectors.a68 \
	$(SAI)/neogeo/header.a68 \
	$(SAI)/neogeo/security.a68 \
	$(SAI)/neogeo/boot.a68 \
	$(SAI)/boot/crt0.a68 \
	$(SAI)/boot/exceptions.a68 \
	$(SAI)/boot/memcheck.a68 \
	$(SAI)/neogeo/io.a68 \
	$(SAI)/neogeo/irq.a68 \
	$(SAI)/neogeo/pal.a68 \
	$(SAI)/neogeo/sysrom.a68 \
	$(SAI)/sai.a68 \
	$(SAI)/input.a68 \
	$(SAI)/palcmd.a68 \

SAI_SOURCES_C :=

SAI_SOURCES_CPP := 

SAI_OBJECTS_ASM := $(patsubst $(SAI)/%.a68,$(SAIOBJDIR)/%.o,$(SAI_SOURCES_ASM))
SAI_OBJECTS_C   := $(patsubst $(SAI)/%.c,$(SAIOBJDIR)/%.o,$(SAI_SOURCES_C))
SAI_OBJECTS_CPP := $(patsubst $(SAI)/%.cpp,$(SAIOBJDIR)/%.o,$(SAI_SOURCES_CPP))
