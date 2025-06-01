#
# Saikodev Megadrive / Genesis rules and sources.
#

SAI_TARGET=SAI_TARGET_MD

SAI_SOURCES_ASM := \
	$(SAI)/boot/md.a68 \
	$(SAI)/boot/crt0.a68 \
	$(SAI)/video/vdp.a68 \
	$(SAI)/io/md.a68 \
	$(SAI)/irq/md.a68 \
	$(SAI)/video/vdp_dma.a68 \
	$(SAI)/sound/psg.a68 \
	$(SAI)/pal/md.a68

SAI_SOURCES_C := \
	$(SAI)/video/vdp_dma_queue.c

SAI_SOURCES_CPP := 

SAI_OBJECTS_ASM := $(patsubst $(SAI)/%.a68,$(SAIOBJDIR)/%.o,$(SAI_SOURCES_ASM))
SAI_OBJECTS_C   := $(patsubst $(SAI)/%.c,$(SAIOBJDIR)/%.o,$(SAI_SOURCES_C))
SAI_OBJECTS_CPP := $(patsubst $(SAI)/%.cpp,$(SAIOBJDIR)/%.o,$(SAI_SOURCES_CPP))
