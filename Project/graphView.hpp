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
// \author    Abilio AZEVEDO & Shuo LI
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
#include <gtkmm/table.h>
#include <gtkmm/image.h>
#include <gtkmm/messagedialog.h>

#include <iostream>
#include <string>

#include "game.hpp"
#include "abstractObservator.hpp"

#include "player.hpp"
#include "my_sql.hpp"

class graphView : public abstractObservator<game *>, public Gtk::Window{
	public:
		player pGraphView;
		Gtk::Box box1_;
		Gtk::Box box2_;
	    Gtk::Box box3_;

	    int widthGame;
		int lengthGame;
		int sizeGame = 1;
		int Card0;
		int Card1;
		bool badMove=false;
		game *gameGraph;

		sigc::connection connectionBadMove;
		sigc::connection connectionTimming;
		double timming = 0.0;

		Gtk::Button bMenu_;
	    Gtk::Button bPoints_;
        Gtk::Button bTiming_;
        Gtk::Button bRevealCards_;
	
		Gtk::Grid m_grid;
		Gtk::Image *img;
		Gtk::Button *bImg;

		my_sql *server_mysql;

		graphView(int w,int l,player p,my_sql *sMySQL):widthGame(w),lengthGame(l),
					box1_(Gtk::ORIENTATION_HORIZONTAL),
	                box2_(Gtk::ORIENTATION_HORIZONTAL),
					box3_(Gtk::ORIENTATION_VERTICAL),
	                bMenu_("Menu"),
	                bPoints_("Points: 0"),
                    bTiming_("Timer: 0.0"),
                    pGraphView(p),
                    server_mysql(sMySQL)
			{
			set_default_size(400,400);

			set_position(Gtk::WIN_POS_CENTER);
			std::cout<<"Graph Construction "<<widthGame << " "<< lengthGame<< std::endl;
			img=new Gtk::Image[widthGame*lengthGame];
			bImg=new Gtk::Button[widthGame*lengthGame];

			box1_.pack_start(bMenu_);
			box1_.pack_start(bPoints_);
			box1_.pack_start(bTiming_);
			switch(widthGame*lengthGame){
				case(9):
					sizeGame = 1;
					break;
				case(16):
					sizeGame = 2;
					break;
				case(25):
					sizeGame = 3;
					break;
				default:
					break;
			}
		}

		bool timer_badMove()
		{
			if (badMove){
				updateDeck();
			  	badMove = false;
			}
			connectionBadMove.disconnect();
			//std::cout<<"Thread"<<std::endl;
			return true;
		}
		bool timer_timming(){
			timming=timming+0.1;
			char strT[5]; //Change the size to meet your requirement
			sprintf(strT, "%.1lf", timming);
			std::string str(strT);
			bTiming_.set_label("Timer: "+str);
			return true;
		}

		void pause(){
			if(!gameGraph->getWin()){
				connectionTimming.disconnect();
				connectionBadMove.disconnect();
			}
		}
		void resume(){
			if(!gameGraph->getWin()){
				connectionTimming=Glib::signal_timeout().connect( sigc::mem_fun(*this, &graphView::timer_timming), 100);
			}
		}
		void updateDeck(){
			int i =0;
			for(int j = 0; j<(widthGame);j++){
				for(int k = 0; k<(lengthGame);k++){
					std::string imagePath;
					if(gameGraph->getDeck()[i].GetFaceUp()){
						if(gameGraph->getDeck()[i].GetValue()==-1){
							imagePath = "imagesResized/chance.jpg";
						}else if (gameGraph->getDeck()[i].GetValue()==-2){
							imagePath = "imagesResized/bomb.jpg";
						}else{
							imagePath = "imagesResized/b"+std::to_string(gameGraph->getDeck()[i].GetValue()+1)+std::string(".jpg");
						}
					}else{
						imagePath = "imagesResized/dos.jpg";
					}
					img[i].set(imagePath);
					i++;
				}
			}
		}
		void update(game *g) override {
			gameGraph = g;
			//std::cout << "Update" << std::endl;
			if (!badMove){
				updateDeck();
				bPoints_.set_label("Points: "+std::to_string(g->getPoints()));
				if(gameGraph->getBadMove()){
					badMove = true;
					connectionBadMove=Glib::signal_timeout().connect( sigc::mem_fun(*this, &graphView::timer_badMove), 500);
				}
				if(gameGraph->getWin()){
					connectionTimming.disconnect();
					connectionBadMove.disconnect();
					int finalPoints = g->getPoints()-5*timming;
					if (finalPoints<0){
						finalPoints=0;
					}
					g->setPoints(finalPoints);

					pGraphView.set_lastScore(g->getPoints());
			    	std::cout<<pGraphView<<std::endl;
			    	int ranking_pos=0;
			      	try {
		      			std::cout << "My_SQL!" << std::endl;
						std::cout << "Player Updated!" << std::endl;
						std::cout <<"Pseudo "+pGraphView.get_pseudo() << std::endl;
						std::cout <<"LastScore "+std::to_string(pGraphView.get_lastScore()) << std::endl;
						std::cout <<"Date "+pGraphView.get_date() << std::endl;
						server_mysql->update_data(pGraphView,sizeGame);
						std::cout << "Top Ranking!" << std::endl;
						std::vector<player> v = server_mysql->top_ranking(3,sizeGame);
						for_each(v.begin(), v.end(), [] (player p) {
							std::cout<<p<<std::endl;});
						std::cout << "Getting Ranking!" << std::endl;
						ranking_pos=server_mysql->ranking(pGraphView,sizeGame);
						std::cout<<"ranking_pos= "<<std::to_string(ranking_pos)<<std::endl;
					}catch(sql::SQLException &e) {
						my_sql::exception_catch(e);
					}
					Gtk::MessageDialog dialog(*this,"YOU WON!\nFinal Score: "+std::to_string(g->getPoints())+"\nRanking Position = "+std::to_string(ranking_pos),false,Gtk::MESSAGE_INFO,Gtk::BUTTONS_NONE);
					//Gtk::MESSAGE_WARNING, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE
				    dialog.add_button("Ok", 1);
					int result = dialog.run();

					switch(result){
					    case(1):{
					    	//Ok
					      	break;
					    }
					    default:{
					    	resume();
					     	std::cout << "Unexpected button clicked." << std::endl;
					      	break;
						}
	     			}
	     		}
				if (gameGraph->getGameMove()){
					pause();
					Gtk::MessageDialog dialog(*this,"Shuffle Cards!",false,Gtk::MESSAGE_INFO,Gtk::BUTTONS_NONE);
					//Gtk::MESSAGE_WARNING, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE
				    dialog.add_button("Ok", 1);
				    
					int result = dialog.run();
					switch(result){
					    case(1):{
					      	std::cout << "Resume." << std::endl;
					      	resume();
					      	break;
					    }
					    default:{
					    	resume();
					     	std::cout << "Unexpected button clicked." << std::endl;
					      	break;
					    }
				  	}
				}
			}
		}
		virtual ~graphView() {}
};