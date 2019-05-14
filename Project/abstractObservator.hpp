// header-start
////////////////////////////////////////////////////////////////////////////////
// \project   memory
//
// \file      abstractObservator.hpp
//
// \brief     abstractObservator class (MCV modele : observable)
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

template <typename T>
class abstractObservator {
	public:
		virtual void update(T info) = 0;
};