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
	InsertErrorPair(ttmath::err_unexpected_final_bracket,"Nieoczekiwany nawias zamykaj¹cy");
	InsertErrorPair(ttmath::err_stack_not_clear,"Pozosta³ nieznany znak");
	InsertErrorPair(ttmath::err_unknown_variable,"Nieznana zmienna");
	InsertErrorPair(ttmath::err_division_by_zero,"Dzielenie przez zero");
	InsertErrorPair(ttmath::err_interrupt,"Obliczenia zosta³y przerwane");
	InsertErrorPair(ttmath::err_overflow,"Przekroczony zakres");
	InsertErrorPair(ttmath::err_unknown_function,"Nieznana funkcja");
	InsertErrorPair(ttmath::err_unknown_operator,"Nieznany operator");
	InsertErrorPair(ttmath::err_unexpected_semicolon_operator,"Nieoczekiwany operator 'œrednik'");
	InsertErrorPair(ttmath::err_improper_amount_of_arguments,"Niew³aœciwa liczba argumentów");
	InsertErrorPair(ttmath::err_improper_argument,"Niew³aœciwy argument");
	InsertErrorPair(ttmath::err_unexpected_end,"Nieoczekiwany koniec");
	InsertErrorPair(ttmath::err_internal_error,"B³¹d wewnêtrzny programu!");

	InsertErrorPair(ttmath::err_incorrect_name,"Nieprawid³owa nazwa zmiennej lub funkcji");
	InsertErrorPair(ttmath::err_incorrect_value,"Nieprawid³owa wartoœæ zmiennej lub funkcji");
	InsertErrorPair(ttmath::err_variable_exists,"Ta zmienna juz istnieje");
	InsertErrorPair(ttmath::err_variable_loop,"Pomiêdzy zmiennymi zachodzi wywo³anie rekurencyjne");
	InsertErrorPair(ttmath::err_functions_loop,"Pomiêdzy funkcjami zachodzi wywo³anie rekurencyjne");
	InsertErrorPair(ttmath::err_must_be_only_one_value,"Zmienne albo funkcje mog¹ posiadaæ (zwracaæ) tylko jedn¹ wartoœæ");
	InsertErrorPair(ttmath::err_still_calculating,"Obliczanie...");
	InsertErrorPair(ttmath::err_percent_from,"Nieprawid³owo u¿yty operator procentu");


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
	InsertErrorPair(ttmath::err_stack_not_clear,"Et ukendt tegn er væk");
	InsertErrorPair(ttmath::err_unknown_variable,"En ukendt variabel");
	InsertErrorPair(ttmath::err_division_by_zero,"Division med nul");
	InsertErrorPair(ttmath::err_interrupt,"Beregningen er blevet afbrydt");
	InsertErrorPair(ttmath::err_overflow,"Overflow fejl");
	InsertErrorPair(ttmath::err_unknown_function,"En ukendt funktion");
	InsertErrorPair(ttmath::err_unknown_operator,"En ukendt operator");
	InsertErrorPair(ttmath::err_unexpected_semicolon_operator,"En ukendt semikolon operator");
	InsertErrorPair(ttmath::err_improper_amount_of_arguments,"Ukorrekt mængde af argumenter");
	InsertErrorPair(ttmath::err_improper_argument,"Uægte argumentation");
	InsertErrorPair(ttmath::err_unexpected_end,"Uforventet slutning");
	InsertErrorPair(ttmath::err_internal_error,"En intern fejl");

	InsertErrorPair(ttmath::err_incorrect_name,"Ukorrekt navn for en variabel eller function");
	InsertErrorPair(ttmath::err_incorrect_value,"Ukorrekt værdi for en variabel eller funktion");
	InsertErrorPair(ttmath::err_variable_exists,"Denne variabel eksisterer allerede");
	InsertErrorPair(ttmath::err_variable_loop,"Det er en gentagelse mellem variabler");
	InsertErrorPair(ttmath::err_functions_loop,"Der er en gentagelse mellem funktioner");
	InsertErrorPair(ttmath::err_must_be_only_one_value,"Variabler eller funktioner skal kun returnere én værdi");
	InsertErrorPair(ttmath::err_still_calculating,"Regner...");
	InsertErrorPair(ttmath::err_percent_from,"Procent operatoren blev ikke brugt korrekt");


/*
	chinese messages relating to ttmath library
*/
	error_messages_tab.push_back( std::map<ttmath::ErrorCode, std::string>() );

	InsertErrorPair(ttmath::err_ok,"È·¶¨");
	InsertErrorPair(ttmath::err_nothing_has_read,"");
	InsertErrorPair(ttmath::err_unknown_character,"Î´Öª×Ö·û");
	InsertErrorPair(ttmath::err_unexpected_final_bracket,"ÒâÍâ¶àÓàµÄÀ¨»¡");
	InsertErrorPair(ttmath::err_stack_not_clear,"¶ÑÕ»Ã»ÓĞÇå¿Õ");
	InsertErrorPair(ttmath::err_unknown_variable,"Î´Öª±äÁ¿");
	InsertErrorPair(ttmath::err_division_by_zero,"³ıÊı²»ÄÜÎªÁã");
	InsertErrorPair(ttmath::err_interrupt,"¼ÆËã±»ÖĞ¶Ï");
	InsertErrorPair(ttmath::err_overflow,"Êı¾İÒç³ö");
	InsertErrorPair(ttmath::err_unknown_function,"Î´Öªº¯Êı");
	InsertErrorPair(ttmath::err_unknown_operator,"Î´ÖªÔËËã·û");
	InsertErrorPair(ttmath::err_unexpected_semicolon_operator,"ÒâÍâµÄ·ÖºÅÔËËã·û");
	InsertErrorPair(ttmath::err_improper_amount_of_arguments,"´íÎó²ÎÊıÊıÁ¿");
	InsertErrorPair(ttmath::err_improper_argument,"´íÎó²ÎÊı");
	InsertErrorPair(ttmath::err_unexpected_end,"ÒâÍâÄ©¶Ë");
	InsertErrorPair(ttmath::err_internal_error,"ÄÚ²¿´íÎó");

	InsertErrorPair(ttmath::err_incorrect_name,"´íÎóµÄ±äÁ¿Ãû»òº¯ÊıÃû");
	InsertErrorPair(ttmath::err_incorrect_value,"´íÎóµÄ±äÁ¿Öµ»òº¯ÊıÖµ");
	InsertErrorPair(ttmath::err_variable_exists,"Õâ¸ö±äÁ¿ÒÑ¾­´æÔÚ");
	InsertErrorPair(ttmath::err_variable_loop,"±äÁ¿Ñ­»·");
	InsertErrorPair(ttmath::err_functions_loop,"º¯ÊıÑ­»·");
	InsertErrorPair(ttmath::err_must_be_only_one_value,"±äÁ¿»òº¯Êı±ØĞëÓĞ·µ»ØÖµ");
	InsertErrorPair(ttmath::err_still_calculating,"ÕıÔÚ¼ÆËã...");
	InsertErrorPair(ttmath::err_percent_from,"Incorrectly used percentage operator");


/*
	russian messages relating to ttmath library
*/
	error_messages_tab.push_back( std::map<ttmath::ErrorCode, std::string>() );

	InsertErrorPair(ttmath::err_ok,"ok");
	InsertErrorPair(ttmath::err_nothing_has_read,"");
	InsertErrorPair(ttmath::err_unknown_character,"Íåèçâåñòíûé ñèìâîë");
	InsertErrorPair(ttmath::err_unexpected_final_bracket,"Íåîæèäàííàÿ çàêğûâàşùàÿ ñêîáêà");
	InsertErrorPair(ttmath::err_stack_not_clear,"Îñòàëñÿ íåèçâåñòíûé ñèìâîë");
	InsertErrorPair(ttmath::err_unknown_variable,"Íåèçâåñòíàÿ ïåğåìåííàÿ");
	InsertErrorPair(ttmath::err_division_by_zero,"Äåëèòü íà íîëü íåëüçÿ");
	InsertErrorPair(ttmath::err_interrupt,"Ïîäñ÷åò ïğåğâàí");
	InsertErrorPair(ttmath::err_overflow,"Ïåğåïîëíåíèå");
	InsertErrorPair(ttmath::err_unknown_function,"Íåèçâåñòíàÿ ôóíêöèÿ");
	InsertErrorPair(ttmath::err_unknown_operator,"Íåèçâåñòíûé îïåğàòîğ");
	InsertErrorPair(ttmath::err_unexpected_semicolon_operator,"Íåîæèäàííûé îïåğàòîğ ñ ;");
	InsertErrorPair(ttmath::err_improper_amount_of_arguments,"Íåäîïóñòèìîå êîëè÷åñòâî àğãóìåíòîâ");
	InsertErrorPair(ttmath::err_improper_argument,"Íåäîïóñòèìûé àğãóìåíò");
	InsertErrorPair(ttmath::err_unexpected_end,"Íåîæèäàííûé êîíåö");
	InsertErrorPair(ttmath::err_internal_error,"Âíóòğåííÿÿ îøèáêà");

	InsertErrorPair(ttmath::err_incorrect_name,"Íåïğàâèëüíîå èìÿ ïåğåìåííîé èëè ôóíêöèè");
	InsertErrorPair(ttmath::err_incorrect_value,"Íåïğàâèëüíîå çíà÷åíèå ïåğåìåííîé èëè ôóíêöèè");
	InsertErrorPair(ttmath::err_variable_exists,"Òàêàÿ ïåğåìåííàÿ óæå çàäàíà");
	InsertErrorPair(ttmath::err_variable_loop,"Ïåğåìåííûå ğåêóğğåíòíû");
	InsertErrorPair(ttmath::err_functions_loop,"Ôóíêöèè ğåêóğğåíòíû");
	InsertErrorPair(ttmath::err_must_be_only_one_value,"Ïåğåìåííûå èëè ôóíêöèè äîëæíû âîçâğàùàòü òîëüêî îäíî çíà÷åíèå");
	InsertErrorPair(ttmath::err_still_calculating,"Ïîäñ÷èòûâàåì...");
	InsertErrorPair(ttmath::err_percent_from,"Íåïğàâèëüíîå èñïîëüçîâàíèå îïåğàòîğà ïğîöåíòà");

