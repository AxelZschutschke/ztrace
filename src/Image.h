//
// Created by Axel Zschutschke on 10/8/16.
//

#ifndef ZTRACER_IMAGE_H
#define ZTRACER_IMAGE_H

#include <vector>
#include <iostream>
#include "Types.h"

namespace ztrace {

    template<class Colour>
    class Image {
        using ColourVector = std::vector<Colour>;
    public:
        Image(Size width, Size height)
                : width_(width)
                , height_(height)
                , imageData_(width*height)
        { }

        void setPixel( Size const & xPos, Size const & yPos, Colour const & color ) { imageData_[ yPos * width_ + xPos] = color; }

        Colour & getPixel( Size const & xPos, Size const & yPos ){ return imageData_[yPos * width_ + xPos]; }

        ColourVector & getRawData() { return imageData_; }

        Size width() const { return  width_; }
        Size height() const { return  height_; }
        Size size() const { return imageData_.size(); }
        static Int getRange() { return Colour::getRange(); }

        friend std::ostream & operator<<( std::ostream & out, Image const & image ) {
            Size counter = 0;
            for (Size y = 0; y < image.height_; ++y) {
                for (Size x = 0; x < image.width_; ++x) {
                    out << image.imageData_[counter] << " ";
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
