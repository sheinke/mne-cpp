//=============================================================================================================
/**
* @file     types.h
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
*           Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>;
*
* @version  1.0
* @date     March, 2018
*
* @section  LICENSE
*
* Copyright (C) 2018, Lars Debor, Simon Heinke and Matti Hamalainen. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that
* the following conditions are met:
*     * Redistributions of source code must retain the above copyright notice, this list of conditions and the
*       following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
*       the following disclaimer in the documentation and/or other materials provided with the distribution.
*     * Neither the name of MNE-CPP authors nor the names of its contributors may be used
*       to endorse or promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*
* @brief    Contains general application specific types
*
*/
#ifndef ANSHARED_TYPES_H
#define ANSHARED_TYPES_H

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include <Eigen/Core>


//*************************************************************************************************************
//=============================================================================================================
// Qt INCLUDES
//=============================================================================================================

#include <QSharedPointer>


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace Eigen;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE MNEANALYZE
//=============================================================================================================

namespace ANSHAREDLIB
{
    //=========================================================================================================
    /**
    * The following directory paths are only imaginary.
    * They should be used for models that are not stored to the file system yet.
    *
    * Convention: Imaginary paths start with '*', end with '/' and all characters are upper case.
    */
    #define ECD_SET_MODEL_DEFAULT_DIR_PATH  QStringLiteral("*ECDSETMODEL/")

    //=========================================================================================================
    /**
    * The MODEL_TYPE enum lists all available model types.
    * Naming convention: NAMESPACE_CLASSNAME_MODEL
    */
    enum MODEL_TYPE
    {
        ANSHAREDLIB_SURFACE_MODEL,
        ANSHAREDLIB_QENTITYLIST_MODEL,
        ANSHAREDLIB_ECDSET_MODEL
    };

    //=========================================================================================================
    /**
    * Public enum for all available Event types.
    */
    enum EVENT_TYPE
    {
        PING,
        EXTENSION_INIT_FINISHED,    //send when all extensions finished initializing
        STATUS_BAR_MSG              //Sending a message to the status bar(part of gui)

    };
} //NAMESPACE

#endif // TYPES_H
