/***************************************************************************
 *   Copyright (C) 2006      by Carsten Niehaus,    cniehaus@kde.org       *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/
#ifndef GAMES_H
#define GAMES_H

#include <QWidget>

#include <krandomsequence.h>
#include <kmainwindow.h>
#include <kdialog.h>

class KRandomSequence;

class Stone;

/**
 * @author Carsten Niehaus
 * @brief The playground of a game
 */
class Field
{
	public:
		/**
		 * Constructor
		 *
		 * @param x The x-size of the field
		 * @param y The y-size of the field
		 */
		Field(int x, int y);

		/**
		 * @return the x-size of the field
		 */
		int xSize() const
		{
			return m_size_x;
		}
		
		/**
		 * @return the y-size of the field
		 */
		int ySize() const
		{
			return m_size_y;
		}
		
		/**
		 * Destructor
		 */
		virtual ~Field();

		/**
		 * Add the Stone @p stone to position @p pos
		 */
		virtual void addStone( Stone* stone ) = 0;

		/**
		 * @return the Stone at the position @p pos
		 */
		virtual Stone* stoneAtPosition( const QPoint& pos );

		/**
		 * moves Stone @p stone from the current position to the @p newPosition
		 */
		virtual void moveStoneTo( Stone* stone, const QPoint& newPosition ) = 0;
		
		/**
		 * moves the Stone in @p currentPosition to the @p newPosition
		 */
		virtual void moveStoneTo( const QPoint& currentPosition, const QPoint& newPosition )
		{
			Stone* s = stoneAtPosition( currentPosition );
			moveStoneTo( s, newPosition );
		}
		
		/**
		 * @return the stones of the game
		 */
		virtual QList<Stone*> stones() const
		{
			return m_stones;
		}
		
	protected:
		int m_size_x;
		
		int m_size_y;
		
		QList<Stone*> m_stones;
};


/**
 * @author Carsten Niehaus
 * @brief Baseclass for all games
 */
class Game : public QObject
{
	Q_OBJECT
 	public:
		/**
		 * Destructor
		 */
		virtual ~Game();

		/**
		 * starts the game
		 */
		virtual void startGame();

		virtual void rollDice() = 0;
		
		/**
		 * stops the game
		 */
		virtual void stopGame();

		/**
		 * set the field to @p field
		 */
		void setField( Field* field )
		{
			m_field = field;
		}

		Field* field() const{
			return m_field;
		}

	signals:
		/**
		 * the game has ended
		 */
		void gameOver();

	public slots:
		/**
		 * Start the next draw/roll
		 */
		void slotNextMove();

	protected:
		Field* m_field;

		KRandomSequence random;

		/**
		 * The constructor
		 */
		Game();
};

/**
 * @author Carsten Niehaus
 *
 * Rules of the game: The game starts with an all white board. In each turn, a 
 * Stone on the board will be selected randomly. If the stone is white, it will
 * turn black. This represents the radioactive decay.
 * The game becomes interesting if you alter the number of turns it runs. The 
 * probablilty that a stone swap the colour from white to black decreases with
 * each turn. It is proportional to the halflife period.
 */
class RAGame : public Game
{
	public:
		RAGame();

		void rollDice();
		
		class RAField : public Field
		{
			public:
				/**
				 * Constructor
				 *
				 * @param x The x-size of the field
				 * @param y The y-size of the field
				 */
				RAField(int x, int y);

				/**
				 * moves Stone @p stone from the current position to the @p newPosition
				 */
				virtual void moveStoneTo( Stone* stone, const QPoint& newPosition );
		
				virtual void addStone( Stone* stone );
		};
	private:
		int m_counter;
		int m_number;

	protected:
		RAField* m_field;
};

/**
 * @author Carsten Niehaus
 */
class CrystallizationGame : public Game
{
	public:
		CrystallizationGame();

		void rollDice();
		
		class CrystallizationField : public Field
		{
			public:
				/**
				 * Constructor
				 *
				 * @param x The x-size of the field
				 * @param y The y-size of the field
				 */
				CrystallizationField(int x, int y);

				/**
				 * moves Stone @p stone from the current position to the @p newPosition
				 */
				virtual void moveStoneTo( Stone* stone, const QPoint& newPosition );
		
				virtual void addStone( Stone* stone );
		};
		
		/**
		 * starts the game
		 */
		virtual void startGame();
	private:
		int m_number;

		int neighboursTeam( Stone* stone );
		
		int neighboursNum( Stone* stone );

		QList<Stone*> openentStoneNeighbours( Stone* stone );

		/**
		 * Exange the Stone at the postion @p point with one stone
		 * of the other team. That other Stone has to be in orthogonal
		 * contact with the Stone in @p point
		 */
		void exchangeStones( const QPoint& point );

	protected:
		CrystallizationField* m_field;
};
#endif // GAMES_H
