/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ZTRACE_IMAGE_H
#define ZTRACE_IMAGE_H

#include <vector>
#include <iostream>
#include <algorithm>

#include "Types.h"
#include "Vector.h"
#include "ColourConverter.h"

namespace ztrace
{

template <class Vector, class Colour>
class Image
{
    using ColourVector = std::vector<Vector>;
    using Iterator = typename ColourVector::iterator;
    using IteratorConst = typename ColourVector::const_iterator;

public:
    Image(Size width, Size height)
        : width_(width)
        , height_(height)
        , imageData_(width * height)
    {
    }

    /*! \brief set colour at location/pixel
     *  \param[in] xPos position of pixel
     *  \param[in] yPos position of pixel
     *  \param[in] color to be set at specified position
     */
    void setPixel(Size const& xPos, Size const& yPos, Vector const& color)
    {
        if (xPos < 0 || xPos >= width_)
            throw std::range_error("ztrace::Image::setPixel xPos outside valid range");
        if (yPos < 0 || yPos >= height_)
            throw std::range_error("ztrace::Image::setPixel yPos outside valid range");
        imageData_[yPos * width_ + xPos] = color;
    }

    /*! \brief set colour at location/pixel
     *  \param[in] xPos position of pixel
     *  \param[in] yPos position of pixel
     *  \returns color to get at specified position
     */
    Vector const& getPixel(Size const& xPos, Size const& yPos) const
    {
        if (xPos < 0 || xPos >= width_)
            throw std::range_error("ztrace::Image::getPixel xPos outside valid range");
        if (yPos < 0 || yPos >= height_)
            throw std::range_error("ztrace::Image::getPixel yPos outside valid range");
        return imageData_[yPos * width_ + xPos];
    }

    ColourVector& getRawData() { return imageData_; }
    ColourVector const & getRawData() const { return imageData_; }

    Size const& width() const { return width_; }
    Size const& height() const { return height_; }
    Size size() const { return imageData_.size(); }
    static Int getColourRange() { return Colour::range(); }

    Iterator begin() { return imageData_.begin(); }
    Iterator end() { return imageData_.end(); }
    IteratorConst begin() const { return imageData_.begin(); }
    IteratorConst end() const { return imageData_.end(); }


private:
    Size width_;
    Size height_;
    ColourVector imageData_;
};

template <class Vector, class Colour>
std::ostream& operator<<(std::ostream& out, Image<Vector, Colour> const& image);

} // namespace ztrace

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Implementation
//

template <class Vector, class Colour>
std::ostream& ztrace::operator<<(std::ostream& out,
                                 ztrace::Image<Vector, Colour> const& image)
{
    auto const rawImageData = image.getRawData();
    Size position = 0;
    for (Size y = 0; y < image.height(); ++y) {
        for (Size x = 0; x < image.width(); ++x) {
            out << Colour{rawImageData[position]} << " ";
            ++position;
        }
        out << std::endl;
    }
    return out;
}

#endif // ZTRACE_IMAGE_H
