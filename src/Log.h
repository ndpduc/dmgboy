/*
 This file is part of DMGBoy.
 
 DMGBoy is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 DMGBoy is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with DMGBoy.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __LOGQUEUE_H__
#define __LOGQUEUE_H__

class Registers;

struct ItemLog {
	std::string prefix;
	Registers * regs;
	std::string suffix;
	ItemLog * next;
	ItemLog * prev;
};

//Esta clase se encarga de guardar los valores de los registros hasta un
//número máximo. Cuando se alcanza dicho máximo y antes de insertar uno
//nuevo, se borra el más antiguo. Es decir, tiene el funcionamiento de un cola.
//El más nuevo será el apuntado por last y el más viejo por first.
//	item1 -> item2 -> item3
//	  ^					^
//  first			  last
class QueueLog
{
private:
	int m_maxItems;
	int m_numItems;
	ItemLog * m_first;
	ItemLog * m_last;
    void Empty();
    void DestroyAll();
public:
	QueueLog(int maxItems);
	~QueueLog();
	void Enqueue(std::string prefix, Registers *regs, std::string suffix);
	void Save(std::string path, bool empty=true);
};

#endif
