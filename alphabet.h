//*******************************************************************************
//
//  alphabet.h
//
//  Provides a function to return an interger value from 1 to 13
//  for every different letter of the English alphabet
//
//  A=1     G=7         M=13        S=8         Y=2      
//  B=2     H=8         N=13        T=7         Z=1
//  C=3     I=9         O=12        U=6
//  D=4     J=10        P=11        V=5
//  E=5     K=11        Q=10        W=4
//  F=6     L=12        R=9         X=3
//
//  N.B. It only works with letters expressed in the form of a char
//  It is case sensitive. It only works with capital letters (UPPERCASE)
//
//  Author: Lorenzo Stilo
//  Date: Jan 2016
//
//*******************************************************************************

#ifndef __ALPHABET_H__
#define __ALPHABET_H__

extern int alphabet(char letter);

#endif // __ALPHABET_H__