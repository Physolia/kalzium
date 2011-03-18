/***************************************************************************
 *   Copyright (C) 2009 	by Kashyap R Puranik, kashthealien@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "gasCalculator.h"
#include "prefs.h"
#include <kunitconversion/converter.h>


using namespace KUnitConversion;

gasCalculator::gasCalculator(QWidget * parent)
        : QFrame(parent)
{
    ui.setupUi(this);

    /**************************************************************************/
    //                       Gas Calculator set up
    /**************************************************************************/

    // initialise the initially selected values
    init();

    // Connect signals with slots
    connect(ui.temp , SIGNAL(valueChanged(double)),
            this, SLOT(tempChanged()));
    connect(ui.temp_unit , SIGNAL(activated(int)),
            this, SLOT(tempChanged()));
    connect(ui.volume , SIGNAL(valueChanged(double)),
            this, SLOT(volChanged()));
    connect(ui.volume_unit , SIGNAL(activated(int)),
            this, SLOT(volChanged()));
    connect(ui.pressure , SIGNAL(valueChanged(double)),
            this, SLOT(pressureChanged()));
    connect(ui.pressure_unit , SIGNAL(activated(int)),
            this, SLOT(pressureChanged()));
    connect(ui.mass ,  SIGNAL(valueChanged(double)),
            this, SLOT(massChanged()));
    connect(ui.mass_unit , SIGNAL(activated(int)),
            this, SLOT(massChanged()));
    connect(ui.moles , SIGNAL(valueChanged(double)),
            this, SLOT(molesChanged(double)));
    connect(ui.molarMass, SIGNAL(valueChanged(double)),
            this, SLOT(molarMassChanged(double)));
    connect(ui.a , SIGNAL(valueChanged(double)),
            this, SLOT(Vand_aChanged()));
    connect(ui.b ,  SIGNAL(valueChanged(double)),
            this, SLOT(Vand_bChanged()));
    connect(ui.b_unit , SIGNAL(activated(int)),
            this, SLOT(Vand_bChanged()));
    connect(ui.mode, SIGNAL(activated(int)),
    		this, SLOT(setMode(int)));
    connect(ui.reset, SIGNAL(clicked()),
    		this, SLOT(init()));
    /**************************************************************************/
    // gas Calculator setup complete
    /**************************************************************************/
}

gasCalculator:: ~gasCalculator()
{

}

void gasCalculator::init()
{
	error(RESET_GAS_MESSAGE);
	
	ui.molarMass-> setValue(2.008);
    ui.temp     -> setValue(273.0);
    ui.volume   -> setValue(22.400);
    ui.pressure -> setValue(1.0);
    ui.a        -> setValue(0.0);
    ui.b        -> setValue(0.0);
    ui.mass     -> setValue(2.016);
    ui.moles    -> setValue(1.0);
    
    QList<int> units;
    units << Gram << Milligram << Kilogram << Ton;
    poulateUnitCombobox( ui.mass_unit, units );

    units.clear();
    units << Atmosphere << Pascal << Bar << Millibar << Torr;
    poulateUnitCombobox( ui.pressure_unit, units );

    units.clear();
    units << Kelvin << Celsius << Fahrenheit;
    poulateUnitCombobox( ui.temp_unit, units );

    units.clear();
    units << Liter << Milliliter << CubicMeter << KUnitConversion::GallonUS;
    poulateUnitCombobox( ui.volume_unit, units );

    units.clear();
    units << Liter << Milliliter << CubicMeter << KUnitConversion::GallonUS;
    poulateUnitCombobox( ui.b_unit, units );
    // Setup of the UI done

    // Initialise values
    m_temp = Value(273.0, KUnitConversion::Kelvin);
    m_molarMass = 2.016;
    m_pressure = Value(1.0, KUnitConversion::Atmosphere);
    m_mass = Value(2.016, KUnitConversion::Gram);
    m_moles = 1.0;
    m_Vand_a = 0.0;
    m_Vand_b = Value(0.0, KUnitConversion::Liter);
    m_vol = Value(22.4, KUnitConversion::Liter);
    // Initialisation of values done
    
    if (Prefs::ideal())
    {
    	ui.non_ideal->hide();
    }
    
    setMode(3);
}

