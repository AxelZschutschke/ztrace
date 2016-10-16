//
// Created by Axel Zschutschke on 10/8/16.
//

#ifndef ZTRACER_COLOURCONVERTER_H
#define ZTRACER_COLOURCONVERTER_H

#include <ostream>
#include <utility>
#include <algorithm>
#include "Types.h"
#include "Vector.h"

namespace ztrace
{
    template<Int colourRange>
    class ColourConverter
    {
    public:
        ColourConverter()
            : red_()
            , green_()
            , blue_()
            , alpha_()
        {};
        ColourConverter( Real const & gray, Real const & alpha = 1. )
            : red_( gray )
            , green_( gray )
            , blue_( gray )
            , alpha_( alpha )
        {
            Vector temp( gray, gray, gray );
            setRGB( temp, alpha );
        }
        ColourConverter( Int const & red, Int const & green, Int const & blue, Int alpha = colourRange )
            : red_( red )
            , green_( green )
            , blue_( blue )
            , alpha_( alpha )
        {
            setRGB( red, green, blue, alpha );
        }
        ColourConverter( Real const & red, Real const & green, Real const & blue, Real alpha = 1.0 )
            : red_( )
            , green_( )
            , blue_( )
            , alpha_( alpha )
        {
            Vector temp( red, green, blue );
            setRGB( temp, alpha );
        }
        ColourConverter( Vector const & colourRepresentation, Real const & alpha = 1.0 )
            : red_( )
            , green_( )
            , blue_( )
            , alpha_( alpha )
        {
            setRGB( colourRepresentation, alpha );
        }

        void limitRange( Int & colourComponent ){
            colourComponent = colourComponent < 0 ? 0 : colourComponent;
            colourComponent = colourComponent > colourRange ? colourRange : colourComponent;
        }


        void limitRange( ){
            limitRange( red_ );
            limitRange( green_ );
            limitRange( blue_ );
            limitRange( alpha_ );
        }
        void convertToRange( Vector const & colourRepresentation ){
            red_ = toIndexedColour( colourRepresentation.x() );
            green_ = toIndexedColour( colourRepresentation.y() );
            blue_ = toIndexedColour( colourRepresentation.z() );
        }
        void setRGB( Int const & red, Int const & green, Int const & blue, Int alpha = colourRange )
        {
            red_ = red;
            green_ = green;
            blue_ = blue;
            alpha_ = alpha;
            limitRange();
        }
        void setRGB( Real const & red, Real const & green, Real const & blue, Int alpha = colourRange )
        {
            Vector temp( red, green, blue );
            setRGB( temp, alpha );
        }
        void setRGB( Vector const & colourRepresentation, Real const & alpha = 1.0 )
        {
            convertToRange( colourRepresentation );
            alpha_ = toIndexedColour( alpha );
            limitRange( );
        }
        Vector const vector( ) const { 
            return Vector{ toRealColour( red_), toRealColour( green_ ), toRealColour( blue_ ) };
        }

        Int const & red() const { return red_; }
        Int const & green() const { return green_; }
        Int const & blue() const { return blue_; }
        static Int range(){ return colourRange; }
        static Int toIndexedColour( Real const & colour ) { return (Int) (colour * (Real) colourRange); }
        static Real toRealColour( Int const & colour ) { return ((Real) colour / (Real) colourRange); }

        friend std::ostream & operator<<( std::ostream & out, ColourConverter const & colour ){
            out << colour.red() << " ";
            out << colour.green() << " ";
            out << colour.blue();
            return out;
        }
    private:
        Int red_;
        Int green_;
        Int blue_;
        Int alpha_;
    };

}


#endif //ZTRACER_COLOURCONVERTER_H
