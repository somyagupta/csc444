/*
 * This file is a part of TTCalc - a mathematical calculator
 * and is distributed under the (new) BSD licence.
 * Author: Tomasz Sowa <t.sowa@ttmath.org>
 */

/* 
 * Copyright (c) 2006-2011, Tomasz Sowa
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *    
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *    
 *  * Neither the name Tomasz Sowa nor the names of contributors to this
 *    project may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef headerfileresource
#define headerfileresource

/*!
	\file resource.h
    \brief the name tags for all controls
    
    There are the name tags of all the controls which we're using.
	Note that in resource.rc file we don't use these tags but
	there are only simple numbers there then if you change the numbers here
	you must change them in the resource.rc file as well.
*/

#define IDD_MAIN_DIALOG					101
#define IDD_ABOUT_DIALOG				200
#define IDD_UPDATE_DIALOG				210
#define IDD_DIALOG_ADD_VARIABLE			102
#define IDD_DIALOG_ADD_FUNCTION			103

#define IDD_DIALOG_STANDARD				110
#define IDD_DIALOG_VARIABLES			111
#define IDD_DIALOG_FUNCTIONS			112
#define IDD_DIALOG_PRECISION			113
#define IDD_DIALOG_DISPLAY				114
#define IDD_DIALOG_CONVERT				115

#define IDR_MENU						200
#define IDR_MENU2						201
#define IDC_TAB							1010
#define IDI_ICON2						104
#define IDI_ICON1						105
#define IDC_INPUT_EDIT					1000
#define IDC_OUTPUT_EDIT					1001


// all controls on the tabs should have different numbers
// because we're using the same window proc for all tabs
// (for all dialogs which will be kinds of the tab control)

// standard tab

#define IDC_BUTTON_ERROR				1070
#define IDC_BUTTON_ASIN					1071
#define IDC_BUTTON_ACOS					1072
#define IDC_BUTTON_ATAN					1073
#define IDC_BUTTON_ACOT					1074
#define IDC_BUTTON_SGN					1075
#define IDC_BUTTON_MOD					1076

// values from IDC_BUTTON_PRESS_0 to IDC_BUTTON_PRESS_9 should be with the step equal one
#define IDC_BUTTON_PRESS_0				1100
#define IDC_BUTTON_PRESS_1				1101
#define IDC_BUTTON_PRESS_2				1102
#define IDC_BUTTON_PRESS_3				1103
#define IDC_BUTTON_PRESS_4				1104
#define IDC_BUTTON_PRESS_5				1105
#define IDC_BUTTON_PRESS_6				1106
#define IDC_BUTTON_PRESS_7				1107
#define IDC_BUTTON_PRESS_8				1108
#define IDC_BUTTON_PRESS_9				1109
#define IDC_BUTTON_CLEAR				1110
#define IDC_BUTTON_FIRST_BRACKET		1111
#define IDC_BUTTON_LAST_BRACKET			1112
#define IDC_BUTTON_DIV					1113
#define IDC_BUTTON_MUL					1114
#define IDC_BUTTON_SUB					1115
#define IDC_BUTTON_ADD					1116
#define IDC_BUTTON_COMMA				1117
#define IDC_BUTTON_E					1118
#define IDC_BUTTON_PI					1119
#define IDC_BUTTON_SIN					1120
#define IDC_BUTTON_COS					1121
#define IDC_BUTTON_TAN					1122
#define IDC_BUTTON_COT					1123
#define IDC_BUTTON_LN					1124
#define IDC_BUTTON_LOG					1125
#define IDC_BUTTON_ABS					1126
#define IDC_BUTTON_FACTORIAL			1127
#define IDC_BUTTON_POWER				1128
#define IDC_BUTTON_INT					1129
#define IDC_BUTTON_ROUND				1130
#define IDC_BUTTON_EXP					1131
#define IDC_BUTTON_SQRT					1400
#define IDC_BUTTON_BRACKETS_ALL			1401
#define IDC_BUTTON_1DIV_BRACKETS_ALL	1402
#define IDC_BUTTON_SEMICOLON			1403
#define IDC_BUTTON_PERCENTAGE			1404
#define IDC_BUTTON_FLOOR				1405
#define IDC_BUTTON_CEIL					1406
#define IDC_BUTTON_MIN					1407
#define IDC_BUTTON_MAX					1408
#define IDC_BUTTON_AVG					1409
#define IDC_BUTTON_SUM					1410
#define IDC_BUTTON_ROOT					1411
#define IDC_BUTTON_GAMMA				1412


