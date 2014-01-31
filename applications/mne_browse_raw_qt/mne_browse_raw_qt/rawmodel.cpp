//=============================================================================================================
/**
* @file     rawmodel.cpp
* @author   Florian Schlembach <florian.schlembach@tu-ilmenau.de>;
*           Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>;
*           Jens Haueisen <jens.haueisen@tu-ilmenau.de>
* @version  1.0
* @date     January, 2014
*
* @section  LICENSE
*
* Copyright (C) 2014, Florian Schlembach, Christoph Dinh and Matti Hamalainen. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that
* the following conditions are met:
*     * Redistributions of source code must retain the above copyright notice, this list of conditions and the
*       following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
*       the following disclaimer in the documentation and/or other materials provided with the distribution.
*     * Neither the name of the Massachusetts General Hospital nor the names of its contributors may be used
*       to endorse or promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MASSACHUSETTS GENERAL HOSPITAL BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*
* @brief    Implementation of data model of mne_browse_raw_qt
*
*/

#include "rawmodel.h"

//*************************************************************************************************************

RawModel::RawModel(QObject *parent)
: QAbstractTableModel(parent)
, m_qSettings()
, m_bStartReached(false)
, m_bEndReached(false)
{
    m_iWindowSize = m_qSettings.value("RawModel/window_size").toInt();
    n_reloadPos = m_qSettings.value("RawModel/reload_pos").toInt();
    n_maxWindows = m_qSettings.value("RawModel/max_windows").toInt();
}

//*************************************************************************************************************

RawModel::RawModel(QFile &qFile, QObject *parent)
: QAbstractTableModel(parent)
, m_qSettings()
, m_bStartReached(false)
, m_bEndReached(false)
{
    m_iWindowSize = m_qSettings.value("RawModel/window_size").toInt();
    n_reloadPos = m_qSettings.value("RawModel/reload_pos").toInt();
    n_maxWindows = m_qSettings.value("RawModel/max_windows").toInt();

    //read fiff data
    loadFiffData(qFile);
}

//=============================================================================================================
//virtual functions

int RawModel::rowCount(const QModelIndex & /*parent*/) const
{
    if(!m_chInfolist.empty())
        return m_chInfolist.size();
    else return 0;
}

int RawModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 2;
}

//*************************************************************************************************************

QVariant RawModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole && role != Qt::BackgroundRole)
        return QVariant();


    if (index.isValid()) {
        //******** first column (chname) ********
        if(index.column()==0 && role == Qt::DisplayRole)
            return QVariant(m_chInfolist[index.row()].ch_name);

        //******** second column (data plot) ********
        if(index.column()==1) {
            QVariant v;

            switch(role) {
            case Qt::DisplayRole: {
                //form RowVectorPair of pointer and length of RowVector
                QPair<const double*,qint32> rowVectorPair;

                //pack all adjacent (after reload) RowVectorPairs into a QList
                QList<RowVectorPair> listRowVectorPair;

                for(qint16 i=0; i < m_data.size(); ++i) {
                    rowVectorPair.first = m_data[i].data() + index.row()*m_data[i].cols();
                    rowVectorPair.second = m_data[i].cols();

                    listRowVectorPair.append(rowVectorPair);
                }

                v.setValue(listRowVectorPair);
                return v;
                break;
            }
            case Qt::BackgroundRole: {
                if(m_fiffInfo.bads.contains(m_chInfolist[index.row()].ch_name)) {
                    QBrush brush;
                    brush.setStyle(Qt::SolidPattern);
                    qDebug() << m_chInfolist[index.row()].ch_name << "is marked as bad, index:" << index.row();
                    brush.setColor(Qt::red);
                    return QVariant(brush);
                }
                else
                    return QVariant();

                break;
            }
        } // end role switch
    } // end column check

    } // end index.valid() check

    return QVariant();
}

//*************************************************************************************************************

QVariant RawModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole && role != Qt::TextAlignmentRole)
        return QVariant();

    if(orientation == Qt::Horizontal) {
        switch(section) {
        case 0: //chname column
            return QVariant();
        case 1: //data plot column
            return QVariant("data plot");
            switch(role) {
            case Qt::DisplayRole:
                return QVariant("data plot");
            case Qt::TextAlignmentRole:
                return QVariant(Qt::AlignLeft);
            }
        }
    }
    else if(orientation == Qt::Vertical) {
        QModelIndex chname = createIndex(section,0);
        switch(role) {
        case Qt::DisplayRole:
            return QVariant(data(chname).toString());
//        case Qt::TextAlignmentRole:
//            return QVariant();
        }
    }

    return QVariant();
}


