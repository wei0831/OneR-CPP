@echo off

GOTO EndComment
/******************************************************************************/
/*!
 \file   _build_vs2015.bat
 \author Jack Chang
 \date   09/19/2015
 \par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
 \brief
    Generate Visual Studio 2015 Project
 */
/******************************************************************************/
:EndComment

cd tools
.\premake5 vs2015