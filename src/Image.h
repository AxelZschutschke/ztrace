//
// Created by Axel Zschutschke on 10/8/16.
//

#ifndef ZTRACER_IMAGE_H
#define ZTRACER_IMAGE_H

#include <vector>
#include <iostream>
#include <algorithm>
#include "Types.h"
#include "Vector.h"
#include "ColourConverter.h"

namespace ztrace {

    template<class Vector,class Colour>
    class Image {
    public:
        using ColourVector = std::vector<Vector>;
        using Iterator = typename ColourVector::iterator;
        using IteratorConst = typename ColourVector::const_iterator;
    public:
        Image(Size width, Size height)
                : width_(width)
                , height_(height)
                , imageData_(width*height)
        { }

        void setPixel( Size const & xPos, Size const & yPos, Vector const & color ) { imageData_[ yPos * width_ + xPos] = color; }

        Vector & getPixel( Size const & xPos, Size const & yPos ){ return imageData_[yPos * width_ + xPos]; }

        ColourVector & getRawData() { return imageData_; }

        Size width() const { return  width_; }
        Size height() const { return  height_; }
        Size size() const { return imageData_.size(); }
        static Int getRange() { return Colour::range(); }

        Iterator begin() { return imageData_.begin(); }
        Iterator end() { return imageData_.end(); }
        IteratorConst begin() const { return imageData_.begin(); }
        IteratorConst end() const { return imageData_.end(); }

        friend std::ostream & operator<<( std::ostream & out, Image const & image ) {
            Size counter = 0;
            for (Size y = 0; y < image.height_; ++y) {
                for (Size x = 0; x < image.width_; ++x) {
                    out << Colour{ image.imageData_[counter] } << " ";
                    ++counter;
                }
                out << std::endl;
            }
            return out;
        }

    private:
        Size width_;
        Size height_;
        ColourVector imageData_;
    };
}

#endif //ZTRACER_IMAGE_H
