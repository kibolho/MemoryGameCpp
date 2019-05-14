// header-start
////////////////////////////////////////////////////////////////////////////////
// \project   memory
//
// \file      my_sql.hpp
//
// \brief     my_sql class : server
//
// \legal     MIT licence
//
// \author    Maéva MANUEL
//
// \version   01/05/2018
//
////////////////////////////////////////////////////////////////////////////////
// header-end

#pragma once

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <boost/algorithm/string.hpp>
#include "player.hpp"

class my_sql {
	private:
		sql::mysql::MySQL_Driver *driver;
		sql::Connection *conn;
		sql::Statement *stmt;
		sql::ResultSet *res;
		sql::PreparedStatement *prep_stmt;

		my_sql() {/*constructeur privé*/
			driver = sql::mysql::get_mysql_driver_instance ();
			//conn = driver->connect("localhost:3306", "root", "mdp");
			conn = driver->connect("localhost:3306", "root", "root"); // abilio
			stmt=conn->createStatement();

			stmt->execute("CREATE DATABASE IF NOT EXISTS Memory ");
			stmt->execute("USE Memory");
			/*création et utlisation de la base de données Memory*/
			stmt->execute("CREATE TABLE IF NOT EXISTS players( id_ref INT PRIMARY KEY NOT NULL AUTO_INCREMENT,\
			pseudo VARCHAR (100), passeword VARCHAR (100), highScore1 DOUBLE, highScore2 DOUBLE,highScore3 DOUBLE,lastScore DOUBLE, date VARCHAR (100))");
			/*création de la table "players"
			**numéro du joueur, pseudo, mot de passe, score, date de la partie*/

			prep_stmt = conn->prepareStatement( " INSERT \
			INTO players (pseudo , passeword, lastScore , date) \
			VALUES (? , ? , ? , ?) " );
			

			player pTEST=player("abilio","master",0,0,0,0,"Sun Apr  8 15:07:41 2018");
			fill_database(pTEST);
			std::cout<<"My_sql Init DONE"<<std::endl;
		}
				
	public:

		static my_sql& unique_instance() {
			/*retourne une instance unique de la connexion au serveur
			**construit le modèle du singleton*/
			static my_sql server_mysql;
			return server_mysql;
		}

		bool exist_pseudo(player p){
			/*fonction qui vérifie dans la base de donnée si le pseudo du player p existe déjà*/
			res = stmt->executeQuery("SELECT * FROM players ORDER BY pseudo ASC");
			/*liste des joueurs ordonnées par ordre lexical croissant afin de réduire la complexité de lalgorithme*/
			
			while(res->next()){
				if(res->getString("pseudo")==boost::to_upper_copy(p.get_pseudo())){
					std::cout<<"Exist pseudo = "<< p.get_pseudo()<<std::endl;
					return true;
				}
			}
			std::cout<<"Doesn't exist pseudo = "<< p.get_pseudo()<<std::endl;
			return false;
		}
		void fill_database(player p){/*ajouter un joueur à la base de données*/
			if(exist_pseudo(p)==false){/*le joueur n'est pas déjà enregistré*/
				prep_stmt->setString (1, boost::to_upper_copy(p.get_pseudo()));
				prep_stmt->setString (2, p.get_passeword());
				prep_stmt->setDouble (3, p.get_lastScore());
				prep_stmt->setString (4, p.get_date());
				prep_stmt->execute();
				/*incrémente le nombre de joueurs différents enregistrés*/
			}
		}
	 
	 	bool right_passeword(player p){
		/*fonction qui vérifie que le passeword du player p est correct*/
			res = stmt->executeQuery("SELECT * FROM players ORDER BY pseudo ASC");
			/*liste des joueurs ordonnées par ordre lexical croissant afin de réduire la complexité de lalgorithme*/
			while(res->next()){
				if(res->getString("pseudo")==boost::to_upper_copy(p.get_pseudo()) && res->getString("passeword")==p.get_passeword()){
					return true;
				}
				else if(res->getString("pseudo")==boost::to_upper_copy(p.get_pseudo()) && res->getString("passeword")!=p.get_passeword()){
					return false;
				}
			}
			return false;
		}