/*
	swedish messages relating to ttmath library
*/
	error_messages_tab.push_back( std::map<ttmath::ErrorCode, std::string>() );

	InsertErrorPair(ttmath::err_ok,"ok");
	InsertErrorPair(ttmath::err_nothing_has_read,"");
	InsertErrorPair(ttmath::err_unknown_character,"Ett okänt tecken");
	InsertErrorPair(ttmath::err_unexpected_final_bracket,"En oförväntat slutlig klass");
	InsertErrorPair(ttmath::err_stack_not_clear,"Ett okänt tecken saknas");
	InsertErrorPair(ttmath::err_unknown_variable,"En okänd variabel");
	InsertErrorPair(ttmath::err_division_by_zero,"Division med noll");
	InsertErrorPair(ttmath::err_interrupt,"Beräkningen har avbrutits");
	InsertErrorPair(ttmath::err_overflow,"Overflow fel");
	InsertErrorPair(ttmath::err_unknown_function,"En okänd funktion");
	InsertErrorPair(ttmath::err_unknown_operator,"En okänd operator");
	InsertErrorPair(ttmath::err_unexpected_semicolon_operator,"En okänd semikolonoperator");
	InsertErrorPair(ttmath::err_improper_amount_of_arguments,"Felaktig mängd argument");
	InsertErrorPair(ttmath::err_improper_argument,"Ogiltig argumentation");
	InsertErrorPair(ttmath::err_unexpected_end,"Oförväntat fel");
	InsertErrorPair(ttmath::err_internal_error,"Ett internt fel");

	InsertErrorPair(ttmath::err_incorrect_name,"Ogiltigt namn för en variabel eller funktion");
	InsertErrorPair(ttmath::err_incorrect_value,"Ogiltigt värde för en variabel eller funktion");
	InsertErrorPair(ttmath::err_variable_exists,"Denna variabel existerar redan");
	InsertErrorPair(ttmath::err_variable_loop,"Det är en upprepepning mellan variabler");
	InsertErrorPair(ttmath::err_functions_loop,"Det är en upprepning mellan funktioner");
	InsertErrorPair(ttmath::err_must_be_only_one_value,"Variabler eller funktioner skall kunna avge ett värde");
	InsertErrorPair(ttmath::err_still_calculating,"Räknar bävrar...");
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
	InsertErrorPair(ttmath::err_unexpected_final_bracket,"Unerwartete schließende Klammer");
	InsertErrorPair(ttmath::err_stack_not_clear,"Unbekanntes Zeichen ist übrig");
	InsertErrorPair(ttmath::err_unknown_variable,"Unbekannte Variable");
	InsertErrorPair(ttmath::err_division_by_zero,"Division durch Null");
	InsertErrorPair(ttmath::err_interrupt,"Berechnung unterbrochen");
	InsertErrorPair(ttmath::err_overflow,"Überlauf");
	InsertErrorPair(ttmath::err_unknown_function,"Unbekannte Funktion");
	InsertErrorPair(ttmath::err_unknown_operator,"Unbekannter Operator");
	InsertErrorPair(ttmath::err_unexpected_semicolon_operator,"Unerwarteter Semikolon-Operator");
	InsertErrorPair(ttmath::err_improper_amount_of_arguments,"Falsche Parameteranzahl");
	InsertErrorPair(ttmath::err_improper_argument,"Unpassender Parameter");
	InsertErrorPair(ttmath::err_unexpected_end,"Unerwartetes Ende");
	InsertErrorPair(ttmath::err_internal_error,"Interner Fehler");

	InsertErrorPair(ttmath::err_incorrect_name,"Ungültiger Name einer Variablen oder Funktion");
	InsertErrorPair(ttmath::err_incorrect_value,"Ungültiger Wert einer Variablen oder Funktion");
	InsertErrorPair(ttmath::err_variable_exists,"Diese Variable existiert bereits");
	InsertErrorPair(ttmath::err_variable_loop,"Rekursion zwischen Variablen gefunden");
	InsertErrorPair(ttmath::err_functions_loop,"Rekursion zwischen Funktionen gefunden");
	InsertErrorPair(ttmath::err_must_be_only_one_value,"Variablen und Funktionen dürfen nur einen Rückgabewert haben");
	InsertErrorPair(ttmath::err_still_calculating,"Berechnung läuft...");
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

	InsertGuiPair(dialog_box_add_variable_caption,"Dodaj now¹ zmienn¹");
	InsertGuiPair(dialog_box_edit_variable_caption,"Zmieñ wartoœæ zmiennej");
	InsertGuiPair(dialog_box_add_variable_incorrect_name,"Nie prawid³owa nazwa zmiennej");
	InsertGuiPair(dialog_box_add_variable_incorrect_value,"Nie prawid³owa wartoœæ zmiennej");
	InsertGuiPair(dialog_box_add_variable_variable_exists,"Podana zmienna ju¿ istnieje");
	InsertGuiPair(dialog_box_edit_variable_unknown_variable,"Podanej zmiennej nie ma w tablicy. Prawdopodobnie b³¹d wewnêtrzny programu.");
	InsertGuiPair(dialog_box_delete_variable_confirm,"Czy napewno usun¹æ zaznaczone zmienne?");
	InsertGuiPair(dialog_box_variable_not_all_deleted,"Zosta³o kilka zmiennych których nie mo¿na by³o skasowaæ. Prawdopodobnie b³¹d wewnêtrzny programu.");
	InsertGuiPair(dialog_box_add_function_caption,"Dodaj now¹ funkcjê");
	InsertGuiPair(dialog_box_edit_function_caption,"Zmieñ wartoœæ funkcji");
	InsertGuiPair(dialog_box_add_function_function_exists,"Podana funkcja ju¿ istnieje");
	InsertGuiPair(dialog_box_edit_function_unknown_function,"Podanej funkcji nie ma w tablicy. Prawdopodobnie b³¹d wewnêtrzny programu.");
	InsertGuiPair(dialog_box_delete_function_confirm,"Czy napewno usun¹æ zaznaczone funkcje?");
	InsertGuiPair(dialog_box_function_not_all_deleted,"Zosta³o kilka funkcji których nie mo¿na by³o skasowaæ. Prawdopodobnie b³¹d wewnêtrzny programu.");
	InsertGuiPair(dialog_box_add_function_incorrect_name,"Nieprawid³owa nazwa funkcji");


	InsertGuiPair(list_variables_header_1,"Nazwa");
	InsertGuiPair(list_variables_header_2,"Wartoœæ");
	InsertGuiPair(list_functions_header_1,"Nazwa");
	InsertGuiPair(list_functions_header_2,"Parametry");
	InsertGuiPair(list_functions_header_3,"Wartoœæ");

	InsertGuiPair(button_add,"Dodaj");
	InsertGuiPair(button_edit,"Edytuj");
	InsertGuiPair(button_delete,"Usuñ");
	InsertGuiPair(button_clear,"C");
	InsertGuiPair(tab_standard,"Standard");
	InsertGuiPair(tab_variables,"Zmienne");
	InsertGuiPair(tab_functions,"Funkcje");
	InsertGuiPair(tab_precision,"Precyzja");
	InsertGuiPair(tab_display,"Wyœwietlanie");
	InsertGuiPair(tab_convert,"Konwersja");

	InsertGuiPair(radio_precision_1,"Ma³a - 96 bitowa mantysa, 32 bitowy wyk³adnik");
	InsertGuiPair(radio_precision_2,"Œrednia - 512 bitowa mantysa, 64 bitowy wyk³adnik");
	InsertGuiPair(radio_precision_3,"Du¿a - 1024 bitowa mantysa, 128 bitowy wyk³adnik");

	InsertGuiPair(precision_1_info,"(+/-)6.97...e+646457021, 26 cyfr znacz¹cych (w rozwiniêciu dziesiêtnym)");
	InsertGuiPair(precision_2_info,"(+/-)9.25...e+2776511644261678719, 152 cyfr znacz¹cych");
	InsertGuiPair(precision_3_info,"(+/-)9.80...*10^(10^37), 306 cyfr znacz¹cych");

	InsertGuiPair(overflow_during_printing,"Przepe³nienie podczas wypisywania");

	InsertGuiPair(combo_rounding_none,"bez zmian");
	InsertGuiPair(combo_rounding_integer,"do ca³kowitej");
	InsertGuiPair(combo_rounding_to_number,"do");
	InsertGuiPair(combo_rounding_after_comma,"cyfr(y)");
	InsertGuiPair(check_remove_zeroes,"Skasuj nieznacz¹ce zera");

	InsertGuiPair(display_input,"Wejœcie");
	InsertGuiPair(display_output,"Wyjœcie");
	InsertGuiPair(display_rounding,"Zaokr¹glenie");
	InsertGuiPair(display_always_scientific,"Zawsze");
	InsertGuiPair(display_not_always_scientific,"Jeœli eksponent jest wiêkszy ni¿:");
	InsertGuiPair(display_as_scientific,	"Wyœwietl wynik w postaci naukowej");
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
	InsertGuiPair(convert_input,			"Wejœcie");
	InsertGuiPair(convert_output,			"Wyjœcie");
	InsertGuiPair(convert_dynamic_output,	"Automatyczny prefiks");

	InsertGuiPair(menu_view,				"&Widok");
	InsertGuiPair(menu_edit,				"&Edycja");
	InsertGuiPair(menu_help,				"&Pomoc");
	InsertGuiPair(menu_language,			"&Jêzyk");
	InsertGuiPair(menu_view_new_window,		"&Nowe okno");
	InsertGuiPair(menu_view_normal_view,	"Widok no&rmalny");
	InsertGuiPair(menu_view_compact_view,	"Widok &kompaktowy");
	InsertGuiPair(menu_view_always_on_top,	"Zawsze na &wierzchu");
	InsertGuiPair(menu_view_pad,			"&Calcpad");
	InsertGuiPair(menu_view_lang_english,	"&Angielski");
	InsertGuiPair(menu_view_lang_polish,	"&Polski");
	InsertGuiPair(menu_view_lang_spanish,	"&Hiszpañski");
	InsertGuiPair(menu_view_lang_danish,	"&Duñski");
	InsertGuiPair(menu_view_lang_chinese,	"&Chiñski");
	InsertGuiPair(menu_view_lang_russian,	"&Rosyjski");
	InsertGuiPair(menu_view_lang_swedish,   "&Szwedzki");
	InsertGuiPair(menu_view_lang_italian,   "&W³oski");
	InsertGuiPair(menu_view_lang_german,    "&Niemiecki");
	InsertGuiPair(menu_view_close_program,	"&Zamknij");
	InsertGuiPair(menu_edit_undo,			"&Cofnij \tCtrl+Z");
	InsertGuiPair(menu_edit_cut,			"Wy&tnij \tCtrl+X");
	InsertGuiPair(menu_edit_copy,			"&Kopiuj \tCtrl+C");
	InsertGuiPair(menu_edit_paste,			"&Wklej");
	InsertGuiPair(menu_edit_del,			"&Usuñ \tDel");
	InsertGuiPair(menu_edit_paste_formula,	"Wklej &formu³ê \tCtrl+V");
	InsertGuiPair(menu_edit_copy_result,	"Kopiuj w&ynik \tCtrl+R");
	InsertGuiPair(menu_edit_copy_both,		"Kopiuj &obydwa \tCtrl+B");
	InsertGuiPair(menu_edit_select_all,		"Zaznacz w&szystko \tEsc");
	InsertGuiPair(menu_edit_bracket,		"&Nawiasy \tCtrl+0");
	InsertGuiPair(menu_edit_swap,			"&Zamiana \tCtrl+W");
	InsertGuiPair(menu_help_help,			"&Pomoc");
	InsertGuiPair(menu_help_project_page,	"&Strona projektu");
	InsertGuiPair(menu_help_check_update,	"SprawdŸ &aktualizacjê...");
	InsertGuiPair(menu_help_about,			"&O programie");
	InsertGuiPair(menu_update_available,	"&Jest nowa wersja programu");

	InsertGuiPair(cant_init_calculations,	"Nie uda³o siê zainicjalizowaæ modu³u obs³ugi obliczeñ");
	InsertGuiPair(message_box_error_caption,"TTCalc");
	InsertGuiPair(cant_create_thread,		"Nie uda³o siê utworzyæ drugiego w¹tku do obliczeñ");
	InsertGuiPair(cant_create_pad,			"I could not create the pad window");
	InsertGuiPair(cant_create_main_window,	"Nie uda³o siê utworzyæ g³ównego okna aplikacji");
	InsertGuiPair(cant_init_common_controls,"Nie uda³o siê zainicjalizowaæ obs³ugi Common Controls (InitCommonControlsEx)");
	InsertGuiPair(about_text,
		"Kalkulator matematyczny TTCalc %d.%d.%d%s%s\r\n\r\n"
		"Autor: Tomasz Sowa\r\n"
		"Kontakt: t.sowa@ttmath.org\r\n"
		"Licencja: BSD (open source)\r\n"
		"Strona projektu: http://ttcalc.sourceforge.net\r\n"
		"Biblioteka du¿ych liczb: TTMath %d.%d.%d%s (%s)\r\n"
		"Rodzaj du¿ych liczb: liczby binarne zmiennoprzecinkowe\r\n"
		"Jêzyk programowania: C++\r\n"
		"Kompilator: %s\r\n"
		"%s"	// for upx
		"\r\n"
		"Ten program u¿ywa biblioteki du¿ych liczb TTMath"
		" która jest dostêpna na http://www.ttmath.org\r\n"
		"\r\n"
#ifdef TTCALC_PORTABLE
		"To jest wersja portable programu TTCalc. W tej wersji mo¿esz dokonywaæ "
		"obliczeñ jedynie z jednym rodzajem precyzji (96 bitowa mantysa "
		"oraz 32 bitowy wyk³adnik) to jest oko³o +/-6.9e+646457021.\r\n"
		"\r\n"
#endif
		"T³umaczenie:\r\n"
		"Hiszpañski	Alejandro S. Valdezate\r\n"
		"		alesanval@gmail.com\r\n"
		"Duñski		Rune Bisgaard Vammen\r\n"
		"		runebisgaard@gmail.com\r\n"
		"Chiñski		Juis\r\n"
		"		zsyfly@gmail.com\r\n"
		"Rosyjski		Vladimir Gladilovich\r\n"
		"		vdgladilovich@gmail.com\r\n"
		"Szwedzki		Lars 'Bafvert' Gafvert\r\n"
		"		lars.gafvert@gmail.com\r\n"
		"W³oski		Damiano Monaco\r\n"
		"		dmonax@gmail.com\r\n"
		"Niemiecki		Moritz Beleites\r\n"
		"		morbel@gmx.net\r\n"
		);
	InsertGuiPair(about_text_portable_version, " wersja portable");
	InsertGuiPair(about_text_exe_packer,	"Paker exe: UPX 3.07\r\n");
	InsertGuiPair(about_box_title,			"O programie");
	InsertGuiPair(about_box_button_close,	"Zamknij");
	InsertGuiPair(unknown_error,			"Nieznany kod b³êdu");
	InsertGuiPair(cant_find_help,			"Nie mogê znale¿æ ¿adnych plików pomocy");
	InsertGuiPair(cant_open_project_page,   "Nie mogê otworzyæ strony projektu");
	InsertGuiPair(update_title,				"SprawdŸ dostêpnoœæ nowej wersji");
	InsertGuiPair(update_button_next,		"Dalej");
	InsertGuiPair(update_button_finish,		"Zakoñcz");
	InsertGuiPair(update_button_cancel,		"Anuluj");
	InsertGuiPair(update_check_at_startup,	"Raz na trzy dni sprawdŸ dostêpnoœæ aktualizacji automatycznie");
	InsertGuiPair(update_check_for_info,	"Wciœnij Dalej aby sprawdziæ dostêpnoœæ aktualizacji...");
	InsertGuiPair(update_is_new_version,	"Wersja %d.%d.%d jest dostêpna, wciœnij Dalej aby j¹ pobraæ...");
	InsertGuiPair(update_no_new_version1,	"Nie ma dostêpnej nowszej wersji programu.");
	InsertGuiPair(update_no_new_version2,	"Obecnie masz najnowsz¹ wersjê.");
	InsertGuiPair(update_download_from,		"Pobieranie z...");
