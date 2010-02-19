// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2009, Roboterclub Aachen e.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Roboterclub Aachen e.V. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ROBOTERCLUB AACHEN E.V. ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ROBOTERCLUB AACHEN E.V. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id$
 */
// ----------------------------------------------------------------------------

#ifndef	XPCC__LOCATION_HPP
#define	XPCC__LOCATION_HPP

#include <xpcc/communication/io/iostream.hpp>

#include "angle.hpp"
#include "position.hpp"

namespace xpcc
{
	/**
	 * \brief		Location
	 * 
	 * \ingroup	math
	 */
	template <typename T = int16_t>
	class Location
	{
	public:
		typedef ::xpcc::Position<T> Position;

		Location();

		Location(const Position& position, const Angle& phi);

		Location(const Position& position, float phi);

		Location(const T& x, const T& y, const Angle& phi);

		Location(const T& x, const T& y, float phi);

		/// \brief	Add a position increment
		void
		update(Location& diff);
		
		/// \brief	Add a increment only in x-direction
		void
		update(T x, Angle& phi);
		
		// Attributes
		Position position;
		Angle phi;

		template <typename U>
		friend IOStream&
		operator <<( IOStream&, const Location<U>&);
	};

	/**
	 * \brief	Stream operator to \b xpcc::Location<T>
	 *
	 * \ingroup	math
	 */
	template<typename T>
	IOStream&
	operator<<(IOStream& os, const Location<T>& l);
}

// -----------------------------------------------------------------------------
// IMPLEMENTATION
// -----------------------------------------------------------------------------

template <typename T>
xpcc::Location<T>::Location() :
	position(), phi()
{
}

// -----------------------------------------------------------------------------
template <typename T>
xpcc::Location<T>::Location(const Position& position, const Angle& phi) :
	position(position),
	phi(phi)
{
}

// -----------------------------------------------------------------------------
template <typename T>
xpcc::Location<T>::Location(const Position& position, float phi) :
	position(position),
	phi(phi)
{

}

// -----------------------------------------------------------------------------
template <typename T>
xpcc::Location<T>::Location(const T& x, const T& y, const Angle& phi) :
	position(x, y),
	phi(phi)
{

}

// -----------------------------------------------------------------------------
template <typename T>
xpcc::Location<T>::Location(const T& x, const T& y, float phi) :
	position(x, y),
	phi(phi)
{

}

// -----------------------------------------------------------------------------
template <typename T>
void
xpcc::Location<T>::update(Location<T>& diff)
{
	position += diff.position.rotate(phi);
	phi += diff.phi;
	phi.normalize();
}

// ----------------------------------------------------------------------------

template <typename T>
void
xpcc::Location<T>::update(T x, xpcc::Angle& phi)
{
	position += Position(x * cos(this->phi).toFloat(),
						 x * sin(this->phi).toFloat());

	this->phi += phi;
	this->phi.normalize();
}

// ----------------------------------------------------------------------------

template<class T>
xpcc::IOStream&
xpcc::operator<<(xpcc::IOStream& os, const xpcc::Location<T>& l)
{
	os << "position=( " << l.position << " )";
	os << "\nphi     =" << l.phi;

	return os;
}


#endif	// XPCC__LOCATION_HPP
