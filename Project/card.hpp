// header-start
////////////////////////////////////////////////////////////////////////////////
// \project   memory
//
// \file      card.hpp
//
// \brief     card class
//
// \legal     MIT licence
//
// \author    Abilio AZEVEDO
//
// \version   19/04/2018
//
////////////////////////////////////////////////////////////////////////////////
// header-end

#pragma once

class card{
	private:
		int xCor;
  		int yCor;
  		int value;
  		bool faceUp;

	public:
		
		card(){ // Constructor
   		 	faceUp = false;
		}

		void SetValue(int v){
  			this->value = v;
		}

		int GetValue(){
  			return this->value;
		}

		void SetCor(int x,int y){
  			this->xCor = x;
  			this->yCor = y;
		}

		int* GetCor(int x,int y){
			int* cor;
			cor[0]=this->xCor;
			cor[1]=this->yCor;
  			return cor;
		}

		void SetFaceUp(bool state){
  			this->faceUp = state;
		}

		bool GetFaceUp(){
  			return this->faceUp;
		}
		
		virtual ~card() {}
};