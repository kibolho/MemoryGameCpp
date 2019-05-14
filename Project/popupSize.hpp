// header-start
////////////////////////////////////////////////////////////////////////////////
// \project   memory
//
// \file      popupSize.hpp
//
// \brief     popup class
//
// \legal     MIT licence
//
// \author    Maéva MANUEL e Abilio AZEVEDO
//
// \version   19/04/2018
//
////////////////////////////////////////////////////////////////////////////////
// header-end

#pragma once

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/radiobutton.h>

#include <iostream>

class popupSize : public Gtk::Window{
	private:
		Gtk::RadioButton button1, button2, button3;
		Gtk::Box box_1;
		Gtk::Box box_2;
		Gtk::Button b_OK;

		void button_OK(){
			hide();
		}

	public:

		popupSize(): box_1(Gtk::ORIENTATION_HORIZONTAL),
			box_2(Gtk::ORIENTATION_VERTICAL),
	  		b_OK("OK"),
	  		button1("3x3"),
	  		button2("4x4"),
	  		button3("5x5"){
	  		set_default_size(300,100);
	  		set_position(Gtk::WIN_POS_CENTER);
		 	set_title("Size of the game");

			// Put radio buttons 2 and 3 in the same group as 1:
			button2.join_group(button1);
			button3.join_group(button1);

			// Put the radio buttons in Box1:
			box_1.pack_start(button1);
			box_1.pack_start(button2);
			box_1.pack_start(button3);

			// Set the second button active
			button2.set_active();

			b_OK.signal_clicked().connect( sigc::mem_fun(*this,&popupSize::button_OK));		

			box_2.pack_start(box_1);
			// Put Close button in Box2:
			box_2.pack_start(b_OK);

			add(box_2);
			show_all_children();
		}

  		virtual ~popupSize(){}

  		int getSize(){
  			if(button1.get_active()){
				std::cout<<"size 3x3"<<std::endl;
				return 3;
			}else if(button2.get_active()){
				std::cout<<"size 4x4"<<std::endl;
				return 4;
			}else if(button3.get_active()){
				std::cout<<"size 5x4"<<std::endl;
				return 5;
			}
  		}

};