#ifndef TTCALC_PORTABLE
	InsertGuiPair(update_downloaded_info1,	"Nowy instalator zosta³ œci¹gniêty.");
	InsertGuiPair(update_downloaded_info2,	"Wciœnij Zakoñcz aby zamkn¹æ program TTCalc i uruchomiæ pobrany instalator.");
#else
	InsertGuiPair(update_downloaded_info1,	"Nowa wersja programu TTCalc zosta³a œci¹gniêta");
	InsertGuiPair(update_downloaded_info2,	"");
#endif
	InsertGuiPair(update_download_error,	"Wyst¹pi³y problemy ze œci¹gniêciem, proszê spróbowaæ ponownie póŸniej.");

	InsertGuiPair(pad_title,				"Calcpad");
	InsertGuiPair(pad_menu_file,			"&Plik");
	InsertGuiPair(pad_menu_edit,			"&Edycja");
	InsertGuiPair(pad_menu_file_new,		"&Nowy"); // temporarily not used
	InsertGuiPair(pad_menu_file_open,		"&Otwórz...");
	InsertGuiPair(pad_menu_file_saveas,		"Zapisz &jako...");
	InsertGuiPair(pad_menu_file_close,		"&Zamknij");
	InsertGuiPair(pad_menu_edit_undo,		"&Cofnij \tCtrl+Z");
	InsertGuiPair(pad_menu_edit_cut,		"Wy&tnij \tCtrl+X");
	InsertGuiPair(pad_menu_edit_copy,		"&Kopiuj \tCtrl+C");
	InsertGuiPair(pad_menu_edit_paste,		"&Wklej \tCtrl+V");
	InsertGuiPair(pad_menu_edit_del,		"&Usuñ \tDel");
	InsertGuiPair(pad_menu_edit_select_all,	"Zaznacz wszystko \tCtrl+A");

	InsertGuiPair(cannot_open_file,			"Nie mogê otworzyæ podanego pliku");
	InsertGuiPair(cannot_save_file,			"Nie mogê zapisaæ podanego pliku");
	InsertGuiPair(file_too_long,			"Podany plik jest zbyt du¿y");
	InsertGuiPair(other_error,				"Wyst¹pi³ problem z t¹ operacj¹"); // for other kinds of errors


/*
	spanish gui messages
*/


	gui_messages_tab.push_back( std::map<GuiMsg, std::string>() );

	InsertGuiPair(button_ok,"Ok");
	InsertGuiPair(button_cancel,"Cancelar");
	InsertGuiPair(message_box_caption,"TTCalc");

	InsertGuiPair(dialog_box_add_variable_caption,"Añadir nueva variable");
	InsertGuiPair(dialog_box_edit_variable_caption,"Editar variable");
	InsertGuiPair(dialog_box_add_variable_incorrect_name,"Nombre incorrecto de variable");
	InsertGuiPair(dialog_box_add_variable_incorrect_value,"Valor incorrecto de variable");
	InsertGuiPair(dialog_box_add_variable_variable_exists,"Esta variable ya existe");
	InsertGuiPair(dialog_box_edit_variable_unknown_variable,"No hay variable en mi tabla!. Seguramente haya un error interno!");
	InsertGuiPair(dialog_box_delete_variable_confirm,"Quiere borrar las variables internas?");
	InsertGuiPair(dialog_box_variable_not_all_deleted,"Hay algunas variables que no se pueden borrar. Probablemente hay un error interno!");
	InsertGuiPair(dialog_box_add_function_caption,"Añadir funcion");
	InsertGuiPair(dialog_box_edit_function_caption,"Editar funcion");
	InsertGuiPair(dialog_box_add_function_function_exists,"Esta funcion ya existe");
	InsertGuiPair(dialog_box_edit_function_unknown_function,"No existe esta funcion en mi tabla. Problablemente error interno!");
	InsertGuiPair(dialog_box_delete_function_confirm,"Quiere borrar estas funciones?");
	InsertGuiPair(dialog_box_function_not_all_deleted,"Hay algunas funciones que no se pueden borrar. Probablmenete error interno!");
	InsertGuiPair(dialog_box_add_function_incorrect_name,"Nombre incorrecto de la función");

	InsertGuiPair(list_variables_header_1,"Nombre");
	InsertGuiPair(list_variables_header_2,"Valor");
	InsertGuiPair(list_functions_header_1,"Nombre");
	InsertGuiPair(list_functions_header_2,"Param.");
	InsertGuiPair(list_functions_header_3,"Valor");

	InsertGuiPair(button_add,"Añadir");
	InsertGuiPair(button_edit,"Editar");
	InsertGuiPair(button_delete,"Borrar");
	InsertGuiPair(button_clear,"C");
	InsertGuiPair(tab_standard,"Standard");
	InsertGuiPair(tab_variables,"Variables");
	InsertGuiPair(tab_functions,"Funciones");
	InsertGuiPair(tab_precision,"Precisión");
	InsertGuiPair(tab_display,"Pantalla");
	InsertGuiPair(tab_convert,"Convertir");

	InsertGuiPair(radio_precision_1,"Pequeño - 96 bits para la mantisa, 32 bits para el exponente");
	InsertGuiPair(radio_precision_2,"Mediano - 512 bits para la mantissa, 64 bits para el exponente");
	InsertGuiPair(radio_precision_3,"Grande - 1024 bits para la mantissa, 128 bits para el exponente");

	InsertGuiPair(precision_1_info,"(+/-)6.97...e+646457021, 26 dígitos válidos (decimal)");
	InsertGuiPair(precision_2_info,"(+/-)9.25...e+2776511644261678719, 152 dígitos válidos");
	InsertGuiPair(precision_3_info,"(+/-)9.80...*10^(10^37), 306 dígitos válidos");

	InsertGuiPair(overflow_during_printing,"Sobredimensión durante impresión");

	InsertGuiPair(combo_rounding_none,"Ninguno");
	InsertGuiPair(combo_rounding_integer,"a entero");
	InsertGuiPair(combo_rounding_to_number,"a número");
	// this text was too long (the combobox is shorter now) -- I'm not sure whether this is correct now... (I don't know spanish at all)
	//InsertGuiPair(combo_rounding_to_number,"a");
	InsertGuiPair(combo_rounding_after_comma,"digito(s)");
	InsertGuiPair(check_remove_zeroes,"Eliminar ceros no-significativos");

	InsertGuiPair(display_input,			"Entrada");
	InsertGuiPair(display_output,			"Salida");
	InsertGuiPair(display_rounding,			"Redondeo");
	InsertGuiPair(display_always_scientific,"Siempre");
	InsertGuiPair(display_not_always_scientific,"Cuando el exponente es mas grande que:");
	InsertGuiPair(display_as_scientific,	"Imprimir el resultado como valor científico");
	InsertGuiPair(display_deg_rad_grad,		"Funciones trigonométricas");
	InsertGuiPair(display_grouping,			"Agrupar");					// e.g. 1`000`000
	InsertGuiPair(display_grouping_none,	"Ninguno");
	InsertGuiPair(display_grouping_space,	"Espacio");
	InsertGuiPair(display_grouping_digits,  "digito(s)");
	InsertGuiPair(display_input_decimal_point,	"Símbolo decimal en fórmula"); 	// Decimal point in formula
	InsertGuiPair(display_input_decimal_point_item1,	" . o ,");     	
	InsertGuiPair(display_output_decimal_point,	"Símbolo decimal en resultado");	// Decimal point in result
	InsertGuiPair(display_param_sep,		"Parámetros separados por");
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
	InsertGuiPair(menu_view_lang_english,	"&Inglés");
	InsertGuiPair(menu_view_lang_polish,	"&Polaco");
	InsertGuiPair(menu_view_lang_spanish,	"&Español");
	InsertGuiPair(menu_view_lang_danish,	"&Danés");
	InsertGuiPair(menu_view_lang_chinese,	"&Chino");
	InsertGuiPair(menu_view_lang_russian,	"&Ruso");
	InsertGuiPair(menu_view_lang_swedish,   "&Sueco");
	InsertGuiPair(menu_view_lang_italian,   "I&talian");
	InsertGuiPair(menu_view_lang_german,    "&Alemán"); // is it a good translation?
	InsertGuiPair(menu_view_close_program,	"&Close");

	InsertGuiPair(menu_edit_undo,			"&Deshacer \tCtrl+Z");
	InsertGuiPair(menu_edit_cut,			"Cor&tar \tCtrl+X");
	InsertGuiPair(menu_edit_copy,			"&Copiar \tCtrl+C");
	InsertGuiPair(menu_edit_paste,			"&Pegar");						// cutting, copying, pasting the the edit which has a focus
	InsertGuiPair(menu_edit_del,			"&Borrar \tDel");
	InsertGuiPair(menu_edit_copy_result,	"Copiar &resultado \tCtrl+R");
	InsertGuiPair(menu_edit_copy_both,		"Copiar &ambos \tCtrl+B");		// copying in this way: "input edit = output edit"
	InsertGuiPair(menu_edit_paste_formula,	"Pegar &fórmula\tCtrl+V");		// pasting directly to the input edit
	InsertGuiPair(menu_edit_select_all,		"Seleccionar &todo \tEsc");
	InsertGuiPair(menu_edit_bracket,		"C&orchetes \tCtrl+0");			// inserting brackets: (...)
	InsertGuiPair(menu_edit_swap,			"&Swap \tCtrl+W");				// swapping input edit for output edit
	InsertGuiPair(menu_update_available,	"&Nueva versión disponible");

	InsertGuiPair(menu_help_help,			"&Ayuda");
	InsertGuiPair(menu_help_project_page,	"&Pagina del proyecto");
	InsertGuiPair(menu_help_check_update,	"&Comprobar actualizaciones...");
	InsertGuiPair(menu_help_about,			"Acerca &de");

	InsertGuiPair(cant_init_calculations,	"No se puede inicializar el modulo de cálculo");
	InsertGuiPair(message_box_error_caption,"TTCalc");
	InsertGuiPair(cant_create_thread,		"No se puede crear la segunda hebra para calcular");
	InsertGuiPair(cant_create_pad,			"I could not create the pad window");
	InsertGuiPair(cant_create_main_window,	"No se puede crear la ventana principal para la aplicación");
	InsertGuiPair(cant_init_common_controls,"No se pueden inicializar los controles comunes (InitCommonControlsEx)");
	InsertGuiPair(about_text,
		"Calculadora matemática TTCalc %d.%d.%d%s%s\r\n\r\n"
		"Autor: Tomasz Sowa\r\n"
		"Contacto: t.sowa@ttmath.org\r\n"
		"Licencia: BSD (open source)\r\n"
		"Página del proyecto: http://ttcalc.sourceforge.net\r\n"
		"Biblioteca Bignum: TTMath %d.%d.%d%s (%s)\r\n"
		"Bignum type: binary floating point numbers\r\n"
		"Lenguaje de programación: C++\r\n"
		"Compilador: %s\r\n"
		"%s"	// for upx
		"\r\n"
		"TTCalc usa la biblioteca TTMath bignum"
		" la cual se puede ser encontrar en http://www.ttmath.org\r\n"
		"\r\n"
#ifdef TTCALC_PORTABLE
		"Esta es la version portátil del programa TTCalc. En esta versión "
		"se puede calcular con una precisión tal que (96 bits para la "
		"mantisa y 32 bits para el exponente)  que es aproximadamente +/-6.9e+646457021.\r\n"
		"\r\n"
#endif
		"Traducciones:\r\n"
		"Español	Alejandro Valdezate\r\n"
		"	valdezate@gmail.com\r\n"
		"Danés	Rune Bisgaard Vammen\r\n"
		"	runebisgaard@gmail.com\r\n"
		"Chino	Juis\r\n"
		"	zsyfly@gmail.com\r\n"
		"Ruso	Vladimir Gladilovich\r\n"
		"	vdgladilovich@gmail.com\r\n"
		"Sueco	Lars 'Bafvert' Gafvert\r\n"
		"	lars.gafvert@gmail.com\r\n"
		"Italian	Damiano Monaco\r\n"
		"	dmonax@gmail.com\r\n"
		"Alemán	Moritz Beleites\r\n"
		"	morbel@gmx.net\r\n"
		);
	InsertGuiPair(about_text_portable_version, " versión portatil");
	InsertGuiPair(about_text_exe_packer,	"EXE Packer: UPX 3.07\r\n");
	InsertGuiPair(about_box_title,			"Acerca de");
	InsertGuiPair(about_box_button_close,	"Cerrar");
	InsertGuiPair(unknown_error,			"Ocurrió un error desconocido");
	InsertGuiPair(cant_find_help,			"No se encuentran los ficheros de ayuda");
	InsertGuiPair(cant_open_project_page,   "No se puede abrir la página del proyecto");

	InsertGuiPair(update_title,				"Comprobar si hay alguna version nueva");
	InsertGuiPair(update_button_next,		"Siguiente");
	InsertGuiPair(update_button_finish,		"Finalizar");
	InsertGuiPair(update_button_cancel,		"Cancelar");
	InsertGuiPair(update_check_at_startup,	"Comprobar siempre al comienzo si hay una nueva versión"); // this text has been changed, need correction!
	InsertGuiPair(update_check_for_info,	"Pulse Siguiente para ver si hay actualizaciones...");
	InsertGuiPair(update_is_new_version,	"La versión %d.%d.%d está disponible, pulse Siguiente para descargar...");
	InsertGuiPair(update_no_new_version1,	"No hay versiones nuevas disponibles.");
	InsertGuiPair(update_no_new_version2,	"");
	InsertGuiPair(update_download_from,		"Descargando desde...");