// variables tab
#define IDC_VARIABLES_LIST				1132
#define IDC_BUTTON_ADD_VARIABLE			1133
#define IDC_EDIT_VARIABLE_NAME			1134
#define IDC_EDIT_VARIABLE_VALUE			1135
#define IDC_STATIC_VARIABLE_NAME		1136
#define IDC_STATIC_VARIABLE_VALUE		1137
#define IDC_BUTTON_EDIT_VARIABLE		1138
#define IDC_BUTTON_DELETE_VARIABLE		1139

// functions tab
#define IDC_BUTTON_ADD_FUNCTION			1140
#define IDC_BUTTON_EDIT_FUNCTION		1141
#define IDC_BUTTON_DELETE_FUNCTION		1142
#define IDC_FUNCTIONS_LIST				1143
#define IDC_EDIT_FUNCTION_NAME			1144
#define IDC_EDIT_FUNCTION_VALUE			1145
#define IDC_COMBO_FUNCTION_PARAM		1146
#define IDC_STATIC_FUNCTION_NAME		1147
#define IDC_STATIC_FUNCTION_PARAM		1148
#define IDC_STATIC_FUNCTION_VALUE		1149

// precision tab
// there must be the following order:
// IDC_RADIO_PRECISION_2 = IDC_RADIO_PRECISION_1 + 1
// IDC_RADIO_PRECISION_3 = IDC_RADIO_PRECISION_2 + 1
#define IDC_RADIO_PRECISION_1			1150
#define IDC_RADIO_PRECISION_2			1151
#define IDC_RADIO_PRECISION_3			1152
#define IDC_LABEL_PRECISION_1_INFO		1153
#define IDC_LABEL_PRECISION_2_INFO		1154
#define IDC_LABEL_PRECISION_3_INFO		1155

// display tab
#define IDC_COMBO_DISPLAY_INPUT					1156
#define IDC_COMBO_DISPLAY_OUTPUT				1157
#define IDC_COMBO_DISPLAY_ROUNDING				1158
#define IDC_RADIO_DISPLAY_ALWAYS_SCIENTIFIC		1159
#define IDC_RADIO_DISPLAY_NOT_ALWAYS_SCIENTIFIC	1160
#define IDC_LABEL_DISPLAY_INPUT					1161
#define IDC_LABEL_DISPLAY_OUTPUT				1162
#define IDC_LABEL_DISPLAY_ROUNDING				1163
#define IDC_UPDOWN_DISPLAY_WHEN_SCIENTIFIC		1164
#define IDC_EDIT_DISPLAY_WHEN_SCIENTIFIC		1165
#define IDC_LABEL_DISPLAY_SCIENTIFIC			1166
#define IDC_LABEL_OUTPUT_DECIMAL_POINT			1167
#define IDC_COMBO_OUTPUT_DECIMAL_POINT			1168
#define IDC_CHECK_REMOVE_ZEROES					1169
#define IDC_LABEL_DISPLAY_DEG_RAD_GRAD			1170
#define IDC_COMBO_DEG_RAD_GRAD					1171
#define IDC_COMBO_DISPLAY_GROUPING				1172
#define IDC_COMBO_DISPLAY_GROUPING_DIGITS		1173
#define IDC_LABEL_DISPLAY_GROUPING				1174
#define IDC_COMBO_INPUT_DECIMAL_POINT			1175
#define IDC_LABEL_INPUT_DECIMAL_POINT			1176
#define IDC_LABEL_PARAM_SEPARATE				1177
#define IDC_COMBO_PARAM_SEPARATE				1178

