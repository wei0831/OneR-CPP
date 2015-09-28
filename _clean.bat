@echo off

GOTO EndComment
/******************************************************************************/
/*!
 \file   _clean.bat
 \author Jack Chang
 \date   09/08/2014
 \par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
 \brief
    Clear all unneeded files
 */
/******************************************************************************/
:EndComment

del /q *.sdf
del /q *.opensdf
del /q *.sln
del /q *.suo
del /q /a:h *.suo
del /q *.vcxproj*
rd /S /Q ipch
rd /S /Q obj
rd /S /Q bin