#ifndef TTCALC_PORTABLE
	InsertGuiPair(update_downloaded_info1,	"Se ha descargado un nuevo programa de instalación.");
	InsertGuiPair(update_downloaded_info2,	"Pulse Finalizar para cerrar TTCalc y ejecutar el programa de instalación.");
#else
	InsertGuiPair(update_downloaded_info1,	"Se ha descargado un nuevo programa de instalación.");
	InsertGuiPair(update_downloaded_info2,	"");
#endif
	InsertGuiPair(update_download_error,	"Hubo un problema durante la descarga. Por favor, inténtelo mas tarde.");

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

	InsertGuiPair(dialog_box_add_variable_caption,"Tilføj en ny variabel");
	InsertGuiPair(dialog_box_edit_variable_caption,"Rediger en variabel");
	InsertGuiPair(dialog_box_add_variable_incorrect_name,"Et ukorrekt navn for variablen");
	InsertGuiPair(dialog_box_add_variable_incorrect_value,"En ukorrekt værdi for variablen");
	InsertGuiPair(dialog_box_add_variable_variable_exists,"Denne variabel eksisterer allerede");
	InsertGuiPair(dialog_box_edit_variable_unknown_variable,"Der er ikke denne variabel i min tabel. Der er sandsynligvis en intern fejl!");
	InsertGuiPair(dialog_box_delete_variable_confirm,"Vil du slette disse variabler?");
	InsertGuiPair(dialog_box_variable_not_all_deleted,"Der er nogle variabler det ikke var mugligt at slette. Sandsynligvis en intern fejl!");
	InsertGuiPair(dialog_box_add_function_caption,"Tilføj en ny funktion");
	InsertGuiPair(dialog_box_edit_function_caption,"Rediger en funktion");
	InsertGuiPair(dialog_box_add_function_function_exists,"Denne funktion eksisterer allerede");
	InsertGuiPair(dialog_box_edit_function_unknown_function,"Der er ikke denne funktion i min tabel. Der er sandsynligvis en intern fejl!");
	InsertGuiPair(dialog_box_delete_function_confirm,"Vil du slette disse funktioner?");
	InsertGuiPair(dialog_box_function_not_all_deleted,"Der er nogle funktioner det ikke var mugligt at slette. Sandsynligvis en intern fejl!");
	InsertGuiPair(dialog_box_add_function_incorrect_name,"Et ukorrekt navn for funktionen");

	InsertGuiPair(list_variables_header_1,"Navn");
	InsertGuiPair(list_variables_header_2,"Værdi");
	InsertGuiPair(list_functions_header_1,"Navn");
	InsertGuiPair(list_functions_header_2,"Param.");
	InsertGuiPair(list_functions_header_3,"Værdi");

	InsertGuiPair(button_add,"Tilføj");
	InsertGuiPair(button_edit,"Rediger");
	InsertGuiPair(button_delete,"Slet");
	InsertGuiPair(button_clear,"C");
	InsertGuiPair(tab_standard,"Standard");
	InsertGuiPair(tab_variables,"Variabler");
	InsertGuiPair(tab_functions,"Funktioner");
	InsertGuiPair(tab_precision,"Præcision");
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
	InsertGuiPair(check_remove_zeroes,"Fjern sidste overflødige nuller");


	InsertGuiPair(display_input,			"Input");
	InsertGuiPair(display_output,			"Output");
	InsertGuiPair(display_rounding,			"Afrunding");
	InsertGuiPair(display_always_scientific,"Altid");
	InsertGuiPair(display_not_always_scientific,"Når exponenten er større end:");
	InsertGuiPair(display_as_scientific,	"Print resultatet som den videnskablige værdi");
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
	InsertGuiPair(convert_dynamic_output,	"Auto præfix");

	InsertGuiPair(menu_view,				"&Vis");
	InsertGuiPair(menu_edit,				"&Rediger");
	InsertGuiPair(menu_help,				"&Hjælp");
	InsertGuiPair(menu_language,			"&Sprog");
	InsertGuiPair(menu_view_new_window,		"&Nyt vindue");
	InsertGuiPair(menu_view_normal_view,	"No&rmal visning");
	InsertGuiPair(menu_view_compact_view,	"K&ompakt visning");
	InsertGuiPair(menu_view_always_on_top,	"&Altid øverst");
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
	InsertGuiPair(menu_edit_paste,			"&Sæt ind");					// cutting, copying, pasting the the edit which has a focus
	InsertGuiPair(menu_edit_del,			"S&let \tDel");
	InsertGuiPair(menu_edit_copy_result,	"Kopier &resultatet \tCtrl+R");
	InsertGuiPair(menu_edit_copy_both,		"Kopier &begge \tCtrl+B");		// copying in this way: "input edit = output edit"
	InsertGuiPair(menu_edit_paste_formula,	"Indsæt f&ormular\tCtrl+V");	// pasting directly to the input edit
	InsertGuiPair(menu_edit_select_all,		"Marker &alt \tEsc");
	InsertGuiPair(menu_edit_bracket,		"Bracke&t \tCtrl+0");			// inserting brackets: (...)
	InsertGuiPair(menu_edit_swap,			"&Swap \tCtrl+W");				// swapping input edit for output edit
	InsertGuiPair(menu_help_help,			"&Hjælp");
	InsertGuiPair(menu_help_project_page,	"&Projekt side");
	InsertGuiPair(menu_help_check_update,	"O&pdater...");
	InsertGuiPair(menu_help_about,			"&Om");
	InsertGuiPair(menu_update_available,	"Der er en &ny version tilgængelig");

	InsertGuiPair(cant_init_calculations,	"Moduelt for uregninger kunne ikke sættes i gang");
	InsertGuiPair(message_box_error_caption,"TTCalc");
	InsertGuiPair(cant_create_thread,		"Den anden tråd for udregning kunne ikke laves");
	InsertGuiPair(cant_create_pad,			"I could not create the pad window");
	InsertGuiPair(cant_create_main_window,	"Applikationens hovedvindue kunne ikke laves");
	InsertGuiPair(cant_init_common_controls,"De fælles kontrol indstillinger kunne ikke sættes i gang (InitCommonControlsEx)");
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
		"kan du kun udregne med én slags præcision (96 bits for "
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
	InsertGuiPair(cant_find_help,			"Der kunne ikke findes nogen hjælp filer");
	InsertGuiPair(cant_open_project_page,   "Projekt siden kunne ikke åbnes");

	InsertGuiPair(update_title,				"Opdater TTCalc");
	InsertGuiPair(update_button_next,		"Næste");
	InsertGuiPair(update_button_finish,		"Færdig");
	InsertGuiPair(update_button_cancel,		"Annuler");
	InsertGuiPair(update_check_at_startup,	"Tjek altid for opdateringer ved opstart");  // this text has been changed, need correction!
	InsertGuiPair(update_check_for_info,	"Tryk på Næste for at tjekke for opdateringen...");
	InsertGuiPair(update_is_new_version,	"Version %d.%d.%d er tilgængelig, tryk på Næste for at downloade...");
	InsertGuiPair(update_no_new_version1,	"Der er ingen ny version tilgængelig.");
	InsertGuiPair(update_no_new_version2,	"");
	InsertGuiPair(update_download_from,		"Downloader fra...");
#ifndef TTCALC_PORTABLE
	InsertGuiPair(update_downloaded_info1,	"Et nyt installerings program er blevet downloadet.");
	InsertGuiPair(update_downloaded_info2,	"Tryk Færdig for at lukke TTCalc og køre installerings programmet.");
#else
	InsertGuiPair(update_downloaded_info1,	"En ny version af TTCalc er blevet downloadet.");
	InsertGuiPair(update_downloaded_info2,	"");
