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
        using ColourVector = std::vector<Int>;

    public:
        ColourConverter()
            : colourRepresentation_(4)
        {};
        ColourConverter( Int const & red, Int const & green, Int const & blue, Int alpha = colourRange )
            : colourRepresentation_(4)
        {
            setRGB( red, green, blue, alpha );
        }
        ColourConverter( Real const & red, Real const & green, Real const & blue, Real alpha = 1.0 )
            : colourRepresentation_(4)
        {
            Vector temp( red, green, blue );
            setRGB( temp, alpha );
        }
        ColourConverter( Vector const & colourRepresentation, Real const & alpha = 1.0 )
            : colourRepresentation_(4)
        {
            setRGB( colourRepresentation, alpha );
        }


        void limitRange( ){
            std::transform( colourRepresentation_.begin(), colourRepresentation_.end(), colourRepresentation_.begin(), []( Int const & value ){ return value < 0 ? 0 : value > colourRange ? colourRange : value; });
        }
        void convertToRange( Vector const & colourRepresentation ){
            std::transform( colourRepresentation.begin(), colourRepresentation.end(), colourRepresentation_.begin(), []( Real const & value ){ return toIndexedColour( value ); });
        }
        void setRGB( Int const & red, Int const & green, Int const & blue, Int alpha = colourRange )
        {
            colourRepresentation_[0] = red;
            colourRepresentation_[1] = green;
            colourRepresentation_[2] = blue;
            colourRepresentation_[3] = alpha;
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
            colourRepresentation_[3] = toIndexedColour( alpha );
            limitRange( );
        }
        Vector const vector( ) const { 
            return Vector{ toRealColour( colourRepresentation_[0]), toRealColour( colourRepresentation_[1] ), toRealColour( colourRepresentation_[2] ) };
        }

        Int const & red() const { return colourRepresentation_[0]; }
        Int const & green() const { return colourRepresentation_[1]; }
        Int const & blue() const { return colourRepresentation_[2]; }
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
        ColourVector colourRepresentation_;
    };

}


#endif //ZTRACER_COLOURCONVERTER_H
