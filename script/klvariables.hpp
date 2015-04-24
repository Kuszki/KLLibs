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

#ifndef KLVARIABLES_HPP
#define KLVARIABLES_HPP

#ifndef size_t
	typedef unsigned int size;
#else
	typedef size_t size;
#endif

class KLVariables
{

	protected: class KLVariable
	{

		protected:

			void* Adress;

			bool Binded;

			size Size;

		public:

			template<typename Mixed> KLVariable(Mixed& Variable);

			KLVariable(void);
			~KLVariable(void);

			template<typename Mixed> void SetValue(Mixed Value);
			template<typename Mixed> Mixed GetValue(void) const;

			template<typename Mixed> void Bind(Mixed& Variable);

			void Unbind(void);

			void Free(void);

	};

	protected:



	public:

		KLVariables();
};

#endif // KLVARIABLES_HPP