#endif
	InsertGuiPair(update_download_error,	"Der var et problem ved download, prøv igen senere.");

	InsertGuiPair(pad_title,				"Redigeringsvindue");
	InsertGuiPair(pad_menu_file,			"&Fil");
	InsertGuiPair(pad_menu_edit,			"&Rediger");
	InsertGuiPair(pad_menu_file_new,		"&Ny"); // temporarily not used
	InsertGuiPair(pad_menu_file_open,		"&Åben...");
	InsertGuiPair(pad_menu_file_saveas,		"Gem &som...");
	InsertGuiPair(pad_menu_file_close,		"&Luk");
	InsertGuiPair(pad_menu_edit_undo,		"&Fortryd \tCtrl+Z");
	InsertGuiPair(pad_menu_edit_cut,		"Kli&p \tCtrl+X");
	InsertGuiPair(pad_menu_edit_copy,		"&Kopier \tCtrl+C");
	InsertGuiPair(pad_menu_edit_paste,		"&Sæt ind \tCtrl+V");
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

	InsertGuiPair(button_ok,"È·¶¨");
	InsertGuiPair(button_cancel,"È¡Ïû");
	InsertGuiPair(message_box_caption,"TTCalc");

	InsertGuiPair(dialog_box_add_variable_caption,"Ìí¼ÓĞÂ±äÁ¿");
	InsertGuiPair(dialog_box_edit_variable_caption,"±à¼­±äÁ¿");
	InsertGuiPair(dialog_box_add_variable_incorrect_name,"´íÎóµÄ±äÁ¿Ãû");
	InsertGuiPair(dialog_box_add_variable_incorrect_value,"´íÎóµÄ±äÁ¿Öµ");
	InsertGuiPair(dialog_box_add_variable_variable_exists,"´Ë±äÁ¿ÒÑ´æÔÚ");
	InsertGuiPair(dialog_box_edit_variable_unknown_variable,"±í¸ñÖĞÃ»ÓĞ´Ë±äÁ¿¡£¿ÉÄÜÊÇÄÚ²¿´íÎó£¡");
	InsertGuiPair(dialog_box_delete_variable_confirm,"ÄãÈ·¶¨ÒªÉ¾³ıÕâĞ©±äÁ¿Âğ£¿");
	InsertGuiPair(dialog_box_variable_not_all_deleted,"Ò»Ğ©±äÁ¿²»ÄÜÉ¾³ı¡£¿ÉÄÜÊÇÄÚ²¿´íÎó£¡");
	InsertGuiPair(dialog_box_add_function_caption,"Ìí¼ÓĞÂº¯Êı");
	InsertGuiPair(dialog_box_edit_function_caption,"±à¼­º¯Êı");
	InsertGuiPair(dialog_box_add_function_function_exists,"´Ëº¯ÊıÒÑ´æÔÚ");
	InsertGuiPair(dialog_box_edit_function_unknown_function,"±í¸ñÖĞÃ»ÓĞ´Ëº¯Êı¡£¿ÉÄÜÊÇÄÚ²¿´íÎó£¡");
	InsertGuiPair(dialog_box_delete_function_confirm,"ÄãÈ·¶¨ÒªÉ¾³ıÕâĞ©º¯ÊıÂğ£¿");
	InsertGuiPair(dialog_box_function_not_all_deleted,"Ò»Ğ©º¯Êı²»ÄÜÉ¾³ı¡£¿ÉÄÜÊÇÄÚ²¿´íÎó£¡");
	InsertGuiPair(dialog_box_add_function_incorrect_name,"´íÎóµÄº¯ÊıÃû");

	InsertGuiPair(list_variables_header_1,"Ãû³Æ");
	InsertGuiPair(list_variables_header_2,"Öµ");
	InsertGuiPair(list_functions_header_1,"º¯ÊıÃû");
	InsertGuiPair(list_functions_header_2,"²ÎÊı¸öÊı");
	InsertGuiPair(list_functions_header_3,"±í´ïÊ½");

	InsertGuiPair(button_add,"Ìí¼Ó");
	InsertGuiPair(button_edit,"±à¼­");
	InsertGuiPair(button_delete,"É¾³ı");
	InsertGuiPair(button_clear,"C");
	InsertGuiPair(tab_standard,"±ê×¼");
	InsertGuiPair(tab_variables,"±äÁ¿");
	InsertGuiPair(tab_functions,"º¯Êı");
	InsertGuiPair(tab_precision,"¾«¶È");
	InsertGuiPair(tab_display,"ÏÔÊ¾");
	InsertGuiPair(tab_convert,"×ª»»");

	InsertGuiPair(radio_precision_1,"Ğ¡ -   96 Î» Î²Êı,  32Î» Ö¸Êı");
	InsertGuiPair(radio_precision_2,"ÖĞ -  512 Î» Î²Êı,  64Î» Ö¸Êı");
	InsertGuiPair(radio_precision_3,"´ó - 1024 Î» Î²Êı, 128Î» Ö¸Êı");


	InsertGuiPair(precision_1_info,"(+/-)6.97...e+646457021, 26Î»ÓĞĞ§Êı×Ö(Ê®½øÖÆ)");
	InsertGuiPair(precision_2_info,"(+/-)9.25...e+2776511644261678719, 152Î»ÓĞĞ§Êı×Ö");
	InsertGuiPair(precision_3_info,"(+/-)9.80...*10^(10^37), 306Î»ÓĞĞ§Êı×Ö");

	InsertGuiPair(overflow_during_printing,"Êä³öÊ±Êı¾İÒç³ö");

	InsertGuiPair(combo_rounding_none,"ÎŞ");
	InsertGuiPair(combo_rounding_integer,"ÕûÊı");
	InsertGuiPair(combo_rounding_to_number,"×ª»»");
	InsertGuiPair(combo_rounding_after_comma,"Î»Êı");
	InsertGuiPair(check_remove_zeroes,"È¥µôÎ²ËæÁã");

	InsertGuiPair(display_input,			"ÊäÈë");
	InsertGuiPair(display_output,			"Êä³ö");
	InsertGuiPair(display_rounding,			"ËÄÉáÎåÈë");
	InsertGuiPair(display_always_scientific,"Ò»Ö±");
	InsertGuiPair(display_not_always_scientific,"µ±Ö¸Êı´óÓÚ:");
	InsertGuiPair(display_as_scientific,	"°´¿ÆÑ§¼ÆÊı·¨Êä³ö½á¹û");
	InsertGuiPair(display_deg_rad_grad,		"Èı½Çº¯Êı");
	InsertGuiPair(display_grouping,			"·Ö×é·û");					// e.g. 1`000`000
	InsertGuiPair(display_grouping_none,	"ÎŞ");
	InsertGuiPair(display_grouping_space,	"¿Õ¸ñ");
	InsertGuiPair(display_grouping_digits,  "Î»Êı");
	InsertGuiPair(display_input_decimal_point,	"Ğ¡ÊıµãÊäÈë¸ñÊ½"); 	// Decimal point in formula
	InsertGuiPair(display_input_decimal_point_item1,	" . or ,");     	
	InsertGuiPair(display_output_decimal_point,	"Ğ¡ÊıµãÊä³ö¸ñÊ½");	// Decimal point in result
	InsertGuiPair(display_param_sep,		"²ÎÊı·Ö¸ô·û");
	InsertGuiPair(display_param_sep_item2,	" ; or .");
	InsertGuiPair(display_param_sep_item3,	" ; or ,");

	InsertGuiPair(convert_type,				"ÀàĞÍ");
	InsertGuiPair(convert_input,			"ÊäÈë");
	InsertGuiPair(convert_output,			"Êä³ö");
	InsertGuiPair(convert_dynamic_output,	"×Ô¶¯");

	InsertGuiPair(menu_view,				"&²é¿´");
	InsertGuiPair(menu_edit,				"&±à¼­");
	InsertGuiPair(menu_help,				"&°ïÖú");
	InsertGuiPair(menu_language,			"&ÓïÑÔ");
	InsertGuiPair(menu_view_new_window,		"&ĞÂ´°¿Ú");
	InsertGuiPair(menu_view_normal_view,	"&ÆÕÍ¨ÊÓÍ¼");
	InsertGuiPair(menu_view_compact_view,	"&ÃÔÄãÊÓÍ¼");
	InsertGuiPair(menu_view_always_on_top,	"&×ÜÔÚ×îÇ°");
	InsertGuiPair(menu_view_pad,			"&ÎÄ±¾¼ÆËãÆ÷");
	InsertGuiPair(menu_view_lang_english,	"&Ó¢Óï");
	InsertGuiPair(menu_view_lang_polish,	"&²¨À¼Óï");
	InsertGuiPair(menu_view_lang_spanish,	"&Î÷°àÑÀÓï");
	InsertGuiPair(menu_view_lang_danish,	"&µ¤ÂóÓï");
	InsertGuiPair(menu_view_lang_chinese,	"&¼òÌåÖĞÎÄ");
	InsertGuiPair(menu_view_lang_russian,	"&¶íÓï");
	InsertGuiPair(menu_view_lang_swedish,   "&ÈğµäÓï");
	InsertGuiPair(menu_view_lang_italian,   "&Òâ´óÀûÓï");
	InsertGuiPair(menu_view_lang_german,    "&µÂÓï");
	InsertGuiPair(menu_view_close_program,	"&¹Ø±Õ");
	InsertGuiPair(menu_edit_undo,			"&È¡Ïû \tCtrl+Z");
	InsertGuiPair(menu_edit_cut,			"&¼ôÇĞ \tCtrl+X");
	InsertGuiPair(menu_edit_copy,			"&¸´ÖÆ \tCtrl+C");
	InsertGuiPair(menu_edit_paste,			"&Õ³Ìù");						// cutting, copying, pasting the the edit which has a focus
	InsertGuiPair(menu_edit_del,			"&É¾³ı \tDel");
	InsertGuiPair(menu_edit_copy_both,		"&¸´ÖÆÈ«²¿(ÊäÈë=Êä³ö) \tCtrl+B");			// copying in this way: "input edit = output edit"
	InsertGuiPair(menu_edit_paste_formula,	"&Õ³Ìùµ½ÊäÈë¿ò\tCtrl+V");		// pasting directly to the input edit
	InsertGuiPair(menu_edit_copy_result,	"&¸´ÖÆ½á¹û \tCtrl+R");
	InsertGuiPair(menu_edit_select_all,		"&È«Ñ¡\tEsc");
	InsertGuiPair(menu_edit_bracket,		"&¼ÓÀ¨ºÅ \tCtrl+0");			// inserting brackets: (...)
	InsertGuiPair(menu_edit_swap,			"&ÊäÈë¼ÆËã½á¹û \tCtrl+W");				// swapping input edit for output edit
	InsertGuiPair(menu_help_help,			"&°ïÖú");
	InsertGuiPair(menu_help_project_page,	"&Èí¼şÖ÷Ò³");
	InsertGuiPair(menu_help_about,			"&¹ØÓÚ");
	InsertGuiPair(menu_help_check_update,	"&¼ì²é¸üĞÂ");
	InsertGuiPair(menu_update_available,	"&ÓĞ¿ÉÓÃµÄĞÂ°æ±¾"); // not implemented yet
	// this will be a new main menu item (one after 'help') visible only if there is an update

	InsertGuiPair(cant_init_calculations,	"²»ÄÜ³õÊ¼»¯¼ÆËãÆ÷Ä£ĞÍ");
	InsertGuiPair(message_box_error_caption,"TTCalc");
	InsertGuiPair(cant_create_thread,		"²»ÄÜ´´½¨Á½¸öÏß³ÌÀ´ÔËËã");
	InsertGuiPair(cant_create_pad,			"²»ÄÜ´´½¨ÎÄ±¾¼ÆËã´°¿Ú");
	InsertGuiPair(cant_create_main_window,	"²»ÄÜ´´½¨Ó¦ÓÃ³ÌĞòÖ÷´°¿Ú");
	InsertGuiPair(cant_init_common_controls,"²»ÄÜ³õÊ¼»¯Í¨ÓÃ¿Ø¼ş");
	InsertGuiPair(about_text,
		"Mathematical calculator TTCalc %d.%d.%d%s%s\r\n\r\n"
		"¿ª·¢ÈË: Tomasz Sowa\r\n"
		"ÁªÏµ: t.sowa@ttmath.org\r\n"
		"Ğí¿É: BSD (¿ªÔ´ÏîÄ¿)\r\n"
		"Ö÷Ò³: http://ttcalc.sourceforge.net\r\n"
		"Bignum library: TTMath %d.%d.%d%s (%s)\r\n"
		"Bignum type: binary floating point numbers\r\n"
		"±à³ÌÓïÑÔ: C++\r\n"
		"±àÒëÆ÷: %s\r\n"
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
		"·­Òë:\r\n"
		"Î÷°àÑÀÓï	Alejandro S. Valdezate\r\n"
		"		alesanval@gmail.com\r\n"
		"µ¤ÂóÓï		Rune Bisgaard Vammen\r\n"
		"		runebisgaard@gmail.com\r\n"
		"¼òÌåÖĞÎÄ	Juis\r\n"
		"		zsyfly@gmail.com\r\n"
		"¶íÓï		Vladimir Gladilovich\r\n"
		"		vdgladilovich@gmail.com\r\n"
		"ÈğµäÓï		Lars 'Bafvert' Gafvert\r\n"
		"		lars.gafvert@gmail.com\r\n"
		"Òâ´óÀûÓï		Damiano Monaco\r\n"
		"		dmonax@gmail.com\r\n"
		"µÂÓï		Moritz Beleites\r\n"
		"		morbel@gmx.net\r\n"
		);
	InsertGuiPair(about_text_portable_version, " portable version");
	InsertGuiPair(about_text_exe_packer,	"EXE Packer: UPX 3.07\r\n");
	InsertGuiPair(about_box_title,			"¹ØÓÚ");
	InsertGuiPair(about_box_button_close,	"¹Ø±Õ");
	InsertGuiPair(unknown_error,			"·¢ÉúÎ´Öª´íÎó");
	InsertGuiPair(cant_find_help,			"Ã»ÓĞÕÒµ½°ïÖúÎÄ¼ş");
	InsertGuiPair(cant_open_project_page,   "²»ÄÜ´ò¿ªÈí¼şÖ÷Ò³");

	InsertGuiPair(update_title,				"¼ì²é¸üĞÂ");
	InsertGuiPair(update_button_next,		"ÏÂÒ»²½");
	InsertGuiPair(update_button_finish,		"Íê³É");
	InsertGuiPair(update_button_cancel,		"È¡Ïû");
	InsertGuiPair(update_check_at_startup,	"Æô¶¯Ê±×ÜÊÇ¼ì²é¸üĞÂ");  // this text has been changed, need correction!
	InsertGuiPair(update_check_for_info,	"°´\"ÏÂÒ»²½\"½øĞĞ¸üĞÂ...");
	InsertGuiPair(update_is_new_version,	"°æ±¾ %d.%d.%d ¿ÉÓÃ, °´\"ÏÂÒ»²½\"ÏÂÔØ...");
	InsertGuiPair(update_no_new_version1,	"Ã»ÓĞ¿ÉÓÃµÄĞÂ°æ±¾¡£");
	InsertGuiPair(update_no_new_version2,	"");
	InsertGuiPair(update_download_from,		"ÕıÔÚÏÂÔØ...");
#ifndef TTCALC_PORTABLE
	InsertGuiPair(update_downloaded_info1,	"ĞÂ°æ±¾°²×°³ÌĞòÒÑ¾­ÏÂÔØÍê±Ï¡£");
	InsertGuiPair(update_downloaded_info2,	"µã»÷Íê³ÉÒÔ¹Ø±Õ³ÌĞò²¢¿ªÊ¼°²×°¡£.");
