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

#include "compileconfig.h"
#include "languages.h"
#include <string.h>



void Languages::SelectCurrentLanguage(const std::string & lang)
{
	// those names should be the same as in [Languages] section in innosetup.iss file

	const char * langtab[] = {
		"english",
		"polish",
		"spanish",
		"danish",
		"chinese",
		"russian",
		"swedish",
		"italian",
		"german"   };

	current_country = en; // default is english
	size_t len = sizeof(langtab) / sizeof(const char*);
	size_t i;

	for(i=0 ; i<len ; ++i)
	{
		if( strcmp(lang.c_str(), langtab[i]) == 0 )
		{
			SetCurrentLanguage((Languages::Country)i);
			break;
		}
	}
}


const char * Languages::ErrorMessage(Country country, ttmath::ErrorCode code)
{
const char * unk_err = "unknown error";

	if( error_messages_tab.empty() )
		InitErrorMessagesTab();

	unsigned int cid = static_cast<unsigned int>( country );
	if( cid >= error_messages_tab.size() )
		return unk_err;

	std::map<ttmath::ErrorCode, std::string>::const_iterator i;

	i = error_messages_tab[cid].find( code );
	if( i == error_messages_tab[cid].end() )
		return unk_err;

return i->second.c_str();
}


const char * Languages::ErrorMessage(ttmath::ErrorCode code)
{
	return ErrorMessage(current_country, code);
}



void Languages::InsertErrorPair(ttmath::ErrorCode code, const char * message)
{
	if( error_messages_tab.empty() )
		return;

	error_messages_tab.back().insert( std::make_pair(code, message) );
}


void Languages::InitErrorMessagesTab()
{
	error_messages_tab.push_back( std::map<ttmath::ErrorCode, std::string>() );

/*
	english messages relating to ttmath library
*/
	InsertErrorPair(ttmath::err_ok,"ok");
	InsertErrorPair(ttmath::err_nothing_has_read,"");
	InsertErrorPair(ttmath::err_unknown_character,"An unknown character");
	InsertErrorPair(ttmath::err_unexpected_final_bracket,"An unexpected final bracket");
	InsertErrorPair(ttmath::err_stack_not_clear,"An unknown character has left");
	InsertErrorPair(ttmath::err_unknown_variable,"An unknown variable");
	InsertErrorPair(ttmath::err_division_by_zero,"Division by zero");
	InsertErrorPair(ttmath::err_interrupt,"The calculating has been broken");
	InsertErrorPair(ttmath::err_overflow,"Overflow");
	InsertErrorPair(ttmath::err_unknown_function,"An unknown function");
	InsertErrorPair(ttmath::err_unknown_operator,"An unknown operator");
	InsertErrorPair(ttmath::err_unexpected_semicolon_operator,"An unexpected semicolon operator");
	InsertErrorPair(ttmath::err_improper_amount_of_arguments,"Improper amount of arguments");
	InsertErrorPair(ttmath::err_improper_argument,"Improper argument");
	InsertErrorPair(ttmath::err_unexpected_end,"Unexpected end");
	InsertErrorPair(ttmath::err_internal_error,"An internal error");

	InsertErrorPair(ttmath::err_incorrect_name,"Incorrect name of a variable or function");
	InsertErrorPair(ttmath::err_incorrect_value,"Incorrect value of a variable or function");
	InsertErrorPair(ttmath::err_variable_exists,"This variable already exists");
	InsertErrorPair(ttmath::err_variable_loop,"There's a recurrence between variables");
	InsertErrorPair(ttmath::err_functions_loop,"There's a recurrence between functions");
	InsertErrorPair(ttmath::err_must_be_only_one_value,"Variables or functions must return only one value");
	InsertErrorPair(ttmath::err_still_calculating,"Calculating...");
	InsertErrorPair(ttmath::err_percent_from,"Incorrectly used percentage operator");
	// other english translation: the percentage operator used in incorrect way


/*
	polish messages relating to ttmath library
*/

	error_messages_tab.push_back( std::map<ttmath::ErrorCode, std::string>() );

	InsertErrorPair(ttmath::err_ok,"ok");
	InsertErrorPair(ttmath::err_nothing_has_read,"");
	InsertErrorPair(ttmath::err_unknown_character,"Nieznany znak");
	InsertErrorPair(ttmath::err_unexpected_final_bracket,"Nieoczekiwany nawias zamykaj�cy");
	InsertErrorPair(ttmath::err_stack_not_clear,"Pozosta� nieznany znak");
	InsertErrorPair(ttmath::err_unknown_variable,"Nieznana zmienna");
	InsertErrorPair(ttmath::err_division_by_zero,"Dzielenie przez zero");
	InsertErrorPair(ttmath::err_interrupt,"Obliczenia zosta�y przerwane");
	InsertErrorPair(ttmath::err_overflow,"Przekroczony zakres");
	InsertErrorPair(ttmath::err_unknown_function,"Nieznana funkcja");
	InsertErrorPair(ttmath::err_unknown_operator,"Nieznany operator");
	InsertErrorPair(ttmath::err_unexpected_semicolon_operator,"Nieoczekiwany operator '�rednik'");
	InsertErrorPair(ttmath::err_improper_amount_of_arguments,"Niew�a�ciwa liczba argument�w");
	InsertErrorPair(ttmath::err_improper_argument,"Niew�a�ciwy argument");
	InsertErrorPair(ttmath::err_unexpected_end,"Nieoczekiwany koniec");
	InsertErrorPair(ttmath::err_internal_error,"B��d wewn�trzny programu!");

	InsertErrorPair(ttmath::err_incorrect_name,"Nieprawid�owa nazwa zmiennej lub funkcji");
	InsertErrorPair(ttmath::err_incorrect_value,"Nieprawid�owa warto�� zmiennej lub funkcji");
	InsertErrorPair(ttmath::err_variable_exists,"Ta zmienna juz istnieje");
	InsertErrorPair(ttmath::err_variable_loop,"Pomi�dzy zmiennymi zachodzi wywo�anie rekurencyjne");
	InsertErrorPair(ttmath::err_functions_loop,"Pomi�dzy funkcjami zachodzi wywo�anie rekurencyjne");
	InsertErrorPair(ttmath::err_must_be_only_one_value,"Zmienne albo funkcje mog� posiada� (zwraca�) tylko jedn� warto��");
	InsertErrorPair(ttmath::err_still_calculating,"Obliczanie...");
	InsertErrorPair(ttmath::err_percent_from,"Nieprawid�owo u�yty operator procentu");


/*
	spanish messages relating to ttmath library
*/

	error_messages_tab.push_back( std::map<ttmath::ErrorCode, std::string>() );
	
	InsertErrorPair(ttmath::err_ok,"ok");
	InsertErrorPair(ttmath::err_nothing_has_read,"");
	InsertErrorPair(ttmath::err_unknown_character,"Un caracter desconocido");
	InsertErrorPair(ttmath::err_unexpected_final_bracket,"corchete final inesperado");
	InsertErrorPair(ttmath::err_stack_not_clear,"caracter desconocido a la izquierda");
	InsertErrorPair(ttmath::err_unknown_variable,"Variable desconocida");
	InsertErrorPair(ttmath::err_division_by_zero,"Division por cero");
	InsertErrorPair(ttmath::err_interrupt,"El calculo se ha colapsado");
	InsertErrorPair(ttmath::err_overflow,"Overflow");
	InsertErrorPair(ttmath::err_unknown_function,"Funcion desconocida");
	InsertErrorPair(ttmath::err_unknown_operator,"Operador desconocida");
	InsertErrorPair(ttmath::err_unexpected_semicolon_operator,"punto y coma inesperado");
	InsertErrorPair(ttmath::err_improper_amount_of_arguments,"numero de argumentos incorrecto");
	InsertErrorPair(ttmath::err_improper_argument,"argumento incorrecto");
	InsertErrorPair(ttmath::err_unexpected_end,"Fin inexperado");
	InsertErrorPair(ttmath::err_internal_error,"Error interno");

	InsertErrorPair(ttmath::err_incorrect_name,"nombre incorrecto de variable o funcion");
	InsertErrorPair(ttmath::err_incorrect_value,"valor incorrecto de variable o funcion");
	InsertErrorPair(ttmath::err_variable_exists,"Variable ya existe");
	InsertErrorPair(ttmath::err_variable_loop,"Recurrencia entre variables");
	InsertErrorPair(ttmath::err_functions_loop,"Recurrencia entre funciones");
	InsertErrorPair(ttmath::err_must_be_only_one_value,"Variables o funciones solo pueden retornar un valor");
	InsertErrorPair(ttmath::err_still_calculating,"Calculando ...");
	InsertErrorPair(ttmath::err_percent_from,"Incorrectly used percentage operator");


/*
	danish messages relating to ttmath library
*/
	error_messages_tab.push_back( std::map<ttmath::ErrorCode, std::string>() );

	InsertErrorPair(ttmath::err_ok,"ok");
	InsertErrorPair(ttmath::err_nothing_has_read,"");
	InsertErrorPair(ttmath::err_unknown_character,"Et ukendt tegn");
	InsertErrorPair(ttmath::err_unexpected_final_bracket,"En uforventet endelig klasse");
	InsertErrorPair(ttmath::err_stack_not_clear,"Et ukendt tegn er v�k");
	InsertErrorPair(ttmath::err_unknown_variable,"En ukendt variabel");
	InsertErrorPair(ttmath::err_division_by_zero,"Division med nul");
	InsertErrorPair(ttmath::err_interrupt,"Beregningen er blevet afbrydt");
	InsertErrorPair(ttmath::err_overflow,"Overflow fejl");
	InsertErrorPair(ttmath::err_unknown_function,"En ukendt funktion");
	InsertErrorPair(ttmath::err_unknown_operator,"En ukendt operator");
	InsertErrorPair(ttmath::err_unexpected_semicolon_operator,"En ukendt semikolon operator");
	InsertErrorPair(ttmath::err_improper_amount_of_arguments,"Ukorrekt m�ngde af argumenter");
	InsertErrorPair(ttmath::err_improper_argument,"U�gte argumentation");
	InsertErrorPair(ttmath::err_unexpected_end,"Uforventet slutning");
	InsertErrorPair(ttmath::err_internal_error,"En intern fejl");

	InsertErrorPair(ttmath::err_incorrect_name,"Ukorrekt navn for en variabel eller function");
	InsertErrorPair(ttmath::err_incorrect_value,"Ukorrekt v�rdi for en variabel eller funktion");
	InsertErrorPair(ttmath::err_variable_exists,"Denne variabel eksisterer allerede");
	InsertErrorPair(ttmath::err_variable_loop,"Det er en gentagelse mellem variabler");
	InsertErrorPair(ttmath::err_functions_loop,"Der er en gentagelse mellem funktioner");
	InsertErrorPair(ttmath::err_must_be_only_one_value,"Variabler eller funktioner skal kun returnere �n v�rdi");
	InsertErrorPair(ttmath::err_still_calculating,"Regner...");
	InsertErrorPair(ttmath::err_percent_from,"Procent operatoren blev ikke brugt korrekt");


/*
	chinese messages relating to ttmath library
*/
	error_messages_tab.push_back( std::map<ttmath::ErrorCode, std::string>() );

	InsertErrorPair(ttmath::err_ok,"ȷ��");
	InsertErrorPair(ttmath::err_nothing_has_read,"");
	InsertErrorPair(ttmath::err_unknown_character,"δ֪�ַ�");
	InsertErrorPair(ttmath::err_unexpected_final_bracket,"������������");
	InsertErrorPair(ttmath::err_stack_not_clear,"��ջû�����");
	InsertErrorPair(ttmath::err_unknown_variable,"δ֪����");
	InsertErrorPair(ttmath::err_division_by_zero,"��������Ϊ��");
	InsertErrorPair(ttmath::err_interrupt,"���㱻�ж�");
	InsertErrorPair(ttmath::err_overflow,"�������");
	InsertErrorPair(ttmath::err_unknown_function,"δ֪����");
	InsertErrorPair(ttmath::err_unknown_operator,"δ֪�����");
	InsertErrorPair(ttmath::err_unexpected_semicolon_operator,"����ķֺ������");
	InsertErrorPair(ttmath::err_improper_amount_of_arguments,"�����������");
	InsertErrorPair(ttmath::err_improper_argument,"�������");
	InsertErrorPair(ttmath::err_unexpected_end,"����ĩ��");
	InsertErrorPair(ttmath::err_internal_error,"�ڲ�����");

	InsertErrorPair(ttmath::err_incorrect_name,"����ı�����������");
	InsertErrorPair(ttmath::err_incorrect_value,"����ı���ֵ����ֵ");
	InsertErrorPair(ttmath::err_variable_exists,"��������Ѿ�����");
	InsertErrorPair(ttmath::err_variable_loop,"����ѭ��");
	InsertErrorPair(ttmath::err_functions_loop,"����ѭ��");
	InsertErrorPair(ttmath::err_must_be_only_one_value,"�������������з���ֵ");
	InsertErrorPair(ttmath::err_still_calculating,"���ڼ���...");
	InsertErrorPair(ttmath::err_percent_from,"Incorrectly used percentage operator");


/*
	russian messages relating to ttmath library
*/
	error_messages_tab.push_back( std::map<ttmath::ErrorCode, std::string>() );

	InsertErrorPair(ttmath::err_ok,"ok");
	InsertErrorPair(ttmath::err_nothing_has_read,"");
	InsertErrorPair(ttmath::err_unknown_character,"����������� ������");
	InsertErrorPair(ttmath::err_unexpected_final_bracket,"����������� ����������� ������");
	InsertErrorPair(ttmath::err_stack_not_clear,"������� ����������� ������");
	InsertErrorPair(ttmath::err_unknown_variable,"����������� ����������");
	InsertErrorPair(ttmath::err_division_by_zero,"������ �� ���� ������");
	InsertErrorPair(ttmath::err_interrupt,"������� �������");
	InsertErrorPair(ttmath::err_overflow,"������������");
	InsertErrorPair(ttmath::err_unknown_function,"����������� �������");
	InsertErrorPair(ttmath::err_unknown_operator,"����������� ��������");
	InsertErrorPair(ttmath::err_unexpected_semicolon_operator,"����������� �������� � ;");
	InsertErrorPair(ttmath::err_improper_amount_of_arguments,"������������ ���������� ����������");
	InsertErrorPair(ttmath::err_improper_argument,"������������ ��������");
	InsertErrorPair(ttmath::err_unexpected_end,"����������� �����");
	InsertErrorPair(ttmath::err_internal_error,"���������� ������");

	InsertErrorPair(ttmath::err_incorrect_name,"������������ ��� ���������� ��� �������");
	InsertErrorPair(ttmath::err_incorrect_value,"������������ �������� ���������� ��� �������");
	InsertErrorPair(ttmath::err_variable_exists,"����� ���������� ��� ������");
	InsertErrorPair(ttmath::err_variable_loop,"���������� �����������");
	InsertErrorPair(ttmath::err_functions_loop,"������� �����������");
	InsertErrorPair(ttmath::err_must_be_only_one_value,"���������� ��� ������� ������ ���������� ������ ���� ��������");
	InsertErrorPair(ttmath::err_still_calculating,"������������...");
	InsertErrorPair(ttmath::err_percent_from,"������������ ������������� ��������� ��������");

/*
	swedish messages relating to ttmath library
*/
	error_messages_tab.push_back( std::map<ttmath::ErrorCode, std::string>() );

	InsertErrorPair(ttmath::err_ok,"ok");
	InsertErrorPair(ttmath::err_nothing_has_read,"");
	InsertErrorPair(ttmath::err_unknown_character,"Ett ok�nt tecken");
	InsertErrorPair(ttmath::err_unexpected_final_bracket,"En of�rv�ntat slutlig klass");
	InsertErrorPair(ttmath::err_stack_not_clear,"Ett ok�nt tecken saknas");
	InsertErrorPair(ttmath::err_unknown_variable,"En ok�nd variabel");
	InsertErrorPair(ttmath::err_division_by_zero,"Division med noll");
	InsertErrorPair(ttmath::err_interrupt,"Ber�kningen har avbrutits");
	InsertErrorPair(ttmath::err_overflow,"Overflow fel");
	InsertErrorPair(ttmath::err_unknown_function,"En ok�nd funktion");
	InsertErrorPair(ttmath::err_unknown_operator,"En ok�nd operator");
	InsertErrorPair(ttmath::err_unexpected_semicolon_operator,"En ok�nd semikolonoperator");
	InsertErrorPair(ttmath::err_improper_amount_of_arguments,"Felaktig m�ngd argument");
	InsertErrorPair(ttmath::err_improper_argument,"Ogiltig argumentation");
	InsertErrorPair(ttmath::err_unexpected_end,"Of�rv�ntat fel");
	InsertErrorPair(ttmath::err_internal_error,"Ett internt fel");

	InsertErrorPair(ttmath::err_incorrect_name,"Ogiltigt namn f�r en variabel eller funktion");
	InsertErrorPair(ttmath::err_incorrect_value,"Ogiltigt v�rde f�r en variabel eller funktion");
	InsertErrorPair(ttmath::err_variable_exists,"Denna variabel existerar redan");
	InsertErrorPair(ttmath::err_variable_loop,"Det �r en upprepepning mellan variabler");
	InsertErrorPair(ttmath::err_functions_loop,"Det �r en upprepning mellan funktioner");
	InsertErrorPair(ttmath::err_must_be_only_one_value,"Variabler eller funktioner skall kunna avge ett v�rde");
	InsertErrorPair(ttmath::err_still_calculating,"R�knar b�vrar...");
	InsertErrorPair(ttmath::err_percent_from,"Incorrectly used percentage operator");


/*
	italian traslation relating to ttmath library
*/
	error_messages_tab.push_back( std::map<ttmath::ErrorCode, std::string>() );

	InsertErrorPair(ttmath::err_ok,"ok");
	InsertErrorPair(ttmath::err_nothing_has_read,"");
	InsertErrorPair(ttmath::err_unknown_character,"Carattere sconosciuto");
	InsertErrorPair(ttmath::err_unexpected_final_bracket,"Parentesi finale inaspettata");
	InsertErrorPair(ttmath::err_stack_not_clear,"Carattere sconosciuto mancante");
	InsertErrorPair(ttmath::err_unknown_variable,"Variabile sconosciuta");
	InsertErrorPair(ttmath::err_division_by_zero,"Divisione per zero");
	InsertErrorPair(ttmath::err_interrupt,"Il calcolo e stato interrotto");
	InsertErrorPair(ttmath::err_overflow,"Overflow");
	InsertErrorPair(ttmath::err_unknown_function,"Funzione sconosciuta");
	InsertErrorPair(ttmath::err_unknown_operator,"Operatore sconosciuto");
	InsertErrorPair(ttmath::err_unexpected_semicolon_operator,"Operatore punto e virgola inaspettato");
	InsertErrorPair(ttmath::err_improper_amount_of_arguments,"Quantita non appropriata di argomenti");
	InsertErrorPair(ttmath::err_improper_argument,"Argomento inappropriato");
	InsertErrorPair(ttmath::err_unexpected_end,"Fine inaspettata");
	InsertErrorPair(ttmath::err_internal_error,"Errore interno");

	InsertErrorPair(ttmath::err_incorrect_name,"Nome di funzione o variabile non corretto");
	InsertErrorPair(ttmath::err_incorrect_value,"Valore di funzione o variabile non corretto");
	InsertErrorPair(ttmath::err_variable_exists,"Questa variabile esiste gia");
	InsertErrorPair(ttmath::err_variable_loop,"C'e una ricorrenza tra variabili");
	InsertErrorPair(ttmath::err_functions_loop,"C'e una ricorrenza tra funzioni");
	InsertErrorPair(ttmath::err_must_be_only_one_value,"Le variabili o le funzioni devono dare come risultato un solo valore");
	InsertErrorPair(ttmath::err_still_calculating,"Calcolo...");
	InsertErrorPair(ttmath::err_percent_from,"Operatore percentuale non correttamente utilizzato");


/*
	german messages relating to ttmath library
*/
	error_messages_tab.push_back( std::map<ttmath::ErrorCode, std::string>() );
	
	InsertErrorPair(ttmath::err_ok,"ok");
	InsertErrorPair(ttmath::err_nothing_has_read,"");
	InsertErrorPair(ttmath::err_unknown_character,"Unbekanntes Zeichen");
	InsertErrorPair(ttmath::err_unexpected_final_bracket,"Unerwartete schlie�ende Klammer");
	InsertErrorPair(ttmath::err_stack_not_clear,"Unbekanntes Zeichen ist �brig");
	InsertErrorPair(ttmath::err_unknown_variable,"Unbekannte Variable");
	InsertErrorPair(ttmath::err_division_by_zero,"Division durch Null");
	InsertErrorPair(ttmath::err_interrupt,"Berechnung unterbrochen");
	InsertErrorPair(ttmath::err_overflow,"�berlauf");
	InsertErrorPair(ttmath::err_unknown_function,"Unbekannte Funktion");
	InsertErrorPair(ttmath::err_unknown_operator,"Unbekannter Operator");
	InsertErrorPair(ttmath::err_unexpected_semicolon_operator,"Unerwarteter Semikolon-Operator");
	InsertErrorPair(ttmath::err_improper_amount_of_arguments,"Falsche Parameteranzahl");
	InsertErrorPair(ttmath::err_improper_argument,"Unpassender Parameter");
	InsertErrorPair(ttmath::err_unexpected_end,"Unerwartetes Ende");
	InsertErrorPair(ttmath::err_internal_error,"Interner Fehler");

	InsertErrorPair(ttmath::err_incorrect_name,"Ung�ltiger Name einer Variablen oder Funktion");
	InsertErrorPair(ttmath::err_incorrect_value,"Ung�ltiger Wert einer Variablen oder Funktion");
	InsertErrorPair(ttmath::err_variable_exists,"Diese Variable existiert bereits");
	InsertErrorPair(ttmath::err_variable_loop,"Rekursion zwischen Variablen gefunden");
	InsertErrorPair(ttmath::err_functions_loop,"Rekursion zwischen Funktionen gefunden");
	InsertErrorPair(ttmath::err_must_be_only_one_value,"Variablen und Funktionen d�rfen nur einen R�ckgabewert haben");
	InsertErrorPair(ttmath::err_still_calculating,"Berechnung l�uft...");
	InsertErrorPair(ttmath::err_percent_from,"Falsch angewandter Prozent-Operator");

}







