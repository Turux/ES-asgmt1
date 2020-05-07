//*******************************************************************************
//
//  alphabet.cpp
//
//  Provides a function to return an interger value from 1 to 13
//  for every different letter of the English alphabet passed as a char
//
//  A=1     G=7         M=13        S=8         Y=2      
//  B=2     H=8         N=13        T=7         Z=1
//  C=3     I=9         O=12        U=6
//  D=4     J=10        P=11        V=5
//  E=5     K=11        Q=10        W=4
//  F=6     L=12        R=9         X=3
//
//  Author: Lorenzo Stilo
//  Date: Jan 2016
//
//*******************************************************************************

#include "alphabet.h"

int alphabet(char letter)
{
    //Returns integer in a range 1-13 for chars in a range A-M
    
    if ('A' <= letter && letter <= 'M')
        return letter-64;
    
    //Returns integer in a range 13-1 for chars ina range N-Z
    
    else if ('N' <= letter && letter <='Z')
        return (90-letter)+1;
        
    //Returns 0 for all the other chars
        
    else 
        return 0;
}