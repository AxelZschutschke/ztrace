//
// Created by Axel Zschutschke on 10/8/16.
//

#ifndef ZTRACER_COLOUR_H
#define ZTRACER_COLOUR_H

#include <ostream>
#include "Types.h"
#include "Component.h"

namespace ztrace
{
    template<Int colourRange>
    class Colour;

    template<Int colourRange>
    std::ostream & operator<<( std::ostream &, Colour<colourRange> const & );

    template<Int colourRange>
    class Colour {
    public:
        Colour( );

        template<typename T>
        void setRGB( T const & red, T const & green, T const & blue );

        friend std::ostream & operator<< <colourRange>( std::ostream &, Colour<colourRange> const & );

        Component<colourRange> redValue() const { return red_; }
        Component<colourRange> greenValue() const { return green_; }
        Component<colourRange> blueValue() const { return blue_; }

        static Int getRange() { return colourRange; }

    private:
        Component<colourRange> red_;
        Component<colourRange> green_;
        Component<colourRange> blue_;
    };
}

using Int = ztrace::Int;
using Real = ztrace::Real;

template<Int colourRange>
ztrace::Colour<colourRange>::Colour( )
        : red_( 0 )
        , green_( 0 )
        , blue_( 0 )
{}

template<Int colourRange>
template<typename T>
void ztrace::Colour<colourRange>::setRGB(
          T const &red
        , T const &green
        , T const &blue)
{
    red_.setValue( red );
    green_.setValue( green );
    blue_.setValue( blue );
}

template<Int colourRange>
std::ostream & ztrace::operator<<( std::ostream & out, ztrace::Colour<colourRange> const & colour) {
    out << colour.red_.getInt() << " ";
    out << colour.green_.getInt() << " ";
    out << colour.blue_.getInt();
    return out;
}
#endif //ZTRACER_COLOUR_H
