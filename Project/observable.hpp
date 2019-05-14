// header-start
////////////////////////////////////////////////////////////////////////////////
// \project   memory
//
// \file      observable.hpp
//
// \brief     observable class (MCV modele : observable)
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

#include <list>

#include "abstractObservator.hpp"

template <typename T>
class observable {
	private:
		std::list<abstractObservator<T> *> list_obs_;
	public:
		void notifyObservator(T info){
			for (auto obs : list_obs_){
				obs->update(info);
			}
		}

		void addObservator(abstractObservator<T> * obs){
			list_obs_.push_back(obs);
		}
};