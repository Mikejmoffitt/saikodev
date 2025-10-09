#pragma once

; Basic colors:
BLACK           0x8000
MIDRED          0x4700
RED             0x4F00
MIDGREEN        0x2070
GREEN           0x20F0
MIDBLUE         0x1007
BLUE            0x100F
MIDYELLOW       0x6770
YELLOW          0x6FF0
MIDMAGENTA      0x5707
MAGENTA         0x5F0F
MIDCYAN         0x3077
CYAN            0x30FF
ORANGE          0x6F70
MIDGREY         0x7777
WHITE           0x7FFF



; Entry points
ENTRY0          0xC00402
ENTRY1          0xC00408
ENTRY2          0xC0040E
ENTRY3          0xC00414
ENTRY4          0xC0041A
ENTRY5          0xC00420
ENTRY6          0xC00426
ENTRY7          0xC0042C
ENTRY8          0xC00432

; System ROM calls:
SYSTEM_INT1         0xC00438
SYSTEM_INT2         0xC0043E
SYSTEM_RETURN       0xC00444
SYSTEM_IO           0xC0044A
CREDIT_CHECK        0xC00450
CREDIT_DOWN         0xC00456
READ_CALENDAR       0xC0045C   ; MVS only
CARD                0xC00468
CARD_ERROR          0xC0046E
HOW_TO_PLAY         0xC00474   ; MVS only
CALC_CHECKSUM_DEBUG 0xC0047A
FIX_CLEAR           0xC004C2
LSP_1ST             0xC004C8   ; Clear sprites
MESS_OUT            0xC004CE
CONTROLLER_SETUP    0xC004D4

; RAM locations:

; Memory Card
BIOS_CARD_COMMAND 0x10FDC4
BIOS_CARD_MODE 0x10FDC5
BIOS_CARD_ANSWER 0x10FDC6
BIOS_CARD_START 0x10FDC8
BIOS_CARD_SIZE 0x10FDCC
BIOS_CARD_FCB 0x10FDCE
BIOS_CARD_SUB 0x10FDD0

; Internal
BIOS_SLOTCHECK 0x10FCEF
BIOS_COMPULSION_TIMER 0x10FDDA
BIOS_COMPULSION_FRAME_TIMER 0x10FDDB
BIOS_FRAME_COUNTER 0x10FE88
BIOS_BRAM_USED 0x10FEBF
BIOS_FRAME_SKIP 0x10FEE1
BIOS_INT1_SKIP 0x10FEE3
BIOS_INT1_FRAME_COUNTER 0x10FEE4

BIOS_SYSTEM_MODE  0x10FD80
BIOS_MVS_FLAG     0x10FD82
BIOS_COUNTRY_CODE 0x10FD83
BIOS_GAME_DIP     0x10FD84   ; Start of soft DIPs settings (up to $10FD93)

; Set by SYS_IO:
BIOS_P1STATUS   0x10FD94
BIOS_P1PREVIOUS 0x10FD95
BIOS_P1CURRENT  0x10FD96
BIOS_P1CHANGE   0x10FD97
BIOS_P1REPEAT   0x10FD98
BIOS_P1TIMER    0x10FD99

BIOS_P2STATUS   0x10FD9A
BIOS_P2PREVIOUS 0x10FD9B
BIOS_P2CURRENT  0x10FD9C
BIOS_P2CHANGE   0x10FD9D
BIOS_P2REPEAT   0x10FD9E
BIOS_P2TIMER    0x10FD99

BIOS_STATCURNT    0x10FDAC
BIOS_STATCHANGE   0x10FDAD
BIOS_USER_REQUEST 0x10FDAE
BIOS_USER_MODE    0x10FDAF
BIOS_START_FLAG   0x10FDB4
BIOS_MESS_POINT   0x10FDBE
BIOS_MESS_BUSY    0x10FDC2

; Player MOD states
BIOS_PLAYER_MOD_NEVER_PLAYED = 0
BIOS_PLAYER_MOD_PLAYING      = 1
BIOS_PLAYER_MOD_CONTINUE     = 2
BIOS_PLAYER_MOD_GAME_OVER    = 3

; Memory card:
BIOS_CRDF       0x10FDC4   ; Byte: function to perform when calling BIOSF_CRDACCESS
BIOS_CRDRESULT  0x10FDC6   ; Byte: 00 on success, else 80+ and encodes the error
BIOS_CRDPTR     0x10FDC8   ; Longword: pointer to read from/write to
BIOS_CRDSIZE    0x10FDCC   ; Word: how much data to read/write from/to card
BIOS_CRDNGH     0x10FDCE   ; Word: usually game NGH. Unique identifier for the game that owns the save file
BIOS_CRDFILE    0x10FDD0   ; Word: each NGH has up to 16 save files associated with

; Calendar, MVS only (in BCD):
BIOS_YEAR       0x10FDD2   ; Last 2 digits of year
BIOS_MONTH      0x10FDD3
BIOS_DAY        0x10FDD4
BIOS_WEEKDAY    0x10FDD5   ; Sunday = 0, Monday = 1 ... Saturday = 6
BIOS_HOUR       0x10FDD6   ; 24 hour time
BIOS_MINUTE     0x10FDD7
BIOS_SECOND     0x10FDD8

BIOS_SELECT_TIMER 0x10FDDA   ; Byte: game start countdown
BIOS_DEVMODE      0x10FE80   ; Byte: non-zero for developer mode

; Upload system ROM call:
BIOS_UPDEST     0x10FEF4   ; Longword
BIOS_UPSRC      0x10FEF8   ; Longword
BIOS_UPSIZE     0x10FEFC   ; Longword
BIOS_UPZONE     0x10FEDA   ; Byte: zone (0=PRG, 1=FIX, 2=SPR, 3=Z80, 4=PCM, 5=PAT)
BIOS_UPBANK     0x10FEDB   ; Byte: bank

SOUND_STOP      0xD00046

; Button definitions:
CNT_UP      = 0
CNT_DOWN    = 1
CNT_LEFT    = 2
CNT_RIGHT   = 3
CNT_A       = 4
CNT_B       = 5
CNT_C       = 6
CNT_D       = 7
CNT_START1  = 0
CNT_SELECT1 = 1
CNT_START2  = 2
CNT_SELECT2 = 3

; Structures
NEOHDR	struct
neostr	ds.b	8
ngh	ds.w	1
romsize	ds.l	1
dd_ptr	ds.l	1
dd_size	ds.w	1
bioslgo	ds.b	1  ; If set, BIOS shows the logo
lgotile	ds.b	1  ; Tile index for the logo
cfg_jp	ds.l	1  ; Pointer to JP config
cfg_us	ds.l	1  ; Pointer to US config
cfg_eu	ds.l	1  ; Pointer to EU config
NEOHDR	endstruct

; Macro for FIX address calculation
FIXOFFS	function x, y, (y+(x*$20))
FIXADDR	function x, y, ($7000 + FIXOFFS(x, y))