// convert tab
#define IDC_LIST_UNIT_TYPE						1200
#define IDC_LIST_UNIT_INPUT1					1201
#define IDC_LIST_UNIT_INPUT2					1202
#define IDC_LIST_UNIT_OUTPUT1					1203
#define IDC_LIST_UNIT_OUTPUT2					1204
#define IDC_STATIC_UNIT_INPUT					1205
#define IDC_STATIC_UNIT_OUTPUT					1206
#define IDC_COMBO_DYNAMIC_OUTPUT				1207
#define IDC_EDIT_OUTPUT_INFO					1208
#define IDC_STATIC_UNIT_CONVERSION				1209


// menu
#define IDM_VIEW_INDEX					0
#define IDM_EDIT_INDEX					1
#define IDM_HELP_INDEX					2
#define IDM_UPDATE_AVAILABLE_INDEX		3

#define IDM_LANGUAGE_INDEX				8

#define IDM_NEW_WINDOW					40001
#define IDM_NORMAL_VIEW					40002
#define IDM_COMPACT_VIEW				40003
#define IDM_PAD							40004
#define IDM_ALWAYS_ON_TOP				40005
#define IDM_LANGUAGE_ENGLISH			40010
#define IDM_LANGUAGE_POLISH				40011
#define IDM_LANGUAGE_SPANISH			40012
#define IDM_LANGUAGE_DANISH				40013
#define IDM_LANGUAGE_CHINESE			40014
#define IDM_LANGUAGE_RUSSIAN			40015
#define IDM_LANGUAGE_SWEDISH			40016
#define IDM_LANGUAGE_ITALIAN			40017
#define IDM_LANGUAGE_GERMAN				40018
#define IDM_CLOSE_PROGRAM				40020

#define IDM_EDIT_UNDO					40030
#define IDM_EDIT_CUT					40031
#define IDM_EDIT_COPY					40032
#define IDM_EDIT_PASTE					40033
#define IDM_EDIT_DEL					40034
#define IDM_EDIT_COPY_RESULT			40035
#define IDM_EDIT_COPY_BOTH				40036
#define IDM_EDIT_PASTE_FORMULA			40037
#define IDM_EDIT_SELECT_ALL				40038
#define IDM_EDIT_BRACKET				40039
#define IDM_EDIT_SWAP					40040

#define IDM_HELP_HELP					40050
#define IDM_HELP_PROJECT_PAGE			40051
#define IDM_HELP_CHECK_UPDATE			40052
#define IDM_HELP_ABOUT					40053

#define IDM_UPDATE_AVAILABLE			40060

// pad menu
#define MENUPAD_FILE_INDEX				0
#define MENUPAD_EDIT_INDEX				1
#define MENUPAD_FILE_NEW				40500
#define MENUPAD_FILE_OPEN				40501
#define MENUPAD_FILE_SAVE				40502
#define MENUPAD_FILE_CLOSE				40503
#define MENUPAD_EDIT_UNDO				40510
#define MENUPAD_EDIT_CUT				40511
#define MENUPAD_EDIT_COPY				40512
#define MENUPAD_EDIT_PASTE				40513
#define MENUPAD_EDIT_DEL				40514
#define MENUPAD_EDIT_SELECTALL			40515


// about dialog
#define IDC_ABOUT_TEXT					1200
#define IDB_BITMAP_ABOUT				1201

// update dialog
#define IDC_UPDATE_INFO1				1300
#define IDC_UPDATE_INFO2				1301
#define IDC_PROGRESS					1302
#define IDC_BUTTON_NEXT					1303
#define IDC_BUTTON_CANCEL				1304
#define IDC_CHECK_UPDATE_STURTUP		1305

#endif
