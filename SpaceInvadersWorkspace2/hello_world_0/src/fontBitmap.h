/*
 * fontBitmap.h
 *
 *  Created on: Oct 13, 2015
 *      Author: superman
 */

#ifndef FONTBITMAP_H_
#define FONTBITMAP_H_

#define packword5(b4,b3,b2,b1,b0)                                                 \
((b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )

#define FONT_WIDTH 5
#define FONT_HEIGHT 8
#define FONT_ROWS_OFFSET (FONT_HEIGHT + 1)
#define FONT_COLS_OFFSET (FONT_WIDTH + 1)

#define CHARACTER_OFFSET_UPPERCASE_A 0
#define CHARACTER_OFFSET_LOWERCASE_A 26
#define CHARACTER_OFFSET_NUMBER_0 26+26

static const int character_A_5x8[] = {
	packword5(0, 1, 1, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 1, 1, 1, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_B_5x8[] = {
	packword5(1, 1, 1, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 1, 1, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 1, 1, 1, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_C_5x8[] = {
	packword5(0, 1, 1, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 1, 1, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_D_5x8[] = {
	packword5(1, 1, 1, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 1, 1, 1, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_E_5x8[] = {
	packword5(1, 1, 1, 1, 1),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 1, 1, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 1, 1, 1, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_F_5x8[] = {
	packword5(1, 1, 1, 1, 1),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 1, 1, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_G_5x8[] = {
	packword5(0, 1, 1, 1, 1),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 1, 1, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 1, 1, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_H_5x8[] = {
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 1, 1, 1, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_I_5x8[] = {
	packword5(0, 1, 1, 1, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 1, 1, 1, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_J_5x8[] = {
	packword5(0, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 1, 1, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_K_5x8[] = {
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 1, 0),
	packword5(1, 1, 1, 0, 0),
	packword5(1, 0, 0, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_L_5x8[] = {
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 1, 1, 1, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_M_5x8[] = {
	packword5(1, 0, 0, 0, 1),
	packword5(1, 1, 0, 1, 1),
	packword5(1, 0, 1, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_N_5x8[] = {
	packword5(1, 0, 0, 0, 1),
	packword5(1, 1, 0, 0, 1),
	packword5(1, 0, 1, 0, 1),
	packword5(1, 0, 0, 1, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_O_5x8[] = {
	packword5(0, 1, 1, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 1, 1, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_P_5x8[] = {
	packword5(1, 1, 1, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 1, 1, 1, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_Q_5x8[] = {
	packword5(0, 1, 1, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 1, 0),
	packword5(0, 1, 1, 0, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_R_5x8[] = {
	packword5(1, 1, 1, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 1, 1, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_S_5x8[] = {
	packword5(0, 1, 1, 1, 1),
	packword5(1, 0, 0, 0, 0),
	packword5(0, 1, 1, 1, 0),
	packword5(0, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 1, 1, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_T_5x8[] = {
	packword5(1, 1, 1, 1, 1),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_U_5x8[] = {
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 1, 1, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_V_5x8[] = {
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 0, 1, 0),
	packword5(0, 1, 0, 1, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_W_5x8[] = {
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 1, 0, 1),
	packword5(1, 1, 0, 1, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_X_5x8[] = {
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 0, 1, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 1, 0, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_Y_5x8[] = {
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 0, 1, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_Z_5x8[] = {
	packword5(1, 1, 1, 1, 1),
	packword5(0, 0, 0, 0, 1),
	packword5(0, 0, 0, 1, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 1, 0, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 1, 1, 1, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_a_5x8[] = {
	packword5(0, 0, 0, 0, 0),
	packword5(0, 0, 0, 0, 0),
	packword5(0, 1, 1, 1, 0),
	packword5(0, 0, 0, 0, 1),
	packword5(0, 1, 1, 1, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 1, 1, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_b_5x8[] = {
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 1, 1, 0),
	packword5(1, 1, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 1, 1, 1, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_c_5x8[] = {
	packword5(0, 0, 0, 0, 0),
	packword5(0, 0, 0, 0, 0),
	packword5(0, 1, 1, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 1, 1, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_d_5x8[] = {
	packword5(0, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 1),
	packword5(0, 1, 1, 0, 1),
	packword5(1, 0, 0, 1, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 1, 1, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_e_5x8[] = {
	packword5(0, 0, 0, 0, 0),
	packword5(0, 0, 0, 0, 0),
	packword5(0, 1, 1, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 1, 1, 1, 1),
	packword5(1, 0, 0, 0, 0),
	packword5(0, 1, 1, 1, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_f_5x8[] = {
	packword5(0, 0, 1, 1, 0),
	packword5(0, 1, 0, 0, 0),
	packword5(1, 1, 1, 1, 0),
	packword5(0, 1, 0, 0, 0),
	packword5(0, 1, 0, 0, 0),
	packword5(0, 1, 0, 0, 0),
	packword5(0, 1, 0, 0, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_g_5x8[] = {
	packword5(0, 0, 0, 0, 0),
	packword5(0, 0, 0, 0, 0),
	packword5(0, 1, 1, 1, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 1, 1, 1),
	packword5(0, 0, 0, 0, 1),
	packword5(1, 1, 1, 1, 0)
};

static const int character_h_5x8[] = {
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 1, 1, 0),
	packword5(1, 1, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_i_5x8[] = {
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 0, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_j_5x8[] = {
	packword5(0, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 0),
	packword5(0, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 1, 1, 0)
};

static const int character_k_5x8[] = {
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 1, 0),
	packword5(1, 0, 1, 0, 0),
	packword5(1, 1, 0, 0, 0),
	packword5(1, 0, 1, 0, 0),
	packword5(1, 0, 0, 1, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_l_5x8[] = {
	packword5(0, 1, 0, 0, 0),
	packword5(0, 1, 0, 0, 0),
	packword5(0, 1, 0, 0, 0),
	packword5(0, 1, 0, 0, 0),
	packword5(0, 1, 0, 0, 0),
	packword5(0, 1, 0, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_m_5x8[] = {
	packword5(0, 0, 0, 0, 0),
	packword5(0, 0, 0, 0, 0),
	packword5(1, 1, 0, 1, 0),
	packword5(1, 0, 1, 0, 1),
	packword5(1, 0, 1, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_n_5x8[] = {
	packword5(0, 0, 0, 0, 0),
	packword5(0, 0, 0, 0, 0),
	packword5(1, 1, 1, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_o_5x8[] = {
	packword5(0, 0, 0, 0, 0),
	packword5(0, 0, 0, 0, 0),
	packword5(0, 1, 1, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 1, 1, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_p_5x8[] = {
	packword5(0, 0, 0, 0, 0),
	packword5(0, 0, 0, 0, 0),
	packword5(1, 0, 1, 1, 0),
	packword5(1, 1, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 1, 1, 1, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 0)
};

static const int character_q_5x8[] = {
	packword5(0, 0, 0, 0, 0),
	packword5(0, 0, 0, 0, 0),
	packword5(0, 1, 1, 0, 1),
	packword5(1, 0, 0, 1, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 1, 1, 1),
	packword5(0, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 1)
};

static const int character_r_5x8[] = {
	packword5(0, 0, 0, 0, 0),
	packword5(0, 0, 0, 0, 0),
	packword5(1, 0, 1, 1, 0),
	packword5(1, 1, 0, 0, 1),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_s_5x8[] = {
	packword5(0, 0, 0, 0, 0),
	packword5(0, 0, 0, 0, 0),
	packword5(0, 1, 1, 1, 1),
	packword5(1, 0, 0, 0, 0),
	packword5(0, 1, 1, 1, 0),
	packword5(0, 0, 0, 0, 1),
	packword5(1, 1, 1, 1, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_t_5x8[] = {
	packword5(0, 0, 1, 0, 0),
	packword5(0, 1, 1, 1, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 0, 1, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_u_5x8[] = {
	packword5(0, 0, 0, 0, 0),
	packword5(0, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 1, 1, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_v_5x8[] = {
	packword5(0, 0, 0, 0, 0),
	packword5(0, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 0, 1, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_w_5x8[] = {
	packword5(0, 0, 0, 0, 0),
	packword5(0, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 1, 0, 1),
	packword5(1, 0, 1, 0, 1),
	packword5(0, 1, 1, 1, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_x_5x8[] = {
	packword5(0, 0, 0, 0, 0),
	packword5(0, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 0, 1, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 1, 0, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_y_5x8[] = {
	packword5(0, 0, 0, 0, 0),
	packword5(0, 0, 0, 0, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 1, 1, 1),
	packword5(0, 0, 0, 0, 1),
	packword5(1, 1, 1, 1, 0)
};

static const int character_z_5x8[] = {
	packword5(0, 0, 0, 0, 0),
	packword5(0, 0, 0, 0, 0),
	packword5(1, 1, 1, 1, 1),
	packword5(0, 0, 0, 1, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 1, 0, 0, 0),
	packword5(1, 1, 1, 1, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_0_5x8[] = {
	packword5(0, 1, 1, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 1, 1),
	packword5(1, 0, 1, 0, 1),
	packword5(1, 1, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 1, 1, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_1_5x8[] = {
	packword5(0, 0, 1, 0, 0),
	packword5(0, 1, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(1, 1, 1, 1, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_2_5x8[] = {
	packword5(0, 1, 1, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 1),
	packword5(0, 0, 1, 1, 0),
	packword5(0, 1, 0, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 1, 1, 1, 1),
	packword5(0, 0, 0, 0, 0)
};

static const int character_3_5x8[] = {
	packword5(0, 1, 1, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 1),
	packword5(0, 0, 1, 1, 0),
	packword5(0, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 1, 1, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_4_5x8[] = {
	packword5(0, 0, 0, 1, 0),
	packword5(0, 0, 1, 1, 0),
	packword5(0, 1, 0, 1, 0),
	packword5(1, 0, 0, 1, 0),
	packword5(1, 1, 1, 1, 1),
	packword5(0, 0, 0, 1, 0),
	packword5(0, 0, 0, 1, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_5_5x8[] = {
	packword5(1, 1, 1, 1, 1),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 1, 1, 1, 0),
	packword5(0, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 1, 1, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_6_5x8[] = {
	packword5(0, 0, 1, 1, 0),
	packword5(0, 1, 0, 0, 0),
	packword5(1, 0, 0, 0, 0),
	packword5(1, 1, 1, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 1, 1, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_7_5x8[] = {
	packword5(1, 1, 1, 1, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 0, 0, 0, 1),
	packword5(0, 0, 0, 1, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 1, 0, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_8_5x8[] = {
	packword5(0, 1, 1, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 1, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 1, 1, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int character_9_5x8[] = {
	packword5(0, 1, 1, 1, 0),
	packword5(1, 0, 0, 0, 1),
	packword5(1, 0, 0, 0, 1),
	packword5(0, 1, 1, 1, 1),
	packword5(0, 0, 0, 0, 1),
	packword5(0, 0, 0, 1, 0),
	packword5(0, 1, 1, 0, 0),
	packword5(0, 0, 0, 0, 0)
};

static const int* characters[26*2+10] = {
	character_A_5x8,
	character_B_5x8,
	character_C_5x8,
	character_D_5x8,
	character_E_5x8,
	character_F_5x8,
	character_G_5x8,
	character_H_5x8,
	character_I_5x8,
	character_J_5x8,
	character_K_5x8,
	character_L_5x8,
	character_M_5x8,
	character_N_5x8,
	character_O_5x8,
	character_P_5x8,
	character_Q_5x8,
	character_R_5x8,
	character_S_5x8,
	character_T_5x8,
	character_U_5x8,
	character_V_5x8,
	character_W_5x8,
	character_X_5x8,
	character_Y_5x8,
	character_Z_5x8,
	character_a_5x8,
	character_b_5x8,
	character_c_5x8,
	character_d_5x8,
	character_e_5x8,
	character_f_5x8,
	character_g_5x8,
	character_h_5x8,
	character_i_5x8,
	character_j_5x8,
	character_k_5x8,
	character_l_5x8,
	character_m_5x8,
	character_n_5x8,
	character_o_5x8,
	character_p_5x8,
	character_q_5x8,
	character_r_5x8,
	character_s_5x8,
	character_t_5x8,
	character_u_5x8,
	character_v_5x8,
	character_w_5x8,
	character_x_5x8,
	character_y_5x8,
	character_z_5x8,
	character_0_5x8,
	character_1_5x8,
	character_2_5x8,
	character_3_5x8,
	character_4_5x8,
	character_5_5x8,
	character_6_5x8,
	character_7_5x8,
	character_8_5x8,
	character_9_5x8};

#endif /* FONTBITMAP_H_ */
