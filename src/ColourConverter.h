/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ZTRACE_COLOURCONVERTER_H
#define ZTRACE_COLOURCONVERTER_H

#include <ostream>
#include <utility>
#include <algorithm>
#include "Types.h"
#include "Vector.h"

namespace ztrace
{
template <Int colourRange>
class ColourConverter
{
public:
    ColourConverter();
    ColourConverter(Real const& gray, Real const& alpha = 1.);
    ColourConverter(Int const& red, Int const& green, Int const& blue,
                    Int const& alpha = colourRange);
    ColourConverter(Real const& red, Real const& green, Real const& blue,
                    Real const& alpha = 1.0);
    ColourConverter(Vector const& colourRepresentation, Real const& alpha = 1.0);

    /*! \brief limit (Int) range of single component to interval [0,colourRange]
     *  \param[inout] colourComponent to be cropped
     */
    void limitRange(Int& colourComponent);
    /*! \brief limit (Int) range of all components to interval [0,colourRange] */
    void limitRange();
    /*! \brief covert vector representation to integer values
     *  \param[in] colourRepresentation vector representing a colour x=>R, y=>G, z=>B
     * values in [0,1]
     */
    void convertToRange(Vector const& colourRepresentation);
    /*! \brief set rgb value via colourRange (Int) values
     *  values/elements > colourRange and < 0 will be cropped!
     *  \param[in] red colour value in [0,colourRange]
     *  \param[in] green colour value in [0,colourRange]
     *  \param[in] blue colour value in [0,colourRange]
     *  \param[in] alpha value in [0,colourRange]
     */
    void setRGB(Int const& red, Int const& green, Int const& blue,
                Int const& alpha = colourRange);
    /*! \brief set rgb value via fractional (Real) values
     *  values/elements > 1 and < 0 will be cropped!
     *  \param[in] red colour value in [0,1]
     *  \param[in] green colour value in [0,1]
     *  \param[in] blue colour value in [0,1]
     *  \param[in] alpha value in [0,1]
     */
    void setRGB(Real const& red, Real const& green, Real const& blue,
                Real const& alpha = 1.0);
    /*! \brief set RGB value using a colour vector representation
     *  \param[in] colourRepresentation vector representing a colour x=>R, y=>G, z=>B
     * values in [0,1]
     *  \param[in] alpha value in [0,1]
     */
    void setRGB(Vector const& colourRepresentation, Real const& alpha = 1.0);
    /*! \brief get RGB-vector representation
     *  \result colourRepresentation vector representing a colour x=>R, y=>G, z=>B values
     * in [0,1]
     */
    Vector const vector() const;

    Int const& red() const { return red_; }
    Int const& green() const { return green_; }
    Int const& blue() const { return blue_; }
    static Int range() { return colourRange; }
    static Int toIndexedColour(Real const& colour);
    static Real toRealColour(Int const& colour);

private:
    Int red_;
    Int green_;
    Int blue_;
    Int alpha_;
};

template <Int colourRange>
std::ostream& operator<<(std::ostream& out, ColourConverter<colourRange> const& colour);
} // namespace ztrace

template <ztrace::Int colourRange>
ztrace::ColourConverter<colourRange>::ColourConverter()
    : red_()
    , green_()
    , blue_()
    , alpha_(){};

template <ztrace::Int colourRange>
ztrace::ColourConverter<colourRange>::ColourConverter(Real const& gray, Real const& alpha)
    : red_(gray)
    , green_(gray)
    , blue_(gray)
    , alpha_(alpha)
{
    Vector temp(gray, gray, gray);
    setRGB(temp, alpha);
}

template <ztrace::Int colourRange>
ztrace::ColourConverter<colourRange>::ColourConverter(Int const& red, Int const& green,
                                                      Int const& blue, Int const& alpha)
    : red_(red)
    , green_(green)
    , blue_(blue)
    , alpha_(alpha)
{
    setRGB(red, green, blue, alpha);
}

template <ztrace::Int colourRange>
ztrace::ColourConverter<colourRange>::ColourConverter(Real const& red, Real const& green,
                                                      Real const& blue, Real const& alpha)
    : red_()
    , green_()
    , blue_()
    , alpha_(alpha)
{
    Vector temp(red, green, blue);
    setRGB(temp, alpha);
}

template <ztrace::Int colourRange>
ztrace::ColourConverter<colourRange>::ColourConverter(Vector const& colourRepresentation,
                                                      Real const& alpha)
    : red_()
    , green_()
    , blue_()
    , alpha_(alpha)
{
    setRGB(colourRepresentation, alpha);
}

template <ztrace::Int colourRange>
void ztrace::ColourConverter<colourRange>::limitRange(Int& colourComponent)
{
    colourComponent = colourComponent < 0 ? 0 : colourComponent;
    colourComponent = colourComponent > colourRange ? colourRange : colourComponent;
}

template <ztrace::Int colourRange>
void ztrace::ColourConverter<colourRange>::limitRange()
{
    limitRange(red_);
    limitRange(green_);
    limitRange(blue_);
    limitRange(alpha_);
}

template <ztrace::Int colourRange>
void ztrace::ColourConverter<colourRange>::convertToRange(Vector const& colourRepresentation)
{
    red_ = toIndexedColour(colourRepresentation.x());
    green_ = toIndexedColour(colourRepresentation.y());
    blue_ = toIndexedColour(colourRepresentation.z());
}

template <ztrace::Int colourRange>
void ztrace::ColourConverter<colourRange>::setRGB(Int const& red, Int const& green,
                                                  Int const& blue, Int const& alpha)
{
    red_ = red;
    green_ = green;
    blue_ = blue;
    alpha_ = alpha;
    limitRange();
}

template <ztrace::Int colourRange>
void ztrace::ColourConverter<colourRange>::setRGB(Real const& red, Real const& green,
                                                  Real const& blue, Real const& alpha)
{
    Vector temp(red, green, blue);
    setRGB(temp, alpha);
}

template <ztrace::Int colourRange>
void ztrace::ColourConverter<colourRange>::setRGB(Vector const& colourRepresentation,
                                                  Real const& alpha)
{
    convertToRange(colourRepresentation);
    alpha_ = toIndexedColour(alpha);
    limitRange();
}

template <ztrace::Int colourRange>
ztrace::Vector const ztrace::ColourConverter<colourRange>::vector() const
{
    return Vector{toRealColour(red_), toRealColour(green_), toRealColour(blue_)};
}

template <ztrace::Int colourRange>
ztrace::Int ztrace::ColourConverter<colourRange>::toIndexedColour(Real const& colour)
{
    return (Int)(colour * (Real)colourRange);
}

template <ztrace::Int colourRange>
ztrace::Real ztrace::ColourConverter<colourRange>::toRealColour(Int const& colour)
{
    return ((Real)colour / (Real)colourRange);
}

template <ztrace::Int colourRange>
std::ostream& ztrace::operator<<(std::ostream& out,
                                 ztrace::ColourConverter<colourRange> const& colour)
{
    out << colour.red() << " ";
    out << colour.green() << " ";
    out << colour.blue();
    return out;
}

#endif // ZTRACE_COLOURCONVERTER_H
