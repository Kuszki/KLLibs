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

#ifdef QT_VERSION
	#include "kllibs.hpp"
#else
	#define EXPORT
#endif

#include "containers/klmap.hpp"
#include "containers/kllist.hpp"
#include "containers/klstring.hpp"

#ifndef size_t
	typedef unsigned int size;
#else
	typedef size_t size;
#endif

class EXPORT KLVariables
{

	public: enum TYPE
	{
		STRING,
		NUMBER,
		INTEGER
	};

	private: class KLVariable
	{



		protected: union DATA
		{
			KLString* String;

			double* Number;
			int* Integer;
		};

		protected:

			const TYPE Type;

			const bool Binded;

			void* Pointer;

		public:


			KLVariable(const KLVariable& Object);

			KLVariable(KLVariable&& Object);

			KLVariable(TYPE VarType = STRING,
					 void* Bind = nullptr);

			~KLVariable(void);

			KLString ToString(void) const;

			double ToNumber(void) const;

			int ToInt(void) const;

			void Set(const KLString& String);

			void Set(double Number);

			void Set(int Integer);

	};

	protected:

		KLMap<KLVariable, KLString> Variables;

		KLVariables* const UpperScoope;

	public:

		explicit KLVariables(KLVariables* Parent = nullptr);

		KLVariables(const KLVariables& Objects);

		bool Add(const KLString& Name,
			    TYPE Type);

		bool Add(const KLString& Name,
			    KLString& String);

		bool Add(const KLString& Name,
			    double& Number);

		bool Add(const KLString& Name,
			    int& Integer);

		bool Delete(const KLString& Name);

		bool Exists(const KLString& Name) const;

		KLVariable& operator[] (const KLString& Name);

		const KLVariable& operator[] (const KLString& Name) const;

};

#endif // KLVARIABLES_HPP
