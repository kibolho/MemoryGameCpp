// header-start
////////////////////////////////////////////////////////////////////////////////
// \project   memory
//
// \file      main.cpp
//
// \brief     main : mysql server and GUI
//
// \legal     MIT licence
//
// \author    Maéva MANUEL & Abilio AZEVEDO
//
// \version   19/04/2018
//
////////////////////////////////////////////////////////////////////////////////
// header-end

/*
abilio dir: cd ~/Desktop/Parallels\ Shared\ Folders/Google\ Drive/Projet\ Programation/Ensemble3
			cd ~/Home/Google\ Drive/Projet\ Programation/Ensemble3
			cd media/psf/Google\ Drive/Projet\ Programation/Ensemble3
maeva dir : cd ~/Documents/ELEC4/C++/S8/Projet/Ensemble3

mysql -u root -p
SHOW DATABASES
USE Test_BD
SHOW FULL TABLES
*/

#include <iostream>
#include <ctime>
#include <string>
#include <gtkmm.h>

#include "graphView.hpp"
#include "game.hpp"
#include "control.hpp"
#include "popupUser.hpp"
#include "popupSize.hpp"

int main(int argc, char *argv[]){
	my_sql server_mysql = my_sql::unique_instance();
	auto appPopupUser = Gtk::Application::create(argc, argv);//pointeur
	popupUser *pUser = new popupUser(&server_mysql);
	appPopupUser->run(*pUser);

	auto appPopupSize = Gtk::Application::create(argc, argv);//pointeur
	popupSize *pSize = new popupSize(); 
	appPopupSize->run(*pSize);
	int sizeGame = pSize->getSize();
	std::cout << "Size Game: " << sizeGame << std::endl;

	auto appGame = Gtk::Application::create(argc, argv);
	game *g=new game(sizeGame,sizeGame);
	std::cout<<"Player PopupUSER = "<<pUser->getPlayer()<<std::endl;
	graphView *vg = new graphView(sizeGame,sizeGame,pUser->getPlayer(),&server_mysql);
	g->addObservator(vg);
	control *c=new control(g,vg,sizeGame,&server_mysql);
	appGame->run(*vg);
	//double score = g->getPoints();
	server_mysql.close_connexion();
	return EXIT_SUCCESS;
}