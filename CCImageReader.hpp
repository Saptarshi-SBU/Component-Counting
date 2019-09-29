/*
 * MIT License
 *
 * Copyright (c) 2019 Saptarshi Sen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *
 *  ImageReader Class : Supports loading and writing different image formats.
 *
 */

#ifndef _CCIMAGEREADER_HPP_
#define _CCIMAGEREADER_HPP_

#include "CCDataObject.hpp"

// Image Format
enum class CCImageSourceType {

    PNG, // png format

    BMP, // bitmap format

    JPG, // jpeg

    UNSUPPORTED,
};

   // data source
class CCImageReader : public CCDataObject {

   public:

   CCImageReader(const char *filename, CCImageSourceType type);

   CCImageReader(const char *filename, CCImageSourceType type,
        int width, int height, int num_channels, unsigned char* data);

   CCImageReader(const CCImageReader &srcImg);

   ~CCImageReader();

   int getWidth(void);

   int getHeight(void);

   int getNumChannels(void);

   unsigned char *getDataBlob(void);

   bool Load(void);

   bool Save(void);

   bool Destroy(void);

   private:

   // file name
   const char *filename_;

   // image type
   CCImageSourceType type_;

   // image width pixels
   int width_;

   // image height pixels
   int height_;

   // image channels
   int numChannels_;

   // image blob
   unsigned char *data_;

};

#endif
