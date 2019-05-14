// header-start
////////////////////////////////////////////////////////////////////////////////
// \project   memory
//
// \file      popupUser.hpp
//
// \brief     popup class
//
// \legal     MIT licence
//
// \author    Maéva MANUEL & Abilio AZEVEDO
//
// \version   01/05/2018
//
////////////////////////////////////////////////////////////////////////////////
// header-end

#pragma once

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>

#include <iostream>
#include "player.hpp"
#include "my_sql.hpp"

class popupUser : public Gtk::Window{
	private:

		Gtk::Box box_1;
		Gtk::Box box_2;
		Gtk::Box box_3;
		Gtk::Box box_4;
		Gtk::Entry entry_1;
		Gtk::Entry entry_2;
		Gtk::Button b_OK;
		Gtk::Button b_Cancel;
		Gtk::Label label_1;
		Gtk::Label label_2;
		player p;
		my_sql *server_mysql;
		void button_OK(){
			std::cout<<"ok"<<std::endl;
			time_t now = time(0);
			char* dt = ctime(&now);
			std::string date(dt);
			p=player(entry_1.get_text(),entry_2.get_text(),0,0,0,0,date);
			try {
				if(server_mysql->exist_pseudo(p) && !server_mysql->right_passeword(p)){
					label_2.set_text("Mot de passe incorrect : ");
					entry_2.set_text("");
				}
				else{
					std::cout<<"pseudo : "<<entry_1.get_text()<<std::endl;
					std::cout<<"passeword : "<<entry_2.get_text()<<std::endl;
					server_mysql->fill_database(p);
					hide();
				}
			}catch(sql::SQLException &e) {
				my_sql::exception_catch(e);
			}
		}

		void button_Cancel(){
			std::cout<<"cancel"<<std::endl;
			entry_1.set_text("");
			entry_2.set_text("");
		}

public:
		popupUser(my_sql *sMySQL): server_mysql(sMySQL),
			box_1(Gtk::ORIENTATION_VERTICAL),
			box_2(Gtk::ORIENTATION_HORIZONTAL),
			box_3(Gtk::ORIENTATION_HORIZONTAL),
			box_4(Gtk::ORIENTATION_HORIZONTAL),
	  		b_OK("OK"),
	  		b_Cancel("CANCEL"),
	  		label_1("Pseudo : ", true),
	  		label_2("Passeword : ", true){
	  		set_default_size(300,100);
	  		set_position(Gtk::WIN_POS_CENTER);
		 	set_title("registration");
			entry_1.set_max_length(50);
			entry_2.set_max_length(50);

			box_2.pack_start(label_1);
			box_2.pack_start(entry_1);
			
			box_3.pack_start(label_2);
			box_3.pack_start(entry_2);

			b_OK.set_focus_on_click();
			b_OK.signal_clicked().connect( sigc::mem_fun(*this,&popupUser::button_OK));
			b_Cancel.signal_clicked().connect( sigc::mem_fun(*this,&popupUser::button_Cancel));

			box_4.pack_start(b_OK);
			box_4.pack_start(b_Cancel);
		
			box_1.pack_start(box_2);
			box_1.pack_start(box_3);
			box_1.pack_start(box_4);			

			add(box_1);
			show_all_children();
		}
		player getPlayer(){
			return this->p;
		}
  		virtual ~popupUser(){}
};