// for gui messages




const char * Languages::GuiMessage(Country country, GuiMsg code)
{
const char * unk_msg = "unknown";

	if( gui_messages_tab.empty() )
		InitGuiMessagesTab();

	unsigned int cid = static_cast<unsigned int>( country );
	if( cid >= gui_messages_tab.size() )
		return unk_msg;

	std::map<GuiMsg, std::string>::const_iterator i;

	i = gui_messages_tab[cid].find( code );
	if( i == gui_messages_tab[cid].end() )
		return unk_msg;

return i->second.c_str();
}



const char * Languages::GuiMessage(GuiMsg code)
{
	return GuiMessage(current_country, code);
}



void Languages::InsertGuiPair(GuiMsg code, const char * message)
{
	if( gui_messages_tab.empty() )
		return;

	gui_messages_tab.back().insert( std::make_pair(code, message) );
}



void Languages::InitGuiMessagesTab()
{
/*
	english gui messages

	&    - (ampersand) used in menus or buttons, means the key shortcut
	       (when left Alt is pressed you see a small line at the bottom of a specified letter)
	\t   - tabulator
	\r\n - new line
	%d   - another integer value (leave it)
	%s   - another string (leave it)
*/

	gui_messages_tab.push_back( std::map<GuiMsg, std::string>() );

	InsertGuiPair(button_ok,"Ok");
	InsertGuiPair(button_cancel,"Cancel");
	InsertGuiPair(message_box_caption,"TTCalc");

	InsertGuiPair(dialog_box_add_variable_caption,"Add a new variable");
	InsertGuiPair(dialog_box_edit_variable_caption,"Edit a variable");
	InsertGuiPair(dialog_box_add_variable_incorrect_name,"An incorrect name of the variable");
	InsertGuiPair(dialog_box_add_variable_incorrect_value,"An incorrect value of the variable");
	InsertGuiPair(dialog_box_add_variable_variable_exists,"This variable already exists");
	InsertGuiPair(dialog_box_edit_variable_unknown_variable,"There isn't this variable in my table. There's probably an internal error!");
	InsertGuiPair(dialog_box_delete_variable_confirm,"Do you want to delete these variables?");
	InsertGuiPair(dialog_box_variable_not_all_deleted,"There are some variables which I was not able to delete. Probably an internal error!");
	InsertGuiPair(dialog_box_add_function_caption,"Add a new function");
	InsertGuiPair(dialog_box_edit_function_caption,"Edit a function");
	InsertGuiPair(dialog_box_add_function_function_exists,"This function already exists");
	InsertGuiPair(dialog_box_edit_function_unknown_function,"There isn't this function in my table. There's probably an internal error!");
	InsertGuiPair(dialog_box_delete_function_confirm,"Do you want to delete these functions?");
	InsertGuiPair(dialog_box_function_not_all_deleted,"There are some functions which I was not able to delete. Probably an internal error!");
	InsertGuiPair(dialog_box_add_function_incorrect_name,"An incorrect name of the function");

	InsertGuiPair(list_variables_header_1,"Name");
	InsertGuiPair(list_variables_header_2,"Value");
	InsertGuiPair(list_functions_header_1,"Name");
	InsertGuiPair(list_functions_header_2,"Parameters");
	InsertGuiPair(list_functions_header_3,"Value");

	InsertGuiPair(button_add,"Add");
	InsertGuiPair(button_edit,"Edit");
	InsertGuiPair(button_delete,"Delete");
	InsertGuiPair(button_clear,"C");
	InsertGuiPair(tab_standard,"Standard");
	InsertGuiPair(tab_variables,"Variables");
	InsertGuiPair(tab_functions,"Functions");
	InsertGuiPair(tab_precision,"Precision");
	InsertGuiPair(tab_display,"Display");
	InsertGuiPair(tab_convert,"Convert");

	InsertGuiPair(radio_precision_1,"Small - 96 bits for the mantissa, 32 bits for the exponent");
	InsertGuiPair(radio_precision_2,"Medium - 512 bits for the mantissa, 64 bits for the exponent");
	InsertGuiPair(radio_precision_3,"Big - 1024 bits for the mantissa, 128 bits for the exponent");

	InsertGuiPair(precision_1_info,"(+/-)6.97...e+646457021, 26 valid decimal digits");
	InsertGuiPair(precision_2_info,"(+/-)9.25...e+2776511644261678719, 152 valid decimal digits");
	InsertGuiPair(precision_3_info,"(+/-)9.80...*10^(10^37), 306 valid decimal digits");

	InsertGuiPair(overflow_during_printing,"Overflow during printing");

	InsertGuiPair(combo_rounding_none,"none");
	InsertGuiPair(combo_rounding_integer,"to integer");
	InsertGuiPair(combo_rounding_to_number,"to");
	InsertGuiPair(combo_rounding_after_comma,"digit(s)");
	InsertGuiPair(check_remove_zeroes,"Remove last trailing zeroes");

	InsertGuiPair(display_input,			"Input base");
	InsertGuiPair(display_output,			"Output base");
	InsertGuiPair(display_rounding,			"Rounding");
	InsertGuiPair(display_always_scientific,"Always");
	InsertGuiPair(display_not_always_scientific,"When the exponent is greater than:");
	InsertGuiPair(display_as_scientific,	"Print the result as the scientific value");
	InsertGuiPair(display_deg_rad_grad,		"Trigonometric functions");
	InsertGuiPair(display_grouping,			"Grouping");					// e.g. 1`000`000
	InsertGuiPair(display_grouping_none,	"none");
	InsertGuiPair(display_grouping_space,	"space");
	InsertGuiPair(display_grouping_digits,  "digit(s)");

	InsertGuiPair(display_input_decimal_point,	"Input decimal point"); 	// Decimal point in formula
	InsertGuiPair(display_input_decimal_point_item1,	" . or ,");
	InsertGuiPair(display_output_decimal_point,	"Output decimal point");	// Decimal point in result
	InsertGuiPair(display_param_sep,		"Parameters separated by");
	InsertGuiPair(display_param_sep_item2,	" ; or .");
	InsertGuiPair(display_param_sep_item3,	" ; or ,");

	InsertGuiPair(convert_type,				"Type");
	InsertGuiPair(convert_input,			"Input");
	InsertGuiPair(convert_output,			"Output");
	InsertGuiPair(convert_dynamic_output,	"Auto prefix");

	InsertGuiPair(menu_view,				"&View");
	InsertGuiPair(menu_edit,				"&Edit");
	InsertGuiPair(menu_help,				"&Help");
	InsertGuiPair(menu_language,			"&Language");
	InsertGuiPair(menu_view_new_window,		"&New window");
	InsertGuiPair(menu_view_normal_view,	"No&rmal view");
	InsertGuiPair(menu_view_compact_view,	"C&ompact view");
	InsertGuiPair(menu_view_pad,			"&Calcpad");      // something like a notepad for calculating
	InsertGuiPair(menu_view_always_on_top,	"&Always on top");
	InsertGuiPair(menu_view_lang_english,	"&English");
	InsertGuiPair(menu_view_lang_polish,	"&Polish");
	InsertGuiPair(menu_view_lang_spanish,	"&Spanish");
	InsertGuiPair(menu_view_lang_danish,	"&Danish");
	InsertGuiPair(menu_view_lang_chinese,	"&Chinese");
	InsertGuiPair(menu_view_lang_russian,	"&Russian");
	InsertGuiPair(menu_view_lang_swedish,   "S&wedish");
	InsertGuiPair(menu_view_lang_italian,   "&Italian");
	InsertGuiPair(menu_view_lang_german,    "&German");
	InsertGuiPair(menu_view_close_program,	"&Close");
	InsertGuiPair(menu_edit_undo,			"&Undo \tCtrl+Z");
	InsertGuiPair(menu_edit_cut,			"Cu&t \tCtrl+X");
	InsertGuiPair(menu_edit_copy,			"&Copy \tCtrl+C");
	InsertGuiPair(menu_edit_paste,			"&Paste");					// cutting, copying, pasting the the edit which has a focus
	InsertGuiPair(menu_edit_del,			"&Delete \tDel");
	InsertGuiPair(menu_edit_paste_formula,	"Paste &formula\tCtrl+V");	// pasting directly to input edit
	InsertGuiPair(menu_edit_copy_result,	"Copy &result \tCtrl+R");
	InsertGuiPair(menu_edit_copy_both,		"Copy &both \tCtrl+B");		// copying in this way: "input edit = output edit"
	InsertGuiPair(menu_edit_select_all,		"Select &all \tEsc");
	InsertGuiPair(menu_edit_bracket,		"Brac&ket \tCtrl+0");		// inserting brackets: (...)
	InsertGuiPair(menu_edit_swap,			"&Swap \tCtrl+W");			// swapping input edit for output edit
	InsertGuiPair(menu_help_help,			"&Help");
	InsertGuiPair(menu_help_project_page,	"&Project page");
	InsertGuiPair(menu_help_check_update,	"&Check update...");
	InsertGuiPair(menu_help_about,			"&About");
	InsertGuiPair(menu_update_available,	"&New version is available");
	// this will be a new main menu item (one after 'help') visible only if there is an update

	InsertGuiPair(cant_init_calculations,	"I could not initialize the module for calculations");
	InsertGuiPair(message_box_error_caption,"TTCalc");
	InsertGuiPair(cant_create_thread,		"I could not create the second thread for calculating");
	InsertGuiPair(cant_create_pad,			"I could not create the pad window");
	InsertGuiPair(cant_create_main_window,	"I could not create the main window of the application");
	InsertGuiPair(cant_init_common_controls,"I could not initialize the common controls (InitCommonControlsEx)");
	InsertGuiPair(about_text,
		"Mathematical calculator TTCalc %d.%d.%d%s%s\r\n\r\n"
		"Author: Tomasz Sowa\r\n"
		"Contact: t.sowa@ttmath.org\r\n"
		"Licence: BSD (open source)\r\n"
		"Project page: http://ttcalc.sourceforge.net\r\n"
		"Bignum library: TTMath %d.%d.%d%s (%s)\r\n"
		"Bignum type: binary floating point numbers\r\n"
		"Programming language: C++\r\n"
		"Compiler: %s\r\n"
		"%s"	// for upx
		"\r\n"
		"TTCalc uses the TTMath bignum library"
		" which can be found at http://www.ttmath.org\r\n"
		"\r\n"
#ifdef TTCALC_PORTABLE
		"This is the portable version of the program TTCalc. In this version "
		"you can calculate only with one kind of precision (96 bits for the "
		"mantissa and 32 bits for the exponent) it's about +/-6.9e+646457021.\r\n"
		"\r\n"
#endif
		"Translations:\r\n"
		"Spanish	Alejandro S. Valdezate\r\n"
		"	alesanval@gmail.com\r\n"
		"Danish	Rune Bisgaard Vammen\r\n"
		"	runebisgaard@gmail.com\r\n"
		"Chinese	Juis\r\n"
		"	zsyfly@gmail.com\r\n"
		"Russian	Vladimir Gladilovich\r\n"
		"	vdgladilovich@gmail.com\r\n"
		"Swedish	Lars 'Bafvert' Gafvert\r\n"
		"	lars.gafvert@gmail.com\r\n"
		"Italian	Damiano Monaco\r\n"
		"	dmonax@gmail.com\r\n"
		"German	Moritz Beleites\r\n"
		"	morbel@gmx.net\r\n"
		);
	InsertGuiPair(about_text_portable_version, " portable version");
	InsertGuiPair(about_text_exe_packer,	"EXE Packer: UPX 3.07\r\n");
	InsertGuiPair(about_box_title,			"About");
	InsertGuiPair(about_box_button_close,	"Close");
	InsertGuiPair(unknown_error,			"An unknown error has occurred");
	InsertGuiPair(cant_find_help,			"I can't find any help files");
	InsertGuiPair(cant_open_project_page,   "I can't open the project webpage");
	InsertGuiPair(update_title,				"Check for a new version");
	InsertGuiPair(update_button_next,		"Next");
	InsertGuiPair(update_button_finish,		"Finish");
	InsertGuiPair(update_button_cancel,		"Cancel");
	InsertGuiPair(update_check_at_startup,	"Once every three days check for an update automatically");
	InsertGuiPair(update_check_for_info,	"Press Next to check for the update...");
	InsertGuiPair(update_is_new_version,	"Version %d.%d.%d is available, press Next to download...");
	InsertGuiPair(update_no_new_version1,	"There is not a newer version available.");
	InsertGuiPair(update_no_new_version2,	"You have the newest version.");
	InsertGuiPair(update_download_from,		"Downloading from...");
#ifndef TTCALC_PORTABLE
	InsertGuiPair(update_downloaded_info1,	"A new setup program has been downloaded.");
	InsertGuiPair(update_downloaded_info2,	"Press Finish to close TTCalc and run the installer.");
#else
	InsertGuiPair(update_downloaded_info1,	"A new version of TTCalc has been downloaded.");
	InsertGuiPair(update_downloaded_info2,	"");
#endif
	InsertGuiPair(update_download_error,	"There was a problem with downloading, please try again later.");

	InsertGuiPair(pad_title,				"Calcpad");
	InsertGuiPair(pad_menu_file,			"&File");
	InsertGuiPair(pad_menu_edit,			"&Edit");
	InsertGuiPair(pad_menu_file_new,		"&New"); // temporarily not used
	InsertGuiPair(pad_menu_file_open,		"&Open...");
	InsertGuiPair(pad_menu_file_saveas,		"Save &as...");
	InsertGuiPair(pad_menu_file_close,		"&Close");
	InsertGuiPair(pad_menu_edit_undo,		"&Undo \tCtrl+Z");
	InsertGuiPair(pad_menu_edit_cut,		"Cu&t \tCtrl+X");
	InsertGuiPair(pad_menu_edit_copy,		"&Copy \tCtrl+C");
	InsertGuiPair(pad_menu_edit_paste,		"&Paste \tCtrl+V");
	InsertGuiPair(pad_menu_edit_del,		"&Del \tDel");
	InsertGuiPair(pad_menu_edit_select_all,	"Select &all \tCtrl+A");

	InsertGuiPair(cannot_open_file,			"I cannot open the file");
	InsertGuiPair(cannot_save_file,			"I cannot save to such a file");
	InsertGuiPair(file_too_long,			"The file is too long");
	InsertGuiPair(other_error,				"There was a problem with this operation"); // for other kinds of errors

/*
	polish gui messages
*/

	gui_messages_tab.push_back( std::map<GuiMsg, std::string>() );

	InsertGuiPair(button_ok,"Ok");
	InsertGuiPair(button_cancel,"Anuluj");
	InsertGuiPair(message_box_caption,"TTCalc");

	InsertGuiPair(dialog_box_add_variable_caption,"Dodaj now� zmienn�");
	InsertGuiPair(dialog_box_edit_variable_caption,"Zmie� warto�� zmiennej");
	InsertGuiPair(dialog_box_add_variable_incorrect_name,"Nie prawid�owa nazwa zmiennej");
	InsertGuiPair(dialog_box_add_variable_incorrect_value,"Nie prawid�owa warto�� zmiennej");
	InsertGuiPair(dialog_box_add_variable_variable_exists,"Podana zmienna ju� istnieje");
	InsertGuiPair(dialog_box_edit_variable_unknown_variable,"Podanej zmiennej nie ma w tablicy. Prawdopodobnie b��d wewn�trzny programu.");
	InsertGuiPair(dialog_box_delete_variable_confirm,"Czy napewno usun�� zaznaczone zmienne?");
	InsertGuiPair(dialog_box_variable_not_all_deleted,"Zosta�o kilka zmiennych kt�rych nie mo�na by�o skasowa�. Prawdopodobnie b��d wewn�trzny programu.");
	InsertGuiPair(dialog_box_add_function_caption,"Dodaj now� funkcj�");
	InsertGuiPair(dialog_box_edit_function_caption,"Zmie� warto�� funkcji");
	InsertGuiPair(dialog_box_add_function_function_exists,"Podana funkcja ju� istnieje");
	InsertGuiPair(dialog_box_edit_function_unknown_function,"Podanej funkcji nie ma w tablicy. Prawdopodobnie b��d wewn�trzny programu.");
	InsertGuiPair(dialog_box_delete_function_confirm,"Czy napewno usun�� zaznaczone funkcje?");
	InsertGuiPair(dialog_box_function_not_all_deleted,"Zosta�o kilka funkcji kt�rych nie mo�na by�o skasowa�. Prawdopodobnie b��d wewn�trzny programu.");
	InsertGuiPair(dialog_box_add_function_incorrect_name,"Nieprawid�owa nazwa funkcji");


	InsertGuiPair(list_variables_header_1,"Nazwa");
	InsertGuiPair(list_variables_header_2,"Warto��");
	InsertGuiPair(list_functions_header_1,"Nazwa");
	InsertGuiPair(list_functions_header_2,"Parametry");
	InsertGuiPair(list_functions_header_3,"Warto��");

	InsertGuiPair(button_add,"Dodaj");
	InsertGuiPair(button_edit,"Edytuj");
	InsertGuiPair(button_delete,"Usu�");
	InsertGuiPair(button_clear,"C");
	InsertGuiPair(tab_standard,"Standard");
	InsertGuiPair(tab_variables,"Zmienne");
	InsertGuiPair(tab_functions,"Funkcje");
	InsertGuiPair(tab_precision,"Precyzja");
	InsertGuiPair(tab_display,"Wy�wietlanie");
	InsertGuiPair(tab_convert,"Konwersja");

	InsertGuiPair(radio_precision_1,"Ma�a - 96 bitowa mantysa, 32 bitowy wyk�adnik");
	InsertGuiPair(radio_precision_2,"�rednia - 512 bitowa mantysa, 64 bitowy wyk�adnik");
	InsertGuiPair(radio_precision_3,"Du�a - 1024 bitowa mantysa, 128 bitowy wyk�adnik");

	InsertGuiPair(precision_1_info,"(+/-)6.97...e+646457021, 26 cyfr znacz�cych (w rozwini�ciu dziesi�tnym)");
	InsertGuiPair(precision_2_info,"(+/-)9.25...e+2776511644261678719, 152 cyfr znacz�cych");
	InsertGuiPair(precision_3_info,"(+/-)9.80...*10^(10^37), 306 cyfr znacz�cych");

	InsertGuiPair(overflow_during_printing,"Przepe�nienie podczas wypisywania");

	InsertGuiPair(combo_rounding_none,"bez zmian");
	InsertGuiPair(combo_rounding_integer,"do ca�kowitej");
	InsertGuiPair(combo_rounding_to_number,"do");
	InsertGuiPair(combo_rounding_after_comma,"cyfr(y)");
	InsertGuiPair(check_remove_zeroes,"Skasuj nieznacz�ce zera");

	InsertGuiPair(display_input,"Wej�cie");
	InsertGuiPair(display_output,"Wyj�cie");
	InsertGuiPair(display_rounding,"Zaokr�glenie");
	InsertGuiPair(display_always_scientific,"Zawsze");
	InsertGuiPair(display_not_always_scientific,"Je�li eksponent jest wi�kszy ni�:");
	InsertGuiPair(display_as_scientific,	"Wy�wietl wynik w postaci naukowej");
	InsertGuiPair(display_deg_rad_grad,		"Funkcje trygonometryczne");
	InsertGuiPair(display_grouping,			"Grupowanie");
	InsertGuiPair(display_grouping_none,	"brak");
	InsertGuiPair(display_grouping_space,	"spacja");
	InsertGuiPair(display_grouping_digits,  "cyfr(y)");
	InsertGuiPair(display_input_decimal_point,	"Przecinek w formule");
	InsertGuiPair(display_input_decimal_point_item1,	" . lub ,");
	InsertGuiPair(display_output_decimal_point,	"Przecinek w wyniku");
	InsertGuiPair(display_param_sep,		"Parametry oddzielane przez");
	InsertGuiPair(display_param_sep_item2,	" ; lub .");
	InsertGuiPair(display_param_sep_item3,	" ; lub ,");

	InsertGuiPair(convert_type,				"Typ");
	InsertGuiPair(convert_input,			"Wej�cie");
	InsertGuiPair(convert_output,			"Wyj�cie");
	InsertGuiPair(convert_dynamic_output,	"Automatyczny prefiks");

	InsertGuiPair(menu_view,				"&Widok");
	InsertGuiPair(menu_edit,				"&Edycja");
	InsertGuiPair(menu_help,				"&Pomoc");
	InsertGuiPair(menu_language,			"&J�zyk");
	InsertGuiPair(menu_view_new_window,		"&Nowe okno");
	InsertGuiPair(menu_view_normal_view,	"Widok no&rmalny");
	InsertGuiPair(menu_view_compact_view,	"Widok &kompaktowy");
	InsertGuiPair(menu_view_always_on_top,	"Zawsze na &wierzchu");
	InsertGuiPair(menu_view_pad,			"&Calcpad");
	InsertGuiPair(menu_view_lang_english,	"&Angielski");
	InsertGuiPair(menu_view_lang_polish,	"&Polski");
	InsertGuiPair(menu_view_lang_spanish,	"&Hiszpa�ski");
	InsertGuiPair(menu_view_lang_danish,	"&Du�ski");
	InsertGuiPair(menu_view_lang_chinese,	"&Chi�ski");
	InsertGuiPair(menu_view_lang_russian,	"&Rosyjski");
	InsertGuiPair(menu_view_lang_swedish,   "&Szwedzki");
	InsertGuiPair(menu_view_lang_italian,   "&W�oski");
	InsertGuiPair(menu_view_lang_german,    "&Niemiecki");
	InsertGuiPair(menu_view_close_program,	"&Zamknij");
	InsertGuiPair(menu_edit_undo,			"&Cofnij \tCtrl+Z");
	InsertGuiPair(menu_edit_cut,			"Wy&tnij \tCtrl+X");
	InsertGuiPair(menu_edit_copy,			"&Kopiuj \tCtrl+C");
	InsertGuiPair(menu_edit_paste,			"&Wklej");
	InsertGuiPair(menu_edit_del,			"&Usu� \tDel");
	InsertGuiPair(menu_edit_paste_formula,	"Wklej &formu�� \tCtrl+V");
	InsertGuiPair(menu_edit_copy_result,	"Kopiuj w&ynik \tCtrl+R");
	InsertGuiPair(menu_edit_copy_both,		"Kopiuj &obydwa \tCtrl+B");
	InsertGuiPair(menu_edit_select_all,		"Zaznacz w&szystko \tEsc");
	InsertGuiPair(menu_edit_bracket,		"&Nawiasy \tCtrl+0");
	InsertGuiPair(menu_edit_swap,			"&Zamiana \tCtrl+W");
	InsertGuiPair(menu_help_help,			"&Pomoc");
	InsertGuiPair(menu_help_project_page,	"&Strona projektu");
	InsertGuiPair(menu_help_check_update,	"Sprawd� &aktualizacj�...");
	InsertGuiPair(menu_help_about,			"&O programie");
	InsertGuiPair(menu_update_available,	"&Jest nowa wersja programu");

	InsertGuiPair(cant_init_calculations,	"Nie uda�o si� zainicjalizowa� modu�u obs�ugi oblicze�");
	InsertGuiPair(message_box_error_caption,"TTCalc");
	InsertGuiPair(cant_create_thread,		"Nie uda�o si� utworzy� drugiego w�tku do oblicze�");
	InsertGuiPair(cant_create_pad,			"I could not create the pad window");
	InsertGuiPair(cant_create_main_window,	"Nie uda�o si� utworzy� g��wnego okna aplikacji");
	InsertGuiPair(cant_init_common_controls,"Nie uda�o si� zainicjalizowa� obs�ugi Common Controls (InitCommonControlsEx)");
	InsertGuiPair(about_text,
		"Kalkulator matematyczny TTCalc %d.%d.%d%s%s\r\n\r\n"
		"Autor: Tomasz Sowa\r\n"
		"Kontakt: t.sowa@ttmath.org\r\n"
		"Licencja: BSD (open source)\r\n"
		"Strona projektu: http://ttcalc.sourceforge.net\r\n"
		"Biblioteka du�ych liczb: TTMath %d.%d.%d%s (%s)\r\n"
		"Rodzaj du�ych liczb: liczby binarne zmiennoprzecinkowe\r\n"
		"J�zyk programowania: C++\r\n"
		"Kompilator: %s\r\n"
		"%s"	// for upx
		"\r\n"
		"Ten program u�ywa biblioteki du�ych liczb TTMath"
		" kt�ra jest dost�pna na http://www.ttmath.org\r\n"
		"\r\n"
#ifdef TTCALC_PORTABLE
		"To jest wersja portable programu TTCalc. W tej wersji mo�esz dokonywa� "
		"oblicze� jedynie z jednym rodzajem precyzji (96 bitowa mantysa "
		"oraz 32 bitowy wyk�adnik) to jest oko�o +/-6.9e+646457021.\r\n"
		"\r\n"
#endif
		"T�umaczenie:\r\n"
		"Hiszpa�ski	Alejandro S. Valdezate\r\n"
		"		alesanval@gmail.com\r\n"
		"Du�ski		Rune Bisgaard Vammen\r\n"
		"		runebisgaard@gmail.com\r\n"
		"Chi�ski		Juis\r\n"
		"		zsyfly@gmail.com\r\n"
		"Rosyjski		Vladimir Gladilovich\r\n"
		"		vdgladilovich@gmail.com\r\n"
		"Szwedzki		Lars 'Bafvert' Gafvert\r\n"
		"		lars.gafvert@gmail.com\r\n"
		"W�oski		Damiano Monaco\r\n"
		"		dmonax@gmail.com\r\n"
		"Niemiecki		Moritz Beleites\r\n"
		"		morbel@gmx.net\r\n"
		);
	InsertGuiPair(about_text_portable_version, " wersja portable");
	InsertGuiPair(about_text_exe_packer,	"Paker exe: UPX 3.07\r\n");
	InsertGuiPair(about_box_title,			"O programie");
	InsertGuiPair(about_box_button_close,	"Zamknij");
	InsertGuiPair(unknown_error,			"Nieznany kod b��du");
	InsertGuiPair(cant_find_help,			"Nie mog� znale�� �adnych plik�w pomocy");
	InsertGuiPair(cant_open_project_page,   "Nie mog� otworzy� strony projektu");
	InsertGuiPair(update_title,				"Sprawd� dost�pno�� nowej wersji");
	InsertGuiPair(update_button_next,		"Dalej");
	InsertGuiPair(update_button_finish,		"Zako�cz");
	InsertGuiPair(update_button_cancel,		"Anuluj");
	InsertGuiPair(update_check_at_startup,	"Raz na trzy dni sprawd� dost�pno�� aktualizacji automatycznie");
	InsertGuiPair(update_check_for_info,	"Wci�nij Dalej aby sprawdzi� dost�pno�� aktualizacji...");
	InsertGuiPair(update_is_new_version,	"Wersja %d.%d.%d jest dost�pna, wci�nij Dalej aby j� pobra�...");
	InsertGuiPair(update_no_new_version1,	"Nie ma dost�pnej nowszej wersji programu.");
	InsertGuiPair(update_no_new_version2,	"Obecnie masz najnowsz� wersj�.");
	InsertGuiPair(update_download_from,		"Pobieranie z...");
#ifndef TTCALC_PORTABLE
	InsertGuiPair(update_downloaded_info1,	"Nowy instalator zosta� �ci�gni�ty.");
	InsertGuiPair(update_downloaded_info2,	"Wci�nij Zako�cz aby zamkn�� program TTCalc i uruchomi� pobrany instalator.");
#else
	InsertGuiPair(update_downloaded_info1,	"Nowa wersja programu TTCalc zosta�a �ci�gni�ta");
	InsertGuiPair(update_downloaded_info2,	"");
#endif
	InsertGuiPair(update_download_error,	"Wyst�pi�y problemy ze �ci�gni�ciem, prosz� spr�bowa� ponownie p�niej.");

	InsertGuiPair(pad_title,				"Calcpad");
	InsertGuiPair(pad_menu_file,			"&Plik");
	InsertGuiPair(pad_menu_edit,			"&Edycja");
	InsertGuiPair(pad_menu_file_new,		"&Nowy"); // temporarily not used
	InsertGuiPair(pad_menu_file_open,		"&Otw�rz...");
	InsertGuiPair(pad_menu_file_saveas,		"Zapisz &jako...");
	InsertGuiPair(pad_menu_file_close,		"&Zamknij");
	InsertGuiPair(pad_menu_edit_undo,		"&Cofnij \tCtrl+Z");
	InsertGuiPair(pad_menu_edit_cut,		"Wy&tnij \tCtrl+X");
	InsertGuiPair(pad_menu_edit_copy,		"&Kopiuj \tCtrl+C");
	InsertGuiPair(pad_menu_edit_paste,		"&Wklej \tCtrl+V");
	InsertGuiPair(pad_menu_edit_del,		"&Usu� \tDel");
	InsertGuiPair(pad_menu_edit_select_all,	"Zaznacz wszystko \tCtrl+A");

	InsertGuiPair(cannot_open_file,			"Nie mog� otworzy� podanego pliku");
	InsertGuiPair(cannot_save_file,			"Nie mog� zapisa� podanego pliku");
	InsertGuiPair(file_too_long,			"Podany plik jest zbyt du�y");
	InsertGuiPair(other_error,				"Wyst�pi� problem z t� operacj�"); // for other kinds of errors


/*
	spanish gui messages
*/


	gui_messages_tab.push_back( std::map<GuiMsg, std::string>() );

	InsertGuiPair(button_ok,"Ok");
	InsertGuiPair(button_cancel,"Cancelar");
	InsertGuiPair(message_box_caption,"TTCalc");

	InsertGuiPair(dialog_box_add_variable_caption,"A�adir nueva variable");
	InsertGuiPair(dialog_box_edit_variable_caption,"Editar variable");
	InsertGuiPair(dialog_box_add_variable_incorrect_name,"Nombre incorrecto de variable");
	InsertGuiPair(dialog_box_add_variable_incorrect_value,"Valor incorrecto de variable");
	InsertGuiPair(dialog_box_add_variable_variable_exists,"Esta variable ya existe");
	InsertGuiPair(dialog_box_edit_variable_unknown_variable,"No hay variable en mi tabla!. Seguramente haya un error interno!");
	InsertGuiPair(dialog_box_delete_variable_confirm,"Quiere borrar las variables internas?");
	InsertGuiPair(dialog_box_variable_not_all_deleted,"Hay algunas variables que no se pueden borrar. Probablemente hay un error interno!");
	InsertGuiPair(dialog_box_add_function_caption,"A�adir funcion");
	InsertGuiPair(dialog_box_edit_function_caption,"Editar funcion");
	InsertGuiPair(dialog_box_add_function_function_exists,"Esta funcion ya existe");
	InsertGuiPair(dialog_box_edit_function_unknown_function,"No existe esta funcion en mi tabla. Problablemente error interno!");
	InsertGuiPair(dialog_box_delete_function_confirm,"Quiere borrar estas funciones?");
	InsertGuiPair(dialog_box_function_not_all_deleted,"Hay algunas funciones que no se pueden borrar. Probablmenete error interno!");
	InsertGuiPair(dialog_box_add_function_incorrect_name,"Nombre incorrecto de la funci�n");

	InsertGuiPair(list_variables_header_1,"Nombre");
	InsertGuiPair(list_variables_header_2,"Valor");
	InsertGuiPair(list_functions_header_1,"Nombre");
	InsertGuiPair(list_functions_header_2,"Param.");
	InsertGuiPair(list_functions_header_3,"Valor");

	InsertGuiPair(button_add,"A�adir");
	InsertGuiPair(button_edit,"Editar");
	InsertGuiPair(button_delete,"Borrar");
	InsertGuiPair(button_clear,"C");
	InsertGuiPair(tab_standard,"Standard");
	InsertGuiPair(tab_variables,"Variables");
	InsertGuiPair(tab_functions,"Funciones");
	InsertGuiPair(tab_precision,"Precisi�n");
	InsertGuiPair(tab_display,"Pantalla");
	InsertGuiPair(tab_convert,"Convertir");

	InsertGuiPair(radio_precision_1,"Peque�o - 96 bits para la mantisa, 32 bits para el exponente");
	InsertGuiPair(radio_precision_2,"Mediano - 512 bits para la mantissa, 64 bits para el exponente");
	InsertGuiPair(radio_precision_3,"Grande - 1024 bits para la mantissa, 128 bits para el exponente");

	InsertGuiPair(precision_1_info,"(+/-)6.97...e+646457021, 26 d�gitos v�lidos (decimal)");
	InsertGuiPair(precision_2_info,"(+/-)9.25...e+2776511644261678719, 152 d�gitos v�lidos");
	InsertGuiPair(precision_3_info,"(+/-)9.80...*10^(10^37), 306 d�gitos v�lidos");

	InsertGuiPair(overflow_during_printing,"Sobredimensi�n durante impresi�n");

	InsertGuiPair(combo_rounding_none,"Ninguno");
	InsertGuiPair(combo_rounding_integer,"a entero");
	InsertGuiPair(combo_rounding_to_number,"a n�mero");
	// this text was too long (the combobox is shorter now) -- I'm not sure whether this is correct now... (I don't know spanish at all)
	//InsertGuiPair(combo_rounding_to_number,"a");
	InsertGuiPair(combo_rounding_after_comma,"digito(s)");
	InsertGuiPair(check_remove_zeroes,"Eliminar ceros no-significativos");

	InsertGuiPair(display_input,			"Entrada");
	InsertGuiPair(display_output,			"Salida");
	InsertGuiPair(display_rounding,			"Redondeo");
	InsertGuiPair(display_always_scientific,"Siempre");
	InsertGuiPair(display_not_always_scientific,"Cuando el exponente es mas grande que:");
	InsertGuiPair(display_as_scientific,	"Imprimir el resultado como valor cient�fico");
	InsertGuiPair(display_deg_rad_grad,		"Funciones trigonom�tricas");
	InsertGuiPair(display_grouping,			"Agrupar");					// e.g. 1`000`000
	InsertGuiPair(display_grouping_none,	"Ninguno");
	InsertGuiPair(display_grouping_space,	"Espacio");
	InsertGuiPair(display_grouping_digits,  "digito(s)");
	InsertGuiPair(display_input_decimal_point,	"S�mbolo decimal en f�rmula"); 	// Decimal point in formula
	InsertGuiPair(display_input_decimal_point_item1,	" . o ,");     	
	InsertGuiPair(display_output_decimal_point,	"S�mbolo decimal en resultado");	// Decimal point in result
	InsertGuiPair(display_param_sep,		"Par�metros separados por");
	InsertGuiPair(display_param_sep_item2,	" ; o .");
	InsertGuiPair(display_param_sep_item3,	" ; o ,");

	InsertGuiPair(convert_type,				"Tipo");
	InsertGuiPair(convert_input,			"Entrada");
	InsertGuiPair(convert_output,			"Salida");
	InsertGuiPair(convert_dynamic_output,	"Auto prefijo");

	InsertGuiPair(menu_view,				"&Ver");
	InsertGuiPair(menu_edit,				"&Editar");
	InsertGuiPair(menu_help,				"&Ayuda");
	InsertGuiPair(menu_language,			"&Idioma");
	InsertGuiPair(menu_view_new_window,		"&Nueva ventana");
	InsertGuiPair(menu_view_normal_view,	"Vista No&rmal");
	InsertGuiPair(menu_view_compact_view,	"Vista C&ompacta");
	InsertGuiPair(menu_view_always_on_top,	"&Siempre arriba");
	InsertGuiPair(menu_view_pad,			"&Pad");
	InsertGuiPair(menu_view_lang_english,	"&Ingl�s");
	InsertGuiPair(menu_view_lang_polish,	"&Polaco");
	InsertGuiPair(menu_view_lang_spanish,	"&Espa�ol");
	InsertGuiPair(menu_view_lang_danish,	"&Dan�s");
	InsertGuiPair(menu_view_lang_chinese,	"&Chino");
	InsertGuiPair(menu_view_lang_russian,	"&Ruso");
	InsertGuiPair(menu_view_lang_swedish,   "&Sueco");
	InsertGuiPair(menu_view_lang_italian,   "I&talian");
	InsertGuiPair(menu_view_lang_german,    "&Alem�n"); // is it a good translation?
	InsertGuiPair(menu_view_close_program,	"&Close");

	InsertGuiPair(menu_edit_undo,			"&Deshacer \tCtrl+Z");
	InsertGuiPair(menu_edit_cut,			"Cor&tar \tCtrl+X");
	InsertGuiPair(menu_edit_copy,			"&Copiar \tCtrl+C");
	InsertGuiPair(menu_edit_paste,			"&Pegar");						// cutting, copying, pasting the the edit which has a focus
	InsertGuiPair(menu_edit_del,			"&Borrar \tDel");
	InsertGuiPair(menu_edit_copy_result,	"Copiar &resultado \tCtrl+R");
	InsertGuiPair(menu_edit_copy_both,		"Copiar &ambos \tCtrl+B");		// copying in this way: "input edit = output edit"
	InsertGuiPair(menu_edit_paste_formula,	"Pegar &f�rmula\tCtrl+V");		// pasting directly to the input edit
	InsertGuiPair(menu_edit_select_all,		"Seleccionar &todo \tEsc");
	InsertGuiPair(menu_edit_bracket,		"C&orchetes \tCtrl+0");			// inserting brackets: (...)
	InsertGuiPair(menu_edit_swap,			"&Swap \tCtrl+W");				// swapping input edit for output edit
	InsertGuiPair(menu_update_available,	"&Nueva versi�n disponible");

	InsertGuiPair(menu_help_help,			"&Ayuda");
	InsertGuiPair(menu_help_project_page,	"&Pagina del proyecto");
	InsertGuiPair(menu_help_check_update,	"&Comprobar actualizaciones...");
	InsertGuiPair(menu_help_about,			"Acerca &de");

	InsertGuiPair(cant_init_calculations,	"No se puede inicializar el modulo de c�lculo");
	InsertGuiPair(message_box_error_caption,"TTCalc");
	InsertGuiPair(cant_create_thread,		"No se puede crear la segunda hebra para calcular");
	InsertGuiPair(cant_create_pad,			"I could not create the pad window");
	InsertGuiPair(cant_create_main_window,	"No se puede crear la ventana principal para la aplicaci�n");
	InsertGuiPair(cant_init_common_controls,"No se pueden inicializar los controles comunes (InitCommonControlsEx)");
	InsertGuiPair(about_text,
		"Calculadora matem�tica TTCalc %d.%d.%d%s%s\r\n\r\n"
		"Autor: Tomasz Sowa\r\n"
		"Contacto: t.sowa@ttmath.org\r\n"
		"Licencia: BSD (open source)\r\n"
		"P�gina del proyecto: http://ttcalc.sourceforge.net\r\n"
		"Biblioteca Bignum: TTMath %d.%d.%d%s (%s)\r\n"
		"Bignum type: binary floating point numbers\r\n"
		"Lenguaje de programaci�n: C++\r\n"
		"Compilador: %s\r\n"
		"%s"	// for upx
		"\r\n"
		"TTCalc usa la biblioteca TTMath bignum"
		" la cual se puede ser encontrar en http://www.ttmath.org\r\n"
		"\r\n"
#ifdef TTCALC_PORTABLE
		"Esta es la version port�til del programa TTCalc. En esta versi�n "
		"se puede calcular con una precisi�n tal que (96 bits para la "
		"mantisa y 32 bits para el exponente)  que es aproximadamente +/-6.9e+646457021.\r\n"
		"\r\n"
#endif
		"Traducciones:\r\n"
		"Espa�ol	Alejandro Valdezate\r\n"
		"	valdezate@gmail.com\r\n"
		"Dan�s	Rune Bisgaard Vammen\r\n"
		"	runebisgaard@gmail.com\r\n"
		"Chino	Juis\r\n"
		"	zsyfly@gmail.com\r\n"
		"Ruso	Vladimir Gladilovich\r\n"
		"	vdgladilovich@gmail.com\r\n"
		"Sueco	Lars 'Bafvert' Gafvert\r\n"
		"	lars.gafvert@gmail.com\r\n"
		"Italian	Damiano Monaco\r\n"
		"	dmonax@gmail.com\r\n"
		"Alem�n	Moritz Beleites\r\n"
		"	morbel@gmx.net\r\n"
		);
	InsertGuiPair(about_text_portable_version, " versi�n portatil");
	InsertGuiPair(about_text_exe_packer,	"EXE Packer: UPX 3.07\r\n");
	InsertGuiPair(about_box_title,			"Acerca de");
	InsertGuiPair(about_box_button_close,	"Cerrar");
	InsertGuiPair(unknown_error,			"Ocurri� un error desconocido");
	InsertGuiPair(cant_find_help,			"No se encuentran los ficheros de ayuda");
	InsertGuiPair(cant_open_project_page,   "No se puede abrir la p�gina del proyecto");

	InsertGuiPair(update_title,				"Comprobar si hay alguna version nueva");
	InsertGuiPair(update_button_next,		"Siguiente");
	InsertGuiPair(update_button_finish,		"Finalizar");
	InsertGuiPair(update_button_cancel,		"Cancelar");
	InsertGuiPair(update_check_at_startup,	"Comprobar siempre al comienzo si hay una nueva versi�n"); // this text has been changed, need correction!
	InsertGuiPair(update_check_for_info,	"Pulse Siguiente para ver si hay actualizaciones...");
	InsertGuiPair(update_is_new_version,	"La versi�n %d.%d.%d est� disponible, pulse Siguiente para descargar...");
	InsertGuiPair(update_no_new_version1,	"No hay versiones nuevas disponibles.");
	InsertGuiPair(update_no_new_version2,	"");
	InsertGuiPair(update_download_from,		"Descargando desde...");
#ifndef TTCALC_PORTABLE
	InsertGuiPair(update_downloaded_info1,	"Se ha descargado un nuevo programa de instalaci�n.");
	InsertGuiPair(update_downloaded_info2,	"Pulse Finalizar para cerrar TTCalc y ejecutar el programa de instalaci�n.");
#else
	InsertGuiPair(update_downloaded_info1,	"Se ha descargado un nuevo programa de instalaci�n.");
	InsertGuiPair(update_downloaded_info2,	"");
#endif
	InsertGuiPair(update_download_error,	"Hubo un problema durante la descarga. Por favor, int�ntelo mas tarde.");

	InsertGuiPair(pad_title,				"Pad");
	InsertGuiPair(pad_menu_file,			"&Archivo");
	InsertGuiPair(pad_menu_edit,			"&Editar");
	InsertGuiPair(pad_menu_file_new,		"&Nuevo"); // temporarily not used
	InsertGuiPair(pad_menu_file_open,		"&Abrir...");
	InsertGuiPair(pad_menu_file_saveas,		"Guardar &como...");
	InsertGuiPair(pad_menu_file_close,		"&Cerrar");
	InsertGuiPair(pad_menu_edit_undo,		"&Deshacer \tCtrl+Z");
	InsertGuiPair(pad_menu_edit_cut,		"Cor&tar \tCtrl+X");
	InsertGuiPair(pad_menu_edit_copy,		"&Copiar \tCtrl+C");
	InsertGuiPair(pad_menu_edit_paste,		"&Pegar \tCtrl+V");
	InsertGuiPair(pad_menu_edit_del,		"&Borrar \tDel");
	InsertGuiPair(pad_menu_edit_select_all,	"Seleccionar &todo \tCtrl+A");

	InsertGuiPair(cannot_open_file,			"I cannot open the file");
	InsertGuiPair(cannot_save_file,			"I cannot save to such a file");
	InsertGuiPair(file_too_long,			"The file is too long");
	InsertGuiPair(other_error,				"There was a problem with this operation"); // for other kinds of errors



/*
	danish gui messages
*/
	gui_messages_tab.push_back( std::map<GuiMsg, std::string>() );

	InsertGuiPair(button_ok,"Ok");
	InsertGuiPair(button_cancel,"Anuller");
	InsertGuiPair(message_box_caption,"TTCalc");

	InsertGuiPair(dialog_box_add_variable_caption,"Tilf�j en ny variabel");
	InsertGuiPair(dialog_box_edit_variable_caption,"Rediger en variabel");
	InsertGuiPair(dialog_box_add_variable_incorrect_name,"Et ukorrekt navn for variablen");
	InsertGuiPair(dialog_box_add_variable_incorrect_value,"En ukorrekt v�rdi for variablen");
	InsertGuiPair(dialog_box_add_variable_variable_exists,"Denne variabel eksisterer allerede");
	InsertGuiPair(dialog_box_edit_variable_unknown_variable,"Der er ikke denne variabel i min tabel. Der er sandsynligvis en intern fejl!");
	InsertGuiPair(dialog_box_delete_variable_confirm,"Vil du slette disse variabler?");
	InsertGuiPair(dialog_box_variable_not_all_deleted,"Der er nogle variabler det ikke var mugligt at slette. Sandsynligvis en intern fejl!");
	InsertGuiPair(dialog_box_add_function_caption,"Tilf�j en ny funktion");
	InsertGuiPair(dialog_box_edit_function_caption,"Rediger en funktion");
	InsertGuiPair(dialog_box_add_function_function_exists,"Denne funktion eksisterer allerede");
	InsertGuiPair(dialog_box_edit_function_unknown_function,"Der er ikke denne funktion i min tabel. Der er sandsynligvis en intern fejl!");
	InsertGuiPair(dialog_box_delete_function_confirm,"Vil du slette disse funktioner?");
	InsertGuiPair(dialog_box_function_not_all_deleted,"Der er nogle funktioner det ikke var mugligt at slette. Sandsynligvis en intern fejl!");
	InsertGuiPair(dialog_box_add_function_incorrect_name,"Et ukorrekt navn for funktionen");

	InsertGuiPair(list_variables_header_1,"Navn");
	InsertGuiPair(list_variables_header_2,"V�rdi");
	InsertGuiPair(list_functions_header_1,"Navn");
	InsertGuiPair(list_functions_header_2,"Param.");
	InsertGuiPair(list_functions_header_3,"V�rdi");

	InsertGuiPair(button_add,"Tilf�j");
	InsertGuiPair(button_edit,"Rediger");
	InsertGuiPair(button_delete,"Slet");
	InsertGuiPair(button_clear,"C");
	InsertGuiPair(tab_standard,"Standard");
	InsertGuiPair(tab_variables,"Variabler");
	InsertGuiPair(tab_functions,"Funktioner");
	InsertGuiPair(tab_precision,"Pr�cision");
	InsertGuiPair(tab_display,"Visning");
	InsertGuiPair(tab_convert,"Konverter");

	InsertGuiPair(radio_precision_1,"Lille - 96 bits for mantissen, 32 bits for exponenten");
	InsertGuiPair(radio_precision_2,"Mellem - 512 bits for mantissen, 64 bits for exponenten");
	InsertGuiPair(radio_precision_3,"Stor - 1024 bits for mantissen, 128 bits for exponenten");


	InsertGuiPair(precision_1_info,"(+/-)6.97...e+646457021, 26 gyldige cifre (decimaler)");
	InsertGuiPair(precision_2_info,"(+/-)9.25...e+2776511644261678719, 152 gyldige cifre");
	InsertGuiPair(precision_3_info,"(+/-)9.80...*10^(10^37), 306 gyldige cifre");

	InsertGuiPair(overflow_during_printing,"Overflow under printning");

	InsertGuiPair(combo_rounding_none,"Ingen");
	InsertGuiPair(combo_rounding_integer,"til heltal");
	InsertGuiPair(combo_rounding_to_number,"til");
	InsertGuiPair(combo_rounding_after_comma,"ciffer(cifre)");
	InsertGuiPair(check_remove_zeroes,"Fjern sidste overfl�dige nuller");


	InsertGuiPair(display_input,			"Input");
	InsertGuiPair(display_output,			"Output");
	InsertGuiPair(display_rounding,			"Afrunding");
	InsertGuiPair(display_always_scientific,"Altid");
	InsertGuiPair(display_not_always_scientific,"N�r exponenten er st�rre end:");
	InsertGuiPair(display_as_scientific,	"Print resultatet som den videnskablige v�rdi");
	InsertGuiPair(display_deg_rad_grad,		"Trigonometriske funktioner");
	InsertGuiPair(display_grouping,			"Gruppering");					// e.g. 1`000`000
	InsertGuiPair(display_grouping_none,	"Ingen");
	InsertGuiPair(display_grouping_space,	"Mellemrum");
	InsertGuiPair(display_grouping_digits,  "ciffer(cifre)");
	InsertGuiPair(display_input_decimal_point,	"Input decimal tegn");		// Decimal point in formula
	InsertGuiPair(display_input_decimal_point_item1,	" . eller ,");     	
	InsertGuiPair(display_output_decimal_point,	"Output decimal tegn");		// Decimal point in result
	InsertGuiPair(display_param_sep,		"Parametre separeret af");
	InsertGuiPair(display_param_sep_item2,	" ; eller .");
	InsertGuiPair(display_param_sep_item3,	" ; eller ,");

	InsertGuiPair(convert_type,				"Type");
	InsertGuiPair(convert_input,			"Input");
	InsertGuiPair(convert_output,			"Udput");
	InsertGuiPair(convert_dynamic_output,	"Auto pr�fix");

	InsertGuiPair(menu_view,				"&Vis");
	InsertGuiPair(menu_edit,				"&Rediger");
	InsertGuiPair(menu_help,				"&Hj�lp");
	InsertGuiPair(menu_language,			"&Sprog");
	InsertGuiPair(menu_view_new_window,		"&Nyt vindue");
	InsertGuiPair(menu_view_normal_view,	"No&rmal visning");
	InsertGuiPair(menu_view_compact_view,	"K&ompakt visning");
	InsertGuiPair(menu_view_always_on_top,	"&Altid �verst");
	InsertGuiPair(menu_view_pad,			"Re&digeringsvindue");
	InsertGuiPair(menu_view_lang_english,	"&Engelsk");
	InsertGuiPair(menu_view_lang_polish,	"&Polsk");
	InsertGuiPair(menu_view_lang_spanish,	"&Spansk");
	InsertGuiPair(menu_view_lang_danish,	"&Dansk");
	InsertGuiPair(menu_view_lang_chinese,	"&Kinesisk");
	InsertGuiPair(menu_view_lang_russian,	"&Russisk");
	InsertGuiPair(menu_view_lang_swedish,   "S&vensk");
	InsertGuiPair(menu_view_lang_italian,   "&Italian");
	InsertGuiPair(menu_view_lang_german,    "&Tysk"); // is it a good translation?
	InsertGuiPair(menu_view_close_program,	"Afslu&t");
	InsertGuiPair(menu_edit_undo,			"&Fortryd \tCtrl+Z");
	InsertGuiPair(menu_edit_cut,			"Kli&p \tCtrl+X");
	InsertGuiPair(menu_edit_copy,			"&Kopier \tCtrl+C");
	InsertGuiPair(menu_edit_paste,			"&S�t ind");					// cutting, copying, pasting the the edit which has a focus
	InsertGuiPair(menu_edit_del,			"S&let \tDel");
	InsertGuiPair(menu_edit_copy_result,	"Kopier &resultatet \tCtrl+R");
	InsertGuiPair(menu_edit_copy_both,		"Kopier &begge \tCtrl+B");		// copying in this way: "input edit = output edit"
	InsertGuiPair(menu_edit_paste_formula,	"Inds�t f&ormular\tCtrl+V");	// pasting directly to the input edit
	InsertGuiPair(menu_edit_select_all,		"Marker &alt \tEsc");
	InsertGuiPair(menu_edit_bracket,		"Bracke&t \tCtrl+0");			// inserting brackets: (...)
	InsertGuiPair(menu_edit_swap,			"&Swap \tCtrl+W");				// swapping input edit for output edit
	InsertGuiPair(menu_help_help,			"&Hj�lp");
	InsertGuiPair(menu_help_project_page,	"&Projekt side");
	InsertGuiPair(menu_help_check_update,	"O&pdater...");
	InsertGuiPair(menu_help_about,			"&Om");
	InsertGuiPair(menu_update_available,	"Der er en &ny version tilg�ngelig");

	InsertGuiPair(cant_init_calculations,	"Moduelt for uregninger kunne ikke s�ttes i gang");
	InsertGuiPair(message_box_error_caption,"TTCalc");
	InsertGuiPair(cant_create_thread,		"Den anden tr�d for udregning kunne ikke laves");
	InsertGuiPair(cant_create_pad,			"I could not create the pad window");
	InsertGuiPair(cant_create_main_window,	"Applikationens hovedvindue kunne ikke laves");
	InsertGuiPair(cant_init_common_controls,"De f�lles kontrol indstillinger kunne ikke s�ttes i gang (InitCommonControlsEx)");
	InsertGuiPair(about_text,
		"Matematisk lommeregner TTCalc %d.%d.%d%s%s\r\n\r\n"
		"Forfatter: Tomasz Sowa\r\n"
		"Kontakt: t.sowa@ttmath.org\r\n"
		"Licens: BSD (open source)\r\n"
		"Projekt side: http://ttcalc.sourceforge.net\r\n"
		"Bignum biblotek: TTMath %d.%d.%d%s (%s)\r\n"
		"Bignum type: binary floating point numbers\r\n"
		"Programmerings sprog: C++\r\n"
		"Kompilator: %s\r\n"
		"%s"	// for upx
		"\r\n"
		"TTCalc bruger TTMath bignum bibloteket"
		" som kan findes ved http://www.ttmath.org\r\n"
		"\r\n"
#ifdef TTCALC_PORTABLE
		"Dette er den transportable version af programmet TTCalc. I denne version "
		"kan du kun udregne med �n slags pr�cision (96 bits for "
		"mantissen og 32 bits for exponenten) det er omkring +/-6.9e+646457021.\r\n"
		"\r\n"
#endif
		"Translations:\r\n"
		"Spanish	Alejandro S. Valdezate\r\n"
		"	alesanval@gmail.com\r\n"
		"Danish	Rune Bisgaard Vammen\r\n"
		"	runebisgaard@gmail.com\r\n"
		"Chinese	Juis\r\n"
		"	zsyfly@gmail.com\r\n"
		"Russian	Vladimir Gladilovich\r\n"
		"	vdgladilovich@gmail.com\r\n"
		"Swedish	Lars 'Bafvert' Gafvert\r\n"
		"	lars.gafvert@gmail.com\r\n"
		"Italian	Damiano Monaco\r\n"
		"	dmonax@gmail.com\r\n"
		"Tysk	Moritz Beleites\r\n"
		"	morbel@gmx.net\r\n"
		);
	InsertGuiPair(about_text_portable_version, " transportabel version");
	InsertGuiPair(about_text_exe_packer,	"EXE Packer: UPX 3.07\r\n");
	InsertGuiPair(about_box_title,			"Om");
	InsertGuiPair(about_box_button_close,	"Luk");
	InsertGuiPair(unknown_error,			"Der er sket en ukendt fejl");
	InsertGuiPair(cant_find_help,			"Der kunne ikke findes nogen hj�lp filer");
	InsertGuiPair(cant_open_project_page,   "Projekt siden kunne ikke �bnes");

	InsertGuiPair(update_title,				"Opdater TTCalc");
	InsertGuiPair(update_button_next,		"N�ste");
	InsertGuiPair(update_button_finish,		"F�rdig");
	InsertGuiPair(update_button_cancel,		"Annuler");
	InsertGuiPair(update_check_at_startup,	"Tjek altid for opdateringer ved opstart");  // this text has been changed, need correction!
	InsertGuiPair(update_check_for_info,	"Tryk p� N�ste for at tjekke for opdateringen...");
	InsertGuiPair(update_is_new_version,	"Version %d.%d.%d er tilg�ngelig, tryk p� N�ste for at downloade...");
	InsertGuiPair(update_no_new_version1,	"Der er ingen ny version tilg�ngelig.");
	InsertGuiPair(update_no_new_version2,	"");
	InsertGuiPair(update_download_from,		"Downloader fra...");
#ifndef TTCALC_PORTABLE
	InsertGuiPair(update_downloaded_info1,	"Et nyt installerings program er blevet downloadet.");
	InsertGuiPair(update_downloaded_info2,	"Tryk F�rdig for at lukke TTCalc og k�re installerings programmet.");
#else
	InsertGuiPair(update_downloaded_info1,	"En ny version af TTCalc er blevet downloadet.");
	InsertGuiPair(update_downloaded_info2,	"");
#endif
	InsertGuiPair(update_download_error,	"Der var et problem ved download, pr�v igen senere.");

	InsertGuiPair(pad_title,				"Redigeringsvindue");
	InsertGuiPair(pad_menu_file,			"&Fil");
	InsertGuiPair(pad_menu_edit,			"&Rediger");
	InsertGuiPair(pad_menu_file_new,		"&Ny"); // temporarily not used
	InsertGuiPair(pad_menu_file_open,		"&�ben...");
	InsertGuiPair(pad_menu_file_saveas,		"Gem &som...");
	InsertGuiPair(pad_menu_file_close,		"&Luk");
	InsertGuiPair(pad_menu_edit_undo,		"&Fortryd \tCtrl+Z");
	InsertGuiPair(pad_menu_edit_cut,		"Kli&p \tCtrl+X");
	InsertGuiPair(pad_menu_edit_copy,		"&Kopier \tCtrl+C");
	InsertGuiPair(pad_menu_edit_paste,		"&S�t ind \tCtrl+V");
	InsertGuiPair(pad_menu_edit_del,		"&Del \tDel");
	InsertGuiPair(pad_menu_edit_select_all,	"Marker &alt \tCtrl+A");

	InsertGuiPair(cannot_open_file,			"I cannot open the file");
	InsertGuiPair(cannot_save_file,			"I cannot save to such a file");
	InsertGuiPair(file_too_long,			"The file is too long");
	InsertGuiPair(other_error,				"There was a problem with this operation"); // for other kinds of errors



/*
	chinese gui messages
*/
	gui_messages_tab.push_back( std::map<GuiMsg, std::string>() );

	InsertGuiPair(button_ok,"ȷ��");
	InsertGuiPair(button_cancel,"ȡ��");
	InsertGuiPair(message_box_caption,"TTCalc");

	InsertGuiPair(dialog_box_add_variable_caption,"����±���");
	InsertGuiPair(dialog_box_edit_variable_caption,"�༭����");
	InsertGuiPair(dialog_box_add_variable_incorrect_name,"����ı�����");
	InsertGuiPair(dialog_box_add_variable_incorrect_value,"����ı���ֵ");
	InsertGuiPair(dialog_box_add_variable_variable_exists,"�˱����Ѵ���");
	InsertGuiPair(dialog_box_edit_variable_unknown_variable,"�����û�д˱������������ڲ�����");
	InsertGuiPair(dialog_box_delete_variable_confirm,"��ȷ��Ҫɾ����Щ������");
	InsertGuiPair(dialog_box_variable_not_all_deleted,"һЩ��������ɾ�����������ڲ�����");
	InsertGuiPair(dialog_box_add_function_caption,"����º���");
	InsertGuiPair(dialog_box_edit_function_caption,"�༭����");
	InsertGuiPair(dialog_box_add_function_function_exists,"�˺����Ѵ���");
	InsertGuiPair(dialog_box_edit_function_unknown_function,"�����û�д˺������������ڲ�����");
	InsertGuiPair(dialog_box_delete_function_confirm,"��ȷ��Ҫɾ����Щ������");
	InsertGuiPair(dialog_box_function_not_all_deleted,"һЩ��������ɾ�����������ڲ�����");
	InsertGuiPair(dialog_box_add_function_incorrect_name,"����ĺ�����");

	InsertGuiPair(list_variables_header_1,"����");
	InsertGuiPair(list_variables_header_2,"ֵ");
	InsertGuiPair(list_functions_header_1,"������");
	InsertGuiPair(list_functions_header_2,"��������");
	InsertGuiPair(list_functions_header_3,"���ʽ");

	InsertGuiPair(button_add,"���");
	InsertGuiPair(button_edit,"�༭");
	InsertGuiPair(button_delete,"ɾ��");
	InsertGuiPair(button_clear,"C");
	InsertGuiPair(tab_standard,"��׼");
	InsertGuiPair(tab_variables,"����");
	InsertGuiPair(tab_functions,"����");
	InsertGuiPair(tab_precision,"����");
	InsertGuiPair(tab_display,"��ʾ");
	InsertGuiPair(tab_convert,"ת��");

	InsertGuiPair(radio_precision_1,"С -   96 λ β��,  32λ ָ��");
	InsertGuiPair(radio_precision_2,"�� -  512 λ β��,  64λ ָ��");
	InsertGuiPair(radio_precision_3,"�� - 1024 λ β��, 128λ ָ��");


	InsertGuiPair(precision_1_info,"(+/-)6.97...e+646457021, 26λ��Ч����(ʮ����)");
	InsertGuiPair(precision_2_info,"(+/-)9.25...e+2776511644261678719, 152λ��Ч����");
	InsertGuiPair(precision_3_info,"(+/-)9.80...*10^(10^37), 306λ��Ч����");

	InsertGuiPair(overflow_during_printing,"���ʱ�������");

	InsertGuiPair(combo_rounding_none,"��");
	InsertGuiPair(combo_rounding_integer,"����");
	InsertGuiPair(combo_rounding_to_number,"ת��");
	InsertGuiPair(combo_rounding_after_comma,"λ��");
	InsertGuiPair(check_remove_zeroes,"ȥ��β����");

	InsertGuiPair(display_input,			"����");
	InsertGuiPair(display_output,			"���");
	InsertGuiPair(display_rounding,			"��������");
	InsertGuiPair(display_always_scientific,"һֱ");
	InsertGuiPair(display_not_always_scientific,"��ָ������:");
	InsertGuiPair(display_as_scientific,	"����ѧ������������");
	InsertGuiPair(display_deg_rad_grad,		"���Ǻ���");
	InsertGuiPair(display_grouping,			"�����");					// e.g. 1`000`000
	InsertGuiPair(display_grouping_none,	"��");
	InsertGuiPair(display_grouping_space,	"�ո�");
	InsertGuiPair(display_grouping_digits,  "λ��");
	InsertGuiPair(display_input_decimal_point,	"С���������ʽ"); 	// Decimal point in formula
	InsertGuiPair(display_input_decimal_point_item1,	" . or ,");     	
	InsertGuiPair(display_output_decimal_point,	"С���������ʽ");	// Decimal point in result
	InsertGuiPair(display_param_sep,		"�����ָ���");
	InsertGuiPair(display_param_sep_item2,	" ; or .");
	InsertGuiPair(display_param_sep_item3,	" ; or ,");

	InsertGuiPair(convert_type,				"����");
	InsertGuiPair(convert_input,			"����");
	InsertGuiPair(convert_output,			"���");
	InsertGuiPair(convert_dynamic_output,	"�Զ�");

	InsertGuiPair(menu_view,				"&�鿴");
	InsertGuiPair(menu_edit,				"&�༭");
	InsertGuiPair(menu_help,				"&����");
	InsertGuiPair(menu_language,			"&����");
	InsertGuiPair(menu_view_new_window,		"&�´���");
	InsertGuiPair(menu_view_normal_view,	"&��ͨ��ͼ");
	InsertGuiPair(menu_view_compact_view,	"&������ͼ");
	InsertGuiPair(menu_view_always_on_top,	"&������ǰ");
	InsertGuiPair(menu_view_pad,			"&�ı�������");
	InsertGuiPair(menu_view_lang_english,	"&Ӣ��");
	InsertGuiPair(menu_view_lang_polish,	"&������");
	InsertGuiPair(menu_view_lang_spanish,	"&��������");
	InsertGuiPair(menu_view_lang_danish,	"&������");
	InsertGuiPair(menu_view_lang_chinese,	"&��������");
	InsertGuiPair(menu_view_lang_russian,	"&����");
	InsertGuiPair(menu_view_lang_swedish,   "&�����");
	InsertGuiPair(menu_view_lang_italian,   "&�������");
	InsertGuiPair(menu_view_lang_german,    "&����");
	InsertGuiPair(menu_view_close_program,	"&�ر�");
	InsertGuiPair(menu_edit_undo,			"&ȡ�� \tCtrl+Z");
	InsertGuiPair(menu_edit_cut,			"&���� \tCtrl+X");
	InsertGuiPair(menu_edit_copy,			"&���� \tCtrl+C");
	InsertGuiPair(menu_edit_paste,			"&ճ��");						// cutting, copying, pasting the the edit which has a focus
	InsertGuiPair(menu_edit_del,			"&ɾ�� \tDel");
	InsertGuiPair(menu_edit_copy_both,		"&����ȫ��(����=���) \tCtrl+B");			// copying in this way: "input edit = output edit"
	InsertGuiPair(menu_edit_paste_formula,	"&ճ���������\tCtrl+V");		// pasting directly to the input edit
	InsertGuiPair(menu_edit_copy_result,	"&���ƽ�� \tCtrl+R");
	InsertGuiPair(menu_edit_select_all,		"&ȫѡ\tEsc");
	InsertGuiPair(menu_edit_bracket,		"&������ \tCtrl+0");			// inserting brackets: (...)
	InsertGuiPair(menu_edit_swap,			"&��������� \tCtrl+W");				// swapping input edit for output edit
	InsertGuiPair(menu_help_help,			"&����");
	InsertGuiPair(menu_help_project_page,	"&�����ҳ");
	InsertGuiPair(menu_help_about,			"&����");
	InsertGuiPair(menu_help_check_update,	"&������");
	InsertGuiPair(menu_update_available,	"&�п��õ��°汾"); // not implemented yet
	// this will be a new main menu item (one after 'help') visible only if there is an update

	InsertGuiPair(cant_init_calculations,	"���ܳ�ʼ��������ģ��");
	InsertGuiPair(message_box_error_caption,"TTCalc");
	InsertGuiPair(cant_create_thread,		"���ܴ��������߳�������");
	InsertGuiPair(cant_create_pad,			"���ܴ����ı����㴰��");
	InsertGuiPair(cant_create_main_window,	"���ܴ���Ӧ�ó���������");
	InsertGuiPair(cant_init_common_controls,"���ܳ�ʼ��ͨ�ÿؼ�");
	InsertGuiPair(about_text,
		"Mathematical calculator TTCalc %d.%d.%d%s%s\r\n\r\n"
		"������: Tomasz Sowa\r\n"
		"��ϵ: t.sowa@ttmath.org\r\n"
		"���: BSD (��Դ��Ŀ)\r\n"
		"��ҳ: http://ttcalc.sourceforge.net\r\n"
		"Bignum library: TTMath %d.%d.%d%s (%s)\r\n"
		"Bignum type: binary floating point numbers\r\n"
		"�������: C++\r\n"
		"������: %s\r\n"
		"%s"	// for upx
		"\r\n"
		"TTCalc uses the TTMath bignum library"
		" which can be found at http://www.ttmath.org\r\n"
		"\r\n"
#ifdef TTCALC_PORTABLE
		"This is the portable version of the program TTCalc. In this version "
		"you can calculate only with one kind of precision (96 bits for the "
		"mantissa and 32 bits for the exponent) it's about +/-6.9e+646457021.\r\n"
		"\r\n"
#endif
		"����:\r\n"
		"��������	Alejandro S. Valdezate\r\n"
		"		alesanval@gmail.com\r\n"
		"������		Rune Bisgaard Vammen\r\n"
		"		runebisgaard@gmail.com\r\n"
		"��������	Juis\r\n"
		"		zsyfly@gmail.com\r\n"
		"����		Vladimir Gladilovich\r\n"
		"		vdgladilovich@gmail.com\r\n"
		"�����		Lars 'Bafvert' Gafvert\r\n"
		"		lars.gafvert@gmail.com\r\n"
		"�������		Damiano Monaco\r\n"
		"		dmonax@gmail.com\r\n"
		"����		Moritz Beleites\r\n"
		"		morbel@gmx.net\r\n"
		);
	InsertGuiPair(about_text_portable_version, " portable version");
	InsertGuiPair(about_text_exe_packer,	"EXE Packer: UPX 3.07\r\n");
	InsertGuiPair(about_box_title,			"����");
	InsertGuiPair(about_box_button_close,	"�ر�");
	InsertGuiPair(unknown_error,			"����δ֪����");
	InsertGuiPair(cant_find_help,			"û���ҵ������ļ�");
	InsertGuiPair(cant_open_project_page,   "���ܴ������ҳ");

	InsertGuiPair(update_title,				"������");
	InsertGuiPair(update_button_next,		"��һ��");
	InsertGuiPair(update_button_finish,		"���");
	InsertGuiPair(update_button_cancel,		"ȡ��");
	InsertGuiPair(update_check_at_startup,	"����ʱ���Ǽ�����");  // this text has been changed, need correction!
	InsertGuiPair(update_check_for_info,	"��\"��һ��\"���и���...");
	InsertGuiPair(update_is_new_version,	"�汾 %d.%d.%d ����, ��\"��һ��\"����...");
	InsertGuiPair(update_no_new_version1,	"û�п��õ��°汾��");
	InsertGuiPair(update_no_new_version2,	"");
	InsertGuiPair(update_download_from,		"��������...");
#ifndef TTCALC_PORTABLE
	InsertGuiPair(update_downloaded_info1,	"�°汾��װ�����Ѿ�������ϡ�");
	InsertGuiPair(update_downloaded_info2,	"�������Թرճ��򲢿�ʼ��װ��.");
#else
	InsertGuiPair(update_downloaded_info1,	"�°汾��װ�����Ѿ�������ϡ�");
	InsertGuiPair(update_downloaded_info2,	"");
#endif
	InsertGuiPair(update_download_error,	"����ʱ���ִ������Ժ����ԡ�");

	InsertGuiPair(pad_title,				"&�ı�������");
	InsertGuiPair(pad_menu_file,			"&�ļ�");
	InsertGuiPair(pad_menu_edit,			"&�༭");
	InsertGuiPair(pad_menu_file_new,		"&�½�"); // temporarily not used
	InsertGuiPair(pad_menu_file_open,		"&��...");
	InsertGuiPair(pad_menu_file_saveas,		"&���Ϊ...");
	InsertGuiPair(pad_menu_file_close,		"&�ر�");
	InsertGuiPair(pad_menu_edit_undo,		"&���� \tCtrl+Z");
	InsertGuiPair(pad_menu_edit_cut,		"&���� \tCtrl+X");
	InsertGuiPair(pad_menu_edit_copy,		"&���� \tCtrl+C");
	InsertGuiPair(pad_menu_edit_paste,		"&ճ�� \tCtrl+V");
	InsertGuiPair(pad_menu_edit_del,		"&ɾ�� \tDel");
	InsertGuiPair(pad_menu_edit_select_all,	"&ȫѡ \tCtrl+A");

	InsertGuiPair(cannot_open_file,			"δ�ܴ��ļ�");
	InsertGuiPair(cannot_save_file,			"���ܱ����ļ�");
	InsertGuiPair(file_too_long,			"�ļ�̫��");
	InsertGuiPair(other_error,				"There was a problem with this operation"); // for other kinds of errors




/*
	russian gui messages
*/
	gui_messages_tab.push_back( std::map<GuiMsg, std::string>() );

	InsertGuiPair(button_ok,"Ok");
	InsertGuiPair(button_cancel,"������");
	InsertGuiPair(message_box_caption,"TTCalc");

	InsertGuiPair(dialog_box_add_variable_caption,"�������� ����� ����������");
	InsertGuiPair(dialog_box_edit_variable_caption,"������������� ����������");
	InsertGuiPair(dialog_box_add_variable_incorrect_name,"������������ ��� ����������");
	InsertGuiPair(dialog_box_add_variable_incorrect_value,"������������ �������� ����������");
	InsertGuiPair(dialog_box_add_variable_variable_exists,"����� ���������� ��� ������");
	InsertGuiPair(dialog_box_edit_variable_unknown_variable,"����� ���������� ��� � �������. ��������, ���������� ������!");
	InsertGuiPair(dialog_box_delete_variable_confirm,"������� ��� ����������?");
	InsertGuiPair(dialog_box_variable_not_all_deleted,"��������� ���������� �� ���������� �������. ��������, ���������� ������!");
	InsertGuiPair(dialog_box_add_function_caption,"�������� ����� �������");
	InsertGuiPair(dialog_box_edit_function_caption,"������������� ����� �������");
	InsertGuiPair(dialog_box_add_function_function_exists,"����� ������� ��� ������");
	InsertGuiPair(dialog_box_edit_function_unknown_function,"����� ������� ��� � �������. ��������, ���������� ������!");
	InsertGuiPair(dialog_box_delete_function_confirm,"������� ��� �������?");
	InsertGuiPair(dialog_box_function_not_all_deleted,"��������� ������� �� ���������� �������. ��������, ���������� ������!");
	InsertGuiPair(dialog_box_add_function_incorrect_name,"������������ ��� �������");

	InsertGuiPair(list_variables_header_1,"���");
	InsertGuiPair(list_variables_header_2,"��������");
	InsertGuiPair(list_functions_header_1,"���");
	InsertGuiPair(list_functions_header_2,"���������");
	InsertGuiPair(list_functions_header_3,"��������");

	InsertGuiPair(button_add,"��������");
	InsertGuiPair(button_edit,"������");
	InsertGuiPair(button_delete,"�������");
	InsertGuiPair(button_clear,"C");
	InsertGuiPair(tab_standard,"��������");
	InsertGuiPair(tab_variables,"����������");
	InsertGuiPair(tab_functions,"�������");
	InsertGuiPair(tab_precision,"��������");
	InsertGuiPair(tab_display,"�����");
	InsertGuiPair(tab_convert,"�����������");

	InsertGuiPair(radio_precision_1,"����� - 96 ��� �� ��������, 32 ���� �� ���������� �������");
	InsertGuiPair(radio_precision_2,"������� - 512 ��� �� ��������, 64 ���� �� ���������� �������");
	InsertGuiPair(radio_precision_3,"������� - 1024 ��� �� ��������, 128 ���� �� ���������� �������");


	InsertGuiPair(precision_1_info,"(+/-)6.97...e+646457021, 26 �������� ���� (����������)");
	InsertGuiPair(precision_2_info,"(+/-)9.25...e+2776511644261678719, 152 �������� ����");
	InsertGuiPair(precision_3_info,"(+/-)9.80...*10^(10^37), 306 �������� ����");

	InsertGuiPair(overflow_during_printing,"������������ ��� ������");

	InsertGuiPair(combo_rounding_none,"���");
	InsertGuiPair(combo_rounding_integer,"�� ������");
	InsertGuiPair(combo_rounding_to_number,"��");
	InsertGuiPair(combo_rounding_after_comma,"�����");
	InsertGuiPair(check_remove_zeroes,"������� ����. ������� �����");

	InsertGuiPair(display_input,			"����");
	InsertGuiPair(display_output,			"�����");
	InsertGuiPair(display_rounding,			"����������");
	InsertGuiPair(display_always_scientific,"������");
	InsertGuiPair(display_not_always_scientific,"����� ���������� ������� ������");
	InsertGuiPair(display_as_scientific,	"�������� ��������� � ���������������� ����");

	InsertGuiPair(display_deg_rad_grad,		"�������������");
	InsertGuiPair(display_grouping,			"����. �����");					// e.g. 1`000`000
	InsertGuiPair(display_grouping_none,	"������");
	InsertGuiPair(display_grouping_space,	"������");
	InsertGuiPair(display_grouping_digits,  "�����");
	InsertGuiPair(display_input_decimal_point,	"���������� ����� (����)"); 	// Decimal point in formula
	InsertGuiPair(display_input_decimal_point_item1,	" . ��� ,");     	
	InsertGuiPair(display_output_decimal_point,	"���������� ����� (�����)");	// Decimal point in result
	InsertGuiPair(display_param_sep,		"��������� �����������");
	InsertGuiPair(display_param_sep_item2,	" ; ��� .");
	InsertGuiPair(display_param_sep_item3,	" ; ��� ,");

	InsertGuiPair(convert_type,				"���");
	InsertGuiPair(convert_input,			"����");
	InsertGuiPair(convert_output,			"�����");
	InsertGuiPair(convert_dynamic_output,	"�����������");

	InsertGuiPair(menu_view,				"&���");
	InsertGuiPair(menu_edit,				"&������");
	InsertGuiPair(menu_help,				"�&�����");
	InsertGuiPair(menu_language,			"&����");
	InsertGuiPair(menu_view_new_window,		"&����� ����");
	InsertGuiPair(menu_view_normal_view,	"&������� ���");
	InsertGuiPair(menu_view_compact_view,	"&���������� ���");
	InsertGuiPair(menu_view_always_on_top,	"&������ ������ ������ ����");
	InsertGuiPair(menu_view_pad,			"�&������");
	InsertGuiPair(menu_view_lang_english,	"&����������");
	InsertGuiPair(menu_view_lang_polish,	"&��������");
	InsertGuiPair(menu_view_lang_spanish,	"&���������");
	InsertGuiPair(menu_view_lang_danish,	"&�������");
	InsertGuiPair(menu_view_lang_chinese,	"&���������");
	InsertGuiPair(menu_view_lang_russian,	"&�������");
	InsertGuiPair(menu_view_lang_swedish,   "&��������");
	InsertGuiPair(menu_view_lang_italian,   "&Italian"); // to translate
	InsertGuiPair(menu_view_lang_german,    "&German");  // to translate
	InsertGuiPair(menu_view_close_program,	"&�������");

	InsertGuiPair(menu_edit_undo,			"&�������� �������� \tCtrl+Z");
	InsertGuiPair(menu_edit_cut,			"��&������ \tCtrl+X");
	InsertGuiPair(menu_edit_copy,			"&���������� \tCtrl+C");
	InsertGuiPair(menu_edit_paste,			"&��������");							// cutting, copying, pasting the the edit which has a focus
	InsertGuiPair(menu_edit_del,			"&������� \tDel");
	InsertGuiPair(menu_edit_copy_result,	"�&��������� ��������� \tCtrl+R");
	InsertGuiPair(menu_edit_copy_both,		"���������� ���� &� ����� \tCtrl+B");	// copying in this way: "input edit = output edit"
	InsertGuiPair(menu_edit_paste_formula,	"�������� � &������� \tCtrl+V");		// pasting directly to the input edit
	InsertGuiPair(menu_edit_select_all,		"���&����� �� \tEsc");
	InsertGuiPair(menu_edit_bracket,		"�������� ���&��� \tCtrl+0");			// inserting brackets: (...)
	InsertGuiPair(menu_edit_swap,			"&Swap \tCtrl+W");						// swapping input edit for output edit
	InsertGuiPair(menu_help_check_update,	"��������� �&���������...");
	InsertGuiPair(menu_update_available,	"&�������� ����� ������!");

	InsertGuiPair(menu_help_help,			"&������");
	InsertGuiPair(menu_help_project_page,	"&���� �������");
	InsertGuiPair(menu_help_about,			"&� ���������");

	InsertGuiPair(cant_init_calculations,	"�� ���� ���������������� ������ ��� ��������");
	InsertGuiPair(message_box_error_caption,"TTCalc");
	InsertGuiPair(cant_create_thread,		"�� ���� ������� ������ ����� ��� ��������");
	InsertGuiPair(cant_create_pad,			"I could not create the pad window");
	InsertGuiPair(cant_create_main_window,	"�� ���� ������� ������� ���� ���������");
	InsertGuiPair(cant_init_common_controls,"�� ���� ���������������� ���������� (InitCommonControlsEx)");
	InsertGuiPair(about_text,
		"�������������� ����������� TTCalc %d.%d.%d%s%s\r\n\r\n"
		"�����: Tomasz Sowa\r\n"
		"e-mail: t.sowa@ttmath.org\r\n"
		"��������: BSD (open source)\r\n"
		"���� �������: http://ttcalc.sourceforge.net\r\n"
		"���������� Bignum: TTMath %d.%d.%d%s (%s)\r\n"
		"��� Bignum: �������� ������� ���������, ����� � ��������� �������\r\n"
		"���� ����������������: C++\r\n"
		"����������: %s\r\n"
		"%s"	// for upx
		"\r\n"
		"TTCalc ���������� ���������� ������������ ����� TTMath,"
		" ������� ��������� �����: http://www.ttmath.org\r\n"
		"\r\n"
#ifdef TTCALC_PORTABLE
		"��� ���������� ������ ���������. � ���� ������ ����� ������� "
		"������ � ����� (�����) ��������� (96 ��� �� ��������,"
		" 32 ���� �� ���������� �������), �� ���� +/-6.9e+646457021.\r\n"
		"\r\n"
#endif

		"�����������:\r\n"
		"���������	Alejandro S. Valdezate\r\n"
		"		alesanval@gmail.com\r\n"
		"�������		Rune Bisgaard Vammen\r\n"
		"		runebisgaard@gmail.com\r\n"
		"���������	Juis\r\n"
		"		zsyfly@gmail.com\r\n"
		"�������		Vladimir Gladilovich\r\n"
		"		vdgladilovich@gmail.com\r\n"
		"��������		Lars 'Bafvert' Gafvert\r\n"
		"		lars.gafvert@gmail.com\r\n"
		"Italian		Damiano Monaco\r\n"
		"		dmonax@gmail.com\r\n"
		"German		Moritz Beleites\r\n"
		"		morbel@gmx.net\r\n"
		);
	InsertGuiPair(about_text_portable_version, " ���������� ������");
	InsertGuiPair(about_text_exe_packer,	"��������� EXE: UPX 3.07\r\n");
	InsertGuiPair(about_box_title,			"� ���������");
	InsertGuiPair(about_box_button_close,	"�������");
	InsertGuiPair(unknown_error,			"����������� ������");
	InsertGuiPair(cant_find_help,			"�� ������� ����� ����� ������");
	InsertGuiPair(cant_open_project_page,   "�� ������� ������� ���� �������");


	InsertGuiPair(update_title,				"����� ����� ������");
	InsertGuiPair(update_button_next,		"�����");
	InsertGuiPair(update_button_finish,		"������");
	InsertGuiPair(update_button_cancel,		"������");
	InsertGuiPair(update_check_at_startup,	"��������� ������� ���������� ��� �������� ���������");  // this text has been changed, need correction!
	InsertGuiPair(update_check_for_info,	"������� �����, ����� ��������� ������� ����������...");
	InsertGuiPair(update_is_new_version,	"�������� ������ %d.%d.%d, ������� �����, ����� ������� �...");
	InsertGuiPair(update_no_new_version1,	"��� ����� ������.");
	InsertGuiPair(update_no_new_version2,	"");
	InsertGuiPair(update_download_from,		"��������� �...");
#ifndef TTCALC_PORTABLE
	InsertGuiPair(update_downloaded_info1,	"����� ����������� ��������.");
	InsertGuiPair(update_downloaded_info2,	"������� ������, ����� ������� TTCalc � ��������� ����������.");
#else
	InsertGuiPair(update_downloaded_info1,	"����� ����������� ��������.");
	InsertGuiPair(update_downloaded_info2,	"");
#endif
	InsertGuiPair(update_download_error,	"�������� � ���������, ���������� ��� ��� �������.");

	InsertGuiPair(pad_title,				"�������");
	InsertGuiPair(pad_menu_file,			"&����");
	InsertGuiPair(pad_menu_edit,			"&������");
	InsertGuiPair(pad_menu_file_new,		"&�����"); // temporarily not used
	InsertGuiPair(pad_menu_file_open,		"&�������...");
	InsertGuiPair(pad_menu_file_saveas,		"��������� &���...");
	InsertGuiPair(pad_menu_file_close,		"&�������");
	InsertGuiPair(pad_menu_edit_undo,		"&�������� \tCtrl+Z");
	InsertGuiPair(pad_menu_edit_cut,		"��&������ \tCtrl+X");
	InsertGuiPair(pad_menu_edit_copy,		"&���������� \tCtrl+C");
	InsertGuiPair(pad_menu_edit_paste,		"&�������� \tCtrl+V");
	InsertGuiPair(pad_menu_edit_del,		"&������� \tDel");
	InsertGuiPair(pad_menu_edit_select_all,	"���&����� �� \tCtrl+A");

	InsertGuiPair(cannot_open_file,			"I cannot open the file");
	InsertGuiPair(cannot_save_file,			"I cannot save to such a file");
	InsertGuiPair(file_too_long,			"The file is too long");
	InsertGuiPair(other_error,				"There was a problem with this operation"); // for other kinds of errors





/*
	swedish gui messages
*/
	gui_messages_tab.push_back( std::map<GuiMsg, std::string>() );

	InsertGuiPair(button_ok,"Ok");
	InsertGuiPair(button_cancel,"Avbryt");
	InsertGuiPair(message_box_caption,"TTCalc");

	InsertGuiPair(dialog_box_add_variable_caption,"L�gg till en ny variabel");
	InsertGuiPair(dialog_box_edit_variable_caption,"Redigera en variabel");
	InsertGuiPair(dialog_box_add_variable_incorrect_name,"Ogiltigt namn f�r variabel");
	InsertGuiPair(dialog_box_add_variable_incorrect_value,"Ogiltigt v�rde f�r variabel");
	InsertGuiPair(dialog_box_add_variable_variable_exists,"Denna variabel finns redan");
	InsertGuiPair(dialog_box_edit_variable_unknown_variable,"Denna variabel saknas i tabellen. F�rmodligen ett internt fel!");
	InsertGuiPair(dialog_box_delete_variable_confirm,"Vill du f�rg�ra dessa variabler?");
	InsertGuiPair(dialog_box_variable_not_all_deleted,"N�gra variabler gick ej att ta k�l p�. Det m�ste vara ett internt fel!");
	InsertGuiPair(dialog_box_add_function_caption,"L�gg till en ny funktion");
	InsertGuiPair(dialog_box_edit_function_caption,"Redigera en funktion");
	InsertGuiPair(dialog_box_add_function_function_exists,"Denna funktion finns redan");
	InsertGuiPair(dialog_box_edit_function_unknown_function,"Denna funktion saknas i tabellen. F�rmodligen ett internt fel!");
	InsertGuiPair(dialog_box_delete_function_confirm,"Vill du f�rg�ra dessa funktioner?");
	InsertGuiPair(dialog_box_function_not_all_deleted,"N�gra funktioner gick inte att f�rg�ra. Jag tror vi har att g�ra med ett internt fel!");
	InsertGuiPair(dialog_box_add_function_incorrect_name,"Ogiltigt namn f�r funktionen");

	InsertGuiPair(list_variables_header_1,"Namn");
	InsertGuiPair(list_variables_header_2,"V�rde");
	InsertGuiPair(list_functions_header_1,"Namn");
	InsertGuiPair(list_functions_header_2,"Param.");
	InsertGuiPair(list_functions_header_3,"V�rde");

	InsertGuiPair(button_add,"L�gga till");
	InsertGuiPair(button_edit,"Redigera");
	InsertGuiPair(button_delete,"F�rg�ra");
	InsertGuiPair(button_clear,"C");
	InsertGuiPair(tab_standard,"Standard");
	InsertGuiPair(tab_variables,"Variabler");
	InsertGuiPair(tab_functions,"Funktioner");
	InsertGuiPair(tab_precision,"Precision");
	InsertGuiPair(tab_display,"Visning");
	InsertGuiPair(tab_convert,"Konvertera");

	InsertGuiPair(radio_precision_1,"Liten - 96 bitar f�r mantissan, 32 bitar f�r exponenten");
	InsertGuiPair(radio_precision_2,"Mellan - 512 bitar f�r mantissan, 64 bitar f�r exponenten");
	InsertGuiPair(radio_precision_3,"Stor - 1024 bitar f�r mantissan, 128 bitar f�r exponenten");


	InsertGuiPair(precision_1_info,"(+/-)6.97...e+646457021, 26 giltiga siffror (decimaler)");
	InsertGuiPair(precision_2_info,"(+/-)9.25...e+2776511644261678719, 152 giltiga siffror");
	InsertGuiPair(precision_3_info,"(+/-)9.80...*10^(10^37), 306 giltiga siffror");

	InsertGuiPair(overflow_during_printing,"Overflow under printning");

	InsertGuiPair(combo_rounding_none,"Ingen");
	InsertGuiPair(combo_rounding_integer,"till heltal");
	InsertGuiPair(combo_rounding_to_number,"till");
	InsertGuiPair(combo_rounding_after_comma,"siffror(cifre)");
	InsertGuiPair(check_remove_zeroes,"Radera sista �verfl�diga nollor");


	InsertGuiPair(display_input,			"Input");
	InsertGuiPair(display_output,			"Output");
	InsertGuiPair(display_rounding,			"Avrundning");
	InsertGuiPair(display_always_scientific,"Alltid");
	InsertGuiPair(display_not_always_scientific,"N�r exponenten �r st�rre �n:");
	InsertGuiPair(display_as_scientific,	"Visa resultatet som vetenskapligt v�rde");
	InsertGuiPair(display_deg_rad_grad,		"Trigonometric functions");
	InsertGuiPair(convert_type,				"Type");
	InsertGuiPair(convert_input,			"Input");
	InsertGuiPair(convert_output,			"Output");
	InsertGuiPair(convert_dynamic_output,	"Auto prefix");
	InsertGuiPair(display_grouping,			"Grouping");
	InsertGuiPair(display_grouping_none,	"None");
	InsertGuiPair(display_grouping_space,	"Space");
	InsertGuiPair(display_grouping_digits,  "siffror(cifre)");
	InsertGuiPair(display_input_decimal_point,	"Input decimal point");
	InsertGuiPair(display_input_decimal_point_item1,	" . or ,");
	InsertGuiPair(display_output_decimal_point,	"Output decimal point");
	InsertGuiPair(display_param_sep,		"Parameters separated by");
	InsertGuiPair(display_param_sep_item2,	" ; or .");
	InsertGuiPair(display_param_sep_item3,	" ; or ,");

	InsertGuiPair(menu_view,				"&Visa");
	InsertGuiPair(menu_edit,				"&Redigera");
	InsertGuiPair(menu_help,				"&Hj�lp");
	InsertGuiPair(menu_language,			"&Spr�g");
	InsertGuiPair(menu_view_new_window,		"&Nytt f�nster");
	InsertGuiPair(menu_view_normal_view,	"No&rmal visning");
	InsertGuiPair(menu_view_compact_view,	"K&ompakt visning");
	InsertGuiPair(menu_view_always_on_top,	"&Alltid �verst");
	InsertGuiPair(menu_view_pad,			"&Pad");
	InsertGuiPair(menu_view_lang_english,	"&Engelska");
	InsertGuiPair(menu_view_lang_polish,	"&Polska");
	InsertGuiPair(menu_view_lang_spanish,	"&Spanska");
	InsertGuiPair(menu_view_lang_danish,	"&Danska");
	InsertGuiPair(menu_view_lang_chinese,	"&Kinesiska");
	InsertGuiPair(menu_view_lang_russian,	"&Ryska");
	InsertGuiPair(menu_view_lang_swedish,   "S&venska");
	InsertGuiPair(menu_view_lang_italian,   "&Italian");
	InsertGuiPair(menu_view_lang_german,    "&Tyska"); // is it a good translation?
	InsertGuiPair(menu_view_close_program,	"A&vsluta");
	InsertGuiPair(menu_edit_undo,			"&�ngra \tCtrl+Z");
	InsertGuiPair(menu_edit_cut,			"Cu&t \tCtrl+X");
	InsertGuiPair(menu_edit_copy,			"&Copy \tCtrl+C");
	InsertGuiPair(menu_edit_paste,			"&Paste");
	InsertGuiPair(menu_edit_del,			"&Delete \tDel");
	InsertGuiPair(menu_edit_paste_formula,	"Klistra &in");
	InsertGuiPair(menu_edit_copy_result,	"&Kopiera result \tCtrl+R");
	InsertGuiPair(menu_edit_copy_both,		"Copy &both \tCtrl+B");
	InsertGuiPair(menu_edit_select_all,		"Select &all \tEsc");
	InsertGuiPair(menu_edit_bracket,		"B&racket \tCtrl+0");
	InsertGuiPair(menu_edit_swap,			"&Swap \tCtrl+W");
	InsertGuiPair(menu_help_help,			"&Hj�lp");
	InsertGuiPair(menu_help_project_page,	"&Projektsida");
	InsertGuiPair(menu_help_check_update,	"&Check update...");
	InsertGuiPair(menu_help_about,			"&Om");
	InsertGuiPair(menu_update_available,	"&New version is available");

	InsertGuiPair(cant_init_calculations,	"Ber�kningen kunde inte initieras!");
	InsertGuiPair(message_box_error_caption,"TTCalc");
	InsertGuiPair(cant_create_thread,		"Den andra tr�den f�r utr�kning kunde inte skapas");
	InsertGuiPair(cant_create_pad,			"I could not create the pad window");
	InsertGuiPair(cant_create_main_window,	"Applikationens huvudf�nster kunde inte skapas");
	InsertGuiPair(cant_init_common_controls,"Inst�llningarna kunde inte initieras (InitCommonControlsEx)");
	InsertGuiPair(about_text,
		"Matematisk kalkylator TTCalc %d.%d.%d%s%s\r\n\r\n"
		"Upphovsman: Tomasz Sowa\r\n"
		"Kontakt: t.sowa@ttmath.org\r\n"
		"Licens: BSD (open source)\r\n"
		"Projektsida: http://ttcalc.sourceforge.net\r\n"
		"Bignumbiblotek: TTMath %d.%d.%d%s (%s)\r\n"
		"Bignum type: binary floating point numbers\r\n"
		"Programmeringsspr�k: C++\r\n"
		"Kompilator: %s\r\n"
		"%s"	// for upx
		"\r\n"
		"TTCalc anv�nder sig av TTMath bignum bibloteket"
		" som kan hittas h�r: http://www.ttmath.org\r\n"
		"\r\n"
#ifdef TTCALC_PORTABLE
		"Detta �r den portabla versionen av TTCalc. I denna version "
		"kan du r�kna med begr�nsad precision, dvs (96 bitar f�r "
		"mantissan och 32 bitar f�r exponenten) det �r omkring +/-6.9e+646457021.\r\n"
		"\r\n"
#endif
		"Translations:\r\n"
		"Spanish	Alejandro S. Valdezate\r\n"
		"	alesanval@gmail.com\r\n"
		"Danish	Rune Bisgaard Vammen\r\n"
		"	runebisgaard@gmail.com\r\n"
		"Chinese	Juis\r\n"
		"	zsyfly@gmail.com\r\n"
		"Russian	Vladimir Gladilovich\r\n"
		"	vdgladilovich@gmail.com\r\n"
		"Swedish	Lars 'Bafvert' Gafvert\r\n"
		"	lars.gafvert@gmail.com\r\n"
		"Italian	Damiano Monaco\r\n"
		"	dmonax@gmail.com\r\n"
		"Tyska	Moritz Beleites\r\n"
		"	morbel@gmx.net\r\n"
		);
	InsertGuiPair(about_text_portable_version, " transportabel version");
	InsertGuiPair(about_text_exe_packer,	"EXE Packer: UPX 3.07\r\n");
	InsertGuiPair(about_box_title,			"Om");
	InsertGuiPair(about_box_button_close,	"B�ver");
	InsertGuiPair(unknown_error,			"Det har intr�ffat ett ok�nt fel");
	InsertGuiPair(cant_find_help,			"Kunde inte hitta n�gra hj�lpfiler");
	InsertGuiPair(cant_open_project_page,   "Projektsidan kunde inte �ppnas");
	InsertGuiPair(update_title,				"Check for a new version");
	InsertGuiPair(update_button_next,		"Next");
	InsertGuiPair(update_button_finish,		"Finish");
	InsertGuiPair(update_button_cancel,		"Cancel");
	InsertGuiPair(update_check_at_startup,	"Always check for the update on startup");
	InsertGuiPair(update_check_for_info,	"Press Next to check for the update...");
	InsertGuiPair(update_is_new_version,	"Version %d.%d.%d is available, press Next to download...");
	InsertGuiPair(update_no_new_version1,	"There is not a newer version available.");
	InsertGuiPair(update_no_new_version2,	"You have the newest version.");
	InsertGuiPair(update_download_from,		"Downloading from...");
#ifndef TTCALC_PORTABLE
	InsertGuiPair(update_downloaded_info1,	"A new setup program has been downloaded.");
	InsertGuiPair(update_downloaded_info2,	"Press Finish to close TTCalc and run the installer.");
#else
	InsertGuiPair(update_downloaded_info1,	"A new version of TTCalc has been downloaded.");
	InsertGuiPair(update_downloaded_info2,	"");
#endif
	InsertGuiPair(update_download_error,	"There was a problem with downloading, please try again later.");

	InsertGuiPair(pad_title,				"Pad");
	InsertGuiPair(pad_menu_file,			"&File");
	InsertGuiPair(pad_menu_edit,			"&Edit");
	InsertGuiPair(pad_menu_file_new,		"&New"); // temporarily not used
	InsertGuiPair(pad_menu_file_open,		"&Open...");
	InsertGuiPair(pad_menu_file_saveas,		"Save &as...");
	InsertGuiPair(pad_menu_file_close,		"&Close");
	InsertGuiPair(pad_menu_edit_undo,		"&Undo \tCtrl+Z");
	InsertGuiPair(pad_menu_edit_cut,		"Cu&t \tCtrl+X");
	InsertGuiPair(pad_menu_edit_copy,		"&Copy \tCtrl+C");
	InsertGuiPair(pad_menu_edit_paste,		"&Paste \tCtrl+V");
	InsertGuiPair(pad_menu_edit_del,		"&Del \tDel");
	InsertGuiPair(pad_menu_edit_select_all,	"Select &all \tCtrl+A");

	InsertGuiPair(cannot_open_file,			"I cannot open the file");
	InsertGuiPair(cannot_save_file,			"I cannot save to such a file");
	InsertGuiPair(file_too_long,			"The file is too long");
	InsertGuiPair(other_error,				"There was a problem with this operation"); // for other kinds of errors




/*
	italian gui messages
*/
	gui_messages_tab.push_back( std::map<GuiMsg, std::string>() );

	InsertGuiPair(button_ok,"Ok");
	InsertGuiPair(button_cancel,"Annulla");
	InsertGuiPair(message_box_caption,"TTCalc");

	InsertGuiPair(dialog_box_add_variable_caption,"Aggiungi una nuova variabile");
	InsertGuiPair(dialog_box_edit_variable_caption,"Modifica una variabile");
	InsertGuiPair(dialog_box_add_variable_incorrect_name,"Nome di variabile non corretto");
	InsertGuiPair(dialog_box_add_variable_incorrect_value,"Valore di variabile non corretto");
	InsertGuiPair(dialog_box_add_variable_variable_exists,"Questa variabile esiste gia");
	InsertGuiPair(dialog_box_edit_variable_unknown_variable,"Variabile non presente nella mia tabella. Probabilmente un errore interno!");
	InsertGuiPair(dialog_box_delete_variable_confirm,"Vuoi eliminare queste variabili?");
	InsertGuiPair(dialog_box_variable_not_all_deleted,"Non sono stato in grado di eliminare alcune variabili. Probabilmente un errore interno!");
	InsertGuiPair(dialog_box_add_function_caption,"Aggiungi una nuova funzione");
	InsertGuiPair(dialog_box_edit_function_caption,"Modifica una funzione");
	InsertGuiPair(dialog_box_add_function_function_exists,"Questa funzione esiste gia");
	InsertGuiPair(dialog_box_edit_function_unknown_function,"Funzione non presente nella mia tabella. Probabilmente un errore interno!");
	InsertGuiPair(dialog_box_delete_function_confirm,"Vuoi eliminare queste funzioni?");
	InsertGuiPair(dialog_box_function_not_all_deleted,"Non sono stato in grado di eliminare alcune funzioni. Probabilmente un errore interno!");
	InsertGuiPair(dialog_box_add_function_incorrect_name,"Nome di funzione non corretto");

	InsertGuiPair(list_variables_header_1,"Nome");
	InsertGuiPair(list_variables_header_2,"Valore");
	InsertGuiPair(list_functions_header_1,"Nome");
	InsertGuiPair(list_functions_header_2,"Param.");
	InsertGuiPair(list_functions_header_3,"Valore");

	InsertGuiPair(button_add,"Aggiungi");
	InsertGuiPair(button_edit,"Modifica");
	InsertGuiPair(button_delete,"Elimina");
	InsertGuiPair(button_clear,"C");
	InsertGuiPair(tab_standard,"Standard");
	InsertGuiPair(tab_variables,"Variabili");
	InsertGuiPair(tab_functions,"Funzioni");
	InsertGuiPair(tab_precision,"Precisione");
	InsertGuiPair(tab_display,"Display");
	InsertGuiPair(tab_convert,"Conversione");

	InsertGuiPair(radio_precision_1,"Piccola - 96 bit per la mantissa, 32 bit per l'esponente");
	InsertGuiPair(radio_precision_2,"Media - 512 bit per la mantissa, 64 bit per l'esponente");
	InsertGuiPair(radio_precision_3,"Grande - 1024 bit per la mantissa, 128 bit per l'esponente");

	InsertGuiPair(precision_1_info,"(+/-)6.97...e+646457021, 26 cifre decimali valide");
	InsertGuiPair(precision_2_info,"(+/-)9.25...e+2776511644261678719, 152 cifre decimali valide");
	InsertGuiPair(precision_3_info,"(+/-)9.80...*10^(10^37), 306 cifre decimali valide");

	InsertGuiPair(overflow_during_printing,"Overflow durante la stampa a video");

	InsertGuiPair(combo_rounding_none,"nessuno");
	InsertGuiPair(combo_rounding_integer,"ad intero");
	InsertGuiPair(combo_rounding_to_number,"a");
	InsertGuiPair(combo_rounding_after_comma,"cifra/e");
	InsertGuiPair(check_remove_zeroes,"Rimuovere ultimi zeri finali");

	InsertGuiPair(display_input,			"Bese di input");
	InsertGuiPair(display_output,			"Bese di output");
	InsertGuiPair(display_rounding,			"Arrotonda");
	InsertGuiPair(display_always_scientific,"Sempre");
	InsertGuiPair(display_not_always_scientific,"Quando l'esponente e maggiore di:");
	InsertGuiPair(display_as_scientific,	"Stampa il risultato in notazione scientifica");
	InsertGuiPair(display_deg_rad_grad,		"Funzioni trigonometriche");
	InsertGuiPair(display_grouping,			"Raggruppo");
	InsertGuiPair(display_grouping_none,	"nessuno");
	InsertGuiPair(display_grouping_space,	"spazio");
	InsertGuiPair(display_grouping_digits,  "cifra/e");
	InsertGuiPair(display_input_decimal_point,	"Punto decimale in input"); 	// Decimal point in formula
	InsertGuiPair(display_input_decimal_point_item1,	" . oppure ,");
	InsertGuiPair(display_output_decimal_point,	"Punto decimale in output");	// Decimal point in result
	InsertGuiPair(display_param_sep,		"Parametri separati da");
	InsertGuiPair(display_param_sep_item2,	" ; oppure .");
	InsertGuiPair(display_param_sep_item3,	" ; oppure ,");


	InsertGuiPair(convert_type,				"Tipo");
	InsertGuiPair(convert_input,			"Input");
	InsertGuiPair(convert_output,			"Output");
	InsertGuiPair(convert_dynamic_output,	"Prefisso automatico");

	InsertGuiPair(menu_view,				"&Vista");
	InsertGuiPair(menu_edit,				"&Modifica");
	InsertGuiPair(menu_help,				"&Aiuto");
	InsertGuiPair(menu_language,			"&Linguaggio");
	InsertGuiPair(menu_view_new_window,		"&Nuova finestra");
	InsertGuiPair(menu_view_normal_view,	"Vista &normale");
	InsertGuiPair(menu_view_compact_view,	"Vista &compatta");
	InsertGuiPair(menu_view_pad,			"&Bloc-notes"); 
	InsertGuiPair(menu_view_always_on_top,	"&In primo piano");
	InsertGuiPair(menu_view_lang_english,	"&Inglese");
	InsertGuiPair(menu_view_lang_polish,	"&Polacco");
	InsertGuiPair(menu_view_lang_spanish,	"&Spagnolo");
	InsertGuiPair(menu_view_lang_danish,	"&Danese");
	InsertGuiPair(menu_view_lang_chinese,	"&Cinese");
	InsertGuiPair(menu_view_lang_russian,	"&Russo");
	InsertGuiPair(menu_view_lang_swedish,   "S&vedese");
	InsertGuiPair(menu_view_lang_italian,   "I&taliano");
	InsertGuiPair(menu_view_lang_german,    "&Tedesco"); // is it a good translation?
	InsertGuiPair(menu_view_close_program,	"&Esci");
	InsertGuiPair(menu_edit_undo,			"&Annulla \tCtrl+Z");
	InsertGuiPair(menu_edit_cut,			"&Taglia \tCtrl+X");
	InsertGuiPair(menu_edit_copy,			"&Copia \tCtrl+C");
	InsertGuiPair(menu_edit_paste,			"&Incolla");					// cutting, copying, pasting the the edit which has a focus
	InsertGuiPair(menu_edit_del,			"&Elimina \tDel");
	InsertGuiPair(menu_edit_paste_formula,	"Incolla &formula\tCtrl+V");	// pasting directly to input edit
	InsertGuiPair(menu_edit_copy_result,	"Copia &risultato \tCtrl+R");
	InsertGuiPair(menu_edit_copy_both,		"Copia &entrambe \tCtrl+B");	// copying in this way: "input edit = output edit"
	InsertGuiPair(menu_edit_select_all,		"Seleziona &tutto \tEsc");
	InsertGuiPair(menu_edit_bracket,		"&Parentesi \tCtrl+0");			// inserting brackets: (...)
	InsertGuiPair(menu_edit_swap,			"&Scambia \tCtrl+W");
	InsertGuiPair(menu_help_help,			"&Aiuto");
	InsertGuiPair(menu_help_project_page,	"&Pagina del progetto");
	InsertGuiPair(menu_help_check_update,	"&Ottieni aggiornamenti");
	InsertGuiPair(menu_help_about,			"&Informazioni su...");
	InsertGuiPair(menu_update_available,	"&Una nuova versione e disponibile"); // not implemented yet
	// this will be a new main menu item (one after 'help') visible only if there is an update

	InsertGuiPair(cant_init_calculations,	"Non posso inizializzare il modulo per i calcoli");
	InsertGuiPair(message_box_error_caption,"TTCalc");
	InsertGuiPair(cant_create_thread,		"Non posso creare il secondo thread per i calcoli");
	InsertGuiPair(cant_create_pad,			"Non posso creare la finestra del bloc-notes");
	InsertGuiPair(cant_create_main_window,	"Non posso creare la finestra principale dell'applicazione");
	InsertGuiPair(cant_init_common_controls,"Non posso inizializzare i controlli usuali (InitCommonControlsEx)");
	InsertGuiPair(about_text,
		"Calcolatore matematico TTCalc %d.%d.%d%s%s\r\n\r\n"
		"Autore: Tomasz Sowa\r\n"
		"Contatto: t.sowa@ttmath.org\r\n"
		"Licenza: BSD (open source)\r\n"
		"Pagina del progetto: http://ttcalc.sourceforge.net\r\n"
		"Bignum library: TTMath %d.%d.%d%s (%s)\r\n"
		"Bignum type: binary floating point numbers\r\n"
		"Linguaggio di programmazione: C++\r\n"
		"Compilatore: %s\r\n"
		"%s"	// for upx
		"\r\n"
		"TTCalc usa la TTMath bignum library"
		" che pu� essere trovata su http://www.ttmath.org\r\n"
		"\r\n"
#ifdef TTCALC_PORTABLE
		"Questa e la verisone portatile del programma TTCalc. In questa versione "
		"potrete fare calcoli con un solo tipo di precisione (96 bit per la "
		"mantissa e 32 bit per l'esponente) di circa +/-6.9e+646457021.\r\n"
		"\r\n"
#endif
		"Traduzione:\r\n"
		"Spagnolo	Alejandro S. Valdezate\r\n"
		"	alesanval@gmail.com\r\n"
		"Danese	Rune Bisgaard Vammen\r\n"
		"	runebisgaard@gmail.com\r\n"
		"Cinese	Juis\r\n"
		"	zsyfly@gmail.com\r\n"
		"Russo	Vladimir Gladilovich\r\n"
		"	vdgladilovich@gmail.com\r\n"
		"Svedese	Lars 'Bafvert' Gafvert\r\n"
		"	lars.gafvert@gmail.com\r\n"
		"Italian	Damiano Monaco\r\n"
		"	dmonax@gmail.com\r\n"
		"Tedesco	Moritz Beleites\r\n"
		"	morbel@gmx.net\r\n"
		);
	InsertGuiPair(about_text_portable_version, " versione portabile");
	InsertGuiPair(about_text_exe_packer,	"EXE Packer: UPX 3.07\r\n");
	InsertGuiPair(about_box_title,			"Informazioni su");
	InsertGuiPair(about_box_button_close,	"Esci");
	InsertGuiPair(unknown_error,			"Si e verificato un errore sconosciuto");
	InsertGuiPair(cant_find_help,			"Non posso trovare nessun file di aiuto");
	InsertGuiPair(cant_open_project_page,   "Non posso aprire la webpage del progetto");
	InsertGuiPair(update_title,				"Controllo per una nuova versione");
	InsertGuiPair(update_button_next,		"Continua");
	InsertGuiPair(update_button_finish,		"Finito");
	InsertGuiPair(update_button_cancel,		"Annulla");
	InsertGuiPair(update_check_at_startup,	"Controlla sempre aggiornamenti all'avvio");  // this text has been changed, need correction!
	InsertGuiPair(update_check_for_info,	"Premi Continua per controllare gli aggiornamenti...");
	InsertGuiPair(update_is_new_version,	"Versione %d.%d.%d disponibile, premi Continua per scaricare...");
	InsertGuiPair(update_no_new_version1,	"Nessuna versione nuova disponibile.");
	InsertGuiPair(update_no_new_version2,	"Hai la versione piu aggiornata.");
	InsertGuiPair(update_download_from,		"Scaricamento da...");
#ifndef TTCALC_PORTABLE
	InsertGuiPair(update_downloaded_info1,	"E' stato scaricato un nuovo programma di installazione.");
	InsertGuiPair(update_downloaded_info2,	"Premi Finito per chiudere TTCalc far partire l'installazione.");
#else
	InsertGuiPair(update_downloaded_info1,	"E' stata scaricata una nuova versione di TTCalc.");
	InsertGuiPair(update_downloaded_info2,	"");
#endif
	InsertGuiPair(update_download_error,	"Si e verificato un problema durante il download, prova piu tardi.");

	InsertGuiPair(pad_title,				"Pad");
	InsertGuiPair(pad_menu_file,			"&File");
	InsertGuiPair(pad_menu_edit,			"&Modifica");
	InsertGuiPair(pad_menu_file_new,		"&Nuovo"); // temporarily not used
	InsertGuiPair(pad_menu_file_open,		"&Apri...");
	InsertGuiPair(pad_menu_file_saveas,		"Salva &come...");
	InsertGuiPair(pad_menu_file_close,		"&Chiudi");
	InsertGuiPair(pad_menu_edit_undo,		"&Annulla \tCtrl+Z");
	InsertGuiPair(pad_menu_edit_cut,		"&Taglia \tCtrl+X");
	InsertGuiPair(pad_menu_edit_copy,		"&Copia \tCtrl+C");
	InsertGuiPair(pad_menu_edit_paste,		"&Incolla \tCtrl+V");
	InsertGuiPair(pad_menu_edit_del,		"&Elimina \tDel");
	InsertGuiPair(pad_menu_edit_select_all,	"Seleziona &tutto \tCtrl+A");

	InsertGuiPair(cannot_open_file,			"Non posso aprire il file");
	InsertGuiPair(cannot_save_file,			"Non posso salvare in tale file");
	InsertGuiPair(file_too_long,			"Il file e troppo lungo");
	InsertGuiPair(other_error,				"Si e verificato un problema con questa operazione"); // for other kinds of errors



/*
	german gui messages
*/
	gui_messages_tab.push_back( std::map<GuiMsg, std::string>() );

	InsertGuiPair(button_ok,"Ok");
	InsertGuiPair(button_cancel,"Abbrechen");
	InsertGuiPair(message_box_caption,"TTCalc");

	InsertGuiPair(dialog_box_add_variable_caption,"Variable hinzuf�gen");
	InsertGuiPair(dialog_box_edit_variable_caption,"Variable bearbeiten");
	InsertGuiPair(dialog_box_add_variable_incorrect_name,"Ung�ltiger Variablenname");
	InsertGuiPair(dialog_box_add_variable_incorrect_value,"Ung�ltiger Wert der Variablen");
	InsertGuiPair(dialog_box_add_variable_variable_exists,"Diese Variable existiert bereits");
	InsertGuiPair(dialog_box_edit_variable_unknown_variable,"Variable unbekannt - vermutlich interner Fehler!");
	InsertGuiPair(dialog_box_delete_variable_confirm,"Diese Variablen l�schen?");
	InsertGuiPair(dialog_box_variable_not_all_deleted,"Variablen konnten nicht alle gel�scht werden - vermutlich interner Fehler!");
	InsertGuiPair(dialog_box_add_function_caption,"Funktion hinzuf�gen");
	InsertGuiPair(dialog_box_edit_function_caption,"Funktion bearbeiten");
	InsertGuiPair(dialog_box_add_function_function_exists,"Diese Funktion existiert bereits");
	InsertGuiPair(dialog_box_edit_function_unknown_function,"Funktion unbekannt - vermutlich interner Fehler!");
	InsertGuiPair(dialog_box_delete_function_confirm,"Diese Funktionen l�schen?");
	InsertGuiPair(dialog_box_function_not_all_deleted,"Funktionen konnten nicht alle gel�scht werden - vermutlich interner Fehler!");
	InsertGuiPair(dialog_box_add_function_incorrect_name,"Ung�ltiger Funktionsname");

	InsertGuiPair(list_variables_header_1,"Name");
	InsertGuiPair(list_variables_header_2,"Wert");
	InsertGuiPair(list_functions_header_1,"Name");
	InsertGuiPair(list_functions_header_2,"Parameter");
	InsertGuiPair(list_functions_header_3,"Wert");

	InsertGuiPair(button_add,"Hinzuf�gen");
	InsertGuiPair(button_edit,"Bearbeiten");
	InsertGuiPair(button_delete,"L�schen");
	InsertGuiPair(button_clear,"C");
	InsertGuiPair(tab_standard,"Standard");
	InsertGuiPair(tab_variables,"Variablen");
	InsertGuiPair(tab_functions,"Funktionen");
	InsertGuiPair(tab_precision,"Genauigkeit");
	InsertGuiPair(tab_display,"Anzeige");
	InsertGuiPair(tab_convert,"Konvertieren");

	InsertGuiPair(radio_precision_1,"Klein - 96 bit f�r Mantisse, 32 bit f�r Exponenten");
	InsertGuiPair(radio_precision_2,"Mittel - 512 bit f�r Mantisse, 64 bit f�r Exponenten");
	InsertGuiPair(radio_precision_3,"Gro� - 1024 bit f�r Mantisse, 128 bit f�r Exponenten");

	InsertGuiPair(precision_1_info,"(+/-)6.97...e+646457021, 26 g�ltige Dezimalstellen");
	InsertGuiPair(precision_2_info,"(+/-)9.25...e+2776511644261678719, 152 g�ltige Dezimalstellen");
	InsertGuiPair(precision_3_info,"(+/-)9.80...*10^(10^37), 306 g�ltige Dezimalstellen");

	InsertGuiPair(overflow_during_printing,"�berlauf w�hrend der Ausgabe");

	InsertGuiPair(combo_rounding_none,"keine");
	InsertGuiPair(combo_rounding_integer,"auf Ganzzahl");
	InsertGuiPair(combo_rounding_to_number,"auf");
	InsertGuiPair(combo_rounding_after_comma,"Stelle(n)");
	InsertGuiPair(check_remove_zeroes,"Nur signifikante Nachkommastellen");

	InsertGuiPair(display_input,			"Eingabebasis");
	InsertGuiPair(display_output,			"Ergebnisbasis");
	InsertGuiPair(display_rounding,			"Rundung");
	InsertGuiPair(display_always_scientific,"Immer");
	InsertGuiPair(display_not_always_scientific,"Wenn der Exponent gr��er ist als:");
	InsertGuiPair(display_as_scientific,	"Ergebnis in wissenschaftlicher Schreibweise");
	InsertGuiPair(display_deg_rad_grad,		"Winkelma�");
	InsertGuiPair(display_grouping,			"Gruppierung");					// e.g. 1`000`000
	InsertGuiPair(display_grouping_none,	"keine");
	InsertGuiPair(display_grouping_space,	"Leerzeichen");
	InsertGuiPair(display_grouping_digits,  "Stelle(n)");
	InsertGuiPair(display_input_decimal_point,	"Dezimaltrennz. Eingabe"); 	// Decimal point in formula
	InsertGuiPair(display_input_decimal_point_item1,	" . oder ,");
	InsertGuiPair(display_output_decimal_point,	"Dezimaltrennz. Ergebnis");	// Decimal point in result
	InsertGuiPair(display_param_sep,		"Parameter getrennt durch");
	InsertGuiPair(display_param_sep_item2,	" ; oder .");
	InsertGuiPair(display_param_sep_item3,	" ; oder ,");

	InsertGuiPair(convert_type,				"Typ");
	InsertGuiPair(convert_input,			"Eingabe");
	InsertGuiPair(convert_output,			"Ergebnis");
	InsertGuiPair(convert_dynamic_output,	"Auto pr�fix");

	InsertGuiPair(menu_view,				"&Ansicht");
	InsertGuiPair(menu_edit,				"&Bearbeiten");
	InsertGuiPair(menu_help,				"&Hilfe");
	InsertGuiPair(menu_language,			"&Sprache");
	InsertGuiPair(menu_view_new_window,		"&Neues Fenster");
	InsertGuiPair(menu_view_normal_view,	"No&rmale Ansicht");
	InsertGuiPair(menu_view_compact_view,	"&Kompakte Ansicht");
	InsertGuiPair(menu_view_pad,			"&Rechenblock"); 
	InsertGuiPair(menu_view_always_on_top,	"Immer im &Vordergrund");
	InsertGuiPair(menu_view_lang_english,	"&Englisch");
	InsertGuiPair(menu_view_lang_polish,	"&Polnisch");
	InsertGuiPair(menu_view_lang_spanish,	"&Spanisch");
	InsertGuiPair(menu_view_lang_danish,	"&D�nisch");
	InsertGuiPair(menu_view_lang_chinese,	"&Chinesisch");
	InsertGuiPair(menu_view_lang_russian,	"&Russisch");
	InsertGuiPair(menu_view_lang_swedish,   "Sch&wedisch");
	InsertGuiPair(menu_view_lang_italian,   "&Italienisch");
	InsertGuiPair(menu_view_lang_german,    "De&utsch");
	InsertGuiPair(menu_view_close_program,	"&Beenden");
	InsertGuiPair(menu_edit_undo,			"&R�ckg�ngig \tStrg+Z");
	InsertGuiPair(menu_edit_cut,			"&Ausschneiden \tStrg+X");
	InsertGuiPair(menu_edit_copy,			"&Kopieren \tStrg+C");
	InsertGuiPair(menu_edit_paste,			"&Einf�gen");					// cutting, copying, pasting the the edit which has a focus
	InsertGuiPair(menu_edit_del,			"&L�schen \tEntf");
	InsertGuiPair(menu_edit_paste_formula,	"&Formel einf�gen \tStrg+V");	// pasting directly to input edit
	InsertGuiPair(menu_edit_copy_result,	"E&rgebnis kopieren \tStrg+R");
	InsertGuiPair(menu_edit_copy_both,		"&Beides kopieren \tStrg+B");	// copying in this way: "input edit = output edit"
	InsertGuiPair(menu_edit_select_all,		"Alles a&usw�hlen \tEsc");
	InsertGuiPair(menu_edit_bracket,		"Eingabe eink&lammern \tStrg+0"); // inserting brackets: (...)
	InsertGuiPair(menu_edit_swap,			"&Vertauschen \tStrg+W");		// swapping input edit for output edit
	InsertGuiPair(menu_help_help,			"&Hilfe");
	InsertGuiPair(menu_help_project_page,	"&Projektseite aufrufen");
	InsertGuiPair(menu_help_check_update,	"Nach &Updates suchen...");
	InsertGuiPair(menu_help_about,			"&�ber TTCalc");
	InsertGuiPair(menu_update_available,	"&Neue Version ist verf�gbar");
	// this will be a new main menu item (one after 'help') visible only if there is an update

	InsertGuiPair(cant_init_calculations,	"Konnte Modul f�r die Berechnungen nicht initialisieren");
	InsertGuiPair(message_box_error_caption,"TTCalc");
	InsertGuiPair(cant_create_thread,		"Konnte zweiten Thread f�r die Berechung nicht erzeugen");
	InsertGuiPair(cant_create_pad,			"Konnte Fenster f�r Rechenblock nicht erzeugen");
	InsertGuiPair(cant_create_main_window,	"Konnte das Hauptfenster nicht erzeugen");
	InsertGuiPair(cant_init_common_controls,"Konnte die \"common controls\" nicht initialisieren (InitCommonControlsEx)");
	InsertGuiPair(about_text,
		"Mathematischer Taschenrechner TTCalc %d.%d.%d%s%s\r\n\r\n"
		"Autor: Tomasz Sowa\r\n"
		"Kontakt: t.sowa@ttmath.org\r\n"
		"Lizenz: BSD (open source)\r\n"
		"Projektseite: http://ttcalc.sourceforge.net\r\n"
		"Bignum library: TTMath %d.%d.%d%s (%s)\r\n"
		"Bignum Typ: binary floating point numbers\r\n"
		"Programmiersprache: C++\r\n"
		"Compiler: %s\r\n"
		"%s"	// for upx
		"\r\n"
		"TTCalc verwendet das TTMath bignum library,"
		" welches auf http://www.ttmath.org verf�gbar ist.\r\n"
		"\r\n"
#ifdef TTCALC_PORTABLE
		"Dies ist die portable Version des Programms TTCalc. In dieser Version "
		"kann nur mit einer einzigen Pr�zision gerechnet werden (96 bit f�r die "
		"Mantisse und 32 bit f�r den Exponenten) ungef�hr +/-6.9e+646457021.\r\n"
		"\r\n"
#endif
		"�bersetzungen:\r\n"
		"Spanisch	Alejandro S. Valdezate\r\n"
		"	alesanval@gmail.com\r\n"
		"D�nisch	Rune Bisgaard Vammen\r\n"
		"	runebisgaard@gmail.com\r\n"
		"Chinesisch Juis\r\n"
		"	zsyfly@gmail.com\r\n"
		"Russisch	Vladimir Gladilovich\r\n"
		"	vdgladilovich@gmail.com\r\n"
		"Schwedisch Lars 'Bafvert' Gafvert\r\n"
		"	lars.gafvert@gmail.com\r\n"
		"Italienisch	Damiano Monaco\r\n"
		"	dmonax@gmail.com\r\n"
		"Deutsch	Moritz Beleites\r\n"
		"	morbel@gmx.net\r\n"
		);
	InsertGuiPair(about_text_portable_version, " portable Version");
	InsertGuiPair(about_text_exe_packer,	"EXE Packer: UPX 3.07\r\n");
	InsertGuiPair(about_box_title,			"�ber TTCalc");
	InsertGuiPair(about_box_button_close,	"Schlie�en");
	InsertGuiPair(unknown_error,			"Ein unbekannter Fehler ist aufgetreten");
	InsertGuiPair(cant_find_help,			"Hilfe-Dateien k�nnen nicht gefunden werden");
	InsertGuiPair(cant_open_project_page,   "Projektseite kann nicht ge�ffnet werden");
	InsertGuiPair(update_title,				"Nach neuer Version suchen");
	InsertGuiPair(update_button_next,		"Weiter");
	InsertGuiPair(update_button_finish,		"Ende");
	InsertGuiPair(update_button_cancel,		"Abbrechen");
	InsertGuiPair(update_check_at_startup,	"Beim Start des Programms immer nach Update suchen");  // this text has been changed, need correction!
	InsertGuiPair(update_check_for_info,	"\"Weiter\", um die Update-Suche zu starten...");
	InsertGuiPair(update_is_new_version,	"Version %d.%d.%d ist verf�gbar \"Weiter\", um den Download zu starten...");
	InsertGuiPair(update_no_new_version1,	"Es ist keine neuere Version verf�gbar.");
	InsertGuiPair(update_no_new_version2,	"Die neueste Version ist bereits installiert.");
	InsertGuiPair(update_download_from,		"Download l�uft...");
#ifndef TTCALC_PORTABLE
	InsertGuiPair(update_downloaded_info1,	"Eine neue Setup-Datei wurde heruntergeladen.");
	InsertGuiPair(update_downloaded_info2,	"\"Ende\", um TTCalc zu beenden und die neue version zu installieren.");
#else
	InsertGuiPair(update_downloaded_info1,	"Eine neue Version wurde heruntergeladen.");
	InsertGuiPair(update_downloaded_info2,	"");
#endif
	InsertGuiPair(update_download_error,	"Problem beim Download - bitte sp�ter erneut versuchen.");

	InsertGuiPair(pad_title,				"Notizblock");
	InsertGuiPair(pad_menu_file,			"&Datei");
	InsertGuiPair(pad_menu_edit,			"&Bearbeiten");
	InsertGuiPair(pad_menu_file_new,		"&Neu"); // temporarily not used
	InsertGuiPair(pad_menu_file_open,		"�&ffnen...");
	InsertGuiPair(pad_menu_file_saveas,		"Speichern &als...");
	InsertGuiPair(pad_menu_file_close,		"&Beenden");
	InsertGuiPair(pad_menu_edit_undo,		"&R�ckg�ngig \tStrg+Z");
	InsertGuiPair(pad_menu_edit_cut,		"&Ausschneiden \tStrg+X");
	InsertGuiPair(pad_menu_edit_copy,		"&Kopieren \tStrg+C");
	InsertGuiPair(pad_menu_edit_paste,		"&Einf�gen \tStrg+V");
	InsertGuiPair(pad_menu_edit_del,		"&L�schen \tEntf");
	InsertGuiPair(pad_menu_edit_select_all,	"Alles a&usw�hlen \tStrg+A");

	InsertGuiPair(cannot_open_file,			"Datei kann nicht ge�ffnet werden");
	InsertGuiPair(cannot_save_file,			"Datei kann nicht gespeichert werden");
	InsertGuiPair(file_too_long,			"Datei ist zu lang");
	InsertGuiPair(other_error,				"Unbekannter Fehler aufgetreten"); // for other kinds of errors

}

