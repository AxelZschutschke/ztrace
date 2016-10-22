/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// (c) 2016, Axel Zschutschke
////
//// This software is provided under the terms of the BSD-3 license, see LICENSE.md
////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ZTRACE_PPMWRITER_H
#define ZTRACE_PPMWRITER_H

#include <string>
#include <fstream>
#include "Types.h"

namespace ztrace {
    template<class Image>
    class PPMWriter {
        using ofstream = std::ofstream;
    public:
        ofstream openFile(std::string const &filename) const {
            ofstream out;
            out.open(filename, std::ofstream::out | std::ofstream::trunc);
            return out;
        }

        ofstream &writePPM(std::ofstream &out, Image const &image) const {
            out << "P3" << std::endl;
            out << image.width() << std::endl;
            out << image.height() << std::endl;
            out << image.getRange() << std::endl;
            out << image << std::endl;
            return out;
        }
        void operator()( std::string const & filename, Image const & image ) const {
            ofstream out = openFile( filename );
            writePPM( out, image );
            out.close();
        }
    };
}
#endif //ZTRACE_PPMWRITER_H