//=============================================================================================================
//non-virtual functions

bool RawModel::loadFiffData(QFile &qFile)
{
    beginResetModel();
    clearModel();

    MatrixXd t_data,t_times;

    m_pfiffIO = QSharedPointer<FiffIO>(new FiffIO(qFile));
    if(!m_pfiffIO->m_qlistRaw.empty()) {
        m_iAbsFiffCursor = m_pfiffIO->m_qlistRaw[0]->first_samp; //Set cursor somewhere into fiff file [in samples]
        if(!m_pfiffIO->m_qlistRaw[0]->read_raw_segment(t_data, t_times, m_iAbsFiffCursor, m_iAbsFiffCursor+m_iWindowSize-1))
            return false;
    }
    else {
        qDebug("RawModel: ERROR! Data set does not contain any fiff data!");
        endResetModel();
        return false;
    }

    //set loaded fiff data
    m_data.append(t_data);
    m_times.append(t_times);

    loadFiffInfos();

    endResetModel();
    return true;
}

//*************************************************************************************************************

bool RawModel::writeFiffData(QFile &qFile)
{
    //replace m_fiffInfo with the one contained in the m_pfiffIO object (for replacing bad channesls)
    m_pfiffIO->m_qlistRaw[0]->info = m_fiffInfo;

    m_pfiffIO->write_raw(qFile,0); //ToDo: by now, fiff data file is completely written new -> substitute only FiffInfo in old file?
    qDebug() << "Done saving as fiff file" << qFile.fileName() << "...";
}

//*************************************************************************************************************

void RawModel::clearModel() {
    m_pfiffIO.clear();
    m_data.clear();
    m_times.clear();
    m_chInfolist.clear();

    m_iAbsFiffCursor = 0;
    m_iCurAbsScrollPos = 0;
    m_bStartReached = false;
    m_bEndReached = false;

    qDebug("RawModel cleared.");
}

//*************************************************************************************************************

void RawModel::reloadFiffData(bool before) {
    MatrixXd t_reloaddata,t_reloadtimes;
    qint32 start,end;

    //update scroll position
    if(before) {
        m_iAbsFiffCursor -= m_iWindowSize;
        end = m_iAbsFiffCursor-1;
        start = end-m_iWindowSize+1;

        //check if start of fiff file is reached
        if(start < firstSample()) {
            qDebug() << "RawModel: Start of fiff file reached.";
            m_bStartReached = true;
            m_iAbsFiffCursor += m_iWindowSize;
            return;
        }
    }
    else {
        start = m_iAbsFiffCursor + sizeOfPreloadedData();
        end = start + m_iWindowSize -1;

        //check if end of fiff file is reached
        if(end > lastSample()) {
            qDebug() << "RawModel: End of fiff file reached.";
            m_bEndReached = true;
            return;
        }
    }

    //read data with respect to set start and end point
    if(!m_pfiffIO->m_qlistRaw[0]->read_raw_segment(t_reloaddata, t_reloadtimes, start, end))
        printf("RawModel: Error when reading raw data!");

    //extend m_data with reloaded data
    if(before) {
        m_data.prepend(t_reloaddata);
        m_times.prepend(t_reloadtimes);

        //maintain at maximum n_maxWindows data windows and drop the rest
        if(m_data.size() > n_maxWindows)
            m_data.removeLast();
    }
    else {
        m_data.append(t_reloaddata);
        m_times.append(t_reloadtimes);

        //maintain at maximum n_maxWindows data windows and drop the rest
        if(m_data.size() > n_maxWindows) {
            m_data.removeFirst();
            m_iAbsFiffCursor += m_iWindowSize;
        }
    }

    qDebug() << "RawModel: Fiff data REloaded from " << start << "to" << end;

    QModelIndex topLeft = createIndex(0,1);
    QModelIndex bottomRight = createIndex(m_chInfolist.size(),1);

    emit dataChanged(topLeft,bottomRight);
}

