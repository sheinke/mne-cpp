//=============================================================================================================
/**
* @file     mainviewer.cpp
* @author   Simon Heinke <simon.heinke@tu-ilmenau.de>;
*           Lars Debor <lars.debor@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     May, 2018
*
* @section  LICENSE
*
* Copyright (C) 2018, Simon Heinke, Lars Debor and Matti Hamalainen. All rights reserved.
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
* @brief    MainViewer class definition.
*
*/


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "mainviewer.h"


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace MAINVIEWEREXTENSION;
using namespace ANSHAREDLIB;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE GLOBAL METHODS
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

MainViewer::MainViewer()
{

}


//*************************************************************************************************************

MainViewer::~MainViewer()
{

}


//*************************************************************************************************************

QSharedPointer<IExtension> MainViewer::clone() const
{
    QSharedPointer<MainViewer> pMainViewerClone = QSharedPointer<MainViewer>::create();
    return pMainViewerClone;
}


//*************************************************************************************************************

void MainViewer::init()
{

}


//*************************************************************************************************************

void MainViewer::unload()
{

}


//*************************************************************************************************************

QString MainViewer::getName() const
{
    return "MainViewer";
}


//*************************************************************************************************************

QMenu *MainViewer::getMenu()
{
    return Q_NULLPTR;
}


//*************************************************************************************************************

QDockWidget *MainViewer::getControl()
{
//    if(!m_pControl) {
//        m_pControl = new QDockWidget(tr("Surfer Control"));
//        m_pControl->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
//        m_pControl->setMinimumWidth(180);
//    }

//    return m_pControl;
    return nullptr;
}


//*************************************************************************************************************

QWidget *MainViewer::getView()
{
    /*
    if(!m_pView) {
        //
        //Pial surface
        //
        m_pView = new View3DSurfer();
        m_pView->setWindowTitle(QStringLiteral("Pial surface"));
        m_pView->setModel(m_pSurfaceModel);
    }

    return m_pView;
    */
    return nullptr;
}


//*************************************************************************************************************

void MainViewer::handleEvent(QSharedPointer<Event> e)
{

}


//*************************************************************************************************************

QVector<EVENT_TYPE> MainViewer::getEventSubscriptions(void) const
{
    return QVector<EVENT_TYPE>();
}


//*************************************************************************************************************

void MainViewer::onNewModelAvailable(QSharedPointer<AbstractModel> model)
{

}
