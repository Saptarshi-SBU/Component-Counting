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

// selection for desired number of desiredChannels
enum class CCColorChannels {

    DEFAULT,

    GRAY,

    GRAY2,

    RGB,  // digital color-space

    RGBA, // with aplha channel

};

// jpeg loss quality
const static int CCJPEG_LOSS = 100;

// data source
class CCImageReader : public CCDataObject {

   public:

   CCImageReader();

   CCImageReader(const char *filename, CCImageSourceType type);

   CCImageReader(const char *filename, CCImageSourceType type, CCColorChannels color);

   CCImageReader(const CCImageReader &srcImg);

   ~CCImageReader();

   int getWidth(void);

   int getHeight(void);

   int getNumChannels(void);

   int getSize(void);

   CCImageSourceType getFormat(void);

   CCColorChannels getColorChannels(void);

   unsigned char *getDataBlob(void);

   std::string generateUUIDName(void);

   void setFilename(const char *);

   const char *getFilename(void);

   void setWidth(int);

   void setHeight(int);

   void setNumChannels(int);

   void setFormat(CCImageSourceType);

   void setColorChannels(CCColorChannels);

   void setDataBlob(unsigned char *);

   virtual bool Load(void);

   bool Save(void);

   virtual bool Destroy(void);

   CCImageReader ConvertRGB2GRAY(bool &);

   private:

   // file name
   std::string filename_ ;

   // image type
   CCImageSourceType type_;

   // color space
   CCColorChannels desiredChannels_;

   // image width pixels
   int width_;

   // image height pixels
   int height_;

   // image channels
   int numChannels_;

   // image blob
   unsigned char *data_ {nullptr};

};

#endif