//*************************************************************************************************************

void RawModel::resetPosition(qint32 position) {
    beginResetModel();

    //reset members
    m_data.clear();
    m_times.clear();

    m_bStartReached = false;
    m_bEndReached = false;

    //calculate multiple integer of m_iWindowSize from beginning of Fiff file (rounded down)
    qint32 distance = position - firstSample();
    qint32 mult = floor(distance / m_iWindowSize);

    m_iAbsFiffCursor = firstSample() + mult*m_iWindowSize;

    MatrixXd t_data,t_times;

    if(!m_pfiffIO->m_qlistRaw[0]->read_raw_segment(t_data, t_times, m_iAbsFiffCursor, m_iAbsFiffCursor+m_iWindowSize-1))
        qDebug() << "RawModel: Error resetting position of Fiff file!";

    //append loaded block
    m_data.append(t_data);
    m_times.append(t_times);

    QModelIndex topLeft = createIndex(0,1);
    QModelIndex bottomRight = createIndex(m_chInfolist.size(),1);
    emit dataChanged(topLeft,bottomRight);

    endResetModel();

    qDebug() << "RawModel: Model Position RESET, samples from " << m_iAbsFiffCursor << "to" << m_iAbsFiffCursor+m_iWindowSize-1 << "reloaded.";

}

//*************************************************************************************************************

void RawModel::loadFiffInfos()
{
    //loads chinfos
    for(qint32 i=0; i < m_pfiffIO->m_qlistRaw[0]->info.nchan; ++i)
        m_chInfolist.append(m_pfiffIO->m_qlistRaw[0]->info.chs[i]);

    //loads fiffInfo
    m_fiffInfo = m_pfiffIO->m_qlistRaw[0]->info;
}

//*************************************************************************************************************

double RawModel::maxDataValue(qint16 chan) const {
    double dMax;

    double max = m_data[0].row(0).maxCoeff();
    double min = m_data[0].row(0).minCoeff();

    dMax = (double) (m_chInfolist[chan].range*m_chInfolist[chan].cal)/2;

    return dMax;
}

//=============================================================================================================
//SLOTS

void RawModel::reloadData(int value) {
    m_iCurAbsScrollPos = firstSample()+value;
    qDebug() << "RawModel: absolute Fiff Scroll Cursor" << m_iCurAbsScrollPos << "(m_iAbsFiffCursor" << m_iAbsFiffCursor << ", sizeOfPreloadedData" << sizeOfPreloadedData() << ")";

    //if a scroll position is selected, which is not ym_iFiffCursoret loaded, reset position of model
    if(m_iCurAbsScrollPos > (m_iAbsFiffCursor+sizeOfPreloadedData()) || m_iCurAbsScrollPos < m_iAbsFiffCursor) {
        resetPosition(m_iCurAbsScrollPos);
        return;
    }

    if((m_iCurAbsScrollPos-m_iAbsFiffCursor) < n_reloadPos && !m_bStartReached) {
        qDebug() << "RawModel: Reload requested at FRONT of loaded fiff data.";
        reloadFiffData(1);
    }
    else if(m_iCurAbsScrollPos > m_iAbsFiffCursor+sizeOfPreloadedData()-n_reloadPos && !m_bEndReached) {
        qDebug() << "RawModel: Reload requested at END of loaded fiff data.";
        reloadFiffData(0);
    }
}

//*************************************************************************************************************

void RawModel::markChBad(QModelIndexList selected, bool status)
{
    for(qint8 i=0; i < selected.size(); ++i) {
        if(status) {
            m_fiffInfo.bads.append(m_chInfolist[selected[i].row()].ch_name);
            qDebug() << "RawModel:" << m_chInfolist[selected[i].row()].ch_name << "marked as bad.";
        }
        else {
            if(m_fiffInfo.bads.contains(m_chInfolist[selected[i].row()].ch_name)) {
                int index = m_fiffInfo.bads.indexOf(m_chInfolist[selected[i].row()].ch_name);
                m_fiffInfo.bads.removeAt(index);
                qDebug() << "RawModel:" << m_chInfolist[selected[i].row()].ch_name << "marked as good.";
            }
        }

        emit dataChanged(selected[i],selected[i]);
    }
}
