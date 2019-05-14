// header-start
////////////////////////////////////////////////////////////////////////////////
// \project   memory
//
// \file      Game.hpp
//
// \brief     Game class (MCV modele : observed)
//
// \legal     MIT licence
//
// \author    Abilio AZEVEDO
//
// \version   01/05/2018
//
////////////////////////////////////////////////////////////////////////////////
// header-end

#pragma once

#include <algorithm>    // std::random_shuffle
#include <vector>       // std::vector
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand

#include "card.hpp"
#include "observable.hpp"

class game : public observable<game *>{
	private:
		int width;
        int length;
        int size;
        card *deck;
        int moves = -1;
        int *selectedCards;
        int points =0;
        bool win = false;
        bool badMove = false;
        bool gameMove = false;
        bool bombCard = false;
        bool chanceCard = false;
        int PairsToDo = 0;
        int PairsMade = 0;
	public:
  		game(int w,int l):width(w),length(l){
            deck = new card[w*l];
            size = width*length;
            for(int i = 0; i < width; i++){
                for(int j = 0; j < length; j++){
                    int index = i * (width) + j;
                    //std::cout << "Index = " << index <<std::endl;
                    deck[index].SetValue(1);
                }
            }
        }
        bool getWin(){
            return win;
        }
        bool getBadMove(){
            return badMove;
        }
        bool getGameMove(){
            return gameMove;
        }
        bool getChanceCard(){
            return chanceCard;
        }
        bool getBombCard(){
            return bombCard;
        }
        int getPoints(){
            return points;
        }
        void setPoints(int p){ //GraphView will modify the point at the end of the game
            this->points=p;
        }
        card* getDeck(){
            return deck;
        }
        card& at(int i1, int i2) {
            return deck[i1 * width + i2];
        }

  		virtual ~game() {}

         // random generator function:
        static int myrandom (int i) { return std::rand()%i;}

        void NewGame(){
            points=0;
            moves=-1;
            win=false;
            SetGame();
            ShowGame(true);
            ShowBoard();
            ShowGame(false);
            ShowBoard();
            notifyObservator(this);
        }

  		void SetGame(){
            std::srand ( unsigned ( std::time(0) ) );
            std::vector<int> myvector;

            // set some values:
            switch(size){
                case(9):{
                    PairsToDo=size/2;
                    std::cout<<"PairsToDo = "<<PairsToDo <<std::endl;
                    for (int j=0;j<2;j++){ //Pairs
                        for (int i=1; i<=(PairsToDo); i++){
                            myvector.push_back(i); // 1 2 3 4 5 6 7 8
                        }
                    }
                    myvector.push_back(-2); //Bomb
                    break;
                }
                default:{
                    PairsToDo=size/2-1;
                    std::cout<<"PairsToDo = "<<PairsToDo <<std::endl;
                    for (int j=0;j<2;j++){ //Pairs
                        for (int i=1; i<=(PairsToDo); i++){
                            myvector.push_back(i); // 1 2 3 4 5 6 7 8
                        }
                    }
                    myvector.push_back(-2); //Bomb
                    myvector.push_back(-1); //Chance
                    break;
                }
            }
            // using built-in random generator:
            std::random_shuffle ( myvector.begin(), myvector.end() );

            // using myrandom:
            std::random_shuffle ( myvector.begin(), myvector.end(), myrandom);

            int i=0;
            for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it){
                 deck[i].SetValue(*it);
                 i++;
            }
		} 

        void MoveGame(){
            card temp = deck[0];
            for (int i=0; i<(size - 1); i++)
            {
                deck[i] = deck[i+1] ;
            }
            deck[size - 1] = temp;
            gameMove = false;
        }       

		void ShowBoard(){
            for(int i = 0; i < width; i++){
                for(int j = 0; j < length; j++){
                    if (deck[i * width + j].GetFaceUp()){
                        std::cout << deck[i * width + j].GetValue() << " ";
                    }else{
                        std::cout << "*" << " ";
                    }
                }
                std::cout << std::endl;
            }
        }
        void ShowGame(bool show){
            for(int i = 0; i < size; i++){
                deck[i].SetFaceUp(show);
            }
        }

    	void SelectCard(int cardIndex){
            badMove = false;
            gameMove = false;
            bombCard = false;
            chanceCard = false;
            std::cout<<"cardIndex: "<<cardIndex<<std::endl;
            if (cardIndex>(size-1) || cardIndex<0){
                std::cerr << "Out of bounds!" << std::endl;
            }else{
        		if (this->moves == -1){
        			selectedCards = new int[2];
        		}
        		if (!deck[cardIndex].GetFaceUp()){
        			deck[cardIndex].SetFaceUp(true);
                    if(deck[cardIndex].GetValue()==-1){ //Chance Card
                        points=points+100;
                        chanceCard =true;
                    }else if(deck[cardIndex].GetValue()==-2){ //Bomb Card
                        points=points-100;
                        bombCard =true;
                    }else{
            			this->moves++;
            			std::cout << "Move " << this->moves << std::endl;
            			selectedCards[moves]=cardIndex;
                        if(this->moves>0){
                            if(deck[selectedCards[0]].GetValue()==deck[selectedCards[1]].GetValue()){
                                PairsMade++;
                                this->moves=-1;
                                points=points+100;
                                if(PairsMade>=PairsToDo){
                                    PairsMade=0;
                                    std::cout << "Good Move! Points = "<< points << " Winner!" << std::endl;
                                    win = true;
                                }else{
                                    std::cout << "Good Move! Points = "<< points << std::endl;
                                    if(PairsMade==size/4){
                                        std::cout << "Move Game"<<std::endl;
                                        gameMove = true;
                                        this->ShowBoard();
                                        notifyObservator(this);
                                        MoveGame();
                                    }
                                }
                            }else{
                                std::cout << "Bad Move! No Points!" << std::endl;
                                badMove = true;
                                this->ShowBoard();
                                notifyObservator(this);
                                std::cout << std::endl;
                                deck[selectedCards[0]].SetFaceUp(false);
                                deck[selectedCards[1]].SetFaceUp(false);
                                this->moves=-1;
                            }
                        }
                    }
        		}else{
        			std::cout << "Card already selected!" << std::endl;
        		}
            }
            this->ShowBoard();
            notifyObservator(this);
        }
};