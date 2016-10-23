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

namespace ztrace
{
/*! \brief write PPM file using image data */
template <class Image>
class PPMWriter
{
    using ofstream = std::ofstream;

public:
    /*! \brief perform data write to file
     *  \param[in] filename of output file to be written to (truncated if exists)
     *  \param[in] image data to be written to the file
     */
    void operator()(std::string const& filename, Image const& image) const
    {
        ofstream out = openFile(filename);
        writePPM(out, image);
        out.close();
    }

    /* \brief open file stream to output file
     * \param[in] filename of output file
     * \returns filestream to output file
     */
    ofstream openFile(std::string const& filename) const
    {
        ofstream out;
        out.open(filename, std::ofstream::out | std::ofstream::trunc);
        return out;
    }

    /*! \brief perform actual write operations
     *  \param[in] out filestream to output file
     *  \param[in] image data to be written
     *  \returns out filestream
     */
    ofstream& writePPM(std::ofstream& out, Image const& image) const
    {
        out << "P3" << std::endl;
        out << image.width() << std::endl;
        out << image.height() << std::endl;
        out << image.getRange() << std::endl;
        out << image << std::endl;
        return out;
    }
};

} // namespace ztrace
#endif // ZTRACE_PPMWRITER_H