void gasCalculator::poulateUnitCombobox(QComboBox *comboBox, const QList< int > &unitList)
{
    foreach( int unit, unitList) {
       comboBox->addItem( KUnitConversion::Converter().unit(unit).data()->description(), unit);
    }
}

int gasCalculator::getCurrentUnitId(QComboBox* comboBox)
{
    return comboBox->itemData( comboBox->currentIndex() ).toInt();
}


/*
    Note:-

    Van der Val's gas equation
    ( P + n^2 a / V^2) ( V - nb ) = nRT

    where P - pressure
          V - Volume
          n - number of moles
          R - Universal gas constant
          T - temperature

          a,b - Van der Val's constants
*/

// Calculates the Pressure
void gasCalculator::calculatePressure()
{
    double pressure;
    double volume = m_vol.convertTo( KUnitConversion::Liter ).number();
    double temp = m_temp.convertTo( KUnitConversion::Kelvin ).number();
    double b = m_Vand_b.convertTo( KUnitConversion::Liter ).number();

    pressure = m_moles * R * temp / (volume - m_moles * b) - m_moles * m_moles * m_Vand_a / volume / volume;
    m_pressure = Value(pressure, KUnitConversion::Atmosphere );
    m_pressure = m_pressure.convertTo(getCurrentUnitId(ui.pressure_unit));
    ui.pressure->setValue(m_pressure.number());

    //pressure =
}

// Calculates the molar mass of the gas
void gasCalculator::calculateMolarMass()
{
    double mass = m_mass.convertTo(KUnitConversion::Gram).number();
    double volume = m_vol.convertTo(KUnitConversion::Liter).number();
    double pressure = m_pressure.convertTo(KUnitConversion::Atmosphere).number();
    double temp = m_temp.convertTo(KUnitConversion::Kelvin).number();
    double b = m_Vand_b.convertTo(KUnitConversion::Liter).number();

    m_molarMass = mass * R * temp / (pressure + m_moles * m_moles * m_Vand_a / volume / volume)\
                  / (volume - m_moles * b);
    ui.molarMass->setValue(m_molarMass);
}

// Calculates the Volume
void gasCalculator::calculateVol()
{
    double volume;
    double pressure = m_pressure.convertTo(KUnitConversion::Atmosphere).number();
    double temp = m_temp.convertTo(KUnitConversion::Kelvin).number();
    double b = m_Vand_b.convertTo(KUnitConversion::Liter).number();

    volume = m_moles * R * temp / pressure + (m_moles * b);
    m_vol = Value(volume, KUnitConversion::Liter);
    m_vol = m_vol.convertTo( getCurrentUnitId(ui.volume_unit) );
    ui.volume->setValue(m_vol.number());
}

// Calculates the Temperature
void gasCalculator::calculateTemp()
{
    double temp;
    double volume = m_vol.convertTo(KUnitConversion::Liter).number();
    double pressure = m_pressure.convertTo(KUnitConversion::Atmosphere).number();
    double b = m_Vand_b.convertTo(KUnitConversion::Liter).number();

    temp = (pressure + (m_moles * m_moles * m_Vand_a / volume / volume))\
           * (volume - m_moles * b) / m_moles / R;
    m_temp = Value(temp, KUnitConversion::Kelvin);
    m_temp = m_temp.convertTo( getCurrentUnitId( ui.temp_unit ) );
    ui.temp->setValue(m_temp.number());
}

// Calculates the number of moles
void gasCalculator::calculateMoles()
{
    double volume = m_vol.convertTo(KUnitConversion::Liter).number();
    double pressure = m_pressure.convertTo(KUnitConversion::Atmosphere).number();
    double temp = m_temp.convertTo(KUnitConversion::Kelvin).number();
    double b = m_Vand_b.convertTo(KUnitConversion::Liter).number();

    m_moles = (pressure + m_moles * m_moles * m_Vand_a / volume / volume)\
              * (volume - m_moles * b) / R / temp;
    ui.moles->setValue(m_moles);
}

