//=============================================================================================================
/**
* @file     connectivitysettingsmanager.h
* @author   Lorenz Esch <Lorenz.Esch@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     September, 2018
*
* @section  LICENSE
*
* Copyright (C) 2018, Lorenz Esch and Matti Hamalainen. All rights reserved.
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
* @brief    Definition of the DraggableFramelessWidget Class.
*
*/

#ifndef CONNECTIVITYSETTINGSMANAGER_H
#define CONNECTIVITYSETTINGSMANAGER_H

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include <rtprocessing/rtconnectivity.h>

#include <connectivity/connectivitysettings.h>
#include <connectivity/network/network.h>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QWidget>
#include <QDebug>
#include <QElapsedTimer>


//*************************************************************************************************************
//=============================================================================================================
// Eigen INCLUDES
//=============================================================================================================


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace CONNECTIVITYLIB;
using namespace RTPROCESSINGLIB;


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================


//=============================================================================================================
/**
* DECLARE CLASS ConnectivitySettingsManager
*
* @brief The ConnectivitySettingsManager class provides a manager to handle connectivty computation for the ex_connectivity example.
*/
class ConnectivitySettingsManager : public QObject
{
    Q_OBJECT

public:

    ConnectivitySettingsManager(int iBlockSize, QObject *parent = 0)
    : QObject(parent)
    , m_pRtConnectivity(RtConnectivity::SPtr::create())
    {
        QObject::connect(m_pRtConnectivity.data(), &RtConnectivity::newConnectivityResultAvailable,
                         this, &ConnectivitySettingsManager::onNewConnectivityResultAvailable);

        // Default frequency range
        m_fFreqBandLow = 7.0f;
        m_fFreqBandHigh = 13.0f;
    }

    ConnectivitySettings    m_settings;
    RtConnectivity::SPtr    m_pRtConnectivity;
    QList<Network>          m_networkData;

    float                   m_fFreqBandLow;
    float                   m_fFreqBandHigh;

    QVector<int>            m_indexList;

    QList<ConnectivitySettings::IntermediateTrialData>    m_dataListOriginal;

    void onConnectivityMetricChanged(const QString& sMetric)
    {
        if(m_settings.getConnectivityMethods().contains(sMetric)) {
            return;
        }

        m_pRtConnectivity->restart();

        m_settings.setConnectivityMethods(QStringList() << sMetric);

        m_pRtConnectivity->append(m_settings);
    }

    void onNumberTrialsChanged(int iNumberTrials)
    {
//        QElapsedTimer timer;
//        qint64 iTime = 0;
//        timer.start();

        //The maximum number of trials will always be the number of orginal trials stored
        if(iNumberTrials > m_dataListOriginal.size()) {
            iNumberTrials = m_dataListOriginal.size();
        }

        //Pop data from connectivity settings
        int size = m_settings.size();

        if(size > iNumberTrials) {
            m_settings.removeLast(size-iNumberTrials);
        }

        while(m_settings.size() < iNumberTrials) {
//            bool finish = false;
//            int index = 0;

//            while(!finish) {
//                index = rand() % iNumberTrials;

//                if(!m_indexList.contains(index)) {
//                    m_indexList.append(index);
//                    finish = true;
//                }
//            }

            m_settings.append(m_dataListOriginal.at(m_settings.size()-1));
        }

        //qDebug() << "ConnectivitySettingsManager::onNumberTrialsChanged - m_indexList" << m_indexList;

        m_pRtConnectivity->append(m_settings);

//        iTime = timer.elapsed();
//        qDebug() << "Coherency::computeCoherencyImag timer - Preparation:" << iTime;
//        timer.restart();
    }

    void onFreqBandChanged(float fFreqLow, float fFreqHigh)
    {
        if(m_settings.isEmpty()) {
            return;
        }

        // Convert to frequency bins
        m_fFreqBandLow = fFreqLow;
        m_fFreqBandHigh = fFreqHigh;

        onNewConnectivityResultAvailable(m_networkData, m_settings);
    }

    void onNewConnectivityResultAvailable(const QList<Network>& connectivityResults,
                                          const ConnectivitySettings& connectivitySettings)
    {
        m_settings = connectivitySettings;
        m_networkData = connectivityResults;

        for(int i = 0; i < connectivityResults.size(); ++i) {
            m_networkData[i].setFrequencyRange(m_fFreqBandLow, m_fFreqBandHigh);
            m_networkData[i].normalize();
        }

        if(!m_networkData.isEmpty()) {
            for(int i = 0; i < m_networkData.size(); ++i) {
                emit newConnectivityResultAvailable("sample",
                                                    "Connectivity",
                                                    m_networkData.at(i));
            }
        }
    }

signals:
    void newConnectivityResultAvailable(const QString& sSubject,
                                        const QString& sMeasurement,
                                        const Network& tNetworkData);

};

#endif // CONNECTIVITYSETTINGSMANAGER_H
