/***********************************************************************
 *
 * {description}
 * Copyright (C) {year}  {fullname}
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 **********************************************************************/

#include <iostream>
#include <string.h>

#include "containers/klmap.hpp"
#include "containers/kllist.hpp"

using namespace std;

int main(int ArgC, char* Args[])
{
	KLList<int> mapa;

	cout << "args C: " << ArgC << '\n';

	for (int i = 1; i < ArgC; i++)
	{
		float f = strlen(Args[i]);

		cout << "adding " << f << " size: " << mapa.Insert(f) << endl;
	}

	for (const auto& item: mapa)
	{
		cout << item << "\n";
	}

	mapa.Delete(3);

	for (const auto& item: mapa)
	{
		cout << item << "\n";
	}

	return 0;
}