#else
	InsertGuiPair(update_downloaded_info1,	"ĞÂ°æ±¾°²×°³ÌĞòÒÑ¾­ÏÂÔØÍê±Ï¡£");
	InsertGuiPair(update_downloaded_info2,	"");
#endif
	InsertGuiPair(update_download_error,	"ÏÂÔØÊ±³öÏÖ´íÎó£¬ÇëÉÔºóÖØÊÔ¡£");

	InsertGuiPair(pad_title,				"&ÎÄ±¾¼ÆËãÆ÷");
	InsertGuiPair(pad_menu_file,			"&ÎÄ¼ş");
	InsertGuiPair(pad_menu_edit,			"&±à¼­");
	InsertGuiPair(pad_menu_file_new,		"&ĞÂ½¨"); // temporarily not used
	InsertGuiPair(pad_menu_file_open,		"&´ò¿ª...");
	InsertGuiPair(pad_menu_file_saveas,		"&Áí´æÎª...");
	InsertGuiPair(pad_menu_file_close,		"&¹Ø±Õ");
	InsertGuiPair(pad_menu_edit_undo,		"&³·Ïú \tCtrl+Z");
	InsertGuiPair(pad_menu_edit_cut,		"&¼ôÇĞ \tCtrl+X");
	InsertGuiPair(pad_menu_edit_copy,		"&¸´ÖÆ \tCtrl+C");
	InsertGuiPair(pad_menu_edit_paste,		"&Õ³Ìù \tCtrl+V");
	InsertGuiPair(pad_menu_edit_del,		"&É¾³ı \tDel");
	InsertGuiPair(pad_menu_edit_select_all,	"&È«Ñ¡ \tCtrl+A");

	InsertGuiPair(cannot_open_file,			"Î´ÄÜ´ò¿ªÎÄ¼ş");
	InsertGuiPair(cannot_save_file,			"²»ÄÜ±£´æÎÄ¼ş");
	InsertGuiPair(file_too_long,			"ÎÄ¼şÌ«³¤");
	InsertGuiPair(other_error,				"There was a problem with this operation"); // for other kinds of errors




/*
	russian gui messages
*/
	gui_messages_tab.push_back( std::map<GuiMsg, std::string>() );

	InsertGuiPair(button_ok,"Ok");
	InsertGuiPair(button_cancel,"Îòìåíà");
	InsertGuiPair(message_box_caption,"TTCalc");

	InsertGuiPair(dialog_box_add_variable_caption,"Äîáàâèòü íîâóş ïåğåìåííóş");
	InsertGuiPair(dialog_box_edit_variable_caption,"Ğåäàêòèğîâàòü ïåğåìåííóş");
	InsertGuiPair(dialog_box_add_variable_incorrect_name,"Íåïğàâèëüíîå èìÿ ïåğåìåííîé");
	InsertGuiPair(dialog_box_add_variable_incorrect_value,"Íåïğàâèëüíîå çíà÷åíèå ïåğåìåííîé");
	InsertGuiPair(dialog_box_add_variable_variable_exists,"Òàêàÿ ïåğåìåííàÿ óæå çàäàíà");
	InsertGuiPair(dialog_box_edit_variable_unknown_variable,"Òàêîé ïåğåìåííîé íåò â òàáëèöå. Âåğîÿòíî, âíóòğåííÿÿ îøèáêà!");
	InsertGuiPair(dialog_box_delete_variable_confirm,"Óäàëèòü ıòè ïåğåìåííûå?");
	InsertGuiPair(dialog_box_variable_not_all_deleted,"Íåêîòîğûå ïåğåìåííûå íå ïîëó÷èëîñü óäàëèòü. Âåğîÿòíî, âíóòğåííÿÿ îøèáêà!");
	InsertGuiPair(dialog_box_add_function_caption,"Äîáàâèòü íîâóş ôóíêöèş");
	InsertGuiPair(dialog_box_edit_function_caption,"Ğåäàêòèğîâàòü íîâóş ôóíêöèş");
	InsertGuiPair(dialog_box_add_function_function_exists,"Òàêàÿ ôóíêöèÿ óæå çàäàíà");
	InsertGuiPair(dialog_box_edit_function_unknown_function,"Òàêîé ôóíêöèè íåò â òàáëèöå. Âåğîÿòíî, âíóòğåííÿÿ îøèáêà!");
	InsertGuiPair(dialog_box_delete_function_confirm,"Óäàëèòü ıòè ôóíêöèè?");
	InsertGuiPair(dialog_box_function_not_all_deleted,"Íåêîòîğûå ôóíêöèè íå ïîëó÷èëîñü óäàëèòü. Âåğîÿòíî, âíóòğåííÿÿ îøèáêà!");
	InsertGuiPair(dialog_box_add_function_incorrect_name,"Íåïğàâèëüíîå èìÿ ôóíêöèè");

	InsertGuiPair(list_variables_header_1,"Èìÿ");
	InsertGuiPair(list_variables_header_2,"Çíà÷åíèå");
	InsertGuiPair(list_functions_header_1,"Èìÿ");
	InsertGuiPair(list_functions_header_2,"Ïàğàìåòğû");
	InsertGuiPair(list_functions_header_3,"Çíà÷åíèå");

	InsertGuiPair(button_add,"Äîáàâèòü");
	InsertGuiPair(button_edit,"Ïğàâêà");
	InsertGuiPair(button_delete,"Óäàëèòü");
	InsertGuiPair(button_clear,"C");
	InsertGuiPair(tab_standard,"Ñòàíäàğò");
	InsertGuiPair(tab_variables,"Ïåğåìåííûå");
	InsertGuiPair(tab_functions,"Ôóíêöèè");
	InsertGuiPair(tab_precision,"Òî÷íîñòü");
	InsertGuiPair(tab_display,"Îïöèè");
	InsertGuiPair(tab_convert,"Êîíâåğòàöèÿ");

	InsertGuiPair(radio_precision_1,"Ìàëàÿ - 96 áèò íà ìàíòèññó, 32 áèòà íà ïîêàçàòåëü ñòåïåíè");
	InsertGuiPair(radio_precision_2,"Ñğåäíÿÿ - 512 áèò íà ìàíòèññó, 64 áèòà íà ïîêàçàòåëü ñòåïåíè");
	InsertGuiPair(radio_precision_3,"Áîëüøàÿ - 1024 áèò íà ìàíòèññó, 128 áèòà íà ïîêàçàòåëü ñòåïåíè");


	InsertGuiPair(precision_1_info,"(+/-)6.97...e+646457021, 26 çíà÷àùèõ öèôğ (äåñÿòè÷íûõ)");
	InsertGuiPair(precision_2_info,"(+/-)9.25...e+2776511644261678719, 152 çíà÷àùèõ öèôğ");
	InsertGuiPair(precision_3_info,"(+/-)9.80...*10^(10^37), 306 çíà÷àùèõ öèôğ");

	InsertGuiPair(overflow_during_printing,"Ïåğåïîëíåíèå ïğè âûâîäå");

	InsertGuiPair(combo_rounding_none,"íåò");
	InsertGuiPair(combo_rounding_integer,"äî öåëîãî");
	InsertGuiPair(combo_rounding_to_number,"äî");
	InsertGuiPair(combo_rounding_after_comma,"çíàêà");
	InsertGuiPair(check_remove_zeroes,"Óäàëÿòü êîíö. íóëåâûå áàéòû");

	InsertGuiPair(display_input,			"Ââîä");
	InsertGuiPair(display_output,			"Âûâîä");
	InsertGuiPair(display_rounding,			"Îêğóãëåíèå");
	InsertGuiPair(display_always_scientific,"Âñåãäà");
	InsertGuiPair(display_not_always_scientific,"Êîãäà ïîêàçàòåëü ñòåïåíè áîëüøå");
	InsertGuiPair(display_as_scientific,	"Âûâîäèòü ğåçóëüòàò â ıêñïîíåíöèàëüíîì âèäå");

	InsertGuiPair(display_deg_rad_grad,		"Òğèãîíîìåòğèÿ");
	InsertGuiPair(display_grouping,			"Ğàçä. ãğóïï");					// e.g. 1`000`000
	InsertGuiPair(display_grouping_none,	"Íè÷åãî");
	InsertGuiPair(display_grouping_space,	"Ïğîáåë");
	InsertGuiPair(display_grouping_digits,  "çíàêà");
	InsertGuiPair(display_input_decimal_point,	"Äåñÿòè÷íàÿ òî÷êà (ââîä)"); 	// Decimal point in formula
	InsertGuiPair(display_input_decimal_point_item1,	" . èëè ,");     	
	InsertGuiPair(display_output_decimal_point,	"Äåñÿòè÷íàÿ òî÷êà (âûâîä)");	// Decimal point in result
	InsertGuiPair(display_param_sep,		"Ïàğàìåòğû ğàçäåëÿşòñÿ");
	InsertGuiPair(display_param_sep_item2,	" ; èëè .");
	InsertGuiPair(display_param_sep_item3,	" ; èëè ,");

	InsertGuiPair(convert_type,				"Òèï");
	InsertGuiPair(convert_input,			"Ââîä");
	InsertGuiPair(convert_output,			"Âûâîä");
	InsertGuiPair(convert_dynamic_output,	"Àâòîïğåôèêñ");

	InsertGuiPair(menu_view,				"&Âèä");
	InsertGuiPair(menu_edit,				"&Ïğàâêà");
	InsertGuiPair(menu_help,				"Ï&îìîùü");
	InsertGuiPair(menu_language,			"&ßçûê");
	InsertGuiPair(menu_view_new_window,		"&Íîâîå îêíî");
	InsertGuiPair(menu_view_normal_view,	"&Îáû÷íûé âèä");
	InsertGuiPair(menu_view_compact_view,	"&Êîìïàêòíûé âèä");
	InsertGuiPair(menu_view_always_on_top,	"&Âñåãäà ïîâåğõ äğóãèõ îêîí");
	InsertGuiPair(menu_view_pad,			"Á&ëîêíîò");
	InsertGuiPair(menu_view_lang_english,	"&Àíãëèéñêèé");
	InsertGuiPair(menu_view_lang_polish,	"&Ïîëüñêèé");
	InsertGuiPair(menu_view_lang_spanish,	"&Èñïàíñêèé");
	InsertGuiPair(menu_view_lang_danish,	"&Äàòñêèé");
	InsertGuiPair(menu_view_lang_chinese,	"&Êèòàéñêèé");
	InsertGuiPair(menu_view_lang_russian,	"&Ğóññêèé");
	InsertGuiPair(menu_view_lang_swedish,   "&Øâåäñêèé");
	InsertGuiPair(menu_view_lang_italian,   "&Italian"); // to translate
	InsertGuiPair(menu_view_lang_german,    "&German");  // to translate
	InsertGuiPair(menu_view_close_program,	"&Çàêğûòü");

	InsertGuiPair(menu_edit_undo,			"&Îòìåíèòü äåéñòâèå \tCtrl+Z");
	InsertGuiPair(menu_edit_cut,			"Âû&ğåçàòü \tCtrl+X");
	InsertGuiPair(menu_edit_copy,			"&Êîïèğîâàòü \tCtrl+C");
	InsertGuiPair(menu_edit_paste,			"&Âñòàâèòü");							// cutting, copying, pasting the the edit which has a focus
	InsertGuiPair(menu_edit_del,			"&Óäàëèòü \tDel");
	InsertGuiPair(menu_edit_copy_result,	"Ê&îïèğîâàòü ğåçóëüòàò \tCtrl+R");
	InsertGuiPair(menu_edit_copy_both,		"Êîïèğîâàòü ââîä &è âûâîä \tCtrl+B");	// copying in this way: "input edit = output edit"
	InsertGuiPair(menu_edit_paste_formula,	"Âñòàâèòü â &ôîğìóëó \tCtrl+V");		// pasting directly to the input edit
	InsertGuiPair(menu_edit_select_all,		"Âûä&åëèòü âñ¸ \tEsc");
	InsertGuiPair(menu_edit_bracket,		"Âñòàâèòü ñêî&áêè \tCtrl+0");			// inserting brackets: (...)
	InsertGuiPair(menu_edit_swap,			"&Swap \tCtrl+W");						// swapping input edit for output edit
	InsertGuiPair(menu_help_check_update,	"Ïğîâåğèòü î&áíîâëåíèÿ...");
	InsertGuiPair(menu_update_available,	"&Äîñòóïíà íîâàÿ âåğñèÿ!");

	InsertGuiPair(menu_help_help,			"&Ïîìîùü");
	InsertGuiPair(menu_help_project_page,	"&Ñàéò ïğîåêòà");
	InsertGuiPair(menu_help_about,			"&Î ïğîãğàììå");

	InsertGuiPair(cant_init_calculations,	"Íå ìîãó èíèöèàëèçèğîâàòü ìîäóëü äëÿ ğàñ÷åòîâ");
	InsertGuiPair(message_box_error_caption,"TTCalc");
	InsertGuiPair(cant_create_thread,		"Íå ìîãó ñîçäàòü âòîğîé ïîòîê äëÿ ğàñ÷åòîâ");
	InsertGuiPair(cant_create_pad,			"I could not create the pad window");
	InsertGuiPair(cant_create_main_window,	"Íå ìîãó îòêğûòü ãëàâíîå îêíî ïğîãğàììû");
	InsertGuiPair(cant_init_common_controls,"Íå ìîãó èíèöèàëèçèğîâàòü óïğàâëåíèå (InitCommonControlsEx)");
	InsertGuiPair(about_text,
		"Ìàòåìàòè÷åñêèé êàëüêóëÿòîğ TTCalc %d.%d.%d%s%s\r\n\r\n"
		"Àâòîğ: Tomasz Sowa\r\n"
		"e-mail: t.sowa@ttmath.org\r\n"
		"Ëèöåíçèÿ: BSD (open source)\r\n"
		"Ñàéò ïğîåêòà: http://ttcalc.sourceforge.net\r\n"
		"Áèáëèîòåêà Bignum: TTMath %d.%d.%d%s (%s)\r\n"
		"Òèï Bignum: Äâîè÷íàÿ ñèñòåìà ñ÷èñëåíèÿ, ÷èñëà ñ ïëàâàşùåé çàïÿòîé\r\n"
		"ßçûê ïğîãğàììèğîâàíèÿ: C++\r\n"
		"Êîìïèëÿòîğ: %s\r\n"
		"%s"	// for upx
		"\r\n"
		"TTCalc èñïîëüçóåò áèáëèîòåêó ñâåğõáîëüøèõ ÷èñåë TTMath,"
		" êîòîğàÿ íàõîäèòñÿ çäåñü: http://www.ttmath.org\r\n"
		"\r\n"
#ifdef TTCALC_PORTABLE
		"İòî ïåğåíîñíàÿ âåğñèÿ ïğîãğàììû. Â ıòîé âåğñèè ìîæíî ñ÷èòàòü "
		"òîëüêî ñ îäíîé (ìàëîé) òî÷íîñòüş (96 áèò íà ìàíòèññó,"
		" 32 áèòà íà ïîêàçàòåëü ñòåïåíè), òî åñòü +/-6.9e+646457021.\r\n"
		"\r\n"
#endif

		"Ïåğåâîä÷èêè:\r\n"
		"Èñïàíñêèé	Alejandro S. Valdezate\r\n"
		"		alesanval@gmail.com\r\n"
		"Äàòñêèé		Rune Bisgaard Vammen\r\n"
		"		runebisgaard@gmail.com\r\n"
		"Êèòàéñêèé	Juis\r\n"
		"		zsyfly@gmail.com\r\n"
		"Ğóññêèé		Vladimir Gladilovich\r\n"
		"		vdgladilovich@gmail.com\r\n"
		"Øâåäñêèé		Lars 'Bafvert' Gafvert\r\n"
		"		lars.gafvert@gmail.com\r\n"
		"Italian		Damiano Monaco\r\n"
		"		dmonax@gmail.com\r\n"
		"German		Moritz Beleites\r\n"
		"		morbel@gmx.net\r\n"
		);
	InsertGuiPair(about_text_portable_version, " ïåğåíîñíàÿ âåğñèÿ");
	InsertGuiPair(about_text_exe_packer,	"óïàêîâùèê EXE: UPX 3.07\r\n");
	InsertGuiPair(about_box_title,			"Î ïğîãğàììå");
	InsertGuiPair(about_box_button_close,	"Çàêğûòü");
	InsertGuiPair(unknown_error,			"Íåèçâåñòíàÿ îøèáêà");
	InsertGuiPair(cant_find_help,			"Íå óäàåòñÿ íàéòè ôàéëû ïîìîùè");
	InsertGuiPair(cant_open_project_page,   "Íå óäàåòñÿ îòêğûòü ñàéò ïğîåêòà");


	InsertGuiPair(update_title,				"Ïîèñê íîâîé âåğñèè");
	InsertGuiPair(update_button_next,		"Äàëåå");
	InsertGuiPair(update_button_finish,		"Ãîòîâî");
	InsertGuiPair(update_button_cancel,		"Îòìåíà");
	InsertGuiPair(update_check_at_startup,	"Ïğîâåğÿòü íàëè÷èå îáíîâëåíèé ïğè çàãğóçêå ïğîãğàììû");  // this text has been changed, need correction!
	InsertGuiPair(update_check_for_info,	"Íàæìèòå Äàëåå, ÷òîáû ïğîâåğèòü íàëè÷èå îáíîâëåíèé...");
	InsertGuiPair(update_is_new_version,	"Äîñòóïíà âåğñèÿ %d.%d.%d, íàæìèòå Äàëåå, ÷òîáû ñêà÷àòü å¸...");
	InsertGuiPair(update_no_new_version1,	"Íåò íîâûõ âåğñèé.");
	InsertGuiPair(update_no_new_version2,	"");
	InsertGuiPair(update_download_from,		"Çàãğóæàåì ñ...");
