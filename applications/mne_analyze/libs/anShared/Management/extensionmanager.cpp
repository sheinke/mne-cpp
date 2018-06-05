//=============================================================================================================
/**
* @file     extensionmanager.cpp
* @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
*           Lars Debor <lars.debor@tu-ilmenau.de>;
*           Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     February, 2017
*
* @section  LICENSE
*
Copyright (C) 2017, Christoph Dinh, Lars Debor, Simon Heinke and Matti Hamalainen. All rights reserved.
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
* @brief    Contains the implementation of the ExtensionManager class.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "extensionmanager.h"
#include "../Interfaces/IExtension.h"
#include "communicator.h"
#include <iostream>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QDir>
#include <QDebug>


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace ANSHAREDLIB;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

ExtensionManager::ExtensionManager(QObject *parent)
: QPluginLoader(parent)
{

}


//*************************************************************************************************************

ExtensionManager::~ExtensionManager()
{
    for(IExtension* extension : m_qVecExtensions)
    {
        delete extension;
    }
}


//*************************************************************************************************************

void ExtensionManager::loadExtension(const QString& dir)
{
    QDir extensionsDir(dir);

    foreach(const QString &file, extensionsDir.entryList(QDir::Files))
    {
        fprintf(stderr,"Loading Extension %s... ",file.toUtf8().constData());

        this->setFileName(extensionsDir.absoluteFilePath(file));
        QObject *pExtension = this->instance();

        // IExtension
        if(pExtension) {
            fprintf(stderr,"Extension %s loaded.\n",file.toUtf8().constData());
            m_qVecExtensions.push_back(qobject_cast<IExtension*>(pExtension));
        }
        else {
            fprintf(stderr,"Extension %s could not be instantiated!\n",file.toUtf8().constData());
        }
    }
}


//*************************************************************************************************************

void ExtensionManager::initExtensions(QSharedPointer<AnalyzeSettings> settings, QSharedPointer<AnalyzeData> data)
{
    for(IExtension* extension : m_qVecExtensions)
    {
        extension->setGlobalSettings(settings);
        extension->setGlobalData(data);
        extension->init();
    }
    // tell everyone that INIT-phase is finished
    // @TODO consider communicator as class member, since this is kinda nasty
    Communicator con;
    con.publishEvent(EVENT_TYPE::EXTENSION_INIT_FINISHED);
}

//*************************************************************************************************************

int ExtensionManager::findByName(const QString& name)
{
    QVector<IExtension*>::const_iterator it = m_qVecExtensions.begin();
    for(int i = 0; it != m_qVecExtensions.end(); ++i, ++it)
        if((*it)->getName() == name)
            return i;

    return -1;
}


//*************************************************************************************************************

void ExtensionManager::shutdown()
{
    for(IExtension* extension : m_qVecExtensions)
    {
        extension->unload();
    }
}
