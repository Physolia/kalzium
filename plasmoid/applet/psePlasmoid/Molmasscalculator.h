/*
    Mass calculator: Plasmoid to calculate mass of a Molecule.
    SPDX-FileCopyrightText: 2009, 2010 Etienne Rebetez etienne.rebetez@oberwallis.ch

    SPDX-License-Identifier: GPL-2.0-or-later

*/

// Here we avoid loading the header multiple times
#ifndef MOLMASSCALCULATOR_H
#define MOLMASSCALCULATOR_H

#include <QColor>
#include <QGraphicsGridLayout>

// We need the Plasma Applet headers
#include <plasma/widgets/label.h>
#include <plasma/widgets/lineedit.h>
#include <plasma/widgets/iconwidget.h>
#include <plasma/popupapplet.h>

#include "ui_Molmassconfig.h"

#include "Periodictable.h"
#include <QTimer>

// Define our plasma Applet
class Molmasscalculator : public Plasma::PopupApplet
{
    Q_OBJECT
public:
    // Basic Create/Destroy
    Molmasscalculator(QObject *parent, const QVariantList &args);
    ~Molmasscalculator();

    /// Returns the GraphicsWidget used by the plasmoid
    QGraphicsWidget *graphicsWidget();

    /// loads the settings.
    void init();

public slots:
    /// Appends the given String to the lineedit text and let it parse by ParseMolecule.
    void appendElement(QString ElementSymbol);

    void configChanged();

private slots:
    ///Sends the requests to the Dataengine
    void ParseMolecule(QString molecule);

    ///The lineEdit Text is parsed.
    void ParseMolecule();

    void toggleTable();

protected slots:
    void configAccepted();

protected:
    void createConfigurationInterface(KConfigDialog* parent);

private:
    /// Sets the new Mass and Molecule after a calculation.
    void newCalculatedMass();

    ///Resets the size of the plasmoid if the periodsystem is shown or not.
    void managePeriodSystem();

    void saveConfig();

    // Configuration
    bool m_showPeriodicTable;
    bool m_copyToClipboard;
    int  m_tableType;

    // Widgets
    QGraphicsWidget *m_widget;
    PeriodicGrid *m_PeriodWidget;

    QTimer *m_triggerTimer;

    Plasma::LineEdit *m_lineedit;
    Plasma::Label *m_MassLabel;
    Plasma::IconWidget *m_switchButton;

    Plasma::DataEngine::Data m_molecule;

    Ui::periodicConfig m_ui;
};

// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(Molmasscalculator, Molmasscalculator)
#endif // MOLMASSCALCULATOR_H