		void update_data(player p,int size){
			/*fonction qui met à jour le score et la date associés à une nouvelle partie*/
			std::cout<<"Update pseudo = "<< p.get_pseudo() << " lastScore = "<<std::to_string(p.get_lastScore())<<std::endl;
			stmt->executeUpdate("UPDATE players SET lastScore='"+std::to_string(p.get_lastScore())+"' WHERE pseudo='" + p.get_pseudo()+"'");
			/*met à jour le score du player p
			**utilisation des '' entre le nom des variables converties en string préalablement*/
			stmt->executeUpdate("UPDATE players SET date='"+p.get_date()+"' WHERE pseudo='"+p.get_pseudo()+"'");
			/*met à jour la date de la partie*/
			std::cout<<"Updated"<<std::endl;
			double highScore = 0;
			switch(size){
				case(1):{
					res = stmt->executeQuery("SELECT * FROM players WHERE pseudo='"+p.get_pseudo()+"' AND highScore1 IS NOT NULL;");
					if (res->next()){
						highScore=res->getDouble("highScore1");
					}
					if(p.get_lastScore()>highScore)
						stmt->executeUpdate("UPDATE players SET highScore1='"+std::to_string(p.get_lastScore())+"' WHERE pseudo='" + p.get_pseudo()+"'");
					break;
				}
				case(2):{
					res = stmt->executeQuery("SELECT * FROM players WHERE pseudo='"+p.get_pseudo()+"' AND highScore2 IS NOT NULL;");
					if (res->next()){
						highScore=res->getDouble("highScore2");
					}
					if(p.get_lastScore()>highScore)
						stmt->executeUpdate("UPDATE players SET highScore2='"+std::to_string(p.get_lastScore())+"' WHERE pseudo='" + p.get_pseudo()+"'");
					break;
				}
				case(3):{
					res = stmt->executeQuery("SELECT * FROM players WHERE pseudo='"+p.get_pseudo()+"' AND highScore3 IS NOT NULL;");
					if (res->next()){
						highScore=res->getDouble("highScore3");
					}
					if(p.get_lastScore()>highScore)
						stmt->executeUpdate("UPDATE players SET highScore3='"+std::to_string(p.get_lastScore())+"' WHERE pseudo='" + p.get_pseudo()+"'");
					break;
				}
				default:
				break;
			}
		}

		int ranking(player p,int gameSize){
			switch(gameSize){
				case(1):
					std::cout<<"ranking init1"<<std::endl;
					stmt->execute("SET @row_num := 0");
					std::cout<<"ranking row1"<<std::endl;
					res = stmt->executeQuery("SELECT * FROM (SELECT @row_num := @row_num + 1 as id_ref,pseudo,highScore1 FROM Memory.players ORDER BY highScore1 DESC) AS players WHERE pseudo='"+ p.get_pseudo()+"'");
					break;
				case(2):
					stmt->execute("SET @row_num:=0");
					res = stmt->executeQuery("SELECT * FROM (SELECT @row_num := @row_num + 1 as id_ref,pseudo,highScore2 FROM Memory.players ORDER BY highScore2 DESC) AS players WHERE pseudo='"+ p.get_pseudo()+"'");
					break;
				case(3):
					stmt->execute("SET @row_num:=0");
					res = stmt->executeQuery("SELECT * FROM (SELECT @row_num := @row_num + 1 as id_ref,pseudo,highScore3 FROM Memory.players ORDER BY highScore3 DESC) AS players WHERE pseudo='"+ p.get_pseudo()+"'");
					break;
				default:
					stmt->execute("SET @row_num:=0");
					res = stmt->executeQuery("SELECT * FROM (SELECT @row_num := @row_num + 1 as id_ref,pseudo,lastScore FROM Memory.players ORDER BY lastScore DESC) AS players WHERE pseudo='"+ p.get_pseudo()+"'");
					break;	
			}
			std::cout<<"ranking query OK"<<std::endl;
			if(res->next()){
				return res->getInt("id_ref");
			}else{
				return 0;
			}
		}
		//SET @row_num:=0; SELECT * FROM (SELECT @row_num := @row_num + 1 as `id_ref`,`pseudo`,`highScore1` FROM players ORDER BY highScore1 DESC)) AS `players` WHERE `pseudo` = 'b'

		std::vector<player> top_ranking(int nb,int gameSize){
			/*fonction qui renvoie le classement des 15 premiers joueurs sous forme de vecteur de player*/
			switch(gameSize){
				case(1):
					res = stmt->executeQuery(" SELECT * FROM players ORDER BY highScore1 DESC ");
					break;
				case(2):
					res = stmt->executeQuery(" SELECT * FROM players ORDER BY highScore2 DESC ");
					break;
				case(3):
					res = stmt->executeQuery(" SELECT * FROM players ORDER BY highScore3 DESC ");
					break;
				default:
					res = stmt->executeQuery(" SELECT * FROM players ORDER BY lastScore DESC ");
					break;	
			}
			
			/*classement de la base de donnée par score décroissant*/
			int i=1;
			std::vector<player> v;
			while (res->next() && i<=nb){

				player p;
				p.set_pseudo(res->getString("pseudo"));
				p.set_lastScore(res->getDouble("lastScore"));
				p.set_Score1(res->getDouble("highScore1"));
				p.set_Score2(res->getDouble("highScore2"));
				p.set_Score3(res->getDouble("highScore3"));
				p.set_date(res->getString("date"));
				p.set_passeword("");

				/*on crée un player avec les membres pseudo, score et date*/
				v.push_back(p);
				/*on ajoute le player au vecteur*/
				i++;
			}
			return v;
		}
		 
		void close_connexion() {/*achever la connection au serveur mysql*/
			delete conn;
		}
		
		static void exception_catch(sql::SQLException &e) {/*affichage des messages d'erreur liés à mysql*/
			std :: cerr << " # ERR : SQLException in "<< __FILE__ ;
			std :: cerr << " ( " << __FUNCTION__ << " ) on line " << __LINE__ << std :: endl ;
			std :: cerr << " # ERR : " << e . what ();
			std :: cerr << " ( MySQL error code : " << e . getErrorCode ();
			std :: cerr << " , SQLState : " << e . getSQLState () << " ) " << std :: endl ;
		}
};