#ifndef TTCALC_PORTABLE
	InsertGuiPair(update_downloaded_info1,	"Íîâûé äèñòğèáóòèâ çàãğóæåí.");
	InsertGuiPair(update_downloaded_info2,	"Íàæìèòå Ãîòîâî, ÷òîáû çàêğûòü TTCalc è çàïóñòèòü óñòàíîâùèê.");
#else
	InsertGuiPair(update_downloaded_info1,	"Íîâûé äèñòğèáóòèâ çàãğóæåí.");
	InsertGuiPair(update_downloaded_info2,	"");
#endif
	InsertGuiPair(update_download_error,	"Ïğîáëåìà ñ çàãğóçêîé, ïîïğîáóéòå åùå ğàç ïîçäíåå.");

	InsertGuiPair(pad_title,				"Áëîêíîò");
	InsertGuiPair(pad_menu_file,			"&Ôàéë");
	InsertGuiPair(pad_menu_edit,			"&Ïğàâêà");
	InsertGuiPair(pad_menu_file_new,		"&Íîâûé"); // temporarily not used
	InsertGuiPair(pad_menu_file_open,		"&Îòêğûòü...");
	InsertGuiPair(pad_menu_file_saveas,		"Ñîõğàíèòü &êàê...");
	InsertGuiPair(pad_menu_file_close,		"&Çàêğûòü");
	InsertGuiPair(pad_menu_edit_undo,		"&Îòìåíèòü \tCtrl+Z");
	InsertGuiPair(pad_menu_edit_cut,		"Âû&ğåçàòü \tCtrl+X");
	InsertGuiPair(pad_menu_edit_copy,		"&Êîïèğîâàòü \tCtrl+C");
	InsertGuiPair(pad_menu_edit_paste,		"&Âñòàâèòü \tCtrl+V");
	InsertGuiPair(pad_menu_edit_del,		"&Óäàëèòü \tDel");
	InsertGuiPair(pad_menu_edit_select_all,	"Âûä&åëèòü âñ¸ \tCtrl+A");

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

	InsertGuiPair(dialog_box_add_variable_caption,"Lägg till en ny variabel");
	InsertGuiPair(dialog_box_edit_variable_caption,"Redigera en variabel");
	InsertGuiPair(dialog_box_add_variable_incorrect_name,"Ogiltigt namn för variabel");
	InsertGuiPair(dialog_box_add_variable_incorrect_value,"Ogiltigt värde för variabel");
	InsertGuiPair(dialog_box_add_variable_variable_exists,"Denna variabel finns redan");
	InsertGuiPair(dialog_box_edit_variable_unknown_variable,"Denna variabel saknas i tabellen. Förmodligen ett internt fel!");
	InsertGuiPair(dialog_box_delete_variable_confirm,"Vill du förgöra dessa variabler?");
	InsertGuiPair(dialog_box_variable_not_all_deleted,"Några variabler gick ej att ta kål på. Det måste vara ett internt fel!");
	InsertGuiPair(dialog_box_add_function_caption,"Lägg till en ny funktion");
	InsertGuiPair(dialog_box_edit_function_caption,"Redigera en funktion");
	InsertGuiPair(dialog_box_add_function_function_exists,"Denna funktion finns redan");
	InsertGuiPair(dialog_box_edit_function_unknown_function,"Denna funktion saknas i tabellen. Förmodligen ett internt fel!");
	InsertGuiPair(dialog_box_delete_function_confirm,"Vill du förgöra dessa funktioner?");
	InsertGuiPair(dialog_box_function_not_all_deleted,"Några funktioner gick inte att förgöra. Jag tror vi har att göra med ett internt fel!");
	InsertGuiPair(dialog_box_add_function_incorrect_name,"Ogiltigt namn för funktionen");

	InsertGuiPair(list_variables_header_1,"Namn");
	InsertGuiPair(list_variables_header_2,"Värde");
	InsertGuiPair(list_functions_header_1,"Namn");
	InsertGuiPair(list_functions_header_2,"Param.");
	InsertGuiPair(list_functions_header_3,"Värde");

	InsertGuiPair(button_add,"Lägga till");
	InsertGuiPair(button_edit,"Redigera");
	InsertGuiPair(button_delete,"Förgöra");
	InsertGuiPair(button_clear,"C");
	InsertGuiPair(tab_standard,"Standard");
	InsertGuiPair(tab_variables,"Variabler");
	InsertGuiPair(tab_functions,"Funktioner");
	InsertGuiPair(tab_precision,"Precision");
	InsertGuiPair(tab_display,"Visning");
	InsertGuiPair(tab_convert,"Konvertera");

	InsertGuiPair(radio_precision_1,"Liten - 96 bitar för mantissan, 32 bitar för exponenten");
	InsertGuiPair(radio_precision_2,"Mellan - 512 bitar för mantissan, 64 bitar för exponenten");
	InsertGuiPair(radio_precision_3,"Stor - 1024 bitar för mantissan, 128 bitar för exponenten");


	InsertGuiPair(precision_1_info,"(+/-)6.97...e+646457021, 26 giltiga siffror (decimaler)");
	InsertGuiPair(precision_2_info,"(+/-)9.25...e+2776511644261678719, 152 giltiga siffror");
	InsertGuiPair(precision_3_info,"(+/-)9.80...*10^(10^37), 306 giltiga siffror");

	InsertGuiPair(overflow_during_printing,"Overflow under printning");

	InsertGuiPair(combo_rounding_none,"Ingen");
	InsertGuiPair(combo_rounding_integer,"till heltal");
	InsertGuiPair(combo_rounding_to_number,"till");
	InsertGuiPair(combo_rounding_after_comma,"siffror(cifre)");
	InsertGuiPair(check_remove_zeroes,"Radera sista överflödiga nollor");


	InsertGuiPair(display_input,			"Input");
	InsertGuiPair(display_output,			"Output");
	InsertGuiPair(display_rounding,			"Avrundning");
	InsertGuiPair(display_always_scientific,"Alltid");
	InsertGuiPair(display_not_always_scientific,"När exponenten är större än:");
	InsertGuiPair(display_as_scientific,	"Visa resultatet som vetenskapligt värde");
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
	InsertGuiPair(menu_help,				"&Hjälp");
	InsertGuiPair(menu_language,			"&Språg");
	InsertGuiPair(menu_view_new_window,		"&Nytt fönster");
	InsertGuiPair(menu_view_normal_view,	"No&rmal visning");
	InsertGuiPair(menu_view_compact_view,	"K&ompakt visning");
	InsertGuiPair(menu_view_always_on_top,	"&Alltid överst");
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
	InsertGuiPair(menu_edit_undo,			"&Ångra \tCtrl+Z");
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
	InsertGuiPair(menu_help_help,			"&Hjälp");
	InsertGuiPair(menu_help_project_page,	"&Projektsida");
	InsertGuiPair(menu_help_check_update,	"&Check update...");
	InsertGuiPair(menu_help_about,			"&Om");
	InsertGuiPair(menu_update_available,	"&New version is available");

	InsertGuiPair(cant_init_calculations,	"Beräkningen kunde inte initieras!");
	InsertGuiPair(message_box_error_caption,"TTCalc");
	InsertGuiPair(cant_create_thread,		"Den andra tråden för uträkning kunde inte skapas");
	InsertGuiPair(cant_create_pad,			"I could not create the pad window");
	InsertGuiPair(cant_create_main_window,	"Applikationens huvudfönster kunde inte skapas");
	InsertGuiPair(cant_init_common_controls,"Inställningarna kunde inte initieras (InitCommonControlsEx)");
	InsertGuiPair(about_text,
		"Matematisk kalkylator TTCalc %d.%d.%d%s%s\r\n\r\n"
		"Upphovsman: Tomasz Sowa\r\n"
		"Kontakt: t.sowa@ttmath.org\r\n"
		"Licens: BSD (open source)\r\n"
		"Projektsida: http://ttcalc.sourceforge.net\r\n"
		"Bignumbiblotek: TTMath %d.%d.%d%s (%s)\r\n"
		"Bignum type: binary floating point numbers\r\n"
		"Programmeringsspråk: C++\r\n"
		"Kompilator: %s\r\n"
		"%s"	// for upx
		"\r\n"
		"TTCalc använder sig av TTMath bignum bibloteket"
		" som kan hittas här: http://www.ttmath.org\r\n"
		"\r\n"
#ifdef TTCALC_PORTABLE
		"Detta är den portabla versionen av TTCalc. I denna version "
		"kan du räkna med begränsad precision, dvs (96 bitar för "
		"mantissan och 32 bitar för exponenten) det är omkring +/-6.9e+646457021.\r\n"
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
	InsertGuiPair(about_box_button_close,	"Bäver");
	InsertGuiPair(unknown_error,			"Det har inträffat ett okänt fel");
	InsertGuiPair(cant_find_help,			"Kunde inte hitta några hjälpfiler");
	InsertGuiPair(cant_open_project_page,   "Projektsidan kunde inte öppnas");
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
		" che può essere trovata su http://www.ttmath.org\r\n"
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

	InsertGuiPair(dialog_box_add_variable_caption,"Variable hinzufügen");
	InsertGuiPair(dialog_box_edit_variable_caption,"Variable bearbeiten");
	InsertGuiPair(dialog_box_add_variable_incorrect_name,"Ungültiger Variablenname");
	InsertGuiPair(dialog_box_add_variable_incorrect_value,"Ungültiger Wert der Variablen");
	InsertGuiPair(dialog_box_add_variable_variable_exists,"Diese Variable existiert bereits");
	InsertGuiPair(dialog_box_edit_variable_unknown_variable,"Variable unbekannt - vermutlich interner Fehler!");
	InsertGuiPair(dialog_box_delete_variable_confirm,"Diese Variablen löschen?");
	InsertGuiPair(dialog_box_variable_not_all_deleted,"Variablen konnten nicht alle gelöscht werden - vermutlich interner Fehler!");
	InsertGuiPair(dialog_box_add_function_caption,"Funktion hinzufügen");
	InsertGuiPair(dialog_box_edit_function_caption,"Funktion bearbeiten");
	InsertGuiPair(dialog_box_add_function_function_exists,"Diese Funktion existiert bereits");
	InsertGuiPair(dialog_box_edit_function_unknown_function,"Funktion unbekannt - vermutlich interner Fehler!");
	InsertGuiPair(dialog_box_delete_function_confirm,"Diese Funktionen löschen?");
	InsertGuiPair(dialog_box_function_not_all_deleted,"Funktionen konnten nicht alle gelöscht werden - vermutlich interner Fehler!");
	InsertGuiPair(dialog_box_add_function_incorrect_name,"Ungültiger Funktionsname");

	InsertGuiPair(list_variables_header_1,"Name");
	InsertGuiPair(list_variables_header_2,"Wert");
	InsertGuiPair(list_functions_header_1,"Name");
	InsertGuiPair(list_functions_header_2,"Parameter");
	InsertGuiPair(list_functions_header_3,"Wert");

	InsertGuiPair(button_add,"Hinzufügen");
	InsertGuiPair(button_edit,"Bearbeiten");
	InsertGuiPair(button_delete,"Löschen");
	InsertGuiPair(button_clear,"C");
	InsertGuiPair(tab_standard,"Standard");
	InsertGuiPair(tab_variables,"Variablen");
	InsertGuiPair(tab_functions,"Funktionen");
	InsertGuiPair(tab_precision,"Genauigkeit");
	InsertGuiPair(tab_display,"Anzeige");
	InsertGuiPair(tab_convert,"Konvertieren");

	InsertGuiPair(radio_precision_1,"Klein - 96 bit für Mantisse, 32 bit für Exponenten");
	InsertGuiPair(radio_precision_2,"Mittel - 512 bit für Mantisse, 64 bit für Exponenten");
	InsertGuiPair(radio_precision_3,"Groß - 1024 bit für Mantisse, 128 bit für Exponenten");

	InsertGuiPair(precision_1_info,"(+/-)6.97...e+646457021, 26 gültige Dezimalstellen");
	InsertGuiPair(precision_2_info,"(+/-)9.25...e+2776511644261678719, 152 gültige Dezimalstellen");
	InsertGuiPair(precision_3_info,"(+/-)9.80...*10^(10^37), 306 gültige Dezimalstellen");

	InsertGuiPair(overflow_during_printing,"Überlauf während der Ausgabe");

	InsertGuiPair(combo_rounding_none,"keine");
	InsertGuiPair(combo_rounding_integer,"auf Ganzzahl");
	InsertGuiPair(combo_rounding_to_number,"auf");
	InsertGuiPair(combo_rounding_after_comma,"Stelle(n)");
	InsertGuiPair(check_remove_zeroes,"Nur signifikante Nachkommastellen");

	InsertGuiPair(display_input,			"Eingabebasis");
	InsertGuiPair(display_output,			"Ergebnisbasis");
	InsertGuiPair(display_rounding,			"Rundung");
	InsertGuiPair(display_always_scientific,"Immer");
	InsertGuiPair(display_not_always_scientific,"Wenn der Exponent größer ist als:");
	InsertGuiPair(display_as_scientific,	"Ergebnis in wissenschaftlicher Schreibweise");
	InsertGuiPair(display_deg_rad_grad,		"Winkelmaß");
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
	InsertGuiPair(convert_dynamic_output,	"Auto präfix");

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
	InsertGuiPair(menu_view_lang_danish,	"&Dänisch");
	InsertGuiPair(menu_view_lang_chinese,	"&Chinesisch");
	InsertGuiPair(menu_view_lang_russian,	"&Russisch");
	InsertGuiPair(menu_view_lang_swedish,   "Sch&wedisch");
	InsertGuiPair(menu_view_lang_italian,   "&Italienisch");
	InsertGuiPair(menu_view_lang_german,    "De&utsch");
	InsertGuiPair(menu_view_close_program,	"&Beenden");
	InsertGuiPair(menu_edit_undo,			"&Rückgängig \tStrg+Z");
	InsertGuiPair(menu_edit_cut,			"&Ausschneiden \tStrg+X");
	InsertGuiPair(menu_edit_copy,			"&Kopieren \tStrg+C");
	InsertGuiPair(menu_edit_paste,			"&Einfügen");					// cutting, copying, pasting the the edit which has a focus
	InsertGuiPair(menu_edit_del,			"&Löschen \tEntf");
	InsertGuiPair(menu_edit_paste_formula,	"&Formel einfügen \tStrg+V");	// pasting directly to input edit
	InsertGuiPair(menu_edit_copy_result,	"E&rgebnis kopieren \tStrg+R");
	InsertGuiPair(menu_edit_copy_both,		"&Beides kopieren \tStrg+B");	// copying in this way: "input edit = output edit"
	InsertGuiPair(menu_edit_select_all,		"Alles a&uswählen \tEsc");
	InsertGuiPair(menu_edit_bracket,		"Eingabe eink&lammern \tStrg+0"); // inserting brackets: (...)
	InsertGuiPair(menu_edit_swap,			"&Vertauschen \tStrg+W");		// swapping input edit for output edit
	InsertGuiPair(menu_help_help,			"&Hilfe");
	InsertGuiPair(menu_help_project_page,	"&Projektseite aufrufen");
	InsertGuiPair(menu_help_check_update,	"Nach &Updates suchen...");
	InsertGuiPair(menu_help_about,			"&Über TTCalc");
	InsertGuiPair(menu_update_available,	"&Neue Version ist verfügbar");
	// this will be a new main menu item (one after 'help') visible only if there is an update

	InsertGuiPair(cant_init_calculations,	"Konnte Modul für die Berechnungen nicht initialisieren");
	InsertGuiPair(message_box_error_caption,"TTCalc");
	InsertGuiPair(cant_create_thread,		"Konnte zweiten Thread für die Berechung nicht erzeugen");
	InsertGuiPair(cant_create_pad,			"Konnte Fenster für Rechenblock nicht erzeugen");
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
		" welches auf http://www.ttmath.org verfügbar ist.\r\n"
		"\r\n"
#ifdef TTCALC_PORTABLE
		"Dies ist die portable Version des Programms TTCalc. In dieser Version "
		"kann nur mit einer einzigen Präzision gerechnet werden (96 bit für die "
		"Mantisse und 32 bit für den Exponenten) ungefähr +/-6.9e+646457021.\r\n"
		"\r\n"
#endif
		"Übersetzungen:\r\n"
		"Spanisch	Alejandro S. Valdezate\r\n"
		"	alesanval@gmail.com\r\n"
		"Dänisch	Rune Bisgaard Vammen\r\n"
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
	InsertGuiPair(about_box_title,			"Über TTCalc");
	InsertGuiPair(about_box_button_close,	"Schließen");
	InsertGuiPair(unknown_error,			"Ein unbekannter Fehler ist aufgetreten");
	InsertGuiPair(cant_find_help,			"Hilfe-Dateien können nicht gefunden werden");
	InsertGuiPair(cant_open_project_page,   "Projektseite kann nicht geöffnet werden");
	InsertGuiPair(update_title,				"Nach neuer Version suchen");
	InsertGuiPair(update_button_next,		"Weiter");
	InsertGuiPair(update_button_finish,		"Ende");
	InsertGuiPair(update_button_cancel,		"Abbrechen");
	InsertGuiPair(update_check_at_startup,	"Beim Start des Programms immer nach Update suchen");  // this text has been changed, need correction!
	InsertGuiPair(update_check_for_info,	"\"Weiter\", um die Update-Suche zu starten...");
	InsertGuiPair(update_is_new_version,	"Version %d.%d.%d ist verfügbar \"Weiter\", um den Download zu starten...");
	InsertGuiPair(update_no_new_version1,	"Es ist keine neuere Version verfügbar.");
	InsertGuiPair(update_no_new_version2,	"Die neueste Version ist bereits installiert.");
	InsertGuiPair(update_download_from,		"Download läuft...");
#ifndef TTCALC_PORTABLE
	InsertGuiPair(update_downloaded_info1,	"Eine neue Setup-Datei wurde heruntergeladen.");
	InsertGuiPair(update_downloaded_info2,	"\"Ende\", um TTCalc zu beenden und die neue version zu installieren.");
#else
	InsertGuiPair(update_downloaded_info1,	"Eine neue Version wurde heruntergeladen.");
	InsertGuiPair(update_downloaded_info2,	"");
#endif
	InsertGuiPair(update_download_error,	"Problem beim Download - bitte später erneut versuchen.");

	InsertGuiPair(pad_title,				"Notizblock");
	InsertGuiPair(pad_menu_file,			"&Datei");
	InsertGuiPair(pad_menu_edit,			"&Bearbeiten");
	InsertGuiPair(pad_menu_file_new,		"&Neu"); // temporarily not used
	InsertGuiPair(pad_menu_file_open,		"Ö&ffnen...");
	InsertGuiPair(pad_menu_file_saveas,		"Speichern &als...");
	InsertGuiPair(pad_menu_file_close,		"&Beenden");
	InsertGuiPair(pad_menu_edit_undo,		"&Rückgängig \tStrg+Z");
	InsertGuiPair(pad_menu_edit_cut,		"&Ausschneiden \tStrg+X");
	InsertGuiPair(pad_menu_edit_copy,		"&Kopieren \tStrg+C");
	InsertGuiPair(pad_menu_edit_paste,		"&Einfügen \tStrg+V");
	InsertGuiPair(pad_menu_edit_del,		"&Löschen \tEntf");
	InsertGuiPair(pad_menu_edit_select_all,	"Alles a&uswählen \tStrg+A");

	InsertGuiPair(cannot_open_file,			"Datei kann nicht geöffnet werden");
	InsertGuiPair(cannot_save_file,			"Datei kann nicht gespeichert werden");
	InsertGuiPair(file_too_long,			"Datei ist zu lang");
	InsertGuiPair(other_error,				"Unbekannter Fehler aufgetreten"); // for other kinds of errors

}

