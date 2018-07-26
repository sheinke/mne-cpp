//=============================================================================================================
/**
* @file     channeldatasettingsview.h
* @author   Lorenz Esch <lesch@mgh.harvard.edu>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     July, 2018
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
* @brief    Declaration of the ChannelDataSettingsView Class.
*
*/

#ifndef CHANNELDATASETTINGSVIEW_H
#define CHANNELDATASETTINGSVIEW_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "../disp_global.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QWidget>


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
// FORWARD DECLARATIONS
//=============================================================================================================

namespace Ui {
    class ChannelDataSettingsViewWidget;
}


//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE DISPLIB
//=============================================================================================================

namespace DISPLIB
{


//*************************************************************************************************************
//=============================================================================================================
// DISPLIB FORWARD DECLARATIONS
//=============================================================================================================


//=============================================================================================================
/**
* DECLARE CLASS ChannelDataSettingsView
*
* @brief The ChannelDataSettingsView class provides a view to select different channel data view dependent settings
*/
class DISPSHARED_EXPORT ChannelDataSettingsView : public QWidget
{
    Q_OBJECT

public:    
    typedef QSharedPointer<ChannelDataSettingsView> SPtr;              /**< Shared pointer type for ChannelDataSettingsView. */
    typedef QSharedPointer<const ChannelDataSettingsView> ConstSPtr;   /**< Const shared pointer type for ChannelDataSettingsView. */

    //=========================================================================================================
    /**
    * Constructs a ChannelDataSettingsView which is a child of parent.
    *
    * @param [in] parent        parent of widget
    */
    ChannelDataSettingsView(QWidget *parent = 0,
                Qt::WindowFlags f = Qt::Widget);

    //=========================================================================================================
    /**
    * Update the selection.
    *
    */
    void init();

    //=========================================================================================================
    /**
    * Sets the values of the zoomFactor and windowSize spin boxes
    *
    * @param [in] zoomFactor    new zoomFactor value
    * @param [in] windowSize    new window size value
    */
    void setViewParameters(double zoomFactor, int windowSize);

    //=========================================================================================================
    /**
    * Get current distance time spacer combo box index.
    *
    * @return the current index of the distance time spacer combo box.
    */
    int getDistanceTimeSpacerIndex();

    //=========================================================================================================
    /**
    * Set current distance time spacer combo box index.
    *
    * @param [in] index     the new index value of the combo box
    */
    void setDistanceTimeSpacerIndex(int index);

    //=========================================================================================================
    /**
    * Set current signal and background colors.
    *
    * @param [in] signalColor       The new signal color.
    * @param [in] backgroundColor   The new background color.
    */
    void setSignalBackgroundColors(const QColor& signalColor, const QColor& backgroundColor);

    //=========================================================================================================
    /**
    * Returns the current signal color.
    *
    * @return The current signal color.
    */
    const QColor& getSignalColor();

    //=========================================================================================================
    /**
    * Returns the current background color.
    *
    * @return The current background color.
    */
    const QColor& getBackgroundColor();

protected:
    //=========================================================================================================
    /**
    * Slot called when time window size changes
    */
    void onTimeWindowChanged(int value);

    //=========================================================================================================
    /**
    * Slot called when zoome changes
    */
    void onZoomChanged(double value);

    //=========================================================================================================
    /**
    * Slot called when time spacer distance changes
    *
    * @param [in] value for time spacer distance.
    */
    void onDistanceTimeSpacerChanged(qint32 value);

    //=========================================================================================================
    /**
    * Slot called when the user changes the signal or background color.
    */
    void onViewColorButtonClicked();

    Ui::ChannelDataSettingsViewWidget* ui;

    QColor  m_colCurrentSignalColor;        /**< Current color of the signal. */
    QColor  m_colCurrentBackgroundColor;    /**< Current color of the background. */

signals:
    //=========================================================================================================
    /**
    * Emit this signal whenever the user changes the window size.
    */
    void timeWindowChanged(int value);

    //=========================================================================================================
    /**
    * Emit this signal whenever the user changes the row height (zoom) of the channels.
    */
    void zoomChanged(double value);

    //=========================================================================================================
    /**
    * Emit this signal whenever the user changed the time spacer distance.
    */
    void distanceTimeSpacerChanged(int value);

    //=========================================================================================================
    /**
    * Emit this signal whenever the user changed the signal color.
    */
    void signalColorChanged(const QColor& signalColor);

    //=========================================================================================================
    /**
    * Emit this signal whenever the user changed the background color.
    */
    void backgroundColorChanged(const QColor& backgroundColor);

};

} // NAMESPACE

#endif // CHANNELDATASETTINGSVIEW_H
