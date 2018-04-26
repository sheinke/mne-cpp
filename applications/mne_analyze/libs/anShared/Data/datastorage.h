//=============================================================================================================
/**
* @file     datastorage.h
* @author   Lars Debor <lars.debor@tu-ilmenau.de>;
*           Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     March, 2018
*
* @section  LICENSE
*
* Copyright (C) 2018, Lars Debor and Matti Hamalainen. All rights reserved.
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
* @brief     DataStorage class declaration.
*
*/

#ifndef ANSHAREDLIB_DATASTORAGE_H
#define ANSHAREDLIB_DATASTORAGE_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "../anshared_global.h"
#include "../Model/abstractmodel.h"
#include "../Model/surfacemodel.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QSharedPointer>
#include <QHash>
#include <QString>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE ANSHAREDLIB
//=============================================================================================================

namespace ANSHAREDLIB {


//*************************************************************************************************************
//=============================================================================================================
// ANSHAREDLIB FORWARD DECLARATIONS
//=============================================================================================================

class AbstractData;

//=============================================================================================================
/**
* Description of what this class is intended to do (in detail).
*
* @brief Brief description of this class.
*/
class ANSHAREDSHARED_EXPORT DataStorage
{

public:
    typedef QSharedPointer<DataStorage> SPtr;            /**< Shared pointer type for DataStorage. */
    typedef QSharedPointer<const DataStorage> ConstSPtr; /**< Const shared pointer type for DataStorage. */

    //=========================================================================================================
    /**
    * Constructs a DataStorage object.
    */
    DataStorage();

    //=========================================================================================================
    /**
    * @brief getObjectsOfType Returns a vector of models that have the specified type
    * @param mtype The type to search for
    * @return Vector of models that have the specified type
    */
    QVector<QSharedPointer<AbstractModel> > getObjectsOfType(AbstractModel::MODEL_TYPE mtype);

    //=========================================================================================================
    /**
    * @brief loadSurface Loads a Surface from the specified filepath (only if the object is not loaded yet)
    * @param path The path of the object to load
    * @return SurfaceModel that contains the specified surface
    */
    QSharedPointer<SurfaceModel> loadSurface(const QString& path);

    //=========================================================================================================
    /**
    * @brief loadSurface        Constructs a filepath out of the passed parameters and calls the single-
    *                           parameter version of loadSurface. Path Construction is copied from Surface::read
    * @param subject_id         Name of subject
    * @param hemi               Which hemisphere to load {0 -> lh, 1 -> rh}
    * @param surf               Name of the surface to load (eg. inflated, orig ...)
    * @param subjects_dir       Subjects directory
    * @return SurfaceModel that contains the specified surface
    */
    QSharedPointer<SurfaceModel> loadSurface(const QString &subject_id, qint32 hemi, const QString &surf, const QString &subjects_dir);

protected:

private:
    QHash<QString, QSharedPointer<AbstractModel> >        m_data;

};


//*************************************************************************************************************
//=============================================================================================================
// INLINE DEFINITIONS
//=============================================================================================================


} // namespace ANSHAREDLIB

#endif // ANSHAREDLIB_DATASTORAGE_H
