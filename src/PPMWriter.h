//
// Created by Axel Zschutschke on 10/8/16.
//

#ifndef ZTRACER_PPMWRITER_H
#define ZTRACER_PPMWRITER_H

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
#endif //ZTRACER_PPMWRITER_H