// Calculates the mass of substance
void gasCalculator::calculateMass()
{
    double mass;
    double volume = m_vol.convertTo(KUnitConversion::Liter).number();
    double pressure = m_pressure.convertTo(KUnitConversion::Atmosphere).number();
    double temp = m_temp.convertTo(KUnitConversion::Kelvin).number();
    double b = m_Vand_b.convertTo(KUnitConversion::Liter).number();

    mass = (pressure + m_moles * m_moles * m_Vand_a / volume / volume)\
           * (volume - m_moles * b) * m_molarMass / R / temp;
    m_mass = Value(mass, KUnitConversion::Gram);
    m_mass = m_mass.convertTo( getCurrentUnitId( ui.mass_unit ) );
    ui.mass->setValue(m_mass.number());
}


// Functions ( slots ) that occur on changing a value
// occurs when the volume is changed
void gasCalculator::volChanged()
{
    m_vol = Value(ui.volume->value(), getCurrentUnitId( ui.volume_unit ) );
    calculate();
}

// occurs when the temperature is changed
void gasCalculator::tempChanged()
{
    m_temp = Value(ui.temp->value(), getCurrentUnitId( ui.temp_unit ) );
    calculate();
}

// occurs when the pressure is changed
void gasCalculator::pressureChanged()
{
    m_pressure = Value(ui.pressure->value(), getCurrentUnitId( ui.pressure_unit ) );
    calculate();
}

// occurs when the mass is changed
void gasCalculator::massChanged()
{
    m_mass = Value(ui.mass->value(), getCurrentUnitId( ui.mass_unit ) );
    m_moles = m_mass.convertTo(KUnitConversion::Gram).number() / m_molarMass;
    ui.moles->setValue(m_moles);
    calculate();
}

// occurs when the number of moles is changed
void gasCalculator::molesChanged(double value)
{
    m_moles = value;
    m_mass = Value(m_moles * m_molarMass, KUnitConversion::Gram);
    m_mass = m_mass.convertTo( getCurrentUnitId( ui.mass_unit ) );
    ui.mass->setValue(m_mass.number());
    calculate();
}

// occurs when the molar mass is changed
void gasCalculator::molarMassChanged(double value)
{
	if (value == 0.0) {
		error(MOLAR_MASS_ZERO_);
		return;
	}
    m_molarMass = value;
    m_mass = Value(m_molarMass * m_moles, KUnitConversion::Gram);
    m_mass = m_mass.convertTo( getCurrentUnitId( ui.mass_unit ) );
    ui.mass->setValue(m_mass.number());
    calculate();
}

// occurs when the number of moles is changed
void gasCalculator::Vand_aChanged()
{
    m_Vand_a = ui.a->value();
    calculate();
}

// occurs when the number of moles is changed
void gasCalculator::Vand_bChanged()
{
    m_Vand_b = Value(ui.b->value(), getCurrentUnitId( ui.b_unit ) );
    calculate();
}

// occurs when the mode is changed, eg pressure to volume
void gasCalculator::setMode(int mode)
{
	m_mode = mode;
	
	ui.moles->setReadOnly(false);
	ui.mass->setReadOnly(false);
	ui.pressure->setReadOnly(false);
	ui.temp->setReadOnly(false);
	ui.volume->setReadOnly(false);
	
	switch (mode)
	{
		case MOLES:
			ui.moles->setReadOnly(true);
			ui.mass->setReadOnly(true);
			break;
		case PRESSURE:
			ui.pressure->setReadOnly(true);
			break;		
		case TEMPERATURE:
			ui.temp->setReadOnly(true);
			break;		
		case VOLUME:
			ui.volume->setReadOnly(true);
			break;
	}
	
	calculate();
}

// occurs when any quantity is changed
void gasCalculator::calculate()
{
	error(RESET_GAS_MESSAGE);
		
    switch(m_mode)
    {
	case MOLES:
        calculateMoles();
        break;
	case PRESSURE:
        calculatePressure();
        break;        
    case TEMPERATURE:
        calculateTemp();
        break;        
	case VOLUME:
        calculateVol();    
        break;        
    }
}

void gasCalculator::error(int mode)
{
    switch (mode) { // Depending on the mode, set the error messages.
	case RESET_GAS_MESSAGE:
		ui.error->setText("");
		break;
    case VOL_ZERO :
        ui.error->setText(i18n("Volume cannot be zero, please enter a valid value."));
        break;
    case MOLAR_MASS_ZERO_:
    	ui.error->setText(i18n("Molar mass cannot be zero, please enter a non-zero value."));
    default:
        break;
    }
}

#include "gasCalculator.moc"


