// header-start
////////////////////////////////////////////////////////////////////////////////
// \project   memory
//
// \file      player.hpp
//
// \brief     player class
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

#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>

class player {
    private :
        std::string pseudo;
        std::string passeword;
        double lastScore;
        double Score1;
        double Score2;
        double Score3;
        std::string date;
       
    public :

        player (std::string name, std::string p, double ls, double s1,double s2,double s3,std::string d) : pseudo(name), passeword(p), lastScore(ls),Score1(s1),Score2(s2),Score3(s3), date(d) {}/*constructeur*/
        player () {}/*constructeur par défaut*/

        std::string get_pseudo() const{/*accesseur*/
            return pseudo;
        }

        std::string get_passeword() const{
            return passeword;
        }
        
        double get_lastScore() const { 
          return lastScore;
        }
        double get_Score1() const { 
          return Score1;
        }
        double get_Score2() const { 
          return Score2;
        }        
        double get_Score3() const { 
          return Score3;
        }
        std::string get_date() const { 
          return date; 
        }       

        void set_pseudo(std::string st){/*mutateur*/
            pseudo=st;
        }
        
        void set_lastScore(double sc){ 
            lastScore=sc;
        }
        void set_Score1(double sc){ 
            Score1=sc;
        }
        void set_Score2(double sc){ 
            Score2=sc;
        }
        void set_Score3(double sc){ 
            Score3=sc;
        }
        void set_date(std::string st){ 
            date=st; 
        }

        void set_passeword(std::string st){ 
            passeword=st; 
        }
        std::string toString(int scoreType) const{
            std::ostringstream s;
            switch (scoreType){
                case(1):
                    s << ""<<this->get_pseudo()<<" , Score: "<<this->get_Score1();
                    break;
                case(2):
                    s << ""<<this->get_pseudo()<<" , Score: "<<this->get_Score2();
                    break;
                case(3):
                    s << ""<<this->get_pseudo()<<" , Score: "<<this->get_Score3();
                    break;
                default:
                    break;
            }
            return s.str();
        }
        friend std::ostream& operator<<(std::ostream &os, const player &p){
        /*surcharge de l'opérateur << permettant de visualiser les données du player*/
            os<<"pseudo : "<<p.get_pseudo()<<" , lastScore : "<<p.get_lastScore();
            return os;
        }
};
