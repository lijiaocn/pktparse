/*
 * =====================================================================================
 *
 *       Filename:  arch.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/22/2014 08:57:07 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __ARCH_H__
#define __ARCH_H__

#define ARCH_BIG_ENDIAN (0)
#define ARCH_LITTLE_ENDIAN (1)

#define ARCH_WORD_SIZE 64

//typedef signed char char;
typedef unsigned char u_char;

typedef signed char int8;
typedef unsigned char u_int8;

typedef signed short int int16;
typedef unsigned short int u_int16;

typedef signed int int32;
typedef unsigned int u_int32;

#if ARCH_WORD_SIZE == 32
typedef signed long long int int64;
typedef unsigned long long int u_int64;
#endif

#if ARCH_WORD_SIZE == 64
typedef signed long int int64;
typedef unsigned long int u_int64;
#endif


#endif
