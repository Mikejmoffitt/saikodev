//
// Saikodev Neo-Geo system rom functions and variables.
//

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

#ifndef __ASSEMBLER__
#include <stdint.h>
#endif  // __ASSEMBLER__

#include "sai/neogeo/hw.h"
#include "sai/neogeo/reg.h"
#include "sai/macro.h"

// Country codes
SAI_ENUMSTART
SAI_ENUMNEXT(SAI_NEO_COUNTRYCODE_JAPAN)
SAI_ENUMNEXT(SAI_NEO_COUNTRYCODE_USA)
SAI_ENUMNEXT(SAI_NEO_COUNTRYCODE_ETC)
SAI_ENUMNEXT(SAI_NEO_COUNTRYCODE_COUNT)
SAI_ENUMEND

// User request values
SAI_ENUMSTART
SAI_ENUMNEXT(SAI_NEO_UREQ_INIT)
SAI_ENUMNEXT(SAI_NEO_UREQ_EYE_CATCH)
SAI_ENUMNEXT(SAI_NEO_UREQ_ATTRACT)
SAI_ENUMNEXT(SAI_NEO_UREQ_TITLE)
SAI_ENUMNEXT(SAI_NEO_UREQ_COUNT)
SAI_ENUMEND

// User mode
SAI_ENUMSTART
SAI_ENUMNEXT(SAI_NEO_USERMODE_INIT)
SAI_ENUMNEXT(SAI_NEO_USERMODE_ATTRACT)
SAI_ENUMNEXT(SAI_NEO_USERMODE_INGAME)
SAI_ENUMNEXT(SAI_NEO_USERMODE_COUNT)
SAI_ENUMEND

// Player mode
SAI_ENUMSTART
SAI_ENUMNEXT(SAI_NEO_PMODE_INACTIVE)
SAI_ENUMNEXT(SAI_NEO_PMODE_INGAME)
SAI_ENUMNEXT(SAI_NEO_PMODE_CONTINUE)
SAI_ENUMNEXT(SAI_NEO_PMODE_GAME_OVER)
SAI_ENUMNEXT(SAI_NEO_PMODE_COUNT)
SAI_ENUMEND

#define SAI_NEO_EYECATCH_SYSTEM 0
#define SAI_NEO_EYECATCH_GAME   1

//
// System Calls
//
#define SAI_NEO_SYSTEM_RESET       (SYSROM_BASE+0x402)
#define SAI_NEO_BUS_ERROR          (SYSROM_BASE+0x408)
#define SAI_NEO_ADDR_ERROR         (SYSROM_BASE+0x40E)
#define SAI_NEO_ILLEGAL_INST_ERROR (SYSROM_BASE+0x414)
#define SAI_NEO_PRIV_EXCEPTION     (SYSROM_BASE+0x41A)
#define SAI_NEO_TRACE_EXCEPTION    (SYSROM_BASE+0x420)
#define SAI_NEO_INT_ERROR          (SYSROM_BASE+0x432)

#define SAI_NEO_SYSTEM_INT1        (SYSROM_BASE+0x438)
#define SAI_NEO_SYSTEM_RETURN      (SYSROM_BASE+0x444)
#define SAI_NEO_SYSTEM_IO          (SYSROM_BASE+0x44A)
#define SAI_NEO_CREDIT_CHECK       (SYSROM_BASE+0x450)
#define SAI_NEO_CREDIT_DOWN        (SYSROM_BASE+0x456)
#define SAI_NEO_READ_CALENDAR      (SYSROM_BASE+0x45C)
#define SAI_NEO_SETUP_CALENDAR     (SYSROM_BASE+0x462)
#define SAI_NEO_CARD               (SYSROM_BASE+0x468)
#define SAI_NEO_CARD_ERROR         (SYSROM_BASE+0x46E)
#define SAI_NEO_HOW_TO_PLAY        (SYSROM_BASE+0x474)
#define SAI_NEO_CHECKSUM_DEBUG     (SYSROM_BASE+0x47A)

#define SAI_NEO_FIX_CLEAR          (SYSROM_BASE+0x4C2)

//
// Native player status struct as populated by SAI_NEO_SYSTEM_IO
//

