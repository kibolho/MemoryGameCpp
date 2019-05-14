// header-start
////////////////////////////////////////////////////////////////////////////////
// \project   memory
//
// \file      graphView.hpp
//
// \brief     graphView class (MCV modele : observer)
//
// \legal     MIT licence
//
// \author    Maéva MANUEL & Abilio AZEVEDO
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

#include "graphView.hpp"
#include "game.hpp"


class control {
	private:
		game *g_;
		graphView *vg_;
		my_sql *server_mysql;

		sigc::connection connectionRevealCards;

		int points = 0;

		void exit(){
			vg_->hide();
		}

		void clickMenu(){
			vg_->pause();
			std::cout << "Menu clicked." << std::endl;
			Gtk::MessageDialog dialog(*vg_,"Menu",false,Gtk::MESSAGE_INFO,Gtk::BUTTONS_NONE);
			//Gtk::MESSAGE_WARNING, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE
		    dialog.add_button("Exit", 1);
		    dialog.add_button("Restart", 2);
		    dialog.add_button("Ranking", 3);
		    dialog.add_button("Infos", 4);

			int result = dialog.run();
			switch(result){
			    case(1):{
			    	exit();
			      	std::cout << "Exit clicked." << std::endl;
			      	break;
			    }
			    case(2):{
			      std::cout << "Restart clicked." << std::endl;
			      newGame();
			      break;
			    }
			    case(3):{
			      	std::cout << "Top Ranking! Clicked" << std::endl;
					std::vector<player> v = server_mysql->top_ranking(3,vg_->sizeGame);
					std::string result = "Top Ranking";
					switch (vg_->sizeGame){
		                case(1):
		                   	result=result+" 3x3!\n";
		                    break;
		                case(2):
		                    result=result+" 4x4!\n";
		                    break;
		                case(3):
		                    result=result+" 5x5!\n";
		                    break;
		                default:
		                	result=result+"!\n";
		                    break;
			        }
					int i =1;
					for(auto p:v) {
						result=result+std::to_string(i)+"-"+p.toString(vg_->sizeGame)+"\n";
						i++;
					}
			      	Gtk::MessageDialog dialog2(*vg_,""+result,false,Gtk::MESSAGE_INFO,Gtk::BUTTONS_NONE);
			      	dialog2.add_button("Ok", 1);
					int result2 = dialog2.run();

					switch(result2){
						case(1):{
							break;
						}
						default:{
			    			std::cout << "Unexpected button clicked." << std::endl;
			    			break;
			    		}
			      	}
			      	vg_->resume();
			      	break;
			    }
			    case(4):{
			    	std::cout << "Infos! Clicked" << std::endl;
					std::string info = "INFOS MEMORY GAME\n";
					info=info+"Objective: Find pairs of cards\n";
					info=info+"Bomb Card: Lose 100 points\n";
					info=info+"Chance Card: Win 100 points\n";
					info=info+"Final Score: POINTS-5*TIMER\n";
					info=info+"Reveal Cards: LOSE 10 seconds\n";
					info=info+"Card Appearance Time: 1 second\n";
					info=info+"Shuffle Cards: Once every game, the cards are mixed\n";
					Gtk::MessageDialog dialog3(*vg_,""+info,false,Gtk::MESSAGE_INFO,Gtk::BUTTONS_CLOSE);
					dialog3.run();
					vg_->resume();
			    	break;
			    }
			    default:{
			    	vg_->resume();
			    	std::cout << "Unexpected button clicked." << std::endl;
			    	break;
			    }
		  	}
		}

		void clickCard(int cardPos){
			if(!g_->getWin()){
				g_->SelectCard(cardPos);
			}
		}
		void clickRevealCards(){
			int i =0;
			vg_->timming=vg_->timming+10.0;
			for(int j = 0; j<(vg_->widthGame);j++){
				for(int k = 0; k<(vg_->lengthGame);k++){
					std::string imagePath;
					if(g_->getDeck()[i].GetValue()==-1){
						imagePath = "imagesResized/chance.jpg";
					}else if (g_->getDeck()[i].GetValue()==-2){
						imagePath = "imagesResized/bomb.jpg";
					}else{
						imagePath = "imagesResized/b"+std::to_string(g_->getDeck()[i].GetValue()+1)+std::string(".jpg");
					}
					vg_->img[i].set(imagePath);
					i++;
				}
			}
			connectionRevealCards=Glib::signal_timeout().connect( sigc::mem_fun(*this, &control::timer_revealCards), 1000);
		}
		bool timer_revealCards(){
			connectionRevealCards.disconnect();
			vg_->updateDeck();
		}
		void newGame(){
			std::cout<<"Game Started"<<std::endl;
			//vg_->connectionBadMove=Glib::signal_timeout().connect( sigc::mem_fun(*this, &control::timer_badMove), 500);
			//vg_->connectionTimming=Glib::signal_timeout().connect( sigc::mem_fun(*this, &control::timer_timming), 100);
			g_->NewGame();
			vg_->timming=0;
			points=0;
			for(int i=0;i<(sizeGame*sizeGame);i++){
				vg_->img[i].set("imagesResized/dos.jpg");
			}
			vg_->bPoints_.set_label("Points: ");
			vg_->resume(); // Resume Threads
		}
		/*
		bool timer_badMove()
		{
			if (vg_->badMove){
				vg_->updateDeck();
			  	vg_->badMove = false;
			}
			//std::cout<<"Thread"<<std::endl;
			return true;
		}
		bool timer_timming(){
			timming++;
			vg_->bTiming_.set_label("Timer: "+std::to_string(timming));
			return true;
		}
		*/
	public:
		int sizeGame;
		control(game *g,graphView *vg,int s,my_sql *sMySQL):g_(g), vg_(vg),sizeGame(s),server_mysql(sMySQL){
			int i = 0;
			for(int j=0;j<sizeGame;j++){
				for(int k=0;k<sizeGame;k++){
					//std::cout<<"Debug Control"<< i<<std::endl;
					vg_->bImg[i].set_image(vg_->img[i]); 
					//std::cout<<"Debug Connect"<< i<<std::endl;
					vg_->bImg[i].signal_clicked().connect(sigc::bind<int>( sigc::mem_fun(*this, &control::clickCard), i));
					//void attach(Widget& child, int left, int top, int width, int height);
					//std::cout<<"Debug Attach"<< i<<std::endl;
					vg_->m_grid.attach(vg_->bImg[i],k,j,1,1);
					i++;
				}
			}
			//std::cout<<"Debug Attach End"<< i<<std::endl;
			//qd on clique sur menu, il y a un sous-menu
			vg_->bMenu_.signal_clicked().connect(sigc::mem_fun(*this, &control::clickMenu));

			vg_->box2_.pack_start(vg_->m_grid,true,true,50);
			
			
			vg_->box3_.pack_start(vg_->box1_);
	        vg_->box3_.pack_start(vg_->box2_);
			vg_->box3_.pack_start(vg_->bRevealCards_);
			vg_->bRevealCards_.set_label("Reveal Cards (Lose 10 seconds)");
			vg_->bRevealCards_.signal_clicked().connect(sigc::mem_fun(*this, &control::clickRevealCards));
			vg_->add(vg_->box3_);
			vg_->show_all_children();

			srand(time(NULL));
			newGame();
			
		}
};