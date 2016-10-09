//
// Created by Axel Zschutschke on 10/8/16.
//

#ifndef ZTRACER_COMPONENT_H
#define ZTRACER_COMPONENT_H

#include "Types.h"
#include <ostream>

namespace ztrace {
    template<Int colourRange>
    class Component {
    public:
        template<typename T>
        Component(T const &value) {
            setValue(value);
        }

        void setValue( Real const value) {
            //std::cout << "real value is " << value << std::endl;
            value_ = value;
            limitRange();
        }

        void setValue( int const value) {
            setValue( (Int) value );
        }
        void setValue( Int const value) {
            //std::cout << "int value is " << value << std::endl;
            value_ = (Real) value / (Real) colourRange;
            limitRange();
        }

        void limitRange() {
            value_ = value_ > 1.0 ? 1.0 : value_;
            value_ = value_ < 0.0 ? 0.0 : value_;
        }

        Real getReal() const {
            return value_;
        }

        Int getInt() const {
            return (Int) (value_ * colourRange);
        }

        Component invert() const {
            return Component(1.0 - value_);
        }

        static Int getRange() { return colourRange; }

        friend std::ostream &operator<<(std::ostream &out, Component<colourRange> const &comp) {
            Int result = (Int) (comp.value_ * colourRange);
            out << result;
            return out;
        }

    private:
        Real value_;
    };
}
#endif //ZTRACER_COMPONENT_H