SAI_ENUMSTART
SAI_ENUMNEXT(SAI_NEO_PLSTATUS_NO_CONNECTION)
SAI_ENUMNEXT(SAI_NEO_PLSTATUS_JOYPAD)
SAI_ENUMNEXT(SAI_NEO_PLSTATUS_EXTENDED_JOYPAD)
SAI_ENUMNEXT(SAI_NEO_PLSTATUS_MAHJONG)
SAI_ENUMNEXT(SAI_NEO_PLSTATUS_KEYBOARD)
SAI_ENUMNEXT(SAI_NEO_PLSTATUS_COUNT)
SAI_ENUMEND


#ifndef __ASSEMBLER__

typedef struct SaiNeoBiosInput
{
	uint8_t status;  // One of SAI_NEO_PLSTATUS_*
	uint8_t prev;
	uint8_t now;
	uint8_t pos;
	uint8_t rept_pos;  // Active edge, plus once every 8 frames.
	uint8_t rept_cnt;
} SaiNeoBiosInput;

#else

	.struct	0
SaiNeoBiosInput.status:        ds.b 1
SaiNeoBiosInput.prev:          ds.b 1
SaiNeoBiosInput.now:           ds.b 1
SaiNeoBiosInput.pos:           ds.b 1
SaiNeoBiosInput.rept_pos:      ds.b 1
SaiNeoBiosInput.rept_cnt:      ds.b 1
SaiNeoBiosInput.len:

#endif  // __ASSEMBLER__

//
// System / BIOS variables
//

#ifndef __ASSEMBLER__

extern uint8_t g_sai_neo_system_mode;
extern uint8_t g_sai_neo_sysret_function;
extern uint8_t g_sai_neo_mvs_flag;
extern uint8_t g_sai_neo_country_code;
extern uint8_t g_sai_neo_game_dip[16];
extern SaiNeoBiosInput g_sai_neo_input_pl[4];
extern uint8_t g_sai_neo_input_sys[2];
extern uint8_t g_sai_neo_user_request;
extern uint8_t g_sai_neo_user_mode_for_system;
extern uint32_t g_sai_neo_credit_dec;
extern uint16_t g_sai_neo_start_flag;
extern uint8_t g_sai_neo_player_mode[4];

extern void *g_sai_neo_mess_ptr;
extern uint16_t g_sai_neo_mess_busy;
extern uint8_t g_sai_neo_card_command;
extern uint8_t g_sai_neo_card_mode;
extern uint16_t g_sai_neo_card_answer;
extern void *g_sai_neo_card_start;
extern uint16_t g_sai_neo_card_size;
extern uint16_t g_sai_neo_card_fcb;
extern uint16_t g_sai_neo_card_sub;

extern char g_sai_neo_date_time[8];
extern uint8_t g_sai_neo_select_timer;

#else

	.extern g_sai_neo_system_mode
	.extern g_sai_neo_mvs_flag
	.extern g_sai_neo_country_code
	.extern g_sai_neo_game_dip
	.extern g_sai_neo_input_pl
	.extern g_sai_neo_input_sys
	.extern g_sai_neo_user_request
	.extern g_sai_neo_user_mode_for_system
	.extern g_sai_neo_credit_dec
	.extern g_sai_neo_start_flag
	.extern g_sai_neo_player_mode

	.extern g_sai_neo_mess_ptr
	.extern g_sai_neo_mess_busy
	.extern g_sai_neo_card_command
	.extern g_sai_neo_card_mode
	.extern g_sai_neo_card_answer
	.extern g_sai_neo_card_start
	.extern g_sai_neo_card_size
	.extern g_sai_neo_card_fcb
	.extern g_sai_neo_card_sub

	.extern g_sai_neo_date_time
	.extern g_sai_neo_select_timer

#endif  // __ASSEMBLER__

#ifndef __ASSEMBLER__

static inline void sai_neo_system_return(void)
{
	const void *addr = (const void *)(SAI_NEO_SYSTEM_RETURN);
	asm volatile ("movea.l %0, a0; jmp (a0)"
	              :
	              : "g"(addr)
	              :);
}

#endif  // __ASSEMBLER__

//
// Special RAM locations
//

#ifdef __cplusplus
}
#endif  // __cplusplus
