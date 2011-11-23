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

#ifndef headerfilelanguages
#define headerfilelanguages

#include <string>
#include <map>
#include <vector>

#include "bigtypes.h"


class Languages
{
public:
	
	enum GuiMsg
	{
		button_ok,
		button_cancel,
		message_box_caption,
		dialog_box_add_variable_caption,
		dialog_box_edit_variable_caption,
		dialog_box_add_variable_incorrect_name,
		dialog_box_add_variable_incorrect_value,
		dialog_box_add_variable_variable_exists,
		dialog_box_edit_variable_unknown_variable,
		dialog_box_delete_variable_confirm,
		dialog_box_variable_not_all_deleted,
		dialog_box_add_function_caption,
		dialog_box_edit_function_caption,
		dialog_box_add_function_function_exists,
		dialog_box_edit_function_unknown_function,
		dialog_box_delete_function_confirm,
		dialog_box_function_not_all_deleted,
		dialog_box_add_function_incorrect_name,
		list_variables_header_1,
		list_variables_header_2,
		list_functions_header_1,
		list_functions_header_2,
		list_functions_header_3,
		button_add,
		button_edit,
		button_delete,
		button_clear,
		tab_standard,
		tab_variables,
		tab_functions,
		tab_precision,
		tab_display,
		tab_convert,
		radio_precision_1,
		radio_precision_2,
		radio_precision_3,
		precision_1_info,
		precision_2_info,
		precision_3_info,
		overflow_during_printing,
		combo_rounding_none,
		combo_rounding_integer,
		combo_rounding_to_number,
		combo_rounding_after_comma,
		check_remove_zeroes,
		display_input,
		display_output,
		display_rounding,
		display_always_scientific,
		display_not_always_scientific,
		display_deg_rad_grad,
		display_grouping,
		display_grouping_none,
		display_grouping_space,
		display_grouping_digits,
		display_input_decimal_point,
		display_input_decimal_point_item1,
		display_output_decimal_point,
		display_param_sep,
		display_param_sep_item2,
		display_param_sep_item3,
		convert_type,
		convert_input,
		convert_output,
		convert_dynamic_output,
		menu_view,
		menu_edit,
		menu_help,
		menu_language,
		menu_view_new_window,
		menu_view_normal_view,
		menu_view_compact_view,
		menu_view_always_on_top,
		menu_view_pad,
		menu_view_lang_english,
		menu_view_lang_polish,
		menu_view_lang_spanish,
		menu_view_lang_danish,
		menu_view_lang_chinese,
		menu_view_lang_russian,
		menu_view_lang_swedish,
		menu_view_lang_italian,
		menu_view_lang_german,
		menu_view_close_program,
		menu_edit_undo,
		menu_edit_cut,
		menu_edit_copy,
		menu_edit_paste,
		menu_edit_del,
		menu_edit_paste_formula,
		menu_edit_copy_result,
		menu_edit_copy_both,
		menu_edit_select_all,
		menu_edit_bracket,
		menu_edit_swap,
		menu_help_help,
		menu_help_project_page,
		menu_help_check_update,
		menu_help_about,
		menu_update_available,
		cant_init_calculations,
		message_box_error_caption,
		cant_create_thread,
		cant_create_pad,
		cant_create_main_window,
		cant_init_common_controls,
		cant_find_help,
		cant_open_project_page,
		update_title,
		update_button_next,
		update_button_finish,
		update_button_cancel,
		update_check_at_startup,
		update_check_for_info,
		update_is_new_version,
		update_no_new_version1,
		update_no_new_version2,
		update_download_from,
		update_downloaded_info1,
		update_downloaded_info2,
		update_download_error,
		about_text,
		about_text_portable_version,
		about_text_exe_packer,
		about_box_title,
		about_box_button_close,
		display_as_scientific,
		pad_title,
		pad_menu_file,
		pad_menu_edit,
		pad_menu_file_new,
		pad_menu_file_open,
		pad_menu_file_saveas,
		pad_menu_file_close,
		pad_menu_edit_undo,
		pad_menu_edit_cut,
		pad_menu_edit_copy,
		pad_menu_edit_paste,
		pad_menu_edit_del,
		pad_menu_edit_select_all,
		cannot_open_file,
		cannot_save_file,
		file_too_long,
		other_error,
		unknown_error
	};

	// the first item must be with zero index 
	// and next items must be about one greater (0,1,2,3..)
	// (after conversion to 'int' we pass it into the std::vector)
	// lastnotexisting should be the last item
	// make sure SelectCurrentLanguage(const std::string & lang) is able to recognize a language
	enum Country
	{
		en = 0, pl, sp, da, chn, ru, swe, it, ger, lastnotexisting
	};


	
	
//	TTMathBigMax foo_return;

private:

	Country current_country;

	std::vector<std::map<ttmath::ErrorCode, std::string> > error_messages_tab;
	std::vector<std::map<GuiMsg,            std::string> > gui_messages_tab;

	void InsertErrorPair(ttmath::ErrorCode code, const char * message);
	void InitErrorMessagesTab();

	void InsertGuiPair(GuiMsg code, const char * message);
	void InitGuiMessagesTab();





public:

	Languages()
	{
		current_country = en;
//		foo_return = 0;
	}

	void SetCurrentLanguage(Country c)
	{
		// we are reading a language from *.ini file
		// and it is stored as a number

		if( int(c) < 0 || int(c) >= (int)Languages::lastnotexisting )
			c = en;

		current_country = c;
	}

	void SelectCurrentLanguage(const std::string & lang);


	Country GetCurrentLanguage()
	{
		return current_country;
	}


	/*
		we're using some methods from this class simultaneously in two threads,
		this is to be sure that everything is initialized before the second thread is started

		should be called before calling the second thread 
	*/
	void InitAll()
	{
		InitErrorMessagesTab();
		InitGuiMessagesTab();
	}


	/*
		long form of methods
		(with 'coutry')

		these methods you can call from the second thread without having to
		calling StopCalculating() and StartCalculating() from the thread controller

		the methods return only static value - remember to call InitAll() somewhere
		at the beginning e.g. in the first thread
	*/
	const char * ErrorMessage(Country country, ttmath::ErrorCode code);
	const char * GuiMessage(Country country, GuiMsg code);



	/*
		short form of methods 
		(without 'country' - it's taken from a private variable 'current_country')

		don't use these methods in the second thread - the parameter 'country' can be using
		by the first thread, although the 'country' is simply enum and should be atomic
		but in the future it may be changed
	*/
	const char * ErrorMessage(ttmath::ErrorCode code);
	const char * GuiMessage(GuiMsg code);


};


#endif
